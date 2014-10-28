LKM
===

**18_bbb_ds1307_rtc**: DS1307 RTC Driver

Before inserting this driver, please make the following changes in arch/arm/mach-omap2/board-am335xevm.c

```
static struct i2c_board_info am335x_i2c2_boardinfo[] = {
	{
		I2C_BOARD_INFO("rtc", 0x68),
	},
};
```

1. Compile kernel to create uImage and copy image to boot partition of the SD Card
2. run date -s "2014-10-14 12:42:00" command to set system time
3. run hwclock -f /dev/rtc1 to read time from RTC
4. run hwclock -w -f /dev/rtc1 to write time to RTC

