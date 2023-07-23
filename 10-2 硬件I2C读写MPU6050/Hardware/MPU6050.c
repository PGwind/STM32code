#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT) // 解决while超时
{
	uint32_t Timeout;
	Timeout = 10000; 
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) // EV5事件
	{
		Timeout --;
		if (Timeout == 0)
		{
			// 可以添加信息打印等 try catch throw
			break;
		}
	}
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	
	
	I2C_GenerateSTART(I2C2, ENABLE); // 开始
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); // 发送
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // EV6
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // EV8
	
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE); // 终止
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress) // 读取
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE); // 开始
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // EV5事件
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); // 发送
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // EV6
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // EV8
	
	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	// 接收之前提前设置ACK和STOP
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C2);
	// ---------------------------------------
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return Data;
}

// 读取多个字节
/*
		uint8_t Data[length]; // 声明一个数组用于存储读取的多个字节数据
		MyI2C_Start();
		MyI2C_SendByte(MPU6050_ADDRESS); // 设备地址
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress); // 寄存器地址
		MyI2C_ReceiveAck();

		MyI2C_Start();
		MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 读指令
		MyI2C_ReceiveAck();

		for (int i = 0; i < length; i++) {
				Data[i] = MyI2C_ReceiveByte();
				if (i == length - 1) {
						MyI2C_SendAck(1); // 最后一个字节，不给从机应答，即停止读取
				} else {
						MyI2C_SendAck(0); // 给从机应答，继续读取下一个字节
				}
			}

		MyI2C_Stop();
*/


void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit ;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; // 不作为从机，随便给个地址(不冲突)
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 电源管理寄存器1 解除睡眠、选择X轴陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 采用分频10
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06); // 配置寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪配置寄存器 最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度 最大量程
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
												int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	// 加速度寄存器 XYZ轴
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H); // 高8位
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); // 低8位
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L); 
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L); 
	*AccZ = (DataH << 8) | DataL;
	
	// 陀螺仪
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L); 
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L); 
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H); 
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L); 
	*GyroZ = (DataH << 8) | DataL;
}
