/*
 * Banker's algorithm simulator - Roberto Natella
 */

#ifndef __BANKER_H_
#define __BANKER_H_

#include <pthread.h>

template <int num_resources, int num_processes>
class BankerAlgorithm {

	public:
		BankerAlgorithm(int resources[num_resources], int claim[num_resources][num_processes]);
		virtual ~BankerAlgorithm();
		bool allocateResource(int process, int resource, int amount);
		bool releaseResource(int process, int resource, int amount);
		bool terminateProcess(int process);

	private:
		bool isSafe();
		void printState();

		int m_num_resources;
		int m_num_processes;
		int m_resources[num_resources];
		int m_claim[num_resources][num_processes];
		int m_available[num_resources];
		int m_alloc[num_resources][num_processes];
		int m_running[num_processes];
		pthread_mutex_t m_monitor_mutex;
		pthread_cond_t m_monitor_cv[num_resources];
};

#include "banker.cc"

#endif /*__BANKER_H_ */
