#ifndef I2C_H_
#define I2C_H_
/********************************************************************************************
*					FILENAME: .h
**********************************************************************************************/
/* Title: i2c.h
 * Brief:
 * Author: Sayali Mule
 * Date Created: March 09,2022
 * Course: Advanced Embedded Software Development
 

 * Reference[1]:
 * */
/****************************************************************************************
*					HEADER FILE SECTION
*****************************************************************************************/


/****************************************************************************************
*				     MACROS AND ENUMS
*****************************************************************************************/
#define DEVICE_NAME    ("/dev/video0")
#define HORIZONTAL_RES (640)
#define VERTICAL_RES   (480)
#define HORIZONTAL_RES_STR ("640")
#define VERTICAL_RES_STR   ("480")
/****************************************************************************************
*				GLOBAL VARIABLE SECTION
*****************************************************************************************/

/***************************************************************************************
*				FUNCTION PROTOTYPE
*****************************************************************************************/
void open_device(void);
void init_device(void);
void start_capturing(void);
             

#endif //I2C_H_