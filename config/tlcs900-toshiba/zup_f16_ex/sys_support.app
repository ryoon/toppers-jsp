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
;   @(#) $Id: sys_support.app,v 1.1 2006/04/10 08:19:25 honda Exp $
;

	$MAXIMUM
	module sys_support_app

;
;	ターゲットシステム依存モジュール アセンブリ言語部
;	（Zup-F16拡張ボード用）
;

#include "jsp_rename.h"
#include "cpu_rename.h"
#include "sys_rename.h"
#include "tmp91cy22.h"

f_code section code large align=1,1

;
; ターゲットハードウェアに依存して必要な初期化処理がある場合
;
	public hardware_init_hook
hardware_init_hook:
; 動作クロック設定(高速クロック1分周を4逓倍の1/2)（6.144*(1/1)*4*(1/2)=12.288）
	ldb		(TADR_SFR_SYSCR0), 0xa0
	ldb		(TADR_SFR_SYSCR1), 0x00
	ldb		(TADR_SFR_SYSCR2), 0x2c ; halt命令にてIDLE2に遷移
	ldb		(TADR_SFR_DFMCR1), 0x0b
	ldb		(TADR_SFR_DFMCR0), 0x40
lockup:			; ロックアップ待ち
	bit		5, (TADR_SFR_DFMCR0)
	jr		nz, lockup
	ldb		(TADR_SFR_DFMCR0), 0x80

; 外部アドレス空間2無効
	ldb		(TADR_SFR_B2CS), 0x00
; 外部アドレス空間1有効(1MByteSRAM)
	ldb		(TADR_SFR_P1CR), 0x00
	ldb		(TADR_SFR_P1FC), 0xff	; AD0〜AD15を使用
	ldb		(TADR_SFR_P2CR), 0xff
	ldb		(TADR_SFR_P2FC), 0x0f	; A16〜A19を使用(残りはポート)
	ldb		(TADR_SFR_P2), 0xff
	ldb		(TADR_SFR_P3), 0x78
	ldb		(TADR_SFR_P3CR), 0x7f
	ldb		(TADR_SFR_P3FC), 0x07	; RD/WR/HWRを使用
	ldb		(TADR_SFR_P4FC), 0x02	; CS1を使用
	ldb		(TADR_SFR_P4CR), 0x0e
	ldb		(TADR_SFR_MSAR1), 0x10	; 0x00100000〜
	ldb		(TADR_SFR_MAMR1), 0x3f	; 0x001fffff
	ldb		(TADR_SFR_B1CS), 0x83	; 0ウェイト/16bitDataBus

; WDT停止
	ldb		(TADR_SFR_WDMOD), 0x00
	ldb		(TADR_SFR_WDCR), 0xb1

	ret

;
;  微少時間待ち
;
	public _sil_dly_nse
_sil_dly_nse:
	ld	WA, (XSP+0x4)
	sub	WA, 3173
	ret	le
	nop
sil_dly_nse1:
	sub	WA, 732
	jr	gt, sil_dly_nse1
	ret


	end


