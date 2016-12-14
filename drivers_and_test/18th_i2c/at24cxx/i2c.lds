SECTIONS {
    . = 0x30000000;
    .text : { head.o(.text)
              init.o(.text)
              nand.o(.text)
              *(.text) 
            }
    .rodata ALIGN(4) : {*(.rodata*)} 
    .data   ALIGN(4) : { *(.data) }
    __bss_start = .;
    .bss ALIGN(4)  : { *(.bss)  *(COMMON) }
    __bss_end = .;
}
