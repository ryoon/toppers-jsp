/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: sh1.h,v 1.4 2002/04/11 11:30:20 imai Exp $
 */

#ifndef _SH1_H_
#define _SH1_H_

/*
 *  数値データ文字列化用マクロ
 */
#define TO_STRING(arg)	#arg
#define _TO_STRING(arg)	TO_STRING(arg)



/*
 *  割込みのベクタ番号定義
 */
#define GII	 4	/*  一般不当命令:General Illegal Instruction  */
#define SII	 6	/*  スロット不当命令:Slot Illegal Instruction */
#define CAE	 9	/*  CPUアドレスエラー:CPU Address Error       */
#define DAE	10	/*  DMAアドレスエラー:DMA Address Error       */

#define NMI	11	/*  NMI  		*/
#define USBK	12	/*  ユーザーブレーク  	*/
#define IRQ0	64	/*  外部割込み要求  	*/
#define IRQ1	65
#define IRQ2	66
#define IRQ3	67
#define IRQ4	68
#define IRQ5	69
#define IRQ6	70
#define IRQ7	71

	/*  DMAC:ダイレクトメモリアクセスコントローラ  */
#define DEI0	72	/*  DMAC0  */
#define DEI1	74	/*  DMAC1  */
#define DEI2	76	/*  DMAC2  */
#define DEI3	78	/*  DMAC3  */

	/*  ITU:インテクレーテッドタイマパルスユニット  */
#define IMIA0	80	/*  ITU0  */
#define IMIB0	81
#define OVI0	82
#define IMIA1	84	/*  ITU1  */
#define IMIB1	85
#define OVI1	86
#define IMIA2	88	/*  ITU2  */
#define IMIB2	89
#define OVI2	90
#define IMIA3	92	/*  ITU3  */
#define IMIB3	93
#define OVI3	94
#define IMIA4	96	/*  ITU4  */
#define IMIB4	97
#define OVI4	98

	/*  SCI:シリアルコミュニケーションインターフェース  */
#define ERI0	100	/*  SCI0  */
#define RXI0	101
#define TXI0	102
#define TEI0	103
#define ERI1	104	/*  SCI1  */
#define RXI1	105
#define TXI1	106
#define TEI1	107

#define PEI	108	/*  PRT:バスコントローラのパリティ制御  */
#define ADI	109	/*  A/Dコンバータ  			*/
#define ITI	112	/*  WDT:ウォッチドックタイマ  		*/
#define CMI	113	/*  REF:DRAMリフレッシュ制御  		*/



#ifndef _MACRO_ONLY

/*
 *  SH1の内部レジスタ定義
 */
typedef unsigned char   byte;
typedef volatile byte   IOREG;
typedef unsigned short  word;
typedef volatile word   HIOREG;
typedef volatile int    LIOREG;


/*
 *   バスステートコントローラ
 */
#define	WCR1	((HIOREG *)0x5ffffa2)	/*  ウェイトコントロールレジスタ1 */
#define	WCR2	((HIOREG *)0x5ffffa4)	/*  ウェイトコントロールレジスタ2 */
#define	WCR3	((HIOREG *)0x5ffffa6)	/*  ウェイトコントロールレジスタ3 */

			/*  エリア7：リードサイクルでWAIT信号による  	*/
			/*  ウェートステート挿入あり  			*/
#define    WCR1_RW7		0x8000
			/*  エリア3：リードサイクルでWAIT信号による  	*/
			/*  ウェートステート挿入あり  			*/
#define    WCR1_RW3		0x800
			/*  エリア0：リードサイクルでWAIT信号による  	*/
			/*  ウェートステート挿入あり  			*/
#define    WCR1_RW0		0x100
			/*  エリア0,2：1ステートロングウェイト  	*/
#define    WCR3_A02LW_MASK	~0x6000

/*
 * ピンファンクションコントローラ
 */
#define	PAIOR	((HIOREG *)0x5ffffc4)	/*  ポートA I/Oレジスタ  	  */
#define	PACR1	((HIOREG *)0x5ffffc8)	/*  ポートA コントロールレジスタ1 */
#define	PACR2	((HIOREG *)0x5ffffca)	/*  ポートA コントロールレジスタ2 */
#define	PBIOR	((HIOREG *)0x5ffffc6)	/*  ポートB I/Oレジスタ  	  */
#define	PBCR1	((HIOREG *)0x5ffffcc)	/*  ポートB コントロールレジスタ1 */
#define	PBCR2	((HIOREG *)0x5ffffce)	/*  ポートB コントロールレジスタ2 */
#define	PADR	((HIOREG *)0x5ffffc0)	/*  ポートA データレジスタ  	  */

#define PBCR1_TD0_RD0_MASK 	~0xf	/*  TxD0,RxD0端子設定用マスク	*/
#define PBCR1_TD0 		 0x8	/*  TxD0端子設定用マクロ  	*/
#define PBCR1_RD0 		 0x2	/*  RxD0端子設定用マクロ  	*/


/*
 * 割り込みコントローラレジスタ
 */
#define	IPRA	((HIOREG *)0x5ffff84)	/*  割込みレベル設定レジスタ  */
#define	IPRB	((HIOREG *)0x5ffff86)
#define	IPRC	((HIOREG *)0x5ffff88)
#define	IPRD	((HIOREG *)0x5ffff8a)
#define	IPRE	((HIOREG *)0x5ffff8c)
#define	ICR	((HIOREG *)0x5ffff8e)	/*  割込みコントロールレジスタ  */



/*
 * タイマーレジスタ
 *　　ITU:インテグレーテッド･タイマ･パルス・ユニット
 */
typedef struct{
    	/*  共通  */
    IOREG  TSTR;	/*  タイマスタートレジスタ  			*/
    IOREG  TSNC;	/*  タイマシンクロレジスタ  			*/
    IOREG  TMDR;	/*  タイマモードレジスタ  			*/
    IOREG  TFCR;	/*  タイマファンクションコントロールレジスタ  	*/
	/*  注意  */
    	/*  タイマアウトプットコントロールレジスタTOCRはここではない  	*/

	/*  ITU0  */
    IOREG  TCR0;	/*  タイマコントロールレジスタ0			*/
    IOREG  TIOR0;	/*  タイマI/Oコントロールレジスタ0		*/
    IOREG  TIER0;	/*  タイマインタラプトイネーブルレジスタ0	*/
    IOREG  TSR0;	/*  タイマステータスレジスタ0			*/
    HIOREG TCNT0;	/*  タイマカウンタ0		 	 	*/
    HIOREG GRA0;	/*  ジェネラルレジスタA0	 	 	*/
    HIOREG GRB0;	/*  ジェネラルレジスタB0	 	 	*/

	/*  ITU1  */
    IOREG  TCR1;	/*  タイマコントロールレジスタ1			*/
    IOREG  TIOR1;	/*  タイマI/Oコントロールレジスタ1		*/
    IOREG  TIER1;	/*  タイマインタラプトイネーブルレジスタ1	*/
    IOREG  TSR1;	/*  タイマステータスレジスタ1			*/
    HIOREG TCNT1;	/*  タイマカウンタ1		 	 	*/
    HIOREG GRA1;	/*  ジェネラルレジスタA1	 	 	*/
    HIOREG GRB1;	/*  ジェネラルレジスタB1	 	 	*/

	/*  ITU2  */
    IOREG  TCR2;	/*  タイマコントロールレジスタ2			*/
    IOREG  TIOR2;	/*  タイマI/Oコントロールレジスタ2		*/
    IOREG  TIER2;	/*  タイマインタラプトイネーブルレジスタ2	*/
    IOREG  TSR2;	/*  タイマステータスレジスタ2			*/
    HIOREG TCNT2;	/*  タイマカウンタ2		 	 	*/
    HIOREG GRA2;	/*  ジェネラルレジスタA2	 	 	*/
    HIOREG GRB2;	/*  ジェネラルレジスタB2	 	 	*/

	/*  ITU3  */
    IOREG  TCR3;	/*  タイマコントロールレジスタ3			*/
    IOREG  TIOR3;	/*  タイマI/Oコントロールレジスタ3		*/
    IOREG  TIER3;	/*  タイマインタラプトイネーブルレジスタ3	*/
    IOREG  TSR3;	/*  タイマステータスレジスタ3			*/
    HIOREG TCNT3;	/*  タイマカウンタ3		 	 	*/
    HIOREG GRA3;	/*  ジェネラルレジスタA3	 	 	*/
    HIOREG GRB3;	/*  ジェネラルレジスタB3	 	 	*/
    HIOREG BRA3;	/*  バッファレジスタA3	 	 		*/
    HIOREG BRB3;	/*  バッファレジスタB3	 	 		*/

    	/*  共通  */
    char   dummy;	/*  アラインメント調整  			*/
    IOREG  TOCR;	/*  タイマアウトプットコントロールレジスタ  	*/

	/*  ITU4  */
    IOREG  TCR4;	/*  タイマコントロールレジスタ4			*/
    IOREG  TIOR4;	/*  タイマI/Oコントロールレジスタ4		*/
    IOREG  TIER4;	/*  タイマインタラプトイネーブルレジスタ4	*/
    IOREG  TSR4;	/*  タイマステータスレジスタ4			*/
    HIOREG TCNT4;	/*  タイマカウンタ4		 	 	*/
    HIOREG GRA4;	/*  ジェネラルレジスタA4	 	 	*/
    HIOREG GRB4;	/*  ジェネラルレジスタB4	 	 	*/
    HIOREG BRA4;	/*  バッファレジスタA4	 	 		*/
    HIOREG BRB4;	/*  バッファレジスタB4	 	 		*/
} tmu;

#define ITU  (*(volatile tmu *)0x5ffff00)

#define TSR_IMFA	0x1	/*  GRAコンペアマッチフラグ  		*/
#define TSTR_STR0	0x1	/*  タイマスタートビット  		*/
#define TCR_CCLR_GRA	0x20	/*  GRAコンペアマッチでカウンタをクリア */
#define TIER_RESERVE	0x78	/*  TIERレジスタの予約ビット  		*/
#define TIER_IMIEA	0x1	/*  GRAコンペアマッチによる割込み要求を許可 */
#define TIOR_RESERVE	0x8	/*  TIORレジスタの予約ビット  		*/
#define TIOR_IOA	0x0	/*  GRAコンペアマッチによる端子出力禁止 */



/*
 *  シリアルコミュニケーションインターフェース(SCI)
 */
typedef struct{
	/*  チャネル0  */
    IOREG  SMR0;	/*  シリアルモードレジスタ  			*/
    IOREG  BRR0;	/*  ビットレートレジスタ  			*/
    IOREG  SCR0;	/*  シリアルコントロールレジスタ		*/
    IOREG  TDR0;	/*  トランスミットデータレジスタ  		*/
    IOREG  SSR0;	/*  シリアルステータスレジスタ  		*/
    IOREG  RDR0;	/*  レシーブデータレジスタ  			*/
    char   dummy1;	/*  アラインメント調整  			*/
    char   dummy2;	/*  アラインメント調整  			*/

	/*  チャネル1  */
    IOREG  SMR1;	/*  シリアルモードレジスタ  			*/
    IOREG  BRR1;	/*  ビットレートレジスタ  			*/
    IOREG  SCR1;	/*  シリアルコントロールレジスタ		*/
    IOREG  TDR1;	/*  トランスミットデータレジスタ  		*/
    IOREG  SSR1;	/*  シリアルステータスレジスタ  		*/
    IOREG  RDR1;	/*  レシーブデータレジスタ  			*/
} sci;

#define SCI (*(volatile sci *)0x5fffec0)

/*  シリアルステータスレジスタSSRの各ビット  */
#define SSR_TDRE	0x80	/* トランスミットデータレジスタエンプティ */
#define SSR_RDRF	0x40	/* レシーブデータレジスタフル 		*/
#define SSR_ORER	0x20	/* オーバーランエラー 			*/
#define SSR_FER		0x10	/* フレーミングエラー 			*/
#define SSR_PER		0x08	/* パリティエラー 			*/
#define SSR_TEND	0x04	/* トランスミットエンド 		*/
#define SSR_MPB		0x02	/* マルチプロセッサビット 		*/
#define SSR_MPBT	0x01	/* マルチプロセッサビットトランスファ 	*/

/*  シリアルコントロールレジスタSCRの各ビット  */
#define SCR_TIE		0x80	/* トランスミットインタラプトイネーブル */
#define SCR_RIE		0x40	/* レシーブインタラプトイネーブル 	*/
#define SCR_TE		0x20	/* トランスミットイネーブル 		*/
#define SCR_RE		0x10	/* レシーブイネーブル 			*/
#define SCR_MPIE	0x08	/* マルチプロセッサインタラプトイネーブル */
				/* トランスミットエンドインタラプト	*/
#define SCR_TEIE	0x04	/* イネーブル 				*/
#define SCR_CKE_MASK	0x03	/* クロックソース選択用マスク		*/
#define SCR_CKE		0x00	/* クロックイネーブル			*/
				/*   調歩同期式のとき  			*/
				/*   	内部クロック使用  		*/
				/*   	出力なし  			*/

/*  シリアルモードジスタSMRの各ビット  */
				/* コミュニケーションモード 	*/
#define SMR_CA_CLOCK	0x80	/* 	クロック同期式 		*/
#define SMR_CA_ASYNC	0x00	/* 	調歩同期式通 		*/
	/*  調歩同期式通信方：Asynchronous Communication method */
				/* キャラクタレングス 		*/
#define SMR_CHR8	0x00	/* 	8ビット 		*/
#define SMR_CHR7	0x40	/* 	7ビット 		*/
				/* パリティイネーブル 		*/
#define SMR_PE		0x20	/* 	パリティあり 		*/
#define SMR_PE_NON	0x00	/* 	パリティなし 		*/
#define SMR_OE_ODD	0x10	/* パリティモード（奇数パリティ）*/
#define SMR_STOP2	0x08	/* ストップビットレングス：2 	*/
#define SMR_STOP1	0x00	/* ストップビットレングス：1 	*/
#define SMR_MP		0x04	/* マルチプロセッサモード 	*/
/*  クロックの分周比はボード依存（sys_config.h）  */


/*
 *  gdb stubによる出力	注意：スタブ完成後に使用
 */

Inline int
stub_putc(int c)
{
  Asm("mov #0x00,r0; mov %0,r4; trapa #0x3f"
               : /* no output */
               : "r"(c)
               : "r0","r4");
  return(c);
}

/*
 *  SCIによる出力	注意
 */

Inline int
sci_putc(int c)
{
    return(c);
}


/*
 *  メモリブロック操作ライブラリ
 *	(ItIsからの流用)
 *
 *  関数の仕様は，ANSI C ライブラリの仕様と同じ．標準ライブラリのものを
 *  使った方が効率が良い可能性がある．
 *
 */

VP _dummy_memcpy(VP dest, VP src, UINT len);

VP memcpy(VP dest, VP src, UINT len);

#endif /* _MACRO_ONLY */

#endif /* _SH1_H_ */
