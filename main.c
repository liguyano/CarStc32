/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_UART.h"
#include	"STC32G_NVIC.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Switch.h"
#include "SHT35.h"
#include "ATC02.h"
#include "SSS.h"
#include "Air724.h"

/*************	功能说明	**************

双串口全双工中断方式收发通讯程序。

通过PC向MCU发送数据, MCU收到后通过串口把收到的数据原样返回, 默认波特率：115200,N,8,1.

通过开启 STC32G_UART.h 头文件里面的 UART1~UART4 定义，启动不同通道的串口通信。

用定时器做波特率发生器，建议使用1T模式(除非低波特率用12T)，并选择可被波特率整除的时钟频率，以提高精度。

下载时, 选择时钟 22.1184MHz (可以在配置文件"config.h"中修改).

******************************************/

/*************	本地常量声明	**************/


/*************	本地变量声明	**************/


/*************	本地函数声明	**************/


/*************  外部函数和变量声明 *****************/


/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	//初始化
    GPIO_InitStructure.Pin  = GPIO_Pin_6 | GPIO_Pin_7;		//???¨??????????IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//???¨IO???????ò????·???,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//??????
    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;		//???¨??????????IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_PullUp;	//???¨IO???????ò????·???,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//??????
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
    COMx_InitDefine		COMx_InitStructure;					//结构定义

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer3;			//选择波特率发生器, BRT_Timer3, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    UART_Configuration(UART3, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
    NVIC_UART3_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART3_SW(UART3_SW_P00_P01);		//UART3_SW_P00_P01,UART3_SW_P50_P51

    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//????, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//?????¨????·??ú?÷, BRT_Timer1, BRT_Timer2 (×???: ????2???¨????BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//?¨????, ??°? 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//???????í,   ENABLE?òDISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//?¨??????±?, ENABLE?òDISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//??????????1 UART1,UART2,UART3,UART4
    NVIC_UART1_Init(ENABLE,Priority_1);		//????????, ENABLE/DISABLE; ??????(??????) Priority_0,Priority_1,Priority_2,Priority_3

    UART1_SW(UART1_SW_P36_P37);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

/**********************************************/
void main(void)
{
    u8	i;
    int temp,humi;
    WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
    EAXSFR();		//扩展SFR(XFR)访问使能
    CKCON = 0;      //提高访问XRAM速度

    GPIO_config();
    UART_config();
    EA = 1;
    //set P24 and P25 to high

    //TX3_write2buff(4*16+str[0]%16);
    PrintString3("wait answer");
    connect();
    PrintString3("START");
    TX3_write2buff('\n');
    while (!connect2Tcp());
    ATC_WRITE_DATA(0x00,'T');
    SHT3XInit();
    while (1)
    {
        delay_ms(1);
        //PrintString3("STC32G UART3 Test Programme!\r\n");
        if(COM1.RX_TimeOut > 0)		//超时计数
        {
            if(--COM1.RX_TimeOut == 0)
            {
                if(COM1.RX_Cnt > 0)
                {
                    for(i=0; i<COM1.RX_Cnt; i++)
                        TX3_write2buff(RX1_Buffer[i]);
                    TX3_write2buff(atc_recv_data(0));
                    SHT3X_XHGetTempAndHumi(&temp,&humi);
                    // PrintString3(str);
                    TX3_write2buff(temp/100+'0');
                    TX3_write2buff(temp%100/10+'0');
                    TX3_write2buff(temp%10+'0');
                    TX3_write2buff(humi/100+'0');
                    TX3_write2buff(humi%100/10+'0');
                    TX3_write2buff(humi%10+'0');
                }
                COM1.RX_Cnt = 0;
            }
        }
        if(COM3.RX_TimeOut > 0)		//???±????
        {
            if(--COM3.RX_TimeOut == 0)
            {
                if(COM3.RX_Cnt > 0)
                {
                    for(i=0; i<COM3.RX_Cnt; i++)
                    {
                        TX1_write2buff(RX3_Buffer[i]);

                    }
                }

                COM3.RX_Cnt = 0;
            }
        }
    }
}



