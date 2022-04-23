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
#include<stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HORIZONTAL_RES (640)
#define VERTICAL_RES   (480)
#define DEVICE_NAME    ()
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
int main(int argc, char** argv)
{
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
    return 0;
}
