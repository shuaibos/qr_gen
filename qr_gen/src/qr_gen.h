/*
 * qr_gen.h
 *
 *  Created on: Aug 26, 2016
 *      Author: Dell
 */

#ifndef QR_GEN_H_
#define QR_GEN_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

/* Constants -----------------------------------------------------------------*/
#define QR_VERSION					1				/**< The QR version. */
#define QR_MATRIX_WIDTH				21				/**< Matrix width - as a result of version 1. */
#define QR_MATRIX_HEIGHT			21				/**< Matrix height - as a result of version 1. */
#define QR_USART_BAUD				57600			/**< The USART baud rate. */

#define QR_LEN_MAX_DATA				15				/**< The maximum number of bytes that can be encoded with version 1 and Q error correction. */
#define QR_LEN_MODE					4				/**< Mode field bit length. */
#define QR_LEN_COUNT				9				/**< Data character count field bit length. */
#define QR_LEN_DATA_PAIR_BITS		11				/**< The number of bits in an encoded data pair. */
#define QR_LEN_DATA_SINGLE_BITS		6				/**< The number of bits in an encoded data pair when there was an odd number of data bytes. */
#define QR_LEN_DATA_PAIR			2				/**< The number of bytes in a pair. */
#define QR_LEN_CODEWORDS			13				/**< The number of codewords. */
#define QR_LEN_DATA_Q_LEVEL_ECC		26				/**< The number of bytes in the final QR code data string for Q level ECC. */
#define QR_LEN_MASK					15				/**< The number of bytes in the mask arrays. */

#define QR_MODE_ALPHANUMERIC		b0010			/**< The mode bits for alphanumeric mode. */
#define QR_DATA_PAIR_FACTOR			45				/**< The factor with which to multiply the first byte in a data pair. */
#define QR_LEN_TOTAL_DATA_BITS_REQ	104				/**< The total number of bits required in the encoded binary data string. */

#define QR_MASK_ROW					8				/**< Mask bits. */
#define QR_MASK_COL					8				/**< Mask bits. */
#define QR_MASK_COL_EXCLUDE			6				/**< Position contains a timing cell. */
#define QR_MASK_ROW_EXCLUDE			6				/**< Position contains a timing cell. */
#define QR_MASK_EXC_RNG_1			9				/**< Position range contains a data bits. */
#define QR_MASK_EXC_RNG_2			12				/**< Position range contains a data bits. */

#define SIZE_CHAR					8
#define TRUE						1
#define FALSE						0
#define ONE							1
#define ZERO						0
#define BIT_0						0x01
#define STR_NULL_CHAR				'\0'
#define ASCII_OFFSET_LET			55
#define ASCII_OFFSET_NUM			48
#define ZERO_PAD_4_SIZE				4
#define EIGHT_BITS					8
#define CR_LF						(const char*)"\r\n"
#define CR_LF_2						(const char*)"\r\n\r\n"

/**
 * \brief QR_GEN return status.
 */
typedef enum {
	QR_GEN_SUCCESS  = 0x01,
	QR_GEN_ERROR  = 0x00
} qr_gen_status_t;

/**
 * \brief Modes enum.
 */
typedef enum {
	NUMERIC	=	0x01,
	ALPHANUMERIC =	0x02,
	BYTE = 0x04,
	ECI = 0x07,
	KANJI = 0x08
} qr_gen_mode_t;

/**
 * \brief HL6528 return status.
 */
typedef enum {
	MASK_0  = 0x00,
	MASK_1  = 0x01,
	MASK_2  = 0x02,
	MASK_3  = 0x03,
	MASK_4  = 0x04,
	MASK_5  = 0x05,
	MASK_6  = 0x06,
	MASK_7  = 0x07
} qr_gen_data_mask_level_q_t;

typedef struct {
	qr_gen_mode_t mode;
	uint8_t data_to_encode[QR_LEN_MAX_DATA];
	uint8_t data_size;
	uint8_t codewords_data[QR_LEN_TOTAL_DATA_BITS_REQ];
	uint8_t codewords[QR_LEN_CODEWORDS];
	uint8_t ecc_msg_poly[QR_LEN_CODEWORDS];
	uint8_t (*qr_matrix)[QR_MATRIX_HEIGHT][QR_MATRIX_WIDTH];
	uint8_t qr_matrix_data[QR_LEN_DATA_Q_LEVEL_ECC * EIGHT_BITS];
} qr_gen_t;

/* Public function prototypes -----------------------------------------------*/
qr_gen_status_t qr_gen_init(qr_gen_t* qr_gen, qr_gen_mode_t mode);
qr_gen_status_t qr_gen_encode_data(qr_gen_t* qr_gen, const char* data);
qr_gen_status_t qr_gen_determine_ecc(qr_gen_t* qr_gen);
qr_gen_status_t qr_gen_create_code(qr_gen_t* qr_gen, qr_gen_data_mask_level_q_t mask_level);

#endif /* QR_GEN_H_ */
