/*
 * RTLSDR.cpp
 *
 *  Created on: 18.10.2017
 *      Author: Karol
 */

#include "RTLSDR.h"
#include "USBDriver.h"

#include "convenience.h"
#include "rtl-sdr.h"
#include "BoardMenager.h"

extern bool urdOccured;

RTLSDR::RTLSDR(USBDriver* usbDriverhandle) : usbDriverHandle(usbDriverhandle)
{
	deviceOpen = false;
}

uint8_t RTLSDR::OpenDevice(int32_t  tunerPpmError,
                       uint32_t  tunerFrequency,
                       uint32_t  adcSampRate)
{
    dev = &static_dev;

    int8_t dongle_open = rtlsdr_open(&dev, 0);

    if (dongle_open < 0) {
        //DEBUG_PRINT("Failed to open rtlsdr device (%i).\n", dongle_open);
        return dongle_open;
    } else {
        //DEBUG_PRINT("Opened rtlsdr\n");
    }

    verbose_auto_gain(dev);

    rtlsdr_set_freq_correction(dev, tunerPpmError);
    verbose_set_frequency(dev, tunerFrequency);


    // Set the sample rate
    verbose_set_sample_rate(dev, adcSampRate);

    // Reset endpoint before we start reading from it (mandatory)
    verbose_reset_buffer(dev);

    // begin capture radio data
    rtlsdr_read_sync(dev, buff,2048,0);
    return 0;
}

void RTLSDR::NotifyNewRawSampleRecived()
{
	rawSamplesBuffer.Push();
	if(rawSamplesBuffer.IsBufferFull() == true) { return; };
	//GetNewRawSamples();
}

void RTLSDR::NotifyRawSampleProcessed()
{
	rawSamplesBuffer.Pop();
	GetNewRawSamples();
}

uint8_t* RTLSDR::GetRawSamplesFromBuffer()
{
	return buff;
}
/*
std::array<uint8_t,USB_IN_STREAM_SIZE>& RTLSDR::GetRawSamplesFromBuffer()
{
	return rawSamplesBuffer.GetTailElement();
}
*/
void RTLSDR::GetNewRawSamples()
{
	rtlsdr_read_sync(dev, buff,2048,0);
/*
	rtlsdr_read_sync(dev, rawSamplesBuffer.GetHeadElement().data(),
			              rawSamplesBuffer.GetHeadElement().size(), 0);
			              */
}

extern BoardMenager br;

extern "C" void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
#if (USBH_USE_OS == 1)
  USBH_LL_NotifyURBChange(hhcd->pData);
#endif

  UNUSED(hhcd);
  UNUSED(chnum);


  if (urb_state == URB_DONE)
  {

  }
}

/*
	InitBaseband();

	SetI2CRepeater(true);

	uint8_t answer = I2CReadReg(R820T_I2C_ADDR, R82XX_CHECK_ADDR);
	if(answer == R82XX_CHECK_VAL)
	{
		tuner_type = RTLSDR_TUNER_R820T;
	}
	else
	{
		return;
	}

	DemodWriteReg(1, 0xb1, 0x1a, 1);

	DemodWriteReg(0, 0x08, 0x4d, 1);

	SetIfFreq(R82XX_IF_FREQ);

	DemodWriteReg(1, 0x15, 0x01, 1);

	xtal_cap_sel = XTAL_HIGH_CAP_0P;

	R82xxWrite(0x05,r82xx_init_array, sizeof(r82xx_init_array));

	rc = r82xx_set_tv_standard(priv, 3, TUNER_DIGITAL_TV, 0);

	rc = r82xx_sysfreq_sel(priv, 0, TUNER_DIGITAL_TV, SYS_DVBT);

	SetI2CRepeater(false);
 */

void RTLSDR::InitBaseband()
{
	WriteReg(USBB, USB_SYSCTL, 0x09, 1);
	WriteReg(USBB, USB_EPA_MAXPKT, 0x0002, 2);
	WriteReg(USBB, USB_EPA_CTL, 0x1002, 2);

    /* poweron demod */
	WriteReg(SYSB, DEMOD_CTL_1, 0x22, 1);
	WriteReg(SYSB, DEMOD_CTL, 0xe8, 1);

	DemodWriteReg(1, 0x01, 0x14, 1);
	DemodWriteReg(1, 0x01, 0x10, 1);

    /* disable spectrum inversion and adjacent channel rejection */
	DemodWriteReg(1, 0x15, 0x00, 1);
	DemodWriteReg(1, 0x16, 0x0000, 2);

	/* clear both DDC shift and IF frequency registers  */
	for (uint8_t i = 0; i < 6; i++){
		DemodWriteReg(1, 0x16 + i, 0x00, 1);
	}

	SetFir();

	/* enable SDR mode, disable DAGC (bit 5) */
	DemodWriteReg(0, 0x19, 0x05, 1);

	/* init FSM state-holding register */
	DemodWriteReg(1, 0x93, 0xf0, 1);
	DemodWriteReg(1, 0x94, 0x0f, 1);

	/* disable AGC (en_dagc, bit 0) (this seems to have no effect) */
	DemodWriteReg(1, 0x11, 0x00, 1);

	/* disable RF and IF AGC loop */
	DemodWriteReg(1, 0x04, 0x00, 1);

	/* disable PID filter (enable_PID = 0) */
	DemodWriteReg(0, 0x61, 0x60, 1);

	/* opt_adc_iq = 0, default ADC_I/ADC_Q datapath */
	DemodWriteReg(0, 0x06, 0x80, 1);

	/*  Enable Zero-IF mode (en_bbin bit), DC cancellation (en_dc_est),
	    IQ estimation/compensation (en_iq_comp, en_iq_est) */
	DemodWriteReg(1, 0xb1, 0x1b, 1);

	    /* disable 4.096 MHz clock output on pin TP_CK0 */
	DemodWriteReg(0, 0x0d, 0x83, 1);

}

void RTLSDR::WriteReg(uint8_t block, uint16_t addr, uint16_t val, uint8_t len)
{
    unsigned char data[2];
    uint16_t index = (block << 8) | 0x10;
    if (len == 1) {
    	data[0] = val & 0xff;
    }else{
    	data[0] = val >> 8;
    }

    data[1] = val & 0xff;

    usbDriverHandle->ControlTransfer(CTRL_OUT, 0, addr, index, data, len);

}

void RTLSDR::DemodWriteReg(uint8_t page, uint16_t addr, uint16_t val, uint8_t len)
{
    unsigned char data[2];
    uint16_t index = 0x10 | page;
    addr = (addr << 8) | 0x20;

    if (len == 1){
    	data[0] = val & 0xff;
    }else
    {
    	data[0] = val >> 8;
    }
    data[1] = val & 0xff;

    usbDriverHandle->ControlTransfer(CTRL_OUT, 0, addr, index, data, len);


    DemodReadReg(0x0a, 0x01, 1);
}

uint16_t RTLSDR::DemodReadReg(uint8_t page, uint16_t addr, uint8_t len) {
    unsigned char data[2];

    uint16_t index = page;
    uint16_t reg;
    addr = (addr << 8) | 0x20;

    usbDriverHandle->ControlTransfer(CTRL_IN, 0, addr, index, data, len);

    reg = (data[1] << 8) | data[0];

    return reg;
}

void RTLSDR::SetFir()
{
	int fir_default[] = {
	    -54, -36, -41, -40, -32, -14, 14, 53,	/* 8 bit signed */
	    101, 156, 215, 273, 327, 372, 404, 421	/* 12 bit signed */
	};
    int fir[20];

    /* format: int8_t[8] */
    for (uint8_t i = 0; i < 8; ++i) {
        const int val = fir_default[i];

        if (val < -128 || val > 127) {
            //return -1;
        }

        fir[i] = val;
    }

    /* format: int12_t[8] */
    for (uint8_t i = 0; i < 8; i += 2) {
        const int val0 = fir_default[8 + i];
        const int val1 = fir_default[8 + i + 1];

        if (val0 < -2048 || val0 > 2047 || val1 < -2048 || val1 > 2047) {
            //return -1;
        }

        fir[8 + i * 3 / 2] = val0 >> 4;
        fir[8 + i * 3 / 2 + 1] = (val0 << 4) | ((val1 >> 8) & 0x0f);
        fir[8 + i * 3 / 2 + 2] = val1;
    }

    for (uint16_t i = 0; i < sizeof(fir); i++) {
       // DemodWriteReg(1, 0x1c + i, fir[i], 1); // if fail
        //{ return -1; }
    }

   // return 0;
}

void RTLSDR::SetI2CRepeater(bool on) {
    DemodWriteReg(1, 0x01, on ? 0x18 : 0x10, 1);
}

uint8_t RTLSDR::I2CReadReg(uint8_t i2c_addr, uint8_t reg) {
    uint16_t addr = i2c_addr;
    uint8_t data = 0;

    WriteArray(IICB, addr, &reg, 1);
    ReadArray(IICB, addr, &data, 1);

    return data;
}

int RTLSDR::I2CWriteReg(uint8_t i2c_addr, uint8_t* buffer, int len) {
    uint16_t addr = i2c_addr;


    return WriteArray(IICB, addr, buffer, len);
}
int RTLSDR::ReadArray(uint8_t block, uint16_t addr, uint8_t* array, uint8_t len) {
    uint16_t index = (block << 8);

    usbDriverHandle->ControlTransfer(CTRL_IN, 0, addr, index, array, len);

    return 0;
}

int RTLSDR::WriteArray(uint8_t block, uint16_t addr, uint8_t* array, uint8_t len) {
    uint16_t index = (block << 8) | 0x10;

    usbDriverHandle->ControlTransfer(CTRL_OUT, 0, addr, index, array, len);

    return 0;
}

int RTLSDR::R82xxWrite(uint8_t reg, const uint8_t *val,unsigned int len)
{
	int size, pos = 0;
	unsigned int max_i2c_msg_len = 8;
	/* Store the shadow registers */
	ShadowStore(reg, val, len);

	uint8_t buf[2];
	do {
		if (len > max_i2c_msg_len - 1)
			size = max_i2c_msg_len - 1;
		else
			size = len;

		/* Fill I2C buffer */
		buf[0] = reg;
		memcpy(&buf[1], &val[pos], size);

		//I2CWriteReg(R820T_I2C_ADDR,buf, size + 1);


		reg += size;
		len -= size;
		pos += size;
	} while (len > 0);

	return 0;
}

void RTLSDR::ShadowStore(uint8_t reg, const uint8_t *val, int len)
{
	//int r = reg - REG_SHADOW_START;
	int r = 0;
	if (r < 0) {
		len += r;
		r = 0;
	}
	if (len <= 0)
		return;
	if (len > NUM_REGS - r)
		len = NUM_REGS - r;

	memcpy(&regs[r], val, len);
}

int RTLSDR::SetIfFreq(uint32_t freq) {
	return 0;
}

int RTLSDR::GetXtalFreq(uint32_t* rtl_freq, uint32_t* tuner_freq) {
#define APPLY_PPM_CORR(val,ppm) (((val) * (1.0 + (ppm) / 1e6)))

    if (rtl_freq)
    { *rtl_freq = (uint32_t) APPLY_PPM_CORR(DEF_RTL_XTAL_FREQ, 0); }

    if (tuner_freq)
    { *tuner_freq = (uint32_t) APPLY_PPM_CORR(DEF_RTL_XTAL_FREQ, 0); }

    return 0;
}
