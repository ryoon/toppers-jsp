/* This file is generated from sys_rename.def by genrename. */

#ifndef _SYS_RENAME_H
#define _SYS_RENAME_H

#define sys_mmu_init				_kernel_sys_mmu_init
#define sys_putc					_kernel_sys_putc
#define x_ipi_raise					_kernel_x_ipi_raise
#define mprc_initialize				_kernel_mprc_initialize
#define prc2_boot					_kernel_prc2_boot
#define prc2_boot_end				_kernel_prc2_boot_end

#ifdef LABEL_ASM

#define _sys_mmu_init				__kernel_sys_mmu_init
#define _sys_putc					__kernel_sys_putc
#define _x_ipi_raise				__kernel_x_ipi_raise
#define _mprc_initialize			__kernel_mprc_initialize
#define _prc2_boot					__kernel_prc2_boot
#define _prc2_boot_end				__kernel_prc2_boot_end

#endif /* LABEL_ASM */


#endif /* _SYS_RENAME_H */
