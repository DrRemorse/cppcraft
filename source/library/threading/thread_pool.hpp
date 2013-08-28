#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace library
{
	class ThreadPool
	{
	public:
		typedef void (*threadJobFunc)(void* data);
		
		ThreadPool(size_t threads);
		//template<class F, class... Args>
		//auto enqueue(F&& f, Args&&... args) 
		//	-> std::future<typename std::result_of<F(Args...)>::type>;
		~ThreadPool();
		
		// job starter(TM)
		std::future<void> startJob(threadJobFunc&&, void* data);
		// wait for queue to finish before continuing
		void finishQueue(std::vector<std::future<void>>& futures);
		
	private:
		// thread worker function
		void workFunc();
		// need to keep track of threads so we can join them
		std::vector<std::thread> workers;
		// the task queue
		std::queue<std::function<void()>> tasks;
		
		// synchronization
		std::mutex queue_mutex;
		std::condition_variable condition;
		bool terminate;
	};
	
	// add new work item to the pool
	/*
	template<class F, class... Args>
	auto ThreadPool::enqueue(F&& f, Args&&... args) 
		-> std::future<typename std::result_of<F(Args...)>::type>
	{
		typedef typename std::result_of<F(Args...)>::type return_type;
		
		// don't allow enqueueing after stopping the pool
		if (terminate)
			throw std::runtime_error("enqueue on stopped ThreadPool");
		
		auto task = std::make_shared< std::packaged_task<return_type()> >(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);
		
		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			tasks.push([task](){ (*task)(); });
		}
		condition.notify_one();
		return res;
	}
	*/
	
}

#endif
