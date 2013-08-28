#include "thread_worker.hpp"

namespace library
{
	void WorkerThread::start()
	{
		// create mutex & signal
		this->mutex = new std::mutex();
		this->signal = new std::condition_variable();
		// set state to not yet initialized
		this->status = GT_NOT_INITIALIZED;
		// start thread, using member function
		this->thread = new std::thread(&WorkerThread::thread_function, this);
	}
	
	void WorkerThread::finish()
	{
		this->status = GT_SIG_SUICIDE;
		// wake thread
		//this->signal.notify_one();
	}
	
	void WorkerThread::thread_function()
	{
		while (this->status != GT_SIG_SUICIDE)
		{
			{	// set as ready for more data
				this->status = GT_SIG_READY;
				// wait for data
				std::unique_lock<std::mutex> lock(*mutex);
				// using conditional / signal
				signal->wait(lock);
			}
			
			// now processing?
			// ... do stuff ...
			
			{	// set as finished
				std::lock_guard<std::mutex> lock(*mutex);
				this->status = GT_FINISHED;
			}
			signal->notify_one();
		}
	}
	
}