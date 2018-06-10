#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_VM_Base
{
public:
	Siri_VM_Base() {}
	virtual ~Siri_VM_Base() {};

protected:
	void show_VM(const ns1__VehicleMonitoringAnswerStructure& response);
};


class Siri_VM_Main_Line :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Main_Line()
		: BaseSIRIClient() {}
	~Siri_VM_Main_Line() {};

	virtual void run() override;
};

class Siri_VM_Main_Line_Direction :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Main_Line_Direction()
		: BaseSIRIClient() {}
	~Siri_VM_Main_Line_Direction() {};

	virtual void run() override;
};

class Siri_VM_Main_Vehicle :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Main_Vehicle()
		: BaseSIRIClient() {}
	~Siri_VM_Main_Vehicle() {};

	virtual void run() override;
};

class Siri_VM_Main_Vehicle_Direction :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Main_Vehicle_Direction()
		: BaseSIRIClient() {}
	~Siri_VM_Main_Vehicle_Direction() {};

	virtual void run() override;
};

class Siri_VM_Error_Unknown_Vehicle :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Error_Unknown_Vehicle()
		: BaseSIRIClient() {}
	~Siri_VM_Error_Unknown_Vehicle() {};

	virtual void run() override;
};

class Siri_VM_Error_Unknown_Line :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Error_Unknown_Line()
		: BaseSIRIClient() {}
	~Siri_VM_Error_Unknown_Line() {};

	virtual void run() override;
};

class Siri_VM_Error_Unauth :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_VM_Error_Unauth() {};

	virtual void run() override;
};

class Siri_VM_Error_Context_Missing :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Error_Context_Missing()
		: BaseSIRIClient() {}
	~Siri_VM_Error_Context_Missing() {};

	virtual void run() override;
};

class Siri_VM_Error_Parameter_Missing :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Error_Parameter_Missing()
		: BaseSIRIClient() {}
	~Siri_VM_Error_Parameter_Missing() {};

	virtual void run() override;
};

class Siri_VM_Error_Parameter_null :
	public BaseSIRIClient,
	public Siri_VM_Base
{
public:
	Siri_VM_Error_Parameter_null()
		: BaseSIRIClient() {}
	~Siri_VM_Error_Parameter_null() {};

	virtual void run() override;
};
