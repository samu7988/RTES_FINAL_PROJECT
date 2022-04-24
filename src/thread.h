#ifndef THREAD_H_
#define THREAD_H_
/********************************************************************************************
*					FILENAME: .h
**********************************************************************************************/
/* Title: thread.h
 * Brief:
 * Author: Sayali Mule
 * Date Created: March 09,2022
 * Course: Advanced Embedded Software Development
 

 * Reference[1]:
 * */
/****************************************************************************************
*					HEADER FILE SECTION
*****************************************************************************************/
#include <sys/time.h>
#include <time.h>
#include <semaphore.h>
#include <syslog.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/utsname.h>

/****************************************************************************************
*				     MACROS AND ENUMS
*****************************************************************************************/
// #define SEQ_DEBUG (0)
//#define IMG_STORE_DBG (1)
//#define IMG_CAP_DBG (0)
// #define IMG_DMP_DBG (0)
/****************************************************************************************
*				GLOBAL VARIABLE SECTION
*****************************************************************************************/
typedef struct
{
    int threadIdx;
    unsigned long long sequencePeriods;
} threadParams_t;

extern int abortS1,abortS2;
extern sem_t semS1, semS2, semS3, semS4, semS5, semS6, semS7;
extern int total_frames;
extern struct timespec frame_time;
extern unsigned char bigbuffer[(640*480*3)]; 
extern unsigned char image_store[60][640*480*3];
extern double* image_cap_start_time_buffer;
extern double* image_cap_end_time_buffer;
extern double* image_cap_execution_time_buffer;
extern double* image_store_start_time_buffer;
extern double* image_store_end_time_buffer;
extern double* image_store_execution_time_buffer;
extern double* sequencer_start_time_buffer ;
extern double* sequencer_end_time_buffer;
extern double* sequencer_execution_time_buffer;

/***************************************************************************************
*				FUNCTION PROTOTYPE
*****************************************************************************************/
void* Socket_thread(void* );
void* Image_dump_thread(void* );
void* Image_capture_thread(void* );
void* Image_store_thread(void* );
void* Sequencer(void* );

#endif //THREAD_H_