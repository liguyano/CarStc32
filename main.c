/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include <STRING.H>
#include	"config.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"
#include "SHT35.h"
#include "ATC02.h"
#include "Air724.h"
#include "CAR.h"
#include "MPU6050.h"

/*************	����˵��	**************

˫����ȫ˫���жϷ�ʽ�շ�ͨѶ����

ͨ��PC��MCU��������, MCU�յ���ͨ�����ڰ��յ�������ԭ������, Ĭ�ϲ����ʣ�115200,N,8,1.

ͨ������ STC32G_UART.h ͷ�ļ������ UART1~UART4 ���壬������ͬͨ���Ĵ���ͨ�š�

�ö�ʱ���������ʷ�����������ʹ��1Tģʽ(���ǵͲ�������12T)����ѡ��ɱ�������������ʱ��Ƶ�ʣ�����߾��ȡ�

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

/*************	���س�������	**************/


/*************	���ر�������	**************/


/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//��ʼ��
    GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;		//???��??????????IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//???��IO???????��????��???,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//??????
    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;		//???��??????????IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//???��IO???????��????��???,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//??????
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//�ṹ����

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer3;			//ѡ�����ʷ�����, BRT_Timer3, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
    UART_Configuration(UART3, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
    NVIC_UART3_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    UART3_SW(UART3_SW_P00_P01);		//UART3_SW_P00_P01,UART3_SW_P50_P51

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//????, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//?????��????��??��?��, BRT_Timer1, BRT_Timer2 (��???: ????2???��????BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//?��????, ??��? 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//???????��,   ENABLE?��DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//?��??????��?, ENABLE?��DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//??????????1 UART1,UART2,UART3,UART4
    NVIC_UART1_Init(ENABLE,Priority_1);		//????????, ENABLE/DISABLE; ??????(??????) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P36_P37);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/**********************************************/
void main(void)
{
    u8	i;
    uint time;
    int temp,humi,x,y;
    char str[50] ;
    WTST = 0;		//���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
    EAXSFR();		//��չSFR(XFR)����ʹ��
    CKCON = 0;      //��߷���XRAM�ٶ�
    GPIO_config();
    UART_config();
    EA = 1;
    //set P24 and P25 to high

    //TX3_write2buff(4*16+str[0]%16);
    PrintString3("wait answer");
    //connect();
    PrintString3(
            "START");
    InitMPU6050();
    TX3_write2buff('\n');
   // while (!connect2Tcp());
    ATC_WRITE_DATA(0x05,'T');
    SHT3XInit();
    SendToTcp("wwpdsg");
    delay_ms(100);
    SendToTcp("wwpdsg");
    delay_ms(100);
    SendToTcp("wwpdsg");
    delay_ms(100);
    while (1)
    {
        time++;
        if(time>10000)
        {
            SendToTcp("OK");
            time=0;
        }
        delay_ms(1);
        //PrintString3("STC32G UART3 Test Programme!\r\n");
        if(COM1.RX_TimeOut > 0)		//��ʱ����
        {
            if(--COM1.RX_TimeOut == 0)
            {
                if(COM1.RX_Cnt > 0)
                {
                    for(i=0; i<COM1.RX_Cnt; i++)
                    {
                        if(RX1_Buffer[i]=='T')
                        {

                            if (strncmp(&RX1_Buffer[i], "TEMP", 4) == 0) {
                                TX3_write2buff(atc_recv_data(5));
                                delay_ms(1);
                                SHT3X_XHGetTempAndHumi(&temp,&humi);
                                sprintf(str,"%d,%d"
                                            "",temp,humi);
                                SendToTcp(str);
                            }
                        }
                        else if(RX1_Buffer[i]=='C')
                        {

                            if (strncmp(&RX1_Buffer[i], "CAR", 3) == 0) {
                                SendToTcp("car");
                                switch (RX1_Buffer[i+3]) {
                                    case 'B':
                                        CARBACK();
                                        break;
                                    case 'W':
                                        CARSTART();
                                        break;
                                    case 'L':
                                        CARLEFT();
                                        break;
                                    case 'R':
                                        CARRIGHT();
                                        break;
                                    case 'C':
                                        CARSTART();
                                        break;
                                }
                            }
                            else if (strncmp(&RX1_Buffer[i], "CLOSE", 5) == 0) {
                                connect();
                                PrintString3("START");
                                TX3_write2buff('\n');
                                while (!connect2Tcp());
                            }
                        }
                        TX3_write2buff(RX1_Buffer[i]);
                    }


                }
                COM1.RX_Cnt = 0;
            }
        }
        if(COM3.RX_TimeOut > 0)		//???��????
        {
            if(--COM3.RX_TimeOut == 0)
            {
                if(COM3.RX_Cnt > 0)
                {
                    PrintString1("AT+CIPSEND\r");
                    for(i=0; i<COM3.RX_Cnt; i++)
                    {
                        TX1_write2buff(RX3_Buffer[i]);
                        TX3_write2buff(RX3_Buffer[i]);
                    }
                    TX1_write2buff(0x1A);
                }
                sprintf(str,"x:%d,y:%d",GetData(GYRO_XOUT_H), GetData(GYRO_YOUT_L));
                PrintString3(str);
                COM3.RX_Cnt = 0;
            }
        }
    }
}



