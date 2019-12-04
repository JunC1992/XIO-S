/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
SECTIONS
{
    PROVIDE( kern_start = 0xC0100000);
	. = 0x100000; 
    .init.text :
    {
        *(.init.text)
        . = ALIGN(4096);
    }
    .init.data :
    {
        *(.init.data)
        . = ALIGN(4096);
    }

	. += 0xC0000000;
	.text : AT(ADDR(.text) - 0xC0000000)
    {
        code = .; _code = .; __code = .;
        *(.text)
        . = ALIGN(4096);
    }

    .data : AT(ADDR(.data) - 0xC0000000)
    {
        data = .; _data = .; __data = .;
        *(.data)
        *(.rodata)
        . = ALIGN(4096);
    }

    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
        . = ALIGN(4096);
    }

    end = .; _end = .; __end = .;
    PROVIDE( kern_end = . );
    /DISCARD/ : { *(.comment) *(.eh_frame) }
}
