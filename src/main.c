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

    for (;;)
    {
        int idx;
        int c;

        c = getopt_long(argc, argv,
                    short_options, long_options, &idx);

        if (-1 == c)
            break;

        switch (c)
        {
            case 0: /* getopt_long() flag */
                break;

            case 'd':
                dev_name = optarg;
                break;

            case 'h':
                usage(stdout, argc, argv);
                exit(EXIT_SUCCESS);

            case 'm':
                io = IO_METHOD_MMAP;
                break;

            case 'r':
                io = IO_METHOD_READ;
                break;

            case 'u':
                io = IO_METHOD_USERPTR;
                break;

            case 'o':
                out_buf++;
                break;

            case 'f':
                force_format++;
                break;

            case 'c':
                errno = 0;
                frame_count = strtol(optarg, NULL, 0);
                if (errno)
                        errno_exit(optarg);
                break;

            default:
                usage(stderr, argc, argv);
                exit(EXIT_FAILURE);
        }
    }

    open_device();
    init_device();
    start_capturing();
    mainloop();
    stop_capturing();
    uninit_device();
    close_device();
    fprintf(stderr, "\n");
    return 0;
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