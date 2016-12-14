SECTIONS {
    . = 0x30000000;
    .text          :   { 
    	head.o(.text) 
    	init.o(.text) 
    	nand.o
    	*(.text) 
    }
    .rodata ALIGN(4) : {*(.rodata)} 
    .data ALIGN(4) : { *(.data) }
    .bss ALIGN(4)  : { *(.bss)  *(COMMON) }
}

 
