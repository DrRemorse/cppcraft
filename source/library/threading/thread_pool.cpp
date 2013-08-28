#include "thread_pool.hpp"

namespace library
{
	void ThreadPool::workFunc()
	{
		while(true)
		{
			std::unique_lock<std::mutex> lock(this->queue_mutex);
			
			while (this->terminate == false && this->tasks.empty())
				this->condition.wait(lock);
			
			if (this->terminate == true && this->tasks.empty())
				return;
			
			std::function<void()> task(this->tasks.front());
			this->tasks.pop();
			lock.unlock();
			// perform task
			task();
		}
	}
	
	// [threads] permanent worker threads
	ThreadPool::ThreadPool(size_t threads)
	{
		terminate = false;
		
		while(threads--)
		{
			workers.emplace_back(
				&ThreadPool::workFunc, this
			);
		}
	}
	
	// the destructor joins all threads
	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			terminate = true;
		}
		condition.notify_all();
		for(size_t i = 0; i < workers.size(); ++i)
		{
			workers[i].join();
		}
	}
	
	std::future<void> ThreadPool::startJob(threadJobFunc&& func, void* data)
	{
		// don't allow enqueueing after stopping the pool
		if (terminate)
			throw std::runtime_error("enqueue on stopped ThreadPool");
		
		auto task = std::make_shared< std::packaged_task<void()> >
			(
				std::bind(std::forward<threadJobFunc>(func), std::forward<void*>(data))
			);
		
		std::future<void> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			tasks.push([task](){ (*task)(); });
		}
		condition.notify_one();
		return res;
	}
	
	void ThreadPool::finishQueue(std::vector<std::future<void>>& futures)
	{
		for (std::future<void>& x : futures)
			x.wait();
		futures.clear();
	}
	
}