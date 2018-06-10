#include "stdafx.h"
#include "Stops_Discovery.h"

void Siri_SD_Base::show_SD(const ns1__WsStopPointsDiscoveryAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/stops_discovery.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "StopPoints discovery: " << std::endl << std::endl;

	ofs << "Valid until: " << GSOAP::to_ptime(*response.Answer->ValidUntil) << std::endl;
	int iter = 0;
	for (const auto& stop : response.Answer->AnnotatedStopPointRef)
	{
		ofs << ++iter << '\t';
		ofs << stop->StopName.front()->__item << " ";
		ofs << stop->StopPointRef->__item << " ";
		ofs << std::boolalpha << stop->Monitored << " ";
		ofs << *(stop->Location->Latitude) << " ";
		ofs << *(stop->Location->Longitude) << std::endl;
	}
	ofs << '\n' << GSOAP::to_ptime(response.Answer->ResponseTimestamp) << std::endl;
}

TEST_F(Siri_ClientTestFixture, SD_Main)				{	Siri_SD_Main			SD;		SD.run();	}
TEST_F(Siri_ClientTestFixture, SD_Error_Unauth)		{	Siri_SD_Error_Unauth	SD;		SD.run();	}

void Siri_SD_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsStopPointsDiscoveryStructure request;
	ns1__WsStopPointsDiscoveryAnswerStructure response;

	ns2__StopPointsDiscoveryRequestStructure req;

	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.StopPointsDetailLevel = ns2__StopPointsDetailEnumeration__normal;

	req.AccountId = &user;
	req.AccountKey = &pwd;

	set_proxy(ws);
	ASSERT_EQ(ws.StopPointsDiscovery(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	ASSERT_EQ(response.Answer->ErrorCondition, nullptr);
	ASSERT_FALSE(response.Answer->AnnotatedStopPointRef.empty());
	for (const auto& stop : response.Answer->AnnotatedStopPointRef)
	{
		ASSERT_NE	(stop, nullptr);
		ASSERT_FALSE(stop->StopName.empty());
		for (const auto& name : (stop->StopName))
		{
			ASSERT_NE	(name, nullptr);
			ASSERT_FALSE(name->__item.empty());
		}
		ASSERT_NE	(stop->StopPointRef, nullptr);
		ASSERT_FALSE(stop->StopPointRef->__item.empty());
		ASSERT_NE	(stop->Location, nullptr);
		ASSERT_NE	(stop->Location->Latitude, nullptr);
		ASSERT_NE	(stop->Location->Longitude, nullptr);
		ASSERT_FALSE(stop->Location->Latitude->empty());
		ASSERT_FALSE(stop->Location->Longitude->empty());
	}
	ASSERT_NE	(response.Answer->ValidUntil, nullptr);
	ASSERT_GE	(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->ResponseTimestamp));

	if (siri_debug)
		show_SD(response);
}

void Siri_SD_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsStopPointsDiscoveryStructure request;
	ns1__WsStopPointsDiscoveryAnswerStructure response;

	ns2__StopPointsDiscoveryRequestStructure req;

	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.StopPointsDetailLevel = ns2__StopPointsDetailEnumeration__normal;

	req.AccountId = &unauth_user;
	req.AccountKey = &pwd;

	//set_proxy(ws);
	ASSERT_EQ(ws.StopPointsDiscovery(server_address.c_str(), nullptr, &request, response),0);

	ASSERT_NE(response.Answer->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->ErrorCondition->Description, nullptr);
	ASSERT_EQ(response.Answer->ErrorCondition->Description->__item, unauth_error_text);
	ASSERT_EQ(response.Answer->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
	ASSERT_EQ(*response.Answer->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);

	ASSERT_TRUE(response.Answer->AnnotatedStopPointRef.empty());
}
