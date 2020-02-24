/* On-demand 3-thread synchronization */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Compile with "gcc cv-sync-2.c -o cv-sync-2 -lpthread"

pthread_mutex_t m;
pthread_cond_t cond;
int counter = 0;
int done = 1;
int a, b, c, d;
int thrd1 = 0;
int thrd2 = 0;
int thrd3 = 0;

void usage(char *bin_str)
{
    printf("Usage: %s total_runs t1_runs t2_runs t3_runs\n", bin_str);
}

void *printThread(void *n)
{
    int thread1 = 0;
    int thread2 = 0;
    int thread3 = 0;
    if (a != 0 && b == 0 && c == 0 && d == 0)
    {
        //do nothing
        
    }
    else
    {
        while (counter <= a)
        {
            pthread_mutex_lock(&m);

            if ((int)*(int *)n == 1 && b == 0 && done == 1) //Run T1 for zero times
            {
                if (counter == a) //exhausted all iterations
                {
                    //do nothing
                }
                else
                {
                    if (c != 0)
                        done = 2;
                    else if (d != 0)
                        done = 3;
                }
            }
            else if ((int)*(int *)n == 2 && c == 0 && done == 2) //Run T2 for zero times
            {
                if (counter == a) //exhausted all iterations
                {
                    //do nothing
                }
                else
                {
                    if (d != 0)
                        done = 3;
                    else if (b != 0)
                        done = 1;
                }
            }
            else if ((int)*(int *)n == 3 && d == 0 && done == 3) //Run T3 for zero times
            {
                if (counter == a) //exhausted all iterations
                {
                    //do nothing
                }
                else
                {
                    if (b != 0)
                        done = 1;
                    else if (c != 0)
                        done = 2;
                }
            }
            else if (done != (int)*(int *)n) //We are in the another thread while other thread is stil excuting
            {
                if ((int)*(int *)n == 1 && b != 0)
                {
                    
                    if (counter != a)
                        pthread_cond_wait(&cond, &m);
                }
                else if ((int)*(int *)n == 2 && c != 0)
                {
                   
                    if (counter != a)
                        pthread_cond_wait(&cond, &m);
                }
                else if ((int)*(int *)n == 3 && d != 0)
                {
                    
                    if (counter != a)
                        pthread_cond_wait(&cond, &m);
                }
            }

            if (done == 1 && b != 0) //In T1 thread
            {
                counter++;
                thread1++;
                if (counter <= a)
                {
                    thrd1++;
                    printf("%d : IN T%d, TID=%u\n", counter, done, pthread_self());
                }

                if (thread1 == b && counter != a)
                {
                    thread1 = 0;
                    if (c != 0) //if T2 is not equal to zero then invoke T2
                    {
                        
                        done = 2;
                        pthread_cond_signal(&cond);
                    }
                    else if (d != 0) // If T2 is zero and T3 is not equal to zero then invoke T3
                    {
                        
                        done = 3;
                        pthread_cond_signal(&cond);
                    }
                    //else T2 and T3 continue waiting; No need to signal any other thread unless T1 completes its execution
                }
                else if (counter == a) //Last executing thread
                {
                    thread1 = 0;
                    if (c != 0) //if T2 is not equal to zero then invoke T2
                    {
                        
                        done = 2;
                        pthread_cond_signal(&cond);
                    }
                    if (d != 0) // If T2 is zero and T3 is not equal to zero then invoke T3
                    {
                        
                        done = 3;
                        pthread_cond_signal(&cond);
                    }
                }

                if (counter == a && thread1 == a && c == 0) //T1 completed its excution now invoke T2
                {
                    done = 2;
                }
                else if (counter == a && thread1 == a && d == 0) //T1 completed its excution now invoke T3
                {
                    done = 3;
                }
            }
            else if (done == 2 && c != 0) //In T2 thread
            {
                counter++;
                thread2++;

                if (counter <= a)
                {
                    thrd2++;
                    printf("%d : IN T%d, TID=%u\n", counter, done, pthread_self());
                }

                if (thread2 == c && counter != a)
                {
                    thread2 = 0;
                    if (d != 0) //if T3 is not equal to zero then invoke T3
                    {
                        
                        done = 3;
                        pthread_cond_signal(&cond);
                    }
                    else if (b != 0) // If T3 is zero and T1 is not equal to zero then invoke T1
                    {
                        
                        done = 1;
                        pthread_cond_signal(&cond);
                    }
                    //else T1 and T3 continue waiting; No need to signal any other thread unless T2 completes its execution
                }
                else if (counter == a) //Last excuting thread
                {
                    thread2 = 0;
                    if (d != 0) //if T3 is not equal to zero then invoke T3
                    {
                        
                        done = 3;
                        pthread_cond_signal(&cond);
                    }
                    if (b != 0) // If T3 is zero and T1 is not equal to zero then invoke T1
                    {
                        
                        done = 1;
                        pthread_cond_signal(&cond);
                    }
                }
                if (counter == a && thread2 == a && d == 0) //T2 completed its excution now invoke T3
                {
                    done = 3;
                }
                else if (counter == a && thread2 == a && b == 0) //T2 completed its excution now invoke T1
                {
                    done = 1;
                }
            }
            else if (done == 3 && d != 0) //In T3 thread
            {
                counter++;
                thread3++;

                if (counter <= a)
                {
                    thrd3++;
                    printf("%d : IN T%d, TID=%u\n", counter, done, pthread_self());
                }

                if (thread3 == d && counter != a)
                {
                    thread3 = 0;
                    if (b != 0) //if T1 is not equal to zero then invoke T1
                    {
                        
                        done = 1;
                        pthread_cond_signal(&cond);
                    }
                    else if (c != 0) // If T1 is zero and T2 is not equal to zero then invoke T2
                    {
                        
                        done = 2;
                        pthread_cond_signal(&cond);
                    }
                    //else T1 and T3 continue waiting; No need to signal any other thread unless T2 completes its execution
                }
                else if (counter == a) //Last excuting thread
                {
                    thread3 = 0;
                    if (b != 0) //if T1 is not equal to zero then invoke T1
                    {
                        
                        done = 1;
                        pthread_cond_signal(&cond);
                    }
                    if (c != 0) // If T1 is zero and T2 is not equal to zero then invoke T2
                    {
                        
                        done = 2;
                        pthread_cond_signal(&cond);
                    }
                }

                if (counter == a && thread3 == a && b == 0) //T3 completed its excution now invoke T1
                {
                    done = 1;
                }
                else if (counter == a && thread3 == a && c == 0) //T3 completed its excution now invoke T2
                {
                    done = 2;
                }
            }

            if(counter >= a)
            {
                pthread_cond_signal(&cond);
            }

            pthread_mutex_unlock(&m);
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        usage(argv[0]);
        return 0;
    }

    //printf("Parent Begin!!\n");
    pthread_t tid1, tid2, tid3;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);

    int t1, t2, t3;
    t1 = 1;
    t2 = 2;
    t3 = 3;

    pthread_create(&tid1, NULL, printThread, (void *)&t1);
    pthread_create(&tid2, NULL, printThread, (void *)&t2);
    pthread_create(&tid3, NULL, printThread, (void *)&t3);

    while (counter <= a)
        ;
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    printf("In the main thread: T1 printed %d times. T2 printed %d times. T3 printed %d times.\n", thrd1, thrd2, thrd3);
    return 0;
}
