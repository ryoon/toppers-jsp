#! /usr/bin/perl
#
#  TOPPERS/JSP Kernel
#      Toyohashi Open Platform for Embedded Real-Time Systems/
#      Just Standard Profile Kernel
# 
#  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
#                              Toyohashi Univ. of Technology, JAPAN
#  Copyright (C) 2004 by Embedded and Real-Time Systems Laboratory
#              Graduate School of Information Science, Nagoya Univ., JAPAN
#  Copyright (C) 2005 by Industrial Technology Institute,
#                              Miyagi Prefectural Government, JAPAN
# 
#  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
#  によって公表されている GNU General Public License の Version 2 に記
#  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
#  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
#  利用と呼ぶ）することを無償で許諾する．
#  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
#      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
#      スコード中に含まれていること．
#  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
#      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
#      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
#      の無保証規定を掲載すること．
#  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
#      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
#      と．
#    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
#        作権表示，この利用条件および下記の無保証規定を掲載すること．
#    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
#        報告すること．
#  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
#      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
# 
#  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
#  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
#  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
#  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
# 
#  @(#) $Id: filter2.pl,v 1.5 2005/11/13 14:41:55 honda Exp $
# 


#
#  ベクタテーブル生成用フィルタ２
#  　・DEF_INH()から、ベクタテーブル生成スクリプト(perl)に変換する
#

#　処理内容
#  　・DEF_INHで始まる行を抽出する
#  　・配列vector_tableへの代入文を生成する
#  　・ラベル名は__kernel_＋割込みハンドラルーチン名＋_entryとする
#  　・割込み番号の末尾にu,ulが付いている場合は削除する
#  　・行末に;を付ける
#  　
#  　備考
#  　　第2引数（割込みハンドラ属性）にTA_HLNGが指定されていると仮定している
#  　　　・第2引数はチェックを行っていない
#  　　　・TA_HLNGの値（0x00u）を文字列置換の正規表現に用いている

while($line = <>) {
	if ($line =~ s/^DEF_INH\(/\$vector_table\[/) {
		$line =~ s/,{0x00u,/\]="__kernel_/;
		$line =~ s/}\)/_entry"/g;
		$line =~ s/((ul)|(lu)|(u)|(l))\]/\]/i;
		chomp($line);
		printf "\t%s;\n", $line;
	}
}

# ライブラリとして呼び出せるように戻り値を１にする。
print "1;\n";
