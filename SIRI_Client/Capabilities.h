#pragma once

#include "BaseSIRIClient.h"
#include "Siri_ClientTestFixture.h"

class Siri_CAP_Base
{
public:
	Siri_CAP_Base() {}
	virtual ~Siri_CAP_Base() {};
};


class Siri_CAP_Main :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_Main()
		: BaseSIRIClient() {}
	~Siri_CAP_Main() {};

	virtual void run() override;
};

class Siri_CAP_MissingParameter_PT :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_MissingParameter_PT()
		: BaseSIRIClient() {}
	~Siri_CAP_MissingParameter_PT() {};

	virtual void run() override;
};

class Siri_CAP_MissingParameter_ST :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_MissingParameter_ST()
		: BaseSIRIClient() {}
	~Siri_CAP_MissingParameter_ST() {};

	virtual void run() override;
};

class Siri_CAP_MissingParameter_SM :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_MissingParameter_SM()
		: BaseSIRIClient() {}
	~Siri_CAP_MissingParameter_SM() {};

	virtual void run() override;
};

class Siri_CAP_MissingParameter_VM :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_MissingParameter_VM()
		: BaseSIRIClient() {}
	~Siri_CAP_MissingParameter_VM() {};

	virtual void run() override;
};

class Siri_CAP_MissingParameter_ET :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_MissingParameter_ET()
		: BaseSIRIClient() {}
	~Siri_CAP_MissingParameter_ET() {};

	virtual void run() override;
};

class Siri_CAP_MissingParameter_FM :
	public BaseSIRIClient,
	public Siri_CAP_Base
{
public:
	Siri_CAP_MissingParameter_FM()
		: BaseSIRIClient() {}
	~Siri_CAP_MissingParameter_FM() {};

	virtual void run() override;
};
