#pragma once

#include <gtest\gtest.h>

class Siri_ClientTestFixture :
	public ::testing::Test
{
protected:
	Siri_ClientTestFixture();
	virtual ~Siri_ClientTestFixture();
	
	virtual void SetUp();
	virtual void TearDown();
};
