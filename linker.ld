/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(main)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	. = 0x130000;
	sysEnter_Vector = .;
        /* Begin putting sections at 1 MiB, a conventional place for kernels to be
           loaded at by the bootloader. */
        . = 0x31000000;
}

