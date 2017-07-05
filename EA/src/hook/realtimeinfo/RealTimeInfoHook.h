/*
 * RealTimeInfoHook.h
 *
 *  Created on: May 16, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Core.h"
#include "../../EA/Utility.h"
#include <restbed>
#include <boost/property_tree/ptree.hpp>

namespace ea {

using namespace restbed;

#define EA_REALTIMEINFO_DEFAULT_RESOLUTION	100

class RealTimeInfoHook : public Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	RealTimeInfoHook(uint pPort = 0);
	virtual ~RealTimeInfoHook();

	uint GetPort();

protected:
	virtual void DoStart() override;
	virtual void DoEnd() override;
	virtual void DoGenerational() override;

private:
	uint mPort;
	Service mService;
	Ptr<Settings> mSettings;
	thread mThread;

	ullong mStartGen;

	struct FitnessRecord {
		double max, min, mean, sd;
	};
	vector<FitnessRecord> mFitnessRecords;

	void ConfigurateResources();

	void Response(const Ptr<restbed::Session>& session, boost::property_tree::ptree &data);

	void StatusHandler(const Ptr<restbed::Session> session);
	void FitnessHandler(const Ptr<restbed::Session> session);
	void FitnessAtHandler(const Ptr<restbed::Session> session);
};

}
