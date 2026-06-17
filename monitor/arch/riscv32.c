/*
 *  TOPPERS/JSP Educative Program
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2003-2016 by Ryosuke Takeuchi
 *                     GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)～(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: riscv32.c,v 1.1 2025/11/05 09:47:21 roi Exp $
 */

/* 
 *  TOPPERS/JSP用タスクモニタCPU依存プログラム．
 *
 */

#include <t_services.h>
#include <sil.h>
#include <stdio.h>
#include "task_expansion.h"
#include "monitor.h"

extern void _kernel_break_wait();

#define set_align(a, b)	((ULONG)a & ~(b-1))

/*
 * レジスタの構造体
 */
typedef struct t_reg{
	uint32_t gp;
	uint32_t tp;
	uint32_t s0;
	uint32_t s1;
	uint32_t s2;
	uint32_t s3;
	uint32_t s4;
	uint32_t s5;
}T_REG;

/*
 * メモリのマッピング定義構造体
 */

typedef struct t_memdef{
	uint32_t mstart;
	uint32_t mend;
	uint8_t  mtype;
	uint8_t  mstate;
}T_MEMDEF;

/*
 * RISC-V/32のメモリマッピング
 */

static T_MEMDEF const memdefine[] = {
#if defined(SAPPHIRESOC)
	{0x00001000, 0x0FFFFFFF,           MEMORY_AREA, MREAD_WRITE },
	{0xE1000000, 0xEFFFFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xF8000000, 0xF8FFFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xF9000000, 0xF9FFFFFF,           MEMORY_AREA, MREAD_WRITE},
#elif defined(RP2350A)
	{0x10000000, 0x107FFFFF,           MEMORY_AREA, MREAD_ONLY },
	{0x20000000, 0x203FFFFF,           MEMORY_AREA, MREAD_WRITE},
	{0x40000000, 0x5FFFFFFF,           PORT_AREA,   MREAD_WRITE},
	{0xD0000000, 0xDFFFFFFF,           PORT_AREA,   MREAD_WRITE},
#else
#error "No support board type in RISC-V/32 groups."
#endif
};

static T_REG  sreg;

/******************************************************************************
 * ハードウェアポート属性参照関数
 ******************************************************************************/
/*
 *  アドレスからメモリ領域属性を取り出す
 *  mode=0:領域の型
 *  mode=1:読み取り書き込み属性
 */

char
getMemoryType(ULONG address, INT mode)
{
	INT count = sizeof(memdefine) / sizeof(T_MEMDEF);
	INT i;

	for(i = 0 ; i < count ; i++){
		if(address >= memdefine[i].mstart && address <= memdefine[i].mend){
			if(mode == 0)
				return memdefine[i].mtype;
			else
				return memdefine[i].mstate;
		}
	}
	return NONE_AREA;
}

/*
 *  アドレスからアライン後のアドレスを取り出す
 */

ULONG
MonAlignAddress(ULONG address)
{
	return address;
}

/******************************************************************************
 * メモリアクセス用関数
 ******************************************************************************/
/*
 *  メモリ領域に対する読み出し関数
 *  領域のチェックを行い、エラーならゼロを返す
 */
INT
MemoryRead(ULONG address, VP_INT p, INT type)
{
	INT len;

	switch(getMemoryType(address, 0)){
	case PORT_AREA:
		if(type == 2){
			len = 2;
			*((uint16_t *)p) = sil_reh_mem((uint16_t *)address);
		}
		else if(type == 4){
			len = 4;
			*((uint32_t *)p) = sil_rew_mem((uint32_t *)address);
		}
		else{
			len = 1;
			*((uint8_t *)p) = sil_reb_mem((uint8_t *)address);
		}
		break;
	case MEMORY_AREA:
		if(type == 2){
			len = 2;
			*((uint16_t *)p) = *((uint16_t *)set_align(address, len));
		}
		else if(type == 4){
			len = 4;
			*((uint32_t *)p) = *((uint32_t *)set_align(address, len));
		}
		else{
			len = 1;
			*((uint8_t *)p) = *((uint8_t *)address);
		}
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

/*
 *  メモリ領域に対する書き込み関数
 *  領域のチェックを行い、エラーならゼロを返す
 */
INT
MemoryWrite(ULONG address, VP_INT p, INT type)
{
	INT len;

	switch(getMemoryType(address, 0)){
	case PORT_AREA:
		if(type == 2){
			len = 2;
			address = set_align(address, len);
			sil_wrh_mem((uint16_t *)address, *((uint16_t *)p));
		}
		else if(type == 4){
			len = 4;
			address = set_align(address, len);
			sil_wrw_mem((uint32_t *)address, *((uint32_t *)p));
		}
		else{
			len = 1;
			sil_wrb_mem((uint8_t *)address, *((uint8_t *)p));
		}
		break;
	case MEMORY_AREA:
		if(getMemoryType(address, 1) == MREAD_ONLY){
			len = 0;
		}
		else if(type == 2){
			len = 2;
			*((uint16_t *)address) = *((uint16_t *)set_align(p, len));
		}
		else if(type == 4){
			len = 4;
			*((uint32_t *)address) = *((uint32_t *)set_align(p, len));
		}
		else{
			len = 1;
			*((uint8_t *)address) = *((uint8_t *)p);
		}
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

/******************************************************************************
 * モニタ用関数
 ******************************************************************************/
/*
 *  レジスタ内容の表示
 */
void
display_registers(ID tskid)
{
	ER     ercd;
	T_RTST rtst;

	ercd = ref_tst(tskid, &rtst);
	if(ercd == E_OK){
		if(rtst.tskpc == (FP)_kernel_break_wait){
			sreg = *((T_REG *)rtst.tsksp);
			printf(" PC  =%08lx SP =%08lx\n", (long)rtst.tskpc, (long)((long)rtst.tsksp/*+sizeof(T_REG)*/));
			printf(" GP  =%08lx TP =%08lx S0 =%08lx S1 =%08lx\n", (long)sreg.gp, (long)sreg.tp, (long)sreg.s0, (long)sreg.s1);
			printf(" S2  =%08lx S3 =%08lx S4 =%08lx S5 =%08lx\n", (long)sreg.s2, (long)sreg.s3, (long)sreg.s4, (long)sreg.s5);
			if(rtst.tskpc != NULL)
				printf("     %08lx    %04x\n", (long)rtst.tskpc, *((uint16_t *)((uint32_t)rtst.tskpc & ~1)));
			return;
		}
		else if(rtst.tskstat == TTS_DMT){
			printf("  wait in activate_r() !!\n");
			return;
		}
	}
	printf("  wait in dispatch() !!\n");
}

