/*
 * PopulationCluster.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "Population.h"

#include "../EA/Core.h"
#include <thread>

#include <mpi.h>
#include <csignal>

using namespace std;

namespace ea {

#ifdef EA_POPULATION_CLUSTER
#define TAG_RESERVED 10

void Population::DeployCluster() {
	if (!IsReady())
		throw EA_EXCEPTION(EAException, POPULATION_NOT_READY,
				"Population is not yet ready, operators are missing.");

	if (MPI::Is_initialized())
		return;

	MPI::Init();
	MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);

	if (MPI::COMM_WORLD.Get_size() == 1)
		throw EA_EXCEPTION(EAException, CLUSTER_CANNOT_DEPLOY,
				"Cannot deploy cluster for a single node.");

	if (MPI::COMM_WORLD.Get_rank() != 0)
		ClusterSlaveRoutine();
}

void Population::ClusterSlaveRoutine() {
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	Log::Redirect(Log::TRACE, "stdout");
	Log::Redirect(Log::DEBUG, "stdout");

	int rank = MPI::COMM_WORLD.Get_rank();
	char name[100];
	int nameLen;
	MPI::Get_processor_name(name, nameLen);
	string pc(name, nameLen);
	LOG_TRACE << "Cluster Slave node #" + to_string(rank) + " on " + pc + " started" << flush;

	while (true) {
		MPI::Status status;
		MPI::COMM_WORLD.Probe(0, MPI::ANY_TAG, status);

		int tag = status.Get_tag();

		if (tag == 0)
			break;
		if (tag == 1) {
			long long est;
			MPI::COMM_WORLD.Recv(&est, 1, MPI::LONG_LONG, 0, tag);
			std::this_thread::sleep_for(chrono::nanoseconds(est));

			while (!MPI::COMM_WORLD.Iprobe(0, MPI::ANY_TAG, status)) {
				std::this_thread::sleep_for(chrono::nanoseconds(est / 10));
			}
			continue;
		}

		int count = status.Get_count(MPI::BYTE);
		char* buffer = new char[count];
		MPI::COMM_WORLD.Recv(buffer, count, MPI::BYTE, 0, tag);

		istringstream iss(string(buffer, count), ios::binary);
		GenomePtr genome = BinarySerializer<GenomePtr>::Read(iss);

		FitnessPtr fitness = GetEvaluator()->EvaluateFitness(genome);
		ostringstream oss(ios::binary);
		BinarySerializer<FitnessPtr>::Write(oss, fitness);
		const string data = oss.str();

		MPI::COMM_WORLD.Send(data.c_str(), data.length(), MPI::BYTE, 0, tag);
	}

	// End protocol
	char tmp;
	MPI::COMM_WORLD.Recv(&tmp, 1, MPI::BYTE, 0, 0);
	MPI::Finalize();
	LOG_TRACE<< "Cluster Slave node #" + to_string(rank) + " on " + pc + " shut down" << flush;
	exit(0);
}

void Population::ClusterMasterRoutine(const VectorPtr<GenomePtr>& pGenomePool,
		const OrganismPoolPtr& pDestPool) {
	uint poolSize = pGenomePool->size();
	uint slaveCount = MPI::COMM_WORLD.Get_size() - 1;

	uint sent = min(poolSize, slaveCount);

	// First deployment
	for (uint i = 0; i < sent; i++) {
		ostringstream oss(ios::binary);
		BinarySerializer<GenomePtr>::Write(oss, (*pGenomePool)[i]);
		const string data = oss.str();
		MPI::COMM_WORLD.Send(data.c_str(), data.length(), MPI::BYTE, i + 1,
				i + TAG_RESERVED);
	}

	// Loop
	for (uint received = 0; received < poolSize; received++, mEvaluation++) {
		MPI::Status status;
		MPI::COMM_WORLD.Probe(MPI::ANY_SOURCE, MPI::ANY_TAG, status);

		int source = status.Get_source();
		int tag = status.Get_tag();
		int count = status.Get_count(MPI::BYTE);
		char* buffer = new char[count];
		MPI::COMM_WORLD.Recv(buffer, count, MPI::BYTE, source, tag);

		istringstream iss(string(buffer, count), ios::binary);
		FitnessPtr fitness = BinarySerializer<FitnessPtr>::Read(iss);
		pDestPool->push_back(
				make_shared<Organism>((*pGenomePool)[tag - TAG_RESERVED], fitness));

		if (sent < poolSize) {
			ostringstream oss(ios::binary);
			BinarySerializer<GenomePtr>::Write(oss, (*pGenomePool)[sent]);
			const string data = oss.str();
			MPI::COMM_WORLD.Send(data.c_str(), data.length(), MPI::BYTE, source,
					sent + TAG_RESERVED);
			sent++;
		}
		else {
			uint delta = mGeneration - mStartGen;
			if (delta > 0) {
				long long est = chrono::duration_cast<chrono::nanoseconds>((totalTime - evalTime) / delta).count();
				MPI::COMM_WORLD.Send(&est, 1, MPI::LONG_LONG, source, 1);
			}
		}
	}
}

void Population::ClusterFinalize() {
	if (IsClusterEnabled() && !MPI::Is_finalized() && MPI::COMM_WORLD.Get_rank() == 0) {
		char tmp;
		for (int i = 1; i < MPI::COMM_WORLD.Get_size(); i++) {
			MPI::COMM_WORLD.Send(&tmp, 1, MPI::BYTE, i, 0);
		}
		MPI::Finalize();
		cout << "Cluster Master node shut down" << endl;
	}
}

}
#endif
