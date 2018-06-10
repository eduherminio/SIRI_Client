#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_PT_Base
{
public:
	Siri_PT_Base() {}
	virtual ~Siri_PT_Base() {};

protected:
	void show_PT(const ns1__ProductionTimetableAnswerStructure& response);
};


class Siri_PT_Main :
	public BaseSIRIClient,
	public Siri_PT_Base
{
public:
	Siri_PT_Main()
		: BaseSIRIClient() {}
	~Siri_PT_Main() {};

	virtual void run() override;
};

class Siri_PT_Main_Lines :
	public BaseSIRIClient,
	public Siri_PT_Base
{
public:
	Siri_PT_Main_Lines()
		: BaseSIRIClient() {}
	~Siri_PT_Main_Lines() {};

	virtual void run() override;
};

class Siri_PT_Unknown_Line :
	public BaseSIRIClient,
	public Siri_PT_Base
{
public:
	Siri_PT_Unknown_Line()
		: BaseSIRIClient() {}
	~Siri_PT_Unknown_Line() {};

	virtual void run() override;
};

class Siri_PT_Error_Unauth :
	public BaseSIRIClient,
	public Siri_PT_Base
{
public:
	Siri_PT_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_PT_Error_Unauth() {};

	virtual void run() override;
};

class Siri_PT_Error_Period :
	public BaseSIRIClient,
	public Siri_PT_Base
{
public:
	Siri_PT_Error_Period()
		: BaseSIRIClient() {}
	~Siri_PT_Error_Period() {};

	virtual void run() override;
};

class Siri_PT_Error_ValidityPeriod_Missing :
	public BaseSIRIClient,
	public Siri_PT_Base
{
public:
	Siri_PT_Error_ValidityPeriod_Missing()
		: BaseSIRIClient() {}
	~Siri_PT_Error_ValidityPeriod_Missing() {};

	virtual void run() override;
};
