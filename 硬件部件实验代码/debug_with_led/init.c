/* WOTCH DOG register */
#define 	WTCON				(*(volatile unsigned long *)0x53000000)

/* SDRAM regisers */
#define 	MEM_CTL_BASE		0x48000000
 
void disable_watch_dog();
void memsetup();

/*上电后，WATCH DOG默认是开着的，要把它关掉 */
void disable_watch_dog()
{
	WTCON	= 0;
}

/* 设置控制SDRAM的13个寄存器 */
void memsetup()
{
	int 	i = 0;
	unsigned long *p = (unsigned long *)MEM_CTL_BASE;
#if 0
    /* SDRAM 13个寄存器的值 */
    unsigned long  const    mem_cfg_val[]={ 0x22011110,     //BWSCON
                                            0x00000700,     //BANKCON0
                                            0x00000700,     //BANKCON1
                                            0x00000700,     //BANKCON2
                                            0x00000700,     //BANKCON3  
                                            0x00000700,     //BANKCON4
                                            0x00000700,     //BANKCON5
                                            0x00018005,     //BANKCON6
                                            0x00018005,     //BANKCON7
                                            0x008C07A3,     //REFRESH
                                            0x000000B1,     //BANKSIZE
                                            0x00000030,     //MRSRB6
                                            0x00000030,     //MRSRB7
                                    };

	for(; i < 13; i++)
		p[i] = mem_cfg_val[i];
#endif		

	p[0] = 0x22011110;     //BWSCON
	p[1] = 0x00000700;     //BANKCON0
	p[2] = 0x00000700;     //BANKCON1
	p[3] = 0x00000700;     //BANKCON2
	p[4] = 0x00000700;     //BANKCON3  
	p[5] = 0x00000700;     //BANKCON4
	p[6] = 0x00000700;     //BANKCON5
	p[7] = 0x00018005;     //BANKCON6
	p[8] = 0x00018005;     //BANKCON7
	p[9] = 0x008C07A3;     //REFRESH
	p[10] = 0x000000B1;     //BANKSIZE
	p[11] = 0x00000030;     //MRSRB6
	p[12] = 0x00000030;     //MRSRB7
}
