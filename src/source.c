#include "source.h"
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define RECSIZE (KEYSIZE + DATASIZE)

FILE * fd;
char *addr;

static int TotalThreads = 0;
static int ThreadsRunning = 0;
static pthread_t * p_tids;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

//put functions here

int compareKey(const char * key1, const char * key2) { // returns 0 if equal, -1 if k1<k2, 1 if k1>k2
	return strncpy(key1, key2, KEYSIZE); // <string.h>
}

struct Record* merge(struct Record a[], struct Record b[]) {
    int alength = sizeof(a);
    int blength = sizeof(b);

    int length = alength;
    if (blength > length) {
        length = blength;
    }

    struct Record* toReturn = malloc(RECSIZE * length);

    int ai = 0;
    int bi = 0;
    for (int i = 0; i < length; i++) {
        if (ai >= alength) {
            toReturn[i] = b[bi];
            bi++;
        } else if (bi >= blength) {
            toReturn[i] = a[ai];
            ai++;
        } else {
            int temp = compareKey(a[ai].key, b[bi].key);
            if (temp < 0) {
                toReturn[i] = a[ai];
                ai++;
            } else if (temp > 0) {
                toReturn[i] = b[bi];
                bi++;
            } else {
                toReturn[i] = a[ai];
                ai++;
            }
        }
    }

    return toReturn;
}

void* threadFunc(void *param) {
    int s = pthread_mutex_lock(&mtx);
	if (s != 0) {
		printf("pthread_mutex_lock error");
		exit(EXIT_FAILURE);
	}
	ThreadsRunning++;

	while (ThreadsRunning < TotalThreads) {
	    s = pthread_cond_wait(&cond, &mtx);
        if (s != 0) {
        	printf("pthread_cond_wait error");
        	exit(EXIT_FAILURE);
       	}
	}

	s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        printf("pthread_mutex_unlock error");
    	exit(EXIT_FAILURE);
    }

    s = pthread_cond_signal(&cond);
    if (s != 0) {
    	printf("pthread_cond_signal error");
    	exit(1);
    }

	struct ThdArg *args = (struct ThdArg*)param;

	//printf("Sorting %d\n", args->thdNum);
	qsort(args->first, args->itemCount, RECSIZE, compareKey);
	//printf("Done! %d\n", args->thdNum);

	pthread_exit(args->first);
}

        
int main(int argc, char * argv[]){
        int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        long pageSize = sysconf(_SC_PAGESIZE);

        printf("Number of logical processors: %d\n", numCPU);
        if (argc != 3){
                printf("Usage: %s file threads\n", argv[0]);
        }

        fd = fopen(argv[1], "r");
        if (fd==-1 || NULL == fd){
                printf("File open failed.\n");
                exit(EXIT_FAILURE);
        }
        fseek(fd, 0, SEEK_END);
	int fSize = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	int nRecs = fSize / RECSIZE;

	struct Record* records = malloc(nRecs * RECSIZE);
	fread(records, fSize, 1, fd);

//	for (int r = 0; r < nRecs; r++) {
//		printf("%s %s", records[r].key, records[r].data);
//	}
        
        //addr = mmap(NULL, chunksize, PROT_READ, MAP_PRIVATE, fd, 0);
        //if(addr == MAP_FAILED){
        //        printf("mmap failed \n");
        //        exit(EXIT_FAILURE);
        //}
        
        int i = argv[2];
        printf("Sorting with %d threads.\n", i);
        TotalThreads = i;

	int nRecsPerThd = nRecs / i;

        for (int j = 0; j < i; j++) {
		struct ThdArg * aaa = malloc(sizeof(struct ThdArg));

		int start = nRecsPerThd * j;
		int end = (nRecsPerThd * (j + 1)) - 1;
		
		(*aaa).thdNum = j;
		(*aaa).itemCount = nRecsPerThd;
		(*aaa).first = &records[start];
		(*aaa).last = &records[end];

		pthread_create(&p_tids, NULL, threadFunc,(void *)aaa);
		p_tids++;
        }

    struct Record* final = malloc(nRecs * RECSIZE);
	for (int k = 0; k < i; k++) {
	    void* temp;
	    struct Record* recs;
		pthread_join(p_tids[k], &temp);
		recs = temp;

		final = merge(recs, final);
	}

	for (int r = 0; r < nRecs; r++) {
	    printf("%s %s", final[r].key, final[r].data);
	}

        pthread_exit(NULL);
        close(fd);
        exit(EXIT_SUCCESS);
}
