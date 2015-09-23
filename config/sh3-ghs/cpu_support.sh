/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *                     2003         by Advanced Data Controls, Corp
 * 
 *  �嵭����Ԥϡ��ʲ��� (1)��(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: cpu_support.sh,v 1.4 2003/12/19 11:48:37 honda Exp $
 */


#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include <t_config.h>
 			

/*GHS ������֥鵼��̿���Ѵ�*/
#define	long	data.l
#define	org	offset

/*GHS ROM��˥������ߥ���ȥ꡼���ɥ쥹*/
#ifdef 	GHS_STUB
#define	_CpuExceptionHandler	0xa0000848
#endif


/*
 *  �㳰/�����߽�����������롼����
 *  SH3�ϥ٥����ơ��֥����������㳰,TRAPA��ȯ�������VBR+0x100���Ϥ�
 *  ����/����/NMI�����ߤ�ȯ������ȡ�VBR+0x600���Ϥ�¹Ԥ��롣
 */
        
/*
 *	VBR �����ꤹ����
 */
    .section .text
    .align 2        
    .global _BASE_VBR
_BASE_VBR:
     bra _start
     nop
     nop
     nop

/*
 *  Exception 100 vector
 *  ���ɥ쥹���顼,�����å�����̿���㳰��Trapa�㳰����ȯ������ȼ¹Ԥ���롣
 *
 *  CPU�㳰�ϥ�ɥ�ϡ��󥿥�������ƥ����ȤǼ¹Ԥ��롢���Τ��ᡢCPU�㳰�ϥ�
 *  �ɥ��ƤӽФ������㳰/�����ߤ򼨤��쥸�����Х�1�� r7 �쥸�����򥤥�
 *  �����Ȥ����꥿���󤷤Ƥ�����˥ǥ�����Ȥ��롣CPU�㳰������������ƥ�
 *  ���Ȥ�ȯ������regflg �� TRUE �ˤʤä����ˡ� ret_exc ��ʬ�����롣
 *  regflg ������å��������˳����ߤ�ػߤ��ʤ��ȡ�reqflg ������å���˵�
 *  ư���줿�����ߥϥ�ɥ���ǥǥ����ѥå����׵ᤵ�줿���ˡ��ǥ����ѥå�
 *  ����ʤ���
 *
 *		
 *  �㳰��ȯ������ȡ��㳰ȯ�����Υ���ƥ����ȤǤ���spc,pr,ssr,r0��r7������
 *  ���롣�㳰��ȯ�����������ǤΥ���ƥ����Ȥ�Ĵ���󥿥�������ƥ����Ȥʤ�
 *  _exception_from_int����¹Ԥ򳫻Ϥ��롣�㳰���װ���EXPEVT�쥸�����ɤߡ�
 *  exc_table����CPU�㳰�ϥ�ɥ�γ������Ϥ��ɤ߹��ࡣȯ�������㳰���Ф���
 *  CPU�㳰�ϥ�ɥ餬��Ͽ����Ƥ��ʤ���С�_no_reg_exception�˥֥������ơ�
 *  ssr,sr,spc,pr�����Ƥ���Ϥ�����ߤ��롣CPU�㳰�ϥ�ɥ餫��꥿���󤷤�
 *  �����顢reqflg��Ĵ��0�ʳ��ʤ�rec_exc()�����ӡ�0�ʤ鸵�ν����˥꥿����
 *  �롣
 *
 *
 *  BL�ӥåȤΰ����ˤĤ���
 *  ssr,spc �������Υ���ƥ����Ȥ��ᤷ�� rte ��ȯ�Ԥ���ݤˤϡ�����ߡ�NMI 
 *  ��ޤ�ˤ�����դ��ʤ��褦�� BL �ӥåȤ�1�ˤ���ɬ�פ����롥�㳰/����ߥ�
 *  ��ɥ齪λ���ˤ� BL ��ӥåȤ�1�ˤ��� IPM �� MAX_IPM �����ꤹ�롥����ϡ�
 *  BL �ӥåȤ�1�ξ����㳰��ȯ������� CPU ���ꥻ�åȤ���뤳�Ȥ��θ���ơ�
 *  �����Τ��ᡤssr,spc����������ľ����BL��1�����ꤷ�Ƥ��롥
 *       
 */	
			

    .org   0x0100              /*  _BASE_VBR + 0x0100���Ϥ�����   */
    .align 2
    .global _general_exception
_general_exception:
#ifdef GHS_STUB /* GHS */
	mov.l	r0,@-r15
	mov.l	r1,@-r15
	mov.l   _expevt,r0
	mov.l   @r0,r1     
	shlr2   r1           
	shlr    r1
	mov.l   _exc_table_exc,r0
	mov.l   @(r0,r1),r0        
	tst     r0,r0
	mov.l	@r15+,r1	
	mov.l	@r15+,r0
	bt      _rom_exception   /* _rom_exception�إ�����             */
#endif
 /* spc,pr,ssr �򥹥��å�����¸���� */
    stc.l  spc,@-r15          
    sts.l  pr,@-r15               
    stc.l  ssr,@-r15
    stc.l  gbr,@-r15					
    sts.l  macl,@-r15					
    sts.l  mach,@-r15
    stc.l  r0_bank,@-r15 /* �㳰ȯ������r0��r7����¸        */    
    stc.l  r1_bank,@-r15
    stc.l  r2_bank,@-r15
    stc.l  r3_bank,@-r15
    stc.l  r4_bank,@-r15
    stc.l  r5_bank,@-r15
    stc.l  r6_bank,@-r15    
    stc.l  r7_bank,@-r15
    tst    r7,r7               /* �㳰ȯ�����Υ���ƥ����Ȥ�Ƚ��            */
    bf/s   _exception_from_exc /* ȯ�������㳰/�����ߥϥ�ɥ�ʤ饸���� */
     add    #0x01,r7	       /* �㳰/�����ߥͥ��Ȳ���򥤥󥯥����   */
    mov    r15,r1              /* ����褬�����ߥϥ�ɥ�Ǥʤ��Τǥ�����  */
    mov.l  _stacktop_exc,r15   /* ���������ؤ����Υ����å��ݥ��󥿤���¸    */ 
    mov.l r1,@-r15            
#ifdef GHS_HOOK
	mov.l   r1,@-r15
	mov.l   r7,@-r15
	stc     spc,r0
	mov.l   r0,r5
	mov.l   _expevt,r0        /* �����ߵ��Ĥ���Ⱦä���쥸��������¸    */
	mov.l   _exception_hook_dis,r2
	jsr     @r2
	mov.l   @r0,r4            /* EXPEVT�쥸���������		              */
	mov.l   @r15+,r7
	mov.l   @r15+,r1
#endif

    mov.l  _expevt,r0          /* �㳰����Ĥ���Ⱦä���쥸��������¸      */
    mov.l  @r0,r5              /* EXPEVT�쥸���������                      */
    ldc    r1,r4_bank          /* CPU�㳰�ϥ�ɥ�ؤΰ���                   */
    ldc    r5,r5_bank	     
    mov.l  _unmak_rb_bl_exc,r2 /* BL�ӥåȤ�0�ˡ��쥸�����Х󥯤��ڤ��ؤ�   */
    stc    sr,r3               
    and    r3,r2			
    ldc    r2,sr               
    shlr2  r5                  /* EXPEVT��3�ӥåȱ����ե�                   */
    shlr   r5                
    mov.l  _exc_table_exc,r0   /* exc_table�Υ��ɥ쥹��r0��                 */
    mov.l  @(r0,r5),r2         /* �ϥ�ɥ�Υ��ɥ쥹�����                  */
    tst    r2,r2               /* �ϥ�ɥ餬��Ͽ����Ƥ��ʤ��㳰�ʤ�        */
    bt     _no_reg_exception   /* no_reg_exception�إ�����                */
    jsr    @r2                 /* CPU�㳰�ϥ�ɥ��                         */
     nop
    mov.l  _mask_md_ipm_exc,r0
    ldc    r0,sr               /* �����߶ػ�                              */
    stc    r7_bank,r7          /* �㳰/�����ߤΥͥ��Ȳ����ǥ������   */
    dt     r7
    ldc    r7,r7_bank
    mov.l  _reqflg_exc,r2      /* reqflg�Υ����å�                          */
    mov.l  @r2,r1
    tst    r1,r1               /* reqflg��FALSE�ʤ�ret_to_task_exc������    */
    bt/s   _ret_to_task_exc    /* �����������                              */
     mov.l  @r15,r15           /* ����褬�������ʤΤǥ����å����᤹        */
    xor    r0,r0               
    bra    ret_exc
     mov.l  r0,@r2             /* reqflg�򥯥ꥢ                            */


/*
 *   exception_from_int
 *   CPU�㳰ȯ�����Υ���ƥ����Ȥ��󥿥�������ƥ����Ȥξ��ν���
 *   ����������ƥ����Ȥξ��Ȥν����ΰ㤤�ϡ���CPU�㳰�ϥ�ɥ��
 *   ��������λ�����ɬ�����ν�������뤳�ȤǤ��롣
 */
	
_exception_from_exc:
#ifdef GHS_HOOK
	mov.l   r7,@-r15
	stc     spc,r0
	mov.l   r0,r5
	mov.l   _expevt,r0       /* �����ߵ��Ĥ���Ⱦä���쥸��������¸    */
	mov.l   _exception_hook_dis,r2
	jsr     @r2
	mov.l   @r0,r4           /* EXPEVT�쥸���������                      */
	mov.l   @r15+,r7
	
#endif
    mov.l   _expevt,r0        /* �㳰����Ĥ���Ⱦä���쥸��������¸      */
    mov.l   @r0,r5            /* EXPEVT�쥸���������                      */
    ldc     r15,r4_bank       /* CPU�㳰�ϥ�ɥ�ؤΰ���                   */
    ldc     r5,r5_bank	     
    mov.l   _unmak_rb_bl_exc,r2 /* BL�ӥåȤ�0�ˡ��쥸�����Х󥯤��ڤ��ؤ� */
    stc     sr,r3
    and     r3,r2			
    ldc     r2,sr               
    shlr2   r5                /* EXPECT��3�ӥåȱ����ե�                   */
    shlr    r5                	
    mov.l   _exc_table_exc,r0 /* exc_table�Υ��ɥ쥹��r0��                 */
    mov.l   @(r0,r5),r2       /* �ϥ�ɥ�Υ��ɥ쥹�����                  */
    tst     r2,r2             /* �ϥ�ɥ餬��Ͽ����Ƥ��ʤ��㳰�ʤ�        */
    bt      _no_reg_exception /* no_reg_exception�إ�����                */
    jsr     @r2               /* CPU�㳰�ϥ�ɥ��                         */
     nop
    mov.l  _mask_md_ipm_exc,r0
    ldc    r0,sr              /* BL=1,�����߶ػ�                         */
    stc    r7_bank,r7         /* �㳰/�����ߤΥͥ��Ȳ����ǥ������   */
    dt     r7
    ldc    r7,r7_bank
_ret_to_task_exc:
    mov.l  @r15+,r7           /* r0��r1,ssr,pr,spc�������������������     */
    mov.l  @r15+,r6
    mov.l  @r15+,r5
    mov.l  @r15+,r4
    mov.l  @r15+,r3
    mov.l  @r15+,r2
    mov.l  @r15+,r1        
    mov.l  _mask_md_bl_exc,r0 
    ldc    r0,sr             /* BL��1��                                   */
    mov.l  @r15+,r0
    lds.l  @r15+,mach                
    lds.l  @r15+,macl        
    ldc.l  @r15+,gbr               
    ldc.l  @r15+,ssr            
    lds.l  @r15+,pr
    ldc.l  @r15+,spc
    rte
     nop

#ifdef GHS_STUB /* GHS */
_rom_exception:
	stc.l	ssr,@-r15
	stc.l	spc,@-r15
	mov.l	r0,@-r15
	mov.l	r1,@-r15
	mov.l	L._CpuException1,r1
	jmp	@r1
	mov	#1,r0

	.align	4
L._CpuException1:
	.data.l	_CpuExceptionHandler
#endif

/*	
 *  no_reg_exception()
 *  CPU�㳰�Ȥ�����Ͽ����Ƥ��ʤ��㳰��ȯ������ȸƤӽФ����
 *  �㳰�����ɡ��㳰��ȯ������������pc,sr,pr����Ϥ��ƥ����ͥ�
 *  ����ߤ��롣
 */					
_no_reg_exception:
    mov.l  _expevt,r5
    mov.l  @r5,r4
    stc    spc,r5
    stc    ssr,r6
    sts    pr,r7
    mov.l  _cpu_expevt_k,r1
    jsr    @r1
     nop

	.align  4
_stacktop_exc:
    .long  STACKTOP            /* ��������Ω���Υ����å��ν����  */
_expevt:	
    .long  EXPEVT
_exc_table_exc:	
    .long  _exc_table
_unmak_rb_bl_exc:		
    .long  0x4fffffff
_mask_md_ipm_exc:	
    .long  0x40000000 + MAX_IPM << 4	
_reqflg_exc:
    .long  _reqflg
_cpu_expevt_k:	
    .long  _cpu_expevt	
_mask_md_bl_exc:	
    .long  0x50000000 + MAX_IPM << 4	

#ifdef GHS_HOOK
	.global _exception_hook
_exception_hook_dis:
	.long   _exception_hook
#endif


	
/*
 *  �������ǥ����ѥå���
 *
 *  _dispatch �ϡ�r7_bank1 = 0,����߶ػ߾��֤ǸƤӽФ��ʤ���Фʤ��
 *  ����_exit_and_dispatch �⡤r7_bank1 = 0������߶ػ߾��֤ǸƤӽФ�
 *  �Τ���§�Ǥ��뤬�������ͥ뵯ư�����б����뤿�ᡤr7_bank1 = 1�ǸƤ�
 *  �Ф������ˤ��б����Ƥ��롥
 */

    .text
    .align 2
    .global _dispatch
_dispatch:
    sts.l  pr,@-r15         /* pr,r8��r15 �򥹥��å�����¸           */
    mov.l  r14,@-r15        /* r0��r7�ϸƤӽФ�������¸���Ƥ��뤿��  */    
    mov.l  r13,@-r15        /* ��¸����ɬ�פ�̵��                    */    
    mov.l  r12,@-r15
    mov.l  r11,@-r15
    mov.l  r10,@-r15
    mov.l  r9, @-r15
    mov.l  r8, @-r15
    mov.l  _runtsk_dis,r2   /* r0 <- runtsk                          */
    mov.l  @r2,r0
    mov.l  r15,@(TCB_sp,r0) /* �����������å���TCB����¸ 60�ʲ��ʤ�OK*/		
    mov.l  dispatch_r_k,r1  /* �¹ԺƳ����Ϥ���¸                    */
    mov.l  r1,@(TCB_pc,r0)  /* �¹ԺƳ����Ϥ�TCB����¸  60�ʲ��ʤ�OK */
    bra    dispatcher
     nop

dispatch_r:	
    mov.l  @r15+,r8         /* �쥸����������                       */
    mov.l  @r15+,r9
    mov.l  @r15+,r10
    mov.l  @r15+,r11
    mov.l  @r15+,r12
    mov.l  @r15+,r13
    mov.l  @r15+,r14
    lds.l  @r15+,pr
    /*
     *  �������㳰�����롼����ε�ư
     *  dispatch_r��dispatcher����ƤӽФ���뤿�ᡤ
     *  tcb�Υ��ɥ쥹��r0�����äƤ��롥      
     */
    mov.l  @(TCB_enatex,r0),r1
    mov.l  _enatex_mask_dis,r2
    tst    r1,r2
    bt     dispatch_r_1         /* enatex �� FALSE �ʤ�꥿���� */
    mov.l  @(TCB_texptn,r0),r1  /* texptn�������               */
    tst    r1,r1
    bt     dispatch_r_1         /* texptn �� 0��̵�����        */
    mov.l  _call_texrtn_dis,r1  /* �������㳰�롼����θƤӽФ� */
    jmp    @r1        
     nop                          
dispatch_r_1:   
    rts
     nop                

    .global _exit_and_dispatch
_exit_and_dispatch:
    xor    r1,r1                 /* r7_bank1��0���ꥢ             */
    ldc    r1,r7_bank
dispatcher:
    /*
     * �����ˤϳ����߶ػߤ���뤳��
     */
    mov.l  _schedtsk_dis,r12    /* r0 <- schedtsk                   */
    mov.l  @r12,r0             
    cmp/eq #0,r0                /* schedtsk �����뤫��              */
    bt     dispatcher_1         /* ̵����Х�����                 */
#ifdef GHS_HOOK 
	mov.l   _dispatch_hook_dis,r2
	jsr     @r2
	mov     r0,r4
	mov.l   @r12,r0
#endif

    mov.l  _runtsk_dis,r2
    mov.l  r0,@r2               /* schedtsk��runtsk��               */
    mov.l  @(TCB_sp,r0),r15     /* TCB���饿���������å�������      */
    mov.l  @(TCB_pc,r0),r1      /* TCB����¹ԺƳ����Ϥ�����        */
    jmp    @r1 
     nop
dispatcher_1:
    /* ���ߥǥ��������ͤ�����ɤ߹���Τϸ�Ψ�������Τǻ������Ѱդ���
     * r8  : �����߶ػߤΤ����SR����   
     * r9  : �������Ԥ����SR����
     * r10 : reqflg������
     * r11 : 0x01
     * r12 : 0x00
     * r13 : sleep���SR�ξ���       
     */
    mov.l  _mask_md_ipm_dis,r8  /* �����߶ػߤ�SR����             */
#ifdef NO_SLEEP
    mov.l  _mask_md_dis,r9      /* �������Ԥ����SR����           */        
#else
    mov.l  _mask_bl_md_dis,r9   /* �������Ԥ����SR����           */
#endif                          
#ifdef SUPPORT_CHG_IPM
    mov.l  _task_intmask_k,r2
    mov.l  @r2,r3
    or     r3,r9
#endif
    mov.l  _reqflg_dis,r10
    mov    #0x01,r11
    xor    r12,r12
    mov.l  _stacktop_dis,r15   /* �����å�������ߥ����å���     */
    ldc    r11,r7_bank         /* r7_bank1 ��1�ˤ��Ƴ����߾��֤� */        
dispatcher_2:        
    /*
     *  �����ǳ���ߥ⡼�ɤ��ڤ괹����Τϡ�������ȯ���������߽���
     *  �ˤɤΥ����å���Ȥ����Ȥ�������β��ȡ�����ߥϥ�ɥ����
     *  �Υ������ǥ����ѥå����ɻߤȤ�����Ĥΰ�̣�����롥
     */
     /*
      *  ����ߵ��ĸ��sleep̿���¹Ԥ���ȡ�����ߵ���̿��μ¹�����
      *  ������׵᤬���äƤ����硤����ߵ���̿��μ¹Ԥȶ��˳���ߤ�
      *  �����դ���줽��������ˡ�reqflg�Υ����å��ǤϤʤ� sleep ����
      *  �����äƤ��ޤ���
      *  ��������ϡ�����ߤε��Ĥ� sleep ���֤ؤΰܹԤ����ȥߥå��˼�
      *  �ԤǤ��ʤ����Ȥ˵������롥
      *  ���Τ��ᡤsleep �¹�ľ���� SR �ѹ��Ǥ� BL �ӥåȤ�'1'�����ꤹ
      *  �뤳�Ȥǡ�����ߤε��Ĥ� sleep ���֤ؤΰʹߤ򥢥ȥߥå��˼¹�
      *  ���롥
      *  ldc r9,sr �Ǥ�BL�ӥåȤ����åȤ���뤿�ᡤ����ߤϼ����դ�����
      *  �꡼�ץ⡼�ɤ˰ܹԤ��롥���꡼�ץ⡼����ϡ�BL�ӥåȤ�1�Ǥ��
      *  ���ߤ�����դ��뤿��(SH7709�ϡ��ɥ������ޥ˥奢�� 8-10)�����꡼
      *  �ץ⡼�ɤؤΰܹ�ľ��˳���ߤ���ե��꡼�ץ⡼�ɤ����������롥
      */
               
    ldc    r9,sr               /* �����ߤε���                   */
#ifndef NO_SLEEP
    sleep                      /* ������Ԥ�                       */
#endif /* NO_SLEEP */                
    ldc    r8,sr               /* �����߶ػ�                     */  
    mov.l  @r10,r7             /* r7 <- reqflg                     */
    tst    r7,r7               /* reqflg �� FALSE�ʤ�              */
    bt     dispatcher_2        /* dispatcher_2 ��                  */
    ldc    r12,r7_bank         /* r7_bank1 �򥯥ꥢ                */        
    mov.l  r12,@r10            /* reqflg �� FALSE��                */
    bra    dispatcher
     nop	
    .align 4
_runtsk_dis:	
    .long _runtsk
_schedtsk_dis:	
    .long _schedtsk
_enatex_mask_dis:    
    .long TCB_enatex_mask
_call_texrtn_dis:	
    .long _call_texrtn
_mask_md_ipm_dis:	
    .long 0x40000000 + MAX_IPM << 4
_mask_md_dis:	
    .long 0x40000000
_mask_bl_md_dis:	
    .long 0x50000000	        
dispatch_r_k:	
    .long dispatch_r
_stacktop_dis:
    .long STACKTOP           /* ��������Ω���Υ����å��ν����	*/
_reqflg_dis:
    .long  _reqflg
#ifdef GHS_HOOK
	.global _dispatch_hook
_dispatch_hook_dis:
	.long   _dispatch_hook
#endif


/* 
 *  �����ߥϥ�ɥ�/CPU�㳰�ϥ�ɥ�и�����
 *
 * ����褬��������reqflg�����åȤ���Ƥ�����Τߤ����ˤ��롣
 * r7_bank1 = 0,�����߶ػ߾���,������å��쥸��������¸����
 * ���֤ǸƤӽФ����ȡ� 
 *	
 */
    .text
    .align 2
    .globl ret_int
    .globl ret_exc
ret_exc:
ret_int:
    mov.l  _runtsk_ret,r1     /* r0 <- runtsk                  */
    mov.l  @r1,r0           
    mov.l  _enadsp_ret,r2     /* enadsp�Υ����å�              */
    mov.l  @r2,r3
    tst    r3,r3           
    bt     ret_int_1           
    mov.l  _schedtsk_ret,r4   /* r5 <- schedtsk                */ 
    mov.l  @r4,r5
    cmp/eq r0,r5              /* runtsk �� schedtsk �����     */
    bt     ret_int_1
    mov.l  r14,@-r15          /* �Ĥ�Υ쥸��������¸          */
    mov.l  r13,@-r15					
    mov.l  r12,@-r15					
    mov.l  r11,@-r15					
    mov.l  r10,@-r15					
    mov.l  r9,@-r15					
    mov.l  r8,@-r15
    mov    #TCB_sp,r1        /* �����������å�����¸           */
    mov.l  r15,@(r0,r1)    
    mov.l  ret_int_r_k,r1    /* �¹ԺƳ����Ϥ���¸             */
    mov    #TCB_pc,r2	
    mov.l  r1,@(r0,r2)  
    bra    dispatcher
     nop
    
ret_int_r:
    mov.l  @r15+,r8		
    mov.l  @r15+,r9		
    mov.l  @r15+,r10		
    mov.l  @r15+,r11		
    mov.l  @r15+,r12		
    mov.l  @r15+,r13		
    mov.l  @r15+,r14		
ret_int_1:
    /*
     *  �������㳰�����롼����ε�ư
     *  ret_int_r �� dispatcher ����ƤӽФ���뤿�ᡤtcb�Υ��ɥ쥹��r0��
     *  ���äƤ��롥
     *  �ʤ�������ߤνи��ǥ����������å���ȯ�����ʤ����ϥǥ����ѥå���
     *  ���̤餺�� ret_int ����ľ�� ret_int_1 ������       
     */
    mov.l  @(TCB_enatex,r0),r1
    mov.l  _enatex_mask_ret,r2
    tst    r1,r2
    bt     ret_int_2            /* enatex �� FALSE �ʤ�꥿����  */
    mov.l  @(TCB_texptn,r0),r1  /* texptn�������                */
    tst    r1,r1
    bt     ret_int_2            /* texptn �� 0��̵�����T<= 0    */
    mov.l _call_texrtn_ret,r1   /* �������㳰�롼����θƤӽФ�  */
    jsr   @r1        
     nop       
ret_int_2:               
#ifdef SUPPORT_CHG_IPM
    mov    #32,r0
    mov.l  @(r0,r15),r1
    mov.l  _unmask_ipm,r2
    and    r2,r1
    mov.l  _task_intmask_k,r2
    mov.l  @r2,r3
    or     r3,r1
    mov.l  r1,@(r0,r15)
#endif /* SUPPORT_CHG_IMP */
    mov.l  @r15+,r7            /* spc,pr,ssr,������å��쥸���������� */
    mov.l  @r15+,r6        
    mov.l  @r15+,r5
    mov.l  @r15+,r4
    mov.l  @r15+,r3
    mov.l  @r15+,r2
    mov.l  @r15+,r1
    mov.l  _mask_md_bl_ret,r0
    ldc    r0,sr               /* BL��1��                               */
    mov.l  @r15+,r0
    lds.l  @r15+,mach                
    lds.l  @r15+,macl        
    ldc.l  @r15+,gbr       
    ldc.l  @r15+,ssr    
    lds.l  @r15+,pr
    ldc.l  @r15+,spc
    rte
     nop
        
    .align 4
_runtsk_ret:
    .long _runtsk
_schedtsk_ret:	
    .long _schedtsk
_enadsp_ret:	
    .long _enadsp
ret_int_r_k:
    .long ret_int_r
_enatex_mask_ret:    
    .long TCB_enatex_mask        
_call_texrtn_ret:	
    .long _call_texrtn
_mask_md_bl_ret:	
    .long  0x50000000

/*
 *  ��������ư����
 */ 
    .text
    .align 2
    .globl _activate_r
_activate_r:
    stc    sr,r0               /* �����ߵ��� */
    mov.l  _unmask_ipm,r1
    and    r1,r0	
#ifdef SUPPORT_CHG_IPM
    mov.l  _task_intmask_k,r1
    mov.l  @r1,r2
    or     r2,r0
#endif /* SUPPORT_CHG_IPM */
    ldc    r0,sr
    mov.l  @r15+,r1
    lds.l  @r15+,pr
    mov.l  @r15+,r4
    jmp    @r1
     nop
    .align 4
_unmask_ipm:
    .long  0xffffff0f
#ifdef SUPPORT_CHG_IPM		
_task_intmask_k:
    .long _task_intmask
#endif /* SUPPORT_CHG_IPM */
		
		
/*
 *  Exception 600 vector
 *  �����ߤ�ȯ������ȼ¹Ԥ���롣		
 *
 *
 *  SH3�ϳ����ߤ�ȯ������Ȥ��٤�VBR+0x600���Ϥ���ץ�������¹Ԥ��뤿�ᡢ
 *  ���������֤���롼����Ǥޤ������å����ڤ��ؤ����쥸��������¸,IPM������,
 *  �������װ���Ƚ���Ԥ����θ�BL�ӥåȤ�0�ˤ��Ƴ����ߥϥ�ɥ��Ƥ�ɬ��
 *  �����롣
 *  �������װ����б����������ߥϥ�ɥ�γ������Ϥ�����int_table[]����Ͽ��
 *  �ƸƤӽФ������ե��åȤη׻��ϡ��������װ��쥸�����򱦤�2�ӥåȥ��եȤ�
 *  �ƹԤ����������װ��쥸������SH7708�Ǥ�INTEVT�쥸�����˥��åȤ���뤬��
 *  SH7709�����SH7709A�Ǥ�INTEVT2�˥��åȤ���뤿�ᡢifdef�ˤ���ڤ�ʬ���Ƥ�
 *  �롣�ޤ��������Υǥ��Х����ˤĤ��Ƴ������װ��쥸�������Ϥ�����ͤ�IPM��
 *  �δ֤ˤϴط����ʤ����ᡢ�Ƴ������װ����б�����IPM�����SR�����Ƥ�����
 *  int_plevel_table[]����Ͽ���Ƥ�����ɤ߽Ф���
 *
 *  reqflg ������å��������˳���ߤ�ػߤ��ʤ��ȡ�reqflg ������å���
 *  �˵�ư���줿����ߥϥ�ɥ���ǥǥ����ѥå����׵ᤵ�줿���ˡ��ǥ�
 *  ���ѥå�����ʤ���
 *
 */

    .org    0x0600        /* _BASE_VBR + 0x0600���Ϥ����� */
    .align 2
    .global _interrupt
_interrupt:    
#ifdef GHS_STUB
	mov.l	r0,@-r15
	mov.l	r1,@-r15
	mov.l   _intevt,r0 
	mov.l   @r0,r1     
	shlr2   r1           
	shlr    r1
/*	mov.l   _int_table_k,r0
	mov.l   @(r0,r1),r0
*/	
	mov.l	@r15+,r1
	mov.l	@r15+,r0
	bt      _rom_interrupt   
#endif
    stc.l  spc,@-r15       /* ¿�ų����ߤ�����Ⱦä��Ƥ��ޤ��Τ�        */
    sts.l  pr,@-r15        /* spc,pr,ssr,gbr,macl,mach                    */
    stc.l  ssr,@-r15    
    stc.l  gbr,@-r15                    
    sts.l  macl,@-r15                    
    sts.l  mach,@-r15                                                    
    stc.l  r0_bank,@-r15   /* ,r0��r7�򥹥��å�����¸   */ 
    stc.l  r1_bank,@-r15
    stc.l  r2_bank,@-r15
    stc.l  r3_bank,@-r15
    stc.l  r4_bank,@-r15
    stc.l  r5_bank,@-r15
    stc.l  r6_bank,@-r15    
    stc.l  r7_bank,@-r15
    tst    r7,r7                 /* ������ȯ�����Υ���ƥ����Ȥ�Ƚ��  */  
    bf/s    _interrupt_from_int  /* �㳰/�����ߥϥ�ɥ�ʤ饸����   */
     add    #0x01,r7             /* �����ߤΥͥ��Ȳ���򥤥󥯥����*/
    mov    r15,r1                /* �����å��������ؤ����Υ����å��ݥ�  */
    mov.l  _stacktop_k,r15       /* �󥿤���¸                          */
    mov.l  r1,@-r15            
#ifdef GHS_HOOK
	mov.l   r7,@-r15
	stc     spc,r0
	mov.l   r0,r5
	mov.l   _intevt,r0          /* �㳰�װ��쥸���������              */
	mov.l   _interrupt_hook_dis,r2
	jsr     @r2
	mov.l   @r0,r4              
	mov.l   @r15+,r7
#endif
    mov.l  _intevt,r0            /* �㳰�װ��쥸���������              */
    mov.l  @r0,r4              
    shlr2  r4                    /* 3�ӥåȱ����եȤ����ե��åȤ���� */
    shlr   r4                  
    mov.l  _int_plevel_table_k,r0 
    mov.l  @(r0,r4),r5           /* �����ߤ�ͥ���٤����               */
    mov.l  _int_table_k,r0    
    mov.l  @(r0,r4),r2           /* �����ߥϥ�ɥ�Υ��ɥ쥹�����    */
    ldc    r2,r2_bank            /* �Х�0���ڤ��ؤ��뤿�ᥳ�ԡ�       */    
    ldc    r5,sr                 /* ����ʹ߳����ߤ�����դ��롣      */
    jsr    @r2                   /* �����ߥϥ�ɥ��                  */
     nop
    mov.l  _mask_md_ipm_ret,r0
    ldc    r0,sr                 /* �����߶ػ�                           */
    stc    r7_bank,r7            /* �㳰/�����ߤΥͥ��Ȳ����ǥ������*/
    dt     r7
    ldc    r7,r7_bank
    mov.l  _reqflg_k,r4          /* reqflg�Υ����å�                       */
    mov.l  @r4,r1
    tst    r1,r1                 /* reqflg��FALSE�ʤ�ret_to_task_int������ */
    bt/s   _ret_to_task_int      /* �����������                           */
     mov.l  @r15,r15             /* ����褬�������ʤΤǥ����å����᤹     */
    xor    r0,r0
    bra    ret_int
     mov.l  r0,@r4               /* reqflg�򥯥ꥢ                   */
_interrupt_from_int:
#ifdef GHS_HOOK
	mov.l   r7,@-r15
	stc     spc,r0
	mov.l   r0,r5
	mov.l   _intevt,r0             /* �㳰�װ�INTEVT2�쥸���������    */
	mov.l   _interrupt_hook_dis,r2
	jsr     @r2
	mov.l   @r0,r4
	mov.l   @r15+,r7
#endif
    mov.l   _intevt,r0           /* �㳰�װ��쥸���������           */
    mov.l   @r0,r4
    shlr2   r4                   /* ���ե��åȤ����               */
    shlr    r4                   /* ���ե��åȤ����               */    
    mov.l   _int_plevel_table_k,r0 
    mov.l   @(r0,r4),r5            /* �����ߤ�ͥ���٤����           */
    mov.l   _int_table_k,r0        
    mov.l   @(r0,r4),r2            /* �����ߥϥ�ɥ�Υ��ɥ쥹����� */
    ldc     r2,r2_bank             /* �Х�0���ڤ��ؤ��뤿�ᥳ�ԡ�    */
    ldc     r5,sr                  /* ����ʹ߳����ߤ�����դ��롣   */
    jsr     @r2                    /* �����ߥϥ�ɥ��               */ 
     nop
    mov.l  _mask_md_ipm_ret,r0
    ldc    r0,sr                   /* �����߶ػ�                     */
    stc    r7_bank,r7              /* �㳰/�����ߤΥͥ��Ȳ����ǥ������*/
    dt     r7
    ldc    r7,r7_bank
_ret_to_task_int:
    mov.l  @r15+,r7                /* �����߸������                 */
    mov.l  @r15+,r6
    mov.l  @r15+,r5
    mov.l  @r15+,r4
    mov.l  @r15+,r3
    mov.l  @r15+,r2
    mov.l  @r15+,r1
    mov.l  _mask_md_bl_int,r0 
    ldc    r0,sr                   /* BL ��1��                         */
    mov.l  @r15+,r0
    lds.l  @r15+,mach                
    lds.l  @r15+,macl        
    ldc.l  @r15+,gbr                       
    ldc.l  @r15+,ssr
    lds.l  @r15+,pr
    ldc.l  @r15+,spc
    rte
     nop
#ifdef GHS_STUB
_rom_interrupt:
	stc.l	ssr,@-r15
	stc.l	spc,@-r15
	mov.l	r0,@-r15
	mov.l	r1,@-r15
	mov.l	L._CpuException6,r1
	jmp	@r1
	mov	#6,r0

	.align	4
L._CpuException6:
	.data.l	_CpuExceptionHandler
#endif
    /*
     * ̤��Ͽ�γ���ߤ�ȯ������ȡ����λ��� INTEVT,INTEVT2,SR,PC ��
     * ������cpu_interrupt()��ƤӽФ���
     */   
    .global _no_reg_interrupt
_no_reg_interrupt:
    mov.l _intevt1,r0
    mov.l @r0,r4
#if defined(SH7709) || defined(SH7709A) || defined(SH7729R) || defined(SH7727) 
    mov.l _intevt2,r0    
    mov.l @r0,r5
#endif 
    stc   spc,r6
    stc   ssr,r7
    mov.l _cpu_interrupt_k,r1
    jsr   @r1
     nop
    .align  4
_stacktop_k:
    .long  STACKTOP            /* �󥿥�����Ω���Υ����å��ν����  */
_intevt:        
#if defined(SH7708) || defined(SH7750)    
    .long  INTEVT
#else
    .long  INTEVT2
#endif    
_int_table_k:    
    .long    _int_table       
_int_plevel_table_k:    
    .long    _int_plevel_table 
_reqflg_k:
    .long  _reqflg
_mask_md_ipm_ret:    
    .long  0x40000000 + MAX_IPM << 4
_mask_md_bl_int:    
    .long  0x50000000
_intevt1:        
    .long  INTEVT
#if defined(SH7709) || defined(SH7709A) || defined(SH7729R) || defined(SH7727) 
_intevt2:        
    .long  INTEVT2
#endif 
_cpu_interrupt_k:
    .long  _cpu_interrupt   
#ifdef GHS_HOOK
_interrupt_hook_dis:
	.long   _exception_hook
#endif

   
/*
 *  ���������Ԥ�
 */
	.globl _sil_dly_nse
_sil_dly_nse:
    mov.l  _sil_dly_tim1, r1
    add    r1, r4
    cmp/pl r4
    bt     _sil_dly_nse1
    rts
     nop
_sil_dly_nse1:            
    mov.l  _sil_dly_tim2, r1
    add    r1, r4
    cmp/pl r4
    bt     _sil_dly_nse1
    rts
     nop   
    .align 4
_sil_dly_tim1:
    .long  -SIL_DLY_TIM1    
_sil_dly_tim2:                         
    .long  -SIL_DLY_TIM2