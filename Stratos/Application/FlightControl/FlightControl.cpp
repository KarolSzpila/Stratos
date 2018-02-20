/*
 * FlightControlModel.cpp
 *
 *  Created on: 28.10.2017
 *      Author: Karol
 */

#include "FlightControl.h"


FlightControl::FlightControl()
{
}


bool FlightControl::FindAircraftByICAO_Address(const std::string& ICAO_Address, std::list<AircraftRecord>::iterator& retIt )
{
	bool recordFound = false;
	for(auto it = aircrafts.begin(); it != aircrafts.end(); it++)
	{
		if(it->GetICAO_Address() == ICAO_Address)
		{
			retIt = it;
			recordFound = true;
			break;
		}
	}

	return recordFound;
}

void FlightControl::AddRecord(const AircraftRecord& record)
{
	aircrafts.push_back(record);
}

bool FlightControl::TickAllRecords(uint32_t ticks)
{
	bool anyRecordExpiered = false;
	auto it = aircrafts.begin();
	while(it != aircrafts.end())
	{
		it->Tick(ticks);
		bool recordExpired = it->IsRecordExpiered();
		if(recordExpired == true)
		{
			anyRecordExpiered |= recordExpired;
			auto it2 = it;
			it++;
			aircrafts.erase(it2);
		}
		else
		{
			it->CalcNewPosition(ticks);
			it++;
		}
	}

	return anyRecordExpiered;
}
