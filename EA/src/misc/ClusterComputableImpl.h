/*
 * ClusterComputable.h
 *
 *  Created on: May 31, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "ClusterComputable.h"
#include "Cluster.h"
#include <mpi/mpi.h>
#include "MultiThreading.h"
#include "../rtoc/BinarySerializer.h"

namespace ea {

using namespace std;

template <class InputT, class OutputT>
inline vector<OutputT>
		ClusterComputable<InputT, OutputT>::ExecuteInCluster(const vector<InputT>& pInputArray,
				bool multiThreading, CallbackFunction pCallback) {
	vector<OutputT> outputArray(pInputArray.size());

	// If cluster is enabled
	if (Cluster::IsEnabled() && GetClusterOpId() >= 0) {
		// Activate this operator
		Cluster::Load(GetClusterOpId());

		uint size = pInputArray.size();

		uint slaveCount;
		MPI_Comm_size(MPI_COMM_WORLD, (int*)&slaveCount);
		slaveCount--;

		uint sent = min(size, slaveCount);

		// First deployment
		for (uint i = 0; i < sent; i++) {
			ostringstream oss(ios::binary);
			BinarySerializer<InputT>::Write(oss, pInputArray[i]);
			const string data = oss.str();
			MPI_Send(data.c_str(), data.length(), MPI_BYTE,
					i + 1, i + EA_CLUSTER_TAG_RESERVED,
					MPI_COMM_WORLD);
		}

		// Loop
		for (uint received = 0; received < size; received++) {
			MPI_Status status;
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			int source = status.MPI_SOURCE;
			int tag = status.MPI_TAG;
			int count;
			MPI_Get_count(&status, MPI_BYTE, &count);

			char* buffer = new char[count];
			MPI_Recv(buffer, count, MPI_BYTE, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			uint index = tag - EA_CLUSTER_TAG_RESERVED;
			istringstream iss(string(buffer, count), ios::binary);
			outputArray[index] = BinarySerializer<OutputT>::Read(iss);

			if (pCallback)
				pCallback(index, pInputArray[index], outputArray[index]);

			if (sent < size) {
				ostringstream oss(ios::binary);
				BinarySerializer<InputT>::Write(oss, pInputArray[sent]);
				const string data = oss.str();
				MPI_Send(data.c_str(), data.length(), MPI_BYTE,
						source, sent + EA_CLUSTER_TAG_RESERVED,
						MPI_COMM_WORLD);
				sent++;
			}
		}

		// Deactivate this operator
		Cluster::Unload();
	}

	// If cluster is not enabled, activate multi-threading
	else if (multiThreading)
		MultiThreading::For(0, pInputArray.size(), [&] (int i) {
			outputArray[i] = ProcessOnRemote(pInputArray[i]);
			pCallback(i, pInputArray[i], outputArray[i]);
		});

	// Otherwise, process on single thread
	else
		for (uint i = 0; i < pInputArray.size(); i++) {
			outputArray[i] = ProcessOnRemote(pInputArray[i]);
			pCallback(i, pInputArray[i], outputArray[i]);
		}

	return outputArray;
}

template <class InputT, class OutputT>
Cluster::ClusterFunction ClusterComputable<InputT, OutputT>::SlaveFunction() {
	return [this] (int count, int tag) {
		char* buffer = new char[count];
		MPI_Recv(buffer, count, MPI_BYTE, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		istringstream iss(string(buffer, count), ios::binary);
		InputT input = BinarySerializer<InputT>::Read(iss);

		OutputT output = ProcessOnRemote(input);

		ostringstream oss(ios::binary);
		BinarySerializer<OutputT>::Write(oss, output);
		const string data = oss.str();

		MPI_Send(data.c_str(), data.length(), MPI_BYTE, 0, tag, MPI_COMM_WORLD);
	};
}

}
