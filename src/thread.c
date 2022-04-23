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
}

void* Image_capture_thread(void* params)
{
    printf("\n\rImage capture thread run");
    double start_time,end_time;
    unsigned long frame_number = 0;
    while(!abortS1)
    {
        sem_wait(&semS1);
        printf("\n\rImage capture start");

        get_timestamp(&start_time); //get start time
        printf("\n\rStart time %lf",start_time);
        mainloop(); //Read frame and convert it to RGB

        get_timestamp(&end_time); //get end time
        printf("\n\r Stop time %lf",stop_time);

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
}

void* Sequencer(void* params)
{
    printf("\n\rSequencer thread run");
}