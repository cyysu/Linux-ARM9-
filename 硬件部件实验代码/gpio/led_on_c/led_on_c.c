#define GPFCON      (*(volatile unsigned long *)0x56000050)
#define GPFDAT      (*(volatile unsigned long *)0x56000054)

int main()
{
    GPFCON = 0x00000100;    // 设置GPF4为输出口, 位[9:8]=0b01
    GPFDAT = 0x00000000;    // GPF4输出0，LED1点亮

    return 0;
}
