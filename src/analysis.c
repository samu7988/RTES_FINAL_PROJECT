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
#include "thread.h"
#include <stdio.h>


#define SEQ_FILE "seq.csv"
#define IMG_CAP_FILE "image_cap.csv"
#define IMG_STORE_FILE "image_store.csv"
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


void analysis(char* file_name, double* start_time, double* end_time, double* execution_time)
{
    FILE* fptr = fopen(file_name, "w+");
    fprintf(fptr, "Frame,Start_time, End_time, Execution_time, Jitter");
    //Execution time
    double wcet = 0;
    double jitter = 0;
    int deadline = 1;
    for(int i = 0; i < total_frames ; i++)
    {
        *(execution_time + i) = (*(end_time + i) - *(start_time + i))*1000;

        if(*(execution_time + i) > wcet)
        {
            wcet = *(execution_time + i);
        
        }
        jitter = (*(start_time + i -1) + deadline) - *(start_time + i);
        fprintf(fptr,"\n%d,%lf,%lf,%lf,%lf",i,*(start_time+i),*(end_time+i),*(execution_time+i),jitter);

    }
    fprintf(fptr,"WCET:%lf",wcet);
    //Jitter
    

    fclose(fptr);
}

void write_seq_csv()
{
    analysis(SEQ_FILE,sequencer_start_time_buffer,sequencer_end_time_buffer, sequencer_execution_time_buffer);
}

void write_img_cap_csv()
{
    analysis(IMG_CAP_FILE,image_cap_start_time_buffer,image_cap_end_time_buffer, image_cap_execution_time_buffer);
}

void write_img_store_csv()
{
    analysis(IMG_STORE_FILE,image_store_start_time_buffer,image_store_end_time_buffer, image_store_execution_time_buffer);
}