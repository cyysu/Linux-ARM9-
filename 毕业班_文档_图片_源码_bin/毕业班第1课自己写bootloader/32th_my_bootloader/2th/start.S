
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#define S3C2440_MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))
#define MEM_CTL_BASE    0x48000000

.text
.global _start
_start:

/* 1. 关看门狗 */
	ldr r0, =0x53000000
	mov r1, #0
	str r1, [r0]

/* 2. 设置时钟 */
	ldr r0, =0x4c000014
	//	mov r1, #0x03;			  // FCLK:HCLK:PCLK=1:2:4, HDIVN=1,PDIVN=1
	mov r1, #0x05;			  // FCLK:HCLK:PCLK=1:4:8
	str r1, [r0]

	/* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
	mrc	p15, 0, r1, c1, c0, 0		/* 读出控制寄存器 */ 
	orr	r1, r1, #0xc0000000			/* 设置为“asynchronous bus mode” */
	mcr	p15, 0, r1, c1, c0, 0		/* 写入控制寄存器 */

	/* MPLLCON = S3C2440_MPLL_200MHZ */
	ldr r0, =0x4c000004
	ldr r1, =S3C2440_MPLL_400MHZ
	str r1, [r0]

	/* 启动ICACHE */
	mrc p15, 0, r0, c1, c0, 0	@ read control reg
	orr r0, r0, #(1<<12)
	mcr	p15, 0, r0, c1, c0, 0   @ write it back


/* 3. 初始化SDRAM */
	ldr r0, =MEM_CTL_BASE
	adr r1, sdram_config     /* sdram_config的当前地址 */
	add r3, r0, #(13*4)
1:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne 1b

/* 4. 重定位 : 把bootloader本身的代码从flash复制到它的链接地址去 */
	ldr sp, =0x34000000

	bl nand_init

	mov r0, #0
	ldr r1, =_start
	ldr r2, =__bss_start
	sub r2, r2, r1
	
	bl copy_code_to_sdram
	bl clear_bss
	
/* 5. 执行main */
	ldr lr, =halt
	ldr pc, =main
halt:
	b halt

sdram_config:
	.long 0x22011110	 //BWSCON
	.long 0x00000700	 //BANKCON0
	.long 0x00000700	 //BANKCON1
	.long 0x00000700	 //BANKCON2
	.long 0x00000700	 //BANKCON3  
	.long 0x00000700	 //BANKCON4
	.long 0x00000700	 //BANKCON5
	.long 0x00018005	 //BANKCON6
	.long 0x00018005	 //BANKCON7
	.long 0x008C04F4	 // REFRESH
	.long 0x000000B1	 //BANKSIZE
	.long 0x00000030	 //MRSRB6
	.long 0x00000030	 //MRSRB7

