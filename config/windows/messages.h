/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: messages.h,v 1.3 2002/04/10 11:20:09 takayuki Exp $
 */


/*
 * デバイスエミュレーションサポート メッセージのたぐい
 */

#ifndef __MESSAGES_H
#define __MESSAGES_H

#define MSG_TYP_DEVICE 0x40
#define MSG_TYP_REQUEST 0x80

	//大体共通のメッセージ
#define MSG_ACK 0
#define MSG_NAK 1

	//マネージャからデバイスへのメッセージ
#define MSG_DEVREP_INITIALIZE 0x82
#define MSG_DEVREP_KERNELSTART 0x83
#define MSG_DEVREP_READ 0x84
#define MSG_DEVREP_WRITE 0x85
#define MSG_DEVREP_FINALIZE 0x06
#define MSG_DEVREP_KERNELEXIT 0x07

	//デバイスからマネージャへのメッセージ
#define MSG_DEVREQ_ADD 0xc2
#define MSG_DEVREQ_DELETE 0xc3
#define MSG_DEVREQ_READ 0xc4
#define MSG_DEVREQ_WRITE 0xc5
#define MSG_DEVREQ_INTERRUPT 0xc6
#define MSG_DEVREQ_MAPPING 0xc7
#define MSG_DEVREQ_UNMAPPING 0xc8

	//マネージャからカーネルへ
#define MSG_KERREP_INTERRUPT 0x86

	//カーネルからマネージャへ
#define MSG_KERREQ_KERNELSTART 0x82
#define MSG_KERREQ_KERNELEXIT 0x83
#define MSG_KERREQ_READ 0x84
#define MSG_KERREQ_WRITE 0x85

#endif