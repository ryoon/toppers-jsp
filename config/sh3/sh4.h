
/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: sh4.h,v 1.1 2001/11/05 13:31:13 honda Exp $
 */

#ifndef _SH4_H_
#define _SH4_H_

/*
 *  SH4(SH7750)の内部レジスタ定義
 */
typedef unsigned char   byte;
typedef volatile byte   IOREG;          
typedef unsigned short  word;
typedef volatile word   HIOREG;
typedef volatile int    LIOREG;


/*
 *  例外イベントレジスタ
 */
#define TRA	        0xff000020
#define EXPEVT	    0xff000024
#define INTEVT	    0xff000028


/*
 *   バスステートコントローラ関連
 */
#define	RFCR	((HIOREG *)(0xff800028))	

#define    RFCR_CLEAR         0xa400

/*
 * 割り込みによって INTEVT レジスタに設定される値
 */
#define    TMU0_INTEVT        0x400
#define    SCI_RXI_INTEVT     0x500
#define    SCI_TXI_INTEVT     0x520
#define    SCI_ERI_INTEVT     0x4e0
#define    SCIF_RXI_INTEVT    0x720
#define    SCIF_TXI_INTEVT    0x760
#define    SCIF_BRI_INTEVT    0x740


        
/*
 * 割込みコントロールレジスタ
 */
#define	ICR     ((HIOREG *)(0xffd00000))	
#define	IPRA	((HIOREG *)(0xffd00004))		
#define	IPRB	((HIOREG *)(0xffd00008))		
#define	IPRC	((HIOREG *)(0xffd0000c))	
#define	IPRD	((HIOREG *)(0xffd00010))	


/*
 * タイマーレジスタ
 */
typedef struct{
    IOREG  TOCR;
    HIOREG dummy1;
    IOREG  TSTR;
    IOREG  dummy2;
    LIOREG TCOR0;
    LIOREG TCNT0;
    HIOREG TCR0;
    LIOREG TCOR1;
    LIOREG TCNT1;
    HIOREG TCR1;
    LIOREG TCOR2;
    LIOREG TCNT2;
    HIOREG TCR2;
} tmu;

#define TMU  (*(volatile tmu *)0xffd80000)
                              
#define	TMU_STR0	0x01
#define	TMU_STR1	0x02
#define	TMU_STR2	0x04
#define TCR_UNF     0x0100    



/*
 *
 *  シリアルコミュニケーションインターフェース(SCI)
 *
 */
#define SCI_SCSMR1  ((IOREG *)(0xffe00000))
#define SCI_SCBRR1  ((IOREG *)(0xffe00004))
#define SCI_SCSCR1  ((IOREG *)(0xffe00008))
#define SCI_SCTDR1  ((IOREG *)(0xffe0000c))
#define SCI_SCSSR1  ((IOREG *)(0xffe00010))
#define SCI_SCRDR1  ((IOREG *)(0xffe00014))
#define SCI_SCSPTR1 ((IOREG *)(0xffe0001c))


#define	SCSMR_CHR	  0x0040
#define	SCSMR_PE	  0x0020
#define	SCSMR_OE	  0x0010
#define	SCSMR_STOP	  0x0008
#define	SCSMR_CKS1	  0x0002
#define	SCSMR_CKS0	  0x0001

#define SCSCR_TIE	  0x0080
#define SCSCR_RIE	  0x0040
#define SCSCR_TE	  0x0020
#define SCSCR_RE	  0x0010
#define SCSCR_CKE1	  0x0002
#define SCSCR_CKE0	  0x0001

#define SCSSR_TDRE	  0x80
#define SCSSR_RDRF	  0x40
#define SCSSR_ORER	  0x20
#define SCSSR_FER	  0x10
#define SCSSR_PER	  0x08
#define SCSSR_TEND	  0x04
#define SCSSR_MPB	  0x02
#define SCSSR_MPBT	  0x01



/*
 *
 *  FIFO付きシリアルコミュニケーションインターフェース(SCIF)レジスタ
 *
 */
#define SCIF_SCSMR2  ((HIOREG *)(0xffe80000))
#define SCIF_SCBRR2  ((IOREG  *)(0xffe80004))
#define SCIF_SCSCR2  ((HIOREG *)(0xffe80008))
#define SCIF_SCFTDR2 ((IOREG  *)(0xffe8000c))
#define SCIF_SCFSR2  ((HIOREG *)(0xffe80010))
#define SCIF_SCFRDR2 ((IOREG  *)(0xffe80014))
#define SCIF_SCFCR2  ((HIOREG *)(0xffe80018))
#define SCIF_SCFDR2  ((HIOREG *)(0xffe8001c))
#define	SCIF_SCSPTR2 ((HIOREG *)(0xffe80020))
#define	SCIF_SCLSR2	 ((HIOREG *)(0xffe80024))	


#define	SCSMR2_CHR	  0x0040
#define	SCSMR2_PE	  0x0020
#define	SCSMR2_OE	  0x0010
#define	SCSMR2_STOP	  0x0008
#define	SCSMR2_CKS1	  0x0002
#define	SCSMR2_CKS0	  0x0001

#define SCSCR2_TIE	  0x0080
#define SCSCR2_RIE	  0x0040
#define SCSCR2_TE	  0x0020
#define SCSCR2_RE	  0x0010
#define SCSCR2_CKE1	  0x0002
#define SCSCR2_CKE0	  0x0001

#define SCFSR2_ER	  0x0080
#define SCFSR2_TEND	  0x0040
#define SCFSR2_TDFE	  0x0020
#define SCFSR2_BRK	  0x0010
#define SCFSR2_FER	  0x0008
#define SCFSR2_PER	  0x0004
#define SCFSR2_RDF	  0x0002
#define SCFSR2_DR	  0x0001


#define SCFCR2_RTRG1  0x0080
#define SCFCR2_RTRG0  0x0040
#define SCFCR2_TTRG1  0x0020
#define SCFCR2_TTRG0  0x0010
#define SCFCR2_MCE	  0x0008
#define SCFCR2_TFRST  0x0004
#define SCFCR2_RFRST  0x0002
#define SCFCR2_LOOP	  0x0001


#define SCSPTR2_RTSIO  0x0080
#define SCSPTR2_RTSDT  0x0040
#define SCSPTR2_CTSIO  0x0020
#define SCSPTR2_CTSDT  0x0010
#define SCSPTR2_SPB2IO 0x0002
#define SCSPTR2_SPB2DT 0x0001



/*
 * キャッシュ制御レジスタ
 */
#define	CCR	((LIOREG *)0xff00001c)	



#endif /* _SH4_H_ */






