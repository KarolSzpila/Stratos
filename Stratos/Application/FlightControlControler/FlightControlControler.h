/*
 * FlightControlControler.h
 *
 *  Created on: 30.10.2017
 *      Author: Karol
 */

#ifndef FLIGHTCONTROLCONTROLER_FLIGHTCONTROLCONTROLER_H_
#define FLIGHTCONTROLCONTROLER_FLIGHTCONTROLCONTROLER_H_

#include "FlightCotrolView.h"
#include "ADSBMessage.h"
#include "FlightControl.h"

enum ModeSMessage
{
	DF0 = 0,
	DF4 = 4,
	DF11 = 11,
	DF17 = 17
};
class FlightControlControler
{
public:
	FlightControlControler(FlightControl& model,FlightCotrolView& view);

	void PassNewMessage(const ADS_BMessage& msg);
	std::string GetICAO_AddresAsString(const ADS_BMessage& msg);

	void UpdateRecord(const ADS_BMessage& msg, AircraftRecord& record);

	void UpdateTicksCount(uint32_t ticks);
	void UpdateView();
private:

	FlightControl& model;
	FlightCotrolView& view;

	bool modelChanged;

};

#endif /* FLIGHTCONTROLCONTROLER_FLIGHTCONTROLCONTROLER_H_ */
