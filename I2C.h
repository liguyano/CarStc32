//
// Created by kurum on 2/20/2023.
//

#ifndef CMAKELISTS_TXT_I2C_H
#define CMAKELISTS_TXT_I2C_H
#include <STC32G.H>
#define SDA P24
#define SCL P25
void one_start();
void one_stop();
void one_sendByte(unsigned char Byte);
unsigned char I2C_ReceiveByte(void);
void I2C_send_ac(unsigned char What);
unsigned char I2C_recv_ack();
#endif //CMAKELISTS_TXT_I2C_H
