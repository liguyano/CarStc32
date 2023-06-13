//
// Created by kurum on 西暦2023年6月7日.
//

#include <STRING.H>
#include "Air724.h"
/**
 * @brief senf a AT to the air724 per 200 millms
 * @param no
 *
 * */
void connect()
{
    uchar i;
    while (1)
    {
        PrintString1("AT\r");
        for ( i = 0; i < 200; ++i) {
            if(COM1.RX_TimeOut > 0)		//???±????
            {
                if(--COM1.RX_TimeOut == 0)
                {
                    if(COM1.RX_Cnt > 0)
                    {
                        for(i=0; i<COM1.RX_Cnt; i++)
                        {

                            if(RX1_Buffer[i]==0x4f)
                            {
                                if (RX1_Buffer[i+1]== 0x4b)
                                {
                                    goto out;
                                }
                            }
                        }
                    }
                    COM1.RX_Cnt = 0;
                }
            }
            delay_ms(1);
        }
        }
    out:
    return;

}
/**
 * @brief used to connect to the Tcp Serves
 * @param none
 * @return return 1 when connected else return 0
 * */
uchar connect2Tcp()
{
        uchar i;
        PrintString1("AT+CIPSTART=TCP,43.136.94.231,7001\r");
        for ( i = 0; i < 200; ++i) {
            if(COM1.RX_TimeOut > 0)		//???±????
            {
                if(--COM1.RX_TimeOut == 0)
                {
                    if(COM1.RX_Cnt > 0)
                    {
                        for(i=0; i<COM1.RX_Cnt; i++)
                        {
                            TX3_write2buff(RX1_Buffer[i]);
                            if (RX1_Buffer[i] == 'C') {
                                // 检查是否为 "CONNECT"
                                if (strncmp(&RX1_Buffer[i], "CONNECT OK", 10) == 0) {
                                    // 执行相应的操作
                                    return 1;  // 返回
                                }
                            }
                        }
                    }
                    COM1.RX_Cnt = 0;
                }
            }
            delay_ms(1);
        }
    PrintString1("AT+CIPSHUT\r");
    delay_ms(1000);
    return 0;
}
void SendToTcp(uchar * str)
{
    PrintString1("AT+CIPSEND\r");
    PrintString1(str);
    TX1_write2buff(0x1A);
}
void SendAChar(uchar s)
{
    PrintString1("AT+CIPSEND=1\r");
    TX1_write2buff(s);
    TX1_write2buff('\r');
}