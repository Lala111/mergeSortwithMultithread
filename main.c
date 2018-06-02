#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>

const int size = 100000;

int a[1000005];
int thread = 0;
int halv=0;

void merging(int low, int mid, int high) {
        int i, j, k;
    int n_left = mid - low + 1;
    int n_right =  high - mid;
    int *L = malloc(n_left * sizeof(int));
    int *R = malloc(n_right * sizeof(int));

    for (i = 0; i < n_left; i++)
        L[i] = a[low + i];
    for (j = 0; j < n_right; j++)
        R[j] = a[mid + 1 + j];


    i = 0;    j = 0; k = low; 
    while (i < n_left && j < n_right)
    {
        if (L[i] <= R[j])        a[k] = L[i++];
        else                     a[k] = R[j++];
        k++;
    }

 
    while (i < n_left)
    {
        a[k++] = L[i++];
    }

 
    while (j < n_right)
    {
        a[k++] = R[j++];
    }
    free(L);
    free(R);
}


void sort(int low, int high) {
   if(low < high) {
      int mid =  (high - low) / 2 + low;
      sort(low, mid);
      sort(mid + 1, high);
      merging(low, mid, high);
   }
}

void* sort_thread(void* arg) {
    int low = halv * (size / 2);
    int high = (halv + 1) * (size / 2) - 1;
	if (low < high) {
		int mid = (high - low) / 2 + low;
		sort(low, mid);
		sort(mid + 1, high);
		merging(low, mid, high);
	}
}

int main() { 
   int i;
struct timespec start, finish;
double elapsed;
   
   for(i = 0; i < size; i++)
            a[i] = rand() % 10000;
     
   clock_gettime(CLOCK_MONOTONIC, &start);
		pthread_t threads[2];

		halv++; pthread_create(&threads[0], NULL, sort_thread, (void*) NULL);
		halv++; pthread_create(&threads[1], NULL, sort_thread, (void*) NULL);

		pthread_join(threads[0], NULL);
		pthread_join(threads[1], NULL);
                merging(0, (size / 2 - 1) / 2, size / 2 - 1);

   clock_gettime(CLOCK_MONOTONIC, &finish);
   elapsed = (double)(finish.tv_sec-start.tv_sec);
   double nelapsed=(finish.tv_nsec-start.tv_nsec)/100000000.0;

   printf("\nList after sorting\n");
   
   for(i = 0; i < size; i++)
      printf("%d ", a[i]);

	printf("running time: %.4lf\n", elapsed+nelapsed);
}
