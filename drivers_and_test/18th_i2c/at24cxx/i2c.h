//====================================================================
// File Name : 2410IIC.h
// Function  : S3C2410 IIC Test Program Head file
// Program   : Shin, On Pil (SOP)
// Date      : March 20, 2002
// Version   : 0.0
// History
//   0.0 : Programming start (March 11, 2002) -> SOP
//====================================================================

#ifndef __2410IIC_H__
#define __2410IIC_H__

void i2c_init(void);
void i2c_write(unsigned int slvAddr, unsigned char *buf, int len);
void i2c_read(unsigned int slvAddr, unsigned char *buf, int len);
void I2CIntHandle(void);

#endif    //__2410IIC_H__
