
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
 *  @(#) $Id: sh3.h,v 1.4 2002/04/05 07:43:51 honda Exp $
 */

#ifndef _SH3_H_
#define _SH3_H_

/*
 *  SH3(SH7709A,SH7709,SH7708)の内部レジスタ定義
 */
typedef unsigned char   byte;
typedef volatile byte   IOREG;          
typedef unsigned short  word;
typedef volatile word   HIOREG;
typedef volatile int    LIOREG;


/*
 *  例外イベントレジスタ
 */
#define TRA	        0xFFFFFFD0		
#define EXPEVT	    0xFFFFFFD4		
#define INTEVT	    0xFFFFFFD8		
#define INTEVT2	    0xA4000000		

/*
 *   バスステートコントローラ関連
 */
#define	RFCR	((HIOREG *)(0xffffff74))	

#define    RFCR_CLEAR         0xa400


/*
 * 割り込みによってINTEVT2レジスタに設定される値
 */
#define    TMU0_INTEVT        0x400
#define    IRQ0_INTEVT        0x600
#define    SCI_RXI_INTEVT     0x500
#define    SCI_TXI_INTEVT     0x520
#define    SCI_BRI_INTEVT     0x4ed
#define    SCI1_RXI_INTEVT    0x8a0
#define    SCI1_TXI_INTEVT    0x8e0
#define    SCI1_BRI_INTEVT    0x8c0
#define    SCIF_RXI_INTEVT    0x920
#define    SCIF_TXI_INTEVT    0x960
#define    SCIF_BRI_INTEVT    0x940


        
/*
 * 割り込みコントローラーレジスター
 */
#define	ICR0	((HIOREG *)(0xfffffee0))	
#define	ICR1	((HIOREG *)(0xa4000010))	
#define	ICR2	((HIOREG *)(0xa4000012))	
#define	PINTER	((HIOREG *)(0xa4000014))	
#define	IPRA	((HIOREG *)0xfffffee2)		
#define	IPRB	((HIOREG *)0xfffffee4)		
#define	IPRC	((HIOREG *)(0xa4000016))	
#define	IPRD	((HIOREG *)(0xa4000018))	
#define	IPRE	((HIOREG *)(0xa400001a))	
#define	IRR0	((IOREG  *)(0xa4000004))	
#define	IRR1	((IOREG  *)(0xa4000006))	
#define	IRR2	((IOREG  *)(0xa4000008))	



/*
 * タイマーレジスタ
 */
typedef struct{
    IOREG  TOCR;
    IOREG  dummy1;
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

#define TMU  (*(volatile tmu *)0xfffffe90)
                              
#define	TMU_STR0	0x01
#define	TMU_STR1	0x02
#define	TMU_STR2	0x04
#define TCR_UNF     0x0100    



/*
 *  シリアルコミュニケーションインターフェース(SCI)
 */
typedef struct{
    IOREG  SCSMR;
    IOREG  dummy1;    
    IOREG  SCBRR;
    IOREG  dummy2;    
    IOREG  SCSCR;
    IOREG  dummy3;        
    IOREG  SCTDR;
    IOREG  dummy4;            
    IOREG  SCSSR;
    IOREG  dummy5;            
    IOREG  SCRDR;
} sci;


#define SCI (*(volatile sci *)0xfffffe80)


/*
 *  シリアルコミュニケーションインターフェース(SCI)の
 *  ポートSCコントロール/データレジスタ
 *  SH7709及びSH7709Aのみ
 */
#define	SCPCR	((IOREG  *)(0xa4000136))	
#define	SCPDR	((HIOREG *)(0xa4000116))	


/*
 *  シリアルコミュニケーションインターフェース(SCI)の
 *  シリアルポートレジスタ
 *  SH7708のみ
 */
#define	SCSPTR	((HIOREG *)(0xffffff7c))	

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
 *  FIFO付きシリアルコミュニケーションインターフェース(SCIF)レジスタ
 *  SH7709及びSH7709Aのみ
 */

typedef struct{
    IOREG  SCSMR2;
    IOREG  dummy1;    
    IOREG  SCBRR2;
    IOREG  dummy2;    
    IOREG  SCSCR2;
    IOREG  dummy3;        
    IOREG  SCFTDR2;
    IOREG  dummy4;            
    HIOREG SCSSR2;
    IOREG  SCFRDR2;
    IOREG  dummy5;                
    IOREG  SCFCR2;
    IOREG  dummy6;                    
    HIOREG SCFDR2;
} scif;

#define SCIF (*(volatile scif *)0xa4000150)

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

#define SCSSR2_ER	  0x0080
#define SCSSR2_TEND	  0x0040
#define SCSSR2_TDFE	  0x0020
#define SCSSR2_BRK	  0x0010
#define SCSSR2_FER	  0x0008
#define SCSSR2_PER	  0x0004
#define SCSSR2_RDF	  0x0002
#define SCSSR2_DR	  0x0001


#define SCFCR2_RTRG1  0x0080
#define SCFCR2_RTRG0  0x0040
#define SCFCR2_TTRG1  0x0020
#define SCFCR2_TTRG0  0x0010
#define SCFCR2_MCE	  0x0008
#define SCFCR2_TFRST  0x0004
#define SCFCR2_RFRST  0x0002
#define SCFCR2_LOOP	  0x0001




/*
 *  IrDA (SCI1)レジスタ
 *  SH7709及びSH7709Aのみ
 */

typedef struct{
    IOREG  SCSMR1;
    IOREG  dummy1;    
    IOREG  SCBRR1;
    IOREG  dummy2;    
    IOREG  SCSCR1;
    IOREG  dummy3;        
    IOREG  SCFTDR1;
    IOREG  dummy4;            
    HIOREG SCSSR1;
    IOREG  SCFRDR1;
    IOREG  dummy5;                
    IOREG  SCFCR1;
    IOREG  dummy6;                    
    HIOREG SCFDR1;
} sci1;

#define SCI1 (*(volatile sci1 *)0xa4000140)

#define	SCSMR1_IRMOD    0x0080
#define	SCSMR1_CHR	    0x0040
#define	SCSMR1_PE	    0x0020
#define	SCSMR1_OE	    0x0010
#define	SCSMR1_STOP	    0x0008
#define	SCSMR1_CKS1	    0x0002
#define	SCSMR1_CKS0	    0x0001

#define SCSCR1_TIE      0x0080
#define SCSCR1_RIE      0x0040
#define SCSCR1_TE       0x0020
#define SCSCR1_RE       0x0010
#define SCSCR1_CKE1     0x0002
#define SCSCR1_CKE0     0x0001

#define SCSSR1_ER	0x0080
#define SCSSR1_TEND	0x0040
#define SCSSR1_TDFE	0x0020
#define SCSSR1_BRK	0x0010
#define SCSSR1_FER	0x0008
#define SCSSR1_PER	0x0004
#define SCSSR1_RDF	0x0002
#define SCSSR1_DR	0x0001


#define SCFCR1_RTRG1    0x0080
#define SCFCR1_RTRG0    0x0040
#define SCFCR1_TTRG1    0x0020
#define SCFCR1_TTRG0    0x0010
#define SCFCR1_MCE      0x0008
#define SCFCR1_TFRST    0x0004
#define SCFCR1_RFRST    0x0002
#define SCFCR1_LOOP     0x0001



/*
 * キャッシュ制御レジスタ
 */
#define	CCR	((LIOREG *)0xffffffec)	


/*
 *  gdb stubによる出力
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
 *   SH7709A及びSH7709のSCIFへのポーリングでの出力
 */

Inline int
scif_putc(int c)
{
    /* Wait TDRD = 1 */
    while((SCIF.SCSSR2 & SCSSR2_TDFE)==0);

    SCIF.SCFTDR2=c;
    SCIF.SCSSR2 = SCIF.SCSSR2 & ~SCSSR2_TDFE;
    return(c);
}



    
/*
 *  SCIによる出力
 */

Inline int
sci_putc(int c)
{

    /* Wait TDRD = 1 */
    while((SCI.SCSSR & SCSSR_TDRE)==0);

    SCI.SCTDR=c;
    SCI.SCSSR = SCI.SCSSR & ~SCSSR_TDRE;
    return(c);
}

#endif /* _SH3_H_ */






