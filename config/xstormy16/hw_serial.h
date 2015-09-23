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
 *  シリアルI/Oデバイス（SIO）ドライバ（Xstprmy16用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include <s_services.h>

/*
 *  SIOの割込みハンドラのベクタ番号
 */
#define INHNO_SIO   INHNO_UART

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block {
    const UH uhMagic ;
    const UH uhBase ;
    const UH uhExcpMask ;
    VP_INT vpiExinf ;
    B bNotExist ;
    B bOpened ;
    B bTxIe ;
    B bRxIe ;
    B bLastIsCr ;
} SIOPCB;

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_ERDY_SND    1u      /* 送信可能コールバック */
#define SIO_ERDY_RCV    2u      /* 受信通知コールバック */

/*
 *  SIOドライバの初期化ルーチン
 */
extern void sio_initialize(void) throw() ;

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB *sio_opn_por(ID siopid, VP_INT exinf) throw() ;

/*
 *  シリアルI/Oポートのクローズ
 */
extern void sio_cls_por(SIOPCB *siopcb) throw() ;

/*
 *  SIOの割込みハンドラ
 */
extern void sio_handler(void) throw() ;

/*
 *  シリアルI/Oポートへの文字送信
 */
extern BOOL sio_snd_chr(SIOPCB *siopcb, char c) throw() ;

/*
 *  シリアルI/Oポートからの文字受信
 */
extern INT sio_rcv_chr(SIOPCB *siopcb) throw() ;

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void sio_ena_cbr(SIOPCB *siopcb, UINT cbrtn) throw() ;

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void sio_dis_cbr(SIOPCB *siopcb, UINT cbrtn) throw() ;

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void sio_ierdy_snd(VP_INT exinf) throw() ;

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void sio_ierdy_rcv(VP_INT exinf) throw() ;

/*
 *  シリアルI/Oポートへの非同期文字出力
 */
extern void sio_asnd_chr( INT c ) throw() ;

/*
 *  シリアルI/Oポートからの非同期文字入力
 */
extern INT sio_arcv_chr( void ) throw() ;

#endif /* _HW_SERIAL_H_ */
