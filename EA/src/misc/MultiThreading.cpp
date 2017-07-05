/*
 * MultiThreading.cpp
 *
 *  Created on: May 29, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "MultiThreading.h"
#include "Cluster.h"
#include "../Common.h"
#include <omp.h>

namespace ea {

uint MultiThreading::sNumThreads = 0;
bool MultiThreading::sForced = false;

/**
 * @class MultiThreading
 * Static class providing multi-threading feature.
 * MultiThreading class provides functions to control and execute code in parallel using OpenMP.
 * To execute a for loop in parallel, see For() function.
 * To adjust and query the number of threads, see SetNumThreads() and GetNumThreads() functions.
 *
 * The multi-threading feature is automatically disabled if cluster computation
 * is activated to avoid interference (because cluster computation is usually blocking and takes all
 * of the CPU usage). If users deploy the cluster remotely and still have some CPU cores to spare,
 * multi-threading feature can be forced to be enabled by calling SetForceEnabled().
 *
 * @see For()
 */

/**
 * Execute a <b>for</b> loop in parallel.
 * This function provides a convinient way to implement parallelism in user code.
 * For example, if users need to run this segment of code in parallel:
 *
 * @code
 * for (int i = 0; i < 10; i++)
 *     a[i] = b[i] * i;
 * @endcode
 *
 * Using MultiThreading class, the segment simply changes to:
 *
 * @code
 * MultiThreading::For(0, 10, [&] (int i) {
 *     a[i] = b[i] * i;
 * });
 * @endcode
 *
 * Be caution, thread safety is the responsibility of users. For example,
 * pushing an item into a vector in multi-threading version won't preserve the order
 * of elements. Instead of pushing, it is recommended to access the vector via an absolute index
 * to avoid race condition.
 *
 * @param pFrom The starting number of the index.
 * @param pTo The target number of the index (exclusive).
 * @param pFunc The segment of code.
 */
void MultiThreading::For(int pFrom, int pTo, function<void(int)> pFunc) {
	atomic<bool> hasError(false);
	exception_ptr excp;

#pragma omp parallel for schedule(guided) if (sNumThreads != 1 && (!Cluster::IsEnabled() || sForced))
	for (int i = pFrom; i < pTo; i++) {
		if (hasError)
			continue;

		try {
			pFunc(i);
		} catch (exception& e) {
			hasError = true;
			excp = current_exception();
		}
	}

	if (hasError)
		rethrow_exception(excp);
}

/**
 * Get the maximum number of threads allowed when multi-threading.
 * The returned value of this function is identical to what is set in SetNumThreads().
 * It will returns 0 if default setting is used, or 1 if multi-threading is disabled.
 * Using the default setting, the number of threads used will be equal to the number
 * of cores available in CPU. To get the real number of threads, use GetRealNumThreads().
 * @return The maximum number of threads.
 */
uint MultiThreading::GetNumThreads() {
	return sNumThreads;
}

/**
 * Get the real number of threads used currently.
 * @return The real number of threads. Cannot be 0.
 */
uint MultiThreading::GetRealNumThreads() {
	if (Cluster::IsEnabled() && !IsForceEnabled())
		return 1;

	uint numThreads;
#pragma omp parallel
#pragma omp single
	numThreads = omp_get_num_threads();
	return numThreads;
}

/**
 * Set the maximum number of threads allowed.
 * Set this to 0 if using the default setting, 1 to disable multi-threading, or
 * any arbitrary number as the number of threads.
 * Using the default setting, the number of threads used will be equal to the number
 * of cores available in CPU.
 * @param pNumThreads The maximum number of threads (or 0 for default setting).
 */
void MultiThreading::SetNumThreads(uint pNumThreads) {
	omp_set_num_threads(pNumThreads == 0 ? omp_get_max_threads() : pNumThreads);
	sNumThreads = pNumThreads;
}

/**
 * Force-enable the multi-threading feature.
 * The multi-threading feature is automatically disabled if cluster computation
 * is activated to avoid interference (because cluster computation is usually blocking and takes all
 * of the CPU usage). If users deploy the cluster remotely and still have some CPU cores to spare,
 * multi-threading feature can be forced to be enabled by calling this function.
 * Set this to true to enable the multi-threading feature regardless the use of cluster computation.
 * @param pForced true to force-enable the multi-threading feature.
 */
void MultiThreading::SetForceEnabled(bool pForced) {
	sForced = pForced;
}

/**
 * Whether the multi-threading feature is force-enabled.
 * @return true if it is force-enabled, false otherwise.
 */
bool MultiThreading::IsForceEnabled() {
	return sForced;
}

} /* namespace ea */
