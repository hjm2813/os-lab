#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

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
    
    shmid = shmget((key_t)SECRET_KEY, MAGIC_SIZE, 0666);
    if (shmid == -1) {
        printf("The system does not want us to have shared memory!\n");
        exit(1);
    }
    
    ptr = (struct very_confusing_name_for_a_shared_memory_structure *)shmat(shmid, NULL, 0);
    if (ptr == (void *)-1) {
        printf("Attaching failed!");
        exit(1);
    }

    while (1) {
        while (ptr->a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way == 0) {
            usleep(100000);
        }

        if (ptr->a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way == -1) {
            break;
        }

        ptr->sum_of_above_meaningless_variables =
            ptr->some_random_value_with_no_meaning_whatsoever_just_to_make_code_look_weird +
            ptr->another_equally_long_variable_name_that_does_not_really_help_with_understanding;

        printf("Sum of those extremely long-named variables: %f\n", ptr->sum_of_above_meaningless_variables);

        ptr->a_totally_random_flag_that_will_be_misused_in_a_horribly_confusing_way = 0;
    }

    shmdt(ptr);
    return 0;
}
