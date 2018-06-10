#include "stdafx.h"
#include "Lines_Discovery.h"

void Siri_LD_Base::show_LD(const ns1__WsLinesDiscoveryAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/lines_discovery.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Lines Discovery: " << std::endl << std::endl;

	ofs << "Valid until: " << GSOAP::to_ptime(*response.Answer->ValidUntil) << std::endl;
	int iter = 0;
	for (const auto& line : response.Answer->AnnotatedLineRef)
	{
		ofs << ++iter << '\t';
		ofs << line->LineName.front()->__item << " ";
		ofs << line->LineRef->__item << " ";
		ofs << std::boolalpha << line->Monitored << std::endl;
		for (const auto& destination : line->Destinations->Destination)
		{
			ofs << "\t\t";
			ofs << destination->DestinationRef->__item << " " << destination->PlaceName.front()->__item << std::endl;
		}
	}
	ofs << '\n' << GSOAP::to_ptime(response.Answer->ResponseTimestamp) << std::endl;
}

TEST_F(Siri_ClientTestFixture, LD_Main	)							{	Siri_LD_Main						LD;		LD.run();	}
TEST_F(Siri_ClientTestFixture, LD_Error_Unauth)						{	Siri_LD_Error_Unauth				LD;		LD.run();	}
TEST_F(Siri_ClientTestFixture, LD_Error_Unauth_Missing_User)		{	Siri_LD_Error_Unauth_Missing_User	LD;		LD.run();	}

void Siri_LD_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsLinesDiscoveryStructure request;
	ns1__WsLinesDiscoveryAnswerStructure response;

	ns2__LinesDiscoveryRequestStructure req;
	request.Request = &req;
	req.Language = desired_language;	// en by default
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.LinesDetailLevel = ns2__LinesDetailEnumeration__normal;

	req.AccountId = &user;
	req.AccountKey = &pwd;

	set_proxy(ws);
	ASSERT_EQ(ws.LinesDiscovery(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	ASSERT_EQ	(response.Answer->ErrorCondition, nullptr);
	ASSERT_FALSE(response.Answer->AnnotatedLineRef.empty());
	for (const auto& line : response.Answer->AnnotatedLineRef)
	{
		ASSERT_NE	(line, nullptr);
		ASSERT_NE	(line->LineRef, nullptr);
		ASSERT_FALSE(line->LineRef->__item.empty());
		ASSERT_NE	(line->Destinations, nullptr);
		ASSERT_FALSE(line->Destinations->Destination.empty());
		for (const auto& destination : line->Destinations->Destination)
		{
			ASSERT_NE	(destination->DestinationRef, nullptr);
			ASSERT_FALSE(destination->DestinationRef->__item.empty());
			ASSERT_FALSE(destination->PlaceName.empty());
			ASSERT_FALSE(destination->PlaceName.front()->__item.empty());
		}

		ASSERT_FALSE(line->LineName.empty());
		for (const auto& name : line->LineName)
		{
			ASSERT_NE	(name, nullptr);
			ASSERT_FALSE(name->__item.empty());
		}
		ASSERT_NE	(response.Answer->ValidUntil, nullptr);
		ASSERT_GE	(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->ResponseTimestamp));
	}

	if(siri_debug)
		show_LD(response);
}

void Siri_LD_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsLinesDiscoveryStructure request;
	ns1__WsLinesDiscoveryAnswerStructure response;

	ns2__LinesDiscoveryRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.LinesDetailLevel = ns2__LinesDetailEnumeration__normal;

	//req.AccountId = &unauth_user;
	req.AccountKey = &pwd;

	//set_proxy(ws);
	ASSERT_EQ(ws.LinesDiscovery(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	ASSERT_NE(response.Answer->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->ErrorCondition->Description, nullptr);
	ASSERT_EQ(response.Answer->ErrorCondition->Description->__item, unauth_error_text);
	ASSERT_EQ(response.Answer->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
	ASSERT_EQ(*response.Answer->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);

	ASSERT_TRUE(response.Answer->AnnotatedLineRef.empty());
}

void Siri_LD_Error_Unauth_Missing_User::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsLinesDiscoveryStructure request;
	ns1__WsLinesDiscoveryAnswerStructure response;

	ns2__LinesDiscoveryRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.LinesDetailLevel = ns2__LinesDetailEnumeration__normal;

	req.AccountId = &unauth_user;
	req.AccountKey = &pwd;

	//set_proxy(ws);
	ASSERT_EQ(ws.LinesDiscovery(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	ASSERT_NE(response.Answer->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->ErrorCondition->Description, nullptr);
	ASSERT_EQ(response.Answer->ErrorCondition->Description->__item, unauth_error_text);
	ASSERT_EQ(response.Answer->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
	ASSERT_EQ(*response.Answer->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	
	ASSERT_TRUE(response.Answer->AnnotatedLineRef.empty());
}
