#include "stdafx.h"
#include "Estimated_Timetable.h"

void Siri_ET_Base::show_ET(const ns1__WsEstimatedTimetableAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/estimatedtimetable.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Estimated Timetable: " << std::endl << std::endl;

	int cont_line = 0;
	for (const auto& line : response.Answer->EstimatedTimetableDelivery)
	{
		ofs << "Line number " << ++cont_line;
		ofs << "\t(valid until: " << GSOAP::to_ptime(*line->ValidUntil) << ")" << std::endl;
		for (const auto& trajectory : line->EstimatedJourneyVersionFrame)
		{
			ofs << "\nL" << trajectory->EstimatedVehicleJourney.front()->LineRef->__item << ", ";
			ofs << "Direction: " << trajectory->EstimatedVehicleJourney.front()->DirectionRef->__item << std::endl;

			for (const auto& expedition : trajectory->EstimatedVehicleJourney)
			{
				ofs << "\nExpedition code: " << expedition->VehicleJourneyRef->__item << " - ";
				ofs << "from: " << expedition->OriginName.front()->__item << "(" << expedition->OriginRef->__item << ")";
				ofs << " to: " << expedition->DestinationName.front()->__item << "(" << expedition->DestinationRef->__item << ")" << std::endl;

				for (const auto& stop : expedition->EstimatedCalls->EstimatedCall)
				{
					if (expedition->__EstimatedVehicleJourneyStructure_sequence___->HeadwayService == true)
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

TEST_F(Siri_ClientTestFixture, ET_Main)					{	Siri_ET_Main				ET;		ET.run();	}
TEST_F(Siri_ClientTestFixture, ET_Main_Lines)			{	Siri_ET_Main_Lines			ET;		ET.run();	}
TEST_F(Siri_ClientTestFixture, ET_Unknown_Lines)		{	Siri_ET_Unknown_Lines		ET;		ET.run();	}
TEST_F(Siri_ClientTestFixture, ET_Error_Unauth)			{	Siri_ET_Error_Unauth		ET;		ET.run();	}
TEST_F(Siri_ClientTestFixture, ET_Preview_0)			{	Siri_ET_Preview_0			ET;		ET.run();	}

void Siri_ET_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsEstimatedTimetableRequestStructure request;
	ns1__WsEstimatedTimetableAnswerStructure response;

	ns2__EstimatedTimetableRequestStructure req;
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

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetEstimatedTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete previewinterval;
	delete info_structure;


	ASSERT_FALSE(response.Answer->EstimatedTimetableDelivery.empty());
	for (const auto& line : response.Answer->EstimatedTimetableDelivery)
	{
		ASSERT_NE(line, nullptr);
		ASSERT_EQ(line->ErrorCondition, nullptr);
		ASSERT_FALSE(line->EstimatedJourneyVersionFrame.empty());
		for (const auto& trayecto : line->EstimatedJourneyVersionFrame)		// No timetable services, only frequency ones (TO-CHECK: ¿filtrar los de frecuencia en ese caso?)
		{
			ASSERT_LE(GSOAP::to_ptime(trayecto->RecordedAtTime), boost::posix_time::second_clock::universal_time());
			for (const auto& expedition : trayecto->EstimatedVehicleJourney)
			{
				ASSERT_NE(expedition, nullptr);
				ASSERT_NE(expedition->LineRef, nullptr);
				ASSERT_FALSE(expedition->LineRef->__item.empty());
				ASSERT_NE(expedition->DirectionRef, nullptr);
				ASSERT_FALSE(expedition->DirectionRef->__item.empty());
				ASSERT_NE(expedition->VehicleJourneyRef, nullptr);
				ASSERT_FALSE(expedition->VehicleJourneyRef->__item.empty());
				ASSERT_NE(expedition->DestinationRef, nullptr);
				ASSERT_FALSE(expedition->DestinationRef->__item.empty());
				ASSERT_NE(expedition->OriginRef, nullptr);
				ASSERT_FALSE(expedition->OriginRef->__item.empty());

				ASSERT_FALSE(expedition->DestinationName.empty());
				for (const auto& name : expedition->DestinationName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_FALSE(expedition->OriginName.empty());
				for (const auto& name : expedition->OriginName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->DirectionName.empty());
				for (const auto& name : expedition->__EstimatedVehicleJourneyStructure_sequence->DirectionName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->PublishedLineName.empty());
				for (const auto& name : expedition->__EstimatedVehicleJourneyStructure_sequence->PublishedLineName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence->JourneyPatternRef, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->JourneyPatternRef->__item.empty());
				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence->RouteRef, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->RouteRef->__item.empty());

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence__, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence__->VehicleJourneyName.empty());
				for (const auto& name : expedition->__EstimatedVehicleJourneyStructure_sequence__->VehicleJourneyName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence___, nullptr);

				ASSERT_NE(expedition->EstimatedCalls, nullptr);
				ASSERT_FALSE(expedition->EstimatedCalls->EstimatedCall.empty());
				for (const auto& stop : expedition->EstimatedCalls->EstimatedCall)
				{
					ASSERT_NE(stop, nullptr);
					ASSERT_NE(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());

					if (expedition->__EstimatedVehicleJourneyStructure_sequence___->HeadwayService == true)
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
					}
				}	// stops

				//ASSERT_NE(expedition->RecordedCalls, nullptr);
				//ASSERT_FALSE(expedition->RecordedCalls->RecordedCall.empty());
				if (expedition->RecordedCalls == nullptr)							// TO-CHECK
					continue;
				for (const auto& stop : expedition->RecordedCalls->RecordedCall)
				{
					ASSERT_NE(stop, nullptr);
					ASSERT_NE(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());

					if (expedition->__EstimatedVehicleJourneyStructure_sequence___->HeadwayService == true)
					{
						ASSERT_EQ(stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(stop->ActualArrivalTime, nullptr);
						ASSERT_EQ(stop->ActualDepartureTime, nullptr);
						ASSERT_NE(stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*stop->AimedHeadwayInterval, 0);
						ASSERT_NE(stop->ActualHeadwayInterval, nullptr);
						ASSERT_GT(*stop->ActualHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(stop->AimedArrivalTime, nullptr);
						ASSERT_NE(stop->ActualArrivalTime, nullptr);
						ASSERT_NE(stop->AimedDepartureTime, nullptr);
						ASSERT_NE(stop->ActualDepartureTime, nullptr);
						ASSERT_EQ(stop->AimedHeadwayInterval, nullptr);
						ASSERT_EQ(stop->ActualHeadwayInterval, nullptr);
					}
				}	// stops
			}	// expeditions
		}	// trayectos
	}	// lineas

	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
} 

void Siri_ET_Main_Lines::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsEstimatedTimetableRequestStructure request;
	ns1__WsEstimatedTimetableAnswerStructure response;

	ns2__EstimatedTimetableRequestStructure req;
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

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	_ns2__EstimatedTimetableRequestStructure_Lines* request_lines_structure = new _ns2__EstimatedTimetableRequestStructure_Lines;
		std::wstring requested_line_ref1(non_existing_line);
		std::wstring existing_line (get_random_line().lineref);
		std::wstring requested_line_ref2(existing_line);

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

	set_proxy(ws);
	ASSERT_EQ(ws.GetEstimatedTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";
	
	delete previewinterval;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete request_lines_structure;


	ASSERT_FALSE(response.Answer->EstimatedTimetableDelivery.empty());
	for (const auto& line : response.Answer->EstimatedTimetableDelivery)
	{
		ASSERT_NE(line, nullptr);
		ASSERT_EQ(line->ErrorCondition, nullptr);
		ASSERT_FALSE(line->EstimatedJourneyVersionFrame.empty());
		for (const auto& trayecto : line->EstimatedJourneyVersionFrame)		// No timetable services, only frequency ones (TO-CHECK: ¿filtrar los de frecuencia en ese caso?)
		{
			ASSERT_LE(GSOAP::to_ptime(trayecto->RecordedAtTime), boost::posix_time::second_clock::universal_time());
			for (const auto& expedition : trayecto->EstimatedVehicleJourney)
			{
				ASSERT_NE(expedition, nullptr);
				ASSERT_NE(expedition->LineRef, nullptr);
				ASSERT_FALSE(expedition->LineRef->__item.empty());
				ASSERT_EQ(expedition->LineRef->__item, existing_line);
				ASSERT_NE(expedition->DirectionRef, nullptr);
				ASSERT_FALSE(expedition->DirectionRef->__item.empty());
				ASSERT_NE(expedition->VehicleJourneyRef, nullptr);
				ASSERT_FALSE(expedition->VehicleJourneyRef->__item.empty());
				ASSERT_NE(expedition->DestinationRef, nullptr);
				ASSERT_FALSE(expedition->DestinationRef->__item.empty());
				ASSERT_NE(expedition->OriginRef, nullptr);
				ASSERT_FALSE(expedition->OriginRef->__item.empty());

				ASSERT_FALSE(expedition->DestinationName.empty());
				for (const auto& name : expedition->DestinationName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_FALSE(expedition->OriginName.empty());
				for (const auto& name : expedition->OriginName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->DirectionName.empty());
				for (const auto& name : expedition->__EstimatedVehicleJourneyStructure_sequence->DirectionName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->PublishedLineName.empty());
				for (const auto& name : expedition->__EstimatedVehicleJourneyStructure_sequence->PublishedLineName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence->JourneyPatternRef, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->JourneyPatternRef->__item.empty());
				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence->RouteRef, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence->RouteRef->__item.empty());

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence__, nullptr);
				ASSERT_FALSE(expedition->__EstimatedVehicleJourneyStructure_sequence__->VehicleJourneyName.empty());
				for (const auto& name : expedition->__EstimatedVehicleJourneyStructure_sequence__->VehicleJourneyName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}

				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence___, nullptr);

				ASSERT_NE(expedition->EstimatedCalls, nullptr);
				ASSERT_FALSE(expedition->EstimatedCalls->EstimatedCall.empty());
				for (const auto& stop : expedition->EstimatedCalls->EstimatedCall)
				{
					ASSERT_NE(stop, nullptr);
					ASSERT_NE(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());

					if (expedition->__EstimatedVehicleJourneyStructure_sequence___->HeadwayService == true)
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
					}
				}	// stops

				//ASSERT_NE(expedition->RecordedCalls, nullptr);
				//ASSERT_FALSE(expedition->RecordedCalls->RecordedCall.empty());
				if (expedition->RecordedCalls == nullptr)							// TO-CHECK
					continue;
				for (const auto& stop : expedition->RecordedCalls->RecordedCall)
				{
					ASSERT_NE(stop, nullptr);
					ASSERT_NE(stop->StopPointRef, nullptr);
					ASSERT_FALSE(stop->StopPointRef->__item.empty());

					if (expedition->__EstimatedVehicleJourneyStructure_sequence___->HeadwayService == true)
					{
						ASSERT_EQ(stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(stop->ActualArrivalTime, nullptr);
						ASSERT_EQ(stop->ActualDepartureTime, nullptr);
						ASSERT_NE(stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*stop->AimedHeadwayInterval, 0);
						ASSERT_NE(stop->ActualHeadwayInterval, nullptr);
						ASSERT_GT(*stop->ActualHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(stop->AimedArrivalTime, nullptr);
						ASSERT_NE(stop->ActualArrivalTime, nullptr);
						ASSERT_NE(stop->AimedDepartureTime, nullptr);
						ASSERT_NE(stop->ActualDepartureTime, nullptr);
						ASSERT_EQ(stop->AimedHeadwayInterval, nullptr);
						ASSERT_EQ(stop->ActualHeadwayInterval, nullptr);
					}
				}	// stops
			}	// expeditions
		}	// trayectos
	}	// lineas

	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
	
	if (siri_debug)
		show_ET(response);
}

void Siri_ET_Unknown_Lines::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsEstimatedTimetableRequestStructure request;
	ns1__WsEstimatedTimetableAnswerStructure response;

	ns2__EstimatedTimetableRequestStructure req;
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

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	_ns2__EstimatedTimetableRequestStructure_Lines* request_lines_structure = new _ns2__EstimatedTimetableRequestStructure_Lines;
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
	ASSERT_EQ(ws.GetEstimatedTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";
	
	delete previewinterval;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete request_lines_structure;


	ASSERT_TRUE(response.Answer->EstimatedTimetableDelivery.empty());
	
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_ET_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsEstimatedTimetableRequestStructure request;
	ns1__WsEstimatedTimetableAnswerStructure response;

	ns2__EstimatedTimetableRequestStructure req;
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
		
		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetEstimatedTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete previewinterval;


	ASSERT_FALSE(response.Answer->EstimatedTimetableDelivery.empty());
	for (const auto& line : response.Answer->EstimatedTimetableDelivery)
	{
		ASSERT_NE(line, nullptr);
		ASSERT_NE(line->ErrorCondition, nullptr);
		ASSERT_NE(line->ErrorCondition->Description, nullptr);
		ASSERT_EQ(line->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(line->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*line->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
		ASSERT_TRUE(line->EstimatedJourneyVersionFrame.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_ET_Preview_0::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsEstimatedTimetableRequestStructure request;
	ns1__WsEstimatedTimetableAnswerStructure response;

	ns2__EstimatedTimetableRequestStructure req;
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

		LONG64* previewinterval = new LONG64(0);
	req.PreviewInterval = previewinterval;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetEstimatedTimetable(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete previewinterval;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->EstimatedTimetableDelivery.empty());
	for (const auto& line : response.Answer->EstimatedTimetableDelivery)
	{
		ASSERT_NE(line, nullptr);
		ASSERT_EQ(line->ErrorCondition, nullptr);
		//ASSERT_FALSE(line->EstimatedJourneyVersionFrame.empty());
		for (const auto& trayecto : line->EstimatedJourneyVersionFrame)		// No timetable services, only frequency ones (TO-CHECK: ¿filtrar los de frecuencia en ese caso?)
		{
			ASSERT_NE(trayecto, nullptr);
			for (const auto& expedition : trayecto->EstimatedVehicleJourney)
			{
				ASSERT_NE(expedition, nullptr);
				ASSERT_NE(expedition->__EstimatedVehicleJourneyStructure_sequence___, nullptr);
				ASSERT_EQ(expedition->__EstimatedVehicleJourneyStructure_sequence___->HeadwayService, true);
			}
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}
