/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  ��L���쌠�҂́C�ȉ��� (1)�`(4) �̏������CFree Software Foundation 
 *  �ɂ���Č��\����Ă��� GNU General Public License �� Version 2 �ɋL
 *  �q����Ă�������𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F�A�i�{�\�t�g�E�F�A
 *  �����ς������̂��܂ށD�ȉ������j���g�p�E�����E���ρE�Ĕz�z�i�ȉ��C
 *  ���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC���̓K�p�\����
 *  �܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F�A�̗��p�ɂ�蒼
 *  �ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C���̐ӔC�𕉂�Ȃ��D
 * 
 *  @(#) $Id$
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include <ezarm7.h>
        




/*
 *
 *  �����݂̏o���������
 *         
 */        
        .text
        .align 4    
        .global IRQ_Handler
IRQ_Handler:

        /* 
         * �����݃��[�h 
         * 
         * cpsr��spsr_irq�ɕ��A�悪r14_irq(lp)�ɓ���C
         * spsr_irq��r14_irq��r13(sp)_irq�� r14,r13�ƂȂ�D
         */

        /* 
         *  �^�X�N�̓��쎞���[�h(�X�[�p�[�o�C�U�[���[�h)��
         */
	mov   sp,#(CPSR_SVC | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 
        msr   cpsr_all, sp
        stmfd sp!, {r0-r3,ip,lr,pc} /* pc�̓_�~�[ */


  
        /*
         * spsr�Ɩ߂�Ԓn���擾���邽�߂�IRQ���[�h��
         */
	mov   r0,#(CPSR_IRQ | CPSR_FIQ_BIT | CPSR_IRQ_BIT)
        msr   cpsr,r0
        sub   r0,lr,#4
        mrs   r1,spsr
   
        /*
         *  �X�[�p�[�o�C�U�[���[�h��
         */
        msr   cpsr_all, sp
        str   r0, [sp,#0x18] /* Store pc */
        stmfd sp!,{r1}       /* spsr */


        /*
         *  ���d���荞�݂�����
         */
        ldr     r2, =interrupt_count
        ldr     r3, [r2]
        add     r0,r3,#1
        str     r0, [r2]
        cmp     r3, #0x00

	moveq   r2,sp            /* �l�X�g���荞�݂łȂ��ꍇ */
        ldreq   sp,=STACKTOP     /* �X�^�b�N�̕ύX    */
        stmeqfd sp!,{r2}         /* �^�X�N�X�^�b�N�̕ۑ� */

	
	/*
         *  �����݃n���h���A�h���X�擾�Ƃ̕ۑ�
         */
	ldr	r2, vic_vect_addr
	ldr	r0, [r2]
	ldr	r2, =handler_address
	str	r0, [r2]
	
	/* 
         * ���荞�݋��� 
         */                
        mov     r2,#(CPSR_SVC|CPSR_FIQ_BIT)
        msr     cpsr,r2

        /* 
         * Call Handler  
         */
	mov     lr, pc
        mov     pc, r0

        /* 
         * ���荞�݋֎~ 
         */
        mov     r2,#(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT) 
        msr     cpsr,r2

	/*
         *  EOI���s
         */
	ldr	r2, =handler_address
	ldr	r0, [r2]
	ldr	r2, vic_vect_addr
	str	r0, [r2]

	/* 
         * �����݃l�X�g��(interrupt_count) �f�N�������g 
         */
        ldr     r2, =interrupt_count
        ldr     r1, [r2]        
        sub     r3, r1, #1                        
        str     r3, [r2]
        cmp     r3, #0x00        
        bne     return_to_task_irq
        
        /* 
         * �^�X�N�X�^�b�N�̕��A 
         */
        ldmfd   sp!,{r0}         
        mov     sp, r0        
                
        ldr     r1, =reqflg    /* Check reqflg */
        ldr     r0, [r1]
        cmp     r0, #0
        beq     return_to_task_irq
        mov     r0, #0
        str     r0, [r1]       /* Clear reqflg */
        b       ret_int

return_to_task_irq:
        /*
         *  ���A����
         *  ���荞�݋��ƂȂ邪�^�X�N�R���e�L�X�g��ɕۑ����Ă��邽�߁C
         *  ���͂Ȃ�
         */
        ldmfd   sp!,{r1}           /* CPSR�̕��A���� */
        msr     spsr, r1           /* ���荞�݋���   */
        ldmfd   sp!,{r0-r3,ip,lr,pc}^ /*�^�X�N���A + �����݋��� */

        /*
         *  ����`�̊����݂��������Ƃ��ɌĂяo��
         */    
undefined_interrupt:
         b      undef_interrupt
    
vic_int_enable:
	.long	0xfffff010
vic_inten_clear:
	.long	0xfffff014
vic_vect_addr:
	.long	0xfffff030
vic_vect_def_addr:
	.long	0xfffff034
vic_vect_addr_15:
	.long	0xfffff13c

	.section .bss
handler_address:
	.long	0