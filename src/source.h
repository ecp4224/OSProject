#define KEYSIZE 8
#define DATASIZE 56

struct Record {
	char key[KEYSIZE];
	char data[DATASIZE];
};

struct ThdArg {
	int thdNum;
	int itemCount;
	struct Record * first;
	struct Record * last;
};
