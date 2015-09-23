/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: chk_jsp.h,v 1.1 2001/09/10 09:03:10 takayuki Exp $
 */

#ifndef __CHK_JSP_H
#define __CHK_JSP_H

#include "include/itron.h"

#include "exception.h"
#include "filecontainer.h"
#include "message.h"

/*
チェックしないといけないこと

タスク
	・tskatrにTA_HLNG,TA_ASM,TA_ACT(0x3)以外が指定されていないこと
	・taskが.textにあること
	・texatrにTA_HLNG,TA_ASM(0x1)以外が指定されていないこと
	・texrtnが.textにあること
	・優先度が範囲内にあること (minpri <= ipri <= maxpri)

セマフォ
	・maxsem < isemcntとなっていないこと
	・sematrにTA_TFIFO,TA_TPRI(0x1)以外の値が設定されていないこと

イベントフラグ
	・flgatrに0x7を超える値が設定されていないこと
	・flgatrにTA_WMULがないこと

データキュー
	・dtqatrに0x1を超える値がないこと

メールボックス
	・mbxatrに0x3を超える値がないこと
	・maxpriが有効範囲にあること (負値でない)

固定長メモリプール
	・mpfatrが0x1を超えない
	・mpf,limitが.dataにある (動的になったらヤバい <- グローバルヒープから取るかも)

周期ハンドラ
	・cycatrが0x7を超えないこと
	・TA_ASMとTA_PHSはサポート外
	・cychdrが.textにあること

割込みハンドラ
	・inhnoが有効な番号であること
	・inhatrが0であること
	・inthdrが.textにある

例外ハンドラ
	・excnoが有効な範囲にある
	・excatrが0であること
	・exchdrが.textにある

初期化ハンドラ
	・iniatrが0x1を超えない
	・inirtnが.textにある
*/

#endif
