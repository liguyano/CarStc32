#include <STC32G.H>
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library        
#include <INTRINS.H>
#include "SSS.h"
//****************************************
// ¶¨Òå51µ¥Æ¬»ú¶Ë¿Ú
//****************************************
#define DataPort P0                //LCD1602Êý¾Ý¶Ë¿Ú
#define    SCL P25                        //IICÊ±ÖÓÒý½Å¶¨Òå
#define    SDA P24                        //IICÊý¾ÝÒý½Å¶¨Òå

//****************************************
// ¶¨ÒåMPU6050ÄÚ²¿µØÖ·
//****************************************
#define        SMPLRT_DIV                0x19        //ÍÓÂÝÒÇ²ÉÑùÂÊ£¬µäÐÍÖµ£º0x07(125Hz)
#define        CONFIG                        0x1A        //µÍÍ¨ÂË²¨ÆµÂÊ£¬µäÐÍÖµ£º0x06(5Hz)
#define        GYRO_CONFIG                0x1B        //ÍÓÂÝÒÇ×Ô¼ì¼°²âÁ¿·¶Î§£¬µäÐÍÖµ£º0x18(²»×Ô¼ì£¬2000deg/s)
#define        ACCEL_CONFIG        0x1C        //¼ÓËÙ¼Æ×Ô¼ì¡¢²âÁ¿·¶Î§¼°¸ßÍ¨ÂË²¨ÆµÂÊ£¬µäÐÍÖµ£º0x01(²»×Ô¼ì£¬2G£¬5Hz)
#define        ACCEL_XOUT_H        0x3B
#define        ACCEL_XOUT_L        0x3C
#define        ACCEL_YOUT_H        0x3D
#define        ACCEL_YOUT_L        0x3E
#define        ACCEL_ZOUT_H        0x3F
#define        ACCEL_ZOUT_L        0x40
#define        TEMP_OUT_H                0x41
#define        TEMP_OUT_L                0x42
#define        GYRO_XOUT_H                0x43
#define        GYRO_XOUT_L                0x44
#define        GYRO_YOUT_H                0x45
#define        GYRO_YOUT_L                0x46
#define        GYRO_ZOUT_H                0x47
#define        GYRO_ZOUT_L                0x48
#define        PWR_MGMT_1                0x6B        //µçÔ´¹ÜÀí£¬µäÐÍÖµ£º0x00(Õý³£ÆôÓÃ)
#define        WHO_AM_I                        0x75        //IICµØÖ·¼Ä´æÆ÷(Ä¬ÈÏÊýÖµ0x68£¬Ö»¶Á)
#define        SlaveAddress        0xD0        //IICÐ´ÈëÊ±µÄµØÖ·×Ö½ÚÊý¾Ý£¬+1Îª¶ÁÈ¡
//****************************************
//¶¨ÒåÀàÐÍ¼°±äÁ¿
//**************************************** //ÏÔÊ¾Êý×Ö(-511ÖÁ512)µÄ×Ö·ûÊý×é
//int        Temperature,Temp_h,Temp_l;        //ÎÂ¶È¼°¸ßµÍÎ»Êý¾Ý
//****************************************
//º¯ÊýÉùÃ÷
//****************************************
void  delay(unsigned int k);                                                                                //ÑÓÊ±
                                                                      //LCDÊý¾Ý
void  InitMPU6050();                                                                                                        //³õÊ¼»¯MPU6050
void  Delay5us();
void  I2C_Start();
void  I2C_Stop();
void  I2C_SendACK(bit ack);
bit   I2C_RecvACK();
void  I2C_SendByte(uchar dat);
uchar I2C_RecvByte();
uchar Single_ReadI2C(uchar REG_Address);                                                //¶ÁÈ¡I2CÊý¾Ý
void  Single_WriteI2C(uchar REG_Address,uchar REG_data);        //ÏòI2CÐ´ÈëÊý¾Ý
int GetData(uchar REG_Address);
//****************************************