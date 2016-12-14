@******************************************************************************
@ File：head.s
@ 功能：设置SDRAM，将程序复制到SDRAM，然后跳到SDRAM继续执行
@******************************************************************************       
  
.text
.global _start
_start:
                                            @函数disable_watch_dog, memsetup, init_nand, nand_read_ll在init.c中定义
            ldr     sp, =4096               @设置堆栈 
            bl      disable_watch_dog       @关WATCH DOG
            // 点灯
            bl      memsetup                @初始化SDRAM
            //b 		led_flicker
            bl      nand_init               @初始化NAND Flash
            //b 		led_flicker

                                            @将NAND Flash中地址4096开始的1024字节代码(main.c编译得到)复制到SDRAM中
                                            @nand_read_ll函数需要3个参数：
            ldr     r0,     =0x30000000     @1. 目标地址=0x30000000，这是SDRAM的起始地址
            mov     r1,     #0           	@2.  源地址   = 0
            mov     r2,     #4096           @3.  复制长度= 4096
            bl      nand_read               @调用C函数nand_read
            //b 		led_flicker

            ldr     sp, =0x34000000         @设置栈
            ldr     lr, =halt_loop          @设置返回地址
            ldr     pc, =main               @b指令和bl指令只能前后跳转32M的范围，所以这里使用向pc赋值的方法进行跳转
halt_loop:
            b       halt_loop

led_flicker:
			// 初始化LED
			ldr r0, =0x56000050  @ GPFCON
			ldr r1, =(1<<(4*2))
			str r1, [r0]
			
			// 循环点亮熄灭LED
			ldr r0, =0x56000054  @ GPFDAT
			ldr r1, =0
			ldr r2, =(1<<4)
loop:			
			str r1, [r0]   @ 点亮
			bl delay
			str r2, [r0]   @ 熄灭
			bl delay
			b loop

delay:
			ldr r3, =30000
1:
			sub r3, r3, #1
			cmp r3, #0
			bne 1b
			mov pc, lr
