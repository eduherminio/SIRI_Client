#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_ST_Base
{
public:
	Siri_ST_Base() {}
	virtual ~Siri_ST_Base() {};

protected:
	void show_ST(const ns1__StopTimetableAnswerStructure& response);
};


class Siri_ST_Main :
	public BaseSIRIClient,
	public Siri_ST_Base
{
public:
	Siri_ST_Main()
		: BaseSIRIClient() {}
	~Siri_ST_Main() {};

	virtual void run() override;
};

class Siri_ST_No_Info :
	public BaseSIRIClient,
	public Siri_ST_Base
{
public:
	Siri_ST_No_Info()
		: BaseSIRIClient() {}
	~Siri_ST_No_Info() {};

	virtual void run() override;
};

class Siri_ST_Error_Unauth :
	public BaseSIRIClient,
	public Siri_ST_Base
{
public:
	Siri_ST_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_ST_Error_Unauth() {};

	virtual void run() override;
};

class Siri_ST_Error_Period :
	public BaseSIRIClient,
	public Siri_ST_Base
{
public:
	Siri_ST_Error_Period()
		: BaseSIRIClient() {}
	~Siri_ST_Error_Period() {};

	virtual void run() override;
};

class Siri_ST_Error_Parameter_Missing :
	public BaseSIRIClient,
	public Siri_ST_Base
{
public:
	Siri_ST_Error_Parameter_Missing()
		: BaseSIRIClient() {}
	~Siri_ST_Error_Parameter_Missing() {};

	virtual void run() override;
};

class Siri_ST_Error_Parameter_null :
	public BaseSIRIClient,
	public Siri_ST_Base
{
public:
	Siri_ST_Error_Parameter_null()
		: BaseSIRIClient() {}
	~Siri_ST_Error_Parameter_null() {};

	virtual void run() override;
};
