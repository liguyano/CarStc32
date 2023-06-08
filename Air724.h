//
// Created by kurum on 西暦2023年6月7日.
//

#ifndef STC32_AIR724_H
#define STC32_AIR724_H

#include "STC32G.H"
#include "SSS.h"
#include "STC32G_UART.h"
#include "STC32G_Delay.h"
#include <STDIO.H>
#define Serve "43.136.94.231"
#define port "7001"
void connect();
uchar connect2Tcp();
void SendToTcp(uchar * str);
void SendAChar(uchar s);
#endif //STC32_AIR724_H
