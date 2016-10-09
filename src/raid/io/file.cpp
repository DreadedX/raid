//----------------------------------------------
#include "raid/io/file.h"
//----------------------------------------------
raid::File::File(std::string m_name) {
	name = m_name;
}
//----------------------------------------------
/// @todo Check the overhead of this function, maybe weak_ptr stuff is unnecessary.
std::shared_ptr<std::vector<byte>> raid::File::get_data() {
	// If the weak ptr is invalid, load the data from disk
	if (data.expired()) {
		std::cout << "Now is the time to load the actual data\n";

		// Create new shared ptr
		auto t = std::make_shared<std::vector<byte>>();

		// "Load" the data
		for (byte i = 0; i < 10; ++i) {
			t->push_back(i);
		}

		// Set the weak ptr
		data = t;
		// Returns shared ptr
		return t;
	}

	std::cout << "Data is already loaded in memory, reusing\n";
	// return a shared ptr to the file
	return data.lock();
}
//----------------------------------------------
/// @todo Should only be usedable by the FileManager
std::string raid::File::get_name() {
	return name;
}
//----------------------------------------------
