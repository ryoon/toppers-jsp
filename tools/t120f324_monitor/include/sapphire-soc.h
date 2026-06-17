/*
 *  TOPPERS BASE PLATFORM DRIVER
 * 
 *  Copyright (C) 2017-2022 by TOPPERS PROJECT
 *                             Educational Working Group.
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: sapphire-soc.h 2246 2023-01-18 18:45:10Z roi $
 */

#ifndef _SAPPHIRE_SOC_H_
#define _SAPPHIRE_SOC_H_

#include <sil.h>
#include "soc.h"

#define NUM_GPIO            32

/*
 *  MACHINE EXCEPTION NUMBER
 */
#define EXC_INSTRUCTION_ADDRESS_MISALIGNED  0
#define EXC_INSTRUCTION_ADDRESS_FAULT       1
#define EXC_ILLEGAL_INSTRUCTION             2
#define EXC_BREAKPOINT                      3
#define EXC_LOAD_ADDRESS_MISALIGNED         4
#define EXC_LOAD_ADDRESS_FAULT              5
#define EXC_STORE_AMO_ADDRESS_MISALIGNED    6
#define EXC_STORE_AMO_ACCESS_FAUT           7
#define EXC_ENVIRONMENT_CALL_FROM_MMODE     11

/*
 *  MACHINE INTERRUPT NUMBER
 */
#define IRQ_MACHINE_SOFTWARE                3
#define IRQ_MACHIE_TIMER                    7
#define IRQ_MACHINE_EXTERNAL                11

/*
 *  GLOBAL INTERRUPT NUMBER
 */
#define IRQ_VECTOR_CORE0        SYSTEM_PLIC_SYSTEM_CORES_0_EXTERNAL_INTERRUPT
#define IRQ_VECTOR_UART0        SYSTEM_PLIC_SYSTEM_UART_0_IO_INTERRUPT
#define IRQ_VECTOR_SPI0         SYSTEM_PLIC_SYSTEM_SPI_0_IO_INTERRUPT
#define IRQ_VECTOR_SPI1         SYSTEM_PLIC_SYSTEM_SPI_1_IO_INTERRUPT
#define IRQ_VECTOR_I2C0         SYSTEM_PLIC_SYSTEM_I2C_0_IO_INTERRUPT
#define IRQ_VECTOR_GPIO00       SYSTEM_PLIC_SYSTEM_GPIO_0_IO_INTERRUPTS_0
#define IRQ_VECTOR_GPIO01       SYSTEM_PLIC_SYSTEM_GPIO_0_IO_INTERRUPTS_1


/*
 *  CLINT
 */
#define TADR_CLIC_BASE      SYSTEM_CLINT_CTRL
#define TOFF_CLINT_MSIP     0x0000
#define CLINT_MSIP_size     0x4
#define TOFF_CLINT_MTIMECMP 0x4000
#define CLINT_MTIMECMP_size 0x8
#define TOFF_CLINT_MTIME    0xBFF8
#define CLINT_MTIME_size    0x8


/*
 *  PLIC
 */
#define TADR_PLIC_BASE      SYSTEM_PLIC_CTRL
#define TOFF_PLIC_PRIORITY  0x00000000
#define TOFF_PLIC_PENDING   0x00001000
#define TOFF_PLIC_ENABLE    0x00002000
#define NUM_PLIC_ENABLE     128
#define TOFF_PLIC_THRESHOLD 0x00200000
#define NUM_PLIC_THRESHOLD  4096
#define TOFF_PLIC_CLAIM     0x00200004


/*
 *  GPIO
 */
#define TADR_GPIO0_BASE     SYSTEM_GPIO_0_IO_CTRL
#define TOFF_GPIO_INPUT     0x0000
#define TOFF_GPIO_OUTPUT    0x0004
#define TOFF_GPIO_OUTPUT_EN 0x0008
#define TOFF_GPIO_RISE_IE   0x0020
#define TOFF_GPIO_FALL_IE   0x0024
#define TOFF_GPIO_HIGH_IE   0x0028
#define TOFF_GPIO_LOW_IE    0x002C


/*
 *  UART
 */
#define TADR_UART0_BASE     SYSTEM_UART_0_IO_CTRL
#define TOFF_UART_DATA          0x0000
#define TOFF_UART_STATUS        0x0004
  #define UART_TX_EMPTY_INTERRUPT_ENABLE     0x00000001
  #define UART_RX_NOTEMPTY_INTERRUPT_ENABLE  0x00000002
  #define UART_TXFIFO_EMPTY_INTERRUPT        0x00000100
  #define UART_RXFIFO_NOTEMPTY_INTERRUPT     0x00000200
  #define UART_WRITEAVAILABILITY             0x00FF0000
  #define UART_READOCCUPANCY                 0xFF000000
#define TOFF_UART_CLOCK_DIVIDER 0x0008
#define TOFF_UART_FRAME_CONFIG  0x000C
  #define UART_CONFIG_DATALENGTH             0x000000FF
  #define UART_CONFIG_PARITY                 0x00000300
  #define UART_CONFIG_STOPBIT                0x00010000


/*
 *  SPI
 */
#define TADR_SPI0_BASE      SYSTEM_SPI_0_IO_CTRL
#define TADR_SPI1_BASE      SYSTEM_SPI_1_IO_CTRL
#define TOFF_SPI_DATA       0x0000
  #define SPI_SLAVEID               0x0000007F
  #define SPI_SELECT                0x00000080
  #define SPI_CMD_WRITE             0x00000100
  #define SPI_CMD_READ              0x00000200
  #define SPI_CMD_SS                0x00000800
  #define SPI_RSP_VALID             0x80000000
#define TOFF_SPI_BUFFER     0x0004
  #define SPI_CMDAVAILABILITY       0x0000FFFF
  #define SPI_RSPOCCUPANCY          0xFFFF0000
#define TOFF_SPI_CONFIG     0x0008
  #define SPI_MODE_CPOL             0x00000001
  #define SPI_MODE_CPHA             0x00000002
#define TOFF_SPI_INTERRUPT  0x000C
  #define SPI_STATUS_CMD_INT_ENABLE 0x00000001
  #define SPI_STATUS_RSP_INT_ENABLE 0x00000002
  #define SPI_STATUS_CMD_INT_FLAG   0x00000100
  #define SPI_STATUS_RSP_INT_FLAG   0x00000200
#define TOFF_SPI_CLK_DIV    0x0020
#define TOFF_SPI_SS_SETUP   0x0024
#define TOFF_SPI_SS_HOLD    0x0028
#define TOFF_SPI_SS_DISABLE 0x002C


/*
 *  I2C
 */
#define TADR_I2C0_BASE      SYSTEM_I2C_0_IO_CTRL
#define TOFF_I2C_TX_DATA    0x0000
#define TOFF_I2C_TX_ACK     0x0004
#define TOFF_I2C_RX_DATA    0x0008
#define TOFF_I2C_RX_ACK     0x000C
#define TOFF_I2C_INT_ENABLE 0x0020
#define TOFF_I2C_INT_FLAG   0x0024
#define TOFF_I2C_CLOCK_DIV  0x0028
#define TOFF_I2C_TIMEOUT    0x002C
#define TOFF_I2C_TSUDAT     0x0030
#define TOFF_I2C_MASTER_STS 0x0040
#define TOFF_I2C_TLOW       0x0050
#define TOFF_I2C_THIGH      0x0054
#define TOFF_I2C_TBUF       0x0058
#define TOFF_I2C_FILTER_HIT 0x0080
#define TOFF_I2C_FILTER_STS 0x0084
#define TOFF_I2C_FILTER_CFG 0x0088


#endif /* _SAPPHIRE_SOC_H_ */
