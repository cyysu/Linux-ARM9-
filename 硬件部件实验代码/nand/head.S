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
            bl      memsetup                @初始化SDRAM
            bl      nand_init               @初始化NAND Flash

                                            @将NAND Flash中地址4096开始的1024字节代码(main.c编译得到)复制到SDRAM中
                                            @nand_read_ll函数需要3个参数：
            ldr     r0,     =0x30000000     @1. 目标地址=0x30000000，这是SDRAM的起始地址
            mov     r1,     #4096           @2.  源地址   = 4096，连接的时候，main.c中的代码都存在NAND Flash地址4096开始处
            mov     r2,     #2048           @3.  复制长度= 2048(bytes)，对于本实验的main.c，这是足够了
            bl      nand_read               @调用C函数nand_read

            ldr     sp, =0x34000000         @设置栈
            ldr     lr, =halt_loop          @设置返回地址
            ldr     pc, =main               @b指令和bl指令只能前后跳转32M的范围，所以这里使用向pc赋值的方法进行跳转
halt_loop:
            b       halt_loop
