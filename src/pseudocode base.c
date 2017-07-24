/*
Notable points:
Test file consists of 128 64 byte records for testing and debugging. 
Each record has an 8 byte key and 56 bytes of data.
Input has 1 million fixed size records of 64 bytes each.

Requirements:
Find cores and display to user, who then specifies the number of threads to use
Number of threads must be a power of 2
Record the elapsed time with the test file and the primary input file.
*/


//functions 
int sortThread();
void mergeArrays();
int compareKeys();

//finding pagesize
long pageSize = sysconf(_SC_PAGESIZE);

//taking user input for number of threads
int i;
printf("Please input an integer value for the number of threads to use: ");
scanf("%d", &i);
printf("Sorting with %d threads.\n", i);

int compareKey(const char * key1, const char * key2){ // returns 0 if equal, -1 if k1<k2, 1 if k1>k2
	return strncpy(key1, key2, KEYSIZE); // <string.h>
}

void mergeArrays(){ // make a 3rd array, fill it with elements by comparing the first element of the first two arrays
	int x = 0;
	int y = 0;
	int z = 0;
	while()
	if (compareKey(/*PUT KEY INPUTS HERE*/)>0){
		//take k2
		a3[z++] = a2[y++];
	}else{
		//take k1
		a3[z++] = a1[x++];
	}
	
	
}

