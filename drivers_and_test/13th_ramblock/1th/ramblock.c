
/* 参考:
 * drivers\block\xd.c
 * drivers\block\z2ram.c
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>

static struct gendisk *ramblock_disk;
static request_queue_t *ramblock_queue;

static int major;

static DEFINE_SPINLOCK(ramblock_lock);

static struct block_device_operations ramblock_fops = {
	.owner	= THIS_MODULE,
};

#define RAMBLOCK_SIZE (1024*1024)

static void do_ramblock_request(request_queue_t * q)
{
	static int cnt = 0;
	printk("do_ramblock_request %d\n", ++cnt);
}

static int ramblock_init(void)
{
	/* 1. 分配一个gendisk结构体 */
	ramblock_disk = alloc_disk(16); /* 次设备号个数: 分区个数+1 */

	/* 2. 设置 */
	/* 2.1 分配/设置队列: 提供读写能力 */
	ramblock_queue = blk_init_queue(do_ramblock_request, &ramblock_lock);
	ramblock_disk->queue = ramblock_queue;
	
	/* 2.2 设置其他属性: 比如容量 */
	major = register_blkdev(0, "ramblock");  /* cat /proc/devices */	
	ramblock_disk->major       = major;
	ramblock_disk->first_minor = 0;
	sprintf(ramblock_disk->disk_name, "ramblock");
	ramblock_disk->fops        = &ramblock_fops;
	set_capacity(ramblock_disk, RAMBLOCK_SIZE / 512);

	/* 3. 注册 */
	add_disk(ramblock_disk);

	return 0;
}

static void ramblock_exit(void)
{
	unregister_blkdev(major, "ramblock");
	del_gendisk(ramblock_disk);
	put_disk(ramblock_disk);
	blk_cleanup_queue(ramblock_queue);
}

module_init(ramblock_init);
module_exit(ramblock_exit);

MODULE_LICENSE("GPL");

