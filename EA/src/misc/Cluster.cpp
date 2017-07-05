/*
 * Cluster.cpp
 *
 *  Created on: May 31, 2017
 *      Author: Bui Quang Minh
 */

#include "Cluster.h"
#include "../EA/Type/Utility.h"
#include "ClusterComputableImpl.h"

namespace ea {

bool Cluster::sEnabled = false;
vector<Cluster::ClusterFunction> Cluster::sOperators;
Cluster::ClusterFunction Cluster::sCurrentOp;

/**
 * @class Cluster
 * Static class providing cluster computation feature.
 * To user cluster computation feature, first a ClusterComputable object is needed (e.g. IndividualEvaluator).
 * Then that object needs to be registered by using AddOperator() function, before the first call of Strategy::Evolve().
 * When everything is set up, call Strategy::Evolve() as normal.
 * Finally, call Destroy() to shut down the cluster. The process can be described in this segment of code:
 *
 * @code
 * SomeStrategyPtr strategy = make_shared<SomeStrategy>();
 * SomeOperatorPtr op = strategy->someOp.Create<SomeOperator>();
 * ...
 * Cluster::AddOperator(op);
 * strategy->Evolve();
 * Cluster::Destroy();
 * @endcode
 *
 * When launch the program with cluster computation feature, users must use the command:
 *
 * @code
 * openea cluster (local|remote) exec <the_executable>
 * @endcode
 *
 * to start it in cluster mode locally or remotely. With an EAML file, all ClusterComputable objects
 * will be added automatically if users run that file in cluster mode using this command:
 *
 * @code
 * openea cluster (local|remote) run <EAML_file> <options>
 * @endcode
 *
 * Cluster computation requires OpenMPI library. Be sure that every host in the cluster has installed it.
 * Remote cluster deployment requires a host file, which can be edited by using:
 *
 * @code
 * openea edit cluster
 * @endcode
 *
 * @see ClusterComputable
 */

/**
 * Enable or disable cluster computation feature.
 * If enabled, the program must be launched by using:
 *
 * @code
 * openea cluster (local|remote) exec <the_executable>
 * @endcode
 *
 * Otherwise, an error will be thrown.
 * AddOperator() function will also automatically enable cluster computation.
 *
 * @param pEnabled true to enable the cluster, false otherwise.
 */
void Cluster::SetEnabled(bool pEnabled) {
	sEnabled = pEnabled;
}
/**
 * Whether cluster computation is enabled.
 * @return true if cluster computation is enabled.
 */
bool Cluster::IsEnabled() {
	return sEnabled;
}

void Cluster::Load(int pOp) {
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (int i = 1; i < size; i++)
		MPI_Send(&pOp, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
}
void Cluster::Unload() {
	char tmp;

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (int i = 1; i < size; i++)
		MPI_Send(&tmp, 1, MPI_BYTE, i, 2, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
}

/**
 * @fn void Cluster::AddOperator(const Ptr<T>& pOp)
 * Add an operator to the cluster.
 * The given operator must be a child of ClusterComputable.
 * Cluster is capable of supporting multiple operators at once, so users can add an
 * arbitrary number of operators by using this function.
 * The added operator will be assigned an ID number (which can be get by using
 * ClusterComputable::GetClusterOpId()). When the given operator is encountered in master node,
 * it will load its counterpart on slave nodes by using this ID.
 * @tparam T Must be a child of ClusterComputable.
 * @param pOp The operator to be added.
 */

/**
 * Deploy the cluster.
 * After calling this function, the cluster is initialized and specialized into master and slave nodes.
 * Users cannot add any additional operator using AddOperator() after this point.
 * Subsequent calls of this function will be ignored.
 * This function is called automatically by Strategy::Evolve(), therefore users
 * don't need to call it manually.
 */
void Cluster::Deploy() {
	if (!Cluster::IsEnabled())
		return;

	int flag;
	MPI_Initialized(&flag);

	if (flag)
		return;

	MPI_Init(0, 0);
	MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size == 1)
		throw EA_EXCEPTION(EAException, CLUSTER_CANNOT_DEPLOY,
				"Cannot deploy cluster for a single node.");

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank != 0)
		SlaveRoutine();
	else
		EA_LOG_DEBUG << "Cluster deployed, size = " << size << flush;
}

void Cluster::AddOperatorBase(const Ptr<ClusterComputableBase>& pOp) {
	pOp->SetClusterOpId(sOperators.size());
	sOperators.push_back(pOp->SlaveFunction());
	SetEnabled(true);
}

void Cluster::SlaveRoutine() {
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	Log::Redirect(Log::TRACE, "stdout");
	Log::Redirect(Log::DEBUG, "stdout");

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char name[100];
	int nameLen;
	MPI_Get_processor_name(name, &nameLen);
	string pc(name, nameLen);
	EA_LOG_TRACE << "Cluster Slave node #" + to_string(rank) + " on " + pc + " started" << flush;

	while (true) {
		MPI_Status status;
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD,  &status);

		int tag = status.MPI_TAG;

		// Terminate
		if (tag == 0) {
			char tmp;
			MPI_Recv(&tmp, 1, MPI_BYTE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			break;
		}
		// Load function
		else if (tag == 1) {
			int opId;
			MPI_Recv(&opId, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sCurrentOp = sOperators[opId];
			MPI_Barrier(MPI_COMM_WORLD);
			continue;
		}
		// Unload function
		else if (tag == 2) {
			char tmp;
			MPI_Recv(&tmp, 1, MPI_BYTE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sCurrentOp = { };
			MPI_Barrier(MPI_COMM_WORLD);
			continue;
		}

		if (!sCurrentOp)
			throw EA_EXCEPTION(EAException, CLUSTER_OPERATOR_NOT_LOADED,
					"Cluster operator have not been loaded in cluster slave node #" + to_string(rank));

		int count;
		MPI_Get_count(&status, MPI_BYTE, &count);

		sCurrentOp(count, tag);
	}

	// End protocol
	MPI_Finalize();
	EA_LOG_TRACE<< "Cluster Slave node #" + to_string(rank) + " on " + pc + " shut down" << flush;
	exit(0);
}

/**
 * Destroy the cluster.
 * Calling this function will terminate the cluster by sending stop signal to slave nodes.
 * After this point, no further calculation on cluster is allowed.
 * Subsequent calls will throw an error.
 * This function must be called manually by users.
 */
void Cluster::Destroy() {
	int flag;
	MPI_Finalized(&flag);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (IsEnabled() && !flag && rank == 0) {
		char tmp;
		for (int i = 1; i < size; i++) {
			MPI_Send(&tmp, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}
		MPI_Finalize();
		EA_LOG_TRACE << "Cluster Master node shut down" << flush;
	}
}

} /* namespace ea */
