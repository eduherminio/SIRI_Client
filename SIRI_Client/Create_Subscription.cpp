#include "stdafx.h"
#include "Create_Subscription.h"

TEST_F(Siri_ClientTestFixture, SUB_PT_Main)						{	Siri_SUB_PT_Main					SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ST_Main)						{	Siri_SUB_ST_Main					SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_SM_Main)						{	Siri_SUB_SM_Main					SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_VM_Main_Lines)				{	Siri_SUB_VM_Main_Lines				SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ET_Main_Lines)				{	Siri_SUB_ET_Main_Lines				SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_PT_Error_Unauth)				{	Siri_SUB_PT_Error_Unauth			SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ST_Error_Unauth)				{	Siri_SUB_ST_Error_Unauth			SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_SM_Error_Unauth)				{	Siri_SUB_SM_Error_Unauth			SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_VM_Error_Unauth)				{	Siri_SUB_VM_Error_Unauth			SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ET_Error_Unauth)				{	Siri_SUB_ET_Error_Unauth			SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_PT_Error_SubID_Missing)		{	Siri_SUB_PT_Error_SubID_Missing		SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ST_Error_SubID_Missing)		{	Siri_SUB_ST_Error_SubID_Missing		SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_SM_Error_SubID_Missing)		{	Siri_SUB_SM_Error_SubID_Missing		SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_VM_Error_SubID_Missing)		{	Siri_SUB_VM_Error_SubID_Missing		SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ET_Error_SubID_Missing)		{	Siri_SUB_ET_Error_SubID_Missing		SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_PT_Error_Empty_Line)			{	Siri_SUB_PT_Error_Empty_Line		SUB;		SUB.run();	}
TEST_F(Siri_ClientTestFixture, SUB_ST_Error_Long_Period)		{	Siri_SUB_ST_Error_Long_Period		SUB;		SUB.run();	}

void Siri_SUB_PT_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure; // equivalent to subscription identifier
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"PT Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_ProductionTimetableSubscriptionRequest;

		std::vector<_ns2__ProductionTimetableSubscriptionRequest *> * v_production_timetable_requests= new std::vector<_ns2__ProductionTimetableSubscriptionRequest *>;
			_ns2__ProductionTimetableSubscriptionRequest* higher_production_timetable_request = new _ns2__ProductionTimetableSubscriptionRequest;
			
				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				higher_production_timetable_request->SubscriptionIdentifier = subscription_ref_struct;

				higher_production_timetable_request->SubscriberRef = subscriptor_ref;

					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

				ns2__ProductionTimetableRequestStructure* production_timetable_request_structure = new ns2__ProductionTimetableRequestStructure;
					production_timetable_request_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
				
					ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
						xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
						xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
						timestamp_structure->StartTime = start_time;
						timestamp_structure->EndTime = end_time;
					production_timetable_request_structure->ValidityPeriod = timestamp_structure;

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

					production_timetable_request_structure->Lines = request_lines_structure;

			higher_production_timetable_request->ProductionTimetableRequest = production_timetable_request_structure;
		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.ProductionTimetableSubscriptionRequest = v_production_timetable_requests;

	set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	v_subscription_references.push_back(std::wstring(subscription_ref_struct->__item));
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete timestamp_structure;
	delete request_lines_structure;
	delete production_timetable_request_structure;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ValidUntil, nullptr);
	}
}

void Siri_SUB_PT_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &unauth_user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"PT Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_ProductionTimetableSubscriptionRequest;

		std::vector<_ns2__ProductionTimetableSubscriptionRequest *> * v_production_timetable_requests= new std::vector<_ns2__ProductionTimetableSubscriptionRequest *>;
			_ns2__ProductionTimetableSubscriptionRequest* higher_production_timetable_request = new _ns2__ProductionTimetableSubscriptionRequest;

					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

			// ... 

		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.ProductionTimetableSubscriptionRequest = v_production_timetable_requests;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscriptor_ref;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_EQ(response->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	}
}

void Siri_SUB_PT_Error_SubID_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"PT Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_ProductionTimetableSubscriptionRequest;

		std::vector<_ns2__ProductionTimetableSubscriptionRequest *> * v_production_timetable_requests= new std::vector<_ns2__ProductionTimetableSubscriptionRequest *>;
			_ns2__ProductionTimetableSubscriptionRequest* higher_production_timetable_request = new _ns2__ProductionTimetableSubscriptionRequest;
				
					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				//higher_production_timetable_request->SubscriptionIdentifier = subscription_ref_struct;

				// ...

		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.ProductionTimetableSubscriptionRequest = v_production_timetable_requests;
	
	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(sub_refused_error_text), std::wstring::npos);
	}
}

void Siri_SUB_PT_Error_Empty_Line::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"PT Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_ProductionTimetableSubscriptionRequest;

		std::vector<_ns2__ProductionTimetableSubscriptionRequest *> * v_production_timetable_requests= new std::vector<_ns2__ProductionTimetableSubscriptionRequest *>;
			_ns2__ProductionTimetableSubscriptionRequest* higher_production_timetable_request = new _ns2__ProductionTimetableSubscriptionRequest;
			
				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				higher_production_timetable_request->SubscriptionIdentifier = subscription_ref_struct;

				higher_production_timetable_request->SubscriberRef = subscriptor_ref;

					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

				ns2__ProductionTimetableRequestStructure* production_timetable_request_structure = new ns2__ProductionTimetableRequestStructure;
					production_timetable_request_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
				
					ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
						xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
						xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
						timestamp_structure->StartTime = start_time;
						timestamp_structure->EndTime = end_time;
					production_timetable_request_structure->ValidityPeriod = timestamp_structure;

					_ns2__ProductionTimetableRequestStructure_Lines* request_lines_structure = new _ns2__ProductionTimetableRequestStructure_Lines;
						std::wstring requested_line_ref1(get_random_line().lineref);
						std::wstring requested_line_ref2(L"");

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

					production_timetable_request_structure->Lines = request_lines_structure;

				
			higher_production_timetable_request->ProductionTimetableRequest = production_timetable_request_structure;
		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.ProductionTimetableSubscriptionRequest = v_production_timetable_requests;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete timestamp_structure;
	delete request_lines_structure;
	delete production_timetable_request_structure;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(sub_refused_error_text), std::wstring::npos);

	}
}


void Siri_SUB_ST_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ST Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopTimetableSubscriptionRequest;
	
		std::vector<ns2__StopTimetableSubscriptionStructure *> * v_stop_timetable_subscriptions = new std::vector<ns2__StopTimetableSubscriptionStructure *>;
			ns2__StopTimetableSubscriptionStructure* stop_timetable_subscription = new ns2__StopTimetableSubscriptionStructure;
				
				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				stop_timetable_subscription->SubscriptionIdentifier = subscription_ref_struct;

				stop_timetable_subscription->SubscriberRef = subscriptor_ref;

				boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
				boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
				xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
			stop_timetable_subscription->InitialTerminationTime = termination;

			ns2__StopTimetableRequestStructure* stop_timetable_request = new ns2__StopTimetableRequestStructure;
				stop_timetable_request->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			
				ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
					xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
					xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(desired_days));
					timestamp_structure->StartTime = start_time;
					timestamp_structure->EndTime = end_time;
				stop_timetable_request->DepartureWindow = timestamp_structure;

				ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
					monitoringref_structure->__item = get_random_stop().stopref;
				stop_timetable_request->MonitoringRef = monitoringref_structure;

				ns2__LineRefStructure* lineref_structure = new ns2__LineRefStructure;
					lineref_structure->__item = get_random_line().lineref;
				stop_timetable_request->LineRef = lineref_structure;

			stop_timetable_subscription->StopTimetableRequest = stop_timetable_request;
		v_stop_timetable_subscriptions->push_back(stop_timetable_subscription);
	req.union_SiriSubscriptionRequestStructure.StopTimetableSubscriptionRequest = v_stop_timetable_subscriptions;

	set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	v_subscription_references.push_back(std::wstring(subscription_ref_struct->__item));
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete timestamp_structure;
	delete monitoringref_structure;
	delete lineref_structure;
	delete stop_timetable_request;
	delete stop_timetable_subscription;
	delete v_stop_timetable_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ValidUntil, nullptr);
	}
}

void Siri_SUB_ST_Error_Long_Period::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ST Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopTimetableSubscriptionRequest;
	
		std::vector<ns2__StopTimetableSubscriptionStructure *> * v_stop_timetable_subscriptions = new std::vector<ns2__StopTimetableSubscriptionStructure *>;
			ns2__StopTimetableSubscriptionStructure* stop_timetable_subscription = new ns2__StopTimetableSubscriptionStructure;
				
				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				stop_timetable_subscription->SubscriptionIdentifier = subscription_ref_struct;

				stop_timetable_subscription->SubscriberRef = subscriptor_ref;

				boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
				boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
				xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
			stop_timetable_subscription->InitialTerminationTime = termination;

			ns2__StopTimetableRequestStructure* stop_timetable_request = new ns2__StopTimetableRequestStructure;
				stop_timetable_request->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			
				ns2__ClosedTimestampRangeStructure* timestamp_structure = new ns2__ClosedTimestampRangeStructure;
					xsd__dateTime__ start_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)));
					xsd__dateTime__ end_time = GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour)) + boost::gregorian::days(too_many_days));
					timestamp_structure->StartTime = start_time;
					timestamp_structure->EndTime = end_time;
				stop_timetable_request->DepartureWindow = timestamp_structure;

				ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
					monitoringref_structure->__item = get_random_stop().stopref;
				stop_timetable_request->MonitoringRef = monitoringref_structure;

				ns2__LineRefStructure* lineref_structure = new ns2__LineRefStructure;
					lineref_structure->__item = get_random_line().lineref;
				stop_timetable_request->LineRef = lineref_structure;

			stop_timetable_subscription->StopTimetableRequest = stop_timetable_request;
		v_stop_timetable_subscriptions->push_back(stop_timetable_subscription);
	req.union_SiriSubscriptionRequestStructure.StopTimetableSubscriptionRequest = v_stop_timetable_subscriptions;
	
	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete timestamp_structure;
	delete monitoringref_structure;
	delete lineref_structure;
	delete stop_timetable_request;
	delete stop_timetable_subscription;
	delete v_stop_timetable_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(L"Invalid request"), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____OtherError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.OtherError->ErrorText->find(L"Invalid request"), std::wstring::npos);
	}
}

void Siri_SUB_ST_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &unauth_user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ST Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopTimetableSubscriptionRequest;
	
		std::vector<ns2__StopTimetableSubscriptionStructure *> * v_stop_timetable_subscriptions = new std::vector<ns2__StopTimetableSubscriptionStructure *>;
			ns2__StopTimetableSubscriptionStructure* stop_timetable_subscription = new ns2__StopTimetableSubscriptionStructure;

					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				stop_timetable_subscription->InitialTerminationTime = termination;

			// ...
			
		v_stop_timetable_subscriptions->push_back(stop_timetable_subscription);
	req.union_SiriSubscriptionRequestStructure.StopTimetableSubscriptionRequest = v_stop_timetable_subscriptions;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscriptor_ref;
	delete stop_timetable_subscription;
	delete v_stop_timetable_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_EQ(response->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	}
}

void Siri_SUB_ST_Error_SubID_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ST Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopTimetableSubscriptionRequest;
	
		std::vector<ns2__StopTimetableSubscriptionStructure *> * v_stop_timetable_subscriptions = new std::vector<ns2__StopTimetableSubscriptionStructure *>;
			ns2__StopTimetableSubscriptionStructure* stop_timetable_subscription = new ns2__StopTimetableSubscriptionStructure;
				
					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				stop_timetable_subscription->InitialTerminationTime = termination;

				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				//stop_timetable_subscription->SubscriptionIdentifier = subscription_ref_struct;

				// ...

		v_stop_timetable_subscriptions->push_back(stop_timetable_subscription);
	req.union_SiriSubscriptionRequestStructure.StopTimetableSubscriptionRequest = v_stop_timetable_subscriptions;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete stop_timetable_subscription;
	delete v_stop_timetable_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(sub_refused_error_text), std::wstring::npos);
	}
}


void Siri_SUB_SM_Main::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"SM Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopMonitoringSubscriptionRequest;
	
		std::vector<ns2__StopMonitoringSubscriptionStructure *> * v_stop_monitoring_subscriptions = new std::vector<ns2__StopMonitoringSubscriptionStructure *>;
			ns2__StopMonitoringSubscriptionStructure* stop_monitoring_subscription = new ns2__StopMonitoringSubscriptionStructure;
				
				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				stop_monitoring_subscription->SubscriptionIdentifier = subscription_ref_struct;

				stop_monitoring_subscription->SubscriberRef = subscriptor_ref;

				boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
				boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
				xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
			stop_monitoring_subscription->InitialTerminationTime = termination;

			ns2__StopMonitoringRequestStructure* stop_monitoring_request = new ns2__StopMonitoringRequestStructure;
				stop_monitoring_request->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
				stop_monitoring_request->StartTime = new xsd__dateTime(GSOAP::from_ptime(boost::posix_time::ptime(start_date, boost::posix_time::hours(start_hour))));
					boost::chrono::hours interval(desired_hours);
					boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
				stop_monitoring_request->PreviewInterval = new LONG64(ms.count());

				ns2__MonitoringRefStructure* monitoringref_structure = new ns2__MonitoringRefStructure;
					monitoringref_structure->__item = get_random_stop().stopref;
				stop_monitoring_request->MonitoringRef= monitoringref_structure;

			stop_monitoring_subscription->StopMonitoringRequest = stop_monitoring_request;
		v_stop_monitoring_subscriptions->push_back(stop_monitoring_subscription);
	req.union_SiriSubscriptionRequestStructure.StopMonitoringSubscriptionRequest = v_stop_monitoring_subscriptions;

	set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	v_subscription_references.push_back(std::wstring(subscription_ref_struct->__item));
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete monitoringref_structure;
	delete stop_monitoring_request->StartTime;
	delete stop_monitoring_request;
	delete stop_monitoring_subscription;
	delete v_stop_monitoring_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ValidUntil, nullptr);
	}
}

void Siri_SUB_SM_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &unauth_user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"SM Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopMonitoringSubscriptionRequest;
	
		std::vector<ns2__StopMonitoringSubscriptionStructure *> * v_stop_monitoring_subscriptions = new std::vector<ns2__StopMonitoringSubscriptionStructure *>;
			ns2__StopMonitoringSubscriptionStructure* stop_monitoring_subscription = new ns2__StopMonitoringSubscriptionStructure;
				
					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				stop_monitoring_subscription->InitialTerminationTime = termination;

				//...

		v_stop_monitoring_subscriptions->push_back(stop_monitoring_subscription);
	req.union_SiriSubscriptionRequestStructure.StopMonitoringSubscriptionRequest = v_stop_monitoring_subscriptions;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscriptor_ref;
	delete stop_monitoring_subscription;
	delete v_stop_monitoring_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_EQ(response->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	}
}

void Siri_SUB_SM_Error_SubID_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"SM Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_StopMonitoringSubscriptionRequest;
	
		std::vector<ns2__StopMonitoringSubscriptionStructure *> * v_stop_monitoring_subscriptions = new std::vector<ns2__StopMonitoringSubscriptionStructure *>;
			ns2__StopMonitoringSubscriptionStructure* stop_monitoring_subscription = new ns2__StopMonitoringSubscriptionStructure;
				
				boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
				boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
				xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
			stop_monitoring_subscription->InitialTerminationTime = termination;

				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				//stop_monitoring_subscription->SubscriptionIdentifier = subscription_ref_struct;

				// ...

		v_stop_monitoring_subscriptions->push_back(stop_monitoring_subscription);
	req.union_SiriSubscriptionRequestStructure.StopMonitoringSubscriptionRequest = v_stop_monitoring_subscriptions;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete stop_monitoring_subscription;
	delete v_stop_monitoring_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(sub_refused_error_text), std::wstring::npos);
	}
}


void Siri_SUB_VM_Main_Lines::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"VM Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_VehicleMonitoringSubscriptionRequest;
	
		std::vector<ns2__VehicleMonitoringSubscriptionStructure *> * v_vehicle_monitoring_subscriptions = new std::vector<ns2__VehicleMonitoringSubscriptionStructure *>;
			ns2__VehicleMonitoringSubscriptionStructure* vehicle_monitoring_subscription = new ns2__VehicleMonitoringSubscriptionStructure;
				
			ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				vehicle_monitoring_subscription->SubscriptionIdentifier = subscription_ref_struct;

				vehicle_monitoring_subscription->SubscriberRef = subscriptor_ref;

				boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
				boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
				xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
			vehicle_monitoring_subscription->InitialTerminationTime = termination;

			ns2__VehicleMonitoringRequestStructure* vehicle_monitoring_request = new ns2__VehicleMonitoringRequestStructure;
				vehicle_monitoring_request->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());

				ns2__VehicleMonitoringRefStructure* vehicle_monitoringref_structure = new ns2__VehicleMonitoringRefStructure;
					vehicle_monitoringref_structure->__item = get_random_line().lineref;
					vehicle_monitoring_request->VehicleMonitoringRef= vehicle_monitoringref_structure;
	
				vehicle_monitoring_request->__union_VehicleMonitoringRequestStructure = SOAP_UNION__ns2__union_VehicleMonitoringRequestStructure_LineRef;

				ns2__LineRefStructure* line_ref_structure = new ns2__LineRefStructure;
					line_ref_structure->__item = get_random_line().lineref;
				vehicle_monitoring_request->union_VehicleMonitoringRequestStructure.LineRef = line_ref_structure;

			vehicle_monitoring_subscription->VehicleMonitoringRequest = vehicle_monitoring_request;
		v_vehicle_monitoring_subscriptions->push_back(vehicle_monitoring_subscription);
	req.union_SiriSubscriptionRequestStructure.VehicleMonitoringSubscriptionRequest = v_vehicle_monitoring_subscriptions;

	set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	v_subscription_references.push_back(std::wstring(subscription_ref_struct->__item));
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete line_ref_structure;
	delete vehicle_monitoringref_structure;
	delete vehicle_monitoring_request;
	delete vehicle_monitoring_subscription;
	delete v_vehicle_monitoring_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ValidUntil, nullptr);
	}
}

void Siri_SUB_VM_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &unauth_user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"VM Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_VehicleMonitoringSubscriptionRequest;
	
		std::vector<ns2__VehicleMonitoringSubscriptionStructure *> * v_vehicle_monitoring_subscriptions = new std::vector<ns2__VehicleMonitoringSubscriptionStructure *>;
			ns2__VehicleMonitoringSubscriptionStructure* vehicle_monitoring_subscription = new ns2__VehicleMonitoringSubscriptionStructure;
				
					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				vehicle_monitoring_subscription->InitialTerminationTime = termination;

				// ...

		v_vehicle_monitoring_subscriptions->push_back(vehicle_monitoring_subscription);
	req.union_SiriSubscriptionRequestStructure.VehicleMonitoringSubscriptionRequest = v_vehicle_monitoring_subscriptions;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscriptor_ref;
	delete vehicle_monitoring_subscription;
	delete v_vehicle_monitoring_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_EQ(response->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	}
}

void Siri_SUB_VM_Error_SubID_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;

			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"VM Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_VehicleMonitoringSubscriptionRequest;
	
		std::vector<ns2__VehicleMonitoringSubscriptionStructure *> * v_vehicle_monitoring_subscriptions = new std::vector<ns2__VehicleMonitoringSubscriptionStructure *>;
			ns2__VehicleMonitoringSubscriptionStructure* vehicle_monitoring_subscription = new ns2__VehicleMonitoringSubscriptionStructure;

					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				vehicle_monitoring_subscription->InitialTerminationTime = termination;

				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				//vehicle_monitoring_subscription->SubscriptionIdentifier = subscription_ref_struct;

				// ...

		v_vehicle_monitoring_subscriptions->push_back(vehicle_monitoring_subscription);
	req.union_SiriSubscriptionRequestStructure.VehicleMonitoringSubscriptionRequest = v_vehicle_monitoring_subscriptions;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete vehicle_monitoring_subscription;
	delete v_vehicle_monitoring_subscriptions;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(sub_refused_error_text), std::wstring::npos);
	}
}


void Siri_SUB_ET_Main_Lines::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ET Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_EstimatedTimetableSubscriptionRequest;

		std::vector<ns2__EstimatedTimetableSubscriptionStructure *> * v_production_timetable_requests= new std::vector<ns2__EstimatedTimetableSubscriptionStructure *>;
			ns2__EstimatedTimetableSubscriptionStructure* higher_production_timetable_request = new ns2__EstimatedTimetableSubscriptionStructure;
			
				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				higher_production_timetable_request->SubscriptionIdentifier = subscription_ref_struct;

				higher_production_timetable_request->SubscriberRef = subscriptor_ref;

					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

				ns2__EstimatedTimetableRequestStructure* estimated_timetable_request_structure = new ns2__EstimatedTimetableRequestStructure;
					estimated_timetable_request_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
				
						boost::chrono::hours interval(desired_hours);
						boost::chrono::milliseconds ms = boost::chrono::duration_cast<boost::chrono::milliseconds> (interval);
						LONG64* previewinterval = new LONG64(ms.count());
					estimated_timetable_request_structure->PreviewInterval = previewinterval;

					_ns2__EstimatedTimetableRequestStructure_Lines* request_lines_structure = new _ns2__EstimatedTimetableRequestStructure_Lines;
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

					estimated_timetable_request_structure->Lines = request_lines_structure;

			higher_production_timetable_request->EstimatedTimetableRequest = estimated_timetable_request_structure;
		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.EstimatedTimetableSubscriptionRequest = v_production_timetable_requests;

	set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	v_subscription_references.push_back(std::wstring(subscription_ref_struct->__item));
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete previewinterval;
	delete request_lines_structure;
	delete estimated_timetable_request_structure;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ValidUntil, nullptr);
	}
}

void Siri_SUB_ET_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &unauth_user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ET Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_EstimatedTimetableSubscriptionRequest;

		std::vector<ns2__EstimatedTimetableSubscriptionStructure *> * v_production_timetable_requests= new std::vector<ns2__EstimatedTimetableSubscriptionStructure *>;
			ns2__EstimatedTimetableSubscriptionStructure* higher_production_timetable_request = new ns2__EstimatedTimetableSubscriptionStructure;
			
					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

				// ...

		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.EstimatedTimetableSubscriptionRequest = v_production_timetable_requests;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscriptor_ref;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_EQ(response->ErrorCondition->Description->__item, unauth_error_text);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure__, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure___EndpointDeniedAccessError);
		ASSERT_EQ(*response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure__.EndpointDeniedAccessError->ErrorText, unauth_error_text);
	}
}

void Siri_SUB_ET_Error_SubID_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsSubscriptionRequestStructure request;
	ns1__WsSubscriptionAnswerStructure response;

	ns2__SiriSubscriptionRequestStructure req;
	request.Request = &req;

		ns1__WsSubscriptionRequestInfoStructure* info_structure = new ns1__WsSubscriptionRequestInfoStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->ConsumerAddress = &consumer_address;
			
			ns2__ParticipantRefStructure* subscriptor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				subscriptor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = subscriptor_ref;

			ns2__MessageQualifierStructure* message_ref = new ns2__MessageQualifierStructure;
				std::wstringstream ss_message;
				ss_message.imbue(std::locale::classic());
				ss_message << int_distribution(random_generator);
				message_ref->__item = L"ET Message #" + ss_message.str();
			info_structure->MessageIdentifier = message_ref;
		
		request.SubscriptionRequestInfo = info_structure;

		req.__union_SiriSubscriptionRequestStructure = SOAP_UNION__ns2__union_SiriSubscriptionRequestStructure_EstimatedTimetableSubscriptionRequest;

		std::vector<ns2__EstimatedTimetableSubscriptionStructure *> * v_production_timetable_requests= new std::vector<ns2__EstimatedTimetableSubscriptionStructure *>;
			ns2__EstimatedTimetableSubscriptionStructure* higher_production_timetable_request = new ns2__EstimatedTimetableSubscriptionStructure;
				
					boost::gregorian::date today(boost::posix_time::second_clock::universal_time().date());
					boost::posix_time::time_duration tohour(boost::posix_time::hours(16));
					xsd__dateTime termination = GSOAP::from_ptime(boost::posix_time::ptime(today, tohour));
				higher_production_timetable_request->InitialTerminationTime = termination;

				ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
					boost::uuids::uuid uuid = random_uuid_generator();
					std::wstringstream ss_uuid;
					ss_uuid.imbue(std::locale::classic());
					ss_uuid << uuid;
					subscription_ref_struct->__item = ss_uuid.str();
				//higher_production_timetable_request->SubscriptionIdentifier = subscription_ref_struct;

				// ...

		v_production_timetable_requests->push_back(higher_production_timetable_request);
	req.union_SiriSubscriptionRequestStructure.EstimatedTimetableSubscriptionRequest = v_production_timetable_requests;

	//set_proxy(ws);
	ASSERT_EQ(ws.Subscribe(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete message_ref;
	delete subscription_ref_struct;
	delete subscriptor_ref;
	delete higher_production_timetable_request;
	delete v_production_timetable_requests;
	delete info_structure;


	ASSERT_EQ(response.Answer->ResponseStatus.size(), 1);
	for (const auto& response : response.Answer->ResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_ServiceDeliveryErrorConditionStructure___, SOAP_UNION__ns2__union_ServiceDeliveryErrorConditionStructure____ParametersIgnoredError);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ErrorText->find(missing_parameter_error_text + sub_refused_error_text), std::wstring::npos);
		ASSERT_NE(response->ErrorCondition->union_ServiceDeliveryErrorConditionStructure___.ParametersIgnoredError->ParameterName.front().find(sub_refused_error_text), std::wstring::npos);
	}
}
