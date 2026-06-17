/*
 *  .bin to .uf2 conveter
 *  Copyright (C) 2017-2022, Roi Takeuchi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>

#ifndef TRUE
#define TRUE        1
#endif
#ifndef FALSE
#define FALSE       0
#endif

typedef int BOOL;

#define BUFFER_SIZE	256

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define htol(a)   ((((a) >> 24) & 0xff) | (((a) >> 8) & 0x0000ff00) | (((a) << 8) & 0x00ff0000) | (((a) << 24) & 0xff000000))

#define UF2_MAGIC_START0 0x0A324655U
#define UF2_MAGIC_START1 0x9E5D5157U
#define UF2_MAGIC_END    0x0AB16F30u

#define UF2_FLAG_NOT_MAIN_FLASH     0x00000001u
#define UF2_FLAG_FILE_CONTAINER     0x00001000u
#define UF2_FLAG_FAMILY_ID_PRESENT  0x00002000u
#define UF2_FLAG_MD5_PRESENT        0x00004000u

#define RP2040_FAMILY_ID 0xe48bff56
#define RP2530_HEAD_ID   0xe48bff57
#define RP2530_FAMILY_ID 0xe48bff59

typedef struct UF2_HEAD {
	uint32_t    magic1;
	uint32_t    magic2;
	uint32_t    flags;
	uint32_t    target_addr;
	uint32_t    payload_size;
	uint32_t    block_no;
	uint32_t    num_blocks;
	uint32_t    file_size;
} UF2_HEAD_t;

typedef struct UF2_TAIL {
	uint8_t     data[220];
	uint32_t    magic3;
} UF2_TAIL_t;

const char Version[] = "1.01";
UF2_HEAD_t uf2_head;
UF2_TAIL_t uf2_tail;
uint32_t   base_addr = 0x10000000;
char       *pname; /* プログラムの名前(メッセージ用) */
uint8_t    buffer[BUFFER_SIZE];
char       oname[256];
uint32_t   family_id = RP2040_FAMILY_ID;
uint32_t   family_no = 0;
BOOL       Verbose  = FALSE;

static uint32_t atohex(char *s)
{
	uint32_t val = 0, c;
	while(*s != 0){
		if(*s >= '0' && *s <= '9')
			c = *s - '0';
		else if(*s >= 'A' && *s <= 'F')
			c = *s - 'A' + 10;
		else if(*s >= 'a' && *s <= 'f')
			c = *s - 'a' + 10;
		else
			c = 0;
		val = val * 16 + c;
		s++;
	}
	return val;
}

void
usage(void)
{
	fprintf (stderr, "usage: %s inputfile [-options]\n",         pname);
	fprintf (stderr, "   <-addr>            write address\n"                   );
	fprintf (stderr, "   <-family>          family number 0:rp2040 1:rp2350\n" );
	exit(-1);
}

/*
 *  入力引数の取得
 */
static void
get_options (int argc, char *argv[])
{
	int       i;
	char      *option;

	for(i = 2;  i < argc ; i++){
	    option = argv[i];
		if(*option == '-'){
			switch (*(option+1)) {
			case 'a':
				if(strncmp("addr", option+1, 1) == 0) {
					i++;
					base_addr = atohex(argv[i]);
					break;
		        }
				fprintf (stderr, "%s : illegal option -%c.\n", pname, *(option+1));
				exit(-1);

			case 'f':
				if(strncmp("family", option+1, 1) == 0) {
					i++;
					family_no = atohex(argv[i]);
					switch(family_no){
					case 1:
						family_id = RP2530_FAMILY_ID;
						break;
					default:
						family_no = 0;
						family_id = RP2040_FAMILY_ID;
						break;
					}
					break;
		        }
				fprintf (stderr, "%s : illegal option -%c.\n", pname, *(option+1));
				exit(-1);

			case 'v':
				if(strncmp("verbose", option+1, 4) == 0) {
					Verbose = TRUE;
					break;
				}
				if(strncmp("version", option+1, 3) == 0) {
					fprintf(stderr, "%s : version %s\n", pname, Version);
					exit(-1);
				}
				fprintf(stderr, "%s : illegal option -%c.\n", pname, *(option+1));
				exit (-1);

			default:
				fprintf (stderr, "%s : illegal option -%c.\n", pname, *(option+1));
				exit (-1);
			}
			continue;
		}
		else{
			usage();
		}
	}
	return;
}

int main(int argc, char* argv[])
{
	FILE* pfi, *pfo;
	struct stat buf;
	int fsize, len, no = 0;
	int result, i;
	char *p;

	pname = argv[0];
	if(argc < 2){
		usage();
	}
	if((pfi = fopen(argv[1], "rb")) == NULL){
		fprintf(stderr, "can't open input file !\n");
		exit(1);
	}
    result = stat(argv[1], &buf);
	if(result != 0 || buf.st_size == 0){
		fprintf(stderr, "input file no data !\n");
		fclose(pfi);
		exit(1);
	}
	fsize = buf.st_size;
	strcpy(oname, argv[1]);
	p = oname;
	i = strlen(oname) - 1;
	while(i > 0 && p[i] != '.'){
		--i;
	}
	oname[i+1] = 'u';
	oname[i+2] = 'f';
	oname[i+3] = '2';
	oname[i+4] = 0;

	memset(&uf2_head, 0, sizeof(uf2_head));
	memset(&uf2_tail, 0, sizeof(uf2_tail));
	uf2_head.magic1 = UF2_MAGIC_START0;
	uf2_head.magic2 = UF2_MAGIC_START1;
	uf2_head.flags  = UF2_FLAG_FAMILY_ID_PRESENT;
	uf2_head.payload_size = BUFFER_SIZE;
	uf2_head.block_no     = 0;
	uf2_head.num_blocks   = 0;
	uf2_tail.magic3 = UF2_MAGIC_END;

	get_options(argc, argv);

	pfo = fopen(oname, "wb");
	if(pfo == NULL){
		fprintf(stderr, "%s : can't open output prn %s\n", pname, oname);
		fclose(pfi);
		exit (-1);
	}

	if(Verbose){
		fprintf(stderr, "%s : input binary data is following\n", pname);
		fprintf(stderr, "     input file name  -> %s\n", argv[1]);
		fprintf(stderr, "     output file name -> %s\n", oname);
		fprintf(stderr, "     write addres     -> %x\n", base_addr);
		fprintf(stderr, "     family no        -> %x\n", family_no);
	}

	if(family_id == RP2530_FAMILY_ID){
		uf2_head.target_addr = base_addr + 0xffff00;
		uf2_head.num_blocks  = 2;
		uf2_head.file_size = RP2530_HEAD_ID;
		for(i = 0 ; i < BUFFER_SIZE ; i++)
			buffer[i] = 0xef;
		if((fwrite((char *)&uf2_head, sizeof(UF2_HEAD_t), 1, pfo)) != 1){
			fprintf (stderr, "%s : can't write uf2 header.\n", pname);
			fclose(pfo);
			fclose(pfi);
			exit(-1);
		}
		if((fwrite(buffer, BUFFER_SIZE, 1, pfo)) != 1){
			fprintf (stderr, "%s : can't write image (%d).\n", pname, no);
			fclose(pfo);
			fclose(pfi);
			exit(-1);
		}
		if((fwrite((char *)&uf2_tail, sizeof(UF2_TAIL_t), 1, pfo)) != 1){
			fprintf (stderr, "%s : can't write uf2 tail.\n", pname);
			fclose(pfo);
			fclose(pfi);
			exit(-1);
		}
	}

	uf2_head.num_blocks = (fsize+255) / 256;
	uf2_head.file_size  = family_id;
	while(no < fsize){
		len = MIN(BUFFER_SIZE, (fsize - no));
		result = fread(buffer, len, 1, pfi);
		if(result == 1){
			uf2_head.target_addr = base_addr + no;
			uf2_head.block_no    = no / 256;
			if((fwrite((char *)&uf2_head, sizeof(UF2_HEAD_t), 1, pfo)) != 1){
				fprintf (stderr, "%s : can't write uf2 header.\n", pname);
				fclose(pfo);
				fclose(pfi);
				exit(-1);
			}
			if(len < BUFFER_SIZE)
				memset(&buffer[len], 0, BUFFER_SIZE-len);
			if((fwrite(buffer, BUFFER_SIZE, 1, pfo)) != 1){
				fprintf (stderr, "%s : can't write image (%d).\n", pname, no);
				fclose(pfo);
				fclose(pfi);
				exit(-1);
			}
			no += len;
			if((fwrite((char *)&uf2_tail, sizeof(UF2_TAIL_t), 1, pfo)) != 1){
				fprintf (stderr, "%s : can't write uf2 tail.\n", pname);
				fclose(pfo);
				fclose(pfi);
				exit(-1);
			}
		}
		else{
			fprintf(stderr, "input file read error[%x][%d] !\n", i, len);
			fclose(pfi);
			exit(1);
		}
	}
	fclose(pfo);
	fclose(pfi);
	return 0;
}

