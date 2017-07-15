/*
 * jpegDecoder.c
 *
 *  Created on: 14.07.2017
 *      Author: Karol
 */
#include <stdint.h>
#include "string.h"
#include "jpegDecoder.h"
#include "picojpeg.h"
#include "math.h"

static uint32_t data_remain;

unsigned char pjpeg_callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data)
{
    static uint32_t n = 0;
    uint8_t size = min(buf_size,data_remain);
    memcpy(pBuf,pCallback_data + n,size);
    n+=size;

    *pBytes_actually_read = size;
    return 0;
}

uint8_t decodeJPEG(JPEG img, uint8_t* dst)
{
	pjpeg_image_info_t image_info;
	uint32_t mcu_x = 0;
	uint32_t mcu_y = 0;

	data_remain = img.size;
	uint8_t status = pjpeg_decode_init(&image_info, pjpeg_callback, img.data, 0);

	if(status)
		return status;

	while(!pjpeg_decode_mcu())
	{
		uint32_t x_block = mcu_x * image_info.m_MCUWidth * (image_info.m_comps + 1);
		uint32_t y_block = mcu_y * image_info.m_width * (image_info.m_comps + 1) * image_info.m_MCUHeight;

		const uint8_t *pSrcR = image_info.m_pMCUBufR;
		const uint8_t *pSrcG = image_info.m_pMCUBufG;
		const uint8_t *pSrcB = image_info.m_pMCUBufB;

		for(uint32_t i = 0; i < image_info.m_MCUHeight; i++)
		{
			for(uint32_t j = 0; j < image_info.m_MCUWidth; j++)
			{
				uint32_t y = y_block + i*image_info.m_width*(image_info.m_comps + 1);
				uint32_t x = x_block + j*(image_info.m_comps + 1);

				dst[y + x] = *pSrcB++;
				dst[y + x + 1] = *pSrcG++;
				dst[y + x + 2] = *pSrcR++;
				dst[y + x + 3] = 0xFF;
	        }
		}

		mcu_x++;
		if(mcu_x == image_info.m_MCUSPerRow)
		{
			mcu_x = 0;
			mcu_y++;
		}
	}

	return status;
}



