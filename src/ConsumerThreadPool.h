#pragma once

#include <queue>
#include <condition_variable>
#include <thread>
#include <atomic>


template<typename ItemType>
class ConsumerThreadPool {
private:
	std::vector<std::thread> m_threads;
	std::queue<ItemType> m_items;
	std::condition_variable m_notEmpty, m_notFull;
	std::mutex m_mutex;
	std::atomic<bool> m_killAll;
	size_t m_maxSize;

	template<typename Function>
	void consumeThread(Function &&f) {
		while (!m_killAll) {
			std::unique_lock<std::mutex> lock(m_mutex);
			m_notEmpty.wait(lock, [this] {
				return m_killAll || !m_items.empty();
			});
			if (m_killAll) break;
			ItemType item = std::move(m_items.front());
			m_items.pop();
			lock.unlock();
			m_notFull.notify_one();
			try {
				f(std::move(item));
			}
			catch (const std::exception &e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
	}

	bool isFull() {
		return m_items.size() >= m_maxSize;
	}


public:
	static constexpr size_t DEFAULT_MAX_QUEUE_SIZE = 400;

	template<typename Function>
	ConsumerThreadPool(int numThreads, Function &&f, size_t maxSize = DEFAULT_MAX_QUEUE_SIZE) :
		m_killAll(false), m_maxSize(maxSize) {
		for (int i = 0; i < numThreads; ++i) {
			m_threads.push_back(std::thread(&ConsumerThreadPool<ItemType>::consumeThread<Function>, this, f));
		}
	}

	virtual ~ConsumerThreadPool() {
		// Maybe have a more intelligent method of terminating children?
		m_killAll = true;
		m_notEmpty.notify_all();
		m_notFull.notify_all();
		for (auto &thread : m_threads) {
			thread.join();
		}
	}
	ConsumerThreadPool(ConsumerThreadPool &) = delete;
	ConsumerThreadPool & operator=(ConsumerThreadPool &) = delete;

	// Allows pushing non-copyable objects...I think.
	// I need to review Effective Modern C++.
	void push(ItemType &&value) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_notFull.wait(lock, [this] {
			return m_killAll || !isFull();
		});
		if (m_killAll) {
			return;
		}
		m_items.push(std::move(value));
		lock.unlock();
		m_notEmpty.notify_one();
	}
};
