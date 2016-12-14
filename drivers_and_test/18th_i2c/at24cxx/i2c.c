/*
 * FILE: i2c.c
 * 用于主机发送/接收
 */
#include <stdio.h>
#include "s3c24xx.h"
#include "i2c.h"

void Delay(int time);

#define WRDATA      (1)
#define RDDATA      (2)

typedef struct tI2C {
    unsigned char *pData;   /* 数据缓冲区 */
    volatile int DataCount; /* 等待传输的数据长度 */
    volatile int Status;    /* 状态 */
    volatile int Mode;      /* 模式：读/写 */
    volatile int Pt;        /* pData中待传输数据的位置 */
}tS3C24xx_I2C, *ptS3C24xx_I2C;

static tS3C24xx_I2C g_tS3C24xx_I2C;

/*
 * I2C初始化
 */
void i2c_init(void)
{
    GPEUP  |= 0xc000;       // 禁止内部上拉
    GPECON |= 0xa0000000;   // 选择引脚功能：GPE15:IICSDA, GPE14:IICSCL

    INTMSK &= ~(BIT_IIC);

    /* bit[7] = 1, 使能ACK
     * bit[6] = 0, IICCLK = PCLK/16
     * bit[5] = 1, 使能中断
     * bit[3:0] = 0xf, Tx clock = IICCLK/16
     * PCLK = 50MHz, IICCLK = 3.125MHz, Tx Clock = 0.195MHz
     */
    IICCON = (1<<7) | (0<<6) | (1<<5) | (0xf);  // 0xaf

    IICADD  = 0x10;     // S3C24xx slave address = [7:1]
    IICSTAT = 0x10;     // I2C串行输出使能(Rx/Tx)
}

/*
 * 主机发送
 * slvAddr : 从机地址，buf : 数据存放的缓冲区，len : 数据长度 
 */
void i2c_write(unsigned int slvAddr, unsigned char *buf, int len)
{
    g_tS3C24xx_I2C.Mode = WRDATA;   // 写操作
    g_tS3C24xx_I2C.Pt   = 0;        // 索引值初始为0
    g_tS3C24xx_I2C.pData = buf;     // 保存缓冲区地址
    g_tS3C24xx_I2C.DataCount = len; // 传输长度
    
    IICDS   = slvAddr;
    IICSTAT = 0xf0;         // 主机发送，启动
    
    /* 等待直至数据传输完毕 */    
    while (g_tS3C24xx_I2C.DataCount != -1);
}
        
/*
 * 主机接收
 * slvAddr : 从机地址，buf : 数据存放的缓冲区，len : 数据长度 
 */
void i2c_read(unsigned int slvAddr, unsigned char *buf, int len)
{
    g_tS3C24xx_I2C.Mode = RDDATA;   // 读操作
    g_tS3C24xx_I2C.Pt   = -1;       // 索引值初始化为-1，表示第1个中断时不接收数据(地址中断)
    g_tS3C24xx_I2C.pData = buf;     // 保存缓冲区地址
    g_tS3C24xx_I2C.DataCount = len; // 传输长度
    
    IICDS        = slvAddr;
    IICSTAT      = 0xb0;    // 主机接收，启动
    
    /* 等待直至数据传输完毕 */    
    while (g_tS3C24xx_I2C.DataCount != 0);
}

/*
 * I2C中断服务程序
 * 根据剩余的数据长度选择继续传输或者结束
 */
void I2CIntHandle(void)
{
    unsigned int iicSt,i;

    // 清中断
    SRCPND = BIT_IIC;
    INTPND = BIT_IIC;
    
    iicSt  = IICSTAT; 

    if(iicSt & 0x8){ printf("Bus arbitration failed\n\r"); }

    switch (g_tS3C24xx_I2C.Mode)
    {    
        case WRDATA:
        {
            if((g_tS3C24xx_I2C.DataCount--) == 0)
            {
                // 下面两行用来恢复I2C操作，发出P信号
                IICSTAT = 0xd0;
                IICCON  = 0xaf;
                Delay(10000);  // 等待一段时间以便P信号已经发出
                break;    
            }

            IICDS = g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++];
            
            // 将数据写入IICDS后，需要一段时间才能出现在SDA线上
            for (i = 0; i < 10; i++);   

            IICCON = 0xaf;      // 恢复I2C传输
            break;
        }

        case RDDATA:
        {
            if (g_tS3C24xx_I2C.Pt == -1)
            {
                // 这次中断是发送I2C设备地址后发生的，没有数据
                // 只接收一个数据时，不要发出ACK信号
                g_tS3C24xx_I2C.Pt = 0;
                if(g_tS3C24xx_I2C.DataCount == 1)
                   IICCON = 0x2f;   // 恢复I2C传输，开始接收数据，接收到数据时不发出ACK
                else 
                   IICCON = 0xaf;   // 恢复I2C传输，开始接收数据
                break;
            }

			g_tS3C24xx_I2C.pData[g_tS3C24xx_I2C.Pt++] = IICDS;
			g_tS3C24xx_I2C.DataCount--;
            
            if (g_tS3C24xx_I2C.DataCount == 0)
            {

                // 下面两行恢复I2C操作，发出P信号
                IICSTAT = 0x90;
                IICCON  = 0xaf;
                Delay(10000);  // 等待一段时间以便P信号已经发出
                break;    
            }      
			else
			{           
	           // 接收最后一个数据时，不要发出ACK信号
	           if(g_tS3C24xx_I2C.DataCount == 1)
	               IICCON = 0x2f;   // 恢复I2C传输，接收到下一数据时无ACK
	           else 
	               IICCON = 0xaf;   // 恢复I2C传输，接收到下一数据时发出ACK
			}
           break;
        }
       
        default:
            break;      
    }
}

/*
 * 延时函数
 */
void Delay(int time)
{
    for (; time > 0; time--);
}

