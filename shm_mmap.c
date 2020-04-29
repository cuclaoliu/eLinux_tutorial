#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>   //for mode constants
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <semaphore.h>

#define SHM_SEGMENT_SIZE    65536
#define SHM_SEGMENT_NAME    "/demo-shm"
#define SEMAPHORE_NAME      "/demo-sem"

static sem_t *demo_sem;
// If the shared memroy segment does not exist already, create it 
// returns a pointer to the segment or NULL if there is an error
static void* get_shared_memory(void){
    int shm_fd;
    struct shared_data *shm_p;
    // attemp to create the shared memory segment
    shm_fd = shm_open(SHM_SEGMENT_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shm_fd > 0){
        // succeeded: expand it to the desired size 
        // Don't do this every time because ftruncate fills it with zeros
        printf("Creating shared memory and setting size=%d\n", SHM_SEGMENT_SIZE);
        if (ftruncate(shm_fd, SHM_SEGMENT_SIZE) < 0){
            perror("ftruncate");
            exit(1);
        }
        // create a semaphore as well with default value 1
        demo_sem = sem_open(SEMAPHORE_NAME, O_RDWR | O_CREAT, 0666, 1);
        if (demo_sem == SEM_FAILED)
            perror("sem_open failed");
    }else if (shm_fd == -1 && errno == EEXIST){
        // already exists: open again without O_CREAT
        shm_fd = shm_open(SHM_SEGMENT_NAME, O_RDWR, 0);
        demo_sem = sem_open(SEMAPHORE_NAME, O_RDWR);
        if (demo_sem == SEM_FAILED){
            perror("sem_open failed.");
        }
    }
    if (shm_fd == -1){
        perror("shm_open " SHM_SEGMENT_NAME);
        exit(1);
    }
    // map the shared memory
    shm_p = (struct shared_data*)mmap(NULL, SHM_SEGMENT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_p == NULL){
        perror("mmap");
        exit(1);
    }
    return shm_p;
}

int main(int argc, char* argv[]){
    char* shm_p;
    printf("%s PID=%d\n", argv[0], getpid());
    shm_p = (char*)get_shared_memory();

    while(1){
        printf("Press enter to see the current contents of shm\n");
        getchar();
        sem_wait(demo_sem);
        printf("%s\n", shm_p);
        // write our signature to the shared memory
        sprintf(shm_p, "Hello from the process %d\n", getpid());
        sem_post(demo_sem);
    }
    return 0;
}