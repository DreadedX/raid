#include "raid/queue.h"

#include <algorithm>

std::unordered_map<std::string, QueueController*> _list;
std::mutex _list_mtx;

std::vector<long> _valid_ids;
std::mutex _id_mtx;

long new_uid() {
	std::lock_guard<std::mutex> lck(_id_mtx);
	static long uid = 1;
	_valid_ids.push_back(uid);
	return uid++;
}

void delete_uid(long uid) {
	std::lock_guard<std::mutex> lck(_id_mtx);
	_valid_ids.erase(std::remove_if(_valid_ids.begin(), _valid_ids.end(), [=](long _uid){ return _uid==uid; }));
}

bool is_valid_uid(long uid) {
	if (uid == 0) {
		return true;
	}

	std::lock_guard<std::mutex> lck(_id_mtx);
	auto result = std::find(_valid_ids.begin(), _valid_ids.end(), uid);
	return result != _valid_ids.end();
}

void list_valid_ids() {
	std::lock_guard<std::mutex> lck(_id_mtx);
	debug << "Valid uid:\n";
	for (auto uid : _valid_ids) {
		debug << "id: " << uid << '\n';
	}
}

QueueController::QueueController(std::string name, bool thread) : _name(name), _thread(thread) {
	QueueList::register_queue(this);
}

QueueController::~QueueController() {
	QueueList::unregister_queue(this);
}

// todo: The loop need to check if it needs to quit if the program is exiting
void QueueController::process() {
	// Endless loop (only on seperate thread)
	do {
		// Acquire lock
		std::unique_lock<std::mutex> lck(_mtx);
		// Wait we are notified to start running (only on seperate thread)
		if (_thread) {
			_cv.wait(lck);
		}
		// Make a copy of the queue so it does not keep the queue locked
		Queue local_queue = _queue;
		// Clear the queue so we can start filling it again
		_queue.clear();
		// Unlock the queue
		lck.unlock();
		// Go trough entire local queue
		for (auto& item : local_queue) {
			if (is_valid_uid(item.second)) {
				item.first();
			} else {
				warning << "Invalid caller id: " << item.second << '\n';
			}
		}
	} while(_thread && _running);
}

void QueueController::add(QueueItem queue_item, long uid) {
	std::lock_guard<std::mutex> lck(_mtx);
	_queue.push_back(make_pair(queue_item, uid));
}

void QueueController::go() {
	std::lock_guard<std::mutex> lck(_mtx);
	if (!_queue.empty()) {
		_cv.notify_all();
	}
}

void QueueController::stop() {
	std::lock_guard<std::mutex> lck(_mtx);
	_running = false;
	_cv.notify_all();
}
