/*
 * lpc2388.h
 *
 *  Created on: 2009/08/31
 *      Author: suikan
 */

#ifndef LPC2388_H_
#define LPC2388_H_

	/* Chapter 2 : Memory re-mapping */
#define MEMMAP *(volatile unsigned int *) 0xE01FC040

	/* Chapter 3 : System Control */
#define EXTINT *(volatile unsigned int *) 0xE01FC140
#define EXTMODE *(volatile unsigned int *) 0xE01FC148
#define EXTPOLAR *(volatile unsigned int *) 0xE01FC14C
#define RSID *(volatile unsigned int *) 0xE01FC180
#define AHBCFG1 *(volatile unsigned int *) 0xE01FC188
#define AHBCFG2 *(volatile unsigned int *) 0xE01FC18C
#define SCS *(volatile unsigned int*) 0xE01FC1A0

	/* Chapter 4 : Configuration */
#define CLKSRCSEL	*(volatile unsigned int *) 0xE01FC10C
#define PLLCON		*(volatile unsigned int *) 0xE01FC080
#define PLLCFG		*(volatile unsigned int *) 0xE01FC084
#define PLLSTAT	*(volatile unsigned int *) 0xE01FC088
#define PLLFEED	*(volatile unsigned int *) 0xE01FC08C
#define CCLKCFG	*(volatile unsigned int *) 0xE01FC104
#define USBCLKCFG	*(volatile unsigned int *) 0xE01FC108
#define IRCTRIM	*(volatile unsigned int *) 0xE01FC1A4
#define PCLKSEL0	*(volatile unsigned int *) 0xE01FC1A8
#define PCLKSEL1	*(volatile unsigned int *) 0xE01FC1AC
#define PCON		*(volatile unsigned int *) 0xE01FC0C0
#define INTWAKE	*(volatile unsigned int *) 0xE01FC144
#define PCONP		*(volatile unsigned int *) 0xE01FC0C4

	/* Chapter 7 : Memory Acceleration Module */
#define MAMCR	*(volatile unsigned int *) 0xE01FC000
#define MAMTIM	*(volatile unsigned int *) 0xE01FC004

 /* Chapter 9 : Pin Connect block*/

#define PINSEL0 *(volatile unsigned int*) 0xE002c000
#define PINSEL1 *(volatile unsigned int*) 0xE002c004
#define PINSEL2 *(volatile unsigned int*) 0xE002c008
#define PINSEL3 *(volatile unsigned int*) 0xE002c00C
#define PINSEL4 *(volatile unsigned int*) 0xE002c010
#define PINSEL5 *(volatile unsigned int*) 0xE002c014
#define PINSEL6 *(volatile unsigned int*) 0xE002c018
#define PINSEL7 *(volatile unsigned int*) 0xE002c01C
#define PINSEL8 *(volatile unsigned int*) 0xE002c020
#define PINSEL9 *(volatile unsigned int*) 0xE002c024
#define PINSEL10 *(volatile unsigned int*) 0xE002c028


#define PINMODE0 *(volatile unsigned int *) 0xE002c040
#define PINMODE1 *(volatile unsigned int *) 0xE002c044
#define PINMODE2 *(volatile unsigned int *) 0xE002c048
#define PINMODE3 *(volatile unsigned int *) 0xE002c04C
#define PINMODE4 *(volatile unsigned int *) 0xE002c050
#define PINMODE5 *(volatile unsigned int *) 0xE002c054
#define PINMODE6 *(volatile unsigned int *) 0xE002c058
#define PINMODE7 *(volatile unsigned int *) 0xE002c05C
#define PINMODE8 *(volatile unsigned int *) 0xE002c060
#define PINMODE9 *(volatile unsigned int *) 0xE002c064

 /* Chapter 10 : FAST IO */
#define FIO0DIR *(volatile unsigned int*) 0x3FFFC000
#define FIO1DIR *(volatile unsigned int*) 0x3FFFC020
#define FIO2DIR *(volatile unsigned int*) 0x3FFFC040
#define FIO3DIR *(volatile unsigned int*) 0x3FFFC060
#define FIO4DIR *(volatile unsigned int*) 0x3FFFC080

#define FIO0MASK *(volatile unsigned int*) 0x3FFFC010
#define FIO1MASK *(volatile unsigned int*) 0x3FFFC030
#define FIO2MASK *(volatile unsigned int*) 0x3FFFC050
#define FIO3MASK *(volatile unsigned int*) 0x3FFFC070
#define FIO4MASK *(volatile unsigned int*) 0x3FFFC090

#define FIO0PIN *(volatile unsigned int*) 0x3FFFC014
#define FIO1PIN *(volatile unsigned int*) 0x3FFFC034
#define FIO2PIN *(volatile unsigned int*) 0x3FFFC054
#define FIO3PIN *(volatile unsigned int*) 0x3FFFC074
#define FIO4PIN *(volatile unsigned int*) 0x3FFFC094

#define FIO0SET *(volatile unsigned int*) 0x3FFFC018
#define FIO1SET *(volatile unsigned int*) 0x3FFFC038
#define FIO2SET *(volatile unsigned int*) 0x3FFFC058
#define FIO3SET *(volatile unsigned int*) 0x3FFFC078
#define FIO4SET *(volatile unsigned int*) 0x3FFFC098

#define FIO0CLR *(volatile unsigned int*) 0x3FFFC01C
#define FIO1CLR *(volatile unsigned int*) 0x3FFFC03C
#define FIO2CLR *(volatile unsigned int*) 0x3FFFC05C
#define FIO3CLR *(volatile unsigned int*) 0x3FFFC07C
#define FIO4CLR *(volatile unsigned int*) 0x3FFFC09C

 /* Chapter 16 : UART */
#define U0RBR *( volatile unsigned char *)0xE000C000
#define U2RBR *( volatile unsigned char *)0xE0078000
#define U3RBR *( volatile unsigned char *)0xE007C000

#define U0THR U0RBR
#define U2THR U2RBR
#define U3THR U3RBR

#define U0DLL U0RBR
#define U2DLL U2RBR
#define U3DLL U3RBR

#define U0IER *( volatile unsigned char *)0xE000C004
#define U2IER *( volatile unsigned char *)0xE0078004
#define U3IER *( volatile unsigned char *)0xE007C004

#define U0DLM U0IER
#define U2DLM U2IER
#define U3DLM U3IER

#define U0IIR *( volatile unsigned char *)0xE000C008
#define U2IIR *( volatile unsigned char *)0xE0078008
#define U3IIR *( volatile unsigned char *)0xE007C008

#define U0FCR *( volatile unsigned char *)0xE000C008
#define U2FCR *( volatile unsigned char *)0xE0078008
#define U3FCR *( volatile unsigned char *)0xE007C008

#define U0LCR *( volatile unsigned char *)0xE000C00C
#define U2LCR *( volatile unsigned char *)0xE007800C
#define U3LCR *( volatile unsigned char *)0xE007C00C

#define U0LSR *( volatile unsigned char *)0xE000C014
#define U2LSR *( volatile unsigned char *)0xE0078014
#define U3LSR *( volatile unsigned char *)0xE007C014

#define U0SCR *( volatile unsigned char *)0xE000C01C
#define U2SCR *( volatile unsigned char *)0xE007801C
#define U3SCR *( volatile unsigned char *)0xE007C01C

#define U0ACR *( volatile unsigned char *)0xE000C020
#define U2ACR *( volatile unsigned char *)0xE0078020
#define U3ACR *( volatile unsigned char *)0xE007C020

#define U3ICR *( volatile unsigned char *)0xE000C024

#define U0FDR *( volatile unsigned char *)0xE000C028
#define U2FDR *( volatile unsigned char *)0xE0078028
#define U3FDR *( volatile unsigned char *)0xE007C028

#define U0TER *( volatile unsigned char *)0xE000C030
#define U2TER *( volatile unsigned char *)0xE0078030
#define U3TER *( volatile unsigned char *)0xE007C030

/* Chapter 23 */
#define T0IR *(volatile unsigned int *) 0xE0004000
#define T1IR *(volatile unsigned int *) 0xE0008000
#define T2IR *(volatile unsigned int *) 0xE0070000
#define T3IR *(volatile unsigned int *) 0xE0074000
#define T0TCR *(volatile unsigned int *) 0xE0004004
#define T1TCR *(volatile unsigned int *) 0xE0008004
#define T2TCR *(volatile unsigned int *) 0xE0070004
#define T3TCR *(volatile unsigned int *) 0xE0074004
#define T0TC *(volatile unsigned int *) 0xE0004008
#define T1TC *(volatile unsigned int *) 0xE0008008
#define T2TC *(volatile unsigned int *) 0xE0070008
#define T3TC *(volatile unsigned int *) 0xE0074008
#define T0PR *(volatile unsigned int *) 0xE000400C
#define T1PR *(volatile unsigned int *) 0xE000800C
#define T2PR *(volatile unsigned int *) 0xE007000C
#define T3PR *(volatile unsigned int *) 0xE007400C
#define T0PC *(volatile unsigned int *) 0xE0004010
#define T1PC *(volatile unsigned int *) 0xE0008010
#define T2PC *(volatile unsigned int *) 0xE0070010
#define T3PC *(volatile unsigned int *) 0xE0074010
#define T0MCR *(volatile unsigned int *) 0xE0004014
#define T1MCR *(volatile unsigned int *) 0xE0008014
#define T2MCR *(volatile unsigned int *) 0xE0070014
#define T3MCR *(volatile unsigned int *) 0xE0074014
#define T0MR0 *(volatile unsigned int *) 0xE0004018
#define T1MR0 *(volatile unsigned int *) 0xE0008018
#define T2MR0 *(volatile unsigned int *) 0xE0070018
#define T3MR0 *(volatile unsigned int *) 0xE0074018
#define T0MR1 *(volatile unsigned int *) 0xE000401C
#define T1MR1 *(volatile unsigned int *) 0xE000801C
#define T2MR1 *(volatile unsigned int *) 0xE007001C
#define T3MR1 *(volatile unsigned int *) 0xE007401C
#define T0MR2 *(volatile unsigned int *) 0xE0004020
#define T1MR2 *(volatile unsigned int *) 0xE0008020
#define T2MR2 *(volatile unsigned int *) 0xE0070020
#define T3MR2 *(volatile unsigned int *) 0xE0074020
#define T0MR3 *(volatile unsigned int *) 0xE0004024
#define T1MR3 *(volatile unsigned int *) 0xE0008024
#define T2MR3 *(volatile unsigned int *) 0xE0070024
#define T3MR3 *(volatile unsigned int *) 0xE0074024
#define T0CR0 *(volatile unsigned int *) 0xE000402C
#define T1CR0 *(volatile unsigned int *) 0xE000802C
#define T2CR0 *(volatile unsigned int *) 0xE007002C
#define T3CR0 *(volatile unsigned int *) 0xE007402C
#define T0CR1 *(volatile unsigned int *) 0xE0004030
#define T1CR1 *(volatile unsigned int *) 0xE0008030
#define T2CR1 *(volatile unsigned int *) 0xE0070030
#define T3CR1 *(volatile unsigned int *) 0xE0074030
#define T0EMR *(volatile unsigned int *) 0xE000403C
#define T1EMR *(volatile unsigned int *) 0xE000803C
#define T2EMR *(volatile unsigned int *) 0xE007003C
#define T3EMR *(volatile unsigned int *) 0xE007403C
#define T0CTCR *(volatile unsigned int *) 0xE0004070
#define T1CTCR *(volatile unsigned int *) 0xE0008070
#define T2CTCR *(volatile unsigned int *) 0xE0070070
#define T3CTCR *(volatile unsigned int *) 0xE0074070
#define T0CR0 *(volatile unsigned int *) 0xE000402C
#define T1CR0 *(volatile unsigned int *) 0xE000802C
#define T2CR0 *(volatile unsigned int *) 0xE007002C
#define T3CR0 *(volatile unsigned int *) 0xE007402C
#define T0CR1 *(volatile unsigned int *) 0xE0004030
#define T1CR1 *(volatile unsigned int *) 0xE0008030
#define T2CR1 *(volatile unsigned int *) 0xE0070030
#define T3CR1 *(volatile unsigned int *) 0xE0074030
#define T0EMR *(volatile unsigned int *) 0xE000403C
#define T1EMR *(volatile unsigned int *) 0xE000803C
#define T2EMR *(volatile unsigned int *) 0xE007003C
#define T3EMR *(volatile unsigned int *) 0xE007403C
#define T0CTCR *(volatile unsigned int *) 0xE0004070
#define T1CTCR *(volatile unsigned int *) 0xE0008070
#define T2CTCR *(volatile unsigned int *) 0xE0070070
#define T3CTCR *(volatile unsigned int *) 0xE0074070

#define VICIRQStatus *(volatile unsigned int *)0xFFFFF000
#define VICFIQStatus *(volatile unsigned int *)0xFFFFF004
#define VICRawIntr *(volatile unsigned int *)0xFFFFF008

#define VICIntSelect *(volatile unsigned int *)0xFFFFF00C

#define VICIntEnable *(volatile unsigned int *)0xFFFFF010
#define VICIntEnClr *(volatile unsigned int *)0xFFFFF014

#define VICSoftInt *(volatile unsigned int *)0xFFFFF018
#define VICSoftIntClear *(volatile unsigned int *)0xFFFFF01C

#define VICProtection *(volatile unsigned int *)0xFFFFF020

#define VICSWPriorityMask *(volatile unsigned int *)0xFFFFF024


#define VICVectAddr0 *(volatile void (**)())0xFFFFF100
#define VICVectAddr1 *(volatile void (**)())0xFFFFF104
#define VICVectAddr2 *(volatile void (**)())0xFFFFF108
#define VICVectAddr3 *(volatile void (**)())0xFFFFF10C
#define VICVectAddr4 *(volatile void (**)())0xFFFFF110
#define VICVectAddr5 *(volatile void (**)())0xFFFFF114
#define VICVectAddr6 *(volatile void (**)())0xFFFFF118
#define VICVectAddr7 *(volatile void (**)())0xFFFFF11C
#define VICVectAddr8 *(volatile void (**)())0xFFFFF120
#define VICVectAddr9 *(volatile void (**)())0xFFFFF124
#define VICVectAddr10 *(volatile void (**)())0xFFFFF128
#define VICVectAddr11 *(volatile void (**)())0xFFFFF12C
#define VICVectAddr12 *(volatile void (**)())0xFFFFF130
#define VICVectAddr13 *(volatile void (**)())0xFFFFF134
#define VICVectAddr14 *(volatile void (**)())0xFFFFF138
#define VICVectAddr15 *(volatile void (**)())0xFFFFF13C
#define VICVectAddr16 *(volatile void (**)())0xFFFFF140
#define VICVectAddr17 *(volatile void (**)())0xFFFFF144
#define VICVectAddr18 *(volatile void (**)())0xFFFFF148
#define VICVectAddr19 *(volatile void (**)())0xFFFFF14C
#define VICVectAddr20 *(volatile void (**)())0xFFFFF150
#define VICVectAddr21 *(volatile void (**)())0xFFFFF154
#define VICVectAddr22 *(volatile void (**)())0xFFFFF158
#define VICVectAddr23 *(volatile void (**)())0xFFFFF15C
#define VICVectAddr24 *(volatile void (**)())0xFFFFF160
#define VICVectAddr25 *(volatile void (**)())0xFFFFF164
#define VICVectAddr26 *(volatile void (**)())0xFFFFF168
#define VICVectAddr27 *(volatile void (**)())0xFFFFF16C
#define VICVectAddr28 *(volatile void (**)())0xFFFFF170
#define VICVectAddr29 *(volatile void (**)())0xFFFFF174
#define VICVectAddr30 *(volatile void (**)())0xFFFFF178
#define VICVectAddr31 *(volatile void (**)())0xFFFFF17C

#define VICVectPriority0 *(volatile unsigned int *)0xFFFFF200
#define VICVectPriority1 *(volatile unsigned int *)0xFFFFF204
#define VICVectPriority2 *(volatile unsigned int *)0xFFFFF208
#define VICVectPriority3 *(volatile unsigned int *)0xFFFFF20C
#define VICVectPriority4 *(volatile unsigned int *)0xFFFFF210
#define VICVectPriority5 *(volatile unsigned int *)0xFFFFF214
#define VICVectPriority6 *(volatile unsigned int *)0xFFFFF218
#define VICVectPriority7 *(volatile unsigned int *)0xFFFFF21C
#define VICVectPriority8 *(volatile unsigned int *)0xFFFFF220
#define VICVectPriority9 *(volatile unsigned int *)0xFFFFF224
#define VICVectPriority10 *(volatile unsigned int *)0xFFFFF228
#define VICVectPriority11 *(volatile unsigned int *)0xFFFFF22C
#define VICVectPriority12 *(volatile unsigned int *)0xFFFFF230
#define VICVectPriority13 *(volatile unsigned int *)0xFFFFF234
#define VICVectPriority14 *(volatile unsigned int *)0xFFFFF238
#define VICVectPriority15 *(volatile unsigned int *)0xFFFFF23C
#define VICVectPriority16 *(volatile unsigned int *)0xFFFFF240
#define VICVectPriority17 *(volatile unsigned int *)0xFFFFF244
#define VICVectPriority18 *(volatile unsigned int *)0xFFFFF248
#define VICVectPriority19 *(volatile unsigned int *)0xFFFFF24C
#define VICVectPriority20 *(volatile unsigned int *)0xFFFFF250
#define VICVectPriority21 *(volatile unsigned int *)0xFFFFF254
#define VICVectPriority22 *(volatile unsigned int *)0xFFFFF258
#define VICVectPriority23 *(volatile unsigned int *)0xFFFFF25C
#define VICVectPriority24 *(volatile unsigned int *)0xFFFFF260
#define VICVectPriority25 *(volatile unsigned int *)0xFFFFF264
#define VICVectPriority26 *(volatile unsigned int *)0xFFFFF268
#define VICVectPriority27 *(volatile unsigned int *)0xFFFFF26C
#define VICVectPriority28 *(volatile unsigned int *)0xFFFFF270
#define VICVectPriority29 *(volatile unsigned int *)0xFFFFF274
#define VICVectPriority30 *(volatile unsigned int *)0xFFFFF278
#define VICVectPriority31 *(volatile unsigned int *)0xFFFFF27C

#define VICAddress *(volatile void (**)())0xFFFFFF00



#endif /* LPC2388_H_ */
