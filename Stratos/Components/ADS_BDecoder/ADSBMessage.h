/*
 * ADSBMessage.h
 *
 *  Created on: 21.10.2017
 *      Author: Karol
 */

#ifndef ADS_BDECODER_ADSBMESSAGE_H_
#define ADS_BDECODER_ADSBMESSAGE_H_

#define MODES_LONG_MSG_BITS 112
#define MODES_SHORT_MSG_BITS 56
#define MODES_FULL_LEN (MODES_PREAMBLE_US+MODES_LONG_MSG_BITS)
#define MODES_LONG_MSG_BYTES (112/8)
#define MODES_SHORT_MSG_BYTES (56/8)

struct ADS_BMessage
{
	 /* Generic fields */
	    unsigned char msg[MODES_LONG_MSG_BYTES]; /* Binary message. */
	    int msgbits;                /* Number of bits in message */
	    int msgtype;                /* Downlink format # */
	    int crcok;                  /* True if CRC was valid */
	    uint32_t crc;               /* Message CRC */
	    int errorbit;               /* Bit corrected. -1 if no bit corrected. */
	    int aa1, aa2, aa3;          /* ICAO Address bytes 1 2 and 3 */
	    int phase_corrected;        /* True if phase correction was applied. */

	    /* DF 11 */
	    int ca;                     /* Responder capabilities. */

	    /* DF 17 */
	    int metype;                 /* Extended squitter message type. */
	    int mesub;                  /* Extended squitter message subtype. */
	    int heading_is_valid;
	    float heading;
	    int aircraft_type;
	    int fflag;                  /* 1 = Odd, 0 = Even CPR message. */
	    int tflag;                  /* UTC synchronized? */
	    int raw_latitude;           /* Non decoded latitude */
	    int raw_longitude;          /* Non decoded longitude */
	    char flight[9];             /* 8 chars flight number. */
	    int ew_dir;                 /* 0 = East, 1 = West. */
	    int ew_velocity;            /* E/W velocity. */
	    int ns_dir;                 /* 0 = North, 1 = South. */
	    int ns_velocity;            /* N/S velocity. */
	    int vert_rate_source;       /* Vertical rate source. */
	    int vert_rate_sign;         /* Vertical rate sign. */
	    int vert_rate;              /* Vertical rate. */
	    int velocity;               /* Computed from EW and NS velocity. */

	    /* DF4, DF5, DF20, DF21 */
	    int fs;                     /* Flight status for DF4,5,20,21 */
	    int dr;                     /* Request extraction of downlink request. */
	    int um;                     /* Request extraction of downlink request. */
	    int identity;               /* 13 bits identity (Squawk). */

	    /* Fields used by multiple message types. */
	    int altitude, unit;
};

#endif /* ADS_BDECODER_ADSBMESSAGE_H_ */
