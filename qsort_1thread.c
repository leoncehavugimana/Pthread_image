/***************************************************************************
Sequential version of Quick sort
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define KILO (1024)
#define MEGA (1024*1024)
#define MAX_ITEMS (64*MEGA)

#define swap(v, a, b) {unsigned tmp; tmp=v[a]; v[a]=v[b]; v[b]=tmp;}

static int *v;
struct quick_struct
{
    int* y;
    unsigned low;
    unsigned high;
};
typedef struct quick_struct thestruct;

static void init_array(void)
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
            swap(v,low,high);
    }
    /* put pivot back between two groups */
    if (high != pivot_index)
        swap(v, pivot_index, high);
    return high;
}

//static void quick_sort(int *v, unsigned low, unsigned high)
static void* quick_sort(void* ptr)
{
    thestruct mystruct;
    thestruct *mystructptr;
    mystructptr=(thestruct*)ptr;
    mystruct=*mystructptr;
    unsigned pivot_index;
    /* no need to sort a vector of zero or one element */
    if (mystruct.low >= mystruct.high)
        return 0;
    /* select the pivot value */
    pivot_index = (mystruct.low+mystruct.high)/2;
    /* partition the vector */
    pivot_index = partition(mystruct.y, mystruct.low, mystruct.high, pivot_index);
    /* sort the two sub arrays */
    if (mystruct.low < pivot_index)
    {
        mystructptr->high=pivot_index-1;
        quick_sort ( (void*) v);
    }
    if (pivot_index < mystruct.high)
        quick_sort ( (void*) v);
    pthread_exit(NULL);
}

static void print_array(void)
{
    int i;
    for (i = 0; i < MAX_ITEMS; i++)
    {
        printf("%d  ", v[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    thestruct mainstruct;
    mainstruct.y=v;
    mainstruct.low=0;
    mainstruct.high=MAX_ITEMS-1;
    pthread_t thread;
    init_array();
    pthread_create(&thread,NULL,quick_sort,(void*)v);
    print_array();
}

