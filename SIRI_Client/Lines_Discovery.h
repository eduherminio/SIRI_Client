#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_LD_Base
{
public:
	Siri_LD_Base() {}
	virtual ~Siri_LD_Base() {};

protected:
	void show_LD(const ns1__WsLinesDiscoveryAnswerStructure& response);
};


class Siri_LD_Main :
	public BaseSIRIClient,
	public Siri_LD_Base
{
public:
	Siri_LD_Main()
		: BaseSIRIClient() {}
	~Siri_LD_Main() {};

	virtual void run() override;
};

class Siri_LD_Error_Unauth :
	public BaseSIRIClient,
	public Siri_LD_Base
{
public:
	Siri_LD_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_LD_Error_Unauth() {};

	virtual void run() override;
};

class Siri_LD_Error_Unauth_Missing_User :
	public BaseSIRIClient,
	public Siri_LD_Base
{
public:
	Siri_LD_Error_Unauth_Missing_User()
		: BaseSIRIClient() {}
	~Siri_LD_Error_Unauth_Missing_User() {};

	virtual void run() override;
};
