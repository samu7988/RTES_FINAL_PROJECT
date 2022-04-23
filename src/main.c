/********************************************************************************************
*					FILENAME:c
**********************************************************************************************/
/* Title: 
 * Brief: 
 * Author: Sayali Mule
 * Date Created: April 9, 2022
 * Course: Advanced Embedded Software Development


 * Reference[1]: 
 * 
 * 
 * */
/****************************************************************************************
*					HEADER FILE SECTION
*****************************************************************************************/
#define _GNU_SOURCE

#include<stdint.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>
#include <time.h>
#include <semaphore.h>

#include <syslog.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include "v4l2_driver.h"
#include "thread.h"

#define NUM_CPU_CORES (1)
#define NUM_THREADS	(4 + 1)

/**************************************************************************************
*					Function declaration
*******************************************************************************************/
void print_scheduler(void);
/**************************************************************************************
*					GLOBAL VARIABLE
*******************************************************************************************/

typedef struct
{
    int threadIdx;
    unsigned long long sequencePeriods;
} threadParams_t;
struct utsname system_info;
cpu_set_t allcpuset;
sem_t semS1, semS2, semS3, semS4, semS5, semS6, semS7;
int abortS1 = 0;
int total_frames = 0;
char ppm_header[200];
/**********************************************************************************
*				FUNCTION DEFINITION
***************************************************************************************/
/***********************************************************************************************
 * @brief 
 *
 *
 *
 * @param address:
 *
 * @return 
 *********************************************************************************************/
int main(int argc, char** argv)
{

    struct timeval current_time_val;
    int i, rc, scope;
    cpu_set_t threadcpu;
    pthread_t threads[NUM_THREADS];
    threadParams_t threadParams[NUM_THREADS];
    pthread_attr_t rt_sched_attr[NUM_THREADS];
    int rt_max_prio, rt_min_prio;
    struct sched_param rt_param[NUM_THREADS];
    struct sched_param main_param;
    pthread_attr_t main_attr;
    pid_t mainpid;
    cpu_set_t allcpuset;

    if(argc < 2)
    {
        printf("\n\rInvalid Input: Usage is ./main <freq> <socket enable/disable>");
        exit(0);
    }

    uint8_t freq = atoi(argv[1]);
    if(freq != 1 && freq != 10)
    {
        printf("\n\rInvalid input: Frequency should either be 1Hz or 10 Hz");
        exit(0);
    }

    uint8_t socket = atoi(argv[2]);
    if(socket != 1 && socket != 0)
    {
        printf("\n\rInvalid input: Acceptable value for socket field is either 0 or 1");
        exit(0);
    }

    if(freq == 1)
    {
      total_frames = 1800; //This is based upon calculation that in 30 mins, 1800 frames would be captured at 1 Hz
    }
    else if(freq == 10)
    {
      total_frames = 18000;
    }
    printf("\n\rExecuting program for frequency %u and socket enable %u",freq,socket);
    printf("\n\rMachine has %u processors and %u available",get_nprocs_conf(), get_nprocs());

    if( uname(&system_info) != 0)
    {
        printf("\n\rUname() failed");
        exit(0);
    }

    printf("Host name: %s\n",system_info.sysname);
    printf("Node name: %ld\n",sizeof(system_info.nodename));
    printf("Release:   %s\n",system_info.release);
    printf("Version:   %s\n",system_info.version);
    printf("machine:   %s\n",system_info.machine);
		sprintf(ppm_header,"P6\n#9999999999 sec 9999999999 msec                                                                            \n%s %s\n255\n",HORIZONTAL_RES_STR,VERTICAL_RES_STR);
    
    //Reference: Sam Siewert sequencer code
    CPU_ZERO(&allcpuset);

    for(int i=0; i < NUM_CPU_CORES; i++)
    {
       CPU_SET(i, &allcpuset);
    }

    printf("Using CPUS=%d from total available.\n", CPU_COUNT(&allcpuset));

    // initialize the sequencer semaphores
    //
    if (sem_init (&semS1, 0, 0)) { printf ("Failed to initialize S1 semaphore\n"); exit (-1); }
    if (sem_init (&semS2, 0, 0)) { printf ("Failed to initialize S2 semaphore\n"); exit (-1); }
    if (sem_init (&semS3, 0, 0)) { printf ("Failed to initialize S3 semaphore\n"); exit (-1); }
    if (sem_init (&semS4, 0, 0)) { printf ("Failed to initialize S4 semaphore\n"); exit (-1); }

    mainpid=getpid();

    rt_max_prio = sched_get_priority_max(SCHED_FIFO);
    rt_min_prio = sched_get_priority_min(SCHED_FIFO);

    rc=sched_getparam(mainpid, &main_param);
    main_param.sched_priority=rt_max_prio;
    rc=sched_setscheduler(getpid(), SCHED_FIFO, &main_param);
    if(rc < 0) perror("main_param");
    print_scheduler();


    pthread_attr_getscope(&main_attr, &scope);

    if(scope == PTHREAD_SCOPE_SYSTEM)
      printf("PTHREAD SCOPE SYSTEM\n");
    else if (scope == PTHREAD_SCOPE_PROCESS)
      printf("PTHREAD SCOPE PROCESS\n");
    else
      printf("PTHREAD SCOPE UNKNOWN\n");

    printf("rt_max_prio=%d\n", rt_max_prio);
    printf("rt_min_prio=%d\n", rt_min_prio);    

    for(i=0; i < NUM_THREADS; i++)
    {

      rc=pthread_attr_init(&rt_sched_attr[i]);
      rc=pthread_attr_setinheritsched(&rt_sched_attr[i], PTHREAD_EXPLICIT_SCHED);
      rc=pthread_attr_setschedpolicy(&rt_sched_attr[i], SCHED_FIFO);

      rt_param[i].sched_priority=rt_max_prio-i;
      pthread_attr_setschedparam(&rt_sched_attr[i], &rt_param[i]);

      threadParams[i].threadIdx=i;
    }


    
    //Set affinity of thread to CPU and create threads
    //Socket thread
    CPU_ZERO(&threadcpu);
	CPU_SET(2, &threadcpu);
	rc=pthread_attr_setaffinity_np(&rt_sched_attr[4], sizeof(cpu_set_t), &threadcpu);
    if(rc)
    {
        printf("\n\rpthread_attr_setaffinity() failed for Socket thread");    
    }
	rt_param[4].sched_priority=rt_max_prio-1;
	pthread_attr_setschedparam(&rt_sched_attr[4], &rt_param[4]);
	
	/* Create Thread */
	rc=pthread_create(&threads[4], &rt_sched_attr[4], Socket_thread, (void *)&(threadParams[4]));
	if(rc < 0)
    {
		perror("Socket thread failed");
    }
    else
    {
		printf("Image capture thread success\n");
    }

    //Image dump thread
    CPU_ZERO(&threadcpu);
	CPU_SET(2, &threadcpu);
	rc=pthread_attr_setaffinity_np(&rt_sched_attr[3], sizeof(cpu_set_t), &threadcpu);
    if(rc)
    {
        printf("\n\rpthread_attr_setaffinity() failed for Image dump thread");   
    }
	rt_param[3].sched_priority=rt_max_prio-1;
	pthread_attr_setschedparam(&rt_sched_attr[3], &rt_param[3]);
	
	/* Create Thread */
	rc=pthread_create(&threads[3], &rt_sched_attr[3], Image_dump_thread, (void *)&(threadParams[3]));
	if(rc < 0)
    {
		perror("Socket thread failed");
    }
    else
    {
		printf("Image capture thread success\n");
    }

    //Image capture thread
	CPU_ZERO(&threadcpu);
	CPU_SET(3, &threadcpu);
	rc=pthread_attr_setaffinity_np(&rt_sched_attr[2], sizeof(cpu_set_t), &threadcpu);
    if(rc)
    {
        printf("\n\rpthread_attr_setaffinity() failed for Image capture thread");    
    }
	rt_param[2].sched_priority=rt_max_prio-1;
	pthread_attr_setschedparam(&rt_sched_attr[2], &rt_param[2]);
	
	/* Create Thread */
	rc=pthread_create(&threads[2], &rt_sched_attr[2], Image_capture_thread, (void *)&(threadParams[2]));
	if(rc < 0)
    {
		perror("Image capture thread failed");
    }
    else
    {
		printf("Image capture thread success\n");
    }

    //Image store
    CPU_ZERO(&threadcpu);
	CPU_SET(3, &threadcpu);
	rc=pthread_attr_setaffinity_np(&rt_sched_attr[1], sizeof(cpu_set_t), &threadcpu);
	if(rc)
    {
        printf("\n\rpthread_attr_setaffinity() failed for Image store");
        
    }
    rt_param[1].sched_priority=rt_max_prio-2;
	pthread_attr_setschedparam(&rt_sched_attr[1], &rt_param[1]);
	
	/* Create Thread */
	rc=pthread_create(&threads[1], &rt_sched_attr[1], Image_store_thread, (void *)&(threadParams[1]));
	if(rc < 0)
    {
		perror("Image store thread failed");
    }
    else
    {
		printf("Image store thread success\n");
    }
 
    //Sequencer
    CPU_ZERO(&threadcpu);
	CPU_SET(3, &threadcpu);
	rc=pthread_attr_setaffinity_np(&rt_sched_attr[0], sizeof(cpu_set_t), &threadcpu);
    if(rc)
    {
        printf("\n\rpthread_attr_setaffinity() failed for sequencer");

    }
	rt_param[0].sched_priority=rt_max_prio;
	pthread_attr_setschedparam(&rt_sched_attr[0], &rt_param[0]);
	
	/* Create Thread */
	rc=pthread_create(&threads[0], &rt_sched_attr[0], Sequencer, (void *)&(threadParams[0]));
	if(rc < 0)
    {
		perror("Image store thread failed");
    }
    else
    {
		printf("Image store thread success\n");
    }

    return 0;
}

void print_scheduler(void)
{
   int schedType;

   schedType = sched_getscheduler(getpid());

   switch(schedType)
   {
       case SCHED_FIFO:
           printf("Pthread Policy is SCHED_FIFO\n");
           break;
       case SCHED_OTHER:
           printf("Pthread Policy is SCHED_OTHER\n"); exit(-1);
         break;
       case SCHED_RR:
           printf("Pthread Policy is SCHED_RR\n"); exit(-1);
           break;
       default:
           printf("Pthread Policy is UNKNOWN\n"); exit(-1);
   }
}