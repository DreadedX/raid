#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <unordered_map>

#include <cassert>

#include "logger.h"

// @todo This needs to be in a namespace

long new_uid();
void delete_uid(long uid);
bool is_valid_uid(long uid);

void list_valid_ids();

// typedef std::string QueueItem;
typedef std::function<void()> QueueItem;
typedef std::vector<std::pair<QueueItem, long>> Queue;

class QueueController {
	public:
		QueueController(std::string name, bool thread = true);
		~QueueController();
		void process();
		void add(QueueItem queue_item, long uid);
		void go();
		void stop();

		auto& get_name() { return _name; }

		std::thread spawn() {
			assert(_thread);
			return std::thread([this]{ process(); });
		}

	private:
		Queue _queue;
		std::mutex _mtx;
		std::condition_variable _cv;
		std::string _name;
		bool _thread = false;
		bool _running = true;
};

// todo: This is going to complain because it is in a header file
extern std::unordered_map<std::string, QueueController*> _list;
extern std::mutex _list_mtx;
class QueueList {
	public:
		static void register_queue(QueueController* queue) {
			std::lock_guard<std::mutex> lck(_list_mtx);
			_list[queue->get_name()] = queue;
		}
		static void unregister_queue(QueueController* queue) {
			std::lock_guard<std::mutex> lck(_list_mtx);
			_list.erase(queue->get_name());
		}
		static QueueController* find(std::string name) {
			auto found = _list.find(name);
			if (found != _list.end()) {
				return found->second;
			}
			return nullptr;
		}
		static void list() {
			std::lock_guard<std::mutex> lck(_list_mtx);
			debug << "Registered queues:\n";
			for (auto queue : _list) {
				debug << '\t' << queue.first << '\n';
			}
		}
};
