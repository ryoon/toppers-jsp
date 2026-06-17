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

#ifndef _SH7262_H_
#define _SH7262_H_

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
#define INT_DEI8        140         /* DMACチャネル8 トランスファエンド */
#define INT_HEI8        141         /* DMACチャネル8 ハーフエンド */
#define INT_DEI9        144         /* DMACチャネル9 トランスファエンド */
#define INT_HEI9        145         /* DMACチャネル9 ハーフエンド */
#define INT_DEI10       148         /* DMACチャネル10 トランスファエンド */
#define INT_HEI10       149         /* DMACチャネル10 ハーフエンド */
#define INT_DEI11       152         /* DMACチャネル11 トランスファエンド */
#define INT_HEI11       153         /* DMACチャネル11 ハーフエンド */
#define INT_DEI12       156         /* DMACチャネル12 トランスファエンド */
#define INT_HEI12       157         /* DMACチャネル12 ハーフエンド */
#define INT_DEI13       160         /* DMACチャネル13 トランスファエンド */
#define INT_HEI13       161         /* DMACチャネル13 ハーフエンド */
#define INT_DEI14       164         /* DMACチャネル14 トランスファエンド */
#define INT_HEI14       165         /* DMACチャネル14 ハーフエンド */
#define INT_DEI15       168         /* DMACチャネル15 トランスファエンド */
#define INT_HEI15       169         /* DMACチャネル15 ハーフエンド */
#define INT_USBI        170         /* USBコントローラ */
#define INT_VIVSYNCJ    171         /* ビデオディスプレイコントローラ3 VSYNC周期乱れ検出 */
#define INT_VBUFFER     172         /* ビデオディスプレイコントローラ3 バッファエラー */
#define INT_VIFIELDE    173         /* ビデオディスプレイコントローラ3 フィールド書き込み完了 */
#define INT_VOLINE      174         /* ビデオディスプレイコントローラ3 ライン割り込み */
#define INT_CMI0        175         /* CMTチャネル0 コンペアマッチ */
#define INT_CMI1        176         /* CMTチャネル1 コンペアマッチ */
#define INT_CMI         177         /* BSC リフレッシュタイマコンペアマッチ */
#define INT_ITI         178         /* WDT インターバルタイマ割り込み */
#define INT_TGI0A       179         /* MTU2 TGRA0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0B       180         /* MTU2 TGRB0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0C       181         /* MTU2 TGRC0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0D       182         /* MTU2 TGRD0 インプットキャプチャ／コンペアマッチ */
#define INT_TGI0V       183         /* MTU2 TCNT0 オーバフロー */
#define INT_TGI0E       184         /* MTU2 TGRE0 コンペアマッチ */
#define INT_TGI0F       185         /* MTU2 TGRF0 コンペアマッチ */
#define INT_TGI1A       186         /* MTU2 TGRA1 インプットキャプチャ／コンペアマッチ */
#define INT_TGI1B       187         /* MTU2 TGRB1 インプットキャプチャ／コンペアマッチ */
#define INT_TGI1V       188         /* MTU2 TCNT1 オーバフロー */
#define INT_TGI1U       189         /* MTU2 TCNT1 アンダフロー */
#define INT_TGI2A       190         /* MTU2 TGRA2 インプットキャプチャ／コンペアマッチ */
#define INT_TGI2B       191         /* MTU2 TGRA2 インプットキャプチャ／コンペアマッチ */
#define INT_TGI2V       192         /* MTU2 TCNT2 オーバフロー */
#define INT_TGI2U       193         /* MTU2 TCNT2 アンダフロー */
#define INT_TGI3A       194         /* MTU2 TGRA3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3B       195         /* MTU2 TGRB3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3C       196         /* MTU2 TGRC3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3D       197         /* MTU2 TGRD3 インプットキャプチャ／コンペアマッチ */
#define INT_TGI3V       198         /* MTU2 TCNT3 オーバフロー */
#define INT_TGI4A       199         /* MTU2 TGRA4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4B       200         /* MTU2 TGRB4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4C       201         /* MTU2 TGRC4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4D       202         /* MTU2 TGRD4 インプットキャプチャ／コンペアマッチ */
#define INT_TGI4V       203         /* MTU2 TCNT4 オーバフロー／アンダフロー */
#define INT_CMF1        204         /* PWMチャネル1 コンペアマッチフラグ */
#define INT_CMF2        205         /* PWMチャネル2 コンペアマッチフラグ */
#define INT_ADI         206         /* A/D変換器 変換終了 */
#define INT_SSIF0       207         /* SSIチャネル0 SSIF0 */
#define INT_SSIRXI0     208         /* SSIチャネル0 SSIRXI0 */
#define INT_SSITXI0     209         /* SSIチャネル0 SSITXI0 */
#define INT_SSII1       210         /* SSIチャネル1 SSII1 */
#define INT_SSIRTI1     211         /* SSIチャネル1 SSIRTI1 */
#define INT_SSII2       212         /* SSIチャネル2 SSII2 */
#define INT_SSIRTI2     213         /* SSIチャネル2 SSIRTI2 */
#define INT_SSII3       214         /* SSIチャネル3 SSII2 */
#define INT_SSIRTI3     215         /* SSIチャネル3 SSIRTI2 */
#define INT_SPDIFI      216         /* SPDIFインターフェース割り込み */
#define INT_IIC3STPI0   217         /* IIC3チャネル0 停止条件検出 */
#define INT_IIC3NAKI0   218         /* IIC3チャネル0 NACK 検出 */
#define INT_IIC3RXI0    219         /* IIC3チャネル0 受信データフル */
#define INT_IIC3TXI0    220         /* IIC3チャネル0 送信データエンプティ */
#define INT_IIC3TEI0    221         /* IIC3チャネル0 送信終了 */
#define INT_IIC3STPI1   222         /* IIC3チャネル1 停止条件検出 */
#define INT_IIC3NAKI1   223         /* IIC3チャネル1 NACK 検出 */
#define INT_IIC3RXI1    224         /* IIC3チャネル1 受信データフル */
#define INT_IIC3TXI1    225         /* IIC3チャネル1 送信データエンプティ */
#define INT_IIC3TEI1    226         /* IIC3チャネル1 送信終了 */
#define INT_IIC3STPI2   227         /* IIC3チャネル2 停止条件検出 */
#define INT_IIC3NAKI2   228         /* IIC3チャネル2 NACK 検出 */
#define INT_IIC3RXI2    229         /* IIC3チャネル2 受信データフル */
#define INT_IIC3TXI2    230         /* IIC3チャネル2 送信データエンプティ */
#define INT_IIC3TEI2    231         /* IIC3チャネル2 送信終了 */
#define INT_BRI0        232         /* SCIFチャネル0 ブレーク検出 */
#define INT_ERI0        233         /* SCIFチャネル0 受信エラー */
#define INT_RXI0        234         /* SCIFチャネル0 受信データフル */
#define INT_TXI0        235         /* SCIFチャネル0 送信FIFOデータエンプティ */
#define INT_BRI1        236         /* SCIFチャネル1 ブレーク検出 */
#define INT_ERI1        237         /* SCIFチャネル1 受信エラー */
#define INT_RXI1        238         /* SCIFチャネル1 受信データフル */
#define INT_TXI1        239         /* SCIFチャネル1 送信FIFOデータエンプティ */
#define INT_BRI2        240         /* SCIFチャネル2 ブレーク検出 */
#define INT_ERI2        241         /* SCIFチャネル2 受信エラー */
#define INT_RXI2        242         /* SCIFチャネル2 受信データフル */
#define INT_TXI2        243         /* SCIFチャネル2 送信FIFOデータエンプティ */
#define INT_BRI3        244         /* SCIFチャネル3 ブレーク検出 */
#define INT_ERI3        245         /* SCIFチャネル3 受信エラー */
#define INT_RXI3        246         /* SCIFチャネル3 受信データフル */
#define INT_TXI3        247         /* SCIFチャネル3 送信FIFOデータエンプティ */
#define INT_BRI4        248         /* SCIFチャネル4 ブレーク検出 */
#define INT_ERI4        249         /* SCIFチャネル4 受信エラー */
#define INT_RXI4        250         /* SCIFチャネル4 受信データフル */
#define INT_TXI4        251         /* SCIFチャネル4 送信FIFOデータエンプティ */
#define INT_BRI5        252         /* SCIFチャネル5 ブレーク検出 */
#define INT_ERI5        253         /* SCIFチャネル5 受信エラー */
#define INT_RXI5        254         /* SCIFチャネル5 受信データフル */
#define INT_TXI5        255         /* SCIFチャネル5 送信FIFOデータエンプティ */
#define INT_BRI6        256         /* SCIFチャネル6 ブレーク検出 */
#define INT_ERI6        257         /* SCIFチャネル6 受信エラー */
#define INT_RXI6        258         /* SCIFチャネル6 受信データフル */
#define INT_TXI6        259         /* SCIFチャネル6 送信FIFOデータエンプティ */
#define INT_BRI7        260         /* SCIFチャネル7 ブレーク検出 */
#define INT_ERI7        261         /* SCIFチャネル7 受信エラー */
#define INT_RXI7        262         /* SCIFチャネル7 受信データフル */
#define INT_TXI7        263         /* SCIFチャネル7 送信FIFOデータエンプティ */
#define INT_SIOFI       264         /* SIOF割り込み */
#define INT_SPEI0       265         /* SPIチャネル0 エラー */
#define INT_SPRI0       266         /* SPIチャネル0 受信バッファフル */
#define INT_SPTI0       267         /* SPIチャネル0 送信エンプティ*/
#define INT_SPEI1       268         /* SPIチャネル1 エラー */
#define INT_SPRI1       269         /* SPIチャネル1 受信バッファフル */
#define INT_SPTI1       270         /* SPIチャネル1 送信エンプティ*/
#define INT_ERS0        271         /* RCANチャネル0 ERS要因割り込み */
#define INT_OVR0        272         /* RCANチャネル0 OVR要因割り込み */
#define INT_RM00        273         /* RCANチャネル0 データフレーム受信 */
#define INT_RM10        274         /* RCANチャネル0 リモートフレーム受信 */
#define INT_SLE0        275         /* RCANチャネル0 メッセージ送信 */
#define INT_ERS1        276         /* RCANチャネル1 ERS要因割り込み */
#define INT_OVR1        277         /* RCANチャネル1 OVR要因割り込み */
#define INT_RM01        278         /* RCANチャネル1 データフレーム受信 */
#define INT_RM11        279         /* RCANチャネル1 リモートフレーム受信 */
#define INT_SLE1        280         /* RCANチャネル1 メッセージ送信 */
#define INT_IEBI        281         /* IEBusコントローラ割り込み */
#define INT_ISY         282         /* CD-ROMデコーダ 同期コード異常タイミング */
#define INT_IERR        283         /* CD-ROMデコーダ ECCエラー */
#define INT_IARG        284         /* CD-ROMデコーダ ITARGエラー割り込み */
#define INT_ISEC        285         /* CD-ROMデコーダ セクタ切り替わり */
#define INT_IBUF        286         /* CD-ROMデコーダ バッファデータ転送状態遷移 */
#define INT_IREDAY      287         /* CD-ROMデコーダ バッファデータ転送要求 */
#define INT_FLSTEI      288         /* NANDフラッシュコントローラ STEI要因割割り込み */
#define INT_FLTENDI     289         /* NANDフラッシュコントローラ 転送終了 */
#define INT_FLTREQ0I    290         /* NANDフラッシュコントローラ FIFO0転送要求 */
#define INT_FLTREQ1I    291         /* NANDフラッシュコントローラ FIFO0転送要求 */
#define INT_SDHI3       292         /* SDホストインターフェース SDHI3割り込み */
#define INT_SDHI0       293         /* SDホストインターフェース SDHI0割り込み */
#define INT_SDHI1       294         /* SDホストインターフェース SDHI1割り込み */
#define INT_ARM         296         /* RTC アラーム割り込み */
#define INT_PRD         297         /* RTC 周期割り込み */
#define INT_CUP         298         /* RTC 桁上げ割り込み */
#define INT_OVF0        299         /* SRCチャネル0 出力データFIFOオーバーライト */
#define INT_UDF0        300         /* SRCチャネル0 出力データFIFOアンダーフロー */
#define INT_CEF0        301         /* SRCチャネル0 変換処理終了 */
#define INT_ODFI0       302         /* SRCチャネル0 出力データFIFOフル */
#define INT_IDEI0       303         /* SRCチャネル0 出力データFIFOエンプティ */
#define INT_OVF1        304         /* SRCチャネル1 出力データFIFOオーバーライト */
#define INT_UDF1        305         /* SRCチャネル1 出力データFIFOアンダーフロー */
#define INT_CEF1        306         /* SRCチャネル1 変換処理終了 */
#define INT_ODFI1       307         /* SRCチャネル1 出力データFIFOフル */
#define INT_IDEI1       308         /* SRCチャネル1 出力データFIFOエンプティ */
#define INT_DCUEI       310         /* デコンプレッションユニット DCUEI割り込み */
#define INT_OFFI        311         /* デコンプレッションユニット OFFI割り込み */
#define INT_IFEI        312         /* デコンプレッションユニット IFEI割り込み */

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
#define ACSWR       ((VW *)0xFFFC180C)  /* AC特性切り替えレジスタ */
#define ACKEYR      ((VB *)0xFFFC1BFC)  /* AC特性切り替えキーレジスタ */

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
#define IPR18       ((VH *)0xFFFE0C18)  /* 割り込み優先レベル設定レジスタ18 */
#define IPR19       ((VH *)0xFFFE0C1A)  /* 割り込み優先レベル設定レジスタ19 */
#define IPR20       ((VH *)0xFFFE0C1C)  /* 割り込み優先レベル設定レジスタ20 */
#define IPR21       ((VH *)0xFFFE0C1E)  /* 割り込み優先レベル設定レジスタ21 */
#define IPR22       ((VH *)0xFFFE0C20)  /* 割り込み優先レベル設定レジスタ22 */

/* コンペアマッチタイマ (CMT) */
#define CMSTR       ((VH *)0xFFFEC000)  /* コンペアマッチタイマスタートレジスタ */
#define CMCSR_0     ((VH *)0xFFFEC002)  /* チャネル0 コンペアマッチカウンタコントロール/ステータスレジスタ */
#define CMCNT_0     ((VH *)0xFFFEC004)  /* チャネル0 コンペアマッチカウンタ */
#define CMCOR_0     ((VH *)0xFFFEC006)  /* チャネル0 コンペアマッチコンスタントレジスタ */
#define CMCSR_1     ((VH *)0xFFFEC008)  /* チャネル1 コンペアマッチカウンタコントロール/ステータスレジスタ */
#define CMCNT_1     ((VH *)0xFFFEC00A)  /* チャネル1 コンペアマッチカウンタ */
#define CMCOR_1     ((VH *)0xFFFEC00C)  /* チャネル1 コンペアマッチコンスタントレジスタ */

/* ピンファンクションコントローラ (PFC), I/Oポート */
#define PAIOR0      ((VH *)0xFFFE3812)  /* ポートA IOレジスタ0 */
#define PADR1       ((VH *)0xFFFE3814)  /* ポートA データレジスタ1 */
#define PADR0       ((VH *)0xFFFE3816)  /* ポートA データレジスタ0 */
#define PAPR0       ((VH *)0xFFFE381A)  /* ポートA ポートレジスタ0 */
#define PBCR5       ((VH *)0xFFFE3824)  /* ポートB コントロールレジスタ5 */
#define PBCR4       ((VH *)0xFFFE3826)  /* ポートB コントロールレジスタ4 */
#define PBCR3       ((VH *)0xFFFE3828)  /* ポートB コントロールレジスタ3 */
#define PBCR2       ((VH *)0xFFFE382A)  /* ポートB コントロールレジスタ2 */
#define PBCR1       ((VH *)0xFFFE382C)  /* ポートB コントロールレジスタ1 */
#define PBCR0       ((VH *)0xFFFE382E)  /* ポートB コントロールレジスタ0 */
#define PBIOR1      ((VH *)0xFFFE3830)  /* ポートB IOレジスタ1 */
#define PBIOR0      ((VH *)0xFFFE3832)  /* ポートB IOレジスタ0 */
#define PBDR1       ((VH *)0xFFFE3834)  /* ポートB データレジスタ1 */
#define PBDR0       ((VH *)0xFFFE3836)  /* ポートB データレジスタ0 */
#define PBPR1       ((VH *)0xFFFE3838)  /* ポートB ポートレジスタ1 */
#define PBPR0       ((VH *)0xFFFE383A)  /* ポートB ポートレジスタ1 */
#define PCCR2       ((VH *)0xFFFE384A)  /* ポートC コントロールレジスタ2 */
#define PCCR1       ((VH *)0xFFFE384C)  /* ポートC コントロールレジスタ1 */
#define PCCR0       ((VH *)0xFFFE384E)  /* ポートC コントロールレジスタ0 */
#define PCIOR0      ((VH *)0xFFFE3852)  /* ポートC IOレジスタ0 */
#define PCDR0       ((VH *)0xFFFE3856)  /* ポートC データレジスタ0 */
#define PCPR0       ((VH *)0xFFFE385A)  /* ポートC ポートレジスタ1 */
#define PDCR3       ((VH *)0xFFFE3868)  /* ポートD コントロールレジスタ3 */
#define PDCR2       ((VH *)0xFFFE386A)  /* ポートD コントロールレジスタ2 */
#define PDCR1       ((VH *)0xFFFE386C)  /* ポートD コントロールレジスタ1 */
#define PDCR0       ((VH *)0xFFFE386E)  /* ポートD コントロールレジスタ0 */
#define PDIOR0      ((VH *)0xFFFE3872)  /* ポートD IOレジスタ0 */
#define PDDR0       ((VH *)0xFFFE3876)  /* ポートD データレジスタ0 */
#define PDPR0       ((VH *)0xFFFE387A)  /* ポートD ポートレジスタ1 */
#define PECR1       ((VH *)0xFFFE388C)  /* ポートD コントロールレジスタ1 */
#define PECR0       ((VH *)0xFFFE388E)  /* ポートD コントロールレジスタ0 */
#define PEIOR0      ((VH *)0xFFFE3892)  /* ポートD IOレジスタ0 */
#define PEDR0       ((VH *)0xFFFE3896)  /* ポートD データレジスタ0 */
#define PEPR0       ((VH *)0xFFFE389A)  /* ポートD ポートレジスタ1 */
#define PFCR3       ((VH *)0xFFFE38A8)  /* ポートF コントロールレジスタ3 */
#define PFCR2       ((VH *)0xFFFE38AA)  /* ポートF コントロールレジスタ2 */
#define PFCR1       ((VH *)0xFFFE38AC)  /* ポートF コントロールレジスタ1 */
#define PFCR0       ((VH *)0xFFFE38AE)  /* ポートF コントロールレジスタ0 */
#define PFIOR0      ((VH *)0xFFFE38B2)  /* ポートF IOレジスタ0 */
#define PFDR0       ((VH *)0xFFFE38B6)  /* ポートF データレジスタ0 */
#define PFPR0       ((VH *)0xFFFE38BA)  /* ポートF ポートレジスタ1 */
#define PGCR7       ((VH *)0xFFFE38C0)  /* ポートG コントロールレジスタ5 */
#define PGCR6       ((VH *)0xFFFE38C2)  /* ポートG コントロールレジスタ5 */
#define PGCR5       ((VH *)0xFFFE38C4)  /* ポートG コントロールレジスタ5 */
#define PGCR4       ((VH *)0xFFFE38C6)  /* ポートG コントロールレジスタ4 */
#define PGCR3       ((VH *)0xFFFE38C8)  /* ポートG コントロールレジスタ3 */
#define PGCR2       ((VH *)0xFFFE38CA)  /* ポートG コントロールレジスタ2 */
#define PGCR1       ((VH *)0xFFFE38CC)  /* ポートG コントロールレジスタ1 */
#define PGCR0       ((VH *)0xFFFE38CE)  /* ポートG コントロールレジスタ0 */
#define PGIOR1      ((VH *)0xFFFE38D0)  /* ポートG IOレジスタ1 */
#define PGIOR0      ((VH *)0xFFFE38D2)  /* ポートG IOレジスタ0 */
#define PGDR1       ((VH *)0xFFFE38D4)  /* ポートG データレジスタ1 */
#define PGDR0       ((VH *)0xFFFE38D6)  /* ポートG データレジスタ0 */
#define PGPR1       ((VH *)0xFFFE38D8)  /* ポートG ポートレジスタ1 */
#define PGPR0       ((VH *)0xFFFE38DA)  /* ポートG ポートレジスタ1 */
#define PHCR1       ((VH *)0xFFFE38EC)  /* ポートH コントロールレジスタ1 */
#define PHCR0       ((VH *)0xFFFE38EE)  /* ポートH コントロールレジスタ0 */
#define PHPR0       ((VH *)0xFFFE38FA)  /* ポートH ポートレジスタ1 */
#define PJCR2       ((VH *)0xFFFE390A)  /* ポートJ コントロールレジスタ2 */
#define PJCR1       ((VH *)0xFFFE390C)  /* ポートJ コントロールレジスタ1 */
#define PJCR0       ((VH *)0xFFFE390E)  /* ポートJ コントロールレジスタ0 */
#define PJIOR0      ((VH *)0xFFFE3912)  /* ポートJ IOレジスタ0 */
#define PJDR0       ((VH *)0xFFFE3916)  /* ポートJ データレジスタ0 */
#define PJPR0       ((VH *)0xFFFE391A)  /* ポートJ ポートレジスタ1 */
#define PKCR2       ((VH *)0xFFFE392A)  /* ポートK コントロールレジスタ2 */
#define PKCR1       ((VH *)0xFFFE392C)  /* ポートK コントロールレジスタ1 */
#define PKCR0       ((VH *)0xFFFE392E)  /* ポートK コントロールレジスタ0 */
#define PKIOR0      ((VH *)0xFFFE3932)  /* ポートK IOレジスタ0 */
#define PKDR0       ((VH *)0xFFFE3936)  /* ポートK データレジスタ0 */
#define PKPR0       ((VH *)0xFFFE393A)  /* ポートK ポートレジスタ1 */

/* 低消費電力モード */
#define STBCR1      ((VB *)0xFFFE0014)  /* スタンバイコントロールレジスタ1 */
#define STBCR2      ((VB *)0xFFFE0018)  /* スタンバイコントロールレジスタ2 */
#define STBCR3      ((VB *)0xFFFE0408)  /* スタンバイコントロールレジスタ3 */
#define STBCR4      ((VB *)0xFFFE040C)  /* スタンバイコントロールレジスタ4 */
#define STBCR5      ((VB *)0xFFFE0410)  /* スタンバイコントロールレジスタ5 */
#define STBCR6      ((VB *)0xFFFE0414)  /* スタンバイコントロールレジスタ6 */
#define STBCR7      ((VB *)0xFFFE0418)  /* スタンバイコントロールレジスタ7 */
#define STBCR8      ((VB *)0xFFFE041C)  /* スタンバイコントロールレジスタ8 */
#define SWRSTCR     ((VB *)0xFFFE0430)  /* ソフトウェアリセットコントロールレジスタ8 */
#define SYSCR1      ((VB *)0xFFFE0400)  /* システムコントロールレジスタ1 */
#define SYSCR2      ((VB *)0xFFFE0404)  /* システムコントロールレジスタ2 */
#define SYSCR3      ((VB *)0xFFFE0420)  /* システムコントロールレジスタ3 */
#define SYSCR4      ((VB *)0xFFFE0424)  /* システムコントロールレジスタ4 */
#define SYSCR5      ((VB *)0xFFFE0428)  /* システムコントロールレジスタ5 */
#define RRAMKP      ((VB *)0xFFFE6800)  /* 保持用内蔵RAM 保持エリア指定レジスタ */
#define DSCTR       ((VB *)0xFFFE6802)  /* ディープスタンバイコントロールレジスタ */
#define DSSSR       ((VH *)0xFFFE6804)  /* ディープスタンバイ解除要因セレクトレジスタ */
#define DSESR       ((VH *)0xFFFE6806)  /* ディープスタンバイ解除エッジセレクトレジスタ */
#define DSFR        ((VH *)0xFFFE6808)  /* ディープスタンバイ解除要因フラグレジスタ */
#define XTALCTR     ((VB *)0xFFFE6810)  /* XTAL水晶発振器ゲインコントロールレジスタ */

/* FIFO付きシリアルコミュニケーションインターフェース (SCIF) */

#define SCIF0_BASE  ((UW)0xFFFE8000)    /* チャネル0 レジスタベースアドレス */
#define SCIF1_BASE  ((UW)0xFFFE8800)    /* チャネル1 レジスタベースアドレス */
#define SCIF2_BASE  ((UW)0xFFFE9000)    /* チャネル2 レジスタベースアドレス */
#define SCIF3_BASE  ((UW)0xFFFE9800)    /* チャネル3 レジスタベースアドレス */
#define SCIF4_BASE  ((UW)0xFFFEA000)    /* チャネル4 レジスタベースアドレス */
#define SCIF5_BASE  ((UW)0xFFFEA800)    /* チャネル5 レジスタベースアドレス */
#define SCIF6_BASE  ((UW)0xFFFEB000)    /* チャネル6 レジスタベースアドレス */
#define SCIF7_BASE  ((UW)0xFFFEB800)    /* チャネル7 レジスタベースアドレス */

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
	sil_wrh_mem (IPR18, 0x0000);
	sil_wrh_mem (IPR19, 0x0000);
	sil_wrh_mem (IPR20, 0x0000);
	sil_wrh_mem (IPR21, 0x0000);
	sil_wrh_mem (IPR22, 0x0000);
}

#endif /* _MACRO_ONLY */

#endif /* _SH7262_H_ */
