#define GPFCON      (*(volatile unsigned long *)0x56000050)
#define GPFDAT      (*(volatile unsigned long *)0x56000054)

#define GPGCON      (*(volatile unsigned long *)0x56000060)
#define GPGDAT      (*(volatile unsigned long *)0x56000064)

/*
 * LED1,LED2,LED4对应GPF4、GPF5、GPF6
 */
#define	GPF4_out	(1<<(4*2))
#define	GPF5_out	(1<<(5*2))
#define	GPF6_out	(1<<(6*2))

#define	GPF4_msk	(3<<(4*2))
#define	GPF5_msk	(3<<(5*2))
#define	GPF6_msk	(3<<(6*2))

/*
 * S2,S3,S4对应GPF0、GPF2、GPG3
 */
#define GPF0_in     (0<<(0*2))
#define GPF2_in     (0<<(2*2))
#define GPG3_in     (0<<(3*2))

#define GPF0_msk    (3<<(0*2))
#define GPF2_msk    (3<<(2*2))
#define GPG3_msk    (3<<(3*2))

int main()
{
        unsigned long dwDat;
        // LED1,LED2,LED4对应的3根引脚设为输出
        GPFCON &= ~(GPF4_msk | GPF5_msk | GPF6_msk);
        GPFCON |= GPF4_out | GPF5_out | GPF6_out;
        
        // S2,S3对应的2根引脚设为输入
        GPFCON &= ~(GPF0_msk | GPF2_msk);
        GPFCON |= GPF0_in | GPF2_in;

        // S4对应的引脚设为输入
        GPGCON &= ~GPG3_msk;
        GPGCON |= GPG3_in;

        while(1){
            //若Kn为0(表示按下)，则令LEDn为0(表示点亮)
            dwDat = GPFDAT;             // 读取GPF管脚电平状态
        
            if (dwDat & (1<<0))        // S2没有按下
                GPFDAT |= (1<<4);       // LED1熄灭
            else    
                GPFDAT &= ~(1<<4);      // LED1点亮
                
            if (dwDat & (1<<2))         // S3没有按下
                GPFDAT |= (1<<5);       // LED2熄灭
            else    
                GPFDAT &= ~(1<<5);      // LED2点亮
    
            dwDat = GPGDAT;             // 读取GPG管脚电平状态
            
            if (dwDat & (1<<3))         // S4没有按下
                GPFDAT |= (1<<6);       // LED3熄灭
            else    
                GPFDAT &= ~(1<<6);      // LED3点亮
    }

    return 0;
}
