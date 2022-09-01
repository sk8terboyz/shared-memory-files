#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include "filesharing.h"

#define SIZE (1024*51200)   //50 MBs
#define WRITE_SIZE 1024

int main()
{
  //name of shared memory object
  const char *name = "File_Sharing_Complete.txt";
  //create shared memory file descriptor
  int fd;
  int shm_fd;
  int size;
  //pointer to shared memory object
  fs *ptr;

  //open the shared memory object
  shm_fd = shm_open("skenne5_filesharing", O_RDONLY, 0666);

  //check if shm_fd failed
  if(shm_fd < 0)
  {
    printf("Shared Memory File Descriptor failed\n");
    exit(-1);
  }

  //memory map the shared memory object
  ptr = (fs*)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  //check if mmap failed
  if(ptr == MAP_FAILED)
  {
    printf("mmap failed\n");
    exit(-1);
  }

  //check if there is content within the shared memory
  if(ptr->content == NULL)
  {
    printf("No content in shared memory\n");
    exit(-1);
  }
  //check if there is a size in the shared memory
  if(ptr->size == 0)
  {
    printf("Shared memory has no size\n");
    exit(-1);
  }
  //check if the flag is n
  if(ptr->flag == 'n')
  {
    printf("Flag occurred\n");
    exit(-1);
  }
  //check if there is a name for this
  if(ptr->fileName == NULL)
  {
    printf("No file name is stored in the shared memory\n");
    exit(-1);
  }

  //open new file to write to
  fd = open(name, O_CREAT | O_WRONLY, 0666);

  //check if fd failed
  if(fd < 0)
  {
    printf("File Descriptor failed\n");
    exit(-1);
  }

  size = ptr->size;

  //write to new file
  while (size > 0)
  {
    if(size < WRITE_SIZE)
    {
      write(fd, ptr->content, size);
    }
    else
    {
      write(fd, ptr->content, WRITE_SIZE);
    }
    size -= WRITE_SIZE;
  }
  close(fd);
  shm_unlink("skenne5_filesharing");
  return 0;
}
