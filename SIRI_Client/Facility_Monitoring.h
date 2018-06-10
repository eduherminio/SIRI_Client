#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_FM_Base
{
public:
	Siri_FM_Base() {}
	virtual ~Siri_FM_Base() {};

protected:
	void show_FM(const ns1__WsFacilityMonitoringAnswerStructure& response, const _tstring mode);
};


class Siri_FM_All_Lines :
	public BaseSIRIClient,
	public Siri_FM_Base
{
public:
	Siri_FM_All_Lines()
		: BaseSIRIClient() {}
	~Siri_FM_All_Lines() {};

	virtual void run() override;
};

class Siri_FM_All_Stops :
	public BaseSIRIClient,
	public Siri_FM_Base
{
public:
	Siri_FM_All_Stops()
		: BaseSIRIClient() {}
	~Siri_FM_All_Stops() {};

	virtual void run() override;
};

class Siri_FM_Error_Unauth :
	public BaseSIRIClient,
	public Siri_FM_Base
{
public:
	Siri_FM_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_FM_Error_Unauth() {};

	virtual void run() override;
};

class Siri_FM_Error_LineRef_StopPointRef_Missing :
	public BaseSIRIClient,
	public Siri_FM_Base
{
public:
	Siri_FM_Error_LineRef_StopPointRef_Missing()
		: BaseSIRIClient() {}
	~Siri_FM_Error_LineRef_StopPointRef_Missing() {};

	virtual void run() override;
};

class Siri_FM_Wrong_ALL_LINES_CODE :
	public BaseSIRIClient,
	public Siri_FM_Base
{
public:
	Siri_FM_Wrong_ALL_LINES_CODE()
		: BaseSIRIClient() {}
	~Siri_FM_Wrong_ALL_LINES_CODE() {};

	virtual void run() override;
};

class Siri_FM_Wrong_ALL_STOPS_CODE :
	public BaseSIRIClient,
	public Siri_FM_Base
{
public:
	Siri_FM_Wrong_ALL_STOPS_CODE()
		: BaseSIRIClient() {}
	~Siri_FM_Wrong_ALL_STOPS_CODE() {};

	virtual void run() override;
};
