#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

#define KEYSIZE 8
#define DATASIZE 56

struct Record{
        char key[KEYSIZE];
        char data[DATASIZE];
}

int fd;
char *addr;

//put functions here

int compareKey(const char * key1, const char * key2){ // returns 0 if equal, -1 if k1<k2, 1 if k1>k2
	return strncpy(key1, key2, KEYSIZE); // <string.h>
}

int sortThread()

int *threadFunc(void *param){
        qsort(param, /*NUMBEROFITEMS*/, KEYSIZE, compareKey);
}

        
int main(int argc, char * argv[]){
        struct stat sb;
        int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        pthread_t * p_tids;
        long pageSize = sysconf(_SC_PAGESIZE);

        printf("Number of logical processors: %d\n", numCPU);
        if (argc != 2){
                printf("Usage: %s file\n", argv[0]);
        }

        fd = open(argv[1], O_RDONLY);
        if (fd==-1){
                printf("File open failed.\n");
                exit(EXIT_FAILURE);
        }
        if (fstat(fd,&sb) == 1){
                printf("fstat error\n");
                exit(EXIT_FAILURE);
        }
        
        addr = mmap(NULL, chunksize, PROT_READ, MAP_PRIVATE, fd, 0);
        if(addr == MAP_FAILED){
                printf("mmap failed \n");
                exit(EXIT_FAILURE);
        }
        
        int i;
        printf("Please input an integer value for the number of threads to use: ");
        scanf("%d", &i);
        printf("Sorting with %d threads.\n", i);

        for (j = 0; j < i; i++){
                pthread_create(&p_tids, NULL, threadFunc,void( *)/*POINTERTOFIRSTITEMINARRAY*/);
                p_tids++;
                
        }
        pthread_exit(NULL);
        close(fd);
        exit(EXIT_SUCCESS);
}
