
/*
 * 参考 drivers\net\cs89x0.c
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>

static struct net_device *vnet_dev;

static int virt_net_send_packet(struct sk_buff *skb, struct net_device *dev)
{
	static int cnt = 0;
	printk("virt_net_send_packet cnt = %d\n", ++cnt);

	/* 对于真实的网卡, 把skb里的数据通过网卡发送出去 */

	/* 更新统计信息 */
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;
	
	return 0;
}


static int virt_net_init(void)
{
	/* 1. 分配一个net_device结构体 */
	vnet_dev = alloc_netdev(0, "vnet%d", ether_setup);;  /* alloc_etherdev */

	/* 2. 设置 */
	vnet_dev->hard_start_xmit = virt_net_send_packet;

	/* 设置MAC地址 */
    vnet_dev->dev_addr[0] = 0x08;
    vnet_dev->dev_addr[1] = 0x89;
    vnet_dev->dev_addr[2] = 0x89;
    vnet_dev->dev_addr[3] = 0x89;
    vnet_dev->dev_addr[4] = 0x89;
    vnet_dev->dev_addr[5] = 0x11;

	

	/* 3. 注册 */
	//register_netdevice(vnet_dev);
	register_netdev(vnet_dev);
	
	return 0;
}

static void virt_net_exit(void)
{
	unregister_netdev(vnet_dev);
	free_netdev(vnet_dev);
}

module_init(virt_net_init);
module_exit(virt_net_exit);

MODULE_AUTHOR("thisway.diy@163.com,17653039@qq.com");
MODULE_LICENSE("GPL");

