;
;  TOPPERS/JSP Kernel
;      Toyohashi Open Platform for Embedded Real-Time Systems/
;      Just Standard Profile Kernel
;
;  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
;                              Toyohashi Univ. of Technology, JAPAN
;  Copyright (C) 2001-2004 by Industrial Technology Institute,
;                              Miyagi Prefectural Government, JAPAN
;  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
;                   Tomakomai National College of Technology, JAPAN
;
;  �嵭����Ԥϡ��ʲ��� (1)��(4) �ξ�狼��Free Software Foundation
;  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
;  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
;  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
;  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
;  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
;      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
;      ����������˴ޤޤ�Ƥ��뤳�ȡ�
;  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
;      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
;      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
;      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
;  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
;      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
;      �ȡ�
;    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
;        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
;    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
;        ��𤹤뤳�ȡ�
;  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
;      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
;
;  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
;  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
;  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
;  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
;
;  @(#) $Id: user_config_asm.inc,v 1.6 2005/11/13 14:05:01 honda Exp $
;

;
;���桼����������ե�����졼������������
;

 .AIFDEF _USER_CONFIG_ASM_H_
 .AELSE

_USER_CONFIG_ASM_H_: .DEFINE ""

;  �󥿥�������ƥ������ѥ����å��ݥ��󥿤ν����
STACKTOP:               .DEFINE         "H'ffff0c" ; ��¢RAM

;��chg_ipm�򥵥ݡ��Ȥ��뤫�ɤ��������
;����C�����Ѥ�user_config.h�ˤ�Ʊ�ͤ������ɬ�פʤΤ�����
;
SUPPORT_CHG_IPM:        .DEFINE         " "

;�������å����ȿ� [Hz]
;����C�����Ѥ�user_config.h�ˤ�Ʊ�ͤ������ɬ�פʤΤ�����
;
CPU_CLOCK:              .DEFINE         "24576000"

;
;�����ꥢ��˴ؤ������
;

;  ���ݡ��Ȥ��륷�ꥢ��ǥХ����ο��ʺ��� 2��
;����C�����Ѥ�user_config.h�ˤ�Ʊ�ͤ������ɬ�פʤΤ�����
TNUM_PORT:              .DEFINE         "1"

; ���顼�����ߤ����ϳ����ߤ��̤�������ϥ����Ȥ򳰤���
;����user_config_asm.inc�ˤ�Ʊ�ͤ������ɬ�פʤΤ�����
;
;H8_CFG_SCI_ERR_HANDLER:                .DEFINE         " "

 .AENDI ;  _USER_CONFIG_ASM_H_
