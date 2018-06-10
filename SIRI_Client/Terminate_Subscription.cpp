#include "stdafx.h"
#include "Terminate_Subscription.h"

TEST_F(Siri_ClientTestFixture, TSUB_Error_Unauth)				{ Siri_TSUB_Error_Unauth			TSUB;		TSUB.run();	}
TEST_F(Siri_ClientTestFixture, TSUB_Error_SubID_Missing)		{ Siri_TSUB_Error_SubID_Missing		TSUB;		TSUB.run();	}
TEST_F(Siri_ClientTestFixture, TSUB_One)						{ Siri_TSUB_One						TSUB;		TSUB.run();	}
TEST_F(Siri_ClientTestFixture, TSUB_All)						{ Siri_TSUB_All						TSUB;		TSUB.run();	}

void Siri_TSUB_One::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsDeleteSubscriptionRequestStructure request;
	ns1__WsDeleteSubscriptionAnswerStructure response;

	ns2__TerminateSubscriptionRequestBodyStructure req;
	request.Request = &req;

		ns2__RequestStructure* info_structure = new ns2__RequestStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->Address = &consumer_address;

			ns2__ParticipantRefStructure* requestor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				requestor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = requestor_ref;
			// ...

		request.DeleteSubscriptionInfo = info_structure;

		req.SubscriberRef = requestor_ref;
		req.__union_TerminateSubscriptionRequestBodyStructure = SOAP_UNION__ns2__union_TerminateSubscriptionRequestBodyStructure_SubscriptionRef;
		
		std::vector<ns2__SubscriptionQualifierStructure *>* v_termination_requests_struct = new std::vector<ns2__SubscriptionQualifierStructure *>;
			ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
				std::vector<std::wstring>::const_iterator it;
				if (!v_subscription_references.empty())
				{
					it = v_subscription_references.begin();
					std::uniform_int_distribution<std::size_t> random_ref(0, v_subscription_references.size() - 1);
					std::advance(it, random_ref(random_generator));
					subscription_ref_struct->__item = *it;
				}
				else
					subscription_ref_struct->__item = L"5908c305-6cbb-4817-8c95-22de48c6772b";	// Avoids crash test if v_termination_requests_struct.empty()
		v_termination_requests_struct->push_back(subscription_ref_struct);
	req.union_TerminateSubscriptionRequestBodyStructure.SubscriptionRef = v_termination_requests_struct;

	set_proxy(ws);
	ASSERT_EQ(ws.DeleteSubscription(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete requestor_ref;
	delete info_structure;
	delete subscription_ref_struct;
	delete v_termination_requests_struct;

	ASSERT_EQ(response.Answer->TerminationResponseStatus.size(), 1);
	for (const auto& response : response.Answer->TerminationResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_EQ(response->Status, true);
	}

	if(!v_subscription_references.empty())
		v_subscription_references.erase(it);
}

void Siri_TSUB_All::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsDeleteSubscriptionRequestStructure request;
	ns1__WsDeleteSubscriptionAnswerStructure response;

	ns2__TerminateSubscriptionRequestBodyStructure req;
	request.Request = &req;

		ns2__RequestStructure* info_structure = new ns2__RequestStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->Address = &consumer_address;

			ns2__ParticipantRefStructure* requestor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				requestor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = requestor_ref;
			// ...

		request.DeleteSubscriptionInfo = info_structure;

	req.SubscriberRef = requestor_ref;
	req.__union_TerminateSubscriptionRequestBodyStructure = SOAP_UNION__ns2__union_TerminateSubscriptionRequestBodyStructure_All;
	req.union_TerminateSubscriptionRequestBodyStructure.All = ns2__EmptyType___x0000;

	set_proxy(ws);
	ASSERT_EQ(ws.DeleteSubscription(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete requestor_ref;
	delete info_structure;


	ASSERT_EQ(response.Answer->TerminationResponseStatus.size(), 1);
	for (const auto& response : response.Answer->TerminationResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->ErrorCondition, nullptr);
		ASSERT_EQ(response->Status, true);
	}

	v_subscription_references.clear();
}

void Siri_TSUB_Error_Unauth::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsDeleteSubscriptionRequestStructure request;
	ns1__WsDeleteSubscriptionAnswerStructure response;

	ns2__TerminateSubscriptionRequestBodyStructure req;
	request.Request = &req;

		ns2__RequestStructure* info_structure = new ns2__RequestStructure;
			info_structure->AccountId = &unauth_user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->Address = &consumer_address;

			ns2__ParticipantRefStructure* requestor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				requestor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = requestor_ref;
			// ...

		request.DeleteSubscriptionInfo = info_structure;

		req.SubscriberRef = requestor_ref;

	//set_proxy(ws);
	ASSERT_EQ(ws.DeleteSubscription(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete requestor_ref;
	delete info_structure;


	ASSERT_EQ(response.Answer->TerminationResponseStatus.size(), 1);
	for (const auto& response : response.Answer->TerminationResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_EQ(response->ErrorCondition->Description->__item, unknown_subscriber_error_text);
		ASSERT_EQ(response->ErrorCondition->__union_TerminationResponseStatusStructure_ErrorCondition, SOAP_UNION__ns2__union_TerminationResponseStatusStructure_ErrorCondition_UnknownSubscriberError);
		ASSERT_EQ(*response->ErrorCondition->union_TerminationResponseStatusStructure_ErrorCondition.UnknownSubscriberError->ErrorText, unknown_subscriber_error_text);
		
		ASSERT_EQ(response->Status, false);
	}
}

void Siri_TSUB_Error_SubID_Missing::run()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsDeleteSubscriptionRequestStructure request;
	ns1__WsDeleteSubscriptionAnswerStructure response;

	ns2__TerminateSubscriptionRequestBodyStructure req;
	request.Request = &req;

		ns2__RequestStructure* info_structure = new ns2__RequestStructure;
			info_structure->AccountId = &user;
			info_structure->AccountKey = &pwd;
			info_structure->RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
			info_structure->Address = &consumer_address;

			ns2__ParticipantRefStructure* requestor_ref = new ns2__ParticipantRefStructure;
				std::wstringstream ss;
				ss.imbue(std::locale::classic());
				ss << int_distribution(random_generator);
				requestor_ref->__item = L"Subscriber #" + ss.str();
			info_structure->RequestorRef = requestor_ref;
			// ...

		request.DeleteSubscriptionInfo = info_structure;

		req.SubscriberRef = requestor_ref;
		req.__union_TerminateSubscriptionRequestBodyStructure = SOAP_UNION__ns2__union_TerminateSubscriptionRequestBodyStructure_SubscriptionRef;
		
		std::vector<ns2__SubscriptionQualifierStructure *>* v_termination_requests_struct = new std::vector<ns2__SubscriptionQualifierStructure *>;
			ns2__SubscriptionQualifierStructure* subscription_ref_struct = new ns2__SubscriptionQualifierStructure;		// Subscription identifier
				// ...
		//v_termination_requests_struct->push_back(subscription_ref_struct);
	req.union_TerminateSubscriptionRequestBodyStructure.SubscriptionRef = v_termination_requests_struct;

	//set_proxy(ws);
	ASSERT_EQ(ws.DeleteSubscription(server_address.c_str(), nullptr, &request, response), SOAP_OK) << "## Server might not be running ##";

	delete requestor_ref;
	delete info_structure;
	delete subscription_ref_struct;
	delete v_termination_requests_struct;


	ASSERT_EQ(response.Answer->TerminationResponseStatus.size(), 1);
	for (const auto& response : response.Answer->TerminationResponseStatus)
	{
		ASSERT_NE(response, nullptr);
		ASSERT_EQ(response->Status, false);
		ASSERT_NE(response->ErrorCondition, nullptr);
		ASSERT_NE(response->ErrorCondition->Description, nullptr);
		ASSERT_NE(response->ErrorCondition->Description->__item.find(unknown_subscription_error_text), std::wstring::npos);
		ASSERT_EQ(response->ErrorCondition->__union_TerminationResponseStatusStructure_ErrorCondition, SOAP_UNION__ns2__union_TerminationResponseStatusStructure_ErrorCondition_UnknownSubscriptionError);
		ASSERT_NE(response->ErrorCondition->union_TerminationResponseStatusStructure_ErrorCondition.UnknownSubscriptionError->ErrorText->find(unknown_subscription_error_text), std::wstring::npos);
	}
}
