#ifndef WORKERTHREAD_HPP
#define WORKERTHREAD_HPP

#include <thread>
#include <mutex>
#include <condition_variable>

namespace library
{
	class WorkerThread
	{
	public:
		
		typedef enum
		{
			GT_NOT_INITIALIZED,
			GT_SIG_READY,
			GT_BUSY,
			GT_FINISHED,
			GT_SIG_SUICIDE
			
		} thread_status_t;
		
		// starts this thread
		void start();
		// performs a job, but will cause a wait if the thread is busy!
		void work( void (*)(void) );
		// (permanently) ends this thread
		void finish();
		
	private:
		void thread_function();
		
		thread_status_t status;
		
		std::mutex*              mutex;
		std::condition_variable* signal;
		std::thread*             thread;
	};
	
}

#endif
