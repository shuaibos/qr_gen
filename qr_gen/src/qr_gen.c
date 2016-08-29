/*
 * qr_gen.c
 *
 *  Created on: Aug 26, 2016
 *      Author: Dell
 */

/* Includes -----------------------------------------------*/
#include "qr_gen.h"
#include <stdlib.h>

/* Constants -----------------------------------------------*/
static const uint8_t ECC_GLF_256_ANTILOG_EXP_OF_A[] = {0,1,25,2,50,26,198,3,223,51,238,27,104,199,75,4,100,224,14,52,141,239,129,28,193,105,248,200,8,76,113,5,138,101,47,225,36,15,33,53,147,142,218,240,18,130,69,29,181,194,125,106,39,249,185,201,154,9,120,77,228,114,166,6,191,139,98,102,221,48,253,226,152,37,179,16,145,34,136,54,208,148,206,143,150,219,189,241,210,19,92,131,56,70,64,30,66,182,163,195,72,126,110,107,58,40,84,250,133,186,61,202,94,155,159,10,21,121,43,78,212,229,172,115,243,167,87,7,112,192,247,140,128,99,13,103,74,222,237,49,197,254,24,227,165,153,119,38,184,180,124,17,68,146,217,35,32,137,46,55,63,209,91,149,188,207,205,144,135,151,178,220,252,190,97,242,86,211,171,20,42,93,158,132,60,57,83,71,109,65,162,31,45,67,216,183,123,164,118,196,23,73,236,127,12,111,246,108,161,59,82,41,157,85,170,251,96,134,177,187,204,62,90,203,89,95,176,156,169,160,81,11,245,22,235,122,117,44,215,79,174,213,233,230,231,173,232,116,214,244,234,168,80,88,175};
static const uint8_t ECC_GLF_256_LOG_EXP_OF_A[] = {1,2,4,8,16,32,64,128,29,58,116,232,205,135,19,38,76,152,45,90,180,117,234,201,143,3,6,12,24,48,96,192,157,39,78,156,37,74,148,53,106,212,181,119,238,193,159,35,70,140,5,10,20,40,80,160,93,186,105,210,185,111,222,161,95,190,97,194,153,47,94,188,101,202,137,15,30,60,120,240,253,231,211,187,107,214,177,127,254,225,223,163,91,182,113,226,217,175,67,134,17,34,68,136,13,26,52,104,208,189,103,206,129,31,62,124,248,237,199,147,59,118,236,197,151,51,102,204,133,23,46,92,184,109,218,169,79,158,33,66,132,21,42,84,168,77,154,41,82,164,85,170,73,146,57,114,228,213,183,115,230,209,191,99,198,145,63,126,252,229,215,179,123,246,241,255,227,219,171,75,150,49,98,196,149,55,110,220,165,87,174,65,130,25,50,100,200,141,7,14,28,56,112,224,221,167,83,166,81,162,89,178,121,242,249,239,195,155,43,86,172,69,138,9,18,36,72,144,61,122,244,245,247,243,251,235,203,139,11,22,44,88,176,125,250,233,207,131,27,54,108,216,173,71,142,1};
static const uint8_t ECC_GEN_POLY[QR_LEN_CODEWORDS + ONE] = {0, 74, 152, 176, 100, 86, 100, 106, 104, 130, 218, 206, 140, 78};	/**< Generator polynomial for 13 error correction code words - Q level ecc. */
static const uint8_t ZERO_PAD_4[] = { 0, 0, 0, 0};
static const uint8_t SPECIAL_BIT_STRING_ODD[] = { 1, 1, 1, 0, 1, 1, 0, 0};
static const uint8_t SPECIAL_BIT_STRING_EVEN[] = { 0, 0, 0, 1, 0, 0, 0, 1};
static const uint8_t MASK_Q_0[] = {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1};
static const uint8_t MASK_Q_1[] = {0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0};
static const uint8_t MASK_Q_2[] = {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1};
static const uint8_t MASK_Q_3[] = {0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0};
static const uint8_t MASK_Q_4[] = {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0};
static const uint8_t MASK_Q_5[] = {0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1};
static const uint8_t MASK_Q_6[] = {0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0};
static const uint8_t MASK_Q_7[] = {0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1};

static uint8_t QR_FORMAT[] = {
		1,	1,	1,	1,	1,	1,	1,	0,	0,	138,	137,	136,	135,	0,	1,	1,	1,	1,	1,	1,	1,
		1,	0,	0,	0,	0,	0,	1,	0,	0,	140,	139,	134,	133,	0,	1,	0,	0,	0,	0,	0,	1,
		1,	0,	1,	1,	1,	0,	1,	0,	0,	142,	141,	132,	131,	0,	1,	0,	1,	1,	1,	0,	1,
		1,	0,	1,	1,	1,	0,	1,	0,	0,	144,	143,	130,	129,	0,	1,	0,	1,	1,	1,	0,	1,
		1,	0,	1,	1,	1,	0,	1,	0,	0,	146,	145,	128,	127,	0,	1,	0,	1,	1,	1,	0,	1,
		1,	0,	0,	0,	0,	0,	1,	0,	0,	148,	147,	126,	125,	0,	1,	0,	0,	0,	0,	0,	1,
		1,	1,	1,	1,	1,	1,	1,	0,	1,	0,	1,	0,	1,	0,	1,	1,	1,	1,	1,	1,	1,
		0,	0,	0,	0,	0,	0,	0,	0,	0,	150,	149,	124,	123,	0,	0,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	1,	0,	0,	152,	151,	122,	121,	0,	0,	0,	0,	0,	0,	0,	0,
		202,	201,	200,	199,	186,	185,	0,	184,	183,	154,	153,	120,	119,	74,	73,	72,	71,	26,	25,	24,	23,
		204,	203,	198,	197,	188,	187,	1,	182,	181,	156,	155,	118,	117,	76,	75,	70,	69,	28,	27,	22,	21,
		206,	205,	196,	195,	190,	189,	0,	180,	179,	158,	157,	116,	115,	78,	77,	68,	67,	30,	29,	20,	19,
		208,	207,	194,	193,	192,	191,	1,	178,	177,	160,	159,	114,	113,	80,	79,	66,	65,	32,	31,	18,	17,
		0,	0,	0,	0,	0,	0,	0,	0,	1,	162,	161,	112,	111,	82,	81,	64,	63,	34,	33,	16,	15,
		1,	1,	1,	1,	1,	1,	1,	0,	0,	164,	163,	110,	109,	84,	83,	62,	61,	36,	35,	14,	13,
		1,	0,	0,	0,	0,	0,	1,	0,	0,	166,	165,	108,	107,	86,	85,	60,	59,	38,	37,	12,	11,
		1,	0,	1,	1,	1,	0,	1,	0,	0,	168,	167,	106,	105,	88,	87,	58,	57,	40,	39,	10,	9,
		1,	0,	1,	1,	1,	0,	1,	0,	0,	170,	169,	104,	103,	90,	89,	56,	55,	42,	41,	8,	7,
		1,	0,	1,	1,	1,	0,	1,	0,	0,	172,	171,	102,	101,	92,	91,	54,	53,	44,	43,	6,	5,
		1,	0,	0,	0,	0,	0,	1,	0,	0,	174,	173,	100,	99,	94,	93,	52,	51,	46,	45,	4,	3,
		1,	1,	1,	1,	1,	1,	1,	0,	0,	176,	175,	98,	97,	96,	95,	50,	49,	48,	47,	2,	1 };

/* Private variables -----------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
uint8_t _qr_gen_calc_num_chars_in_string(const char* str);
qr_gen_status_t _qr_gen_uint8_to_bit_array(uint8_t byte_value, uint8_t* bit_array, uint8_t bit_array_length);
qr_gen_status_t _qr_gen_uint16_to_bit_array(uint16_t byte_value, uint8_t* bit_array, uint8_t bit_array_length);
uint8_t _qr_gen_num_bits_req_to_represent_uint(uint16_t byte_value);
qr_gen_status_t _qr_gen_concatenate_bit_arrays(uint8_t* bit_array_0, uint8_t bit_array_0_length, uint8_t* bit_array_1, uint8_t bit_array_1_length, uint8_t* new_bit_array);
uint8_t _qr_gen_division_round_to_whole(uint8_t dividend, uint8_t divisor);
uint8_t _qr_gen_ascii_to_alphanumeric_value(uint8_t ascii_value);
qr_gen_status_t _qr_gen_bit_array_to_uint8(uint8_t* bit_array, uint8_t* byte_value);


/**
 * \brief 		Initializes the main qr_gen struct.
 *
 * \param		qr_gen				A pointer to the main qr_gen struct.
 * \param		mode				The mode of the data to encode. This application only supports ALPHANUMERIC.
 *
 * \returns		qr_gen_status_t		Whether the function completed successfully.
 */
qr_gen_status_t qr_gen_init(qr_gen_t* qr_gen, qr_gen_mode_t mode) {
	qr_gen->mode = mode;
	qr_gen->data_size = ZERO;
	memset(&qr_gen->codewords_data[ZERO], ZERO, QR_LEN_TOTAL_DATA_BITS_REQ);
	memset(&qr_gen->codewords[ZERO], ZERO, QR_LEN_CODEWORDS);
	memset(&qr_gen->ecc_msg_poly[ZERO], ZERO, QR_LEN_CODEWORDS);
	memset(&qr_gen->data_to_encode[ZERO], STR_NULL_CHAR, QR_LEN_MAX_DATA);
	qr_gen->qr_matrix =  QR_FORMAT;

	return QR_GEN_SUCCESS;
}

/**
 * \brief 		Performs the encoding steps of the input data.
 *				First the MODE and COUNT variables are converted to 4 and 9 bit binary arrays, respectively, and then concatenated.
 *				This forms the first part of the encoded binary string. Then the \data is broken up into pairs and (pair[0] * 45 + pair[1]) is converted to an 11-bit binary array
 *				and concatenated to the binary string. If at this stage the binary string has less than 104 bits, up to four 0's are added.
 *				The binary string is then broken up into 8 bit words and if the last byte is not 8 bits in length, then the required number of 0's are added.
 *				If the length is still not 104, then add alternating special bit arrays.
 *				We now have the codewords (coefficients of the message polynomial) and the alpha notation is determined using the Galois Field 256 anti-log tables. 
 *
 * \param		qr_gen				A pointer to the main qr_gen struct.
 * \param		data				The data to encode MAX 15 bytes and must be ALPHANUMERIC chars.
 *
 * \returns		qr_gen_status_t		Whether the function completed successfully.
 */
qr_gen_status_t qr_gen_encode_data(qr_gen_t* qr_gen, const char* data) {
	if (data == NULL)
		return QR_GEN_ERROR;
	qr_gen->data_size = _qr_gen_calc_num_chars_in_string(data);
	if (qr_gen->data_size > QR_LEN_MAX_DATA)
		return QR_GEN_ERROR;
	strcpy(qr_gen->data_to_encode, data);

	/** MODE and COUNT variables are converted to 4 and 9 bit binary arrays, respectively, and then concatenated. */
	uint8_t i;
	uint8_t mode_bit_array[QR_LEN_MODE];
	uint8_t count_bit_array[QR_LEN_COUNT];
	uint8_t mode_and_count_bit_arrays[QR_LEN_MODE + QR_LEN_COUNT];
	if(!_qr_gen_uint8_to_bit_array(qr_gen->mode, mode_bit_array, QR_LEN_MODE))
		return QR_GEN_ERROR;
	if(!_qr_gen_uint8_to_bit_array(qr_gen->data_size, count_bit_array, QR_LEN_COUNT))
		return QR_GEN_ERROR;
	if(!_qr_gen_concatenate_bit_arrays(mode_bit_array, QR_LEN_MODE, count_bit_array, QR_LEN_COUNT, mode_and_count_bit_arrays))
		return QR_GEN_ERROR;

	/** Then data is broken up into pairs and (pair[0] * 45 + pair[1]) is converted to an 11-bit binary array and concatenated to the binary string. */
	uint8_t data_pairs = _qr_gen_division_round_to_whole(qr_gen->data_size, QR_LEN_DATA_PAIR);
	uint8_t data_encoded_size = QR_LEN_MODE + QR_LEN_COUNT;	/** Encoded string starts with mode and count. */
	uint8_t is_odd = ZERO;
	if ((data_pairs * QR_LEN_DATA_PAIR) > qr_gen->data_size) {		/** There was an odd number of pairs. */
		data_encoded_size += ((data_pairs - ONE) * QR_LEN_DATA_PAIR_BITS) + QR_LEN_DATA_SINGLE_BITS;
		is_odd = ONE;
	} else {
		data_encoded_size += data_pairs * QR_LEN_DATA_PAIR_BITS;
	}
	
	uint8_t data_encoded[data_encoded_size];
	uint8_t* p_data_encoded = &data_encoded[ZERO];
	uint8_t data_encoded_index = ZERO;
	memset(data_encoded, ZERO, data_encoded_size);
	memcpy(p_data_encoded, mode_and_count_bit_arrays, QR_LEN_MODE + QR_LEN_COUNT);
	p_data_encoded += (QR_LEN_MODE + QR_LEN_COUNT);

	for (i = 0; i < qr_gen->data_size; i += 2) {
		uint16_t encode = 0;
		uint8_t encode_bit_length = 0;
		if (is_odd && (i == (qr_gen->data_size - ONE))) {
			encode = _qr_gen_ascii_to_alphanumeric_value((uint8_t)qr_gen->data_to_encode[i]);
			encode_bit_length = QR_LEN_DATA_SINGLE_BITS;
		} else {
			encode = (uint16_t)(_qr_gen_ascii_to_alphanumeric_value((uint8_t)qr_gen->data_to_encode[i]) * QR_DATA_PAIR_FACTOR) + _qr_gen_ascii_to_alphanumeric_value((uint8_t)qr_gen->data_to_encode[i + 1]);
			encode_bit_length = QR_LEN_DATA_PAIR_BITS;
		}
		uint8_t encode_bit_array[encode_bit_length];
		if (encode > UINT8_MAX) {
			if(!_qr_gen_uint16_to_bit_array(encode, encode_bit_array, encode_bit_length))
				return QR_GEN_ERROR;
		} else {
			if(!_qr_gen_uint8_to_bit_array(encode, encode_bit_array, encode_bit_length))
				return QR_GEN_ERROR;
		}

		memcpy(p_data_encoded, &encode_bit_array[0], encode_bit_length);
		p_data_encoded += encode_bit_length;
	}

	/** Add up to four 0's if data_encoded_size is less than 104 (level Q error correction). */
	uint8_t data_encoded_pad4_size = data_encoded_size + ZERO_PAD_4_SIZE;
	uint8_t data_encoded_pad4[data_encoded_pad4_size];
	if(!_qr_gen_concatenate_bit_arrays(data_encoded, data_encoded_size, ZERO_PAD_4, ZERO_PAD_4_SIZE, data_encoded_pad4))
		return QR_GEN_ERROR;

	/** Now break up string into groups of 8 bits. If last group is not 8 bits wide, then pad with 0's to the right. */
	uint8_t rem = data_encoded_pad4_size % EIGHT_BITS;
	uint8_t required_extra_zeros = 0;
	if (rem != 0) {
		required_extra_zeros = EIGHT_BITS - rem;
	}
	uint8_t extra_zeros[required_extra_zeros];
	memset(extra_zeros, ZERO, required_extra_zeros);
	uint8_t data_encoded_mod8_size = data_encoded_pad4_size + required_extra_zeros;
	uint8_t data_encoded_mod8[data_encoded_mod8_size];
	if(!_qr_gen_concatenate_bit_arrays(data_encoded_pad4, data_encoded_pad4_size, extra_zeros, required_extra_zeros, data_encoded_mod8))
		return QR_GEN_ERROR;

	/** At this point the number of bits in the encoded string will be a multiple of 8. If the total bits is not 104, then
	 * alternating 11101100 and 00010001 words need to be concatenated to the string. */
	uint8_t total_data_bits = data_encoded_mod8_size;
	uint8_t bits_req = QR_LEN_TOTAL_DATA_BITS_REQ - total_data_bits;
	uint8_t words_req = bits_req / EIGHT_BITS;
	uint8_t word_index;
	memcpy(qr_gen->codewords_data, data_encoded_mod8, data_encoded_mod8_size);
	uint8_t* p_codewords_data = &qr_gen->codewords_data[data_encoded_mod8_size];	/**< Start adding the special bit arrays after all the other data. */
	for (word_index = 1; word_index <= words_req; word_index++) {
		if (word_index % 2 != ZERO) {	// Odd
			memcpy(p_codewords_data, SPECIAL_BIT_STRING_ODD, EIGHT_BITS);
		} else {
			memcpy(p_codewords_data, SPECIAL_BIT_STRING_EVEN, EIGHT_BITS);
		}
		p_codewords_data += EIGHT_BITS;
	}

	p_codewords_data = &qr_gen->codewords_data[ZERO];	/**< Point to the beginning of the array to calculate decimal equivalents of the codewords. */
	uint8_t codeword = 0;
	uint8_t codeword_index = 0;
	for (word_index = 0; word_index < QR_LEN_TOTAL_DATA_BITS_REQ; word_index += EIGHT_BITS, p_codewords_data += EIGHT_BITS) {
		if(_qr_gen_bit_array_to_uint8(p_codewords_data, &codeword))
			qr_gen->codewords[codeword_index++] = codeword;
	}

	/** Determine the alpha notation. */
	for (word_index = 0; word_index <= QR_LEN_CODEWORDS; word_index++) {
		qr_gen->ecc_msg_poly[word_index] = ECC_GLF_256_ANTILOG_EXP_OF_A[qr_gen->codewords[word_index] - ONE];
	}

	return QR_GEN_SUCCESS;
}

/**
 * \brief 		This function calculates the error correction words by performing polynomial long division using the codewords and their alpha notations.
 *
 * \param		qr_gen				A pointer to the main qr_gen struct.
 *
 * \returns		qr_gen_status_t		Whether the function completed successfully.
 */
qr_gen_status_t qr_gen_determine_ecc(qr_gen_t* qr_gen) {
	uint8_t i;
	uint8_t j;
	uint8_t mult[QR_LEN_CODEWORDS + ONE];
	uint8_t alpha[QR_LEN_CODEWORDS + ONE];
	uint8_t xor[QR_LEN_CODEWORDS + ONE];
	uint8_t xor_temp[QR_LEN_CODEWORDS + ONE];
	uint8_t alpha_temp[QR_LEN_CODEWORDS];
	uint8_t alpha_anti[QR_LEN_CODEWORDS];
	memcpy(&alpha_temp[ZERO], &qr_gen->ecc_msg_poly[ZERO], QR_LEN_CODEWORDS);
	xor_temp[ZERO] = ZERO;
	memcpy(&xor_temp[ONE], &qr_gen->codewords[ZERO], QR_LEN_CODEWORDS);
	uint16_t temp = 0;
	for (i = 0; i < QR_LEN_CODEWORDS; i++) {				// message polynomial coefficients
		memcpy(&alpha_anti[ZERO], &alpha_temp[ZERO], QR_LEN_CODEWORDS);
		memcpy(&xor[ZERO], &xor_temp[ZERO], QR_LEN_CODEWORDS + ONE);
		for (j = 0; j < QR_LEN_CODEWORDS + ONE; j++) {		// generator polynomial coefficients
			temp = alpha_anti[ZERO] + ECC_GEN_POLY[j];
			if (temp > UINT8_MAX) {
//				temp = (temp % (UINT8_MAX + ONE)) + (alpha_anti[ZERO] / ECC_GEN_POLY[j]);
				temp = (temp % (UINT8_MAX));
			}
			mult[j] = (uint8_t)temp;
			alpha[j] = (uint8_t)ECC_GLF_256_LOG_EXP_OF_A[mult[j]];
			if (j < QR_LEN_CODEWORDS) {				/**< There are 13 codewords, but 14 polynomial terms. */
				xor_temp[j] = (uint8_t)(xor[j + ONE] ^ alpha[j]);
			} else {
				xor_temp[j] = alpha[j];
			}

			if (j > ZERO) {
				alpha_temp[j - ONE] = ECC_GLF_256_ANTILOG_EXP_OF_A[xor_temp[j] - ONE];
			}
		}
	}

	/** Combine codewords and xor_temp and convert to bit string. */
	uint8_t qr_matrix_byte;
	uint8_t* p_qr_matrix_data = &qr_gen->qr_matrix_data[ZERO];
	for (i = 0; i < QR_LEN_DATA_Q_LEVEL_ECC; i++) {
		if (i < QR_LEN_CODEWORDS) {
			qr_matrix_byte = qr_gen->codewords[i];
		} else {
			qr_matrix_byte = xor_temp[(i - QR_LEN_CODEWORDS) + ONE];	/**< We do not want the 0th element of the final XOR data. */
		}
		if(!_qr_gen_uint8_to_bit_array(qr_matrix_byte, p_qr_matrix_data, EIGHT_BITS))
			return QR_GEN_ERROR;
		p_qr_matrix_data += EIGHT_BITS;
	}

	return QR_GEN_SUCCESS;
}

/**
 * \brief 		This function calculates the final QR code bit matrix.
 *				First the selected mask bits are added to the output matrix. The mask matrix is then determined by performing the corresponding mathematical operation defined by the  
 *				selected mask level. The mask is then applied to the data bits.
 *
 * \param		qr_gen				A pointer to the main qr_gen struct.
 * \param		mask_level			The mask to apply to the data bits. 
 *
 * \returns		qr_gen_status_t		Whether the function completed successfully.
 */
qr_gen_status_t qr_gen_create_code(qr_gen_t* qr_gen, qr_gen_data_mask_level_q_t mask_level) {
	uint8_t row;
	uint8_t col;
	uint8_t* p_mask = NULL;

	switch (mask_level) {
		case MASK_0:
			p_mask = &MASK_Q_0[ZERO];
		break;
		case MASK_1:
			p_mask = &MASK_Q_1[ZERO];
		break;
		case MASK_2:
			p_mask = &MASK_Q_2[ZERO];
		break;
		case MASK_3:
			p_mask = &MASK_Q_3[ZERO];
		break;
		case MASK_4:
			p_mask = &MASK_Q_4[ZERO];
		break;
		case MASK_5:
			p_mask = &MASK_Q_5[ZERO];
		break;
		case MASK_6:
			p_mask = &MASK_Q_6[ZERO];
		break;
		case MASK_7:
			p_mask = &MASK_Q_7[ZERO];
		break;

		default:
		break;
	}

	/** Populate Row 8 - selected mask bits. */
	row = QR_MASK_ROW;
	for (col = 0; col < QR_MATRIX_WIDTH; col++) {
		if (col == QR_MASK_COL_EXCLUDE || ((col >= QR_MASK_EXC_RNG_1) && (col <= QR_MASK_EXC_RNG_2)))
			continue;
		if (col == QR_MASK_EXC_RNG_2 + ONE)
			p_mask -= ONE;
		(*qr_gen->qr_matrix)[row][col] = *p_mask;
		p_mask++;
	}

	/** Populate Col 8 - selected mask bits. */
	p_mask--;
	col = QR_MASK_COL;
	for (row = 0; row < QR_MATRIX_HEIGHT; row++) {
		if (row == QR_MASK_ROW_EXCLUDE || ((row >= QR_MASK_EXC_RNG_1) && (row <= QR_MASK_EXC_RNG_2 + ONE)))
			continue;
		(*qr_gen->qr_matrix)[row][col] = *p_mask;
		p_mask--;
	}

	/** Create mask matrix. */
	uint8_t mask[QR_MATRIX_HEIGHT][QR_MATRIX_WIDTH] = {0};
	for (row = 0; row < QR_MATRIX_HEIGHT; row++) {
		for (col = 0; col < QR_MATRIX_WIDTH; col++) {
			if ((*qr_gen->qr_matrix)[row][col] > ONE) {			/**< Implies data bits. */
				switch (mask_level) {
					case MASK_0:
						mask[row][col] = (((row + col) % 2) == ZERO)? ONE : ZERO;
					break;
					case MASK_1:
						mask[row][col] = ((row % 2) == ZERO)? ONE : ZERO;
					break;
					case MASK_2:
						mask[row][col] = ((col % 3) == ZERO)? ONE : ZERO;
					break;
					case MASK_3:
						mask[row][col] = (((row + col) % 3) == ZERO)? ONE : ZERO;
					break;
					case MASK_4:
						mask[row][col] = ((((row / 2) + (col / 3)) % 3) == ZERO)? ONE : ZERO;
					break;
					case MASK_5:
						mask[row][col] = ((((row * col) % 2) + (((row * col)) % 3)) == ZERO)? ONE : ZERO;
					break;
					case MASK_6:
						mask[row][col] = (((((row * col) % 2) + (((row * col)) % 3))) % 2 == ZERO)? ONE : ZERO;
					break;
					case MASK_7:
						mask[row][col] = (((((row + col) % 2) + (((row * col)) % 3))) % 2 == ZERO)? ONE : ZERO;
					break;

					default:
					break;
				}
			}
		}
	}

	/** Populate data bits and apply mask. */
	for (row = 0; row < QR_MATRIX_HEIGHT; row++) {
		for (col = 0; col < QR_MATRIX_WIDTH; col++) {
			if ((*qr_gen->qr_matrix)[row][col] > ONE) {
				(*qr_gen->qr_matrix)[row][col] = qr_gen->qr_matrix_data[(*qr_gen->qr_matrix)[row][col] - ONE];
				(*qr_gen->qr_matrix)[row][col] = (mask[row][col] == ONE) ? !(*qr_gen->qr_matrix)[row][col] : (*qr_gen->qr_matrix)[row][col];
			}
		}
	}

	return QR_GEN_SUCCESS;
}

/**
  * \brief  Calculates the number of elements in an array that is not a null-terminating character.
  *
  * \param	str		The array to operate on.
  *
  * \returns Number of non-null-terminating characters in the array.
  */
uint8_t _qr_gen_calc_num_chars_in_string(const char* str) {
	if (str == NULL)
		return 0;
	uint8_t bytes = 0;
	while (*str++)
		bytes++;
	return bytes;
}

/**
  * \brief  Converts an 8 bit binary array to uint8_t.
  *
  * \param		bit_array		The bit array.
  * \param		byte_value		A pointer to the calculated value.
  *
  * \returns	qr_gen_status_t		Whether the function completed successfully.
  */
qr_gen_status_t _qr_gen_bit_array_to_uint8(uint8_t* bit_array, uint8_t* byte_value) {
	if (!byte_value || !bit_array) {
		return QR_GEN_ERROR;
	}

	uint8_t i;
	*byte_value = 0;
	for(i = 0 ; i < EIGHT_BITS; i++) {
		*byte_value |= (uint8_t)(bit_array[i] << (EIGHT_BITS - i - ONE));
	}
	return QR_GEN_SUCCESS;
}

/**
  * \brief  Converts a uint8_t to a binary array. If the length of the output array is greater than 8 bits, the array is zero-padded.
  *
  * \param	byte_value			The value to convert.
  * \param	bit_array			A pointer to the output bit array.
  * \param	bit_array_length	The length of the bit array.
  *
  * \returns	qr_gen_status_t		Whether the function completed successfully.
  */
qr_gen_status_t _qr_gen_uint8_to_bit_array(uint8_t byte_value, uint8_t* bit_array, uint8_t bit_array_length) {
	uint8_t min_req_bits = _qr_gen_num_bits_req_to_represent_uint(byte_value);
	if (bit_array_length < min_req_bits || !bit_array) {
		return QR_GEN_ERROR;
	}

	uint8_t i = bit_array_length;
	while (--i != UINT8_MAX) {
		if (i > (bit_array_length - SIZE_CHAR - ONE)) {
			bit_array[i] = (byte_value & (BIT_0 << (bit_array_length - i - ONE))) ? ONE : ZERO;
		} else {
			bit_array[i] = ZERO;
		}
	}
	return QR_GEN_SUCCESS;
}

/**
  * \brief  Converts a uint16_t to a variable length binary array. If the length of the output array is greater than the minimum number of bits required
  *			to represent the number, the array is zero-padded.
  *
  * \param	byte_value			The value to convert.
  * \param	bit_array			A pointer to the output bit array.
  * \param	bit_array_length	The length of the bit array.
  *
  * \returns	qr_gen_status_t		Whether the function completed successfully.
  */
qr_gen_status_t _qr_gen_uint16_to_bit_array(uint16_t byte_value, uint8_t* bit_array, uint8_t bit_array_length) {
	uint8_t min_req_bits = _qr_gen_num_bits_req_to_represent_uint(byte_value);
	if (bit_array_length < min_req_bits || !bit_array) {
		return QR_GEN_ERROR;
	}

	uint8_t i = bit_array_length;
	while (--i != UINT8_MAX) {
		if (i <= min_req_bits) {
			bit_array[i] = (byte_value & (BIT_0 << (bit_array_length - i - ONE))) ? ONE : ZERO;
		} else {
			bit_array[i] = 0;
		}
	}
	return QR_GEN_SUCCESS;
}

/**
  * \brief  Determines the minimum number of bits required to represent an integer.
  *
  * \param	byte_value			The number in question.
  *
  * \returns	uint8_t		The minimum number of bits required to represent \byte_value.
  */
uint8_t _qr_gen_num_bits_req_to_represent_uint(uint16_t byte_value) {
	uint8_t count = 0;
	while (byte_value > ZERO) {
	    count++;
	    byte_value >>= ONE;
	}
	return count;
}

/**
  * \brief  Concatenates two arrays by adding bit_array_1 after bit_array_0. The level at which this function is called from is required to manage the memory - this function assumes that the memory has already been
  *			allocated for all the arrays.
  *
  * \param	bit_array_0				The first bit array.
  * \param	bit_array_0_length		The first bit array's length.
  * \param	bit_array_1				The second bit array.
  * \param	bit_array_1_length		The second bit array's length.
  * \param	new_bit_array			The concatenated array.
  *
  * \returns	qr_gen_status_t		Whether the function completed successfully.
  */
qr_gen_status_t _qr_gen_concatenate_bit_arrays(uint8_t* bit_array_0, uint8_t bit_array_0_length, uint8_t* bit_array_1, uint8_t bit_array_1_length, uint8_t* new_bit_array) {
	if (!bit_array_0 || !bit_array_1)
		return QR_GEN_ERROR;

	uint8_t i;
	for (i = 0; i < (bit_array_0_length + bit_array_1_length); i++) {
		if (i < bit_array_0_length) {
			new_bit_array[i] =  bit_array_0[i];
		} else {
			new_bit_array[i] =  bit_array_1[i - bit_array_0_length];
		}
	}

	return QR_GEN_SUCCESS;
}

/**
  * \brief  Determines the QR alphanumeric value of an ASCII character.
  *
  * \param	ascii_value			The character to convert.
  *
  * \returns	uint8_t		The QR alphanumeric value.
  */
uint8_t _qr_gen_ascii_to_alphanumeric_value(uint8_t ascii_value) {
	uint8_t alphanumeric_value = ZERO;
	if (ascii_value >= 'A' && ascii_value <= 'Z') {
		alphanumeric_value = ascii_value - ASCII_OFFSET_LET;
	} else if (ascii_value >= '0' && ascii_value <= '9') {
		alphanumeric_value = ascii_value - ASCII_OFFSET_NUM;
	} else {
		switch (ascii_value) {
			case ' ':
				alphanumeric_value = 36;
				break;
			case '$':
				alphanumeric_value = 37;
				break;
			case '%':
				alphanumeric_value = 38;
				break;
			case '*':
				alphanumeric_value = 39;
				break;
			case '+':
				alphanumeric_value = 40;
				break;
			case '-':
				alphanumeric_value = 41;
				break;
			case '.':
				alphanumeric_value = 42;
				break;
			case '/':
				alphanumeric_value = 43;
				break;
			case ':':
				alphanumeric_value = 44;
				break;
			default:
				break;
		}
	}
	return alphanumeric_value;
}

/**
  * \brief  Divides two numbers and determines the next whole number.
  *
  * \param	dividend		The dividend.
  * \param	divisor			The divisor.
  *
  * \returns	uint8_t		The quotient .
  */
uint8_t _qr_gen_division_round_to_whole(uint8_t dividend, uint8_t divisor) {
    return ((dividend + (divisor - 1)) / divisor);
}
