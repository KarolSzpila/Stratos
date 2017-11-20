/*
 * FlightCotrolView.h
 *
 *  Created on: 29.10.2017
 *      Author: Karol
 */

#ifndef FLIGHTCONTROLVIEW_FLIGHTCOTROLVIEW_H_
#define FLIGHTCONTROLVIEW_FLIGHTCOTROLVIEW_H_

#include <AircraftRecord.h>
#include <string>
#include <list>
#include "DIALOG.h"

class FlightCotrolView
{
public:
	FlightCotrolView();
	void ShowNewAircraft(const AircraftRecord& aircraft);
	//void UpdateAircraft(const Aircraft& aircraft);
	void Update(const std::list<AircraftRecord>& aircrafts);
	void Init();
private:
	int rows;
};

#endif /* FLIGHTCONTROLVIEW_FLIGHTCOTROLVIEW_H_ */
