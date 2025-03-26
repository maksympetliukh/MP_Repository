#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

typedef enum {RUN, STOP, PAUSE, CLOCK} timer_state;

timer_state state = STOP;

int time_seconds = 0;

sem_t timer_semaphore;
sem_t input_semaphore;

void* timer_thread(void* arg)
{
    while(1)
    {
        sem_wait(&timer_semaphore);
        if(state == RUN)
        {
            sleep(1);
            time_seconds++;
            printf("\r%02d:%02d", time_seconds / 60, time_seconds % 60);
            fflush(stdout);
        }
        sem_post(&timer_semaphore);
    }
    return NULL;
}

void* input_thread(void* arg)
{
    char input;
    while(1)
    {
        sem_wait(&input_semaphore);
        input = getchar();
        switch(input)
        {
            case 'r':
            if (state == STOP)
            {
                state = RUN;
                time_seconds = 0; 
                printf("\nTimer started\n");
            }
            else if(state == PAUSE)
            {
                state = RUN;
                printf("\nTimer started again\n");
            }
            break;
            case 'p':
            if(state == RUN)
            {
                state = PAUSE;
                printf("\nTimer paused\n");
            }
            break;
            case 'b':
            if(state == RUN || state == PAUSE)
            {
            state = STOP;
            time_seconds = 0;
            printf("\nTimer stopped and reset\n");
            }
            break;
            case 'c':
            if (state == RUN || state == PAUSE || state == STOP)
            {
                state = CLOCK;
                printf("\nCurrent time:\n");
            }
            else if(state == CLOCK)
            {
                state = RUN;
                time_seconds = 0;
                printf("\nBack to timer\n");
            }
            break;
            case 'q':
            printf("\nExit the timer...\n");
            exit(0);
            break;
            default:
            break;
        }
        sem_post(&input_semaphore);
    }
    return NULL;
}

void* clock_thread(void* arg)
{
    while(1)
    {
        if(state == CLOCK)
        {

    sem_wait(&input_semaphore);
    time_t time_now;
    struct tm *new_time;
    time(&time_now);
    new_time = localtime(&time_now);
    printf("\rCurrent time: %02d:%02d:%02d", new_time -> tm_hour, new_time -> tm_min, new_time -> tm_sec);
    fflush(stdout);
}
sem_post(&input_semaphore);
sleep(1);
}
return NULL;
}

int main(void)
{
    pthread_t timer_t1, input_t1, clock_t1;

    sem_init(&timer_semaphore, 0, 1);
    sem_init(&input_semaphore, 0, 1);

    pthread_create(&timer_t1, NULL, timer_thread, NULL);
    pthread_create(&input_t1, NULL, input_thread, NULL);
    pthread_create(&clock_t1, NULL, clock_thread, NULL);

    printf("Press 'r' to start the timer\n");

    while(1)
    {
        sleep(1);
    }

    sem_destroy(&timer_semaphore);
    sem_destroy(&timer_semaphore);

    return 0;
}