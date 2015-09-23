#
#  TOPPERS/JSP Kernel
#      Toyohashi Open Platform for Embedded Real-Time Systems/
#      Just Standard Profile Kernel
# 
#  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
#                              Toyohashi Univ. of Technology, JAPAN
# 
#  上記著作権者は，Free Software Foundation によって公表されている 
#  GNU General Public License の Version 2 に記述されている条件か，以
#  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
#  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
#  利用と呼ぶ）することを無償で許諾する．
#  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
#      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
#      スコード中に含まれていること．
#  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
#      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
#      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
#      この利用条件および下記の無保証規定を掲載すること．
#  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
#      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
#    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
#        権表示，この利用条件および下記の無保証規定を掲載すること．
#    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
#        こと．
#  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
#      害からも，上記著作権者を免責すること．
# 
#  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
#  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
#  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
#  かなる損害に関しても，その責任を負わない．
# 
#  @(#) $Id: Makefile,v 1.13 2001/11/12 13:14:45 hiro Exp $
# 

#
#  ターゲット名の定義
#
CPU = @(CPU)
SYS = @(SYS)

#
#  ソースファイルのディレクトリの定義
#
JSPDIR = @(JSPDIR)

#
#  Cygwin上でコンパイルするかどうかの設定
#
#CYGWIN = true

#
#  実行環境の定義（複数の実行環境を同時に選択してはならない）
#		 （どれにも該当しない場合は，すべてコメントアウトする）
#
GDB_STUB = true
#PARTNER_J = true

#
#  ターゲット依存の定義のインクルード
#
-include $(JSPDIR)/config/$(CPU)/Makefile.config
ifdef SYS
    -include $(JSPDIR)/config/$(CPU)/$(SYS)/Makefile.config
endif

#
#  共通コンパイルオプションの定義
#
INCLUDES = -I. -I$(JSPDIR)/include -I$(JSPDIR)/config/$(CPU) \
			-I$(JSPDIR)/config/$(CPU)/$(SYS)
CDEFS = $(CPU_CDEFS) $(SYS_CDEFS) $(ENV_CDEFS)
CFLAGS = $(CPU_CFLAGS) $(SYS_CFLAGS) $(CDEFS) $(INCLUDES) -g -Wall
LDFLAGS = -nostdlib $(CPU_LDFLAGS) $(SYS_LDFLAGS)
LIBS = $(CPU_LIBS) $(SYS_LIBS) -lgcc

#
#  アプリケーションプログラムに関する定義
#
UNAME = @(UNAME)
UTASK_CFG = $(UNAME).cfg

UTASK_DIR = $(JSPDIR)/libjsp
UTASK_ASMOBJS =
UTASK_COBJS = $(UNAME).o @(UTASK)
UTASK_CFLAGS =
UTASK_LIBS =

#
#  システムサービスに関する定義
#
STASK_DIR = $(JSPDIR)/systask:$(JSPDIR)/libjsp
STASK_ASMOBJS =
STASK_COBJS = timer.o serial.o logtask.o \
	      vasyslog.o jsp_perror.o strerror.o
STASK_CFLAGS = -I$(JSPDIR)/systask
STASK_LIBS =

#
#  カーネルに関する定義
#
#  KERNEL_ASMOBJS: カーネルライブラリに含める，ソースがアセンブリ言語の
#		   オブジェクトファイル．
#  KERNEL_COBJS: カーネルのライブラリに含める，ソースがC言語のオブジェク
#		 トファイル．
#  KERNEL_ADD_COBJS: カーネルのライブラリに含めず，ロードモジュールに直
#		     接リンクするソースがC言語のオブジェクトファイル．
#  KERNEL_AUX_COBJS: ロードモジュールに含めないが，カーネルのソースファ
#		     イルと同じオプションを適用してコンパイルすべきファ
#		     イル．
#
KERNEL = $(JSPDIR)/kernel
KERNEL_DIR = $(JSPDIR)/config/$(CPU):$(JSPDIR)/config/$(CPU)/$(SYS):$(KERNEL)
KERNEL_ASMOBJS = cpu_support.o sys_support.o
KERNEL_COBJS = startup.o cpu_config.o sys_config.o \
	       task.o wait.o time_event.o syslog.o \
	       task_manage.o task_sync.o task_except.o \
	       semaphore.o eventflag.o dataqueue.o mailbox.o mempfix.o \
	       time_manage.o cyclic.o sys_manage.o interrupt.o exception.o
KERNEL_ADD_COBJS = banner.o kernel_cfg.o
KERNEL_AUX_COBJS = makeoffset.o kernel_chk.o
KERNEL_CFLAGS = -I$(KERNEL)

#
#  スタートアップモジュールに関する定義
#
ifndef START_OBJS
    START_OBJS = start.o
endif

#
#  オブジェクトファイル名の定義（Cygwin では .exe が付く）
#
ifneq ($(CYGWIN),true)
    JSPOBJ = jsp
else
    JSPOBJ = jsp.exe
endif

#
#  ターゲットファイル（複数を同時に選択してはならない）
#
all: $(JSPOBJ)
#all: jsp.out
#all: jsp.bin
#all: jsp.S

#
#  カーネルのコンフィギュレーションファイルの生成
#
kernel_cfg.c kernel_id.h kernel_chk.c: $(UTASK_CFG)
	$(CPP) $(INCLUDES) $(CDEFS) $(UTASK_CFG) | $(JSPDIR)/cfg/cfg -c

##### 以下は編集しないこと #####

#
#  環境に依存するコンパイルオプションの定義
#
ifeq ($(GDB_STUB),true)
    ENV_CDEFS = -DGDB_STUB
else
ifeq ($(PARTNER_J),true)
    ENV_CDEFS = -DPARTNER_J
endif
endif

#
#  jsp.chk の生成規則（静的APIのパラメータチェック）
#
jsp.chk: kernel_chk.s $(JSPDIR)/utils/gencheck
	$(JSPDIR)/utils/gencheck kernel_chk.s > jsp.chk

#
#  offset.h の生成規則（構造体内のオフセット値の算出）
#
offset.h: makeoffset.s $(JSPDIR)/utils/genoffset
	$(JSPDIR)/utils/genoffset makeoffset.s > offset.h

#
#  ソースファイルのあるディレクトリに関する定義
#
vpath %.c $(KERNEL_DIR):$(STASK_DIR):$(UTASK_DIR)
vpath %.S $(KERNEL_DIR):$(STASK_DIR):$(UTASK_DIR)

#
#  コンパイルのための変数の定義
#
KERNEL_LIB_OBJS = $(KERNEL_ASMOBJS) $(KERNEL_COBJS)
STASK_OBJS = $(STASK_ASMOBJS) $(STASK_COBJS)
UTASK_OBJS = $(UTASK_ASMOBJS) $(UTASK_COBJS)
ALL_OBJS = $(START_OBJS) $(KERNEL_ADD_COBJS) $(STASK_OBJS) $(UTASK_OBJS)
ALL_LIBS = $(STASK_LIBS) $(UTASK_LIBS) libkernel.a $(LIBS)

ifdef TEXT_START_ADDRESS
    LDFLAGS := $(LDFLAGS) -Wl,-Ttext,$(TEXT_START_ADDRESS)
endif
ifdef DATA_START_ADDRESS
    LDFLAGS := $(LDFLAGS) -Wl,-Tdata,$(DATA_START_ADDRESS)
endif
ifdef LDSCRIPT
    LDFLAGS := $(LDFLAGS) -T $(JSPDIR)/config/$(LDSCRIPT)
endif

#
#  特別な依存関係の定義
#
banner.o: Makefile $(filter-out banner.o,$(ALL_OBJS)) libkernel.a

#
#  カーネルライブラリファイルの生成
#
libkernel.a: $(KERNEL_LIB_OBJS)
	rm -f libkernel.a
	$(AR) -rcs libkernel.a $(KERNEL_LIB_OBJS)

#
#  全体のリンク
#
$(JSPOBJ): Makefile.depend $(ALL_OBJS) libkernel.a jsp.chk
	$(CC) $(CFLAGS) $(LDFLAGS) -o jsp $(ALL_OBJS) $(ALL_LIBS)
	$(JSPDIR)/cfg/$(TARGET_PREFIX)chk $(JSPOBJ) jsp.chk

#
#  PARTNER-J のためのファイル
#
jsp.out: $(JSPOBJ)
	cp $(JSPOBJ) jsp.out

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
	rm -f libkernel.a jsp jsp.chk jsp.exe jsp.bin jsp.out jsp.S
	rm -f kernel_cfg.c kernel_chk.c kernel_chk.s kernel_id.h
	rm -f makeoffset.s offset.h

cleandep:
	rm -f Makefile.depend

realclean: cleandep clean

#
#  依存関係ファイルの生成
#
depend: kernel_cfg.c offset.h
	rm -f Makefile.depend
	$(JSPDIR)/utils/makedep -P $(CPP) \
		-T "kernel_cfg.c kernel_id.h kernel_chk.c" \
		-O "$(INCLUDES)" $(UTASK_CFG) \
		>> Makefile.depend
	$(JSPDIR)/utils/makedep -C $(CC) -S $(KERNEL_DIR) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" \
		$(KERNEL_ASMOBJS:.o=.S) $(KERNEL_ADD_COBJS:.o=.c) \
		$(KERNEL_COBJS:.o=.c) >> Makefile.depend
	$(JSPDIR)/utils/makedep -C $(CC) -S $(KERNEL_DIR) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" \
		-s makeoffset.c >> Makefile.depend
	$(JSPDIR)/utils/makedep -C $(CC) -S $(KERNEL_DIR) \
		-O "$(CFLAGS) $(KERNEL_CFLAGS)" \
		-s kernel_chk.c >> Makefile.depend
	$(JSPDIR)/utils/makedep -C $(CC) -S $(STASK_DIR) \
		-O "$(CFLAGS) $(STASK_CFLAGS)" \
		$(STASK_ASMOBJS:.o=.S) $(STASK_COBJS:.o=.c) \
		>> Makefile.depend
	$(JSPDIR)/utils/makedep -C $(CC) -S $(UTASK_DIR) \
		-O "$(CFLAGS) $(UTASK_CFLAGS)" \
		$(UTASK_ASMOBJS:.o=.S) $(UTASK_COBJS:.o=.c) \
		>> Makefile.depend

#
#  依存関係ファイルをインクルード
#
-include Makefile.depend

#
#  コンパイラドライバの定義
#
ifdef TARGET
    TARGET_PREFIX = $(TARGET)-
else
    TARGET_PREFIX = 
endif
CC = @(PREFIX)/bin/$(TARGET_PREFIX)gcc
AS = @(PREFIX)/bin/$(TARGET_PREFIX)as
LD = @(PREFIX)/bin/$(TARGET_PREFIX)ld
AR = @(PREFIX)/bin/$(TARGET_PREFIX)ar
NM = @(PREFIX)/bin/$(TARGET_PREFIX)nm
RANLIB = @(PREFIX)/bin/$(TARGET_PREFIX)ranlib
OBJCOPY = @(PREFIX)/bin/$(TARGET_PREFIX)objcopy
OBJDUMP = @(PREFIX)/bin/$(TARGET_PREFIX)objdump

#
#  Cプリプロセッサのコマンド名の定義
#
CPP = $(shell $(CC) -print-prog-name=cpp)

#
#  コンパイルルールの定義
#
KERNEL_ALL_COBJS = $(KERNEL_COBJS) $(KERNEL_ADD_COBJS) $(KERNEL_AUX_COBJS)

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

$(STASK_ASMOBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(STASK_CFLAGS) $<

$(UTASK_COBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(UTASK_CFLAGS) $<

$(UTASK_COBJS:.o=.s): %.s: %.c
	$(CC) -S $(CFLAGS) $(UTASK_CFLAGS) $<

$(UTASK_ASMOBJS): %.o: %.S
	$(CC) -c $(CFLAGS) $(UTASK_CFLAGS) $<

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
