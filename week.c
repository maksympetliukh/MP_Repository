#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

typedef enum {MON, TUE, WED, THU, FRI, SAT} week;

sem_t sem;

void* week_thread(void* arg) {
    while(1) {
        char input;
        sem_wait(&sem);
        input = getchar();
        switch (input) {
            case '1':
                printf("Your day off is Monday\n");
            break;
            case '2':
                printf("Your day off is Tuesday\n");
            break;
            case '3':
                printf("Your day off is Wednesday\n");
            break;
            case '4':
                printf("Your day off is Thursday\n");
            break;
            case '5':
                printf("Your day off is Friday\n");
            break;
            case '6':
                printf("Your day off is Saturday\n");
            break;
            case '7':
                printf("Your day off is Sunday\n");
            break;
            case '8':
                printf("Finish selection\n");
            exit(0);
            break;
            default:
                break;
        }
        sem_post(&sem);
    }
    return NULL;
}

int main() {
    pthread_t day_off;
    sem_init(&sem, 0, 1);
    pthread_create(&day_off, NULL, week_thread, NULL);

    printf("Select your day off:\n");

    pthread_join(day_off, NULL);
    sem_destroy(&sem);

    return 0;
}
