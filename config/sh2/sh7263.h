/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002 by Hokkaido Industrial Research Institute, JAPAN
 *  Copyright (C) 2010 by Cronus Computer Works, JAPAN
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
 *  @(#) $Id
 */

#ifndef _SH7263_H_
#define _SH7263_H_

#include <sil.h>

/*
 *  割込みベクタ番号
 */
#define INT_IRQ0        64          /* IRQ0 (外部割り込み) */
#define INT_IRQ1        65          /* IRQ1 (外部割り込み) */
#define INT_IRQ2        66          /* IRQ2 (外部割り込み) */
#define INT_IRQ3        67          /* IRQ3 (外部割り込み) */
#define INT_IRQ4        68          /* IRQ4 (外部割り込み) */
#define INT_IRQ5        69          /* IRQ5 (外部割り込み) */
#define INT_IRQ6        70          /* IRQ6 (外部割り込み) */
#define INT_IRQ7        71          /* IRQ7 (外部割り込み) */
#define INT_PINT0       80          /* PINT0 (外部割り込み) */
#define INT_PINT1       81          /* PINT1 (外部割り込み) */
#define INT_PINT2       82          /* PINT2 (外部割り込み) */
#define INT_PINT3       83          /* PINT3 (外部割り込み) */
#define INT_PINT4       84          /* PINT4 (外部割り込み) */
#define INT_PINT5       85          /* PINT5 (外部割り込み) */
#define INT_PINT6       86          /* PINT6 (外部割り込み) */
#define INT_PINT7       87          /* PINT7 (外部割り込み) */
#define INT_DEI0        108         /* DMACチャネル0 トランスファエンド */
#define INT_HEI0        109         /* DMACチャネル0 ハーフエンド */
#define INT_DEI1        112         /* DMACチャネル1 トランスファエンド */
#define INT_HEI1        113         /* DMACチャネル1 ハーフエンド */
#define INT_DEI2        116         /* DMACチャネル2 トランスファエンド */
#define INT_HEI2        117         /* DMACチャネル2 ハーフエンド */
#define INT_DEI3        120         /* DMACチャネル3 トランスファエンド */
#define INT_HEI3        121         /* DMACチャネル3 ハーフエンド */
#define INT_DEI4        124         /* DMACチャネル4 トランスファエンド */
#define INT_HEI4        125         /* DMACチャネル4 ハーフエンド */
#define INT_DEI5        128         /* DMACチャネル5 トランスファエンド */
#define INT_HEI5        129         /* DMACチャネル5 ハーフエンド */
#define INT_DEI6        132         /* DMACチャネル6 トランスファエンド */
#define INT_HEI6        133         /* DMACチャネル6 ハーフエンド */
#define INT_DEI7        136         /* DMACチャネル7 トランスファエンド */
#define INT_HEI7        137         /* DMACチャネル7 ハーフエンド */
#define INT_USBI        140         /* USBコントローラ */
#define INT_LCDCI       141         /* LCDコントローラ */
#define INT_CMI0        142         /* CMTチャネル0 コンペアマッチ */
#define INT_CMI1        143         /* CMTチャネル1 コンペアマッチ */
#define INT_CMI         144         /* BSC リフレッシュタイマコンペアマッチ */
#define INT_ITI         145         /* WDT インターバルタイマ割り込み */
#define INT_TGI0A       146         /* MTU2 TGRA0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0B       147         /* MTU2 TGRB0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0C       148         /* MTU2 TGRC0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0D       149         /* MTU2 TGRD0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0V       150         /* MTU2 TCNT0 オーバフロー */
#define INT_TGI0E       151         /* MTU2 TGRE0 コンペアマッチ */
#define INT_TGI0F       152         /* MTU2 TGRF0 コンペアマッチ */
#define INT_TGI1A       153         /* MTU2 TGRA1 インプットキャプチャ／コンペアマッチ */
#define INT_TGI1B       154         /* MTU2 TGRB1 インプットキャプチャ／コンペアマッチ */
#define INT_TGI1V       155         /* MTU2 TCNT1 オーバフロー */
#define INT_TGI1U       156         /* MTU2 TCNT1 アンダフロー */
#define INT_TGI2A       157         /* MTU2 TGRA2 インプットキャプチャ／コンペアマッチ */
#define INT_TGI2B       158         /* MTU2 TGRA2 インプットキャプチャ／コンペアマッチ */
#define INT_TGI2V       159         /* MTU2 TCNT2 オーバフロー */
#define INT_TGI2U       160         /* MTU2 TCNT2 アンダフロー */
#define INT_TGI3A       161         /* MTU2 TGRA3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3B       162         /* MTU2 TGRB3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3C       163         /* MTU2 TGRC3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3D       164         /* MTU2 TGRD3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3V       165         /* MTU2 TCNT3 オーバフロー */
#define INT_TGI4A       166         /* MTU2 TGRA4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4B       167         /* MTU2 TGRB4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4C       168         /* MTU2 TGRC4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4D       169         /* MTU2 TGRD4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4V       170         /* MTU2 TCNT4 オーバフロー／アンダフロー */
#define INT_ADI         171         /* A/D変換器 変換終了 */
#define INT_IIC3STPI0   172         /* IIC3チャネル0 停止条件検出 */
#define INT_IIC3NAKI0   173         /* IIC3チャネル0 NACK 検出 */
#define INT_IIC3RXI0    174         /* IIC3チャネル0 受信データフル */
#define INT_IIC3TXI0    175         /* IIC3チャネル0 送信データエンプティ */
#define INT_IIC3TEI0    176         /* IIC3チャネル0 送信終了 */
#define INT_IIC3STPI1   177         /* IIC3チャネル1 停止条件検出 */
#define INT_IIC3NAKI1   178         /* IIC3チャネル1 NACK 検出 */
#define INT_IIC3RXI1    179         /* IIC3チャネル1 受信データフル */
#define INT_IIC3TXI1    180         /* IIC3チャネル1 送信データエンプティ */
#define INT_IIC3TEI1    181         /* IIC3チャネル1 送信終了 */
#define INT_IIC3STPI2   182         /* IIC3チャネル2 停止条件検出 */
#define INT_IIC3NAKI2   183         /* IIC3チャネル2 NACK 検出 */
#define INT_IIC3RXI2    184         /* IIC3チャネル2 受信データフル */
#define INT_IIC3TXI2    185         /* IIC3チャネル2 送信データエンプティ */
#define INT_IIC3TEI2    186         /* IIC3チャネル2 送信終了 */
#define INT_IIC3STPI3   187         /* IIC3チャネル3 停止条件検出 */
#define INT_IIC3NAKI3   188         /* IIC3チャネル3 NACK 検出 */
#define INT_IIC3RXI3    189         /* IIC3チャネル3 受信データフル */
#define INT_IIC3TXI3    190         /* IIC3チャネル3 送信データエンプティ */
#define INT_IIC3TEI3    191         /* IIC3チャネル3 送信終了 */
#define INT_BRI0        192         /* SCIFチャネル0 ブレーク検出 */
#define INT_ERI0        193         /* SCIFチャネル0 受信エラー */
#define INT_RXI0        194         /* SCIFチャネル0 受信データフル */
#define INT_TXI0        195         /* SCIFチャネル0 送信FIFOデータエンプティ */
#define INT_BRI1        196         /* SCIFチャネル1 ブレーク検出 */
#define INT_ERI1        197         /* SCIFチャネル1 受信エラー */
#define INT_RXI1        198         /* SCIFチャネル1 受信データフル */
#define INT_TXI1        199         /* SCIFチャネル1 送信FIFOデータエンプティ */
#define INT_BRI2        200         /* SCIFチャネル2 ブレーク検出 */
#define INT_ERI2        201         /* SCIFチャネル2 受信エラー */
#define INT_RXI2        202         /* SCIFチャネル2 受信データフル */
#define INT_TXI2        203         /* SCIFチャネル2 送信FIFOデータエンプティ */
#define INT_BRI3        204         /* SCIFチャネル3 ブレーク検出 */
#define INT_ERI3        205         /* SCIFチャネル3 受信エラー */
#define INT_RXI3        206         /* SCIFチャネル3 受信データフル */
#define INT_TXI3        207         /* SCIFチャネル3 送信FIFOデータエンプティ */
#define INT_SSERI0      208         /* SSUチャネル0 エラー */
#define INT_SSRXI0      209         /* SSUチャネル0 受信データフル */
#define INT_SSTXI0      210         /* SSUチャネル0 送信データエンプティ/送信終了 */
#define INT_SSERI1      211         /* SSUチャネル1 エラー */
#define INT_SSRXI1      212         /* SSUチャネル1 受信データフル */
#define INT_SSTXI1      213         /* SSUチャネル1 送信データエンプティ/送信終了 */
#define INT_SSI0        214         /* SSIチャネル0 割り込み */
#define INT_SSI1        215         /* SSIチャネル1 割り込み */
#define INT_SSI2        216         /* SSIチャネル2 割り込み */
#define INT_SSI3        217         /* SSIチャネル3 割り込み */
#define INT_ISY         218         /* CD-ROMデコーダ 同期コード異常タイミング */
#define INT_IERR        219         /* CD-ROMデコーダ ECCエラー */
#define INT_IARG        220         /* CD-ROMデコーダ ITARG割り込み */
#define INT_ISEC        221         /* CD-ROMデコーダ セクタ切り替わり */
#define INT_IBUF        222         /* CD-ROMデコーダ バッファデータ転送状態遷移 */
#define INT_IREDAY      223         /* CD-ROMデコーダ バッファデータ転送要求 */
#define INT_FLSTEI      224         /* NANDフラッシュコントローラ STEI要因割割り込み */
#define INT_FLTENDI     225         /* NANDフラッシュコントローラ 転送終了 */
#define INT_FLTREQ0I    226         /* NANDフラッシュコントローラ FIFO0転送要求 */
#define INT_FLTREQ1I    227         /* NANDフラッシュコントローラ FIFO0転送要求 */
#define INT_SDHI3       228         /* SDホストインターフェース SDHI3割り込み */
#define INT_SDHI0       229         /* SDホストインターフェース SDHI0割り込み */
#define INT_SDHI1       230         /* SDホストインターフェース SDHI1割り込み */
#define INT_ARM         231         /* RTC アラーム割り込み */
#define INT_PRD         232         /* RTC 周期割り込み */
#define INT_CUP         233         /* RTC 桁上げ割り込み */
#define INT_ERS0        234         /* RCANチャネル0 ERS要因割り込み */
#define INT_OVR0        235         /* RCANチャネル0 OVR要因割り込み */
#define INT_RM00        236         /* RCANチャネル0 データフレーム受信 */
#define INT_RM10        237         /* RCANチャネル0 リモートフレーム受信 */
#define INT_SLE0        238         /* RCANチャネル0 メッセージ送信 */
#define INT_ERS1        239         /* RCANチャネル1 ERS要因割り込み */
#define INT_OVR1        240         /* RCANチャネル1 OVR要因割り込み */
#define INT_RM01        241         /* RCANチャネル1 データフレーム受信 */
#define INT_RM11        242         /* RCANチャネル1 リモートフレーム受信 */
#define INT_SLE1        243         /* RCANチャネル1 メッセージ送信 */
#define INT_OVF         244         /* SRC 出力データFIFOオーバーライト */
#define INT_ODFI        245         /* SRC 出力データFIFOフル */
#define INT_IDEI        246         /* SRC 入力データFIFOエンプティ */
#define INT_IEBI        247         /* IEBusコントローラ 割り込み */

#ifndef _MACRO_ONLY

/*
 *  内蔵モジュールのレジスタ
 */
/* キャッシュ */
#define CCR1        ((VW *)0xFFFC1000)  /* キャッシュ制御レジスタ1 */
#define CCR2        ((VW *)0xFFFC1004)  /* キャッシュ制御レジスタ2 */

/* クロック発振器 (CPG) */
#define FRQCR       ((VH *)0xFFFE0010)  /* 周波数制御レジスタ */

/* バスステートコントローラ (BSC) */
#define CMNCR       ((VW *)0xFFFC0000)  /* 共通コントロールレジスタ */
#define CS0BCR      ((VW *)0xFFFC0004)  /* エリア0 バスコントロールレジスタ */
#define CS1BCR      ((VW *)0xFFFC0008)  /* エリア1 バスコントロールレジスタ */
#define CS2BCR      ((VW *)0xFFFC000C)  /* エリア2 バスコントロールレジスタ */
#define CS3BCR      ((VW *)0xFFFC0010)  /* エリア3 バスコントロールレジスタ */
#define CS4BCR      ((VW *)0xFFFC0014)  /* エリア4 バスコントロールレジスタ */
#define CS5BCR      ((VW *)0xFFFC0018)  /* エリア5 バスコントロールレジスタ */
#define CS6BCR      ((VW *)0xFFFC001C)  /* エリア6 バスコントロールレジスタ */
#define CS7BCR      ((VW *)0xFFFC0020)  /* エリア7 バスコントロールレジスタ */
#define CS0WCR      ((VW *)0xFFFC0028)  /* エリア0 ウェイトコントロールレジスタ */
#define CS1WCR      ((VW *)0xFFFC002C)  /* エリア1 ウェイトコントロールレジスタ */
#define CS2WCR      ((VW *)0xFFFC0030)  /* エリア2 ウェイトコントロールレジスタ */
#define CS3WCR      ((VW *)0xFFFC0034)  /* エリア3 ウェイトコントロールレジスタ */
#define CS4WCR      ((VW *)0xFFFC0038)  /* エリア4 ウェイトコントロールレジスタ */
#define CS5WCR      ((VW *)0xFFFC003C)  /* エリア5 ウェイトコントロールレジスタ */
#define CS6WCR      ((VW *)0xFFFC0040)  /* エリア6 ウェイトコントロールレジスタ */
#define CS7WCR      ((VW *)0xFFFC0044)  /* エリア7 ウェイトコントロールレジスタ */
#define SDCR        ((VW *)0xFFFC004C)  /* SDRAMコントロールレジスタ */
#define RTCSR       ((VW *)0xFFFC0050)  /* リフレッシュタイマコントロール/ステータスレジスタ */
#define RTCNT       ((VW *)0xFFFC0054)  /* リフレッシュタイマカウンタ */
#define RTCOR       ((VW *)0xFFFC0058)  /* リフレッシュタイムコンスタントレジスタ */

/* 割り込みコントローラ (INTC) */
#define ICR0        ((VH *)0xFFFE0800)  /* 割り込みコントロールレジスタ0 */
#define ICR1        ((VH *)0xFFFE0802)  /* 割り込みコントロールレジスタ1 */
#define ICR2        ((VH *)0xFFFE0804)  /* 割り込みコントロールレジスタ2 */
#define IRQRR       ((VH *)0xFFFE0806)  /* IRQ 割り込み要求レジスタ */
#define PINTER      ((VH *)0xFFFE0808)  /* PINT 割り込みイネーブルレジスタ */
#define PIRR        ((VH *)0xFFFE080A)  /* PINT 割り込み要求レジスタ */
#define IBCR        ((VH *)0xFFFE080C)  /* バンクコントロールレジスタ */
#define IBNR        ((VH *)0xFFFE080E)  /* バンク番号レジスタ */
#define IPR01       ((VH *)0xFFFE0818)  /* 割り込み優先レベル設定レジスタ01 */
#define IPR02       ((VH *)0xFFFE081A)  /* 割り込み優先レベル設定レジスタ02 */
#define IPR05       ((VH *)0xFFFE0820)  /* 割り込み優先レベル設定レジスタ05 */
#define IPR06       ((VH *)0xFFFE0C00)  /* 割り込み優先レベル設定レジスタ06 */
#define IPR07       ((VH *)0xFFFE0C02)  /* 割り込み優先レベル設定レジスタ07 */
#define IPR08       ((VH *)0xFFFE0C04)  /* 割り込み優先レベル設定レジスタ08 */
#define IPR09       ((VH *)0xFFFE0C06)  /* 割り込み優先レベル設定レジスタ09 */
#define IPR10       ((VH *)0xFFFE0C08)  /* 割り込み優先レベル設定レジスタ10 */
#define IPR11       ((VH *)0xFFFE0C0A)  /* 割り込み優先レベル設定レジスタ11 */
#define IPR12       ((VH *)0xFFFE0C0C)  /* 割り込み優先レベル設定レジスタ12 */
#define IPR13       ((VH *)0xFFFE0C0E)  /* 割り込み優先レベル設定レジスタ13 */
#define IPR14       ((VH *)0xFFFE0C10)  /* 割り込み優先レベル設定レジスタ14 */
#define IPR16       ((VH *)0xFFFE0C12)  /* 割り込み優先レベル設定レジスタ15 */
#define IPR15       ((VH *)0xFFFE0C14)  /* 割り込み優先レベル設定レジスタ16 */
#define IPR17       ((VH *)0xFFFE0C16)  /* 割り込み優先レベル設定レジスタ17 */

/* コンペアマッチタイマ (CMT) */
#define CMSTR       ((VH *)0xFFFEC000)  /* コンペアマッチタイマスタートレジスタ */
#define CMCSR_0     ((VH *)0xFFFEC002)  /* チャネル0 コンペアマッチカウンタコントロール/ステータスレジスタ */
#define CMCNT_0     ((VH *)0xFFFEC004)  /* チャネル0 コンペアマッチカウンタ */
#define CMCOR_0     ((VH *)0xFFFEC006)  /* チャネル0 コンペアマッチコンスタントレジスタ */
#define CMCSR_1     ((VH *)0xFFFEC008)  /* チャネル1 コンペアマッチカウンタコントロール/ステータスレジスタ */
#define CMCNT_1     ((VH *)0xFFFEC00A)  /* チャネル1 コンペアマッチカウンタ */
#define CMCOR_1     ((VH *)0xFFFEC00C)  /* チャネル1 コンペアマッチコンスタントレジスタ */

/* ピンファンクションコントローラ (PFC), I/Oポート */
#define PADRL       ((VH *)0xFFFE3802)  /* ポートA データレジスタL */
#define PBCRL       ((VH *)0xFFFE3882)  /* ポートB データレジスタL */
#define PBIORL      ((VH *)0xFFFE3886)  /* ポートB IOレジスタL */
#define PBCRL4      ((VH *)0xFFFE3890)  /* ポートB コントロールレジスタL4 */
#define PBCRL3      ((VH *)0xFFFE3892)  /* ポートB コントロールレジスタL3 */
#define PBCRL2      ((VH *)0xFFFE3894)  /* ポートB コントロールレジスタL2 */
#define PBCRL1      ((VH *)0xFFFE3896)  /* ポートB コントロールレジスタL1 */
#define PBPRL       ((VH *)0xFFFE389E)  /* ポートB ポートレジスタL */
#define IFCR        ((VH *)0xFFFE38A2)  /* IRQOUT機能コントロールレジスタ */
#define PCCRL       ((VH *)0xFFFE3902)  /* ポートC データレジスタL */
#define PCIORL      ((VH *)0xFFFE3906)  /* ポートC IOレジスタL */
#define PCCRL4      ((VH *)0xFFFE3910)  /* ポートC コントロールレジスタL4 */
#define PCCRL3      ((VH *)0xFFFE3912)  /* ポートC コントロールレジスタL3 */
#define PCCRL2      ((VH *)0xFFFE3914)  /* ポートC コントロールレジスタL2 */
#define PCCRL1      ((VH *)0xFFFE3916)  /* ポートC コントロールレジスタL1 */
#define PCPRL       ((VH *)0xFFFE391E)  /* ポートC ポートレジスタL */
#define PDCRL       ((VH *)0xFFFE3982)  /* ポートD データレジスタL */
#define PDIORL      ((VH *)0xFFFE3986)  /* ポートD IOレジスタL */
#define PDCRL4      ((VH *)0xFFFE3990)  /* ポートD コントロールレジスタL4 */
#define PDCRL3      ((VH *)0xFFFE3992)  /* ポートD コントロールレジスタL3 */
#define PDCRL2      ((VH *)0xFFFE3994)  /* ポートD コントロールレジスタL2 */
#define PDCRL1      ((VH *)0xFFFE3996)  /* ポートD コントロールレジスタL1 */
#define PDPRL       ((VH *)0xFFFE399E)  /* ポートD ポートレジスタL */
#define PECRL       ((VH *)0xFFFE3A02)  /* ポートE データレジスタL */
#define PEIORL      ((VH *)0xFFFE3A06)  /* ポートE IOレジスタL */
#define PECRL4      ((VH *)0xFFFE3A10)  /* ポートE コントロールレジスタL4 */
#define PECRL3      ((VH *)0xFFFE3A12)  /* ポートE コントロールレジスタL3 */
#define PECRL2      ((VH *)0xFFFE3A14)  /* ポートE コントロールレジスタL2 */
#define PECRL1      ((VH *)0xFFFE3A16)  /* ポートE コントロールレジスタL1 */
#define PEPRL       ((VH *)0xFFFE3A1E)  /* ポートE ポートレジスタL */
#define PFCRH       ((VH *)0xFFFE3A80)  /* ポートE データレジスタH */
#define PFCRL       ((VH *)0xFFFE3A82)  /* ポートE データレジスタL */
#define PFIORH      ((VH *)0xFFFE3A84)  /* ポートF IOレジスタH */
#define PFIORL      ((VH *)0xFFFE3A86)  /* ポートF IOレジスタL */
#define PFCRH4      ((VH *)0xFFFE3A88)  /* ポートF コントロールレジスタH4 */
#define PFCRH3      ((VH *)0xFFFE3A8A)  /* ポートF コントロールレジスタH3 */
#define PFCRH2      ((VH *)0xFFFE3A8C)  /* ポートF コントロールレジスタH2 */
#define PFCRH1      ((VH *)0xFFFE3A8E)  /* ポートF コントロールレジスタH1 */
#define PFCRL4      ((VH *)0xFFFE3A90)  /* ポートF コントロールレジスタL4 */
#define PFCRL3      ((VH *)0xFFFE3A92)  /* ポートF コントロールレジスタL3 */
#define PFCRL2      ((VH *)0xFFFE3A94)  /* ポートF コントロールレジスタL2 */
#define PFCRL1      ((VH *)0xFFFE3A96)  /* ポートF コントロールレジスタL1 */
#define PFPRH       ((VH *)0xFFFE3A9C)  /* ポートE ポートレジスタH */
#define PFPRL       ((VH *)0xFFFE3A9E)  /* ポートE ポートレジスタL */
#define SCSR        ((VH *)0xFFFE3AA2)  /* SSIクロック選択レジスタ */

/* 低消費電力モード */
#define STBCR       ((VB *)0xFFFE0014)  /* スタンバイコントロールレジスタ */
#define STBCR2      ((VB *)0xFFFE0018)  /* スタンバイコントロールレジスタ2 */
#define STBCR3      ((VB *)0xFFFE0408)  /* スタンバイコントロールレジスタ3 */
#define STBCR4      ((VB *)0xFFFE040C)  /* スタンバイコントロールレジスタ4 */
#define STBCR5      ((VB *)0xFFFE0410)  /* スタンバイコントロールレジスタ5 */
#define STBCR6      ((VB *)0xFFFE0414)  /* スタンバイコントロールレジスタ6 */
#define SYSCR1      ((VB *)0xFFFE0402)  /* システムコントロールレジスタ1 */
#define SYSCR2      ((VB *)0xFFFE0404)  /* システムコントロールレジスタ2 */
#define SYSCR3      ((VB *)0xFFFE0418)  /* システムコントロールレジスタ3 */
#define DSCTR       ((VB *)0xFFFE2800)  /* ディープスタンバイコントロールレジスタ */
#define DSCTR2      ((VB *)0xFFFE2802)  /* ディープスタンバイコントロールレジスタ2 */
#define DSSSR       ((VH *)0xFFFE2804)  /* ディープスタンバイ解除要因セレクトレジスタ */
#define DSFR        ((VH *)0xFFFE2808)  /* ディープスタンバイ解除エッジセレクトレジスタ */
#define DSRTR       ((VB *)0xFFFE280C)  /* 保持用内蔵RAMトリミングレジスタ */

/* FIFO付きシリアルコミュニケーションインターフェース (SCIF) */

#define SCIF0_BASE  ((UW)0xFFFE8000)    /* チャネル0 レジスタベースアドレス */
#define SCIF1_BASE  ((UW)0xFFFE8800)    /* チャネル1 レジスタベースアドレス */
#define SCIF2_BASE  ((UW)0xFFFE9000)    /* チャネル2 レジスタベースアドレス */
#define SCIF3_BASE  ((UW)0xFFFE9800)    /* チャネル3 レジスタベースアドレス */

/*
 * 割り込みコントローラの初期化
 */
Inline void sh2_init_intcontorller(void)
{
	/* バンク番号レジスタの設定 */
	sil_wrh_mem(IBNR, 0x0000);		/* BE=00, BOVE=0                         */
									/*  すべての割り込みでバンク使用禁止     */
									/*  レジスタバンクオーバーフロー例外禁止 */
	/* 割り込み優先レベル設定レジスタを初期化 */
	sil_wrh_mem (IPR01, 0x0000);
	sil_wrh_mem (IPR02, 0x0000);
	sil_wrh_mem (IPR05, 0x0000);
	sil_wrh_mem (IPR06, 0x0000);
	sil_wrh_mem (IPR07, 0x0000);
	sil_wrh_mem (IPR08, 0x0000);
	sil_wrh_mem (IPR09, 0x0000);
	sil_wrh_mem (IPR10, 0x0000);
	sil_wrh_mem (IPR11, 0x0000);
	sil_wrh_mem (IPR12, 0x0000);
	sil_wrh_mem (IPR13, 0x0000);
	sil_wrh_mem (IPR14, 0x0000);
	sil_wrh_mem (IPR15, 0x0000);
	sil_wrh_mem (IPR16, 0x0000);
	sil_wrh_mem (IPR17, 0x0000);
}

#endif /* _MACRO_ONLY */

#endif /* _SH7263_H_ */
