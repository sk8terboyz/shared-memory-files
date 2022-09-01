#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "filesharing.h"

#define SIZE (1024*51200)  //size of the shared memory : 50 MBs in bytes
#define READ_SIZE 1024    //max reading size from file

int main()
{
  //name of shared memory object
  const char *name = "skenne5_filesharing";
  //create shared memory file descriptor
  int fd;
  int shm_fd;
  //pointer to shared memory object
  fs *ptr;

  //create the shared memory object
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  //check if fd failed
  if(shm_fd < 0)
  {
    printf("Shared Memory File Descriptor failed\n");
    exit(-1);
  }

  //resize the shared memory object
  ftruncate(shm_fd, SIZE);

  //memory map the shared memory object
  ptr = (fs*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  //check if mmap failed
  if(ptr == MAP_FAILED)
  {
    printf("mmap failed\n");
    exit(-1);
  }

  //Get the file name from the user
  printf("Type the name of the file to be shared: ");
  scanf("%s", ptr->fileName);

  //open file given from user
  fd = open(ptr->fileName, O_RDONLY, 0644);

  //check if fd failed
  if(fd < 0)
  {
    printf("File Descriptor failed\n");
    exit(-1);
  }
  //track the bytes within the content of the file
  int bytes = read(fd, ptr->content, READ_SIZE);
  int totalBytes = bytes;

  while(bytes > 0)
  {
    bytes = read(fd, ptr->content, READ_SIZE);

    if(bytes < 0)
    {
      printf("Error while reading from file : %s", ptr->fileName);
      exit(-1);
    }
    totalBytes += bytes;
  }
  //set size within the filesharing struct
  ptr->size = totalBytes;
  //flag if content did not copy into share memory
  if(ptr->content == NULL)
    ptr->flag = 'n';
  else
    ptr->flag = 'y';

  close(fd);
  return 0;
}
