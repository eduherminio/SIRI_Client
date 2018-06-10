#include "stdafx.h"
#include "Capabilities.h"

TEST_F(Siri_ClientTestFixture, CAP_Main)					{	Siri_CAP_Main						CAP;		CAP.run();	}
TEST_F(Siri_ClientTestFixture, CAP_MissingParameter_PT)		{	Siri_CAP_MissingParameter_PT		CAP;		CAP.run();	}
TEST_F(Siri_ClientTestFixture, CAP_MissingParameter_ST)		{	Siri_CAP_MissingParameter_ST		CAP;		CAP.run();	}
TEST_F(Siri_ClientTestFixture, CAP_MissingParameter_SM)		{	Siri_CAP_MissingParameter_SM		CAP;		CAP.run();	}
TEST_F(Siri_ClientTestFixture, CAP_MissingParameter_VM)		{	Siri_CAP_MissingParameter_VM		CAP;		CAP.run();	}
TEST_F(Siri_ClientTestFixture, CAP_MissingParameter_ET)		{	Siri_CAP_MissingParameter_ET		CAP;		CAP.run();	}
TEST_F(Siri_ClientTestFixture, CAP_MissingParameter_FM)		{	Siri_CAP_MissingParameter_FM		CAP;		CAP.run();	}

void Siri_CAP_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
		production_timetable->RequestTimestamp = request_timestamp;
	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		stop_timetable->RequestTimestamp = request_timestamp;
	req.StopTimetableCapabilitiesRequest = stop_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		stop_monitoring->RequestTimestamp = request_timestamp;	
	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		vehicle_monitoring->RequestTimestamp = request_timestamp;
	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		estimated_timetable->RequestTimestamp = request_timestamp;
	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		facility_monitoring->RequestTimestamp = request_timestamp;
	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete production_timetable;
	delete stop_timetable;
	delete stop_monitoring;
	delete vehicle_monitoring;
	delete estimated_timetable;
	delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}

void Siri_CAP_MissingParameter_PT::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	//_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
	//	production_timetable->RequestTimestamp = request_timestamp;
	//	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		stop_timetable->RequestTimestamp = request_timestamp;
	req.StopTimetableCapabilitiesRequest = stop_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		stop_monitoring->RequestTimestamp = request_timestamp;	
	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		vehicle_monitoring->RequestTimestamp = request_timestamp;
	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		estimated_timetable->RequestTimestamp = request_timestamp;
	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		facility_monitoring->RequestTimestamp = request_timestamp;
	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	//delete production_timetable;
	delete stop_timetable;
	delete stop_monitoring;
	delete vehicle_monitoring;
	delete estimated_timetable;
	delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"production_timetable")), std::wstring::npos);
	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"production_timetable")), std::wstring::npos);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"production_timetable")), std::wstring::npos);

	//ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}

void Siri_CAP_MissingParameter_ST::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
		production_timetable->RequestTimestamp = request_timestamp;
	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	//ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
	//	stop_timetable->RequestTimestamp = request_timestamp;
	//	req.StopTimetableCapabilitiesRequest = stop_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		stop_monitoring->RequestTimestamp= request_timestamp;	
	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		vehicle_monitoring->RequestTimestamp = request_timestamp;
	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		estimated_timetable->RequestTimestamp = request_timestamp;
	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		facility_monitoring->RequestTimestamp = request_timestamp;
	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete production_timetable;
	//delete stop_timetable;
	delete stop_monitoring;
	delete vehicle_monitoring;
	delete estimated_timetable;
	delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);

	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"stop_timetable")), std::wstring::npos);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"stop_timetable")), std::wstring::npos);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"stop_timetable")), std::wstring::npos);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	//ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription,	nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}

void Siri_CAP_MissingParameter_SM::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
		production_timetable->RequestTimestamp = request_timestamp;
	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		stop_timetable->RequestTimestamp = request_timestamp;
	req.StopTimetableCapabilitiesRequest = stop_timetable;

	//ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
	//	stop_monitoring->RequestTimestamp = request_timestamp;	
	//	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		vehicle_monitoring->RequestTimestamp = request_timestamp;
	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		estimated_timetable->RequestTimestamp = request_timestamp;
	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		facility_monitoring->RequestTimestamp = request_timestamp;
	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete production_timetable;
	delete stop_timetable;
	//delete stop_monitoring;
	delete vehicle_monitoring;
	delete estimated_timetable;
	delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"stop_monitoring")), std::wstring::npos);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"stop_monitoring")), std::wstring::npos);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"stop_monitoring")), std::wstring::npos);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription, nullptr);
	//ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}

void Siri_CAP_MissingParameter_VM::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
		production_timetable->RequestTimestamp = request_timestamp;
	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		stop_timetable->RequestTimestamp = request_timestamp;
	req.StopTimetableCapabilitiesRequest = stop_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		stop_monitoring->RequestTimestamp = request_timestamp;	
	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	//ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
	//	vehicle_monitoring->RequestTimestamp = request_timestamp;
	//	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		estimated_timetable->RequestTimestamp = request_timestamp;
	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		facility_monitoring->RequestTimestamp = request_timestamp;
	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete production_timetable;
	delete stop_timetable;
	delete stop_monitoring;
	//delete vehicle_monitoring;
	delete estimated_timetable;
	delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->ErrorCondition,	nullptr);

	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"vehicle_monitoring")), std::wstring::npos);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"vehicle_monitoring")), std::wstring::npos);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"vehicle_monitoring")), std::wstring::npos);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	//ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}

void Siri_CAP_MissingParameter_ET::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
		production_timetable->RequestTimestamp = request_timestamp;
	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		stop_timetable->RequestTimestamp = request_timestamp;
	req.StopTimetableCapabilitiesRequest = stop_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		stop_monitoring->RequestTimestamp = request_timestamp;	
	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		vehicle_monitoring->RequestTimestamp = request_timestamp;
	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	//ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
	//	estimated_timetable->RequestTimestamp = request_timestamp;
	//	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		facility_monitoring->RequestTimestamp = request_timestamp;
	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete production_timetable;
	delete stop_timetable;
	delete stop_monitoring;
	delete vehicle_monitoring;
	//delete estimated_timetable;
	delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->ErrorCondition,	nullptr);

	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"estimated_timetable")), std::wstring::npos);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"estimated_timetable")), std::wstring::npos);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"estimated_timetable")), std::wstring::npos);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	//ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}

void Siri_CAP_MissingParameter_FM::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsGetCapabilitiesRequestStructure request;
	ns1__WsGetCapabilitiesAnswerStructure response;

	ns2__CapabilitiesRequestStructure req;
	request.Request= &req;

	xsd__dateTime__ request_timestamp= GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	req.RequestTimestamp = request_timestamp;

	_ns2__ProductionTimetableCapabilitiesRequest* production_timetable = new _ns2__ProductionTimetableCapabilitiesRequest;
		production_timetable->RequestTimestamp = request_timestamp;
	req.ProductionTimetableCapabilitiesRequest = production_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		stop_timetable->RequestTimestamp = request_timestamp;
	req.StopTimetableCapabilitiesRequest = stop_timetable;

	ns2__ServiceCapabilitiesRequestStructure* stop_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		stop_monitoring->RequestTimestamp = request_timestamp;	
	req.StopMonitoringCapabilitiesRequest = stop_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* vehicle_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
		vehicle_monitoring->RequestTimestamp = request_timestamp;
	req.VehicleMonitoringCapabilitiesRequest = vehicle_monitoring;

	ns2__ServiceCapabilitiesRequestStructure* estimated_timetable = new ns2__ServiceCapabilitiesRequestStructure;
		estimated_timetable->RequestTimestamp = request_timestamp;
	req.EstimatedTimetableCapabilitiesRequest = estimated_timetable;

	//ns2__ServiceCapabilitiesRequestStructure* facility_monitoring = new ns2__ServiceCapabilitiesRequestStructure;
	//	facility_monitoring->RequestTimestamp = request_timestamp;
	//	req.FacilityMonitoringCapabilitiesRequest = facility_monitoring;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetCapabilities(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete production_timetable;
	delete stop_timetable;
	delete stop_monitoring;
	delete vehicle_monitoring;
	delete estimated_timetable;
	//delete facility_monitoring;

	ASSERT_NE(response.Answer, nullptr);
	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse,		nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse,	nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse,	nullptr);

	ASSERT_EQ(response.Answer->ProductionTimetableCapabilitiesResponse->ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopTimetableCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->StopMonitoringCapabilitiesResponse->		ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->VehicleMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_EQ(response.Answer->EstimatedTimetableCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);

	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition, nullptr);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"facility_monitoring")), std::wstring::npos);
	ASSERT_EQ(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"facility_monitoring")), std::wstring::npos);
	ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"facility_monitoring")), std::wstring::npos);

	ASSERT_NE(response.Answer->ProductionTimetableCapabilitiesResponse->ProductionTimetableServiceCapabilities->TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopTimetableCapabilitiesResponse->		StopTimetableServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->StopMonitoringCapabilitiesResponse->		StopMonitoringServiceCapabilities->		TransportDescription, nullptr);
	ASSERT_NE(response.Answer->VehicleMonitoringCapabilitiesResponse->	VehicleMonitoringServiceCapabilities->	TransportDescription, nullptr);
	ASSERT_NE(response.Answer->EstimatedTimetableCapabilitiesResponse->	EstimatedTimetableServiceCapabilities->	TransportDescription, nullptr);
	//ASSERT_NE(response.Answer->FacilityMonitoringCapabilitiesResponse->	FacilityMonitoringServiceCapabilities->	TransportDescription, nullptr);
}
