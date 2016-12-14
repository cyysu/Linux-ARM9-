#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>


static int __devinit at24cxx_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static int __devexit at24cxx_remove(struct i2c_client *client)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static const struct i2c_device_id at24cxx_id_table[] = {
	{ "at24c08", 0 },
	{}
};

static int at24cxx_detect(struct i2c_client *client,
		       struct i2c_board_info *info)
{
	/* 能运行到这里, 表示该addr的设备是存在的
	 * 但是有些设备单凭地址无法分辨(A芯片的地址是0x50, B芯片的地址也是0x50)
	 * 还需要进一步读写I2C设备来分辨是哪款芯片
	 * detect就是用来进一步分辨这个芯片是哪一款，并且设置info->type
	 */
	
	printk("at24cxx_detect : addr = 0x%x\n", client->addr);

	/* 进一步判断是哪一款 */
	
	strlcpy(info->type, "at24c08", I2C_NAME_SIZE);
	return 0;
}

static const unsigned short addr_list[] = { 0x60, 0x50, I2C_CLIENT_END };

/* 1. 分配/设置i2c_driver */
static struct i2c_driver at24cxx_driver = {
	.class  = I2C_CLASS_HWMON, /* 表示去哪些适配器上找设备 */
	.driver	= {
		.name	= "100ask",
		.owner	= THIS_MODULE,
	},
	.probe		= at24cxx_probe,
	.remove		= __devexit_p(at24cxx_remove),
	.id_table	= at24cxx_id_table,
	.detect     = at24cxx_detect,  /* 用这个函数来检测设备确实存在 */
	.address_list	= addr_list,   /* 这些设备的地址 */
};

static int at24cxx_drv_init(void)
{
	/* 2. 注册i2c_driver */
	i2c_add_driver(&at24cxx_driver);
	
	return 0;
}

static void at24cxx_drv_exit(void)
{
	i2c_del_driver(&at24cxx_driver);
}


module_init(at24cxx_drv_init);
module_exit(at24cxx_drv_exit);
MODULE_LICENSE("GPL");


