/*
 * FILE: m41t11.c
 * 调用I2C读写函数，设置、读取RTC芯片m41t11
 */
#include <string.h>
#include "m41t11.h"
#include "i2c.h"

struct rtc_registers {
    unsigned char   secs;
    unsigned char   mins;
    unsigned char   hours;
    unsigned char   wday;
    unsigned char   mday;
    unsigned char   mon;
    unsigned char   year;
    unsigned char   cs;
};

#define BCD_TO_BIN(val) (((val)&15) + ((val)>>4)*10)
#define BIN_TO_BCD(val) ((((val)/10)<<4) + (val)%10)                     


static unsigned long epoch = 2000;  /* 芯片中”年”为0x00时，对应2000年 */

static const unsigned char days_in_mo[] = 
{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * 写m41t11，设置日期与时间
 */
int m41t11_set_datetime(struct rtc_time *dt)
{
    unsigned char leap_yr;
    struct {
        unsigned char addr;
        struct rtc_registers rtc;
    } __attribute__ ((packed)) addr_and_regs;

    memset(&addr_and_regs, 0, sizeof(addr_and_regs));

    leap_yr = ((!(dt->tm_year % 4) && (dt->tm_year % 100))
            || !(dt->tm_year % 400));

    if ((dt->tm_mon < 1) || (dt->tm_mon > 12) || (dt->tm_mday == 0)) {
        return -1;
    }

    if (dt->tm_mday > (days_in_mo[dt->tm_mon] + ((dt->tm_mon == 2)
                && leap_yr))) {
        return -2;
    }

    if ((dt->tm_hour >= 24) || (dt->tm_min >= 60) || (dt->tm_sec >= 60)) {
        return -3;
    }

    if ((dt->tm_year -= epoch) > 255) {
        /* They are unsigned */
        return -4;
    }

    if (dt->tm_year >= 100) {
        dt->tm_year -= 100;
    }

    addr_and_regs.rtc.secs  = BIN_TO_BCD(dt->tm_sec);
    addr_and_regs.rtc.mins  = BIN_TO_BCD(dt->tm_min);
    addr_and_regs.rtc.hours = BIN_TO_BCD(dt->tm_hour);
    addr_and_regs.rtc.mday  = BIN_TO_BCD(dt->tm_mday);
    addr_and_regs.rtc.mon   = BIN_TO_BCD(dt->tm_mon);
    addr_and_regs.rtc.year  = BIN_TO_BCD(dt->tm_year);
    addr_and_regs.rtc.wday  = BIN_TO_BCD(dt->tm_wday);
    addr_and_regs.rtc.cs    = 0;

    i2c_write(0xD0, (unsigned char *)&addr_and_regs, sizeof(addr_and_regs));

    return 0;
}

/*
 * 读取m41t11，获得日期与时间
 */
int m41t11_get_datetime(struct rtc_time *dt)
{
    unsigned char addr[1] = { 0 };
    struct rtc_registers rtc;

    memset(&rtc, 0, sizeof(rtc));

    i2c_write(0xD0, addr, 1);
    i2c_read(0xD0, (unsigned char *)&rtc, sizeof(rtc));

    dt->tm_year = epoch;
    rtc.secs  &= 0x7f;   /* seconds */
    rtc.mins  &= 0x7f;   /* minutes */
    rtc.hours &= 0x3f;   /* hours */
    rtc.wday  &= 0x07;   /* day-of-week */
    rtc.mday  &= 0x3f;   /* day-of-month */
    rtc.mon   &= 0x1f;   /* month */
    rtc.year  &= 0xff;   /* year */

    dt->tm_sec     = BCD_TO_BIN(rtc.secs);
    dt->tm_min     = BCD_TO_BIN(rtc.mins);
    dt->tm_hour    = BCD_TO_BIN(rtc.hours);
    dt->tm_wday    = BCD_TO_BIN(rtc.wday);
    dt->tm_mday    = BCD_TO_BIN(rtc.mday);
    dt->tm_mon     = BCD_TO_BIN(rtc.mon);
    dt->tm_year    += BCD_TO_BIN(rtc.year);

    return 0;
}

