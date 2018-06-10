#include "stdafx.h"
#include "Facility_Monitoring.h"

void Siri_FM_Base::show_FM(const ns1__WsFacilityMonitoringAnswerStructure& response, const _tstring mode)
{
	_tstring path = _T("c:/temp/siri/facilitymonitoring_") + mode + _tstring(_T(".txt"));
	const fs::path p = path;

	_ofstream ofs(p.string());
	ofs.imbue(loc);

	ofs << "Facility Monitoring: " << static_cast<const wchar_t*>(saer::ct2cw(mode)) << std::endl << std::endl;

	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ofs << "\t(valid until: " << GSOAP::to_ptime(*delivery->ValidUntil) << ")" << std::endl;
		ofs << "From: " << GSOAP::to_ptime(delivery->FacilityCondition.front()->ValidityPeriod->StartTime);
		ofs << " to: " << GSOAP::to_ptime(*delivery->FacilityCondition.front()->ValidityPeriod->EndTime) << std::endl << std::endl;

		auto last_type = *delivery->FacilityCondition.front()->MonitoringInfo->MonitoringType;
		auto last_status = delivery->FacilityCondition.front()->FacilityStatus->Status;
		unsigned n_facilities = 0;
		for (const auto& facility_condition : delivery->FacilityCondition)
		{
			ofs << ++n_facilities << "\t" << facility_condition->union_FacilityConditionStructure.Facility->Description.front()->__item;
			if (*facility_condition->MonitoringInfo->MonitoringType != last_type || n_facilities == 1)
				ofs << "\t\t\t(Type: " << *facility_condition->MonitoringInfo->MonitoringType;
			if (facility_condition->FacilityStatus->Status != last_status || n_facilities == 1)
				ofs << ",\tStatus: " << facility_condition->FacilityStatus->Status << ")";

			ofs << std::endl;

			last_type = *facility_condition->MonitoringInfo->MonitoringType;
			last_status = delivery->FacilityCondition.front()->FacilityStatus->Status;
		}
	}

	ofs << "\nResponse time: " << GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp) << std::endl;
}

TEST_F(Siri_ClientTestFixture, FM_All_Lines)						{	Siri_FM_All_Lines							FM;		FM.run();	}
TEST_F(Siri_ClientTestFixture, FM_All_Stops)						{	Siri_FM_All_Stops							FM;		FM.run();	}
TEST_F(Siri_ClientTestFixture, FM_Error_Unauth)						{	Siri_FM_Error_Unauth						FM;		FM.run();	}
TEST_F(Siri_ClientTestFixture, FM_Error_LineRef_StopPointRef_Missing)	{	Siri_FM_Error_LineRef_StopPointRef_Missing	FM;		FM.run();	}
TEST_F(Siri_ClientTestFixture, FM_Wrong_ALL_LINES_CODE)				{	Siri_FM_Wrong_ALL_LINES_CODE				FM;		FM.run();	}
TEST_F(Siri_ClientTestFixture, FM_Wrong_ALL_STOPS_CODE)				{	Siri_FM_Wrong_ALL_STOPS_CODE				FM;		FM.run();	}

void Siri_FM_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsFacilityMonitoringRequestStructure request;
	ns1__WsFacilityMonitoringAnswerStructure response;

	ns2__FacilityMonitoringRequestStructure req;
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
	ASSERT_EQ(ws.GetFacilityMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;


	ASSERT_FALSE(response.Answer->FacilityMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_EQ(delivery->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
		ASSERT_TRUE(delivery->FacilityCondition.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_FM_Error_LineRef_StopPointRef_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsFacilityMonitoringRequestStructure request;
	ns1__WsFacilityMonitoringAnswerStructure response;

	ns2__FacilityMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &auth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;
	
	xsd__dateTime__* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	__ns2__FacilityMonitoringRequestStructure_sequence* sequence_struct = new __ns2__FacilityMonitoringRequestStructure_sequence;
		ns2__LineRefStructure* lineref_struct = new ns2__LineRefStructure;
			lineref_struct->__item = ALL_LINES_CODE;
		//sequence_struct->LineRef = lineref_struct;
		ns2__StopPointRefStructure* stoppointref_struct = new ns2__StopPointRefStructure;
			//stoppointref_struct->__item = ALL_STOPS_CODE;
		sequence_struct->StopPointRef = stoppointref_struct;
	req.__FacilityMonitoringRequestStructure_sequence = sequence_struct;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetFacilityMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete starttime_struct;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete previewinterval;
	
	// ...
	delete lineref_struct;
	delete stoppointref_struct;
	delete sequence_struct;


	ASSERT_FALSE(response.Answer->FacilityMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(missing_parameter_error_text + std::wstring(L"Missing LineRef and StopPointRef")), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + std::wstring(L"Missing LineRef and StopPointRef")), std::wstring::npos);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(std::wstring(L"Missing LineRef and StopPointRef")), std::wstring::npos);

		ASSERT_TRUE(delivery->FacilityCondition.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_FM_Wrong_ALL_LINES_CODE::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsFacilityMonitoringRequestStructure request;
	ns1__WsFacilityMonitoringAnswerStructure response;

	ns2__FacilityMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &auth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;
	
	xsd__dateTime__* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	__ns2__FacilityMonitoringRequestStructure_sequence* sequence_struct = new __ns2__FacilityMonitoringRequestStructure_sequence;
		ns2__LineRefStructure* lineref_struct = new ns2__LineRefStructure;
			lineref_struct->__item = WRONG_ALL_LINES_CODE;
		sequence_struct->LineRef = lineref_struct;
	req.__FacilityMonitoringRequestStructure_sequence = sequence_struct;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetFacilityMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete starttime_struct;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete previewinterval;
	
	// ...
	delete lineref_struct;
	delete sequence_struct;


	ASSERT_FALSE(response.Answer->FacilityMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(unknown_ALL_LINES_CODE_error_text), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____InvalidDataReferencesError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.InvalidDataReferencesError->ErrorText->find(unknown_ALL_LINES_CODE_error_text), std::wstring::npos);
		ASSERT_TRUE(delivery->FacilityCondition.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_FM_Wrong_ALL_STOPS_CODE::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsFacilityMonitoringRequestStructure request;
	ns1__WsFacilityMonitoringAnswerStructure response;

	ns2__FacilityMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &auth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;
	
	xsd__dateTime__* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	__ns2__FacilityMonitoringRequestStructure_sequence* sequence_struct = new __ns2__FacilityMonitoringRequestStructure_sequence;
		ns2__StopPointRefStructure* stoppointref_struct = new ns2__StopPointRefStructure;
			stoppointref_struct->__item = WRONG_ALL_STOPS_CODE;
		sequence_struct->StopPointRef = stoppointref_struct;
	req.__FacilityMonitoringRequestStructure_sequence = sequence_struct;

	//set_proxy(ws);
	ASSERT_EQ(ws.GetFacilityMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete starttime_struct;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete previewinterval;
	
	// ...
	delete stoppointref_struct;
	delete sequence_struct;


	ASSERT_FALSE(response.Answer->FacilityMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_NE(delivery->ErrorCondition, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description, nullptr);
		ASSERT_NE(delivery->ErrorCondition->Description->__item.find(unknown_ALL_STOPS_CODE_error_text), std::wstring::npos);
		ASSERT_EQ(delivery->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____InvalidDataReferencesError);
		ASSERT_NE(delivery->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.InvalidDataReferencesError->ErrorText->find(unknown_ALL_STOPS_CODE_error_text), std::wstring::npos);
		ASSERT_TRUE(delivery->FacilityCondition.empty());
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));
}

void Siri_FM_All_Lines::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsFacilityMonitoringRequestStructure request;
	ns1__WsFacilityMonitoringAnswerStructure response;

	ns2__FacilityMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &auth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;
	
	xsd__dateTime__* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	__ns2__FacilityMonitoringRequestStructure_sequence* sequence_struct = new __ns2__FacilityMonitoringRequestStructure_sequence;
		ns2__LineRefStructure* lineref_struct = new ns2__LineRefStructure;
			lineref_struct->__item = ALL_LINES_CODE;
		sequence_struct->LineRef = lineref_struct;
	req.__FacilityMonitoringRequestStructure_sequence = sequence_struct;

	set_proxy(ws);
	ASSERT_EQ(ws.GetFacilityMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete starttime_struct;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete previewinterval;
	
	// ...
	delete lineref_struct;
	delete sequence_struct;


	ASSERT_FALSE(response.Answer->FacilityMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		
		ASSERT_FALSE(delivery->FacilityCondition.empty());
		for (const auto& facility_condition : delivery->FacilityCondition)
		{
			ASSERT_NE(facility_condition, nullptr);
			ASSERT_NE(facility_condition->FacilityStatus, nullptr);
			ASSERT_NE(facility_condition->ValidityPeriod, nullptr);
			ASSERT_FALSE(GSOAP::to_ptime(facility_condition->ValidityPeriod->StartTime).is_not_a_date_time());
			ASSERT_FALSE(GSOAP::to_ptime(*facility_condition->ValidityPeriod->EndTime).is_not_a_date_time());
			if (facility_condition->__union_FacilityConditionStructure == SOAP_UNION__ns2__union_FacilityConditionStructure_Facility)
			{
				ASSERT_NE(facility_condition->union_FacilityConditionStructure.Facility, nullptr);
				ASSERT_NE(facility_condition->union_FacilityConditionStructure.Facility->FacilityCode, nullptr);
				ASSERT_FALSE(facility_condition->union_FacilityConditionStructure.Facility->FacilityCode->empty());
				ASSERT_FALSE(facility_condition->union_FacilityConditionStructure.Facility->FacilityClass.empty());
				for (const auto& clase : facility_condition->union_FacilityConditionStructure.Facility->FacilityClass)
				{
					ASSERT_TRUE(clase == ns2__FacilityCategoryEnumeration__fixedEquipment);
				}
			}
			else if (facility_condition->__union_FacilityConditionStructure == SOAP_UNION__ns2__union_FacilityConditionStructure_FacilityRef)
			{
				ASSERT_NE(facility_condition->union_FacilityConditionStructure.FacilityRef, nullptr);
				ASSERT_FALSE(facility_condition->union_FacilityConditionStructure.FacilityRef->__item.empty());
			}
			else
				ASSERT_FALSE(true);
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));

	if (siri_debug)
	{
		_tstring mode(_T("lines"));
		show_FM(response, mode);
	}
}

void Siri_FM_All_Stops::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsFacilityMonitoringRequestStructure request;
	ns1__WsFacilityMonitoringAnswerStructure response;

	ns2__FacilityMonitoringRequestStructure req;
	request.Request = &req;
	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

	ns1__WsServiceRequestInfoStructure* info_structure = new ns1__WsServiceRequestInfoStructure;
		info_structure->AccountId = &auth_user;
		info_structure->AccountKey = &pwd;
		info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
		ns2__ServiceRequestContextStructure* context_structure = new ns2__ServiceRequestContextStructure;
			context_structure->Language = desired_language;
			context_structure->VelocityUnits = new std::wstring(velocity_units);
		info_structure->ServiceRequestContext = context_structure;
	request.ServiceRequestInfo = info_structure;
	
	xsd__dateTime__* starttime_struct = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
	req.StartTime = starttime_struct;

		boost::chrono::hours interval(desired_hours);
		boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
		LONG64* previewinterval = new LONG64(ms.count());
	req.PreviewInterval = previewinterval;

	__ns2__FacilityMonitoringRequestStructure_sequence* sequence_struct = new __ns2__FacilityMonitoringRequestStructure_sequence;
		ns2__StopPointRefStructure* stoppointref_struct = new ns2__StopPointRefStructure;
			stoppointref_struct->__item = ALL_STOPS_CODE;
		sequence_struct->StopPointRef = stoppointref_struct;
	req.__FacilityMonitoringRequestStructure_sequence = sequence_struct;

	set_proxy(ws);
	ASSERT_EQ(ws.GetFacilityMonitoring(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete starttime_struct;
	delete context_structure->VelocityUnits;
	delete context_structure;
	delete info_structure;
	delete previewinterval;
	
	// ...
	delete stoppointref_struct;
	delete sequence_struct;


	ASSERT_FALSE(response.Answer->FacilityMonitoringDelivery.empty());
	for (const auto& delivery : response.Answer->FacilityMonitoringDelivery)
	{
		ASSERT_NE(delivery, nullptr);
		ASSERT_EQ(delivery->ErrorCondition, nullptr);
		
		ASSERT_FALSE(delivery->FacilityCondition.empty());
		for (const auto& facility_condition : delivery->FacilityCondition)
		{
			ASSERT_NE(facility_condition, nullptr);
			ASSERT_NE(facility_condition->FacilityStatus, nullptr);
			ASSERT_NE(facility_condition->ValidityPeriod, nullptr);
			ASSERT_FALSE(GSOAP::to_ptime(facility_condition->ValidityPeriod->StartTime).is_not_a_date_time());
			ASSERT_FALSE(GSOAP::to_ptime(*facility_condition->ValidityPeriod->EndTime).is_not_a_date_time());
			if (facility_condition->__union_FacilityConditionStructure == SOAP_UNION__ns2__union_FacilityConditionStructure_Facility)
			{
				ASSERT_NE(facility_condition->union_FacilityConditionStructure.Facility, nullptr);
				ASSERT_NE(facility_condition->union_FacilityConditionStructure.Facility->FacilityCode, nullptr);
				ASSERT_FALSE(facility_condition->union_FacilityConditionStructure.Facility->FacilityCode->empty());
				ASSERT_FALSE(facility_condition->union_FacilityConditionStructure.Facility->FacilityClass.empty());
				for (const auto& clase : facility_condition->union_FacilityConditionStructure.Facility->FacilityClass)
				{
					ASSERT_TRUE(clase == ns2__FacilityCategoryEnumeration__fixedEquipment);
				}
			}
			else if (facility_condition->__union_FacilityConditionStructure == SOAP_UNION__ns2__union_FacilityConditionStructure_FacilityRef)
			{
				ASSERT_NE(facility_condition->union_FacilityConditionStructure.FacilityRef, nullptr);
				ASSERT_FALSE(facility_condition->union_FacilityConditionStructure.FacilityRef->__item.empty());
			}
			else
				ASSERT_FALSE(true);
		}
	}
	ASSERT_GE(boost::posix_time::second_clock::universal_time(), GSOAP::to_ptime(response.ServiceDeliveryInfo->ResponseTimestamp));

	if (siri_debug)
	{
		_tstring mode(_T("stops"));
		show_FM(response, mode);
	}
}
