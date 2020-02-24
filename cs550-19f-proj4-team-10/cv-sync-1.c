/* On-demand 2-thread synchronization */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Compile with "gcc cv-sync-1.c -o cv-sync-1 -lpthread"

pthread_mutex_t m;
pthread_cond_t c;
int counter = 0;
int done = 1;
int a=0, b=0, d=0;
int thread1cnt= 0,thread2cnt = 0;

void usage(char *bin_str)
{
    printf("Usage: %s total_runs t1_runs t2_runs\n", bin_str);
}

void *printThread(void *n)
{
    int theard1 = 0;
    int theard2 = 0;
    while (counter < a && (b > 0 || d > 0))
    {
        // acquire a lock
        pthread_mutex_lock(&m);

        if (done != (int)*(int *)n && counter < a)
        {
            if ((int)*(int *)n == 1)
            {
                pthread_cond_wait(&c, &m);
            }
            else if ((int)*(int *)n == 2 )
            {
                pthread_cond_wait(&c, &m);
            }
        }
        
        if (done == 1 && b != 0)
        {
            counter++;
            theard1++;
            if (counter <= a)
            {
                printf("%d : IN T%d, TID=%u\n", counter, done, pthread_self());
                thread1cnt++;
            }
            if(counter >= a)
            {
                pthread_cond_signal(&c);
            }
            if (theard1 == b)
            {
                theard1 = 0;
                if (d != 0)
                {
                    done = 2;
                    pthread_cond_signal(&c);
                }
            }
        }
        else if (done == 2 && d != 0)
        {
            //printf("Theard 2");
            counter++;
            theard2++;
            if (counter <= a)
            {
                printf("%d : IN T%d, TID=%u\n", counter, done, pthread_self());
                thread2cnt++;
            }
            if(counter >= a)
            {
                pthread_cond_signal(&c);
            }
            if (theard2 == d)
            {

                theard2 = 0;
                if (b != 0)
                {
                    done = 1;
                    pthread_cond_signal(&c);
                }
            }
        }
        pthread_mutex_unlock(&m);
    }
    
}

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        usage(argv[0]);
        return 0;
    }
    // if(argv[2] == "0" && argv[3] == "0")
    // {
    //     printf("Floating point exception");
    // }
    
    pthread_t tid1, tid2;
    
    a = atoi(argv[1]);
    b = atoi(argv[2]);

    d = atoi(argv[3]);

    int t1 = 1, t2 = 2;
    
    if(b == 0)
    {
        done = 2;
    }
    else if(b==0 & d==0)
    {
        done = 0;
    }
    pthread_create(&tid1, NULL, printThread, (void *)&t1);
    pthread_create(&tid2, NULL, printThread, (void *)&t2);

    while (counter < a)
        ;
    {
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
    }

    printf("In the main thread: T1 printed %d times. T2 printed %d times.\n", thread1cnt, thread2cnt);
    return 0;
}
