;
;  TOPPERS/JSP Kernel
;      Toyohashi Open Platform for Embedded Real-Time Systems/
;      Just Standard Profile Kernel
;
;  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
;                              Toyohashi Univ. of Technology, JAPAN
;  Copyright (C) 2001-2007 by Industrial Technology Institute,
;                              Miyagi Prefectural Government, JAPAN
;  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
;                   Tomakomai National College of Technology, JAPAN
;
;  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation
;  によって公表されている GNU General Public License の Version 2 に記
;  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
;  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
;  利用と呼ぶ）することを無償で許諾する．
;  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
;      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
;      スコード中に含まれていること．
;  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
;      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
;      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
;      の無保証規定を掲載すること．
;  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
;      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
;      と．
;    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
;        作権表示，この利用条件および下記の無保証規定を掲載すること．
;    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
;        報告すること．
;  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
;      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
;
;  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
;  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
;  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
;  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
;
;  @(#) $Id: h8_3048f_asm.inc,v 1.7 2007/03/23 07:58:33 honda Exp $
;

;  H8/3048F用定義（アセンブラ用）

 .AIFDEF _H8_3048F_ASM_H_
 .AELSE

_H8_3048F_ASM_H_: .DEFINE ""

        .INCLUDE "util_asm.inc"

;
;  内部レジスタの定義
;

;
;  モードコントロールレジスタ
;
H8MDCR:                 .DEFINE         "H'fffff1"

;
;  I/Oポート
;

;
;  ポート1
;
H8P1DDR:                .DEFINE         "H'ffffc0"

;
;  ポート2
;
H8P2DDR:                .DEFINE         "H'ffffc1"

;
;  ポート5
;
H8P5DDR:                .DEFINE         "H'ffffc8"

;
;  ポート8
;
H8P8DDR:                .DEFINE         "H'ffffcd"
H8P8DDR_CS1:            .DEFINE         "BIT3"


;
;  バスステートコントローラ
;

;　バス幅コントロールレジスタ
H8ABWCR:                .DEFINE         "H'ffffec"
H8ABWCR_ABW1_BIT:       .DEFINE         "1"

;　アクセス・ステート・コントロール・レジスタ
H8ASTCR:                .DEFINE         "H'ffffed"
H8ASTCR_AST1_BIT:       .DEFINE         "1"

;　バス・リリース・コントロール・レジスタ
H8BRCR:                 .DEFINE         "H'fffff3"
H8BRCR_BRLE_BIT:        .DEFINE         "0"
H8BRCR_A21E_BIT:        .DEFINE         "5"

 .AENDI ; _H8_3048F_ASM_H_

