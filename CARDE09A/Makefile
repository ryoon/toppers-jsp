#
#  TOPPERS/JSP Kernel
#      Toyohashi Open Platform for Embedded Real-Time Systems/
#      Just Standard Profile Kernel
# 
#  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
#                              Toyohashi Univ. of Technology, JAPAN
# 
#  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
#  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
#  布（以下，利用と呼ぶ）することを無償で許諾する．
#  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
#      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
#      スコード中に含まれていること．
#  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
#      用する場合には，次のいずれかの条件を満たすこと．
#    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
#        権表示，この利用条件および下記の無保証規定を掲載すること．
#    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
#        こと．
#  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
#      害からも，上記著作権者を免責すること．
# 
#  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
#  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
#  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
#  かなる損害に関しても，その責任を負わない．
# 
#  @(#) $Id: Makefile,v 1.2 2000/11/18 04:47:46 honda Exp $
# 


#
#  ターゲット名の定義
#
CPU = sh3
SYS = card_e09a

#
#  Cygwin上でコンパイルするかどうかの設定
#
#CYGWIN = true

#
#  Cプリプロセッサのコマンド名の定義
#
CPP = cpp

#
#  ターゲット依存の定義
#
include ../config/$(CPU)/Makefile.config
include ../config/$(CPU)/$(SYS)/Makefile.config

#
#  共通コンパイルオプションの定義
#
INCLUDES = -I. -I../include -I../config/$(CPU) -I../config/$(CPU)/$(SYS)
CFLAGS := $(CFLAGS) $(INCLUDES) -g -Wall -DWITH_STUB
LDFLAGS := -nostdlib $(LDFLAGS)
LIBS = -lgcc

#
#  アプリケーションプログラムに関する定義
#
UTASK = sample1
UTASK_CFG = $(UTASK).cfg

UTASK_DIR = ../libjsp
UTASK_ASMOBJS =
UTASK_COBJS = $(UTASK).o
UTASK_CFLAGS =
UTASK_LIBS =

#
#  システムサービスに関する定義
#
STASK_DIR = ../systask:../libjsp
STASK_ASMOBJS =
STASK_COBJS = syslog.o timer.o serial.o logtask.o \
	      jsp_perror.o strerror.o
STASK_CFLAGS = -I../systask
STASK_LIBS =

#
#  カーネルに関する定義
#
KERNEL = ../kernel
KERNEL_DIR = ../config/$(CPU):../config/$(CPU)/$(SYS):$(KERNEL)
KERNEL_ASMOBJS = cpu_support.o sys_support.o
KERNEL_COBJS = startup.o banner.o cpu_config.o sys_config.o \
	       task.o wait.o time_event.o kernel_cfg.o \
	       task_manage.o task_sync.o task_except.o \
	       semaphore.o eventflag.o dataqueue.o mailbox.o mempfix.o \
	       time_manage.o cyclic.o sys_manage.o interrupt.o exception.o
KERNEL_AUX_COBJS = makeoffset.o
KERNEL_CFLAGS = -I$(KERNEL)

#
#  スタートアップモジュールに関する定義
#
ifndef START_OBJS
    START_OBJS = start.o
endif

#
#  ターゲットファイル
#
all: jsp.bin

##### 以下は編集しないこと #####

#
#  カーネルのコンフィギュレーションファイルの生成
#
kernel_cfg.c: $(UTASK_CFG)
	$(CPP) $(INCLUDES) $(UTASK_CFG) | ../cfg/cfg

#
#  offset.h の生成規則
#
offset.h: makeoffset.s ../utils/genoffset
	../utils/genoffset makeoffset.s > offset.h

#
#  オブジェクトファイル名の定義（Cygwin では .exe が付く）
#
ifneq ($(CYGWIN),true)
    JSPOBJ = jsp
else
    JSPOBJ = jsp.exe
endif

#
#  ソースファイルのあるディレクトリに関する定義
#
SRCDIR = $(KERNEL_DIR):$(STASK_DIR):$(UTASK_DIR)

vpath %.c $(SRCDIR)
vpath %.S $(SRCDIR)

#
#  コンパイルのための変数の定義
#
KERNEL_ALL_COBJS = $(KERNEL_COBJS) $(KERNEL_AUX_COBJS)
KERNEL_OBJS = $(KERNEL_ASMOBJS) $(KERNEL_COBJS)
STASK_OBJS = $(STASK_ASMOBJS) $(STASK_COBJS)
UTASK_OBJS = $(UTASK_ASMOBJS) $(UTASK_COBJS)
OBJS = $(START_OBJS) $(KERNEL_OBJS) $(STASK_OBJS) $(UTASK_OBJS)

ifdef TEXT_START_ADDRESS
    LDFLAGS := $(LDFLAGS) -Wl,-Ttext,$(TEXT_START_ADDRESS)
endif
ifdef DATA_START_ADDRESS
    LDFLAGS := $(LDFLAGS) -Wl,-Tdata,$(DATA_START_ADDRESS)
endif
ifdef LDSCRIPT
    LDFLAGS := $(LDFLAGS) -T ../config/$(LDSCRIPT)
endif
LIBS := $(STASK_LIBS) $(UTASK_LIBS) $(LIBS)

#
#  特別な依存関係の定義
#
banner.o: Makefile $(filter-out banner.o,$(OBJS))

#
#  全体のリンク
#
$(JSPOBJ): Makefile Makefile.depend kernel_cfg.c offset.h $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o jsp $(OBJS) $(LIBS)

#
#  バイナリファイルの生成
#
jsp.bin: $(JSPOBJ)
	$(OBJCOPY) -O binary -S $(JSPOBJ) jsp.bin

#
#  Sレコードファイルの生成
#
jsp.S: $(JSPOBJ)
	$(OBJCOPY) -O srec -S $(JSPOBJ) jsp.S

#
#  コンパイル結果の消去
#
clean:
	rm -f \#* *~ *.o
	rm -f jsp jsp.exe jsp.bin jsp.S
	rm -f kernel_cfg.c kernel_id.h
	rm -f makeoffset.s offset.h

cleandep:
	rm -f Makefile.depend

realclean: cleandep clean

#
#  依存関係ファイルの生成
#
depend: kernel_cfg.c offset.h
	rm -f Makefile.depend
	../utils/makedep -P $(CPP) -T kernel_cfg.c \
		-O "$(INCLUDES)" $(UTASK_CFG) \
		>> Makefile.depend
	../utils/makedep -C $(CC) -S $(KERNEL_DIR) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" \
		$(KERNEL_ASMOBJS:.o=.S) $(KERNEL_COBJS:.o=.c) \
		>> Makefile.depend
	../utils/makedep -C $(CC) -S $(KERNEL_DIR) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" \
		-s makeoffset.c >> Makefile.depend
	../utils/makedep -C $(CC) -S $(STASK_DIR) \
		-O "$(CFLAGS) $(STASK_CFLAGS)" \
		$(STASK_ASMOBJS:.o=.S) $(STASK_COBJS:.o=.c) \
		>> Makefile.depend
	../utils/makedep -C $(CC) -S $(UTASK_DIR) \
		-O "$(CFLAGS) $(UTASK_CFLAGS)" \
		$(UTASK_ASMOBJS:.o=.S) $(UTASK_COBJS:.o=.c) \
		>> Makefile.depend

#
#  依存関係ファイルをインクルード
#
-include Makefile.depend

#
#  コンパイルルールの定義
#
$(START_OBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(KERNEL_CFLAGS) $<

$(KERNEL_ALL_COBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(KERNEL_CFLAGS) $<

$(KERNEL_ALL_COBJS:.o=.s): %.s: %.c
	$(CC) -S $(CFLAGS) $(KERNEL_CFLAGS) $<

$(KERNEL_ASMOBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(KERNEL_CFLAGS) $<

$(STASK_COBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(STASK_CFLAGS) $<

$(STASK_COBJS:.o=.s): %.s: %.c
	$(CC) -S $(CFLAGS) $(STASK_CFLAGS) $<

ifdef $(STASK_ASMOBJS)
$(STASK_ASMOBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(STASK_CFLAGS) $<
endif

$(UTASK_COBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(UTASK_CFLAGS) $<

$(UTASK_COBJS:.o=.s): %.s: %.c
	$(CC) -S $(CFLAGS) $(UTASK_CFLAGS) $<

ifdef $(UTASK_ASMOBJS)
$(UTASK_ASMOBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(UTASK_CFLAGS) $<
endif

#
#  デフォルトコンパイルルールを上書き
#
%.o: %.c
	@echo "*** Default compile rules should not be used."
	$(CC) -c $(CFLAGS) $<

%.s: %.c
	@echo "*** Default compile rules should not be used."
	$(CC) -S $(CFLAGS) $<

%.o: %.S
	@echo "*** Default compile rules should not be used."
	$(CC) -c $(CFLAGS) $<
