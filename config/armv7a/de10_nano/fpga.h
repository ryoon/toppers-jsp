/*
 *  TOPPERS BASE PLATFORM DRIVER
 * 
 *  Copyright (C) 2017-2017 by TOPPERS PROJECT
 *                             Educational Working Group.
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  $Id: fpga.h 2416 2017-04-18 17:45:03Z roi $
 */

/*
 *	FPGA設定のヘッダファイル
 */

#ifndef _FPGA_H_
#define _FPGA_H_

#define FPGA_BASE_ADDRESS       0xFF200000

#ifndef LED_PIO_OFFSET
#define LED_PIO_OFFSET          0x00010040
#endif
#ifndef DSW_PIO_OFFSET
#define DSW_PIO_OFFSET          0x00010080
#endif
#ifndef PSW_PIO_OFFSET
#define PSW_PIO_OFFSET          0x000100C0
#endif
#ifndef PIO0_OFFSET
#define PIO0_OFFSET             0x000100E0
#endif
#ifndef UART_OFFSET
#define UART_OFFSET             0x00010100
#endif
#ifndef SPI0_OFFSET
#define SPI0_OFFSET             0x00010180
#endif
#ifndef  I2C0_OFFSET
#define I2C0_OFFSET             0x000101A0
#endif
#ifndef ADC_OFFSET
#define ADC_OFFSET              0x00010200
#endif


#define TADR_LED_PIO_BASE       (FPGA_BASE_ADDRESS+LED_PIO_OFFSET)
#define TADR_DSW_PIO_BASE       (FPGA_BASE_ADDRESS+DSW_PIO_OFFSET)
#define TADR_PSW_PIO_BASE       (FPGA_BASE_ADDRESS+PSW_PIO_OFFSET)

#define TADR_PIO0_BASE          (FPGA_BASE_ADDRESS+PIO0_OFFSET)
#define TOFF_PIO_DATA           0x0000
#define TOFF_PIO_DIRECTION      0x0004
#define TOFF_PIO_IRQ_MASK       0x0008
#define TOFF_PIO_EDGE_CAP       0x000C
#define TOFF_PIO_SET_BITS       0x0010
#define TOFF_PIO_CLEAR_BITS     0x0014

/*
 *  FPGAUART-IP DEFINTIONS
 */
#define TADR_FPGAUART_BASE      (FPGA_BASE_ADDRESS+UART_OFFSET)
#define TOFF_FPGAUART_RXDATA    0x0000
#define TOFF_FPGAUART_TXDATA    0x0004
#define TOFF_FPGAUART_STATUS    0x0008
  #define AVALON_UART_STATUS_PE            0x00000001
  #define AVALON_UART_STATUS_FE            0x00000002
  #define AVALON_UART_STATUS_BRK           0x00000004
  #define AVALON_UART_STATUS_ROE           0x00000008
  #define AVALON_UART_STATUS_TOE           0x00000010
  #define AVALON_UART_STATUS_TMT           0x00000020
  #define AVALON_UART_STATUS_TRDY          0x00000040
  #define AVALON_UART_STATUS_RRDY          0x00000080
  #define AVALON_UART_STATUS_E             0x00000100
  #define AVALON_UART_STATUS_DCTS          0x00000400
  #define AVALON_UART_STATUS_CTS           0x00000800
  #define AVALON_UART_STATUS_EOP           0x00001000
#define TOFF_FPGAUART_CONTROL   0x000C
  #define AVALON_UART_CONTROL_PE           0x00000001
  #define AVALON_UART_CONTROL_FE           0x00000002
  #define AVALON_UART_CONTROL_BRK          0x00000004
  #define AVALON_UART_CONTROL_ROE          0x00000008
  #define AVALON_UART_CONTROL_TOE          0x00000010
  #define AVALON_UART_CONTROL_TMT          0x00000020
  #define AVALON_UART_CONTROL_TRDY         0x00000040
  #define AVALON_UART_CONTROL_RRDY         0x00000080
  #define AVALON_UART_CONTROL_E            0x00000100
  #define AVALON_UART_CONTROL_DCTS         0x00000400
  #define AVALON_UART_CONTROL_RTS          0x00000800
  #define AVALON_UART_CONTROL_EOP          0x00001000
#define TOFF_FPGAUART_DIVISOR   0x0010
#define TOFF_FPGAUART_EOP       0x0014

/*
 *  FPGASPI-IP DEFINTIONS
 */
#define TADR_FPGASPI_BASE       (FPGA_BASE_ADDRESS+SPI0_OFFSET)
#define TOFF_SPI_RXDATA_REG     0x0000
#define TOFF_SPI_TXDATA_REG     0x0004
#define TOFF_SPI_STATUS_REG     0x0008
  #define AVALON_SPI_STATUS_ROE            0x00000008
  #define AVALON_SPI_STATUS_TOE            0x00000010
  #define AVALON_SPI_STATUS_TMT            0x00000020
  #define AVALON_SPI_STATUS_TRDY           0x00000040
  #define AVALON_SPI_STATUS_RRDY           0x00000080
  #define AVALON_SPI_STATUS_E              0x00000100
#define TOFF_SPI_CONTROL_REG    0x000C
  #define AVALON_SPI_CONTROL_IROE          0x00000008
  #define AVALON_SPI_CONTROL_ITOE          0x00000010
  #define AVALON_SPI_CONTROL_ITRDY         0x00000040
  #define AVALON_SPI_CONTROL_IRRDY         0x00000080
  #define AVALON_SPI_CONTROL_IE            0x00000100
  #define AVALON_SPI_CONTROL_SSO           0x00000400
#define TOFF_SPI_SLAVE_SEL_REG  0x0014

/*
 *  FPGAI2C-IP DEFINTIONS
 */
#define TADR_FPGAI2C_BASE       (FPGA_BASE_ADDRESS+I2C0_OFFSET)
#define TOFF_I2C_SCL_DATA_REG   0x0000
#define TOFF_I2C_SDA_DATA_REG   0x0020
#define TOFF_I2C_SDA_DIR_REG    0x0024

/*
 *  FPGAADC-IP DEFINTIONS
 */
#define TADR_FPGAADC_BASE       (FPGA_BASE_ADDRESS+ADC_OFFSET)
#define TOFF_ADC_CH0_REG                   0x00000000
#define TOFF_ADC_UPDATE                    0x00000000
#define TOFF_ADC_CH1_REG                   0x00000004
#define TOFF_ADC_CH2_REG                   0x00000008
#define TOFF_ADC_CH3_REG                   0x0000000C
#define TOFF_ADC_CH4_REG                   0x00000010
#define TOFF_ADC_CH5_REG                   0x00000014

#endif	/* _FPGA_H_ */

