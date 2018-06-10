#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_SM_Base
{
public:
	Siri_SM_Base() {}
	virtual ~Siri_SM_Base() {};

protected:
	void show_SM(const ns1__StopMonitoringAnswerStructure& response);
};


class Siri_SM_Main :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Main()
		: BaseSIRIClient() {}
	~Siri_SM_Main() {};

	virtual void run() override;
};

class Siri_SM_Start_Time_Missing :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Start_Time_Missing()
		: BaseSIRIClient() {}
	~Siri_SM_Start_Time_Missing() {};

	virtual void run() override;
};

class Siri_SM_Unknown_Stop :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Unknown_Stop()
		: BaseSIRIClient() {}
	~Siri_SM_Unknown_Stop() {};

	virtual void run() override;
};

class Siri_SM_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SM_Error_Unauth() {};

	virtual void run() override;
};

class Siri_SM_Error_Context_Missing :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Error_Context_Missing()
		: BaseSIRIClient() {}
	~Siri_SM_Error_Context_Missing() {};

	virtual void run() override;
};

class Siri_SM_Error_Period :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Error_Period()
		: BaseSIRIClient() {}
	~Siri_SM_Error_Period() {};

	virtual void run() override;
};

class Siri_SM_Error_Parameter_Missing :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Error_Parameter_Missing()
		: BaseSIRIClient() {}
	~Siri_SM_Error_Parameter_Missing() {};

	virtual void run() override;
};

class Siri_SM_Error_Parameter_null :
	public BaseSIRIClient,
	public Siri_SM_Base
{
public:
	Siri_SM_Error_Parameter_null()
		: BaseSIRIClient() {}
	~Siri_SM_Error_Parameter_null() {};

	virtual void run() override;
};
