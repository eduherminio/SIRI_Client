#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_ET_Base
{
public:
	Siri_ET_Base() {}
	virtual ~Siri_ET_Base() {};

protected:
	void show_ET(const ns1__WsEstimatedTimetableAnswerStructure& response);
};


class Siri_ET_Main :
	public BaseSIRIClient,
	public Siri_ET_Base
{
public:
	Siri_ET_Main()
		: BaseSIRIClient() {}
	~Siri_ET_Main() {};

	virtual void run() override;
};

class Siri_ET_Main_Lines :
	public BaseSIRIClient,
	public Siri_ET_Base
{
public:
	Siri_ET_Main_Lines()
		: BaseSIRIClient() {}
	~Siri_ET_Main_Lines() {};

	virtual void run() override;
};

class Siri_ET_Unknown_Lines :
	public BaseSIRIClient,
	public Siri_ET_Base
{
public:
	Siri_ET_Unknown_Lines()
		: BaseSIRIClient() {}
	~Siri_ET_Unknown_Lines() {};

	virtual void run() override;
};

class Siri_ET_Preview_0:
	public BaseSIRIClient,
	public Siri_ET_Base
{
public:
	Siri_ET_Preview_0()
		: BaseSIRIClient() {}
	~Siri_ET_Preview_0() {};

	virtual void run() override;
};

class Siri_ET_Error_Unauth :
	public BaseSIRIClient,
	public Siri_ET_Base
{
public:
	Siri_ET_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_ET_Error_Unauth() {};

	virtual void run() override;
};
