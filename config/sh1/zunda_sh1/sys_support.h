/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2003-2004 by CHUO ELECTRIC WORKS Co.,LTD.
 *   51 Tateminami, Watari-cyo, Watari-gun, Miyagi-ken 989-2361, JAPAN
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
#ifndef _SYS_SUPPORT_H_
#define _SYS_SUPPORT_H_

/*
 *  ＰＡ、ＰＢ出力バッファ初期値
 *  下のピンファンクション定義を参照してください
 */
#define	VAL_PA_DR 	(1<<10 | 1<<9 | 1<<8)	/* RTC-4553-CS=1,RTC-4553-WR=1,LED1=1(off) */
#define	VAL_PB_DR 	(1<<11 | 1<<9 | 1<<6)	/* TXD1=1,TXD0=1,LED2=1(off) */

/*
 *  ピンファンクション設定値
 *  下のピンファンクション定義を参照してください
 */
#define VAL_PFC_PAIOR   0xf7aa
#define VAL_PFC_PACR1   0x0002
#define VAL_PFC_PACR2   0x3b11

#define VAL_PFC_PBIOR   0x1a7b
#define VAL_PFC_PBCR1   0x14aa
#define VAL_PFC_PBCR2   0x0000

#define VAL_PFC_CASCR   0x5fff

/*
 *  ピンファンクション定義
 *
 *  PORTA
 *
 *  PNO  I/O(1/0) PACR1/2[xx]
 *  PA15 OUT (1)  [00] − 拡張Ｉ／Ｏに開放（IRQ3／DREQ1）
 *  PA14 OUT (1)  [00] − 拡張Ｉ／Ｏに開放（IRQ2／DACK1）
 *  PA13 OUT (1)  [00] − 拡張Ｉ／Ｏに開放（IRQ1／DREQ0／TCLKB）
 *  PA12 OUT (1)  [00] − 拡張Ｉ／Ｏに開放（IRQ0／DACK0／TCLKA）
 *  PA11 IN  (0)  [00] − RTC-4553-SOUT
 *  PA10 OUT (1)  [00] − RTC-4553-CS(active-low)
 *  PA09 OUT (1)  [00] − RTC-4553-WR(active-low)
 *  PA08 OUT (1)  [00] − LEDCTL1 LED1制御(active-low)
 *  PA07 OUT (1)  [00] − 未使用
 *  PA06 −  (0)  [11] − RD に割り当て−
 *  PA05 OUT (1)  [10] − WD-PULSE ウォッチドックリセットパルス↑↓＿にて
 *  PA04 −  (0)  [11] − WR に割り当て
 *  PA03 OUT (1)  [00] − 拡張Ｉ／Ｏに開放（CS7／WAIT）
 *  PA02 −  (0)  [01] − CS6 に割り当て（ＬＡＮコントローラ用）
 *  PA01 OUT (1)  [00] − 拡張Ｉ／Ｏに開放（CS5／TIOCA0）
 *  PA00 −  (0)  [01] − CS4 に割り当て（ROMICE 用）
 *  PAIOR=0xf7aa
 *  PACR1=0x0000
 *  PACR2=0x3b11
 *
 *
 *  PORTB
 *
 *  PB15 −  (0)  [00] − 未使用 IRQ7 に割り当て（ＲＴＣ用）
 *  PB14 −  (0)  [01] − IRQ6 に割り当て（ＵＳＢコントローラ用）
 *  PB13 −  (0)  [01] − IRQ5 に割り当て（ＬＡＮコントローラ用）
 *  PB12 OUT (1)  [00] − LEDCTL3 LED3 制御(active-low)
 *  PB11 −  (1)  [10] − TDX1 に割り当て（RS485 データ出力）
 *  PB10 −  (0)  [10] − RDX1 に割り当て（RS485 データ入力）
 *  PB09 −  (1)  [10] − TDX0 に割り当て（RS-232C データ出力）
 *  PB08 −  (0)  [10] − RDX0 に割り当て（RS-232C データ入力）
 *  PB07 IN  (0)  [00] − シリアルEEPROM-DO
 *  PB06 OUT (1)  [00] − LEDCTL2 LED2 制御(active-low)
 *  PB05 OUT (1)  [00] − TXE0 RS485 送信イネーブル出力送信停止
 *  PB04 OUT (1)  [00] − シリアルEEPROM-CS(active-high)
 *  PB03 OUT (1)  [00] − CTS0 RS232C のCTS 信号（未実装）
 *  PB02 IN  (0)  [00] − RTS0 RS232C のRTS 信号（未実装）
 *  PB01 OUT (1)  [00] − シリアルEEPROM-SK、RTC-4553-SCK
 *  PB00 OUT (1)  [00] − シリアルEEPROM-DI、RTC-4553-SIN
 *  PBIOR=0x1a7b
 *  PBCR1=0x14aa
 *  PBCR2=0x0000
 *
 */

#ifndef _MACRO_ONLY
/*
 * 関数宣言
 */
void	software_init_hook(void);
void	hardware_init_hook(void);

#endif /* _MACRO_ONLY */
#endif	/* _SYS_SUPPORT_H_ */
