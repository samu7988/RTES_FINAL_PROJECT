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
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sched.h>


#define HORIZONTAL_RES (640)
#define VERTICAL_RES   (480)
#define DEVICE_NAME    ("/dev/video0")
#define NUM_CPU_CORES (1)

/**************************************************************************************
*					GLOBAL VARIABLE
*******************************************************************************************/
struct utsname system_info;
cpu_set_t allcpuset;

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

    for(i=0; i < NUM_CPU_CORES; i++)
    {
       CPU_SET(i, &allcpuset);
    }

    printf("Using CPUS=%d from total available.\n", CPU_COUNT(&allcpuset));
    
    return 0;
}
