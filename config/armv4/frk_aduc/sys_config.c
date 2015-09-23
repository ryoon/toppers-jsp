/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2006 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2006 by Logic Research Co., Ltd.
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
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
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 */

/*
 *    ターゲットシステム依存モジュール（FRK_ADuC用）
 */

#include "jsp_kernel.h"
#include "frk_aduc.h"


/*interrupt mask*/
const UW int_mask_table[MAX_INT_NUM]={
	MASK_ALL_INT,
	MASK_IRQ_SWI,
	MASK_IRQ_TIMER0,
	MASK_IRQ_TIMER1,
	MASK_IRQ_TIMER2,
	MASK_IRQ_TIMER3,
	MASK_IRQ_FLA_CTRL,
	MASK_IRQ_ADC_CH,
	MASK_IRQ_PLL_LOCK,
	MASK_IRQ_I2C0_SLV,
	MASK_IRQ_I2C0_MST,
	MASK_IRQ_I2C1_MST,
	MASK_IRQ_SPI_SLV,
	MASK_IRQ_SPI_MST,
	MASK_IRQ_UART,
	MASK_IRQ_EXT_IRQ0,
	MASK_IRQ_COMP,
	MASK_IRQ_PSM,
	MASK_IRQ_EXT_IRQ1,
	MASK_IRQ_PLA_IRQ0,
	MASK_IRQ_PLA_IRQ1,
	MASK_IRQ_EXT_IRQ2,
	MASK_IRQ_EXT_IRQ3,
	MASK_IRQ_PWM_TRIP
};


/*
 * 割込みハンドラ登録用テーブル
 */
FP   int_table[MAX_INT_NUM];


/*
 * OSCの初期化
 */
void
init_clock(void)
{
    UW    reg;

    /* POWCON key1 */
    sil_wrw_mem((VP)POWKEY1, POWKEY1_DATA);

    /* Core Clock 41.78MHz , Active mode*/
    reg = (POWCON_PC_ACT | POWCON_CD_41M);
    sil_wrw_mem((VP)POWCON, reg);

    /* POWCON key2 */
    sil_wrw_mem((VP)POWKEY2, POWKEY2_DATA);
}


/*
 * GPIOの初期化
 */
void
init_gpio(void)
{
	/* LED port */
	sil_wrw_mem((VP)GP4CON, 0);			/* GPIO mode */
	sil_wrw_mem((VP)GP4DAT, P47_DIR);	/* Output */
	sil_wrw_mem((VP)GP4SET, LED);		/* LED off */

	/* UART port */
	sil_wrw_mem((VP)GP0CON, GP07_SIN);
	sil_wrw_mem((VP)GP1CON, (GP10_SIN|GP11_SOUT|GP12_RTS|
							 GP13_CTS|GP14_RI|GP15_DCD|
							 GP16_DSR|GP17_DTR));
	sil_wrw_mem((VP)GP2CON, GP20_SOUT);

}


/*
 *  割込み関連の初期化
 */
void
init_IRQ(void)
{
    int i;
    
    /* IRQ割込み禁止 */
    sil_wrw_mem((VP)IRQCLR, INT_DISABLE_ALL);
    
    /*
     *  割込みテーブルの初期化
     */
    for(i = 0; i < MAX_INT_NUM; i++){
        int_table[i] = 0x00;        /* リセット */   
    }
}


/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
    int i;

    init_clock();

	init_gpio();

    init_IRQ();

	/*
	 * SRAM
	 */
    for(i = 0; i <=7; i++){
        arm_vector_add[i] = arm_vector_add[i] + VCT_TB_SRAM/4;
    }

  	for(i=0; i <=14; i++) {
		*((volatile UW *)VCT_TB_SRAM + i) = *((volatile UW *)VCT_TB + i);
	}

 	sil_wrw_mem((VP)REMAP, REMAP_SRAM);

    /*
     *  ARMのベクタテーブルへ割込みハンドラを登録
     */
    arm_install_handler(IRQ_Number, IRQ_Handler);

    /*
     *  sys_putc が可能になるようにUARTを初期化
     */
    init_uart();
}


/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
    syslog(LOG_EMERG, "End Kernel.....!");
    while(1);
}


/*
 *  ターゲットシステムの文字出力
 */

void
sys_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
}


/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
void
define_inh(INHNO inhno, FP inthdr)
{
    assert(inhno < MAX_INT_NUM);
    int_table[inhno] = inthdr;

}

/*
 *  未定義の割込みが入った場合の処理
 */
void
undef_interrupt(){
    syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
    while(1);
}


/*IRQレジスタがクリアされるまで待つ*/
#define INT_CLEAR_DELAY  0x70
void poll_intreg(int int_mask)
{
    int i;
    
    //while(*IRQEN & int_mask);
    for(i=0;i<INT_CLEAR_DELAY;i++);
    enaint();

}

