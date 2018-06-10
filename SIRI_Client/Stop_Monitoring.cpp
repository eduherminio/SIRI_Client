#include "stdafx.h"
#include "Stop_Monitoring.h"

void Siri_SM_Base::show_SM(const ns1__StopMonitoringAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/stopmonitoring.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Stop Monitoring: " << std::endl << std::endl;

	unsigned cont = 0;
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ofs << "Delivery #" << ++cont << std::endl;
		ofs << "Valid until: " << GSOAP::to_ptime(*delivery->ValidUntil) << std::endl;
		for (const auto& stopvisit : delivery->MonitoredStopVisit)
		{
			ofs << std::endl;
			ofs << "MonitoringRef: "		<< stopvisit->__MonitoredStopVisitStructure_sequence->MonitoringRef->__item	<< ", ";
			ofs << "LineRef: "				<< stopvisit->MonitoredVehicleJourney->LineRef->__item						<< ", ";
			ofs << "DirectionRef: "			<< stopvisit->MonitoredVehicleJourney->DirectionRef->__item					<< ", ";
			ofs << "VehicleJourneyName: "	<< stopvisit->MonitoredVehicleJourney->VehicleJourneyName.front()->__item	<< std::endl;

			ofs << *stopvisit->MonitoredVehicleJourney->MonitoredCall->Order								<< ". ";
			ofs << stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointName.front()->__item			<< " - ";
			if (stopvisit->MonitoredVehicleJourney->__MonitoredVehicleJourneyStructure_sequence__ && stopvisit->MonitoredVehicleJourney->__MonitoredVehicleJourneyStructure_sequence__->HeadwayService==true)
			{
				ofs << stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointRef->__item << " - frequency: ";
				ofs << *stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval / (1000 * 60) << " minutes" << std::endl;
			}
			else
			{
				ofs << stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointRef->__item << " -  from: ";
				ofs << GSOAP::to_ptime(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime) << " to: ";
				ofs << GSOAP::to_ptime(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime) << std::endl;
			}

			boost::posix_time::time_duration delay= boost::posix_time::milliseconds(*stopvisit->MonitoredVehicleJourney->Delay);
			ofs << "delay: " << delay << std::endl;
		}

		for (const auto& stopcancellation : delivery->MonitoredStopVisitCancellation)
		{

		}
		ofs << "\nResponse time: " << GSOAP::to_ptime(delivery->ResponseTimestamp);
	}
}

TEST_F(Siri_ClientTestFixture, SM_Main)							{	Siri_SM_Main					SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Start_Time_Missing)			{	Siri_SM_Start_Time_Missing		SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Unknown_Stop)					{	Siri_SM_Unknown_Stop			SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Error_Unauth)					{	Siri_SM_Error_Unauth			SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Error_Context_Missing)		{	Siri_SM_Error_Context_Missing	SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Error_Period)					{	Siri_SM_Error_Period			SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Error_Parameter_Missing)		{	Siri_SM_Error_Parameter_Missing SM;		SM.run();	}
TEST_F(Siri_ClientTestFixture, SM_Error_Parameter_null)			{	Siri_SM_Error_Parameter_null	SM;		SM.run();	}

void Siri_SM_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

	xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(desired_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	req.PreviewInterval = new LONG64(ms.count());

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = get_random_stop().stopref;
	req.MonitoringRef = monitoringref_structure;

	const boost::posix_time::ptime ptime_start_time = GSOAP::to_ptime(*req.StartTime);		// >= ptime_start_time of the period of the data we get, but no guarateed to be =
	const boost::posix_time::millisec preview_interval((int64_t)*req.PreviewInterval);
	//const boost::posix_time::ptime ptime_end_time = ptime_start_time + preview_interval;	// Not guarenteed to be equal to the ValidUntil date of the data we get

	set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete monitoringref_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		//ASSERT_EQ(ptime_end_time, GSOAP::to_ptime(*delivery->ValidUntil));	// It's possible that delivery->ValidUntil > requested_valid_until due to a request with a StartTime in the past, 
		ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*delivery->ValidUntil) - preview_interval);			// since PreviewInterval keeps its value

		ASSERT_FALSE(delivery->MonitoredStopVisit.empty());
		for (const auto& stopvisit : delivery->MonitoredStopVisit)
		{
			ASSERT_NE(stopvisit, nullptr);
			ASSERT_NE(stopvisit->__MonitoredStopVisitStructure_sequence, nullptr);
			ASSERT_NE(stopvisit->__MonitoredStopVisitStructure_sequence->MonitoringRef, nullptr);
			ASSERT_FALSE(stopvisit->__MonitoredStopVisitStructure_sequence->MonitoringRef->__item.empty());
			ASSERT_NE(stopvisit->MonitoredVehicleJourney, nullptr);
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->DirectionRef, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->DirectionRef->__item.empty());
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->LineRef, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->LineRef->__item.empty());
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->VehicleJourneyName.empty());
			for (const auto& name : stopvisit->MonitoredVehicleJourney->VehicleJourneyName)
			{
				ASSERT_NE(name, nullptr);
				ASSERT_FALSE(name->__item.empty());
			}
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->Delay, nullptr);

			ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall, nullptr);
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->Order, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->MonitoredCall->Order->empty());
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointName.empty());
			for (const auto& name : stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointName)
			{
				ASSERT_NE(name, nullptr);
				ASSERT_FALSE(name->__item.empty());
			}

			ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointRef, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointRef->__item.empty());

			if (stopvisit->MonitoredVehicleJourney->__MonitoredVehicleJourneyStructure_sequence__ && stopvisit->MonitoredVehicleJourney->__MonitoredVehicleJourneyStructure_sequence__->HeadwayService == true)
			{
				ASSERT_EQ(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime, nullptr);
				ASSERT_EQ(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime, nullptr);
				ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval, nullptr);
				ASSERT_GT(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval, 0);
			}
			else
			{
				ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime, nullptr);
				ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime, nullptr);
				ASSERT_EQ(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval, nullptr);
				ASSERT_LE(GSOAP::to_ptime(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime), GSOAP::to_ptime(*delivery->ValidUntil));
				//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime));
			}
		}
		ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(delivery->ResponseTimestamp));
	}
	
	if (siri_debug)
		show_SM(response);
}

void Siri_SM_Start_Time_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

	xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	//req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(desired_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	//req.PreviewInterval = new LONG64(ms.count());

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = get_random_stop().stopref;
	req.MonitoringRef = monitoringref_structure;

	//const boost::posix_time::ptime ptime_start_time = GSOAP::to_ptime(*req.StartTime);		// >= ptime_start_time of the period of the data we get, but no guaranteed to be =
	//const boost::posix_time::millisec preview_interval((int64_t)*req.PreviewInterval);
	//const boost::posix_time::ptime ptime_end_time = ptime_start_time + preview_interval;	// Not guarenteed to be equal to the ValidUntil date of the data we get

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete monitoringref_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		//ASSERT_EQ(ptime_end_time, GSOAP::to_ptime(*delivery->ValidUntil));	// It's possible that delivery->ValidUntil > requested_valid_until due to a request with a StartTime in the past, 
		//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*delivery->ValidUntil) - preview_interval);			// since PreviewInterval keeps its value

		ASSERT_FALSE(delivery->MonitoredStopVisit.empty());
		for (const auto& stopvisit : delivery->MonitoredStopVisit)
		{
			ASSERT_NE(stopvisit, nullptr);
			ASSERT_NE(stopvisit->__MonitoredStopVisitStructure_sequence, nullptr);
			ASSERT_NE(stopvisit->__MonitoredStopVisitStructure_sequence->MonitoringRef, nullptr);
			ASSERT_FALSE(stopvisit->__MonitoredStopVisitStructure_sequence->MonitoringRef->__item.empty());
			ASSERT_NE(stopvisit->MonitoredVehicleJourney, nullptr);
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->DirectionRef, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->DirectionRef->__item.empty());
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->LineRef, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->LineRef->__item.empty());
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->VehicleJourneyName.empty());
			for (const auto& name : stopvisit->MonitoredVehicleJourney->VehicleJourneyName)
			{
				ASSERT_NE(name, nullptr);
				ASSERT_FALSE(name->__item.empty());
			}
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->Delay, nullptr);

			ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall, nullptr);
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->Order, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->MonitoredCall->Order->empty());
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointName.empty());
			for (const auto& name : stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointName)
			{
				ASSERT_NE(name, nullptr);
				ASSERT_FALSE(name->__item.empty());
			}
			ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointRef, nullptr);
			ASSERT_FALSE(stopvisit->MonitoredVehicleJourney->MonitoredCall->StopPointRef->__item.empty());
			if (stopvisit->MonitoredVehicleJourney->__MonitoredVehicleJourneyStructure_sequence__ && stopvisit->MonitoredVehicleJourney->__MonitoredVehicleJourneyStructure_sequence__->HeadwayService == true)
			{
				ASSERT_EQ(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime, nullptr);
				ASSERT_EQ(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime, nullptr);
				ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval, nullptr);
				ASSERT_GT(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval, 0);
			}
			else
			{
				ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime, nullptr);
				ASSERT_NE(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime, nullptr);
				ASSERT_EQ(stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedHeadwayInterval, nullptr);
				ASSERT_LE(GSOAP::to_ptime(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedDepartureTime), GSOAP::to_ptime(*delivery->ValidUntil));
				//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*stopvisit->MonitoredVehicleJourney->MonitoredCall->AimedArrivalTime));
			}
		}
		ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(delivery->ResponseTimestamp));
	}
}

void Siri_SM_Unknown_Stop::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

	xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(desired_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = non_existing_stop;
	req.MonitoringRef = monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete previewinterval;
	delete monitoringref_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(unknown_stop_error_text), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____InvalidDataReferencesError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.InvalidDataReferencesError->ErrorText->find(unknown_stop_error_text), std::wstring::npos);

		ASSERT_TRUE(delivery->MonitoredStopVisit.empty());
	}
}

void Siri_SM_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_EQ(delivery->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);

		ASSERT_TRUE(delivery->MonitoredStopVisit.empty());
	}
}

void Siri_SM_Error_Context_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &unauth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		//info_structure->ServiceRequestContext = context_structure;	// Missing context on purpose
	request.ServiceRequestInfo = info_structure;

	xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(desired_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = get_random_stop().stopref;
	req.MonitoringRef = monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete previewinterval;
	delete monitoringref_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_EQ(delivery->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);

		ASSERT_TRUE(delivery->MonitoredStopVisit.empty());
	}
}

void Siri_SM_Error_Period::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

	xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(too_many_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		monitoringref_structure->__item = get_random_stop().stopref;
	req.MonitoringRef = monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete previewinterval;
	delete monitoringref_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_EQ(delivery->ErrorCondition->Description->__item, too_many_hours_error_text);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____CapabilityNotSupportedError);
		ASSERT_EQ(*delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.CapabilityNotSupportedError->ErrorText, too_many_hours_error_text);

		ASSERT_TRUE(delivery->MonitoredStopVisit.empty());
	}
}

void Siri_SM_Error_Parameter_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

		xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
		req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(desired_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete previewinterval;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"MonitoringRef")), std::wstring::npos);

		ASSERT_TRUE(delivery->MonitoredStopVisit.empty());
	}
}

void Siri_SM_Error_Parameter_null::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__StopMonitoringRequestStructure request;
	ns1__StopMonitoringAnswerStructure response;

	ns2__StopMonitoringRequestStructure req;
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

	xsd__dateTime* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;
	
	boost::chrono::hours interval(desired_hours);
	boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);

	LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
		//monitoringref_structure->__item = non_existing_stop;
	req.MonitoringRef = monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetStopMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete starttime_struct;
	delete previewinterval;
	delete monitoringref_structure;


	ASSERT_FALSE(response.Answer->StopMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->StopMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"MonitoringRef")), std::wstring::npos);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"MonitoringRef")), std::wstring::npos);

		ASSERT_TRUE(delivery->MonitoredStopVisit.empty());
	}
}
