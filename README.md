# qr_gen
Embedded QR code generator

##General Information
qr_gen generates a 21 x 21 QR code based on a string input. The library was written on a 32-bit Atmel UC3A0512 MCU and contains no dynamic memory allocation. There is no MCU specific code in the library and, as such, should work with any C compiler. 
The program outputs the QR code in ASCII and the raw data matrix on USART1.


##Library Usage
```C
const char data_to_encode[] = "HELLO WORLD";
qr_gen_t qr_gen;                                  /**< The main qr_gen struct. */
qr_gen_init(&qr_gen, ALPHANUMERIC);               /**< Initialise in ALPHANUMERIC mode. */
if(!qr_gen_encode_data(&qr_gen, data_to_encode))  /**< Encode the data and determine the message polynomial coefficients and its alpha notation equivalents. */
	return QR_GEN_ERROR;
if(!qr_gen_determine_ecc(&qr_gen))                /**< Determine the Error Correction words by performing polynomial long division. */
	return QR_GEN_ERROR;
if(!qr_gen_create_code(&qr_gen, MASK_3))          /**< Create the QR code matrix by calculating the mask matrix, masking the data and putting it in the correct QR format. */
	return QR_GEN_ERROR;
/** Then the output data resides in qr_gen.qr_matrix_data */
```

##Output
```C
---------------
    qr_gen
---------------
Data to encode: HELLO WORLD

##############      ##      ##############
##          ##  ####  ##    ##          ##
##  ######  ##  ##    ##    ##  ######  ##
##  ######  ##        ####  ##  ######  ##
##  ######  ##        ####  ##  ######  ##
##          ##    ##  ##    ##          ##
##############  ##  ##  ##  ##############
                  ##                      
  ######  ####                      ####  
##        ##  ########  ####    ##########
##  ######  ####  ##  ####  ##      ##    
  ######      ##  ##      ##    ##  ####  
##  ####    ##    ##  ####          ##    
                ##    ##  ##  ##    ####  
##############      ##  ########  ##  ####
##          ##  ####      ######    ####  
##  ######  ##        ####  ####      ####
##  ######  ##  ##  ##    ##    ####  ##  
##  ######  ##  ########  ##    ####      
##          ##  ##########  ####  ##      
##############        ####    ##    ######


1 1 1 1 1 1 1 0 0 0 1 0 0 0 1 1 1 1 1 1 1 
1 0 0 0 0 0 1 0 1 1 0 1 0 0 1 0 0 0 0 0 1 
1 0 1 1 1 0 1 0 1 0 0 1 0 0 1 0 1 1 1 0 1 
1 0 1 1 1 0 1 0 0 0 0 1 1 0 1 0 1 1 1 0 1 
1 0 1 1 1 0 1 0 0 0 0 1 1 0 1 0 1 1 1 0 1 
1 0 0 0 0 0 1 0 0 1 0 1 0 0 1 0 0 0 0 0 1 
1 1 1 1 1 1 1 0 1 0 1 0 1 0 1 1 1 1 1 1 1 
0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 
0 1 1 1 0 1 1 0 0 0 0 0 0 0 0 0 0 0 1 1 0 
1 0 0 0 0 1 0 1 1 1 1 0 1 1 0 0 1 1 1 1 1 
1 0 1 1 1 0 1 1 0 1 0 1 1 0 1 0 0 0 1 0 0 
0 1 1 1 0 0 0 1 0 1 0 0 0 1 0 0 1 0 1 1 0 
1 0 1 1 0 0 1 0 0 1 0 1 1 0 0 0 0 0 1 0 0 
0 0 0 0 0 0 0 0 1 0 0 1 0 1 0 1 0 0 1 1 0 
1 1 1 1 1 1 1 0 0 0 1 0 1 1 1 1 0 1 0 1 1 
1 0 0 0 0 0 1 0 1 1 0 0 0 1 1 1 0 0 1 1 0 
1 0 1 1 1 0 1 0 0 0 0 1 1 0 1 1 0 0 0 1 1 
1 0 1 1 1 0 1 0 1 0 1 0 0 1 0 0 1 1 0 1 0 
1 0 1 1 1 0 1 0 1 1 1 1 0 1 0 0 1 1 0 0 0 
1 0 0 0 0 0 1 0 1 1 1 1 1 0 1 1 0 1 0 0 0 
1 1 1 1 1 1 1 0 0 0 0 1 1 0 0 1 0 0 1 1 1
```

As there was no LCD available for the MCU, in order to evauluate the QR code, the raw data matrix was copied in to a spreadsheet. Then conditional formatting was applied:
  - If 1, set cell background and font colour to black.
  - If 0, set cell background and font colour to white.

The conditional formatting operation produced the following QR code (for the case above):
  
![alt tag](https://cloud.githubusercontent.com/assets/11454244/18050245/bcdc6a40-6ded-11e6-9f3e-af495755c1c0.png)

##Limitations and Optimizations
- Version 1 QR code
- Level Q Error Correction
- All eight masks have been implemented, but penalty scores are not calculated.
- Only ALPHANUMERIC mode has been implemented.
- The polynomial long divison operation can be optimised. There are some arrays that are created in RAM that is not necessary.
- Another optimization that can be made is the implementation of a programmatic generation of the module placement matrix. As this project only assumed Version 1 with Q Level ECC, the QR matrix format was hardcoded. 
