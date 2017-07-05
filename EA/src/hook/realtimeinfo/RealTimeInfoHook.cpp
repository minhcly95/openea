/*
 * RealTimeInfoHook.cpp
 *
 *  Created on: May 16, 2017
 *      Author: Bui Quang Minh
 */

#include "RealTimeInfoHook.h"
#include <restbed>
#include <thread>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <ostream>

namespace ea {

using namespace restbed;
using namespace boost::property_tree;

EA_TYPEINFO_CUSTOM_IMPL(RealTimeInfoHook) {
	return *ea::TypeInfo("RealTimeInfoHook").
			Add("port", &RealTimeInfoHook::mPort)
			->SetConstructor<RealTimeInfoHook>();
}

/**
 * @class RealTimeInfoHook
 * A Hook which provides real-time information via Web interface.
 * This Hook allows users to access several information of the Population such as status and fitness in real-time
 * by using a Web interface. The Hook will open a server on the given port and users can query the information
 * in JSON format by using a RESTful API.
 *
 * @name{RealTimeInfoHook}
 *
 * @eaml
 * @attr{port, uint - Optional - The port of the server (default in random).}
 * @endeaml
 *
 * @par Web API
 * @dl
 * @attr{/status, Get status of the current Population.}
 * @attr{/fitness, Get fitness summary over a range of generations. (**query**: \tt{"from"}\, \tt{"to"}\, \tt{"res"} (resolution))}
 * @attr{/fitness/\<gen\>, Get fitness summary at a specific generation number.}
 * @attr{/fitness/last, Get fitness summary of the last generation.}
 * @enddl
 */

/**
 * Create a RealTimeInfoHook at the given port number.
 * When the evolution process starts, a server will be opened at the given port.
 * @param pPort The port number of the server (default is random).
 */
RealTimeInfoHook::RealTimeInfoHook(uint pPort) :
	mPort(pPort), mService(), mThread(), mStartGen(0), mFitnessRecords() {

	if (mPort == 0)
		mPort = uniform_int_distribution<uint>(1024, 65535)(Random::generator);

	mSettings = make_shared<Settings>();
    mSettings->set_default_header("Access-Control-Allow-Origin", "*");

    ConfigurateResources();

    mService.set_ready_handler([&] (Service& serv) {
    	EA_LOG_DEBUG<< "RealTimeInfoHook: Server opened at port " << mPort << "." << flush;
    });
}

RealTimeInfoHook::~RealTimeInfoHook() {
}

/**
 * Get the port of the server.
 * @return The port number.
 */
uint RealTimeInfoHook::GetPort() {
	return mPort;
}

void RealTimeInfoHook::DoStart() {
	mStartGen = GetGeneration();

    mSettings->set_port(mPort);
    mThread = thread(bind(&Service::start, ref(mService), mSettings));
}

void RealTimeInfoHook::DoEnd() {
	mService.stop();
	mThread.join();

	EA_LOG_DEBUG<< "RealTimeInfoHook: Server closed." << flush;
	mFitnessRecords.clear();
}

void RealTimeInfoHook::DoGenerational() {
	OrganismPoolPtr pool = GetMainPool();

	FitnessRecord record;
	record.max = -INFINITY;
	record.min = INFINITY;
	record.mean = 0;
	record.sd = 0;

	for (OrganismPtr org : *pool) {
		double fitness = org->GetFitnessValue();
		if (fitness > record.max)
			record.max = fitness;
		if (fitness < record.min)
			record.min = fitness;
		record.mean += fitness;
		record.sd += fitness * fitness;
	}

	record.mean /= pool->size();
	record.sd = sqrt(record.sd / pool->size() - record.mean * record.mean);

	mFitnessRecords.push_back(record);
}

void RealTimeInfoHook::ConfigurateResources() {
	Ptr<Resource> resource;

	resource = make_shared<Resource>();
	resource->set_path("/status");
	resource->set_method_handler("GET", bind(&RealTimeInfoHook::StatusHandler, this, _1));
	mService.publish(resource);

	resource = make_shared<Resource>();
	resource->set_path("/fitness");
	resource->set_method_handler("GET", bind(&RealTimeInfoHook::FitnessHandler, this, _1));
	mService.publish(resource);

	resource = make_shared<Resource>();
	resource->set_paths({"/fitness/{at: [0-9]*}", "/fitness/{cmd: last}"});
	resource->set_method_handler("GET", bind(&RealTimeInfoHook::FitnessAtHandler, this, _1));
	mService.publish(resource);
}

void RealTimeInfoHook::Response(const Ptr<restbed::Session>& session, ptree &data) {
	ostringstream oss;
	json_parser::write_json(oss, data);

	session->close(OK, oss.str(),
			{ {"Content-Length", to_string(oss.tellp())},
				{"Content-Type", "application/json"}});
}

void RealTimeInfoHook::StatusHandler(const Ptr<restbed::Session> session) {
	ptree data;
	data.put("generation", GetGeneration());
	data.put("evaluation", GetEvaluation());

	// Response
	Response(session, data);
}

void RealTimeInfoHook::FitnessHandler(const Ptr<restbed::Session> session) {
	const auto request = session->get_request();
	llong from = 0, to = numeric_limits<llong>::max();
	int res = EA_REALTIMEINFO_DEFAULT_RESOLUTION;

	// Parsing
	try {
		if (request->has_query_parameter("from"))
			from = stoll(request->get_query_parameter("from"));
		if (request->has_query_parameter("to"))
			to = stoll(request->get_query_parameter("to"));
		if (request->has_query_parameter("res"))
			res = stoi(request->get_query_parameter("res"));
	} catch (exception&) {
		session->close(400, "");
		return;
	}

	// Validity check
	if (from > to || from < 0 || to < 0 || res <= 0) {
		session->close(400, "");
		return;
	}

	if (from < (llong)mStartGen) {
		from = mStartGen;
		if (to < from) to = from;
	}
	if (to >= (llong)(mStartGen + mFitnessRecords.size())) {
		to = mStartGen + mFitnessRecords.size() - 1;
		if (from > to) from = to;
	}

	// Resolution calculation
	llong delta = to - from;
	if (delta + 1 < res)
		res = delta + 1;

	int realRes = (res == 1 ? 1 : res - 1);
	int step = delta / realRes;
	double remain = (delta - (llong)step * realRes) / (double)realRes;

	// Put data in
	ptree tree;

	double excess = 0;
	llong gen = (res == 1 ? to : from);
	for (int i = 0; i < res; i++, gen += step) {
		ptree data;
		llong realGen = gen + (excess >= 0.5 ? 1 : 0);
		FitnessRecord record = mFitnessRecords[realGen - mStartGen];
		data.put("max", record.max);
		data.put("min", record.min);
		data.put("mean", record.mean);
		data.put("sd", record.sd);
		tree.put_child(to_string(realGen), data);

		excess += remain;
		if (excess >= 1) {
			excess--;
			gen++;
		}
	}

	// Response
	Response(session, tree);
}

void RealTimeInfoHook::FitnessAtHandler(const Ptr<restbed::Session> session) {
	const auto request = session->get_request();
	llong at = 0;

	// Parsing
	try {
		if (request->has_path_parameter("at"))
			at = stoll(request->get_path_parameter("at"));
		else if (request->has_path_parameter("cmd")) {
			if (request->get_path_parameter("cmd") == "last")
				at = mStartGen + mFitnessRecords.size() - 1;
			else throw exception();
		}
		else throw exception();
	} catch (exception&) {
		session->close(400, "");
		return;
	}

	if (at < 0) {
		session->close(400, "");
		return;
	}

	if (at < (llong)mStartGen || at >= (llong)(mStartGen + mFitnessRecords.size())) {
		session->close(404, "");
		return;
	}

	FitnessRecord record = mFitnessRecords[at - mStartGen];
	ptree data;
	data.put("generation", at);
	data.put("max", record.max);
	data.put("min", record.min);
	data.put("mean", record.mean);
	data.put("sd", record.sd);

	// Response
	Response(session, data);
}

}
