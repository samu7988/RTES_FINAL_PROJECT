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
#include <stdio.h>
#include "thread.h"
#include "v4l2_driver.h"
/**************************************************************************************
*					GLOBAL VARIABLE
*******************************************************************************************/
unsigned char image_store[60][640*480*3];
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

void get_timestamp(double *timestamp)
{
    struct timespec time_val;
    clock_gettime(CLOCK_REALTIME,&time_val);
    *timestamp = ((double)time_val.tv_sec + (double)((time_val.tv_nsec)/(double)1000000000));
}


void* Socket_thread(void* params)
{
    printf("\n\rSocket thread run");
}

void* Image_dump_thread(void* params)
{
     printf("\n\rImage dump thread run");
    double start_time,end_time;
    unsigned long frame_dump_cnt = 0;
    while(frame_dump_cnt < total_frames)
    {
        printf("\n\rWaiting for semaphore s3");
        sem_wait(&semS3);
        printf("\n\rImage dump start");

        get_timestamp(&start_time); //get start time
        printf("\n\rStart time %lf",start_time);

        dump_ppm( (image_store + (frame_dump_cnt % 60)), ((size_of_image*6)/4), frame_dump_cnt, &frame_time);


        get_timestamp(&end_time); //get end time
        printf("\n\r Stop time %lf",end_time);

        frame_dump_cnt++;
        printf("\n\rImage dump end");
    }

    pthread_exit((void *)0);
}

void* Image_capture_thread(void* params)
{
    printf("\n\rImage capture thread run");
    double start_time,end_time;
    unsigned long frame_number = 0;
    while(!abortS1)
    {
        printf("\n\rWaitinf for semaphore s1");
        sem_wait(&semS1);
        printf("\n\rImage capture start");

        get_timestamp(&start_time); //get start time
        printf("\n\rStart time %lf",start_time);
        mainloop(); //Read frame and convert it to RGB

        get_timestamp(&end_time); //get end time
        printf("\n\r Stop time %lf",end_time);

        frame_number++;

        if(frame_number == total_frames)
        {
            break;
        }

        printf("\n\rImage capture end");
    }

    pthread_exit((void *)0);
}


void* Image_store_thread(void* params)
{
    printf("\n\rImage store thread run");
    double start_time,end_time;
    unsigned long frame_store_cnt = 0;
    while(frame_store_cnt < total_frames)
    {
        printf("\n\rWaiting for semaphore s2");
        sem_wait(&semS2);
        printf("\n\rImage store start");

        get_timestamp(&start_time); //get start time
        printf("\n\rStart time %lf",start_time);

        //Copy image from Big buffer into circular buffer(big buffer is populated in image capture thread) 
		for(int i=0;i<(640*480*3);i++)
		{
			image_store[frame_store_cnt % 60][i] = bigbuffer[i];
		}

        get_timestamp(&end_time); //get end time
        printf("\n\r Stop time %lf",end_time);

        frame_store_cnt++;
        sem_post(&semS3);

        printf("\n\rImage store end");
    }

    pthread_exit((void *)0);
}

void* Sequencer(void* params)
{
    printf("\n\rSequencer thread run");
}