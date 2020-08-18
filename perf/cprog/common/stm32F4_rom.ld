/*
 *  @(#) $Id: stm32F4_rom.ld,v 1.1 2015/06/25 22:18:56 roi Exp $
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)

MEMORY
{
         vector_rom(wxai) : ORIGIN = 0x08000000, LENGTH = 392
				rom(wxai) : ORIGIN = 0x08000188, LENGTH = 256K - 392
                ram(wxai) : ORIGIN = 512M, LENGTH = 128K
}

PROVIDE(hardware_init_hook = 0);

SECTIONS
{

.isr_vector : {
IsrVector = . ;
    * (*.vector *.vector.*)
    . = ALIGN(16);
} > vector_rom

.text : {
    . = ALIGN(16);
    * (*.text *.text.* *.glue*)
    . = ALIGN(16);
    * (*.rodata *.rodata.*)
    . = ALIGN(16);
/* added 15-apr-2008 by rwk */
    * (*.gnu.linkonce.* .gcc_except_table)
    . = ALIGN(16);
} > rom

  . = ALIGN(4);
.data : {
        _sdata = . ;
        *(.data)
} > ram
 AT> rom

_sdata = ADDR(.data);
_sidata = LOADADDR(.data); 
_edata = _sdata + SIZEOF(.data);

/* bss is zero initialised data */
.bss : {
    * (*.bss *.bss.*)
    . = ALIGN(16);
} > ram
 AT> rom

_sbss = ADDR(.bss);
_ebss = (ADDR(.bss) + SIZEOF(.bss));

   . = ALIGN(32 / 8);
_end = .		;
_estack = . + 1024;
    PROVIDE (end = .)	;
.comment 0 : { *(.comment) }

  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to 
     the beginning of the section so we begin them at 0.  */

  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }

  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }

  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }

  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}

