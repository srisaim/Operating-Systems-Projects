#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#define Size 1024

/*The following code uses one loop to calculate the avg/std of an
 *array.  It is an online method, applicable to the case when the
 *numbers are not in an array but are streamed in, such as the queue
 *lengths monitored by a thread
 */
void onlineAvgStd(double *avg, double *std, double array[], int size) {
  double sumX, sumX2; //sum of X, sum of X^2
  int i;
  
  sumX = sumX2 = 0.0;
  for (i = 0; i < size; i++) {
    sumX += array[i];
    sumX2 += array[i] * array[i];
  }
  *avg = sumX / size;
  *std = sqrt( (sumX2 - (*avg) * (*avg) * size) / (size - 1) );
}

/*The following code uses two separate loops to calculate avg/std.
 *All the numbers must be stored in the array.  Thus this is an
 *off-line method.  It is troublesome to use an off-line method for
 *project 5
 */
void offlineAvgStd(double *avg, double *std, double array[], int size) {
  double sum;
  int i;
  
  sum = 0.0;
  for (i = 0; i < size; i++)
    sum += array[i];
  *avg = sum / size;
  sum = 0.0;
  for (i = 0; i < size; i++)
    sum += (array[i] - *avg) * (array[i] - *avg);
  *std = sqrt( sum / (size-1) );
}

int main(int argc, char *argv[]) {
  struct drand48_data randData;
  struct timeval tv;
  double array[Size], avg, std;
  int i;

  gettimeofday(&tv, NULL);
  //seed the pseudorandom number generator
  srand48_r(tv.tv_sec + tv.tv_usec, &randData);
  for (i = 0; i < Size; i++)
    drand48_r(&randData, &array[i]);

  onlineAvgStd(&avg, &std, array, Size);
  printf("online avg/std: %lf/%lf\n", avg, std);
  offlineAvgStd(&avg, &std, array, Size);
  printf("offline avg/std: %lf/%lf\n", avg, std);

  return 0;
}
