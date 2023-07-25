#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W24Q64_Init(void)
{
	MySPI_Init();
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID) // 读ID
{
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_JEDEC_ID); // 读ID号
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE); // 无意义，置换会ID号
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	
	MySPI_Stop();
}

void W25Q64_WriteEnable(void) // 写使能
{
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout = 100000;
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
			break;
	}
	
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count) // 页编程
{
	uint16_t i;
	W25Q64_WriteEnable();
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	
	MySPI_Stop();
	W25Q64_WaitBusy(); // 事后等待
}

void W25Q64_SectorErase(uint32_t Address) // 扇区擦除
{
	W25Q64_WriteEnable();
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	MySPI_Stop();
	W25Q64_WaitBusy(); // 事后等待
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count) // 读取数据
{
	uint32_t i;
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	
	MySPI_Stop();
}