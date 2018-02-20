/*
 * Airplaine.h
 *
 *  Created on: 29.10.2017
 *      Author: Karol
 */

#ifndef FLIGHTCONTROL_AIRCRAFTRECORD_H_
#define FLIGHTCONTROL_AIRCRAFTRECORD_H_

#include "string"
#include "ADSBMessage.h"

#define homeLat 51.253811F
#define homeLon 15.395468F
#define dormLat 51.109402F
#define dormLon 17.059798F

#define DEFAULT_LIVE_SPAN 120U //120s
#define latRef dormLat
#define lonRef dormLon

class AircraftRecord
{
public:
	AircraftRecord() = default;
	AircraftRecord(const std::string& ICAO_Address);

	const std::string& GetICAO_Address() const {return ICAO_Address;}
	const std::string& GetFlightName() const {return flightName;}
	bool IsRecordExpiered() const {return recordExpiered;}

	void SetAltitude(const uint32_t& newAltitude);
	void SetFlightName(const char* newFlightName);
	void SetVelocityAndHeading(const int& velocity, const float& heading);

	const float& GetHeading() const {return heading;}
	const float& GetLat() const { return latitude;}
	const float& GetLon() const { return lognitude;}
	uint32_t altitude;

	std::string altStr;
	std::string headStr;
	std::string velocityStr;
	std::string positionStr;

	bool flightNameKnown;
	bool altitudeKnown;
	bool velocityAndHeadingKnown;
	void Tick(uint32_t ticks);

	void decodeCPR(const int& fflag, const int& cprLat, const int& cprLon);

	void CalcNewPosition(int time);
private:
	std::string ICAO_Address;

	std::string flightName;



	uint32_t ticksToExpire;
	bool recordExpiered;

	float latitude;
	float lognitude;
	float heading;
	int velocity;
	int cprNLFunction(double lat);
	int cprNFunction(double lat, int isodd);
	float CprMod(const float& x, const float& y);
	double cprDlonFunction(double lat, int isodd);
};

#endif /* FLIGHTCONTROL_AIRCRAFTRECORD_H_ */
