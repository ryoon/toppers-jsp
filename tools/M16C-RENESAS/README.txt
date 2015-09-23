
        ＝ TOPPERS/JSPカーネル ユーザズマニュアル ＝
                    （m16c:TM開発環境）

        （Release 1.4.4 対応，最終更新: 28-Dec-2010）

------------------------------------------------------------------------
 TOPPERS/JSP Kernel
     Toyohashi Open Platform for Embedded Real-Time Systems/
     Just Standard Profile Kernel

 Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
                             Toyohashi Univ. of Technology, JAPAN
 Copyright (C) 2003-2004 by Platform Development Center
                                         RIOCH COMPANY,LTD. JAPAN
 Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
               Graduate School of Information Science, Nagoya Univ., JAPAN
 Copyright (C) 2010 by Naoki Saito
            Nagoya Municipal Industrial Research Institute, JAPAN

 上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 によって公表されている GNU General Public License の Version 2 に記
 述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 利用と呼ぶ）することを無償で許諾する．
 (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
     権表示，この利用条件および下記の無保証規定が，そのままの形でソー
     スコード中に含まれていること．
 (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
     用できる形で再配布する場合には，再配布に伴うドキュメント（利用
     者マニュアルなど）に，上記の著作権表示，この利用条件および下記
     の無保証規定を掲載すること．
 (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
     用できない形で再配布する場合には，次のいずれかの条件を満たすこ
     と．
   (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
       作権表示，この利用条件および下記の無保証規定を掲載すること．
   (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
       報告すること．
 (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
     害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．

 本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
------------------------------------------------------------------------

このドキュメントでは、M16C用のルネサスエレクトロニクス社の開発ツールを
使用して，TOPPERS/JSPカーネルを構築するために必要なファイルについて解説する．

1. ディレクトリの構成

jsp
  +--config
  |     +---m16c-renesas
  |            +---oaks16
  |            +---oaks16_mini
  |            +---m3029
  +--tools
  |     +-----M16C-RENESAS
  +--utils
        +-----m16c-renesas


2.ディレクトリとファイルの説明

(1) config/m16c-renesas
	M16C 依存部のファイルが置かれるディレクトリ．
(2) utils/m16c-renesas
	ビルドの際に使われる，以下のツールが置かれるディレクトリ．
		・割込みや CPU 例外のベクタを自動生成するツール(m16cvec.exe)
		・TCB のオフセット値をアセンブラソースに設定するツール(m16coffset.exe)
		(以下は，make を用いてビルドする時のみ使われる)
		・ソースの依存関係を生成する perl スクリプト(makedep.m16c)
(3) tools/M16C-RENESAS
	統合開発環境 TM を用いて TOPPERS/JSP カーネルを構築するための
	ファイルが置かれるディレクトリ．


2.1 config/m16c-renesas

a. Makefile.config
    make コマンドを用いてM16C用 TOPPERS/JSP を構築する場合の make ファイル
b. oaks16/Makefile.config 
    make コマンドを用いて M16C-OAKS16用 TOPPERS/JSP を構築する場合の make ファイル
c. oaks16_mini/Makefile.config 
    make コマンドを用いて M16C-OAKS16 MINI用 TOPPERS/JSP を構築する場合の make ファイル
d. その他 ---- M16C依存部のコード


2.2 utils/m16c-renesas

a. makedep.m16c
    makeを用いて開発を場合、ソースの依存関係を生成するPERLスクリプト
b: m16coffset.c
    m16coffset コマンドC言語記述
c: m16cvec.c
    m16cvec コマンドC言語記述
d: Makefile
    MinGW のGCCを用いて m16coffset, m16cvec コマンドをビルドする際に
    用いるファイル

2.3 tools/M16C-RENESAS

a.Jsp14sample1.tmi,Jsp14sample1.tmk 
   TMを用いて OAKS16用の TOPPERS/JSP を構築するためのプロジェクトファイル
b.Jsp14sample1m.tmi,Jsp14samplem1.tmk
   TMを用いて OAKS16 MINI用の TOPPERS/JSP を構築するためのプロジェクトファイル
c.Jsp14sample1_m3029.tmi,Jsp14samplem1_m3029.tmk 
   TMを用いて M3029用の TOPPERS/JSP を構築するためのプロジェクトファイル
d.Jsp14sample1.id 
   生成した MOT ファイルをOAKS16 ボードのフラッシュROMの書き込む時に使用するIDファイル
e.Jsp14sample1m.id 
   生成した MOT ファイルをOAKS16 MINIボードのフラッシュROMの書き込む時に使用するIDファイル
f.Jsp14sample1_m3029.id 
   生成した MOT ファイルを M3029 のフラッシュROMの書き込む時に使用するIDファイル
g.sample1.cfg 
   OAKS16, M3029用 の TOPPERS/JSP を構築するためのコンフィギュレーションファイル
h.sample1m.cfg 
   OAKS16 MINI用の TOPPERS/JSP を構築するためのコンフィギュファイル


2.4  sample

a.Makefile.m16c-oaks16 
   makeコマンドを用いてOAKS16用のTOPPERS/JSPを構築するためのmakeファイル


3．m16cvec, m16coffset コマンドの構築方法

m16cvec 及び m16coffset コマンドの構築は MinGW を用いてコンパイルを行う．
コンパイルする際，utils/m16c-renesas ディレクトリへ移動し，
単に make コマンドを実行すればよい．
動作確認した GCC のバージョンは 4.5.0 である．
