#pragma once

#include "variables_globales.h"

class CLines
{
public:
	CLines(std::wstring _lineref)
		:lineref(_lineref)	{}
	std::wstring lineref;
};

class CStops
{
public:
	CStops(std::wstring _stopref)
		:stopref(_stopref) {}
	std::wstring stopref;
};

class BaseSIRIClient
{
public:
	BaseSIRIClient();
	~BaseSIRIClient();

	virtual void run() = 0; 
protected:
	const CLines get_random_line();
	const CStops get_random_stop();
	void set_proxy(SiriProducerDocBindingProxy& srv);

	std::vector<std::wstring>			v_subscription_references;		// v_subscription_uuids
	boost::uuids::random_generator		random_uuid_generator;			// Subscription uuids	- re-using the generator avoids uuid conflicts
	std::uniform_int_distribution<int>	int_distribution;				// Subscriber	ids
	std::mt19937						random_generator;
private:
	void load_available_lines();
	void load_available_stops();
	void load_v_subs();
	void save_v_subs();

	boost::filesystem::path v_sub_path;

	std::vector<CLines>		available_lines;
	std::vector<CStops>		available_stops;
};
