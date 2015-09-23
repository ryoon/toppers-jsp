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
 *  @(#) $Id: itron.h,v 1.2 2000/11/24 06:05:04 hiro Exp $
 */

/*
 *	ITRON仕様共通規定のデータ型・定数・マクロ
 *
 *  このファイルには，スタンダードプロファイルには必要ない定義も含んで
 *  いる．データ型の定義は，スタンダードプロファイルを満たすちょうどの
 *  長さにはしていない．
 *  アセンブリ言語のソースファイルやシステムコンフィギュレーションファ
 *  イルからこのファイルをインクルードする時は，_MACRO_ONLY を定義して
 *  おくことで，マクロ定義以外の記述を除くことができる．
 *
 *  このファイル中のデータ型の定義は，コンパイラに応じて変更する必要が
 *  ある．また，ターゲットに応じて変更したいかもしれない．今後，一部を
 *  ターゲット依存部に移動するなどの変更を行う可能性がある．
 */

#ifndef _ITRON_H_
#define _ITRON_H_

/*
 *  ITRON仕様共通データ型
 */
#ifndef _MACRO_ONLY

#define	__bool	int			/* ブール型 */
#ifndef _MSC_VER
#define	__int8	char			/* 8ビットの整数型 */
#define	__int16	short			/* 16ビットの整数型 */
#define	__int32	int			/* 32ビットの整数型 */
#define __int64	long long		/* 64ビットの整数型 */
#endif /* _MSC_VER */

typedef	signed __int8	B;		/* 符号付き8ビット整数 */
typedef	signed __int16	H;		/* 符号付き16ビット整数 */
typedef	signed __int32	W;		/* 符号付き32ビット整数 */
typedef	signed __int64	D;		/* 符号付き64ビット整数 */

typedef	unsigned __int8		UB;	/* 符号無し8ビット整数 */
typedef	unsigned __int16 	UH;	/* 符号無し16ビット整数 */
typedef	unsigned __int32	UW;	/* 符号無し32ビット整数 */
typedef	unsigned __int64	UD;	/* 符号無し64ビット整数 */

typedef	__int8		VB;		/* 型が定まらない8ビットの値 */
typedef	__int16		VH;		/* 型が定まらない16ビットの値 */
typedef	__int32		VW;		/* 型が定まらない32ビットの値 */
typedef	__int64		VD;		/* 型が定まらない64ビットの値 */

typedef	void		*VP;		/* 型が定まらないものへのポインタ */
typedef	void		(*FP)();	/* プログラムの起動番地（ポインタ） */

typedef signed int	INT;		/* 自然なサイズの符号付き整数 */
typedef unsigned int	UINT;		/* 自然なサイズの符号無し整数 */

typedef __bool		BOOL;		/* 真偽値 */

typedef INT		FN;		/* 機能コード */
typedef	INT		ER;		/* エラーコード */
typedef	INT		ID;		/* オブジェクトのID番号 */
typedef	UINT		ATR;		/* オブジェクトの属性 */
typedef	UINT		STAT;		/* オブジェクトの状態 */
typedef	UINT		MODE;		/* サービスコールの動作モード */
typedef	INT		PRI;		/* 優先度 */
typedef	UINT		SIZE;		/* メモリ領域のサイズ */

typedef	INT		TMO;		/* タイムアウト指定 */
typedef	UINT		RELTIM;		/* 相対時間 */
typedef	UINT		SYSTIM;		/* システム時刻 */

typedef	VP		VP_INT;		/* VP または INT */

typedef	INT		ER_BOOL;	/* ER または BOOL */
typedef	INT		ER_ID;		/* ER または ID */
typedef	INT		ER_UINT;	/* ER または UINT */

#endif /* _MACRO_ONLY */

/*
 *  ITRON仕様共通定数
 */

/*
 *  一般
 */
#define	NULL		0		/* 無効ポインタ */
#define	TRUE		1		/* 真 */
#define	FALSE		0		/* 偽 */
#define	E_OK		0		/* 正常終了 */

/*
 *  エラーコード
 */
#define	E_SYS		(-5)		/* システムエラー */
#define	E_NOSPT		(-9)		/* 未サポート機能 */
#define	E_RSFN		(-10)		/* 予約機能コード */
#define	E_RSATR		(-11)		/* 予約属性 */
#define	E_PAR		(-17)		/* パラメータエラー */
#define	E_ID		(-18)		/* 不正ID番号 */
#define	E_CTX		(-25)		/* コンテキストエラー */
#define	E_MACV		(-26)		/* メモリアクセス違反 */
#define	E_OACV		(-27)		/* オブジェクトアクセス違反 */
#define	E_ILUSE		(-28)		/* サービスコール不正使用 */
#define	E_NOMEM		(-33)		/* メモリ不足 */
#define	E_NOID		(-34)		/* ID番号不足 */
#define	E_OBJ		(-41)		/* オブジェクト状態エラー */
#define	E_NOEXS		(-42)		/* オブジェクト未生成 */
#define	E_QOVR		(-43)		/* キューイングオーバーフロー */
#define	E_RLWAI		(-49)		/* 待ち状態の強制解除 */
#define	E_TMOUT		(-50)		/* ポーリング失敗またはタイムアウト */
#define	E_DLT		(-51)		/* 待ちオブジェクトの削除 */
#define	E_CLS		(-52)		/* 待ちオブジェクトの状態変化 */
#define	E_WBLK		(-57)		/* ノンブロッキング受付け */
#define	E_BOVR		(-58)		/* バッファオーバーフロー */

/*
 *  オブジェクト属性
 */
#define	TA_NULL		0		/* オブジェクト属性を指定しない */

/*
 *  タイムアウト指定
 */
#define TMO_POL		0		/* ポーリング */
#define TMO_FEVR	(-1)		/* 永久待ち */
#define TMO_NBLK	(-2)		/* ノンブロッキング */

/*
 *  ITRON仕様共通マクロ
 */

/*
 *  エラーコード取出しマクロ
 */
#define	MERCD(ercd)	((ER)((B)(ercd)))
#define	SERCD(ercd)	((ercd) >> 8)

#endif /* _ITRON_H_ */
