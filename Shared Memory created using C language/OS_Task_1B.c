#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define SHM_SIZE 1512

struct shm{
		char wh[40];
};

//Global Variables
key_t key = 2331;
int shm_id;
char *shm_addr;
unsigned char *buf;
int* obj_num;
struct shm* obj;

void allocate_SM(){

	shm_id = shmget(key,SHM_SIZE, 0777); 
	if (shm_id == -1){
		perror("shmget: Shared Memory Not Found ");
		exit(1);
	}
	
/* attach the given shared memory segment, at some free position */
/* that will be allocated by the system.   */
	shm_addr = shmat(shm_id,NULL, 0);
	if (!shm_addr){
		perror("shmat: ");
		exit(1);
	}
	printf("*****************************\nAttached to the Shared Memory.\n");
/* create a name index on the shared memory segment. */	
	obj_num = (int*) shm_addr;
	*obj_num = 0;
	obj = (struct shm*) ((void*)shm_addr+sizeof(int));
	
}

int main(){
	
	allocate_SM();
	int l,k;
	char del;
	printf("Reading from Shared Memory.\n*****************************\n\n");
	for(l = 0; l < 5; l++){
		printf("%s",obj[l].wh);
	}
	
	//Deattaching the process from the Shared Memory
	 if(shmdt(shm_addr) ==  -1){
		perror("shmdt");
		return 1; ; 
	}
	else 
		printf("\nShared Memory Deattached Successfully\n");
	
	
	
	// delete the shared memory
	printf("\nDo you want to Delete Shared Memory (Y/n): ");
	scanf(" %c", &del);
	if(del == 'y' || del == 'Y'){
		shmctl(shm_id, IPC_RMID, NULL);
		printf("Shared Memory Deleted\n");
	}
	
	
	return 0;
}