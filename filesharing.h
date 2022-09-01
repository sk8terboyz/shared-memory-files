#ifndef FILE_SHARING
#define FILE_SHARING
#define SIZE (1024*51200)  //size of the shared memory (50 MBs)

struct filesharing
{
  char flag;
  int size;
  char fileName[20];
  char content[SIZE];
};
typedef struct filesharing fs;
#endif
