//
// Created by kurum on 西暦2023年6月7日.
//

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
        PrintString3("AT\r");
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