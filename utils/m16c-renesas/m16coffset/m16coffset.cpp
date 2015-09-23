/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Ryosuke Takeuchi
 *              Platform Development Center RIOCH COMPANY,LTD. JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: m16coffset.cpp,v 1.1 2004/05/27 12:13:17 honda Exp $
 */
// m16coffset.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

#define	BUF_SIZE		512
#define	STR_SIZE		128

#define	NORMAL_STATE	0
#define	CHECK_STATE		1

struct OFF {
	int		no;
	char	no_value[2][STR_SIZE];
	char	no_label[STR_SIZE];
};

static char const version[] = "01.00";
static char const default_input_file[] = "makeoffset.a30";
static char const default_output_file[] = "offset.inc";
static char const lf[] = "\n";

static char input_file[STR_SIZE];
static char output_file[STR_SIZE];
static int  num_offset = 0;
static int  offset_state = NORMAL_STATE;
static struct OFF off_table[20];

unsigned int atoh(char* s);
void itos(unsigned int i, char* s);
void itoh(unsigned int i, char* s);
static bool test_string(char** s, char const * t);
static bool skip_space(char** s);
static bool skip_char(char** s, char const c);

int main(int argc, char* argv[])
{
	FILE* pfo;
	FILE* pfi;
	struct OFF *v;
	int i, j, k, m;
	char* s;
	char* p;
	char* p2;
	char  buf[BUF_SIZE];
	char  c;

	strcpy(input_file, default_input_file);
	strcpy(output_file, default_output_file);

	for(i = 1 ; i < argc ; i++){
		s = argv[i];
		if(*s++ == '-'){
			c = *s;
			while(*s != 0){
				if(*s == ':')
					break;
				s++;
			}
			if(*s++ == ':'){
				switch(c){
				case 'O':		// アウトプットファイルの設定
				case 'o':
					strcpy(output_file, s);
					break;
				case 'R':		// インプットファイルの設定
				case 'r':
					strcpy(input_file, s);
					break;
				default:
					printf("m16coffset -R:input_file -O:output_file\n");
					break;
				}
			}
		}
	}
	printf("version      = %s\n", version);
	printf("input file   = %s\n", input_file);
	printf("output file  = %s\n", output_file);

	if((pfi = fopen(input_file, "r")) == NULL){
		fprintf(stderr, "can't open input file!");
		exit(1);
	}
	if((pfo = fopen(output_file, "w")) == NULL){
		fclose(pfi);
		fprintf(stderr, "can't open output file!");
		exit(1);
	}

	for(;;){
		if((fgets(buf, BUF_SIZE, pfi)) == NULL)
			break;
		s = buf;
		switch(offset_state){
		case CHECK_STATE:
			if(test_string(&s, "! END")){
				num_offset++;
				offset_state = NORMAL_STATE;
				continue;
			}
			if(skip_space(&s))
				continue;
			v = &off_table[num_offset];
			p = &v->no_value[v->no][0];
			if(test_string(&s, "or.w")){
				if(skip_char(&s, '#'))
					continue;
				if(v->no == 0){
					if(skip_char(&s, ','))
						continue;
				}
				while(*s != ',' && *s > ' ')
					*p++ = *s++;
			}
			else if(test_string(&s, "mov.w")){
				if(skip_char(&s, '#'))
					continue;
				while(*s != ',' && *s > ' ')
					*p++ = *s++;
			}
			else if(test_string(&s, "bset")){
				skip_space(&s);
				p2 = &v->no_value[v->no+1][0];
				while(*s != ',' && *s > ' ')
					*p2++ = *s++;
				s++;
				while(*s != ',' && *s > ' ')
					*p++ = *s++;
				v->no++;
			}
			else
				continue;
			v->no++;
			break;
		default:
			if(!test_string(&s, "! BEGIN"))
				continue;
			if(skip_space(&s))
				continue;
			p = off_table[num_offset].no_label;
			while(*s > ' ')
				*p++ = *s++;
			*p++ = 0;
			off_table[num_offset].no = 0;
			offset_state = CHECK_STATE;
			break;
		}
	}
	for(i = 0 ; i < num_offset ; i++){
		v = &off_table[i];
		fputs(v->no_label, pfo);
		fputs("\t.equ\t", pfo);
		itos(atoh(v->no_value[0]), buf);
		fputs(buf, pfo);
		fputs(lf, pfo);
		printf("%s = %s\n", v->no_label, buf);
		if(v->no > 1){
			for(k = m = 0 ; v->no_value[1][k] >= ' ' ; k++){
				if(v->no_value[1][k] == 'H' || v->no_value[1][k] == 'h'){
					m = 1;
					break;
				}
			}
			if(m == 0){
				j = 1 << atoi(v->no_value[1]);
				itoh(j, buf);
				fputs("TCB_enatex_mask\t.equ\t", pfo);
				fputs(buf, pfo);
				fputs("H", pfo);
				fputs(lf, pfo);
				printf("TCB_enatex_mask = %sH\n",buf);
				fputs("TCB_enatex_bit\t.equ\t", pfo);
				fputs(v->no_value[1], pfo);
				fputs(lf, pfo);
				printf("TCB_enatex_bit = %s\n", v->no_value[1]);
				break;
			}
			else{
				fputs("TCB_enatex_mask\t.equ\t", pfo);
				fputs(v->no_value[1], pfo);
				fputs(lf, pfo);
				printf("TCB_enatex_mask = %s\n", v->no_value[1]);
				p = v->no_value[1];
				j = atoh(p);
				for(k = 0 ; k < 16 ; k++){
					if(j & (1<<k))
						break;
				}
				itos(k, buf);
				fputs("TCB_enatex_bit\t.equ\t", pfo);
				fputs(buf, pfo);
				fputs(lf, pfo);
				printf("TCB_enatex_bit = %s\n", buf);
			}
		}
	}
	return 0;
}

/*
 * 16進文字列をバイナリに変換
 */
unsigned int atoh(char* s)
{
	int i = 0;
	for(;;){
		if(*s >= '0' && *s <= '9')
			i = (i * 16) + (*s - '0');
		else if(*s >= 'A' && *s <= 'F')
			i = (i * 16) + (*s - 'A' + 10);
		else if(*s >= 'a' && *s <= 'f')
			i = (i * 16) + (*s - 'a' + 10);
		else
			break;
		s++;
	}
	return i;
}
 
/*
 * バイナリを10進文字列に変換
 */
void itos(unsigned int i, char* s)
{
	if(i >= 10)
		*s++ = (i / 10) + '0';
	*s++ = (i % 10) + '0';
	*s++ = 0;
}

/*
 * バイナリを16進文字列に変換
 */
void itoh(unsigned int i, char* s)
{
	int j, k;

	for(j = 3 ; j >= 0 ; j--){
		k = (i >> (j*4)) & 15;
		if(k >= 10)
			*s++ = (k-10)+'a';
		else
			*s++ = k+'0';
	}
	*s++ = 0;
}

 /*
 * 文字列を比較して同一ならばTURE
 */
static bool test_string(char** s, char const *t)
{
	char*  p;
	char   c;

	p = *s;
	while(*p <= ' '){
		if(*p == 0)
			return false;
		p++;
	}
	while((c = *p++) != 0){
		if(c != *t++)
			break;
		if(*t == 0){
			*s = p;
			return true;
		}
	}
	return false;
}

/*
 * 指定のキャラクタまでスキップ
 */
static bool skip_char(char** s, char const c)
{
	char* p;

	p = *s;
	while(*p != c){
		if(*p == 0)
			break;
		p++;
	}
	if(*p){
		p++;
		*s = p;
		return false;
	}
	else{
		*s = p;
		return true;
	}
}

/*
 * スペースまたはタブをスキップする
 */
static bool skip_space(char** s)
{
	char* p;

	p = *s;
	while(*p <= ' '){
		if(*p == 0)
			break;
		p++;
	}
	*s = p;
	if(*p)
		return false;
	else
		return true;
}
