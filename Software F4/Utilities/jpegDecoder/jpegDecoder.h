/*
 * jpegDecoder.h
 *
 *  Created on: 14.07.2017
 *      Author: Karol
 */

#ifndef JPEGDECODER_JPEGDECODER_H_
#define JPEGDECODER_JPEGDECODER_H_

#include "JPEG.h"

#ifndef max
#define max(a,b)     (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)     (((a) < (b)) ? (a) : (b))
#endif

uint8_t decodeJPEG(JPEG img, uint8_t* dst);

#endif /* JPEGDECODER_JPEGDECODER_H_ */
