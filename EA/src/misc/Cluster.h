/*
 * Cluster.h
 *
 *  Created on: May 31, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include <csignal>
#include "ClusterComputable.h"

namespace ea {

#define EA_CLUSTER_TAG_RESERVED 10

using namespace std;

class Cluster {
private:
	using ClusterFunction = function<void(int, int)>;

	static bool sEnabled;
	static vector<ClusterFunction> sOperators;
	static ClusterFunction sCurrentOp;

	static void SlaveRoutine();
	static void AddOperatorBase(const Ptr<ClusterComputableBase>& pOp);

	static void Load(int pOp);
	static void Unload();

public:
	static void SetEnabled(bool pEnabled);
	static bool IsEnabled();

	template <class T>
	inline static void AddOperator(const Ptr<T>& pOp) {
		static_assert(is_base_of<ClusterComputableBase, T>::value,
				"Cluster::AddClusterOperator<T>(): T must be ClusterComputable.");
		AddOperatorBase(static_pointer_cast<ClusterComputableBase>(pOp));
	}

	static void Deploy();
	static void Destroy();

	template <class InputT, class OutputT>
	friend class ClusterComputable;
};

} /* namespace ea */
