/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: hw_serial.h,v 1.4 2001/10/20 15:39:15 honda Exp $
 */


#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（MS7709ASE01用）
 *  スーパーI/O(FDC37C935A)上のシリアル1を使用
 */

#include "sh3.h"
#include "sys_config.h"
#include "cpu_config.h"
#include "ms7709ase01.h"

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL	 UART_IRQ0


/*
 *  シリアルポートのハードウェア依存情報の定義
 *  Not yet! m68kのようにコントロールレジスタを構造体にする。
 */
typedef struct hardware_serial_port_descripter {
    INT	  *initflag;	   /* 初期化済フラグへのポインタ   */
    BOOL  sendflag;        /* 送信割り込みイネーブルフラグ */
} HWPORT;



/*
 * 割り込みレベルは固定である．
 */
#define SCI1INTLVL   8


/*
 *  ボーレートの設定
 *  19200bpsに設定
 */

#define BPS_SETTING 0x0006

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	1	/* サポートするシリアルポートの数 */

static INT	initflag[1] = { 0 } ;	/* 初期化済フラグ */

#define HWPORT1	{ &initflag[0],  0 }


/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
    HIOREG dummy;

    /* BIT7 = 1 */
    *SMSC_SCI0_LCR = 0x83 << 8;

    /* Set BPS */
    *SMSC_SCI0_DLL = ((BPS_SETTING & 0x00ff) << 8);   
    *SMSC_SCI0_DLM = ((BPS_SETTING >> 8) << 8);

    /* BIT7 = 0 Divisor Latch BIT6 = 0 No Break : BIT3 = 0 NoParity
       : BIT2 = 0 1Stopbit : BIT1,0 = {1.1} 8bitData */    
    *SMSC_SCI0_LCR = 0x03 << 8;

    /* Do not use FIFO */
    *SMSC_SCI0_FCR = 0x0000;

    /* Clear Status */
    dummy = *SMSC_SCI0_RBR;
    dummy = *SMSC_SCI0_LSR;
    
    
    
    /* Enable Receive Data Interrupt */
    *SMSC_SCI0_IER = (0x01 << 8);
    *SMSC_SCI0_MCR = 0x08 << 8;
    

    /*
     * 割り込み関連の設定
     * エラーハンドラも登録すべき
     */
    if (initflag[0] <= 0) {
        define_int_plevel(INHNO_SERIAL,SCI1INTLVL);
    }

    /*
     * MS7729RSE01では，一度送信割込みを発生させておかないと割込み
     * レベル16の割込みが発生してしまい正しく動作しない．
     */
    /* 送信割り込み要求を許可 */
    *SMSC_SCI0_IER = (((*SMSC_SCI0_IER >> 8) | 0x02) << 8);   
    /* 送信割り込み要求を禁止 */        
    *SMSC_SCI0_IER = (((*SMSC_SCI0_IER >> 8) & ~0x02) << 8);   
    
    *(p->initflag) = 1;			/* 初期化フラグ設定 */
    return(FALSE);
}

/*
 *  シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
	*(p->initflag) = -1;		/* 初期化フラグ設定           */
    *SMSC_SCI0_IER   = 0x00;      /* 割込みの禁止               */
}

/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void	serial_handler_in(int portid);
extern void	serial_handler_out(int portid);


/*
 *  シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler()
{
    HIOREG dummy;
    
    if (initflag[0] > 0){
        serial_handler_in(1);
        serial_handler_out(1);
    }
    else
        dummy = *SMSC_SCI0_LSR;
}

/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	return(((*SMSC_SCI0_LSR >> 8) & 0x01) != 0);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	return(((*SMSC_SCI0_LSR >> 8) & 0x60) != 0);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
    return (*SMSC_SCI0_RBR >> 8);
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
    *SMSC_SCI0_THR = c << 8;
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
    if (!(p->sendflag)) {
        /* 送信割り込み要求を許可 */
        *SMSC_SCI0_IER = (((*SMSC_SCI0_IER >> 8) | 0x02) << 8);   
        p->sendflag = 1;
    }
}

Inline void
hw_port_sendstop(HWPORT *p)
{
	if (p->sendflag) {
        /* 送信割り込み要求を禁止 */        
        *SMSC_SCI0_IER = (((*SMSC_SCI0_IER >> 8) & ~0x02) << 8);   
        p->sendflag = 0;
	}
}

#endif /* _HW_SERIAL_H_ */





















