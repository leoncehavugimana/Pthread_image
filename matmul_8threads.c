/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

pthread_mutex_t mutex;

static void init_matrix(void)
{
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            /* Simple initialization, which enables us to easy check
             * the correct answer. Each element in c will have the same
             * value as SIZE after the matmul operation.
             */
            a[i][j] = 1.0;
            b[i][j] = 1.0;
        }
    }
}

static void* matmul(void *ptr)
{
    int row_controller,*row_ptr;
    row_ptr=(int*)ptr;
    row_controller=*row_ptr;
    int i, j, k;
    for (i = row_controller*128; i < (row_controller*128)+128; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
             pthread_mutex_lock(&mutex);
            c[i][j]=0.0;
            for (k = 0; k < SIZE; k++)
            {
                c[i][j] = c[i][j] + (a[i][k] * b[k][j]);
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}
static void print_matrix(void)
{
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            printf(" %7.2f",c[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    init_matrix();
    pthread_mutex_init(&mutex,NULL);
    pthread_t threads[8];
    for(int i=0; i<8; i++)
    {
        if(pthread_create(&threads[i],NULL,matmul,&i))
        {
            printf(" Thread number %d failed to be created",i);
            EXIT_FAILURE;
        }
    }
    for( int i=0; i<8; i++)
    {
        pthread_join(threads[i],NULL);
    }
    print_matrix();
    return 0;
    pthread_exit(NULL);
}

