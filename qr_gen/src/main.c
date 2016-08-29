/*****************************************************************************
*
* \file
*
* \brief USART example application for AVR UC3 USART driver.
*
*
******************************************************************************/

#include <avr32/io.h>
#include "compiler.h"
#include "board.h"
#include "power_clocks_lib.h"
#include "gpio.h"
#include "usart.h"
#include <stdio.h>
#include "qr_gen.h"

/*! \name USART Settings
*/
//! @{

#  define EXAMPLE_TARGET_PBACLK_FREQ_HZ FOSC0  // PBA clock target frequency, in Hz

#  define QR_GEN_USART                 (&AVR32_USART1)
#  define QR_GEN_USART_RX_PIN          AVR32_USART1_RXD_0_0_PIN
#  define QR_GEN_USART_RX_FUNCTION     AVR32_USART1_RXD_0_0_FUNCTION
#  define QR_GEN_USART_TX_PIN          AVR32_USART1_TXD_0_0_PIN
#  define QR_GEN_USART_TX_FUNCTION     AVR32_USART1_TXD_0_0_FUNCTION
#  define QR_GEN_USART_CLOCK_MASK      AVR32_USART1_CLK_PBA
#  define QR_GEN_PDCA_CLOCK_HSB        AVR32_PDCA_CLK_HSB
#  define QR_GEN_PDCA_CLOCK_PB         AVR32_PDCA_CLK_PBA

#if !defined(QR_GEN_USART)             || \
!defined(QR_GEN_USART_RX_PIN)      || \
!defined(QR_GEN_USART_RX_FUNCTION) || \
!defined(QR_GEN_USART_TX_PIN)      || \
!defined(QR_GEN_USART_TX_FUNCTION)
#  error The USART configuration to use in this example is missing.
#endif

//! @}

#if UC3L
/*! \name Parameters to pcl_configure_clocks().
*/
//! @{
static scif_gclk_opt_t gc_dfllif_ref_opt = { SCIF_GCCTRL_SLOWCLOCK, 0, false };
static pcl_freq_param_t pcl_dfll_freq_param =
{
	.main_clk_src = PCL_MC_DFLL0,
	.cpu_f        = EXAMPLE_TARGET_MCUCLK_FREQ_HZ,
	.pba_f        = EXAMPLE_TARGET_PBACLK_FREQ_HZ,
	.pbb_f        = EXAMPLE_TARGET_PBACLK_FREQ_HZ,
	.dfll_f       = EXAMPLE_TARGET_DFLL_FREQ_HZ,
	.pextra_params = &gc_dfllif_ref_opt
};
//! @}
#endif


int main(void)
{
	/** Configure Osc0 in crystal mode (i.e. use of an external crystal source, with frequency FOSC0) with an appropriate startup time then switch the main clock source to Osc0. */
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	/** Map USART pins. */
	static const gpio_map_t USART_GPIO_MAP =
	{
		{QR_GEN_USART_RX_PIN, QR_GEN_USART_RX_FUNCTION},
		{QR_GEN_USART_TX_PIN, QR_GEN_USART_TX_FUNCTION}
	};

	/** USART options. */
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate     = QR_USART_BAUD,
		.charlength   = EIGHT_BITS,
		.paritytype   = USART_NO_PARITY,
		.stopbits     = USART_1_STOPBIT,
		.channelmode  = USART_NORMAL_CHMODE
	};

	/** Assign GPIO to USART. */
	gpio_enable_module(USART_GPIO_MAP,
	sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

	/** Initialize USART in RS232 mode. */
	usart_init_rs232(QR_GEN_USART, &USART_OPTIONS, EXAMPLE_TARGET_PBACLK_FREQ_HZ);
	
	/***********************************
	*  DEFINE DATA TO ENCODE HERE.
	*  ALL CHARS MUST BE UPPERCASE.
	************************************/
	//const char data_to_encode[] = "ZAPPER TEST-SO";
	const char data_to_encode[] = "HELLO WORLD";
	//const char data_to_encode[] = "THIS IS A TEST";
	
	usart_write_line(QR_GEN_USART, "---------------\r\n");
	usart_write_line(QR_GEN_USART, "    qr_gen\r\n");
	usart_write_line(QR_GEN_USART, "---------------\r\n");
	usart_write_line(QR_GEN_USART, "Data to encode: ");
	usart_write_line(QR_GEN_USART, data_to_encode);
	usart_write_line(QR_GEN_USART, CR_LF_2);
	
	/** 
	* - Initialise qr_gen struct. 
	* - Encodes the data.
	* - Determines the ECC words and performs polynomial long division.
	* - Populates the QR matrix in the correct format with the finder patterns and timing bits. The selected mask level is applied to the data bits.
	* */
	static qr_gen_t qr_gen;					/**< The main qr_gen struct. Only a single instance of this struct exists for the duration of the program to ensure that RAM is used efficiently. */
	qr_gen_init(&qr_gen, ALPHANUMERIC);
	if(!qr_gen_encode_data(&qr_gen, data_to_encode))
		return QR_GEN_ERROR;
	if(!qr_gen_determine_ecc(&qr_gen))
		return QR_GEN_ERROR;
	if(!qr_gen_create_code(&qr_gen, MASK_3))
		return QR_GEN_ERROR;
	
	uint8_t row;
	uint8_t col;
	/** Print generated ASCII QR Code. */
	for (row = 0; row < QR_MATRIX_HEIGHT; row++) {
		for (col = 0; col < QR_MATRIX_WIDTH; col++) {
			uint8_t bit = (*qr_gen.qr_matrix)[row][col];
			if (bit) {
				usart_write_line(QR_GEN_USART, "##");
				} else {
				usart_write_line(QR_GEN_USART, "  ");
			}
		}
		usart_write_line(QR_GEN_USART, CR_LF);
	}
	
	/** Print generated ASCII QR Code. */
	usart_write_line(QR_GEN_USART, CR_LF_2);
	for (row = 0; row < QR_MATRIX_HEIGHT; row++) {
		for (col = 0; col < QR_MATRIX_WIDTH; col++) {
			uint8_t bit = (*qr_gen.qr_matrix)[row][col];
			if (bit) {
				usart_write_line(QR_GEN_USART, "1 ");
				} else {
				usart_write_line(QR_GEN_USART, "0 ");
			}
		}
		usart_write_line(QR_GEN_USART, CR_LF);
	}

	//*** Sleep mode
	// This program won't be doing anything else from now on, so it might as well
	// sleep.
	// Modules communicating with external circuits should normally be disabled
	// before entering a sleep mode that will stop the module operation.
	// Make sure the USART dumps the last message completely before turning it off.
	while(!usart_tx_empty(QR_GEN_USART));
	pcl_disable_module(QR_GEN_USART_CLOCK_MASK);

	// Since we're going into a sleep mode deeper than IDLE, all HSB masters must
	// be stopped before entering the sleep mode.
	pcl_disable_module(QR_GEN_PDCA_CLOCK_HSB);
	pcl_disable_module(QR_GEN_PDCA_CLOCK_PB);

	// If there is a chance that any PB write operations are incomplete, the CPU
	// should perform a read operation from any register on the PB bus before
	// executing the sleep instruction.
	AVR32_INTC.ipr[0];  // Dummy read

	// Go to STATIC sleep mode.
	SLEEP(AVR32_PM_SMODE_STATIC);

	while (true);
}
