#include <iostream>
#include <list>

template<int num_resources, int num_processes>
BankerAlgorithm<num_resources, num_processes>::BankerAlgorithm(int resources[num_resources], int claim[num_resources][num_processes]) {
	m_num_resources = num_resources;
	m_num_processes = num_processes;

	int i,j;

	for(i=0; i<m_num_resources; i++) {
		m_resources[i] = resources[i];
		m_available[i] = resources[i];
	}

	for(i=0; i<m_num_resources; i++) {
		for(j=0; j<m_num_processes; j++) {
			m_claim[i][j] = claim[i][j];
			m_alloc[i][j] = 0;
		}
	}


	for(j=0; j<m_num_processes; j++) {
		m_running[j] = true;
	}

	pthread_mutex_init(&m_monitor_mutex, NULL);

	for(i=0; i<m_num_resources; i++) {
		pthread_cond_init(&m_monitor_cv[i], NULL);
	}

        std::cout << "INITIAL STATE" << std::endl;
        printState();
}


template <int num_resources, int num_processes>
BankerAlgorithm<num_resources, num_processes>::~BankerAlgorithm() {
}


template <int num_resources, int num_processes>
bool BankerAlgorithm<num_resources, num_processes>::allocateResource(int process, int resource, int amount) {

	pthread_mutex_lock(&m_monitor_mutex);

	std::cout << "ALLOCATION REQUEST BY PROCESS " << process << ": RESOURCE " << resource << " --> " << amount << std::endl << std::endl;

	// check parameter correctness
	if( resource < 0 || resource >= num_resources || process < 0 || process >= num_processes ) {
		std::cout << "WRONG PARAMETERS IN RESOURCE REQUEST" << std::endl;
		pthread_mutex_unlock(&m_monitor_mutex);
		return false;
	}

	// check if request is consistent with the claim
	if(m_alloc[resource][process] + amount > m_claim[resource][process]) {
		std::cout << "WRONG RESOURCE REQUEST BY PROCESS " << process << std::endl;
		pthread_mutex_unlock(&m_monitor_mutex);
		return false;
	}


	bool safe = false;

	// try to allocate until the state is safe
	while(!safe) {


		// check if resource is available; if not, sleep until
		// resource becomes available
		if(amount > m_available[resource]) {

			std::cout << "RESOURCE NOT AVAILABLE: SUSPENDING PROCESS " << process << std::endl;
			printState();
			pthread_cond_wait(&m_monitor_cv[resource], &m_monitor_mutex);
			continue;

		}


		// simulate allocation
		m_alloc[resource][process] += amount;
		m_available[resource] -= amount;


		// check if the state is safe
		safe = isSafe();
		//safe = true;	// uncomment this line to disable resource allocation denial


		// if state is not safe, restore the original
		// state and suspend
		if(!safe) {

			// unsafe state detected

			m_alloc[resource][process] -= amount;
			m_available[resource] += amount;
			

			// suspend is state is unsafe
			// (will wake-up when resources will be freed)
	
			std::cout << "UNSAFE STATE DETECTED: SUSPENDING PROCESS " << process << std::endl;
			printState();
			pthread_cond_wait(&m_monitor_cv[resource], &m_monitor_mutex);
			continue;
		}

	}


	// state is safe

	std::cout << "SAFE STATE DETECTED: ALLOCATION GRANTED TO PROCESS " << process << std::endl;
	printState();

	pthread_mutex_unlock(&m_monitor_mutex);

	return true;
}


template <int num_resources, int num_processes>
bool BankerAlgorithm<num_resources, num_processes>::releaseResource(int process, int resource, int amount) {

	pthread_mutex_lock(&m_monitor_mutex);

	std::cout << "RELEASE REQUEST BY PROCESS " << process << ": RESOURCE " << resource << " --> " << amount << std::endl << std::endl;

	// check parameter correctness
	if( resource < 0 || resource >= num_resources || process < 0 || process >= num_processes ) {
		std::cout << "WRONG PARAMETERS IN RELEASE REQUEST" << std::endl;
		pthread_mutex_unlock(&m_monitor_mutex);
		return false;
	}


	// check if resource is actually allocated to the process
	if(m_alloc[resource][process] < amount) {
		std::cout << "WRONG RELEASE REQUEST BY PROCESS " << process << std::endl;
		pthread_mutex_unlock(&m_monitor_mutex);
		return false;
	}


	m_alloc[resource][process] -= amount;
	m_available[resource] += amount;


	std::cout << "RESOURCE RELEASED BY PROCESS " << process << std::endl;
	printState();


	// wake-up suspended processes
	pthread_cond_broadcast(&m_monitor_cv[resource]);

	pthread_mutex_unlock(&m_monitor_mutex);

	return true;
}



template <int num_resources, int num_processes>
bool BankerAlgorithm<num_resources, num_processes>::terminateProcess(int process) {
	pthread_mutex_lock(&m_monitor_mutex);


	std::cout << "DEALLOCATION OF PROCESS " << process << std::endl << std::endl;


	// check parameter correctness
	if( process < 0 || process >= num_processes ) {
		std::cout << "WRONG PARAMETERS IN TERMINATION REQUEST" << std::endl;
		pthread_mutex_unlock(&m_monitor_mutex);
		return false;
	}



	// check if process is running
	if(m_running[process] == false) {
		std::cout << "PROCESS ALREADY TERMINATED" << std::endl;
		pthread_mutex_unlock(&m_monitor_mutex);
		return false;
	}


	// release all resources
	int resource;
	for(resource=0; resource<m_num_resources; resource++) {

		if(m_alloc[resource][process] > 0) {
			m_available[resource] += m_alloc[resource][process];
			m_alloc[resource][process] = 0;

			// wake-up suspended processes waiting for a resource
			pthread_cond_broadcast(&m_monitor_cv[resource]);
		}
	}


	// mark process as "inactive"
	m_running[process] = false;


	printState();

	pthread_mutex_unlock(&m_monitor_mutex);

	return true;
}



template <int num_resources, int num_processes>
bool BankerAlgorithm<num_resources, num_processes>::isSafe() {

	int currentavail[m_num_resources];

	std::list<int> rest_processes;
	std::list<int> safe_sequence;

	// array for simulating resource availability
	int i, j;
	for(i=0; i<m_num_resources; i++) {
		currentavail[i] = m_available[i];
	}


	// the safe state is checked by only considering "running" processes
	for(j=0; j<m_num_processes; j++) {
		if(m_running[j] == true)
			rest_processes.push_back(j);
	}


	bool possible = true;

	while(possible) {

		// find a process such that (claim - alloc <= currentavail)
		bool found = false;
		std::list<int>::iterator it = rest_processes.begin();

		int curr_proc;

		while(!found && it != rest_processes.end() ) {

			found = true;
			curr_proc = *it;

			for(i=0; i<m_num_resources; i++) {
				if(m_claim[i][curr_proc] - m_alloc[i][curr_proc] > currentavail[i]) {
					found = false;
				}
			}

			if(!found) {
				it++;
			}
		}

		if(found) {
			// simulate execution of process "curr_proc"
			for(i=0; i<m_num_resources; i++) {
				currentavail[i] += m_alloc[i][curr_proc];
			}

			rest_processes.erase(it);

			safe_sequence.push_back(curr_proc);

		} else {
			possible = false;
		}
	}


	if(rest_processes.size()==0) {

		// print safe process sequence found
		std::cout << std::endl;
		std::cout << "SAFE PROCESS SEQUENCE: ";

		std::list<int>::iterator it;

		for(it=safe_sequence.begin(); it!=safe_sequence.end(); it++) {
			std::cout << *it << " ";
		}

		std::cout << std::endl << std::endl;
	}

	return (rest_processes.size()==0);
}




template <int num_resources, int num_processes>
void BankerAlgorithm<num_resources, num_processes>::printState() {

	int i, j;

	for(i=0; i<m_num_resources; i++) {

		if(i==0) {
			std::cout << "\nALLOCATED (CLAIM)\n\n";

			std::cout << "+-->   \t\tProcessi\n";
			std::cout << "|\n";
			std::cout << "V\n";
			std::cout << "Risorse\t\t";

		} else {

			std::cout << "       \t\t";
		}


		for(j=0; j<m_num_processes; j++) {

			std::cout << m_alloc[i][j] << " (" << m_claim[i][j] << ")" << "\t";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;


	std::cout << "\nAVAILABLE (TOTAL)\n\n";

	for(i=0; i<m_num_resources; i++) {

		std::cout << m_available[i] << " (" << m_resources[i] << ")" << "\t";
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
	std::cout << std::endl;


}


