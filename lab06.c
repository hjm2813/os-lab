#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

#define DEFAULT_INTERVALS 100000
#define DEFAULT_THREADS 4

typedef struct {
    int start, end;
    double step;
    double sum;
} ThreadData;

void *compute_integral(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    double x, local_sum = 0.0;
    for (int i = data->start; i < data->end; i++) {
        x = (i + 0.5) * data->step;
        local_sum += 4.0 / (1.0 + x * x);
    }
    data->sum = local_sum;
    return NULL;
}

int main(int argc, char *argv[]) {
    int intervals = (argc > 1) ? atoi(argv[1]) : DEFAULT_INTERVALS;
    int num_threads = (argc > 2) ? atoi(argv[2]) : DEFAULT_THREADS;
    if (intervals < num_threads) {
        printf("Intervals must be >= number of threads!\n");
        return -1;
    }

    struct timeval start, end;
    double step = 1.0 / (double)intervals;
    double serial_sum = 0.0, parallel_sum = 0.0;
    gettimeofday(&start, NULL);
    for (int i = 0; i < intervals; i++) {
        double x = (i + 0.5) * step;
        serial_sum += 4.0 / (1.0 + x * x);
    }
    serial_sum *= step;
    gettimeofday(&end, NULL);
    double serial_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    gettimeofday(&start, NULL);
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int chunk_size = intervals / num_threads;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? intervals : (i + 1) * chunk_size;
        thread_data[i].step = step;
        pthread_create(&threads[i], NULL, compute_integral, &thread_data[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        parallel_sum += thread_data[i].sum;
    }
    parallel_sum *= step;
    gettimeofday(&end, NULL);
    double parallel_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    printf("Serial Result: %.10f, Time: %f sec\n", serial_sum, serial_time);
    printf("Parallel Result (%d threads): %.10f, Time: %f sec\n", num_threads, parallel_sum, parallel_time);

    return 0;
}
