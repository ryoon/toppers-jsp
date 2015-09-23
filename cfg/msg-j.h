/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: msg-j.h,v 1.2 2001/03/24 08:22:56 takayuki Exp $
 */

#ifndef __MESSAGE_H
#define __MESSAGE_H

#define MSG_INTERNAL			"内部エラー"
#define MSG_OR					" もしくは "

#define MSG_NULLAPIASSIGNMENT	"NULLの静的APIが登録されようとしました"
#define MSG_NULLSERASSIGNMENT	"NULLのシリアライザが登録されようとしました"

#define MSG_UNKNOWN				"原因不明のエラー"

#define MSG_ILLEGALPRAMDESC		"おかしなパラメータ指定子の指定"
#define MSG_SYNTAXERROR			"パラメータ指定子の文法誤りです"
#define MSG_COULDNOTOPEN		"ファイルが開けませんでした"
#define MSG_FATALONAPI			"静的APIの処理で致命的なエラーが発生しました"
#define MSG_FATALONOUTPUT		"出力処理中にエラーが発生しました"
#define MSG_APINOTEXIST			"その静的APIはありません"
#define MSG_VALIDATIONFAIL		"正当性判定で失敗しました"
#define MSG_VALIDATIONDEADLOCK	"正当性判定がデッドロックを起こしました"
#define MSG_COULDNOTCREATE		"ファイルが作れませんでした"
#define MSG_ILLEGALTYPE			"パラメータの型が異なります"
#define MSG_TOOMANYPARAM		"パラメータの数が多すぎます"
#define MSG_TOOFEWPARAM 		"パラメータの数が少なすぎます"
#define MSG_ALREADYEXISTED		"そのオブジェクトはすでに存在しています"
#define MSG_ALREADYASSIGNED		"そのID値はすでに割り当てられています"
#define MSG_ILLEGALIDENTIFIER	"オブジェクトの識別子が不正です"
#define MSG_NOTEXIST			"オブジェクトが存在しません"
#define MSG_ILLEGALIDENTIFIER2	"\n  (不連続、負値、範囲外などなど)"
#define MSG_NONNULLASSIGNED		"オブジェクトのバッファにNULL以外の値が指定されました"
#define MSG_UNKNOWNOPTION		"不明なオプション[%s]が指定されています"
#define MSG_ALREADYGIVENSRCFILE	"ソースファイル名はすでに与えられている"
#endif
