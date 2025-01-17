SECTIONS
{
	.text.boot : { *(.text.boot) }
	__kernel_start = .;
	.text : { *(.text) }
	__kernel_end = .;
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
}
