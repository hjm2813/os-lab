#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

#define SECRET_KEY 1234
#define MAGIC_SIZE sizeof(struct very_confusing_name_for_a_shared_memory_structure)

struct very_confusing_name_for_a_shared_memory_structure {
    float some_random_value_with_no_meaning_whatsoever_just_to_make_code_look_weird;
    float another_equally_long_variable_name_that_does_not_really_help_with_understanding;
    float sum_of_above_meaningless_variables;
    int a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way;
};

int main() {
    int shmid;
    struct very_confusing_name_for_a_shared_memory_structure *ptr;
    
    shmid = shmget((key_t)SECRET_KEY, MAGIC_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        printf(" The system just refused to give us a shared memory block!\n");
        exit(1);
    }
    
    ptr = (struct very_confusing_name_for_a_shared_memory_structure *)shmat(shmid, NULL, 0);
    if (ptr == (void *)-1) {
        printf("Attaching to shared memory failed! Maybe the ghosts of bad coding practices haunt us!\n");
        exit(1);
    }
    
    while (1) {
        printf("Enter two random floating numbers: ");
        scanf("%f %f", &ptr->some_random_value_with_no_meaning_whatsoever_just_to_make_code_look_weird,
                        &ptr->another_equally_long_variable_name_that_does_not_really_help_with_understanding);

        ptr->a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way = 1;

        if (ptr->some_random_value_with_no_meaning_whatsoever_just_to_make_code_look_weird < -0.5) {
            ptr->a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way = -1;
            break;
        }

        while (ptr->a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way == 1) {
            usleep(100000);
        }
    }
    
    shmdt(ptr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
