#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_TSUB_Base
{
public:
	Siri_TSUB_Base() {}
	virtual ~Siri_TSUB_Base() {};
};


class Siri_TSUB_One :
	public BaseSIRIClient,
	public Siri_TSUB_Base
{
public:
	Siri_TSUB_One()
		: BaseSIRIClient() {}
	~Siri_TSUB_One() {};

	virtual void run() override;
};

class Siri_TSUB_All :
	public BaseSIRIClient,
	public Siri_TSUB_Base
{
public:
	Siri_TSUB_All()
		: BaseSIRIClient() {}
	~Siri_TSUB_All() {};

	virtual void run() override;
};

class Siri_TSUB_Error_Unauth :
	public BaseSIRIClient,
	public Siri_TSUB_Base
{
public:
	Siri_TSUB_Error_Unauth()
		: BaseSIRIClient() {}
	~Siri_TSUB_Error_Unauth() {};

	virtual void run() override;
};

class Siri_TSUB_Error_SubID_Missing :
	public BaseSIRIClient,
	public Siri_TSUB_Base
{
public:
	Siri_TSUB_Error_SubID_Missing()
		: BaseSIRIClient() {}
	~Siri_TSUB_Error_SubID_Missing() {};

	virtual void run() override;
};
