#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h> //for shared memory
#include <sys/shm.h> //for shared memory
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define SHM_SIZE 1512
struct shm{
		char wh[40];
};


/* Global Variable */
FILE *fp;
char Name[5][20];
char City[5][20];
int Age[5];
char whole[40];

//Shared Memory Variables
int shm_id; /* this variable is used to hold the returned segment identifier. */
key_t key = 2331;
pthread_mutex_t mutex;
char* shm_addr;
int* obj_num;
struct shm* obj;


/* Thread Function Declaration */
void *readNames(void *vargp);
void *readCities(void *vargp);
void *readAges(void *vargp);
void *printDetails(void *vargp);
void create_sm();


// Thread 1 Read Name from OS.txt
void *readNames(void *vargp){
	pthread_mutex_lock(&mutex);
	int i;
	
	printf("*********************************************\nThread 1 is running, Reading Names\n");
	for(i=0; i < 5; i++){
	
		fscanf(fp,"%s",Name[i]);
		if(i == 4){
			pthread_mutex_unlock(&mutex);
		}
	}

}

//Thread 2 Read Cities from OS.txt
void *readCities(void *vargp){
	pthread_mutex_lock(&mutex);
	printf("Thread 2 is running, Reading Cities\n");
	int i;
	for(i=0; i<5; i++){
		fscanf(fp,"%s",City[i]);
		if(i == 4){
			pthread_mutex_unlock(&mutex);
		}
	}
}

//Thread 3 Read Ages from OS.txt
void *readAges(void *vargp){
	pthread_mutex_lock(&mutex);
	printf("Thread 3 is running, Reading Names\n");
	int i;
	for(i=0; i<5; i++){
		fscanf(fp,"%d",&Age[i]);
		if(i == 4){
			pthread_mutex_unlock(&mutex);
		}
		
	}
}

//Thread 4 print Details
void *printDetails(void *vargp){
	
	printf("Thread 4 is running, Printing Details\n*********************************************\n\n");
	int i,l;
	
	for(i=0; i<5; i++, l++){
		sprintf(whole,"%s from %s. Age: %d \n", Name[i],City[i],Age[i]);
		
		strcpy(obj[i].wh, (char *) whole); //Copying to the shared memory
		(*obj_num)++; //Incrementing Shared Memory Pointer
		printf("%s",whole);
	}
	printf("\n**Copied to Shared Memory Successfully**\n**Reading From Shared Memory**\n\n");
	for(l = 0; l < 5; l++)
		printf("%s",obj[l].wh);
	
}

//Create and Allocate Shared Memory
void create_sm(){
	
	//allocate a shared memory segment
	shm_id = shmget(key,SHM_SIZE, IPC_CREAT | 0777); 
	if (shm_id == -1){
		perror("shmget: Shared Memory Not Created. ");
		exit(1);
	}
	else
		printf("Shared memory created. Shm ID: %d. Size: %d\n", shm_id, SHM_SIZE);
	
/* attach the given shared memory segment, at some free position */
/* that will be allocated by the system.   */
	shm_addr = shmat(shm_id,NULL, 0);
	if (!shm_addr){
		perror("shmat: ");
		exit(1);
	}

/* create a name index on the shared memory segment. */	
	obj_num = (int*) shm_addr;
	*obj_num = 0;
	obj = (struct shm*) ((void*)shm_addr+sizeof(int));
	
	
}

int main(){
	
	fp = fopen("OS.txt","r");
	
	if(fp==NULL)
	{
		printf("cannot open Files");
		exit(1);
	}
	
	pthread_t thread1,thread2,thread3,thread4;
	int k = 0;
	int t1,t2,t3,t4;
	
	
	
	
	//Thread 1 Create to read names from the OS.txt file
	t1 = pthread_create(&thread1, NULL, readNames, NULL);
	//pthread_join(thread1, NULL);
	//Thread 2 create to read Cities from the OS.txt file
	t2 = pthread_create(&thread2, NULL, readCities, NULL);
	//pthread_join(thread2, NULL);
	//Thread 3 create to read Ages from the OS.txt file
	t3 = pthread_create(&thread3, NULL, readAges, NULL);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	create_sm();
	
	
	//Thread 4 Print details that stored in NAME,CITIES, AGE arrays
	//Store it in shared memory
	t4 = pthread_create(&thread4, NULL, printDetails, NULL);
	pthread_join(thread4, NULL);
	
	fclose(fp);

	for(;;)  //infinite loop to wait from exiting
		k++;

	return 0;
}