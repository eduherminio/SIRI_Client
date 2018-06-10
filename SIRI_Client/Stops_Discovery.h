#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_SD_Base
{
public:
	Siri_SD_Base() {}
	virtual ~Siri_SD_Base() {};

protected:
	void show_SD(const ns1__WsStopPointsDiscoveryAnswerStructure& response);
};


class Siri_SD_Main :
	public BaseSIRIClient,
	public Siri_SD_Base
{
public:
	Siri_SD_Main()
		: BaseSIRIClient() {}
	~Siri_SD_Main() {};

	virtual void run() override;
};

class Siri_SD_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SD_Base
{
public:
	Siri_SD_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SD_Error_Unauth() {};

	virtual void run() override;
};
