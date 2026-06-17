/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

/*
 *  コア依存の特殊命令のインライン関数定義（ARM-A用）
 */

#ifndef _CPU_INSN_H
#define _CPU_INSN_H

/*
 *  ARM 例外ベクタ
 */
#define SVC_Vector		0x00u
#define UND_Vector		0x04u
#define SWI_Vector		0x08u
#define PRFA_Vector		0x0Cu
#define DATAA_Vector	0x10u
#define IRQ_Vector		0x18u
#define FIQ_Vector		0x1Cu

/*
 * ARM 例外ベクタ番号
 */
#define SVC_Number		0
#define UND_Number		1
#define SWI_Number		2
#define PRFA_Number		3
#define DATAA_Number	4
#define IRQ_Number		5
#define FIQ_Number		6

/*
 *  CPSR 割込み禁止ビット
 */
#define CPSR_INT_MASK	0x000000C0u
#define CPSR_IRQ_BIT	0x00000080u
#define CPSR_FIQ_BIT	0x00000040u

/*
 *  CPSR のモードビット
 */
#define CPSR_MODE_MASK	0x0000001fu
#define CPSR_USER		0x00000010u
#define CPSR_FIQ		0x00000011u
#define CPSR_IRQ		0x00000012u
#define CPSR_SVC		0x00000013u
#define CPSR_ABT		0x00000017u
#define CPSR_UND		0x0000001bu
#define CPSR_SYS		0x0000001fu

/*
 *  CPUロックとするCPSRのパターン
 */
#define CPSR_CPULOCK	(CPSR_IRQ_BIT)

/*
 *  割込みロックとするCPSRのパターン
 */
#define CPSR_INTLOCK	(CPSR_FIQ_BIT|CPSR_IRQ_BIT)

/*
 *  CPSRに常にセットするパターン
 */
#define CPSR_ALWAYS_SET	0x00


/*
 * コプロ用の定義
 */
#define CP15_CONTROL_XP_BIT	(1 << 23)
#define CP15_CONTROL_V_BIT	(1 << 13)
#define CP15_CONTROL_I_BIT	(1 << 12)
#define CP15_CONTROL_Z_BIT	(1 << 11)
#define CP15_CONTROL_C_BIT	(1 <<  2)
#define CP15_CONTROL_M_BIT	(1 <<  0)

#define CP15_AUXILIARY_SA_BIT	(1 << 6)
#define CP15_AUXILIARY_EX_BIT	(1 << 7)
#define CP15_AUXILIARY_FW_BIT	(1 << 0)

#define CP15_CPUID_BIT		(0x03)

#define CP15_TTB0_RGN_S		(1 << 1)
#define CP15_TTB0_RGN_WBWA	(1 << 3)
#define CP15_TTB0_IRGN_WBWA	((1 << 6)|(0))

#define CP15_NSACR_NS_SMP	(1 << 18)

#define CCSIDR_LINE_SIZE_MASK		0x00000007
#define CCSIDR_LINE_SIZE_OFFSET		0
#define CCSIDR_ASSOCIATIVITY_MASK	0x00001FF8
#define CCSIDR_ASSOCIATIVITY_OFFSET	3
#define CCSIDR_NUM_SETS_MASK		0x0FFFE000
#define CCSIDR_NUM_SETS_OFFSET		13

#define CLIDR_CTYPE_NO_CACHE          0U
#define CLIDR_CTYPE_INSTRUCTION_ONLY  1U
#define CLIDR_CTYPE_DATA_ONLY         2U
#define CLIDR_CTYPE_INSTRUCTION_DATA  3U
#define CLIDR_CTYPE_UNIFIED           4U

#define CLIDR_MAX_CTYPE         7U
#define CLIDR_CTYPE_MASK      0x7U
#define CLIDR_CTYPE_BITWIDTH    3U

#define CSSELR_IND_DATA_UNIFIED       0U
#define CSSELR_IND_INSTRUCTION        1U

#define RSAE_EN         0x01U
#define RSAE_RS_OFFSET  1U

#define RSAE_32      0x00000004U
#define RSAE_64      0x00000005U
#define RSAE_128     0x00000006U
#define RSAE_256     0x00000007U
#define RSAE_512     0x00000008U
#define RSAE_1K      0x00000009U
#define RSAE_2K      0x0000000AU
#define RSAE_4K      0x0000000BU
#define RSAE_8K      0x0000000CU
#define RSAE_16K     0x0000000DU
#define RSAE_32K     0x0000000EU
#define RSAE_64K     0x0000000FU
#define RSAE_128K    0x00000010U
#define RSAE_256K    0x00000011U
#define RSAE_512K    0x00000012U
#define RSAE_1M      0x00000013U
#define RSAE_2M      0x00000014U
#define RSAE_4M      0x00000015U
#define RSAE_8M      0x00000016U
#define RSAE_16M     0x00000017U
#define RSAE_32M     0x00000018U
#define RSAE_64M     0x00000019U
#define RSAE_128M    0x0000001AU
#define RSAE_256M    0x0000001BU
#define RSAE_512M    0x0000001CU
#define RSAE_1G      0x0000001DU
#define RSAE_2G      0x0000001EU
#define RSAE_4G      0x0000001FU

#define RAC_STRONGO_SHAR	0x00000000U

#define RAC_DEV_SHAR        0x00000001U
#define RAC_DEV_NSHAR       0x00000010U
#define RAC_NORM_OI_WT_NWA  0x00000002U
#define RAC_NORM_OI_WB_NWA  0x00000003U
#define RAC_OI_NCACHE       0x00000008U 
#define RAC_OI_WB_WA        0x0000000BU

#define RAC_NORM_I_NCACHE   0x00000020U
#define RAC_NORM_I_WB_WA    0x00000021U
#define RAC_NORM_I_WT_NWA   0x00000022U
#define RAC_NORM_I_WB_NWA   0x00000023U

#define RAC_NORM_O_NCACHE   0x00000020U
#define RAC_NORM_O_WB_WA    0x00000028U
#define RAC_NORM_O_WT_NWA   0x00000030U
#define RAC_NORM_O_WB_NWA   0x00000038U

#define RAC_AP_OFFSET       8U
#define RAC_AP_NO_ACCESS    (0x00000000U<<RAC_AP_OFFSET)
#define RAC_AP_PRW_UNA      (0x00000001U<<RAC_AP_OFFSET)
#define RAC_AP_PRW_URO      (0x00000002U<<RAC_AP_OFFSET)
#define	RAC_AP_PRW_URW      (0x00000003U<<RAC_AP_OFFSET)
#define RAC_AP_PRO_UNA      (0x00000005U<<RAC_AP_OFFSET)
#define RAC_AP_PRO_URO      (0x00000006U<<RAC_AP_OFFSET)

#define RAC_XN              0x00001000U /* Execute Never */
#define RAC_S               0x00000004U /* Share */

/*
 *  例外フレームのオフセット
 */
#define P_EXCINF_OFFSET_NEST_COUNT	0x00
#define P_EXCINF_OFFSET_IIPM		0x01
#if defined(SUPPORT_VFPv3_NEON)
#define P_EXCINF_OFFSET_PC			0x19
#define P_EXCINF_OFFSET_CPSR		0x1A
#else	/* SUPPORT_VFPv3_NEON */
#define P_EXCINF_OFFSET_PC			0x08
#define P_EXCINF_OFFSET_CPSR		0x09
#endif	/* SUPPORT_VFPv3_NEON */

#define P_INTINF_OFFSET_INTNO		0x00
#define P_INTINF_OFFSET_GICC_IAR	0x01


#ifndef _MACRO_ONLY

/*
 *  メモリが変更されることをコンパイラに伝えるためのマクロ
 */
#define ARM_MEMORY_CHANGED Asm("":::"memory")

/*
 *  制御レジスタの操作関数
 */

#ifndef __thumb__

/*
 *  ステータスレジスタ（CPSR）の現在値の読出し
 */
Inline UW
current_sr(void)
{
	UW sr;
	Asm("mrs  %0,CPSR" : "=r"(sr));
	return(sr);
}

/*
 *  ステータスレジスタ（CPSR）の現在値の変更
 */
Inline void
set_sr(UW sr)
{
	Asm("msr CPSR, %0" : : "r"(sr) : "cc" );
}

#else /* __thumb__ */

/*
 * Thumb Mode では，mrs/msrが使用できないため，関数として，
 * ARM Mode に変更して実行する．
 */
/*
 *  ステータスレジスタ（CPSR）の現在値の読出し
 */
extern UW current_sr(void);

/*
 *  ステータスレジスタ（CPSR）の現在値の変更
 */
extern void set_sr(UW sr);

#endif /* __thumb__ */

/**
 *  例外フレーム構造体
 */
typedef struct {
	UW	nest_count;
	UW	ipm;
#if defined(SUPPORT_VFPv3_NEON)
	UD	d0;
	UD	d1;
	UD	d2;
	UD	d3;
	UD	d4;
	UD	d5;
	UD	d6;
	UD	d7;
	UW	fpscr;
#endif /* SUPPORT_VFPv3_NEON */
	UW	r0;
	UW	r1;
	UW	r2;
	UW	r3;
	UW	r12;
	UW	lr;
	UW	pc;
	UW	cpsr;
} exc_frame_t;

#define CP15_CPUID_READ(x)        Asm("mrc p15, 0, %0, c0, c0, 5":"=r"(x))
#define CP15_CONTROL_READ(x)      Asm("mrc p15, 0, %0, c1, c0, 0":"=r"(x))
#define CP15_CONTROL_WRITE(x)     Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))
#define CP15_AUXILIARY_READ(x)    Asm("mrc p15, 0, %0, c1, c0, 1":"=r"(x))
#define CP15_AUXILIARY_WRITE(x)   Asm("mcr p15, 0, %0, c1, c0, 1"::"r"(x))
#define CP15_ICACHE_INVALIDATE()  Asm("mcr p15, 0, %0, c7, c5, 0"::"r"(0))
#define CP15_BRANCHP_INVALIDATE() Asm("mcr p15, 0, %0, c7, c5, 6"::"r"(0))
#define CPU15_PONLY_THREAD_ID_READ(x)  Asm("mrc p15, 0, %0, c13, c0, 4":"=r"(x))
#define CPU15_PONLY_THREAD_ID_WRITE(x) Asm("mcr p15, 0, %0, c13, c0, 4":: "r"(x))

#define CP15_BPI_INVALIDATEALL(x)           Asm("mcr p15, 0, %0, c7, c5, 6"::"r"(x))
#define CP15_DCACHE_INVALIDATE(x)           Asm("mcr p15, 0, %0, c7, c6, 2"::"r"(x))
#define CP15_DCACHE_CLEAN(x)                Asm("mcr p15, 0, %0, c7, c10,2"::"r"(x))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE(x) Asm("mcr p15, 0, %0, c7, c14,2"::"r"(x))
#define CP15_PBUFFER_FLUSH()                Asm("isb":::"memory")
#define CP15_DATA_SYNC_BARRIER()            Asm("dsb":::"memory")
#define CP15_DATA_MEMORY_BARRIER()          Asm("dmb":::"memory")
#define CP15_CSSELR_WRITE(x)                Asm("mcr p15, 2, %0, c0, c0, 0"::"r"(x))
#define CP15_CCSIDR_READ(x)                 Asm("mrc p15, 1, %0, c0, c0, 0":"=r"(x))
#define CP15_CLIDR_READ(x)                  Asm("mrc p15, 1, %0, c0, c0, 1":"=r"(x))

#define CP15_INVALIDATE_UNIFIED_TLB() Asm("mcr p15, 0, %0, c8, c7,  0"::"r"(0))
#define CP15_TLB_INVALIDATE_ALL()     Asm("mcr p15, 0, %0, c8, c7, 0" ::"r"(0))
#define CP15_TTBCR_WRITE(x)   Asm("mcr p15, 0, %0, c2, c0, 2"::"r"(x))
#define CP15_TTB0_READ(x)     Asm("mrc p15, 0, %0, c2, c0, 0":"=r"(x))
#define CP15_TTB0_WRITE(x)    Asm("mcr p15, 0, %0, c2, c0, 0"::"r"(x))
#define CP15_DOMAINS_WRITE(x) Asm("mcr p15, 0, %0, c3, c0, 0":: "r"(x))
#define CP15_NSACR_WRITE(x)   Asm("mcr p15, 0, %0, c1, c1, 2"::"r"(x))
#define CP15_NSACR_READ(x)    Asm("mrc p15, 0, %0, c1, c1, 2":"=r"(x))
#define CP15_SET_VBAR(x)      Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))

/*
 *  ステータスレジスタ（CPSR）の現在値の読出し
 */
extern UW current_sr(void);

/*
 *  ステータスレジスタ（CPSR）の現在値の変更
 */
extern void set_sr(UW sr);

#endif	/* _MACRO_ONLY */

#endif /* _CPU_INSN_H */
