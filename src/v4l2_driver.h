#ifndef V4L2_H_
#define V4L2_H_
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
#define DEVICE_NAME    "/dev/video0"
#define HORIZONTAL_RES (640)
#define VERTICAL_RES   (480)
#define HORIZONTAL_RES_STR "640"
#define VERTICAL_RES_STR   "480"
/****************************************************************************************
*				GLOBAL VARIABLE SECTION
*****************************************************************************************/
extern char ppm_header[200];
extern struct utsname system_info;
extern int size_of_image;
/***************************************************************************************
*				FUNCTION PROTOTYPE
*****************************************************************************************/
void open_device(void);
void init_device(void);
void start_capturing(void);

 void mainloop(void);
void stop_capturing(void);
 void uninit_device(void);
  void close_device(void);

            

#endif //V4L2_H_