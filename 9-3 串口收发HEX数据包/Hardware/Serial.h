#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];


void Serial_Init(void); // 初始化
void Serial_SendByte(uint8_t Byte); // 字节
void Serial_SendArray(uint8_t *Array, uint16_t Length); // 数组
void Serial_SendString(char *String); // 字符串
void Serial_SendNumber(uint32_t Number, uint8_t Length); // 字符数字
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);
void Serial_SendPacket(void);

#endif
