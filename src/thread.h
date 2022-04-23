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
/****************************************************************************************
*				     MACROS AND ENUMS
*****************************************************************************************/


/****************************************************************************************
*				GLOBAL VARIABLE SECTION
*****************************************************************************************/
extern int abortS1;
extern sem_t semS1, semS2, semS3, semS4, semS5, semS6, semS7;
extern int total_frames;
extern struct timespec frame_time;

/***************************************************************************************
*				FUNCTION PROTOTYPE
*****************************************************************************************/
void* Socket_thread(void* );
void* Image_dump_thread(void* );
void* Image_capture_thread(void* );
void* Image_store_thread(void* );
void* Sequencer(void* );

#endif //THREAD_H_