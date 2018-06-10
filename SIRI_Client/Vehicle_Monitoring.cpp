#include "stdafx.h"
#include "Vehicle_Monitoring.h"

void Siri_VM_Base::show_VM(const ns1__VehicleMonitoringAnswerStructure& response)
{
	_tstring path = _T("c:/temp/siri/vehiclemonitoring.txt");
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Vehicle Monitoring: " << std::endl << std::endl;

	unsigned cont = 0; 
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ofs << "Delivery #" << ++cont << ",  ";
		ofs << "valid until: " << GSOAP::to_ptime(*delivery->ValidUntil) << std::endl;
		for (const auto& activity : delivery->VehicleActivity)
		{
			ofs << std::endl;
			ofs << "LineRef: "				<< activity->MonitoredVehicleJourney.LineRef->__item						<< ", ";
			ofs << "DirectionRef: "			<< activity->MonitoredVehicleJourney.DirectionRef->__item					<< ", ";
			ofs << "VehicleJourneyName: "	<< activity->MonitoredVehicleJourney.VehicleJourneyName.front()->__item		<< std::endl;
			ofs << "Long. tramo: "			<< *activity->ProgressBetweenStops->LinkDistance							<< " m" << std::endl;
			ofs << "% Recorrido: "			<< *activity->ProgressBetweenStops->Percentage								<< " %" << std::endl;
			ofs << "Service ID: "			<< *activity->ItemIdentifier << " " << activity->VehicleMonitoringRef->__item << std::endl;
			boost::posix_time::time_duration delay= boost::posix_time::milliseconds(*activity->MonitoredVehicleJourney.Delay);
			ofs << "Delay: " << delay << std::endl;
			ofs << "Speed: " << *activity->MonitoredVehicleJourney.Velocity << " km/h - ";
			ofs << "Bearing: " << *activity->MonitoredVehicleJourney.Bearing << " degrees" << std::endl;
			for (const auto& future_stop : activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall)
			{
				ofs << *future_stop->Order									<< ". ";
				ofs << future_stop->StopPointName.front()->__item			<< " - ";
				if (activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__&& activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__->HeadwayService == true)
				{
					ofs << future_stop->StopPointRef->__item << " - frequency: ";
					ofs << *future_stop->AimedHeadwayInterval / (1000 * 60) << " minutes" << std::endl;
				}
				else
				{
					ofs << future_stop->StopPointRef->__item << " -  from: ";
					ofs << GSOAP::to_ptime(*future_stop->AimedArrivalTime) << " to: ";
					ofs << GSOAP::to_ptime(*future_stop->AimedDepartureTime) << std::endl;
				}
			}
		}

		for (const auto& stopcandellation : delivery->VehicleActivityCancellation)
		{

		}
		ofs << "\nResponse time: " << GSOAP::to_ptime(delivery->ResponseTimestamp);
	}
}

TEST_F(Siri_ClientTestFixture, VM_Main_Line)					{	Siri_VM_Main_Line				VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Main_Line_Direction)			{	Siri_VM_Main_Line_Direction		VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Main_Vehicle)				{	Siri_VM_Main_Vehicle			VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Main_Vehicle_Direction)		{	Siri_VM_Main_Vehicle_Direction	VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Error_Unknown_Line)			{	Siri_VM_Error_Unknown_Line		VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Error_Unknown_Vehicle)		{	Siri_VM_Error_Unknown_Vehicle	VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Error_Unauth)				{	Siri_VM_Error_Unauth			VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Error_Context_Missing)		{	Siri_VM_Error_Context_Missing	VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Error_Parameter_Missing)		{	Siri_VM_Error_Parameter_Missing VM;		VM.run();	}
TEST_F(Siri_ClientTestFixture, VM_Error_Parameter_null)		{	Siri_VM_Error_Parameter_null	VM;		VM.run();	}

void Siri_VM_Main_Line::run() {
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		vehicle_monitoringref_structure->__item = get_random_line().lineref;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;
	
		req.__union_VehicleMonitoringRequestStructure = 2;

	ns2__LineRefStructure* line_ref_structure = new ns2__LineRefStructure;
	line_ref_structure->__item = std::wstring(req.VehicleMonitoringRef->__item);
	req.union_VehicleMonitoringRequestStructure.LineRef = line_ref_structure;

	// #define SOAP_TAG_MISMATCH				3	stdsoap2.h
	// #define SOAP_TCP_ERROR                  28
	set_proxy(ws);
	//EXPECT_EQ(ws.GetVehicleMonitoring("http://data.itsfactory.fi/siriaccess/vm/siri", nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";
	EXPECT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete line_ref_structure;


	if (!response.Answer)
		ASSERT_EQ(0, 1);

	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(*delivery->ValidUntil));
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(delivery->ResponseTimestamp));

		ASSERT_FALSE(delivery->VehicleActivity.empty());
		for (const auto& activity : delivery->VehicleActivity)
		{
			ASSERT_NE	(activity, nullptr);
			ASSERT_NE	(activity->ItemIdentifier, nullptr);
			ASSERT_FALSE(activity->ItemIdentifier->empty());
			ASSERT_NE	(activity->ProgressBetweenStops, nullptr);
			ASSERT_NE	(activity->ProgressBetweenStops->LinkDistance, nullptr);
			ASSERT_FALSE(activity->ProgressBetweenStops->LinkDistance->empty());
			ASSERT_NE	(activity->ProgressBetweenStops->Percentage, nullptr);
			ASSERT_FALSE(activity->ProgressBetweenStops->Percentage->empty());
			double percentage = 0;
			_tstringstream ss;
			ss.imbue(std::locale::classic());
			ss << *activity->ProgressBetweenStops->Percentage;
			ss >> percentage;
			ASSERT_LE	(percentage, 100);
			ASSERT_NE	(activity->MonitoredVehicleJourney.DirectionRef, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.DirectionRef->__item.empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.LineRef, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.LineRef->__item.empty());
			ASSERT_FALSE(activity->MonitoredVehicleJourney.VehicleJourneyName.empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.Bearing, nullptr);
			ASSERT_NE	(activity->MonitoredVehicleJourney.Velocity, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.Velocity->empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.ProgressRate, nullptr);

			for (const auto& name : activity->MonitoredVehicleJourney.VehicleJourneyName)
			{
				ASSERT_NE(name, nullptr);
				ASSERT_FALSE(name->__item.empty());
			}
			ASSERT_NE(activity->MonitoredVehicleJourney.Delay, nullptr);

			ASSERT_NE(activity->MonitoredVehicleJourney.OnwardCalls, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall.empty());
			for (const auto& future_stop : activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall)
			{
				ASSERT_NE(future_stop->Order, nullptr);
				ASSERT_FALSE(future_stop->Order->empty());

				ASSERT_FALSE(future_stop->StopPointName.empty());
				for (const auto& name : future_stop->StopPointName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_NE(future_stop->StopPointRef, nullptr);
				ASSERT_FALSE(future_stop->StopPointRef->__item.empty());

				if (activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__ && activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__->HeadwayService == true)
				{
					ASSERT_EQ(future_stop->AimedArrivalTime, nullptr);
					ASSERT_EQ(future_stop->AimedDepartureTime, nullptr);
					ASSERT_NE(future_stop->AimedHeadwayInterval, nullptr);
					ASSERT_GT(*future_stop->AimedHeadwayInterval, 0);
				}
				else
				{
					ASSERT_NE(future_stop->AimedArrivalTime, nullptr);
					ASSERT_NE(future_stop->AimedDepartureTime, nullptr);
					ASSERT_EQ(future_stop->AimedHeadwayInterval, nullptr);
					ASSERT_LE(GSOAP::to_ptime(*future_stop->AimedDepartureTime), GSOAP::to_ptime(*delivery->ValidUntil));
					//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*future_stop->AimedArrivalTime));
				}
			}
		}
		ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(delivery->ResponseTimestamp));
	}
	
	if (siri_debug)
		show_VM(response);
}

void Siri_VM_Main_Line_Direction::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		vehicle_monitoringref_structure->__item = get_random_line().lineref;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;
	
	req.__union_VehicleMonitoringRequestStructure = 2;
	
	ns2__LineRefStructure* line_ref_structure = new ns2__LineRefStructure;
		line_ref_structure->__item = std::wstring(req.VehicleMonitoringRef->__item);
	req.union_VehicleMonitoringRequestStructure.LineRef = line_ref_structure;

	ns2__DirectionRefStructure* direction_ref_structure = new ns2__DirectionRefStructure;
		direction_ref_structure->__item = desired_direction;
	req.DirectionRef = direction_ref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete line_ref_structure;
	delete direction_ref_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(*delivery->ValidUntil));
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(delivery->ResponseTimestamp));

		//EXPECT_GT(delivery->VehicleActivity.size(), 0) << "DirectionRef correct, but there might not be vehicles now";
		if (delivery->VehicleActivity.size())
		{
			const std::wstring dir(delivery->VehicleActivity.front()->MonitoredVehicleJourney.DirectionRef->__item);
			for (const auto& activity : delivery->VehicleActivity)
			{
				ASSERT_NE	(activity, nullptr);
				ASSERT_NE	(activity->ItemIdentifier, nullptr);
				ASSERT_FALSE(activity->ItemIdentifier->empty());
				ASSERT_NE	(activity->ProgressBetweenStops, nullptr);
				ASSERT_NE	(activity->ProgressBetweenStops->LinkDistance, nullptr);
				ASSERT_FALSE(activity->ProgressBetweenStops->LinkDistance->empty());
				ASSERT_NE	(activity->MonitoredVehicleJourney.DirectionRef, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.DirectionRef->__item.empty());
				//EXPECT_EQ	(activity->MonitoredVehicleJourney.DirectionRef->__item, dir) << "Might fail if a non existing direction is requested";		// TO-CHECK
				ASSERT_NE	(activity->MonitoredVehicleJourney.LineRef, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.LineRef->__item.empty());
				ASSERT_FALSE(activity->MonitoredVehicleJourney.VehicleJourneyName.empty());
				ASSERT_NE	(activity->MonitoredVehicleJourney.Bearing, nullptr);
				ASSERT_NE	(activity->MonitoredVehicleJourney.Velocity, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.Velocity->empty());
				ASSERT_NE	(activity->MonitoredVehicleJourney.ProgressRate, nullptr);

				for (const auto& name : activity->MonitoredVehicleJourney.VehicleJourneyName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_NE(activity->MonitoredVehicleJourney.Delay, nullptr);

				ASSERT_NE(activity->MonitoredVehicleJourney.OnwardCalls, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall.empty());
				for (const auto& future_stop : activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall)
				{
					ASSERT_NE(future_stop->Order, nullptr);
					ASSERT_FALSE(future_stop->Order->empty());

					ASSERT_FALSE(future_stop->StopPointName.empty());
					for (const auto& name : future_stop->StopPointName)
					{
						ASSERT_NE(name, nullptr);
						ASSERT_FALSE(name->__item.empty());
					}
					ASSERT_NE(future_stop->StopPointRef, nullptr);
					ASSERT_FALSE(future_stop->StopPointRef->__item.empty());
					if (activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__ && activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__->HeadwayService == true)
					{
						ASSERT_EQ(future_stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(future_stop->AimedDepartureTime, nullptr);
						ASSERT_NE(future_stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*future_stop->AimedHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(future_stop->AimedArrivalTime, nullptr);
						ASSERT_NE(future_stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(future_stop->AimedHeadwayInterval, nullptr);
						ASSERT_LE(GSOAP::to_ptime(*future_stop->AimedDepartureTime), GSOAP::to_ptime(*delivery->ValidUntil));
						//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*future_stop->AimedArrivalTime));
					}
				}
			}
		}
			ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(delivery->ResponseTimestamp));
	}
	
	//if (siri_debug)
	//	show_VM(response);
}

void Siri_VM_Main_Vehicle::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		vehicle_monitoringref_structure->__item = desired_vehicle;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;	// TO-CHECK: VehicleMonitoringRef == union.vehicleref (or union.lineref)??
	
		req.__union_VehicleMonitoringRequestStructure = 1;

	ns2__VehicleRefStructure* vehicle_ref_structure = new ns2__VehicleRefStructure;
		vehicle_ref_structure->__item = desired_vehicle;
	req.union_VehicleMonitoringRequestStructure.VehicleRef = vehicle_ref_structure;

	set_proxy(ws);
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete vehicle_ref_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(*delivery->ValidUntil));
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(delivery->ResponseTimestamp));

		ASSERT_FALSE(delivery->VehicleActivity.empty());
		for (const auto& activity : delivery->VehicleActivity)
		{
			ASSERT_NE	(activity, nullptr);
			ASSERT_NE	(activity->ItemIdentifier, nullptr);
			ASSERT_FALSE(activity->ItemIdentifier->empty());
			ASSERT_NE	(activity->ProgressBetweenStops, nullptr);
			ASSERT_NE	(activity->ProgressBetweenStops->LinkDistance, nullptr);
			ASSERT_FALSE(activity->ProgressBetweenStops->LinkDistance->empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.DirectionRef, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.DirectionRef->__item.empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.LineRef, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.LineRef->__item.empty());
			ASSERT_FALSE(activity->MonitoredVehicleJourney.VehicleJourneyName.empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.Bearing, nullptr);
			ASSERT_NE	(activity->MonitoredVehicleJourney.Velocity, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.Velocity->empty());
			ASSERT_NE	(activity->MonitoredVehicleJourney.ProgressRate, nullptr);

			for (const auto& name : activity->MonitoredVehicleJourney.VehicleJourneyName)
			{
				ASSERT_NE(name, nullptr);
				ASSERT_FALSE(name->__item.empty());
			}
			ASSERT_NE(activity->MonitoredVehicleJourney.Delay, nullptr);

			ASSERT_NE(activity->MonitoredVehicleJourney.OnwardCalls, nullptr);
			ASSERT_FALSE(activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall.empty());
			for (const auto& future_stop : activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall)
			{
				ASSERT_NE(future_stop->Order, nullptr);
				ASSERT_FALSE(future_stop->Order->empty());

				ASSERT_FALSE(future_stop->StopPointName.empty());
				for (const auto& name : future_stop->StopPointName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_NE(future_stop->StopPointRef, nullptr);
				ASSERT_FALSE(future_stop->StopPointRef->__item.empty());
				if (activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__ && activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__->HeadwayService == true)
				{
					ASSERT_EQ(future_stop->AimedArrivalTime, nullptr);
					ASSERT_EQ(future_stop->AimedDepartureTime, nullptr);
					ASSERT_NE(future_stop->AimedHeadwayInterval, nullptr);
					ASSERT_GT(*future_stop->AimedHeadwayInterval, 0);
				}
				else
				{
					ASSERT_NE(future_stop->AimedArrivalTime, nullptr);
					ASSERT_NE(future_stop->AimedDepartureTime, nullptr);
					ASSERT_EQ(future_stop->AimedHeadwayInterval, nullptr);
					ASSERT_LE(GSOAP::to_ptime(*future_stop->AimedDepartureTime), GSOAP::to_ptime(*delivery->ValidUntil));
					//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*future_stop->AimedArrivalTime));
				}
			}
		}
		ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(delivery->ResponseTimestamp));
	}
	
	//if (siri_debug)
	//	show_VM(response);
}

void Siri_VM_Main_Vehicle_Direction::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		vehicle_monitoringref_structure->__item = desired_vehicle;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;
	
		req.__union_VehicleMonitoringRequestStructure = 1;
	ns2__VehicleRefStructure* vehicle_ref_structure = new ns2__VehicleRefStructure;
		vehicle_ref_structure->__item = desired_vehicle;
	req.union_VehicleMonitoringRequestStructure.VehicleRef = vehicle_ref_structure;

	ns2__DirectionRefStructure* direction_ref_structure = new ns2__DirectionRefStructure;
		direction_ref_structure->__item = desired_direction;
	req.DirectionRef = direction_ref_structure;

	//set_proxy(ws);Genera
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete vehicle_ref_structure;
	delete direction_ref_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(*delivery->ValidUntil));
		ASSERT_LE(GSOAP::to_ptime(req.RequestTimestamp), GSOAP::to_ptime(delivery->ResponseTimestamp));

		//EXPECT_GT(delivery->VehicleActivity.size(), 0) << "Requested Vehicle might be travelling in the opposite direction";
		if (delivery->VehicleActivity.size())
		{
			EXPECT_EQ(delivery->VehicleActivity.size(), 1) << "Possible duplicated vehicles";
			const std::wstring dir(delivery->VehicleActivity.front()->MonitoredVehicleJourney.DirectionRef->__item);
			for (const auto& activity : delivery->VehicleActivity)
			{
				ASSERT_NE	(activity, nullptr);
				ASSERT_NE	(activity->ItemIdentifier, nullptr);
				ASSERT_FALSE(activity->ItemIdentifier->empty());
				ASSERT_NE	(activity->ProgressBetweenStops, nullptr);
				ASSERT_NE	(activity->ProgressBetweenStops->LinkDistance, nullptr);
				ASSERT_FALSE(activity->ProgressBetweenStops->LinkDistance->empty());
				ASSERT_NE	(activity->MonitoredVehicleJourney.DirectionRef, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.DirectionRef->__item.empty());
				EXPECT_EQ	(activity->MonitoredVehicleJourney.DirectionRef->__item, dir);
				ASSERT_NE	(activity->MonitoredVehicleJourney.LineRef, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.LineRef->__item.empty());
				ASSERT_FALSE(activity->MonitoredVehicleJourney.VehicleJourneyName.empty());
				ASSERT_NE	(activity->MonitoredVehicleJourney.Bearing, nullptr);
				ASSERT_NE	(activity->MonitoredVehicleJourney.Velocity, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.Velocity->empty());
				ASSERT_NE	(activity->MonitoredVehicleJourney.ProgressRate, nullptr);
				
				for (const auto& name : activity->MonitoredVehicleJourney.VehicleJourneyName)
				{
					ASSERT_NE(name, nullptr);
					ASSERT_FALSE(name->__item.empty());
				}
				ASSERT_NE(activity->MonitoredVehicleJourney.Delay, nullptr);

				ASSERT_NE(activity->MonitoredVehicleJourney.OnwardCalls, nullptr);
				ASSERT_FALSE(activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall.empty());
				for (const auto& future_stop : activity->MonitoredVehicleJourney.OnwardCalls->OnwardCall)
				{
					ASSERT_NE(future_stop->Order, nullptr);
					ASSERT_FALSE(future_stop->Order->empty());

					ASSERT_FALSE(future_stop->StopPointName.empty());
					for (const auto& name : future_stop->StopPointName)
					{
						ASSERT_NE(name, nullptr);
						ASSERT_FALSE(name->__item.empty());
					}
					ASSERT_NE(future_stop->StopPointRef, nullptr);
					ASSERT_FALSE(future_stop->StopPointRef->__item.empty());
					if (activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__ && activity->MonitoredVehicleJourney.__VehicleActivityStructure_MonitoredVehicleJourney_sequence__->HeadwayService == true)
					{
						ASSERT_EQ(future_stop->AimedArrivalTime, nullptr);
						ASSERT_EQ(future_stop->AimedDepartureTime, nullptr);
						ASSERT_NE(future_stop->AimedHeadwayInterval, nullptr);
						ASSERT_GT(*future_stop->AimedHeadwayInterval, 0);
					}
					else
					{
						ASSERT_NE(future_stop->AimedArrivalTime, nullptr);
						ASSERT_NE(future_stop->AimedDepartureTime, nullptr);
						ASSERT_EQ(future_stop->AimedHeadwayInterval, nullptr);
						ASSERT_LE(GSOAP::to_ptime(*future_stop->AimedDepartureTime), GSOAP::to_ptime(*delivery->ValidUntil));
						//ASSERT_LE(ptime_start_time, GSOAP::to_ptime(*future_stop->AimedArrivalTime));
					}
				}
			}
		}
		ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(delivery->ResponseTimestamp));
	}
	
	//if (siri_debug)
	//	show_VM(response);
}

void Siri_VM_Error_Unknown_Line::run(){
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		vehicle_monitoringref_structure->__item = non_existing_line;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;
	
		req.__union_VehicleMonitoringRequestStructure = 2;
	ns2__LineRefStructure* line_ref_structure = new ns2__LineRefStructure;
		line_ref_structure->__item = non_existing_line;
	req.union_VehicleMonitoringRequestStructure.LineRef = line_ref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete line_ref_structure;
	delete vehicle_monitoringref_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(unknown_vehicle_error_text), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____InvalidDataReferencesError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.InvalidDataReferencesError->ErrorText->find(unknown_vehicle_error_text), std::wstring::npos);

		ASSERT_TRUE(delivery->VehicleActivity.empty());
	}
}

void Siri_VM_Error_Unknown_Vehicle::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		vehicle_monitoringref_structure->__item = non_existing_vehicle;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;
	
		req.__union_VehicleMonitoringRequestStructure = 1;
	ns2__VehicleRefStructure* vehicle_ref_structure = new ns2__VehicleRefStructure;
		vehicle_ref_structure->__item = non_existing_vehicle;
	req.union_VehicleMonitoringRequestStructure.VehicleRef = vehicle_ref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete vehicle_ref_structure;
	delete vehicle_monitoringref_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(unknown_vehicle_error_text), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____InvalidDataReferencesError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.InvalidDataReferencesError->ErrorText->find(unknown_vehicle_error_text), std::wstring::npos);

		ASSERT_TRUE(delivery->VehicleActivity.empty());
	}
}

void Siri_VM_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_EQ(delivery->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	
		ASSERT_TRUE(delivery->VehicleActivity.empty());
	}
}

void Siri_VM_Error_Context_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &unauth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		//info_structure->ServiceRequestContext = context_structure;		// Missing context on purpose

	request.ServiceRequestInfo = info_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_EQ(delivery->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	
		ASSERT_TRUE(delivery->VehicleActivity.empty());
	}
}

void Siri_VM_Error_Parameter_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"VehicleMonitoringRef")), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"VehicleMonitoringRef")), std::wstring::npos);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"VehicleMonitoringRef")), std::wstring::npos);

		ASSERT_TRUE(delivery->VehicleActivity.empty());
	}
}

void Siri_VM_Error_Parameter_null::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__VehicleMonitoringRequestStructure request;
	ns1__VehicleMonitoringAnswerStructure response;

	ns2__VehicleMonitoringRequestStructure req;
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

	ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
		//vehicle_monitoringref_structure->__item = desired_vehicle;
	req.VehicleMonitoringRef= vehicle_monitoringref_structure;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetVehicleMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete vehicle_monitoringref_structure;


	ASSERT_FALSE(response.Answer->VehicleMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->VehicleMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"VehicleMonitoringRef")), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"VehicleMonitoringRef")), std::wstring::npos);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"VehicleMonitoringRef")), std::wstring::npos);

		ASSERT_TRUE(delivery->VehicleActivity.empty());
	}
}
