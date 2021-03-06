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
#include <stdlib.h>

#define USEC_PER_MSEC (1000)
#define NANOSEC_PER_SEC (1000000000)
/**************************************************************************************
*					GLOBAL VARIABLE
*******************************************************************************************/
unsigned char image_store[60][640*480*3];
struct timeval prev_time_val;
int abortTest=0;
double* image_cap_start_time_buffer = NULL;
double* image_cap_end_time_buffer = NULL;
double* image_cap_execution_time_buffer = NULL;
double* image_store_start_time_buffer = NULL;
double* image_store_end_time_buffer = NULL;
double* image_store_execution_time_buffer = NULL;
double* sequencer_start_time_buffer = NULL;
double* sequencer_end_time_buffer = NULL;
double* sequencer_execution_time_buffer = NULL;
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
    // struct timeval time_val;
    // gettimeofday(&time_val, (struct timezone *)0);
    // *timestamp = ((double)time_val.tv_sec + (double)(time_val.tv_usec/USEC_PER_MSEC));

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
        // printf("\n\rWaiting for semaphore s3");
        sem_wait(&semS3);
        // printf("\n\rImage dump start");

        get_timestamp(&start_time); //get start time
        #ifdef IMG_DMP_DBG
        printf("\n\r[Thread3]   Start time %lf",start_time);
        #endif

        dump_ppm( (image_store + (frame_dump_cnt % 60)), ((size_of_image*6)/4), frame_dump_cnt, &frame_time);


        // get_timestamp(&end_time); //get end time
        // printf("\n\r Stop time %lf",end_time);

        frame_dump_cnt++;
        // printf("\n\rImage dump end");
    }

    pthread_exit((void *)0);
}

void* Image_capture_thread(void* params)
{
    printf("\n\rImage capture thread run");
    double start_time,end_time;
    unsigned long frame_number = 0;
    image_cap_start_time_buffer = malloc(sizeof(double) * total_frames);
    if(image_cap_start_time_buffer == NULL)
    {
        printf("\n\rimage_cap_start_time_buffer failed");
    }
    image_cap_end_time_buffer = malloc(sizeof(double) * total_frames);
    if(image_cap_end_time_buffer == NULL)
    {
        printf("\n\rimage_cap_end_time_buffer failed");
    }
    image_cap_execution_time_buffer = malloc(sizeof(double) * total_frames);
    if(image_cap_execution_time_buffer == NULL)
    {
        printf("\n\rimage_cap_execution_time_buffer failed");
    }

    while(!abortS1)
    {
       // printf("\n\rWaitinf for semaphore s1");
        sem_wait(&semS1);
        //printf("\n\rImage capture start");

        get_timestamp(&start_time); //get start time
        *(image_cap_start_time_buffer + frame_number) = start_time; 
       #ifdef IMG_CAP_DBG
       printf("\n\r[Thread1]   Start time %lf",start_time);
       #endif
       mainloop(); //Read frame and convert it to RGB

       get_timestamp(&end_time); //get end time
       *(image_cap_end_time_buffer + frame_number) = end_time;
        //printf("\n\r Stop time %lf",end_time);

        frame_number++;

        if(frame_number == total_frames)
        {
            break;
        }

       // printf("\n\rImage capture end");
    }

    pthread_exit((void *)0);
}


void* Image_store_thread(void* params)
{
    printf("\n\rImage store thread run");
    double start_time,end_time;
    unsigned long frame_store_cnt = 0;
    image_store_start_time_buffer = malloc(sizeof(double) * total_frames);
    if(image_store_start_time_buffer == NULL)
    {
        printf("\n\rimage_store_start_time_buffer failed");
    }

    image_store_end_time_buffer = malloc(sizeof(double) * total_frames);
     if(image_store_end_time_buffer == NULL)
    {
        printf("\n\rimage_store_end_time_buffer failed");
    }

    image_store_execution_time_buffer = malloc(sizeof(double) * total_frames);
     if(image_store_execution_time_buffer == NULL)
    {
        printf("\n\rimage_store_execution_time_buffer failed");
    }

    while(frame_store_cnt < total_frames)
    {
        //printf("\n\rWaiting for semaphore s2");
        sem_wait(&semS2);
        //printf("\n\rImage store start");

        get_timestamp(&start_time); //get start time
        *(image_store_start_time_buffer + frame_store_cnt) = start_time;
        #ifdef IMG_STORE_DBG 
        printf("\n\r[Thread2]   Start time %lf",start_time);
        #endif
       
        //Copy image from Big buffer into circular buffer(big buffer is populated in image capture thread) 
		for(int i=0;i<(640*480*3);i++)
		{
			image_store[frame_store_cnt % 60][i] = bigbuffer[i];
		}

        get_timestamp(&end_time); //get end time
        *(image_store_end_time_buffer + frame_store_cnt) = end_time;
       // printf("\n\r Stop time %lf",end_time);

        frame_store_cnt++;
        sem_post(&semS3);

       // printf("\n\rImage store end");
    }

    pthread_exit((void *)0);
}

void* Sequencer(void* params)
{
    printf("\n\rSequencer thread run");
    sequencer_start_time_buffer = malloc(sizeof(double) * total_frames);
    if(sequencer_start_time_buffer == NULL)
    {
        printf("\n\rsequencer_start_time_buffer failed");
    }
    
    sequencer_end_time_buffer = malloc(sizeof(double) * total_frames);
    if(sequencer_end_time_buffer == NULL)
    {
        printf("\n\rsequencer_end_time_buffer failed");
    }
    
    sequencer_execution_time_buffer = malloc(sizeof(double) * total_frames);
    if(sequencer_execution_time_buffer == NULL)
    {
        printf("\n\rsequencer_execution_time_buffer failed");
    }
    if(freq == 1)
    {
        printf("\n\rRunning for frequency %d",freq);
        struct timeval current_time_val;
        struct timespec remaining_time;
        struct timespec set_time_1hz;		
        struct timespec reference_time_1hz;	
        double start_time, end_time;
        double residual;
        int rc, delay_cnt=0;
        unsigned long long seqCnt=0;
        threadParams_t *threadParams = (threadParams_t *)params;

        int j =1;
        clock_gettime(CLOCK_REALTIME, &reference_time_1hz);
        set_time_1hz = reference_time_1hz;
        do
        {
            delay_cnt=0; residual=0.0;
            get_timestamp(&start_time); //get start time 
            *(sequencer_start_time_buffer + seqCnt) = start_time;
            do
            {

                set_time_1hz.tv_sec +=1;

                clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &set_time_1hz, NULL);

                if(rc == EINTR)
                { 
                    residual = remaining_time.tv_sec + ((double)remaining_time.tv_nsec / (double)NANOSEC_PER_SEC);

                    if(residual > 0.0) printf("residual=%lf, sec=%d, nsec=%d\n", residual, (int)remaining_time.tv_sec, (int)remaining_time.tv_nsec);
    
                    delay_cnt++;
                }
                else if(rc < 0)
                {
                    perror("Sequencer nanosleep");
                    exit(-1);
                }
            
            } while((residual > 0.0) && (delay_cnt < 100));
            
            #ifdef SEQ_DEBUG
            printf( "\n\r[Sequencer] Start time %lf, frame count %d", ((double)current_time_val.tv_sec + (double)(current_time_val.tv_usec/USEC_PER_MSEC)),seqCnt);
            #endif

            if(delay_cnt > 1)
            {
                printf("Sequencer looping delay %d\n", delay_cnt);
            }

            // Release each service at a sub-rate of the generic sequencer rate

            // Servcie_1 = RT_MAX-1	@ 1 Hz
            if((seqCnt % 1) == 0) sem_post(&semS1);

            // Service_2 = RT_MAX-2	@ 1 Hz
            if((seqCnt % 1) == 0) sem_post(&semS2);

             get_timestamp(&end_time); //get end time
            *(sequencer_end_time_buffer + seqCnt) = end_time;
            seqCnt++;

        } while(!abortTest && (seqCnt < total_frames));

        sem_post(&semS1); 
        sem_post(&semS2);
        abortS1=1;
        abortS2=1; 
        pthread_exit((void *)0);
    }
	else if(freq == 10)
	{
        printf("\n\rRunning sequencer for %d",freq);
		struct timeval current_time_val;		//To store the current time value
		struct timespec remaining_time;		
		struct timespec measure_time;
		struct timespec reference_time;		//To store the reference time at which sequencer starts
		struct timespec set_time;			//To store time for clock_nanosleep()
		double start_time,end_time;
		double residual;
		int rc, delay_cnt=0;
		unsigned long long seqCnt=0;
		threadParams_t *threadParams = (threadParams_t *)params;

		int i = 1;
		do
		{
			delay_cnt=0; residual=0.0;
            get_timestamp(&start_time);
            *(sequencer_start_time_buffer + seqCnt) = start_time;
			do
			{
				//syslog(LOG_INFO,"SEQUENCER");
				if(i == 1)
				{
					clock_gettime(CLOCK_REALTIME, &reference_time);
					set_time = reference_time;
					syslog(LOG_INFO,"REFERENCE TIME: %lf seconds\n",((double)reference_time.tv_sec + (double)((reference_time.tv_nsec)/(double)1000000000)));
					i++;
				}

				/* Wrap around condition */
				if(set_time.tv_nsec > 900000000)
				{
					set_time.tv_sec++;
					set_time.tv_nsec = set_time.tv_nsec - 900000000;
				}

				else
				{
					set_time.tv_nsec += 100000000; //set time for 200ms sleeo
				}


				clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &set_time, NULL);




				if(rc == EINTR)
				{
					residual = remaining_time.tv_sec + ((double)remaining_time.tv_nsec / (double)NANOSEC_PER_SEC);

					if(residual > 0.0) printf("residual=%lf, sec=%d, nsec=%d\n", residual, (int)remaining_time.tv_sec, (int)remaining_time.tv_nsec);

					delay_cnt++;
				}
				else if(rc < 0)
				{
					perror("Sequencer nanosleep");
					exit(-1);
				}

			} while((residual > 0.0) && (delay_cnt < 100));

			//seqCnt++;

			if(delay_cnt > 1) printf("Sequencer looping delay %d\n", delay_cnt);


			// Release each service at a sub-rate of the generic sequencer rate

			// Servcie_1 
			if((seqCnt % 1) == 0) sem_post(&semS1);

			// Service_2 
			if((seqCnt % 1) == 0) sem_post(&semS2);

            get_timestamp(&end_time);
            *(sequencer_end_time_buffer + seqCnt) = end_time;
            seqCnt++;       //Increment the sequencer count



		} while(!abortTest && (seqCnt < (total_frames)));

		sem_post(&semS1);	//Sem post service 1 (image capture)
		sem_post(&semS2);	//Sem post service 2 (image dump)
		abortS1=1;		//abort service 1


		pthread_exit((void *)0);	//exit sequencer
	}

}