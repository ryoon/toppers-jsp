;
;   TOPPERS/JSP Kernel
;       Toyohashi Open Platform for Embedded Real-Time Systems/
;       Just Standard Profile Kernel
;
;   Copyright (C) 2006-2010 by Witz Corporation, JAPAN
;  
;   The above copyright holders grant permission gratis to use,
;   duplicate, modify, or redistribute (hereafter called use) this
;   software (including the one made by modifying this software),
;   provided that the following four conditions (1) through (4) are
;   satisfied.
;  
;   (1) When this software is used in the form of source code, the above
;       copyright notice, this use conditions, and the disclaimer shown
;       below must be retained in the source code without modification.
;  
;   (2) When this software is redistributed in the forms usable for the
;       development of other software, such as in library form, the above
;       copyright notice, this use conditions, and the disclaimer shown
;       below must be shown without modification in the document provided
;       with the redistributed software, such as the user manual.
;  
;   (3) When this software is redistributed in the forms unusable for the
;       development of other software, such as the case when the software
;       is embedded in a piece of equipment, either of the following two
;       conditions must be satisfied:
;  
;     (a) The above copyright notice, this use conditions, and the
;            disclaimer shown below must be shown without modification in
;         the document provided with the redistributed software, such as
;         the user manual.
;  
;     (b) How the software is to be redistributed must be reported to the
;         TOPPERS Project according to the procedure described
;         separately.
;  
;   (4) The above copyright holders and the TOPPERS Project are exempt
;       from responsibility for any type of damage directly or indirectly
;       caused from the use of this software and are indemnified by any
;       users or end users of this software from any and all causes of
;       action whatsoever.
;  
;   THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
;   THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
;   INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
;   PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
;   TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
;   INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
;
;   上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
;   ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
;   再配布（以下，利用と呼ぶ）することを無償で許諾する．
;   (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
;       表示，この利用条件および下記の無保証規定が，そのままの形でソース
;       コード中に含まれていること．
;   (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
;       できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
;       ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
;       証規定を掲載すること．
;   (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
;       できない形で再配布する場合には，次のいずれかの条件を満たすこと．
;     (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
;         権表示，この利用条件および下記の無保証規定を掲載すること．
;     (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
;         告すること．
;   (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
;       からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
;       本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
;       く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
;  
;   本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
;   びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
;   る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
;   により直接的または間接的に生じたいかなる損害に関しても，その責任を負
;   わない．
;  
;   @(#) $Id: start.app,v 1.1 2006/04/10 08:19:25 honda Exp $
;

	$MAXIMUM
	module start_app

;
;	カーネル用のスタートアップモジュール（TLCS-900用）
;
#include "jsp_rename.h"
#include "cpu_rename.h"
#include "sys_rename.h"
#include "tlcs900vec.inc"

; 外部参照シンボル定義
	extern large	__StackTop
	extern large	__AreaRAM
	extern large	__AreaRAM_size
	extern large	__DataRAM
	extern large	__DataRAM_ini
	extern large	__DataRAM_size
	extern large	hardware_init_hook
	extern large	software_init_hook
	extern large	_kernel_start

f_code section code large align=1,1

	public _start
_start:
; 割込み禁止
	di		; リセット時はIFFに7が設定されるためDI状態となる
			; ハードウェアリセット時はdi命令不要
			; ソフトにて jp _start する可能性を考慮しコードを残す

; スタック初期化
	ld		xsp, __StackTop

; INTNESTレジスタ初期化
	ld		hl, 0
	ldc		intnest, hl

; hardware_init_hook を呼出し（0 でない場合）
; ターゲットハードウェアに依存して必要な初期化処理がある場合
; は，hardware_init_hook という関数を用意する．
	ld		xhl, hardware_init_hook
	cp		xhl, 0
	call	nz, hardware_init_hook

; 初期値無しRAM領域のクリア
	ld		xde, __AreaRAM			; 先頭番地とサイズ取得
	ld		xbc, __AreaRAM_size
	ld		ix, bc					; 最下位bit保持(後で使用)
	srl		1, xbc					; 2byte単位で処理するためサイズ/2
	jr		z, area_clr_byte
	ld		xhl, xde				; 先頭番地保持(後で使用)
	ldw		(xde+), 0				; 先頭番地クリア
	sub		xbc, 1					; サイズデクリメント
	jr		z, area_clr_byte
	ldirw	(xde+), (xhl+)			; 先頭番地(前で0設定)の値を次番地へ...
									; の要領でBCレジスタ分ブロックロード
	cp		qbc, 0
	jr		eq, area_clr_byte			; 上位16bitが0なら終了
	ld		wa, qbc
area_clr_loop:						; 上位16bit(0x10000単位)での処理
	ldirw	(xde+), (xhl+)			; 0x10000分ブロックロード
	djnz	wa, area_clr_loop
area_clr_byte:
	bit		0, ix					; 領域サイズが奇数なら1byteクリア
	jr		z, area_clr_end
	ldb		(xde), 0
area_clr_end:

; 初期値有りRAM領域へのデータロード
	ld		xde, __DataRAM			; ロード元・ロード先番地とサイズ取得
	ld		xhl, __DataRAM_ini
	ld		xbc, __DataRAM_size
	or		xbc, xbc				; サイズ0なら終了
	jr		z, data_ld_end
	ldirb	(xde+), (xhl+)			; BCレジスタ分ブロックロード
	cp		qbc, 0
	jr		eq, data_ld_end			; 上位16bitが0なら終了
	ld		wa, qbc
data_ld_loop:						; 上位16bit(0x10000単位)での処理
	ldirb	(xde+), (xhl+)			; 0x10000分ブロックロード
	djnz	wa, data_ld_loop
data_ld_end:

; software_init_hook を呼出し（0 でない場合）
; ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
; 理がある場合は，software_init_hook という関数を用意する．
	ld		xhl, software_init_hook
	cp		xhl, 0
	call	nz, software_init_hook

; カーネル起動
	jp		_kernel_start

; EXITループ
exit:
	jr		exit

	end
