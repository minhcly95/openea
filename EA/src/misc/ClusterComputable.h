/*
 * ClusterComputableDef.h
 *
 *  Created on: Jun 1, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"

namespace ea {

using namespace std;

class Cluster;

/**
 * Base class for all ClusterComputable template.
 * This class provides a way to generalize ClusterComputable template classes, so
 * Cluster class can process them using inheritance polymorphism instead of templating
 * one. This class also provides cluster operator ID management function such as GetClusterOpId().
 *
 * @see ClusterComputable
 * @see Cluster
 */
class ClusterComputableBase {
private:
	int mClusterOpId;

	inline void SetClusterOpId(int pClusterOpId) {
		mClusterOpId = pClusterOpId;
	}
	virtual function<void(int, int)> SlaveFunction() = 0;

public:
	/**
	 * Create an operator with ID equal -1.
	 * This ID will only be changed when the operator is added to the Cluster
	 * using Cluster::AddOperator(). The ID can be queried by GetClusterOpId().
	 */
	inline ClusterComputableBase() : mClusterOpId(-1) { }
	inline virtual ~ClusterComputableBase() { }

	/**
	 * Get the ID of this cluster operator.
	 * @return -1 if the operator is not added to the Cluster, otherwise an positive ID number.
	 */
	inline int GetClusterOpId() {
		return mClusterOpId;
	}

	friend class Cluster;
};

/**
 * The interface for operators which are computable on cluster.
 * Classes which have the capability to parallelize on cluster should derive from this interface.
 * An example is IndividualEvaluator which provides cluster computation on the evaluation of each individual.
 * Cluster parallelization is only suitable for task which take a very long time to finish
 * (e.g. more than 5 seconds). It is recommended that users should conduct experiments between cases.
 *
 * When deriving from this interface, child classes need to override ProcessOnRemote() with
 * the processing code to be run on slave nodes. @tt{InputT} objects will be sent from the master node
 * to the slave nodes, be processed on those nodes and @tt{OutputT} objects will be returned as the result.
 * Then, whenever cluster processing is needed,
 * call ExecuteInCluster() to apply that segment of processing code on a data list.
 * If cluster is not enable of the operator is not added to the Cluster, multi-threading
 * will be used instead unless explicitly specified.
 *
 * @tparam InputT The type of objects to be received from master node (input of processing).
 * @tparam OutputT The type of objects to be sent to master node (output of processing).
 *
 * @see ProcessOnRemote()
 * @see ExecuteInCluster()
 * @see Cluster::AddOperator()
 */
template <class InputT, class OutputT>
class ClusterComputable : public ClusterComputableBase {
protected:
	/**
	 * Typedef for callback function used in ExecuteInCluster().
	 * CallbackFunction requires 3 arguments:
	 * - An uint number served as the index number of the element in the input data list.
	 * - The @tt{InputT} object sent to the slave node.
	 * - The @tt{OutputT} object received from the slave node.
	 * This function type return nothing.
	 */
	using CallbackFunction = function<void(uint, const InputT&, const OutputT&)>;

	/**
	 * Apply ProcessOnRemote() on a list of data of type @tt{InputT}.
	 * By calling this function, the input data list will be processed by the segment of code
	 * defined in ProcessOnRemote() function. This segment of code will be executed on
	 * slave nodes and the output data will be aggregated into an output data list by the master node.
	 *
	 * If Cluster::IsEnabled() is false or the operator is not added to the Cluster,
	 * MultiThreading is used instead to provide parallelism. This can be disabled by using the
	 * second argument. The third argument is a callback function, invoked when the master node
	 * received a result from slave nodes. This may be useful when counting evaluation number for example.
	 *
	 * @param pInputArray The input data list to be processed.
	 * @param multiThreading Whether MultiThreading should be used when cluster computation is disabled.
	 * @param pCallback Callback function invoked when a result is received.
	 * @return The output data list.
	 */
	inline vector<OutputT> ExecuteInCluster(const vector<InputT>& pInputArray,
			bool multiThreading = true, CallbackFunction pCallback = { });

	/**
	 * The segment of code to be processed remotely.
	 * This function needs to be overridden in child classes.
	 * The segment of code defined in this function will be executed remotely on slave node.
	 * Therefore, any state of the operator must be considered in a distributed context.
	 * This segment of code will be invoked when ExecuteInCluster() is called on the master node.
	 *
	 * @param pInput The input data to be processed.
	 * @return The output data.
	 */
	virtual OutputT ProcessOnRemote(const InputT& pInput) = 0;

public:
	inline ClusterComputable() = default;
	inline virtual ~ClusterComputable() { }

private:
	virtual function<void(int, int)> SlaveFunction() override final;
};

}
