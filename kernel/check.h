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
 *  @(#) $Id: check.h,v 1.2 2001/09/06 12:39:51 hiro Exp $
 */

/*
 *	エラーチェック用マクロ
 */

#ifndef _CHECK_H_
#define _CHECK_H_

/*
 *  優先度の範囲の判定
 */
#define VALID_TPRI(tpri) \
	(TMIN_TPRI <= (tpri) && (tpri) <= TMAX_TPRI)

/*
 *  タスク優先度のチェック（E_PAR）
 */
#define CHECK_TPRI(tpri) {					\
	if (!VALID_TPRI(tpri)) {				\
		return(E_PAR);					\
	}							\
}

#define CHECK_TPRI_INI(tpri) {					\
	if (!(VALID_TPRI(tpri) || (tpri) == TPRI_INI)) {	\
		return(E_PAR);					\
	}							\
}

#define CHECK_TPRI_SELF(tpri) {					\
	if (!(VALID_TPRI(tpri) || (tpri) == TPRI_SELF)) {	\
		return(E_PAR);					\
	}							\
}

/*
 *  タイムアウト指定値のチェック（E_PAR）
 */
#define CHECK_TMOUT(tmout) {					\
	if (!(TMO_FEVR <= (tmout))) {				\
		return(E_PAR);					\
	}							\
}

/*
 *  その他のパラメータエラーのチェック（E_PAR）
 */
#define CHECK_PAR(exp) {					\
	if (!(exp)) {						\
		return(E_PAR);					\
	}							\
}

/*
 *  オブジェクトIDの範囲の判定
 */
#define VALID_TSKID(tskid) \
	(TMIN_TSKID <= (tskid) && (tskid) <= tmax_tskid)

#define VALID_SEMID(semid) \
	(TMIN_SEMID <= (semid) && (semid) <= tmax_semid)

#define VALID_FLGID(flgid) \
	(TMIN_FLGID <= (flgid) && (flgid) <= tmax_flgid)

#define VALID_DTQID(dtqid) \
	(TMIN_DTQID <= (dtqid) && (dtqid) <= tmax_dtqid)

#define VALID_MBXID(mbxid) \
	(TMIN_MBXID <= (mbxid) && (mbxid) <= tmax_mbxid)

#define VALID_MPFID(mpfid) \
	(TMIN_MPFID <= (mpfid) && (mpfid) <= tmax_mpfid)

#define VALID_CYCID(cycid) \
	(TMIN_CYCID <= (cycid) && (cycid) <= tmax_cycid)

/*
 *  オブジェクトIDのチェック（E_ID）
 */
#define CHECK_TSKID(tskid) {					\
	if (!VALID_TSKID(tskid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_TSKID_SELF(tskid) {				\
	if (!(VALID_TSKID(tskid) || (tskid) == TSK_SELF)) {	\
		return(E_ID);					\
	}							\
}

#define CHECK_SEMID(semid) {					\
	if (!VALID_SEMID(semid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_FLGID(flgid) {					\
	if (!VALID_FLGID(flgid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_DTQID(dtqid) {					\
	if (!VALID_DTQID(dtqid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_MBXID(mbxid) {					\
	if (!VALID_MBXID(mbxid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_MPFID(mpfid) {					\
	if (!VALID_MPFID(mpfid)) {				\
		return(E_ID);					\
	}							\
}

#define CHECK_CYCID(cycid) {					\
	if (!VALID_CYCID(cycid)) {				\
		return(E_ID);					\
	}							\
}

/*
 *  呼出しコンテキストのチェック（E_CTX）
 */
#define CHECK_TSKCTX() {					\
	if (sense_context()) {					\
		return(E_CTX);					\
	}							\
}

#define CHECK_INTCTX() {					\
	if (!sense_context()) {					\
		return(E_CTX);					\
	}							\
}

/*
 *  呼出しコンテキストとCPUロック状態のチェック（E_CTX）
 */
#define CHECK_TSKCTX_UNL() {					\
	if (sense_context() || t_sense_lock()) {		\
		return(E_CTX);					\
	}							\
}

#define CHECK_INTCTX_UNL() {					\
	if (!sense_context() || i_sense_lock()) {		\
		return(E_CTX);					\
	}							\
}

/*
 *  ディスパッチ保留状態でないかのチェック（E_CTX）
 */
#define CHECK_DISPATCH() {					\
	if (sense_context() || t_sense_lock() || !(enadsp)) {	\
		return(E_CTX);					\
	}							\
}

/*
 *  その他のコンテキストエラーのチェック（E_CTX）
 */
#define CHECK_CTX(exp) {					\
	if (!(exp)) {						\
		return(E_CTX);					\
	}							\
}

/*
 *  自タスクを指定していないかのチェック（E_ILUSE）
 */
#define CHECK_NONSELF(tcb) {					\
	if ((tcb) == runtsk) {					\
		return(E_ILUSE);				\
	}							\
}

/*
 *  その他の不正使用エラーのチェック（E_ILUSE）
 */
#define CHECK_ILUSE(exp) {					\
	if (!(exp)) {						\
		return(E_ILUSE);				\
	}							\
}

#endif /* _CHECK_H_ */
