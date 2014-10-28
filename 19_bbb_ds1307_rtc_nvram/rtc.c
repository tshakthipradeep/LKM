#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/time.h>
#include <linux/bcd.h>
#include <linux/sysfs.h>

#define DISABLE_OSC	(1 << 7)

#define HOUR_FORMAT_12	(1 << 6)
#define HOUR_FORMAT_24	(0 << 6)

#define AM		(0 << 5)
#define PM		(1 << 5)

#define YEAR_1900	1900

#define NVRAM_SIZE	56

static struct rtc_device *ds1307_device;

static int ds1307_read_time(struct device *dev, struct rtc_time *rtc_time)
{
	struct rtc_time tm;
	struct timeval tv;
	unsigned long time = 0;

	unsigned char buf[7] = "";
	int ret = 0;
	struct rtc_time new_rtc_time;

	struct i2c_client *client = container_of(dev,struct i2c_client,dev);

	pr_alert("In %s\n",__func__);

	do_gettimeofday(&tv);

	time = tv.tv_sec;

	rtc_time_to_tm(time,&tm);
	
	memcpy(rtc_time,&tm,sizeof(tm));


	buf[0] = 0;

	ret = i2c_master_send(client,buf,1);
	ret = i2c_master_recv(client,buf,sizeof(buf));

	if(ret < 0)
	{
		pr_alert("%s: error reading rtc time: %d\n",__func__,ret);
	}
	else
	{
		pr_alert("%s: rtc time read successfully: %d\n",__func__,ret);

		new_rtc_time.tm_sec = bcd2bin(buf[0] & 0x7f);
		new_rtc_time.tm_min = bcd2bin(buf[1] & 0xff);
		new_rtc_time.tm_hour = bcd2bin(buf[2] & 0x1f);
		new_rtc_time.tm_wday = bcd2bin(buf[3] & 0x07);
		new_rtc_time.tm_mday = bcd2bin(buf[4] & 0x3f);
		new_rtc_time.tm_mon = bcd2bin(buf[5] & 0x1f);
		new_rtc_time.tm_year = bcd2bin(buf[6] & 0xff) + 100;

		pr_alert("day: %d, month: %d, year: %d, hour: %d, min: %d, sec: %d\n",
			new_rtc_time.tm_mday,new_rtc_time.tm_mon,new_rtc_time.tm_year,
			new_rtc_time.tm_hour,new_rtc_time.tm_min,new_rtc_time.tm_sec);

		if(rtc_valid_tm(&new_rtc_time))
			memcpy(rtc_time,&new_rtc_time,sizeof(struct rtc_time));
	}

	

	return rtc_valid_tm(rtc_time);
	
}

static int ds1307_set_time(struct device *dev, struct rtc_time *rtc_time)
{
	unsigned char buf[8] = "";
	int ret = 0;

	struct i2c_client *client = container_of(dev,struct i2c_client,dev);

	pr_alert("day: %d, month: %d, year: %d, hour: %d, min: %d, sec: %d\n",
		rtc_time->tm_mday,rtc_time->tm_mon,rtc_time->tm_year,
		rtc_time->tm_hour,rtc_time->tm_min,rtc_time->tm_sec);

	buf[0] = 0x0;
	buf[1] = bin2bcd(rtc_time->tm_sec);
	buf[2] = bin2bcd(rtc_time->tm_min);
	buf[3] = bin2bcd(rtc_time->tm_hour) | HOUR_FORMAT_24;
	buf[4] = bin2bcd(rtc_time->tm_wday);
	buf[5] = bin2bcd(rtc_time->tm_mday);
	buf[6] = bin2bcd(rtc_time->tm_mon);
	buf[7] = bin2bcd(rtc_time->tm_year - 100);

	ret = i2c_master_send(client,buf,sizeof(buf));

	if(ret < 0)
		pr_alert("%s: error setting rtc time: %d\n",__func__,ret);
	else
		pr_alert("%s: rtc time set successfully: %d\n",__func__,ret);	

	return rtc_valid_tm(rtc_time);
}

static int ds1307_set_alarm(struct device *dev, struct rtc_wkalrm *rtc_wkalrm)
{
	pr_alert("In %s\n",__func__);
	return 0;
}

static int ds1307_read_alarm(struct device *dev, struct rtc_wkalrm *rtc_wkalrm)
{
	pr_alert("In %s\n",__func__);
	return 0;
}

static struct rtc_class_ops rtc_ops = {
	.read_time = ds1307_read_time,
	.set_time = ds1307_set_time,
	.read_alarm = ds1307_read_alarm,
	.set_alarm = ds1307_set_alarm, 
};

ssize_t nvram_read(struct file *file, struct kobject *kobj, struct bin_attribute *attr,char *buf, loff_t offset, size_t count)
{
	int ret = 0;
        unsigned char nvram_buf[NVRAM_SIZE + 1] = "";
        int size = (count <= NVRAM_SIZE) ? count : NVRAM_SIZE;
	struct device *dev = container_of(kobj,struct device,kobj);
        struct i2c_client *client = container_of(dev,struct i2c_client,dev);

        pr_alert("In %s:%d\n",__func__,count);

	if(count <= 0 || count > NVRAM_SIZE)
		return -1;

        nvram_buf[0] = 0x08;
        ret = i2c_master_send(client,nvram_buf,1);
        ret = i2c_master_recv(client,nvram_buf,size);

        if(ret < 0)
                pr_alert("%s: error reading from rtc nvram: %d\n",__func__,ret);
        else
                pr_alert("%s: read from rtc nvram successful: %d\n",__func__,ret);

	return size;
}

ssize_t nvram_write(struct file *file, struct kobject *kobj, struct bin_attribute *attr,char *buf, loff_t offset, size_t count)
{
	int ret = 0;
	unsigned char nvram_buf[NVRAM_SIZE + 1] = "";
	int size = (count <= NVRAM_SIZE) ? count : NVRAM_SIZE;
	struct device *dev = container_of(kobj,struct device,kobj);
	struct i2c_client *client = container_of(dev,struct i2c_client,dev);

        pr_alert("In %s:%d\n",__func__,count);

        if(count <= 0 || count > NVRAM_SIZE)
                return -1;

        nvram_buf[0] = 0x08;
	memcpy(&nvram_buf[1],buf,size);

        ret = i2c_master_send(client,nvram_buf,size);

        if(ret < 0)
                pr_alert("%s: error writing to rtc nvram: %d\n",__func__,ret);
        else
                pr_alert("%s: write to rtc nvram successful: %d\n",__func__,ret);
        
	return size;
}

const struct bin_attribute nvram_attr = {
	{
		.name = "nvram",
		.mode = 0666,
	},
	.read = nvram_read,
	.write = nvram_write,
	.size = NVRAM_SIZE
};

int i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;

	pr_alert("In %s\n",__func__);

	ds1307_device = rtc_device_register("rtc",&client->dev,&rtc_ops,THIS_MODULE);

	if(IS_ERR(ds1307_device))
		pr_alert("rtc_device_register failed: %ld\n",PTR_ERR(ds1307_device));
	else
		pr_alert("rtc_device_register successful\n");

	ret = sysfs_create_bin_file(&client->dev.kobj,&nvram_attr);

	if(ret != 0)
		pr_alert("sysfs_create_bin_file failed\n");

	return 0;	
}

int i2c_remove(struct i2c_client *client)
{
	pr_alert("In %s\n",__func__);
	sysfs_remove_bin_file(&client->dev.kobj,&nvram_attr);
	rtc_device_unregister(ds1307_device);
	return 0;
}

static const struct i2c_device_id dev_id[] = {
        { "rtc", 0x68 },
        { }
};

static struct i2c_driver i2c_driver_ops = {
	.driver = {
		.name = "rtc",
		.owner = THIS_MODULE,
	},
	.probe = i2c_probe,
	.remove = i2c_remove,
	.id_table = dev_id,
};

int __init init_module(void)
{
	int ret = 0;

	pr_alert("In %s\n",__func__);
	ret = i2c_add_driver(&i2c_driver_ops);

	if(ret != 0)
		pr_alert("i2c_add_driver failed\n");
	else
		pr_alert("i2c_add_driver successful\n");

	return 0;
}

void __exit cleanup_module(void)
{
	
	pr_alert("In %s\n",__func__);
	i2c_del_driver(&i2c_driver_ops);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("19: DS1307 RTC Module with NVRAM");
