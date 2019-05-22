//l72
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
using namespace std;

sem_t *semWrite;
sem_t *semRead;
int fd,*adr;


typedef struct someArgs_tag 
{
    bool flag1;
} someArgs_t;

void* funcThread(void *args) 
{
    someArgs_t *arg = (someArgs_t*) args;
    int a;
while(arg->flag1)
{
        sem_wait(semWrite);
        a = *adr;
        cout << "Reading from shared-memory:"<<endl;
        cout << a << flush;
        cout << endl;
        sem_post(semRead);
  }
}

int main() 
{
     pthread_t id1;
    //create area of shared-memory
    fd=shm_open("/mem",  O_CREAT|O_RDWR, 0644);
    //set size of area
    ftruncate(fd, sizeof(int));
    
   //map shared memory to local address
    adr=(int*)mmap(0,sizeof(int),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

    semWrite = sem_open("/semwrite",O_CREAT,0644, 0);
    semRead = sem_open("/semread",O_CREAT,0644, 0);

    someArgs_t args;
    args.flag1=true;

    pthread_create(&id1, NULL, funcThread,(void*)&args);
    getchar();
 
    args.flag1 = 0;

    pthread_join(id1, NULL);

    sem_close(semWrite );
    sem_close(semRead);

    sem_unlink("/semread");
    sem_unlink("/semwrite");

    munmap(adr, sizeof(int));
    
    close(fd);
    shm_unlink("/mem");
    return 0;
}


