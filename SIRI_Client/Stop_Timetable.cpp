#include "stdafx.h"
#include "Stop_Timetable.h"

void Siri_ST_Base::show_ST(const ns1__StopTimetableAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/stoptimetable.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Stop Timetable: " << std::endl << std::endl;

	ofs << "Valid until: " << GSOAP::to_ptime(*response.Answer->StopTimetableDelivery->ValidUntil) << std::endl;
	for (const auto& stopvisit : response.Answer->StopTimetableDelivery->TimetabledStopVisit)
	{
		ofs << std::endl;
		ofs << "MonitoringRef: " << stopvisit->MonitoringRef->__item << ", ";
		ofs << "LineRef: " << stopvisit->TargetedVehicleJourney->LineRef->__item << ", ";
		ofs << "DirectionRef: " << stopvisit->TargetedVehicleJourney->DirectionRef->__item << std::endl;

		ofs << *stopvisit->TargetedVehicleJourney->TargetedCall->Order << ". ";

		if (stopvisit->TargetedVehicleJourney->__TargetedVehicleJourneyStructure_sequence__ &&
			true == stopvisit->TargetedVehicleJourney->__TargetedVehicleJourneyStructure_sequence__->HeadwayService)
		{
			ofs << stopvisit->TargetedVehicleJourney->TargetedCall->StopPointRef->__item << " - frequency: ";
			ofs << *stopvisit->TargetedVehicleJourney->TargetedCall->AimedHeadwayInterval / (1000 * 60) << " minutes" << std::endl;
		}
		else
		{
			ofs << stopvisit->TargetedVehicleJourney->TargetedCall->StopPointRef->__item << " -  from: ";
			ofs << GSOAP::to_ptime(*stopvisit->TargetedVehicleJourney->TargetedCall->AimedArrivalTime) << " to: ";
			ofs << GSOAP::to_ptime(*stopvisit->TargetedVehicleJourney->TargetedCall->AimedDepartureTime) << std::endl;
		}
	}

	for (const auto& stopcancellation : response.Answer->StopTimetableDelivery->TimetabledStopVisitCancellation)
	{

	}

	ofs << "\nResponse time: " << GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp);
}

TEST_F(Siri_ClientTestFixture, ST_Main)						{	Siri_ST_Main					ST;		ST.run();	}
TEST_F(Siri_ClientTestFixture, ST_No_Info)						{	Siri_ST_No_Info					ST;		ST.run();	}
TEST_F(Siri_ClientTestFixture, ST_Error_Unauth)				{	Siri_ST_Error_Unauth			ST;		ST.run();	}
TEST_F(Siri_ClientTestFixture, ST_Error_Period)				{	Siri_ST_Error_Period			ST;		ST.run();	}
TEST_F(Siri_ClientTestFixture, ST_Error_Parameter_Missing)		{	Siri_ST_Error_Parameter_Missing ST;		ST.run();	}
TEST_F(Siri_ClientTestFixture, ST_Error_Parameter_null)		{	Siri_ST_Error_Parameter_null	ST;		ST.run();	}

void Siri_ST_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopTimetableRequestStructure request;
	ns1__StopTimetableAnswerStructure response;

	ns2__StopTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
		xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
		xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
		timestamp_structure->StartTime = start_time;
		timestamp_structure->EndTime = end_time;
	req.DepartureWindow = timestamp_structure;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = get_random_stop().stopref;
	req.MonitoringRef = monitoringref_structure;

	ns2__LineRefStructure* lineref_structure = new ns2__LineRefStructure;
		lineref_structure->__item = get_random_line().lineref;
	req.LineRef = lineref_structure;

	const boost::posix_time::ptime ptime_start_time = GSOAP::to_ptime(req.DepartureWindow->StartTime);
	const boost::posix_time::ptime ptime_end_time = GSOAP::to_ptime(req.DepartureWindow->EndTime);

	set_proxy(ws);
	ASSERT_EQ(ws.GetStopTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete timestamp_structure;
	delete monitoringref_structure;
	delete lineref_structure;


	ASSERT_NE(response.Answer->StopTimetableDelivery, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition, nullptr);
	//ASSERT_LE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(*response.Answer->StopTimetableDelivery->ValidUntil));
	
	ASSERT_FALSE(response.Answer->StopTimetableDelivery->TimetabledStopVisit.empty());
	for (const auto& stopvisit : response.Answer->StopTimetableDelivery->TimetabledStopVisit)
	{
		ASSERT_NE	(stopvisit, nullptr);
		ASSERT_NE	(stopvisit->MonitoringRef, nullptr);
		ASSERT_FALSE(stopvisit->MonitoringRef->__item.empty());
		ASSERT_NE	(stopvisit->TargetedVehicleJourney, nullptr);
		ASSERT_NE	(stopvisit->TargetedVehicleJourney->LineRef, nullptr);
		ASSERT_FALSE(stopvisit->TargetedVehicleJourney->LineRef->__item.empty());
		ASSERT_NE	(stopvisit->TargetedVehicleJourney->DirectionRef, nullptr);
		ASSERT_FALSE(stopvisit->TargetedVehicleJourney->DirectionRef->__item.empty());
		ASSERT_NE	(stopvisit->TargetedVehicleJourney->TargetedCall, nullptr);
		ASSERT_NE	(stopvisit->TargetedVehicleJourney->TargetedCall->Order, nullptr);
		ASSERT_FALSE(stopvisit->TargetedVehicleJourney->TargetedCall->Order->empty());

		if (stopvisit->TargetedVehicleJourney->__TargetedVehicleJourneyStructure_sequence__ &&
			true== stopvisit->TargetedVehicleJourney->__TargetedVehicleJourneyStructure_sequence__->HeadwayService)
		{
			ASSERT_EQ(stopvisit->TargetedVehicleJourney->TargetedCall->AimedArrivalTime, nullptr);
			ASSERT_EQ(stopvisit->TargetedVehicleJourney->TargetedCall->AimedDepartureTime, nullptr);
			ASSERT_NE(stopvisit->TargetedVehicleJourney->TargetedCall->AimedHeadwayInterval, nullptr);
			ASSERT_GT(*stopvisit->TargetedVehicleJourney->TargetedCall->AimedHeadwayInterval, 0);
		}
		else
		{
			ASSERT_NE(stopvisit->TargetedVehicleJourney->TargetedCall->AimedArrivalTime, nullptr);
			ASSERT_NE(stopvisit->TargetedVehicleJourney->TargetedCall->AimedDepartureTime, nullptr);
			ASSERT_EQ(stopvisit->TargetedVehicleJourney->TargetedCall->AimedHeadwayInterval, nullptr);
			ASSERT_LE(GSOAP::to_ptime(*stopvisit->TargetedVehicleJourney->TargetedCall->AimedDepartureTime), ptime_end_time);
			ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*stopvisit->TargetedVehicleJourney->TargetedCall->AimedArrivalTime));
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp));
	
	if (siri_debug)
		show_ST(response);
}

void Siri_ST_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopTimetableRequestStructure request;
	ns1__StopTimetableAnswerStructure response;

	ns2__StopTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &unauth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
		xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
		xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
		timestamp_structure->StartTime = start_time;
		timestamp_structure->EndTime = end_time;
	req.DepartureWindow = timestamp_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete timestamp_structure;


	ASSERT_NE(response.Answer->StopTimetableDelivery, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->Description, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition->Description->__item, unauth_error_text);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
	ASSERT_EQ(*response.Answer->StopTimetableDelivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);

	ASSERT_TRUE(response.Answer->StopTimetableDelivery->TimetabledStopVisit.empty());
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp));
}

void Siri_ST_Error_Period::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopTimetableRequestStructure request;
	ns1__StopTimetableAnswerStructure response;

	ns2__StopTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
		xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
		xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(too_many_days));
		timestamp_structure->StartTime = start_time;
		timestamp_structure->EndTime = end_time;
	req.DepartureWindow = timestamp_structure;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = get_random_stop().stopref;
	req.MonitoringRef = monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete timestamp_structure;
	delete monitoringref_structure;


	ASSERT_NE(response.Answer->StopTimetableDelivery, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->Description, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition->Description->__item, too_many_days_error_text);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____CapabilityNotSupportedError);
	ASSERT_EQ(*response.Answer->StopTimetableDelivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.CapabilityNotSupportedError->ErrorText, too_many_days_error_text);

	ASSERT_TRUE(response.Answer->StopTimetableDelivery->TimetabledStopVisit.empty());
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp));
}

void Siri_ST_Error_Parameter_Missing::run() {
	SiriProducerDocBindingProxy ws;

	ns1__StopTimetableRequestStructure request;
	ns1__StopTimetableAnswerStructure response;

	ns2__StopTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_NE(response.Answer->StopTimetableDelivery, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->Description, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"MonitoringRef")), std::wstring::npos);

	ASSERT_TRUE(response.Answer->StopTimetableDelivery->TimetabledStopVisit.empty());
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp));
}

void Siri_ST_Error_Parameter_null::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopTimetableRequestStructure request;
	ns1__StopTimetableAnswerStructure response;

	ns2__StopTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
		xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
		xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
		timestamp_structure->StartTime = start_time;
		timestamp_structure->EndTime = end_time;
	req.DepartureWindow = timestamp_structure;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		//monitoringref_structure->__item = non_existing_stop;
	req.MonitoringRef = monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete timestamp_structure;
	delete monitoringref_structure;


	ASSERT_NE(response.Answer->StopTimetableDelivery, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->Description, nullptr);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
	ASSERT_NE(response.Answer->StopTimetableDelivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"MonitoringRef")), std::wstring::npos);

	ASSERT_TRUE(response.Answer->StopTimetableDelivery->TimetabledStopVisit.empty());

	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp));
}

void Siri_ST_No_Info::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopTimetableRequestStructure request;
	ns1__StopTimetableAnswerStructure response;

	ns2__StopTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
		xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
		xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
		timestamp_structure->StartTime = start_time;
		timestamp_structure->EndTime = end_time;
	req.DepartureWindow = timestamp_structure;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = non_existing_stop;
	req.MonitoringRef = monitoringref_structure;

	ns2__LineRefStructure* lineref_structure = new ns2__LineRefStructure;
		lineref_structure->__item = get_random_line().lineref;
	req.LineRef = lineref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete timestamp_structure;
	delete monitoringref_structure;
	delete lineref_structure;


	ASSERT_NE(response.Answer->StopTimetableDelivery, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableDelivery->ErrorCondition, nullptr);
	//ASSERT_LE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(*response.Answer->StopTimetableDelivery->ValidUntil));
	
	//ASSERT_TRUE(response.Answer->StopTimetableDelivery->TimetabledStopVisit.empty());			// No valid for Headway
	for (const auto& stopvisit : response.Answer->StopTimetableDelivery->TimetabledStopVisit)	// Workaround
	{
		ASSERT_NE(stopvisit->TargetedVehicleJourney, nullptr);
		ASSERT_NE(stopvisit->TargetedVehicleJourney->__TargetedVehicleJourneyStructure_sequence__, nullptr);
		ASSERT_EQ(stopvisit->TargetedVehicleJourney->__TargetedVehicleJourneyStructure_sequence__->HeadwayService, true);
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.Answer->StopTimetableDelivery->ResponseTimestamp));
}
