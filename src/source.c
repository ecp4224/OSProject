#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>

int fd;
char *addr;

//put functions here

int main(int argc, char * argv[]){
        struct stat sb;
        int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        static pthread_t * p_tids;
        long pageSize = sysconf(_SC_PAGESIZE);

        printf("Number of logical processors: %d\n", numCPU);

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
                pthread_create(&pid1, NULL, threadFunc,0);
                
        }
        pthread_exit(NULL);
        close(fd);
        exit(EXIT_SUCCESS);
}
