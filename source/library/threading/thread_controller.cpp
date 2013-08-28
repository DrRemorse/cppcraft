#include "thread_controller.hpp"

#include "thread_worker.hpp"

namespace library
{
	ThreadController threadController;
	
	void ThreadController::init(int numThreads)
	{
		// create all threads
		for (int i = 0; i < numThreads; i++)
		{
			WorkerThread* w = new WorkerThread;;
			threads.push_back(w);
		}
		
		// start all threads
		for (std::size_t i = 0; i < threads.size(); i++)
		{
			threads[i]->start();
		}
		
	}
	
	ThreadController::~ThreadController()
	{
		// delete all threads
		for (std::size_t i = 0; i < threads.size(); i++)
		{
			delete threads[i];
		}
		threads.clear();
	}
	
}
