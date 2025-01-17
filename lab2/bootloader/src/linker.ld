SECTIONS
{
	.text.boot : { *(.text.boot) }

	.text 0x60000:
	{
		__bootloader_start = .;
		 *(.text)
		__bootloader_end = .;
	}
	
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
}
