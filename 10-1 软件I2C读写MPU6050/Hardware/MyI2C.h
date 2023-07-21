#ifndef __MYI2C_H
#define __MYI2C_H

void MyI2C_Init(void);
void MyI2C_Start(void); // 开始
void MyI2C_Stop(void); // 结束
void MyI2C_SendByte(uint8_t Byte); // 发送
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit); // 发送应答位
uint8_t MyI2C_ReceiveAck(void); // 读取应答位

#endif