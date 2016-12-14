/* linux/arch/arm/mach-s3c2440/mach-zhiyuan2440.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include <mach/idle.h>
#include <mach/fb.h>
#include <plat/iic.h>

#include <plat/s3c2410.h>
#include <plat/s3c2440.h>
#include <plat/clock.h>
#include <plat/devs.h>
#include <plat/cpu.h>

#include <plat/common-zhiyuan-arm.h>

#include <sound/s3c24xx_uda134x.h>

static struct map_desc zhiyuan2440_iodesc[] __initdata = {
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg zhiyuan2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	}
};

/* LCD driver info */

#if defined(CONFIG_FB_S3C2410_N240320)

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 160000

#define LCD_RIGHT_MARGIN 12
#define LCD_LEFT_MARGIN 0
#define LCD_HSYNC_LEN 0

#define LCD_UPPER_MARGIN 3
#define LCD_LOWER_MARGIN 3
#define LCD_VSYNC_LEN 1

#elif defined(CONFIG_FB_S3C2410_TFT800480)
#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#define LCD_PIXCLOCK 40000

#define LCD_RIGHT_MARGIN 67
#define LCD_LEFT_MARGIN 40
#define LCD_HSYNC_LEN 31

#define LCD_UPPER_MARGIN 25
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1

#elif defined(CONFIG_FB_S3C2410_VGA1024768)
#define LCD_WIDTH 1024
#define LCD_HEIGHT 768
#define LCD_PIXCLOCK 80000

#define LCD_RIGHT_MARGIN 15
#define LCD_LEFT_MARGIN 199
#define LCD_HSYNC_LEN 15

#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 1
#define LCD_VSYNC_LEN 1
#define LCD_CON5 (S3C2410_LCDCON5_FRM565 | S3C2410_LCDCON5_HWSWP)

#elif defined(CONFIG_FB_S3C2410_T320240)
#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_PIXCLOCK 170000

#define LCD_RIGHT_MARGIN 68
#define LCD_LEFT_MARGIN 20
#define LCD_HSYNC_LEN 31

#define LCD_UPPER_MARGIN 12
#define LCD_LOWER_MARGIN 12
#define LCD_VSYNC_LEN 2
//added by HCJ
#elif defined(CONFIG_FB_S3C2410_T480272)
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define LCD_PIXCLOCK 100000

#define LCD_RIGHT_MARGIN 5
#define LCD_LEFT_MARGIN 15
#define LCD_HSYNC_LEN 35

#define LCD_UPPER_MARGIN 2
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 8

#endif

#if defined (LCD_WIDTH)

static struct s3c2410fb_display zhiyuan2440_lcd_cfg __initdata = {

#if !defined (LCD_CON5)
	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_INVVLINE |
			  S3C2410_LCDCON5_INVVFRAME |
			  S3C2410_LCDCON5_PWREN |
			  S3C2410_LCDCON5_HWSWP,
#else
	.lcdcon5	= LCD_CON5,
#endif

	.type		= S3C2410_LCDCON1_TFT,

	.width		= LCD_WIDTH,
	.height		= LCD_HEIGHT,

	.pixclock	= LCD_PIXCLOCK,
	.xres		= LCD_WIDTH,
	.yres		= LCD_HEIGHT,
	.bpp		= 16,
	.left_margin	= LCD_LEFT_MARGIN + 1,
	.right_margin	= LCD_RIGHT_MARGIN + 1,
	.hsync_len	= LCD_HSYNC_LEN + 1,
	.upper_margin	= LCD_UPPER_MARGIN + 1,
	.lower_margin	= LCD_LOWER_MARGIN + 1,
	.vsync_len	= LCD_VSYNC_LEN + 1,
};


static struct s3c2410fb_mach_info zhiyuan2440_fb_info __initdata = {
	.displays	= &zhiyuan2440_lcd_cfg,
	.num_displays	= 1,
	.default_display = 0,

	.gpccon =       0xaa955699,
	.gpccon_mask =  0xffc003cc,
	.gpcup =        0x0000ffff,
	.gpcup_mask =   0xffffffff,

	.gpdcon =       0xaa95aaa1,
	.gpdcon_mask =  0xffc0fff0,
	.gpdup =        0x0000faff,
	.gpdup_mask =   0xffffffff,


	.lpcsel		= 0xf82,
};

#endif

static struct s3c24xx_uda134x_platform_data s3c24xx_uda134x_data = {
	.l3_clk = S3C2410_GPB4,
	.l3_data = S3C2410_GPB3,
	.l3_mode = S3C2410_GPB2,
	.model = UDA134X_UDA1341,
};

static struct platform_device s3c24xx_uda134x = {
	.name = "s3c24xx_uda134x",
	.dev = {
		.platform_data    = &s3c24xx_uda134x_data,
	}
};
//added by hcj
#if 0
static struct s3c2410_ts_mach_info gec2440_ts_cfg __initdata = {
	.delay = 10000,
	.presc = 49,
	.oversampling_shift = 2,
}; 
#endif
static struct platform_device *zhiyuan2440_devices[] __initdata = {
//	&s3c_device_ts,
	&s3c_device_usb,
	&s3c_device_rtc,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c0,
	&s3c_device_iis,
	&s3c_device_dm9k,
	&net_device_cs8900,
	&s3c24xx_uda134x,
};

static void __init zhiyuan2440_map_io(void)
{
//	s3c24xx_init_touchscreen(&gec2440_ts_cfg);
	s3c24xx_init_io(zhiyuan2440_iodesc, ARRAY_SIZE(zhiyuan2440_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(zhiyuan2440_uartcfgs, ARRAY_SIZE(zhiyuan2440_uartcfgs));
}

static void __init zhiyuan2440_machine_init(void)
{
#if defined (LCD_WIDTH)
	s3c24xx_fb_set_platdata(&zhiyuan2440_fb_info);
#endif
	s3c_i2c0_set_platdata(NULL);

	platform_add_devices(zhiyuan2440_devices, ARRAY_SIZE(zhiyuan2440_devices));
	zhiyuan_arm_machine_init();
}

MACHINE_START(PNX4008, "ZhiyuanOpenCenter ZhiYuan2440 development board")
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,

	.init_irq	= s3c24xx_init_irq,
	.map_io		= zhiyuan2440_map_io,
	.init_machine	= zhiyuan2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END

