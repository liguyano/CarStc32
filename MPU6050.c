//***************²âÊÔÆ½Ì¨-BST V51*************************//

//****************************************

//ÕûÊý×ª×Ö·û´®
//****************************************
#include "MPU6050.h"

//****************************************
//ÑÓÊ±
//****************************************
void delay(unsigned int k)
{
    unsigned int i,j;
    for(i=0;i<k;i++)
    {
        for(j=0;j<121;j++);
    }
}
//****************************************
//LCD1602³õÊ¼»¯
//****************************************

//****************************************
//LCD1602Ð´ÔÊÐí

//****************************************
//LCD1602Ð´ÈëÃüÁî
//****************************************

//**************************************
//ÑÓÊ±5Î¢Ãë(STC90C52RC@12M)
//²»Í¬µÄ¹¤×÷»·¾³,ÐèÒªµ÷Õû´Ëº¯Êý
//µ±¸ÄÓÃ1TµÄMCUÊ±,Çëµ÷Õû´ËÑÓÊ±º¯Êý
//**************************************
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
}
//**************************************
//I2CÆðÊ¼ÐÅºÅ
//**************************************
void I2C_Start()
{
    SDA = 1;                    //À­¸ßÊý¾ÝÏß
    SCL = 1;                    //À­¸ßÊ±ÖÓÏß
    Delay5us();                 //ÑÓÊ±
    SDA = 0;                    //²úÉúÏÂ½µÑØ
    Delay5us();                 //ÑÓÊ±
    SCL = 0;                    //À­µÍÊ±ÖÓÏß
}
//**************************************
//I2CÍ£Ö¹ÐÅºÅ
//**************************************
void I2C_Stop()
{
    SDA = 0;                    //À­µÍÊý¾ÝÏß
    SCL = 1;                    //À­¸ßÊ±ÖÓÏß
    Delay5us();                 //ÑÓÊ±
    SDA = 1;                    //²úÉúÉÏÉýÑØ
    Delay5us();                 //ÑÓÊ±
}
//**************************************
//I2C·¢ËÍÓ¦´ðÐÅºÅ
//Èë¿Ú²ÎÊý:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(bit ack)
{
    SDA = ack;                  //Ð´Ó¦´ðÐÅºÅ
    SCL = 1;                    //À­¸ßÊ±ÖÓÏß
    Delay5us();                 //ÑÓÊ±
    SCL = 0;                    //À­µÍÊ±ÖÓÏß
    Delay5us();                 //ÑÓÊ±
}
//**************************************
//I2C½ÓÊÕÓ¦´ðÐÅºÅ
//**************************************
bit I2C_RecvACK()
{
    SCL = 1;                    //À­¸ßÊ±ÖÓÏß
    Delay5us();                 //ÑÓÊ±
    CY = SDA;                   //¶ÁÓ¦´ðÐÅºÅ
    SCL = 0;                    //À­µÍÊ±ÖÓÏß
    Delay5us();                 //ÑÓÊ±
    return CY;
}
//**************************************
//ÏòI2C×ÜÏß·¢ËÍÒ»¸ö×Ö½ÚÊý¾Ý
//**************************************
void I2C_SendByte(uchar dat)
{
    uchar i;
    for (i=0; i<8; i++)         //8Î»¼ÆÊýÆ÷
    {
        dat <<= 1;              //ÒÆ³öÊý¾ÝµÄ×î¸ßÎ»
        SDA = CY;               //ËÍÊý¾Ý¿Ú
        SCL = 1;                //À­¸ßÊ±ÖÓÏß
        Delay5us();             //ÑÓÊ±
        SCL = 0;                //À­µÍÊ±ÖÓÏß
        Delay5us();             //ÑÓÊ±
    }
    I2C_RecvACK();
}
//**************************************
//´ÓI2C×ÜÏß½ÓÊÕÒ»¸ö×Ö½ÚÊý¾Ý
//**************************************
uchar I2C_RecvByte()
{
    uchar i;
    uchar dat = 0;
    SDA = 1;                    //Ê¹ÄÜÄÚ²¿ÉÏÀ­,×¼±¸¶ÁÈ¡Êý¾Ý,
    for (i=0; i<8; i++)         //8Î»¼ÆÊýÆ÷
    {
        dat <<= 1;
        SCL = 1;                //À­¸ßÊ±ÖÓÏß
        Delay5us();             //ÑÓÊ±
        dat |= SDA;             //¶ÁÊý¾Ý               
        SCL = 0;                //À­µÍÊ±ÖÓÏß
        Delay5us();             //ÑÓÊ±
    }
    return dat;
}
//**************************************
//ÏòI2CÉè±¸Ð´ÈëÒ»¸ö×Ö½ÚÊý¾Ý
//**************************************
void Single_WriteI2C(uchar REG_Address,uchar REG_data)
{
    I2C_Start();                  //ÆðÊ¼ÐÅºÅ
    I2C_SendByte(SlaveAddress);   //·¢ËÍÉè±¸µØÖ·+Ð´ÐÅºÅ
    I2C_SendByte(REG_Address);    //ÄÚ²¿¼Ä´æÆ÷µØÖ·£¬
    I2C_SendByte(REG_data);       //ÄÚ²¿¼Ä´æÆ÷Êý¾Ý£¬
    I2C_Stop();                   //·¢ËÍÍ£Ö¹ÐÅºÅ
}
//**************************************
//´ÓI2CÉè±¸¶ÁÈ¡Ò»¸ö×Ö½ÚÊý¾Ý
//**************************************
uchar Single_ReadI2C(uchar REG_Address)
{
    uchar REG_data;
    I2C_Start();                   //ÆðÊ¼ÐÅºÅ
    I2C_SendByte(SlaveAddress);    //·¢ËÍÉè±¸µØÖ·+Ð´ÐÅºÅ
    I2C_SendByte(REG_Address);     //·¢ËÍ´æ´¢µ¥ÔªµØÖ·£¬´Ó0¿ªÊ¼
    I2C_Start();                   //ÆðÊ¼ÐÅºÅ
    I2C_SendByte(SlaveAddress+1);  //·¢ËÍÉè±¸µØÖ·+¶ÁÐÅºÅ
    REG_data=I2C_RecvByte();       //¶Á³ö¼Ä´æÆ÷Êý¾Ý
    I2C_SendACK(1);                //½ÓÊÕÓ¦´ðÐÅºÅ
    I2C_Stop();                    //Í£Ö¹ÐÅºÅ
    return REG_data;
}
//**************************************
//³õÊ¼»¯MPU6050
//**************************************
void InitMPU6050()
{
    Single_WriteI2C(PWR_MGMT_1, 0x00);        //½â³ýÐÝÃß×´Ì¬
    Single_WriteI2C(SMPLRT_DIV, 0x07);
    Single_WriteI2C(CONFIG, 0x06);
    Single_WriteI2C(GYRO_CONFIG, 0x18);
    Single_WriteI2C(ACCEL_CONFIG, 0x01);
}
//**************************************
//ºÏ³ÉÊý¾Ý
//**************************************
int GetData(uchar REG_Address)
{
    char H,L;
    H=Single_ReadI2C(REG_Address);
    L=Single_ReadI2C(REG_Address+1);
    return (H<<8)+L;   //ºÏ³ÉÊý¾Ý
}
//**************************************
//ÔÚ1602ÉÏÏÔÊ¾10Î»Êý¾Ý
//**************************************