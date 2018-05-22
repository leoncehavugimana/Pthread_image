/***************************************************************************
 *
 * Sequential version of Quick sort
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define KILO (1024)
#define MEGA (1024*1024)
#define MAX_ITEMS (64*MEGA)

#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}

struct quicksort_struct
{
    int* y;
    unsigned low;
    unsigned high;
};
typedef struct quicksort_struct thestruct;
static int *v;
 void init_array(void)
{
    int i;
    v = (int *) malloc(MAX_ITEMS*sizeof(int));
    for (i = 0; i < MAX_ITEMS; i++)
        v[i] = rand();
}
static unsigned partition(int *v, unsigned low, unsigned high, unsigned pivot_index)
{
    /* move pivot to the bottom of the vector */
    if (pivot_index != low)
        swap(v, low, pivot_index);
    pivot_index = low;
    low++;
    /* invariant:
     * v[i] for i less than low are less than or equal to pivot
     * v[i] for i greater than high are greater than pivot
     */

    /* move elements into place */
    while (low <= high)
    {
        if (v[low] <= v[pivot_index])
            low++;
        else if (v[high] > v[pivot_index])
            high--;
        else
            swap(v, low, high);
    }
    /* put pivot back between two groups */
    if (high != pivot_index)
        swap(v, pivot_index, high);
    return high;
}
static void* quick_sort(void *ptr)
{
    thestruct functstruct, *functstructptr;
    functstructptr=(thestruct*)ptr;
    functstruct=*functstructptr;
    unsigned pivot_index;
    /* no need to sort a vector of zero or one element */
   // if (functstruct.low >= functstruct.high)
   //    EXIT_FAILURE;
    /* select the pivot value */
    pivot_index = (functstruct.low+functstruct.high)/2;
    /* partition the vector */
    pivot_index = partition(functstruct.y, functstruct.low, functstruct.high, pivot_index);
    /* sort the two sub arrays */
    if (functstruct.low < pivot_index)
    {
        //unsigned low=functstruct.low;
        thestruct mainstruct1= {v,functstruct.low,pivot_index-1}, *mainstructptr1;
        mainstructptr1=&mainstruct1;
        quick_sort((void*)mainstructptr1);
    }
    if (pivot_index < functstruct.high)
    {
        //unsigned low=pivot_index+1;
        thestruct mainstruct2= {v,pivot_index+1,functstruct.high}, *mainstructptr2;
        mainstructptr2=&mainstruct2;
        quick_sort((void*)mainstructptr2);
    }
    pthread_exit(NULL);
}
static void print_array(void)
{
    int i;
    for (i = 0; i < MAX_ITEMS; i++)
        printf("%d \n", v[i]);
}

int main(int argc, char **argv)
{
    init_array();
    thestruct mainstruct= {v,0,MAX_ITEMS-1}, *mainstructptr;
    mainstructptr=&mainstruct;
    pthread_t threads[8];
    for(int i=0; i<8; i++)
    {
        int a=  pthread_create(&threads[i],NULL,quick_sort,(void*)mainstructptr);
        if(a)
        {
            printf("Thread %d failed to create",i);
            EXIT_FAILURE;
        }
    }
    for(int i=0; i<8; i++)
    {
        pthread_join(threads[i],NULL);
    }
    print_array();
    EXIT_SUCCESS;
}

