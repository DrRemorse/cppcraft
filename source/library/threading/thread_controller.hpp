#ifndef THREAD_CONTROLLER_HPP
#define THREAD_CONTROLLER_HPP

#include <vector>

namespace library
{
	class WorkerThread;
	
	class ThreadController
	{
	public:
		void init(int numThreads);
		void startJob();
		void waitThreads();
		void killThreads();
		
		~ThreadController();
		
	private:
		std::vector<WorkerThread*> threads;
		
	};
	extern ThreadController threadController;
}

#endif
