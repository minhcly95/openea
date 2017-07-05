/*
 * Session.h
 *
 *  Created on: May 27, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include <atomic>
#include <chrono>

namespace ea {

using namespace std;

class Session final {
public:
	Session(PopulationPtr pPopulation, StrategyPtr pStrategy);

	const PopulationPtr& GetPopulation() const;
	const StrategyPtr& GetStrategy() const;

	vector<pair<string, float>> GetTimeRecords(vector<string> pOrder = { }) const;
	float GetTotalTime() const;

	bool IsRunning() const;
	void Terminate();

private:
	PopulationPtr mPopulation;
	StrategyPtr mStrategy;

	using MilisFloat = chrono::duration<float, milli>;
	struct TimeRecord {
		MilisFloat totalTime;
		uint count;
		TimeRecord() : totalTime(0), count(0) { }
		float Average() const { return totalTime.count() / count; }

		void Add(MilisFloat duration) {
			totalTime += duration;
			count++;
		}
	};
	map<string, TimeRecord> mTime;
	TimeRecord mTotalTime;

	atomic<bool> mRunning;

	template<class ReturnT, class... Args>
	ReturnT	Measure(string id, function<ReturnT(Args...)> pFunc, Args&&... args) {
		chrono::time_point<chrono::high_resolution_clock> t;
		if (!id.empty())
			t = chrono::high_resolution_clock::now();

		ReturnT rValue = pFunc(std::forward(args)...);

		if (!id.empty())
			mTime[id].Add(MilisFloat(chrono::high_resolution_clock::now() - t));

		return rValue;
	}
	template<class... Args>
	void Measure(string id, function<void(Args...)> pFunc, Args&&... args) {
		chrono::time_point<chrono::high_resolution_clock> t;
		if (!id.empty())
			t = chrono::high_resolution_clock::now();

		pFunc(std::forward(args)...);

		if (!id.empty())
			mTime[id].Add(MilisFloat(chrono::high_resolution_clock::now() - t));
	}
	void MeasureTotalTime(function<void(void)> pFunc);

	friend class Strategy;
};

}
