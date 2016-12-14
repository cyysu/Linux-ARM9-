@******************************************************************************
@ init.S
@ 功能：OpenOCD先将init.bin下载到Steppingstone(内部RAM)中，初始化SDRAM等
@       然后就可以将bootloader直接下载到SDRAM中运行
@******************************************************************************       

#define  DELAY_COUNT 700000

.text 
.global _start 
_start:
Reset: 
    mrs r0, CPSR
    bic r0, r0, #0x1f
    orr r0, r0, #0xd3
    msr CPSR_fc, r0     @ Supervisor mode, sets the I and F bits
    
    mov r0, #0x53000000
    mov r1, #0
    str r1, [r0]        @ disable WTCON
        
    mvn r1, #0  @ 0x0
    ldr r0, =0x4a000008
    str r1, [r0]        @ INTMSK = 0xFFFFFFFF
    ldr r1, =0x000003ff
    ldr r0, =0x4a00001c
    str r1, [r0]        @ INTSUBMSK = 0x000003ff
    
    mov r0, #0
    mcr 15, 0, r0, cr7, cr7, 0    @ invalidate I,D caches on v4
    mcr 15, 0, r0, cr8, cr7, 0    @ invalidate I,D TLBs on v4
    mrc 15, 0, r0, cr1, cr0, 0    @ get control register v4, read control register P539
    bic r0, r0, #0x2300           @ V[bit[13]](Base location of exception registers)=0 = Low addresses = 0x0000 0000
                                  @ R(ROM protection bit[9])=0
                                  @ S(System protection bit[8])=0
                                  @ 由于TTB中AP=0b11(line141)，所以RS位不使用
    bic r0, r0, #0x87
                                  @ M(bit[0])=0 = MMU disabled
                                  @ A(bit[1])=0 =Data address alignment fault checking disable
                                  @ C(bit[2])=0 = Data cache disabled
                                  @ B(bit[7])=0 = Little-endian operation
    orr r0, r0, #0x2              @ /* .... .... .... ..1. *//*A(bit[1])=1 = Data address alignment fault checking enable*/
    orr r0, r0, #0x1000           @ /* ...1 .... .... .... *//*I(bit[12])=1 = Instruction cache enabled*/
    mcr 15, 0, r0, cr1, cr0, 0    @ /* write control register *//*write control register P545*/

    @ set memory control registers
    mov r1, #0x48000000
    adrl  r2, mem_cfg_val
    add r3, r1, #52
1:  ldr r4, [r2], #4
    str r4, [r1], #4
    cmp r1, r3
    bne 1b

    mrc 15, 0, r1, cr1, cr0, 0            @ read ctrl register 
    orr r1, r1, #0xc0000000               @ Asynchronous bus mode
    mcr 15, 0, r1, cr1, cr0, 0            @ write ctrl register		
    
    @ Light LEDs
leds_flicker:    
    ldr r2, =0x56000010   @ GPBCON  
    mov r3, #((1<<(5*2)) | (1<<(6*2)) | (1<<(7*2)) | (1<<(8*2)))      @ GPB5_out|GPB6_out|GPB7_out|GPB8_out
    str r3, [r2]   

    ldr r5, =0x56000014   @ GPBDAT  
    mov  r4, #0  @ 0x0

led_loop:
    ldr r0, =DELAY_COUNT
    bl  wait

    mvn r3, r4, lsl #5
    add r4, r4, #1      @ 0x1
    cmp r4, #16 @ 0x10
    subeq r4, r4, #16     @ 0x10
    str r3, [r5]
    b led_loop

wait:
    subs r0, r0, #1
    bne wait
    mov pc, lr

.align 4
mem_cfg_val:
  .long 0x2201d110
  .long 0x00000700
  .long 0x00000700
  .long 0x00000700
  .long 0x00001f4c
  .long 0x00000700
  .long 0x00000700
  .long 0x00018005
  .long 0x00018005
  .long 0x008C04F4  @ HCLK=100MHz
#  .long 0x008C062C  @ HCLK=60MHz
#  .long 0x008C067A   @ HCLK=50MHz
  .long 0x000000B1
  .long 0x00000030
  .long 0x00000030
