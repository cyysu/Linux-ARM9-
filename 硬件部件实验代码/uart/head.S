@******************************************************************************
@ File：head.S
@ 功能：设置SDRAM，将程序复制到SDRAM，然后跳到SDRAM继续执行
@******************************************************************************       
   
.extern     main
.text 
.global _start 
_start:
Reset:                  
    ldr sp, =4096           @ 设置栈指针，以下都是C函数，调用前需要设好栈
    bl  disable_watch_dog   @ 关闭WATCHDOG，否则CPU会不断重启
    // bl是位置无关码，相当于：PCnew = PC + 偏移
    //                         PCnew = (4+8) + 0x28 = 0x34
    
    ldr pc, =disable_watch_dog
    
    bl  clock_init          @ 设置MPLL，改变FCLK、HCLK、PCLK
    bl  memsetup            @ 设置存储控制器以使用SDRAM
    bl  copy_steppingstone_to_sdram     @ 复制代码到SDRAM中
    ldr pc, =on_sdram                   @ 跳到SDRAM中继续执行
on_sdram:
    ldr sp, =0x34000000     @ 设置栈指针
    ldr lr, =halt_loop      @ 设置返回地址
    ldr pc, =main           @ 调用main函数
halt_loop:
    b   halt_loop
