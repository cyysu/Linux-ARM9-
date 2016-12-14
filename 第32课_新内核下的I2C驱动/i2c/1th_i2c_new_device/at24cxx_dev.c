#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>


static struct i2c_board_info at24cxx_info = {	
	I2C_BOARD_INFO("at24c08", 0x50),
};

static struct i2c_client *at24cxx_client;

static int at24cxx_dev_init(void)
{
	struct i2c_adapter *i2c_adap;

	i2c_adap = i2c_get_adapter(0);
	at24cxx_client = i2c_new_device(i2c_adap, &at24cxx_info);
	i2c_put_adapter(i2c_adap);
	
	return 0;
}

static void at24cxx_dev_exit(void)
{
	i2c_unregister_device(at24cxx_client);
}


module_init(at24cxx_dev_init);
module_exit(at24cxx_dev_exit);
MODULE_LICENSE("GPL");


