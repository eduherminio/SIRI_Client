#include "stdafx.h"
#include "BaseSIRIClient.h"

BaseSIRIClient::BaseSIRIClient()
{
	random_generator.seed((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
	int_distribution = std::uniform_int_distribution<int>(0, 10);

	load_available_lines();
	load_available_stops();

	load_v_subs();
}

BaseSIRIClient::~BaseSIRIClient()
{
	save_v_subs();
}

void BaseSIRIClient::set_proxy(SiriProducerDocBindingProxy& srv)
{
	std::string		proxy_host("localhost");
	int				proxy_port(8888);

	if (siri_proxy == true)
	{
		srv.soap->proxy_host = proxy_host.c_str();
		srv.soap->proxy_port = proxy_port;
	}
}

const CLines BaseSIRIClient::get_random_line()
{
	std::uniform_int_distribution<int> rnd(0, (int)available_lines.size() - 1);

	return random_lines
		? available_lines.empty() ? CLines(L"") : available_lines[rnd(random_generator)]
		: CLines(desired_line);
}

const CStops BaseSIRIClient::get_random_stop()
{
	std::uniform_int_distribution<int> rnd(0, (int)available_stops.size() - 1);

	return random_stops
		? available_stops.empty() ? CStops(L"") : available_stops[rnd(random_generator)]
		: CStops(desired_stop);
}

void BaseSIRIClient::load_available_lines()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsLinesDiscoveryStructure request;
	ns1__WsLinesDiscoveryAnswerStructure response;

	ns2__LinesDiscoveryRequestStructure req;
	request.Request = &req;

	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.AccountId = &user;
	req.AccountKey = &pwd;

	//set_proxy(ws);
	if (ws.LinesDiscovery(server_address.c_str(), nullptr, &request, response) == SOAP_OK
		&& response.Answer->ErrorCondition == nullptr
		&& !response.Answer->AnnotatedLineRef.empty())
	{
		for (const auto& line : response.Answer->AnnotatedLineRef)
		{
			if (line != nullptr
				&& line->LineRef != nullptr
				&& !line->LineRef->__item.empty())
			{
				assert(line->LineRef->__item.find(L"L") != std::wstring::npos);													// TO-CHECK: L2 -> 2
				std::wstring lineref(line->LineRef->__item.begin() + line->LineRef->__item.find(L"L") + 1,
					line->LineRef->__item.end());
				available_lines.push_back(CLines(lineref));
			}
		}
	}
}

void BaseSIRIClient::load_available_stops()
{
	SiriProducerDocBindingProxy ws;

	ns1__WsStopPointsDiscoveryStructure request;
	ns1__WsStopPointsDiscoveryAnswerStructure response;

	ns2__StopPointsDiscoveryRequestStructure req;
	request.Request = &req;

	req.RequestTimestamp = GSOAP::from_ptime(boost::posix_time::second_clock::universal_time());
	req.AccountId = &user;
	req.AccountKey = &pwd;

	//set_proxy(ws);
	if (ws.StopPointsDiscovery(server_address.c_str(), nullptr, &request, response) == SOAP_OK
		&& response.Answer->ErrorCondition == nullptr
		&& !response.Answer->AnnotatedStopPointRef.empty())
	{
		for (const auto& stop : response.Answer->AnnotatedStopPointRef)
		{
			if (stop != nullptr
				&& stop->StopPointRef != nullptr
				&& !stop->StopPointRef->__item.empty())
			{
				std::wstring stoppointref(stop->StopPointRef->__item);
				available_stops.push_back(CStops(stoppointref));
			}
		}
	}
}

void BaseSIRIClient::load_v_subs()
{
	_tstring tmp_path = _T("c:/temp/siri/v_subs.txt");
	v_sub_path = tmp_path;

	std::ifstream ifs(v_sub_path.string());

	std::string str;
	while (getline(ifs, str))
	{
		if (str.empty())
			continue;
		std::wstring wstr(static_cast<const wchar_t*>(saer::ca2cw(str.c_str())));
		v_subscription_references.push_back(wstr);
	}

	ifs.close();
}

void BaseSIRIClient::save_v_subs()
{
	_ofstream ofs(v_sub_path.string()/*, std::ios::app*/);
	ofs.imbue(loc);

	for (const auto& sub : v_subscription_references)
		ofs << sub << std::endl;
	ofs.close();
}
