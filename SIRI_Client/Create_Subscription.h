#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_SUB_Base
{
public:
	Siri_SUB_Base() {}
	virtual ~Siri_SUB_Base() {};
};


class Siri_SUB_PT_Main :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_PT_Main()
		: BaseSIRIClient() {}
	~Siri_SUB_PT_Main() {};

	virtual void run() override;
};

class Siri_SUB_PT_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_PT_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SUB_PT_Error_Unauth() {};

	virtual void run() override;
};

class Siri_SUB_PT_Error_SubID_Missing :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_PT_Error_SubID_Missing()
		: BaseSIRIClient() {}
	~Siri_SUB_PT_Error_SubID_Missing() {};

	virtual void run() override;
};

class Siri_SUB_PT_Error_Empty_Line :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_PT_Error_Empty_Line()
		: BaseSIRIClient() {}
	~Siri_SUB_PT_Error_Empty_Line() {};

	virtual void run() override;
};


class Siri_SUB_ST_Main :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ST_Main()
		: BaseSIRIClient() {}
	~Siri_SUB_ST_Main() {};

	virtual void run() override;
};

class Siri_SUB_ST_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ST_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SUB_ST_Error_Unauth() {};

	virtual void run() override;
};

class Siri_SUB_ST_Error_SubID_Missing :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ST_Error_SubID_Missing()
		: BaseSIRIClient() {}
	~Siri_SUB_ST_Error_SubID_Missing() {};

	virtual void run() override;
};

class Siri_SUB_ST_Error_Long_Period :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ST_Error_Long_Period()
		: BaseSIRIClient() {}
	~Siri_SUB_ST_Error_Long_Period() {};

	virtual void run() override;
};


class Siri_SUB_SM_Main :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_SM_Main()
		: BaseSIRIClient() {}
	~Siri_SUB_SM_Main() {};

	virtual void run() override;
};

class Siri_SUB_SM_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_SM_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SUB_SM_Error_Unauth() {};

	virtual void run() override;
};

class Siri_SUB_SM_Error_SubID_Missing :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_SM_Error_SubID_Missing()
		: BaseSIRIClient() {}
	~Siri_SUB_SM_Error_SubID_Missing() {};

	virtual void run() override;
};


class Siri_SUB_VM_Main_Lines :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_VM_Main_Lines()
		: BaseSIRIClient() {}
	~Siri_SUB_VM_Main_Lines() {};

	virtual void run() override;
};

class Siri_SUB_VM_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_VM_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SUB_VM_Error_Unauth() {};

	virtual void run() override;
};

class Siri_SUB_VM_Error_SubID_Missing :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_VM_Error_SubID_Missing()
		: BaseSIRIClient() {}
	~Siri_SUB_VM_Error_SubID_Missing() {};

	virtual void run() override;
};


class Siri_SUB_ET_Main_Lines :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ET_Main_Lines()
		: BaseSIRIClient() {}
	~Siri_SUB_ET_Main_Lines() {};

	virtual void run() override;
};

class Siri_SUB_ET_Error_Unauth :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ET_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_SUB_ET_Error_Unauth() {};

	virtual void run() override;
};

class Siri_SUB_ET_Error_SubID_Missing :
	public BaseSIRIClient,
	public Siri_SUB_Base
{
public:
	Siri_SUB_ET_Error_SubID_Missing()
		: BaseSIRIClient() {}
	~Siri_SUB_ET_Error_SubID_Missing() {};

	virtual void run() override;
};
