#include "stdafx.h"
#include "Production_Timetable.h"

void Siri_PT_Base::show_PT(const ns1__ProductionTimetableAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/productiontimetable.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Production Timetable: " << std::endl << std::endl;

	int cont_line = 0;
	for (const auto& line : response.Answer->ProductionTimetableDelivery)
	{
		ofs << "Line number " << ++cont_line;
		ofs << "\t(valid until: " << GSOAP::to_ptime(*line->ValidUntil) << ")" << std::endl;
		for (const auto& trajectory : line->DatedTimetableVersionFrame)
		{
			ofs << "\nL" << trajectory->LineRef->__item << ", ";
			ofs << "Trajectory: " << trajectory->RouteRef->__item << ", ";
			ofs << "Direction: " << trajectory->DirectionRef->__item << std::endl;

			for (const auto& expedition : trajectory->DatedVehicleJourney)
			{
				ofs << "\nExpedition code: " << *expedition->DatedVehicleJourneyCode << " - ";
				ofs << "from: " << expedition->OriginDisplay.front()->__item;
				ofs << " to: " << expedition->DestinationDisplay.front()->__item << std::endl;

				for (const auto& stop : expedition->DatedCalls.DatedCall)
				{
					if (expedition->HeadwayService && *expedition->HeadwayService == true)
					{
						ofs << stop->StopPointRef->__item << " - frequency: ";
						ofs << *stop->AimedHeadwayInterval / (1000*60) << " minutes"<<std::endl;
					}
					else
					{
						ofs << stop->StopPointRef->__item << " -  from: ";
						ofs << GSOAP::to_ptime(*stop->AimedArrivalTime) << " to: ";
						ofs << GSOAP::to_ptime(*stop->AimedDepartureTime) << std::endl;
					}
				}
			}
		}
	}

	ofs << "\nResponse time: " << GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp) << std::endl;
}

TEST_F(Siri_ClientTestFixture, PT_Main)							{	Siri_PT_Main							PT;		PT.run();	}
TEST_F(Siri_ClientTestFixture, PT_Main_Lines)					{	Siri_PT_Main_Lines						PT;		PT.run();	}
TEST_F(Siri_ClientTestFixture, PT_Unknown_Line)					{	Siri_PT_Unknown_Line					PT;		PT.run();	}
TEST_F(Siri_ClientTestFixture, PT_Error_Unauth)					{	Siri_PT_Error_Unauth					PT;		PT.run();	}
TEST_F(Siri_ClientTestFixture, PT_Error_Period)					{	Siri_PT_Error_Period					PT;		PT.run();	}
TEST_F(Siri_ClientTestFixture, PT_Error_ValidityPeriod_Missing)	{	Siri_PT_Error_ValidityPeriod_Missing	PT;		PT.run();	}

void Siri_PT_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__ProductionTimetableRequestStructure request;
	ns1__ProductionTimetableAnswerStructure response;

	ns2__ProductionTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId= &user;
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
	req.ValidityPeriod = timestamp_structure;

	const boost::posix_time::ptime ptime_start_time = GSOAP::to_ptime(req.ValidityPeriod->StartTime);
	const boost::posix_time::ptime ptime_end_time = GSOAP::to_ptime(req.ValidityPeriod->EndTime);
	//...
	//set_proxy(ws);
	ASSERT_EQ(ws.GetProductionTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete timestamp_structure;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->ProductionTimetableDelivery.empty());
	for (const auto& line : response.Answer->ProductionTimetableDelivery)
	{
		ASSERT_NE	(line, nullptr);
		ASSERT_EQ	(ptime_end_time, GSOAP::to_ptime(*line->ValidUntil));
		ASSERT_EQ	(line->ErrorCondition, nullptr);
		ASSERT_FALSE(line->DatedTimetableVersionFrame.empty());
		for (const auto& trajectory : line->DatedTimetableVersionFrame)
		{
			ASSERT_NE	(trajectory, nullptr);
			ASSERT_NE	(trajectory->LineRef, nullptr);
			ASSERT_FALSE(trajectory->LineRef->__item.empty());
			ASSERT_NE	(trajectory->DirectionRef, nullptr);
			ASSERT_FALSE(trajectory->DirectionRef->__item.empty());
			ASSERT_FALSE(trajectory->DatedVehicleJourney.empty());
			for (const auto& expedition : trajectory->DatedVehicleJourney)
			{
				ASSERT_NE	(expedition, nullptr);
				ASSERT_FALSE(expedition->DatedVehicleJourneyCode->empty());
				ASSERT_FALSE(expedition->DestinationDisplay.empty());
				for (const auto& name : expedition->DestinationDisplay)
				{
					ASSERT_NE	(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->OriginDisplay.empty());
				for (const auto& name : expedition->OriginDisplay)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->DatedCalls.DatedCall.empty());
				for (const auto& stop : expedition->DatedCalls.DatedCall)
				{
					ASSERT_NE	(stop, nullptr);
					ASSERT_NE	(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());
					if (expedition->HeadwayService && *expedition->HeadwayService == true)
					{
						ASSERT_EQ(stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(stop->AimedDepartureTime, nullptr);
						ASSERT_NE(stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*stop->AimedHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(stop->AimedArrivalTime, nullptr);
						ASSERT_NE(stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(stop->AimedHeadwayInterval, nullptr);
						ASSERT_LE(GSOAP::to_ptime(*stop->AimedDepartureTime), ptime_end_time);
						ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*stop->AimedArrivalTime));
					}
				}
			}
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_PT_Main_Lines::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__ProductionTimetableRequestStructure request;
	ns1__ProductionTimetableAnswerStructure response;

	ns2__ProductionTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId= &user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;

	ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
		xsd__dateTime__ start_time =	GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
		xsd__dateTime__ end_time =		GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
		timestamp_structure->StartTime = start_time;
		timestamp_structure->EndTime = end_time;
	req.ValidityPeriod = timestamp_structure;

	_ns2__ProductionTimetableRequestStructure_Lines* request_lines_structure = new _ns2__ProductionTimetableRequestStructure_Lines;
		std::wstring requested_line_ref1(non_existing_line);
		std::wstring requested_line_ref2(get_random_line().lineref);

		std::unique_ptr<ns2__LineDirectionStructure> p_line_structure(new ns2__LineDirectionStructure);
			std::unique_ptr<ns2__LineRefStructure> p_lineref_structure(new ns2__LineRefStructure);
				p_lineref_structure->__item = requested_line_ref1;
			auto lineref_1 = std::move(p_lineref_structure);
				p_line_structure->LineRef = lineref_1.get();
			auto line_1 = std::move(p_line_structure);

			request_lines_structure->LineDirection.push_back(line_1.get());

		p_line_structure.reset(new ns2__LineDirectionStructure);
			p_lineref_structure.reset(new ns2__LineRefStructure);
				p_lineref_structure->__item = requested_line_ref2;
			auto lineref_2 = std::move(p_lineref_structure);
			p_line_structure->LineRef = lineref_2.get();
			auto line_2 = std::move(p_line_structure);

			request_lines_structure->LineDirection.push_back(line_2.get());
		req.Lines = request_lines_structure;
	
	const boost::posix_time::ptime ptime_start_time = GSOAP::to_ptime(req.ValidityPeriod->StartTime);
	const boost::posix_time::ptime ptime_end_time =	GSOAP::to_ptime(req.ValidityPeriod->EndTime);

	set_proxy(ws);
	ASSERT_EQ(ws.GetProductionTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";
	
	delete timestamp_structure;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete request_lines_structure;
	// ...

	ASSERT_FALSE(response.Answer->ProductionTimetableDelivery.empty());
	for (const auto& line : response.Answer->ProductionTimetableDelivery)
	{
		ASSERT_NE	(line, nullptr);
		ASSERT_EQ	(ptime_end_time, GSOAP::to_ptime(*line->ValidUntil));
		ASSERT_EQ	(line->ErrorCondition, nullptr);
		ASSERT_FALSE(line->DatedTimetableVersionFrame.empty());
		for (const auto& trajectory : line->DatedTimetableVersionFrame)
		{
			ASSERT_NE	(trajectory, nullptr);
			ASSERT_NE	(trajectory->LineRef, nullptr);
			ASSERT_FALSE(trajectory->LineRef->__item.empty());
			ASSERT_EQ	(trajectory->LineRef->__item, requested_line_ref2);
			ASSERT_NE	(trajectory->DirectionRef, nullptr);
			ASSERT_FALSE(trajectory->DirectionRef->__item.empty());
			ASSERT_FALSE(trajectory->DatedVehicleJourney.empty());
			for (const auto& expedition : trajectory->DatedVehicleJourney)
			{
				ASSERT_NE	(expedition, nullptr);
				ASSERT_FALSE(expedition->DatedVehicleJourneyCode->empty());
				ASSERT_FALSE(expedition->DestinationDisplay.empty());
				for (const auto& name : expedition->DestinationDisplay)
				{
					ASSERT_NE	(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->OriginDisplay.empty());
				for (const auto& name : expedition->OriginDisplay)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->DatedCalls.DatedCall.empty());
				for (const auto& stop : expedition->DatedCalls.DatedCall)
				{
					ASSERT_NE	(stop, nullptr);
					ASSERT_NE	(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());

					if (expedition->HeadwayService && *expedition->HeadwayService == true)
					{
						ASSERT_EQ(stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(stop->AimedDepartureTime, nullptr);
						ASSERT_NE(stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*stop->AimedHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(stop->AimedArrivalTime, nullptr);
						ASSERT_NE(stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(stop->AimedHeadwayInterval, nullptr);
						//ASSERT_LE(GSOAP::to_ptime(*stop->AimedDepartureTime), ptime_end_time);
						//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*stop->AimedArrivalTime));
					}
				}
			}
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
	
	if (siri_debug)
		show_PT(response);
}

void Siri_PT_Unknown_Line::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__ProductionTimetableRequestStructure request;
	ns1__ProductionTimetableAnswerStructure response;

	ns2__ProductionTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId= &user;
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
	req.ValidityPeriod = timestamp_structure;

	_ns2__ProductionTimetableRequestStructure_Lines* request_lines_structure = new _ns2__ProductionTimetableRequestStructure_Lines;
		std::wstring requested_line_ref1(non_existing_line);
		std::wstring requested_line_ref2(non_existing_line);	

		std::unique_ptr<ns2__LineDirectionStructure> p_line_structure(new ns2__LineDirectionStructure);
			std::unique_ptr<ns2__LineRefStructure> p_lineref_structure(new ns2__LineRefStructure);
				p_lineref_structure->__item = requested_line_ref1;
			auto lineref_1 = std::move(p_lineref_structure);
				p_line_structure->LineRef = lineref_1.get();
			auto line_1 = std::move(p_line_structure);

			request_lines_structure->LineDirection.push_back(line_1.get());

		p_line_structure.reset(new ns2__LineDirectionStructure);
			p_lineref_structure.reset(new ns2__LineRefStructure);
				p_lineref_structure->__item = requested_line_ref2;
			auto lineref_2 = std::move(p_lineref_structure);
			p_line_structure->LineRef = lineref_2.get();
			auto line_2 = std::move(p_line_structure);

			request_lines_structure->LineDirection.push_back(line_2.get());

	req.Lines = request_lines_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetProductionTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";
	
	delete timestamp_structure;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete request_lines_structure;


	ASSERT_TRUE(response.Answer->ProductionTimetableDelivery.empty());
	
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_PT_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__ProductionTimetableRequestStructure request;
	ns1__ProductionTimetableAnswerStructure response;

	ns2__ProductionTimetableRequestStructure req;
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
	ASSERT_EQ(ws.GetProductionTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->ProductionTimetableDelivery.empty());
	for (const auto& line : response.Answer->ProductionTimetableDelivery)
	{
		ASSERT_NE(line->ErrorCondition, nullptr);
		ASSERT_NE(line->ErrorCondition->Description, nullptr);
		ASSERT_EQ(line->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(line->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*line->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
		
		ASSERT_TRUE(line->DatedTimetableVersionFrame.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_PT_Error_Period::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__ProductionTimetableRequestStructure request;
	ns1__ProductionTimetableAnswerStructure response;

	ns2__ProductionTimetableRequestStructure req;
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
	req.ValidityPeriod = timestamp_structure;
	
	//set_proxy(ws);
	ASSERT_EQ(ws.GetProductionTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete timestamp_structure;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->ProductionTimetableDelivery.empty());
	for (const auto& line : response.Answer->ProductionTimetableDelivery)
	{
		ASSERT_NE(line->ErrorCondition, nullptr);
		ASSERT_NE(line->ErrorCondition->Description, nullptr);
		ASSERT_EQ(line->ErrorCondition->Description->__item, too_many_days_error_text);
		ASSERT_EQ(line->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____CapabilityNotSupportedError);
		ASSERT_EQ(*line->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.CapabilityNotSupportedError->ErrorText, too_many_days_error_text);
		
		ASSERT_TRUE(line->DatedTimetableVersionFrame.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_PT_Error_ValidityPeriod_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__ProductionTimetableRequestStructure request;
	ns1__ProductionTimetableAnswerStructure response;

	ns2__ProductionTimetableRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId= &user;
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
		timestamp_structure->StartTime = start_time;	// It's not possible to send an empty field
		timestamp_structure->EndTime = end_time;
	//req.ValidityPeriod = timestamp_structure;	// Server will take  AppTime::pLocalNow(), so asserts related to second_clock::universal_time() are commented
	
	//set_proxy(ws);
	ASSERT_EQ(ws.GetProductionTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete timestamp_structure;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->ProductionTimetableDelivery.empty());
	for (const auto& line : response.Answer->ProductionTimetableDelivery)
	{
		ASSERT_NE	(line, nullptr);
		//ASSERT_EQ	(GSOAP::to_ptime(end_time), GSOAP::to_ptime(*line->ValidUntil));
		ASSERT_EQ	(line->ErrorCondition, nullptr);
		ASSERT_FALSE(line->DatedTimetableVersionFrame.empty());
		for (const auto& trajectory : line->DatedTimetableVersionFrame)
		{
			ASSERT_NE	(trajectory, nullptr);
			ASSERT_NE	(trajectory->LineRef, nullptr);
			ASSERT_FALSE(trajectory->LineRef->__item.empty());
			ASSERT_NE	(trajectory->DirectionRef, nullptr);
			ASSERT_FALSE(trajectory->DirectionRef->__item.empty());
			ASSERT_FALSE(trajectory->DatedVehicleJourney.empty());
			for (const auto& expedition : trajectory->DatedVehicleJourney)
			{
				ASSERT_NE	(expedition, nullptr);
				ASSERT_FALSE(expedition->DatedVehicleJourneyCode->empty());
				ASSERT_FALSE(expedition->DestinationDisplay.empty());
				for (const auto& name : expedition->DestinationDisplay)
				{
					ASSERT_NE	(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->OriginDisplay.empty());
				for (const auto& name : expedition->OriginDisplay)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->DatedCalls.DatedCall.empty());
				for (const auto& stop : expedition->DatedCalls.DatedCall)
				{
					ASSERT_NE	(stop, nullptr);
					ASSERT_NE	(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());
					if (expedition->HeadwayService && *expedition->HeadwayService == true)
					{
						ASSERT_EQ(stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(stop->AimedDepartureTime, nullptr);
						ASSERT_NE(stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*stop->AimedHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(stop->AimedArrivalTime, nullptr);
						ASSERT_NE(stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(stop->AimedHeadwayInterval, nullptr);
						//ASSERT_LE(GSOAP::to_ptime(*stop->AimedDepartureTime), boost::posix_time::second_clock::universal_time());		// No valid for Headway Services
						//ASSERT_LE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(*stop->AimedArrivalTime));			// No valid for Headway Services
					}
				}
			}
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}
