/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2003      by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 */

/*
 *  Xstormy16共通SFRの定義
 */
#ifndef _CPU_SFRS_H_
#define _CPU_SFRS_H_
                                /*
                                 *  割り込みと例外の制御
                                 */
#define __IL1L 0x7F02
#define __IL1H 0x7F03
#define __IL2L 0x7F04
#define __IL2H 0x7F05
#define __EXCPL 0x7f08
#define __EXCPH 0x7f09
                                /*
                                 *  システムクロックの制御
                                 */
#define __OCR0 0x7F0A
#define __OCR1 0x7F0B
                                /*
                                 *  ワッチドッグタイマー
                                 */
#define __WDTCR 0x7F0C
                                /*
                                 *  ベースタイマー
                                 */
#define __BTCR 0x7F0E
                                /*
                                 *  タイマー0/1
                                 */
#define __T0LR 0x7F10
#define __T0HR 0x7F11
#define __T0CNT 0x7F12
#define __T0PR 0x7F13
#define __T1LR 0x7F14
#define __T1HR 0x7F15
#define __T1CNT 0x7F16
#define __T1PR 0x7F17
                                /*
                                 *  UART
                                 */
#define __U0CR 0x7f38
#define __U1CR 0x7f78
#define __UxCR_RUN 0x80
#define __UxCR_ORERR 0x40
#define __UxCR_BAUDRATE 0x20
#define __UxCR_PARITY 0x10
#define __UxCR_TXEMPTY 0x08
#define __UxCR_TXIE 0x04
#define __UxCR_RXREADY 0x02
#define __UxCR_RXIE 0x01
#define __UxTX_STOPBIT 0x0000

#endif /* _CPU_SFRS_H_ */
