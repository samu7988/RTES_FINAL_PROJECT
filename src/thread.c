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
}


void* Image_store_thread(void* params)
{
    printf("\n\rImage store thread run");
}

void* Sequencer(void* params)
{
    printf("\n\rSequencer thread run");
}