#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>

volatile int simRun = 1;
double totalInterArrivalTime = 0.0, totalServiceTime = 0.0, totalWaitTime = 0.0;
double totalSquaredInterArrivalTime = 0.0, totalSquaredServiceTime = 0.0, totalSquaredWaitTime = 0.0;
double totalQueueLength = 0.0, totalSquaredQueueLength = 0.0;
int totalCustomers = 0, totalQueueObservations = 0;
struct timeval startTime, endTime;

pthread_mutex_t statsMutex, qMutex;
pthread_cond_t qCond;

typedef struct customer {
	struct timeval arrivalTime;
	struct customer *next;
}
customer;

customer *qHead = NULL, *qTail = NULL;
unsigned qLength = 0;
pthread_mutex_t qMutex;
pthread_cond_t qCond;

struct timespec calculateSleepTime(double seconds) {
	struct timespec ts;
	ts.tv_sec = (time_t)seconds;
	ts.tv_nsec = (seconds - ts.tv_sec) * 1e9;
	return ts;
}

double rndExp(struct drand48_data *randData, double lambda) {
	double tmp;
	drand48_r(randData, &tmp);
	return -log(1.0 - tmp) / lambda;
}

void insertCustomerIntoQueue(customer *newCustomer) {
	pthread_mutex_lock(&qMutex);
	if (qTail) {
		qTail->next = newCustomer;
		qTail = newCustomer;
	} else {
		qHead = qTail = newCustomer;
		pthread_cond_signal(&qCond);
	}
	qLength++;
	pthread_mutex_unlock(&qMutex);
}

customer* removeCustomerFromQueue() {
	pthread_mutex_lock(&qMutex);
	while (qHead == NULL && simRun) {
		pthread_cond_wait(&qCond, &qMutex);
	}
	customer *cust = qHead;
	if (qHead != NULL) {
		qHead = qHead->next;
		if (qHead == NULL) {
			qTail = NULL;
		}
		qLength--;
	}
	pthread_mutex_unlock(&qMutex);
	return cust;
}

void *thread1(void *param) {
	struct drand48_data randData;
	double interArrivalTime;
	struct timeval lastArrivalTime, currentArrivalTime;

        srand48_r(time(NULL), &randData);
        gettimeofday(&lastArrivalTime, NULL);

	for (int i = 0; i < 1000; i++) {
		interArrivalTime = rndExp(&randData, 5.0);
                struct timespec sleepTime = calculateSleepTime(interArrivalTime);
                nanosleep(&sleepTime, NULL);

                gettimeofday(&currentArrivalTime, NULL);
                double timeDiff = (currentArrivalTime.tv_sec - lastArrivalTime.tv_sec) + (currentArrivalTime.tv_usec - lastArrivalTime.tv_usec) / 1000000.0;

                pthread_mutex_lock(&statsMutex);
                totalInterArrivalTime += timeDiff;
                totalSquaredInterArrivalTime += timeDiff * timeDiff;
                totalCustomers++;
                pthread_mutex_unlock(&statsMutex);

                lastArrivalTime = currentArrivalTime;

                customer *newCustomer = malloc(sizeof(customer));
                gettimeofday(&newCustomer->arrivalTime, NULL);
                newCustomer->next = NULL;
                insertCustomerIntoQueue(newCustomer);
	}
	return NULL;
}

void *thread2(void *param) {
	struct drand48_data randData;
        double serviceTime;
        srand48_r(time(NULL) + 1, &randData);

        for (int i = 0; i < 1000; i++) {
		customer *servicedCustomer = removeCustomerFromQueue();
		if (servicedCustomer) {
			serviceTime = rndExp(&randData, 7.0);
                        struct timespec sleepTime = calculateSleepTime(serviceTime);
                        nanosleep(&sleepTime, NULL);

                        struct timeval currentTime;
                        gettimeofday(&currentTime, NULL);
                        double waitTime = (currentTime.tv_sec - servicedCustomer->arrivalTime.tv_sec) + (currentTime.tv_usec - servicedCustomer->arrivalTime.tv_usec) / 1000000.0;

                        pthread_mutex_lock(&statsMutex);
                        totalServiceTime += serviceTime;
                        totalWaitTime += waitTime;
                        totalSquaredServiceTime += serviceTime * serviceTime;
                        totalSquaredWaitTime += waitTime * waitTime;
                        pthread_mutex_unlock(&statsMutex);
			
			free(servicedCustomer);
		}
	}
	
	simRun = 0;
	pthread_cond_broadcast(&qCond);
        return NULL;
}

void *thread3(void *param) {
	while (simRun) {
		pthread_mutex_lock(&qMutex);
                int currentQueueLength = qLength;
                pthread_mutex_unlock(&qMutex);

                pthread_mutex_lock(&statsMutex);
                totalQueueLength += currentQueueLength;
                totalSquaredQueueLength += currentQueueLength * currentQueueLength;
                totalQueueObservations++;
                pthread_mutex_unlock(&statsMutex);

                struct timespec sleepDuration = calculateSleepTime(0.005);
                nanosleep(&sleepDuration, NULL);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	int opt;
        double lambda = 5.0;
        double mu = 7.0;
        int numCustomer = 1000;
        int numServer = 1;

	while ((opt = getopt(argc, argv, "l:m:c:s:")) != -1) {
		switch (opt) {
			case 'l':
				lambda = atof(optarg);
				if (lambda <= 0) {
					fprintf(stderr, "Lambda must be a positive number.\n");
					return 1;
				}
				break;
			case 'm':
				mu = atof(optarg);
				if (mu <= 0) {
					fprintf(stderr, "Mu must be a positive number.\n");
					return 1;
				}
				break;
			case 'c':
                                numCustomer = atoi(optarg);
                                if (numCustomer <= 0) {
                                        fprintf(stderr, "Number of customers must be a positive integer.\n");
                                        return 1;
                                }
                                break;
                        case 's':
                                numServer = atoi(optarg);
                                if (numServer < 1 || numServer > 5) {
                                         fprintf(stderr, "Number of servers must be between 1 and 5.\n");
                                         return 1;
                                }
                                break;
                        default:
                                fprintf(stderr, "Usage: %s [-l lambda] [-m mu] [-c numCustomer] [-s numServer]\n", argv[0]);
				return 1;
		}
	}
	
	if (lambda >= mu * numServer) {
		fprintf(stderr, "System is unstable (lambda must be less than mu * numServer).\n");
                return 1;
        }

        pthread_t t1, t3, server_threads[numServer];

        printf("In the process of executing and calculating, please wait:\n");
        printf("lambda %.1f, mu %.1f, num customer %d, num server %d\n", lambda, mu, numCustomer, numServer);

        pthread_mutex_init(&qMutex, NULL);
        pthread_mutex_init(&statsMutex, NULL);
        pthread_cond_init(&qCond, NULL);
     
	gettimeofday(&startTime, NULL);

        if (pthread_create(&t1, NULL, thread1, NULL) != 0) {
		perror("Failed to create thread1");
                return 1;
	}
	
	for (int i = 0; i < numServer; i++) {
		if (pthread_create(&server_threads[i], NULL, thread2, NULL) != 0) {
			perror("Failed to create a server thread");
			return 1;
		}
	}
	
	if (pthread_create(&t3, NULL, thread3, NULL) != 0) {
		perror("Failed to create thread3");
                return 1;
        }
	
	pthread_join(t1, NULL);
	for (int i = 0; i < numServer; i++) {
		pthread_join(server_threads[i], NULL);
	}
	
	pthread_join(t3, NULL);
	gettimeofday(&endTime, NULL);
	
	while (qHead != NULL) {
		customer *temp = qHead;
                qHead = qHead->next;
                free(temp);
	}
	
	pthread_mutex_destroy(&qMutex);
        pthread_mutex_destroy(&statsMutex);
        pthread_cond_destroy(&qCond);

        double totalSimulationTime = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.0;
        double avgInterArrivalTime = totalInterArrivalTime / totalCustomers;
        double stdDevInterArrivalTime = sqrt(totalSquaredInterArrivalTime / totalCustomers - avgInterArrivalTime * avgInterArrivalTime);
        double avgServiceTime = totalServiceTime / totalCustomers;
        double stdDevServiceTime = sqrt(totalSquaredServiceTime / totalCustomers - avgServiceTime * avgServiceTime);
        double avgWaitTime = totalWaitTime / totalCustomers;
        double stdDevWaitTime = sqrt(totalSquaredWaitTime / totalCustomers - avgWaitTime * avgWaitTime);
        double avgQueueLength = totalQueueLength / totalQueueObservations;
        double stdDevQueueLength = sqrt(totalSquaredQueueLength / totalQueueObservations - avgQueueLength * avgQueueLength);
        double utilization = (totalServiceTime / (totalSimulationTime * numServer)) * 100.0;

        printf("queue: %u\n", qLength); // Display final queue length
        printf("\nProcess completed! Here are the statistics:\n");
        printf("\t\t\taverage\t\tstandard deviation\n");
        printf("inter-arrival time\t%.6f\t%.6f\n", avgInterArrivalTime, stdDevInterArrivalTime);
        printf("customer waiting time\t%.6f\t%.6f\n", avgWaitTime, stdDevWaitTime);
        printf("service time\t\t%.6f\t%.6f\n", avgServiceTime, stdDevServiceTime);
        printf("queue length\t\t%.6f\t%.6f\n", avgQueueLength, stdDevQueueLength);
        printf("------utilization:%.1f%%------\n", utilization);
	
	return 0;
}


