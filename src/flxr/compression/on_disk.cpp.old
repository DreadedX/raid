//----------------------------------------------
#include <fstream>
#include "flxr/binary_helper.h"
#include "flxr/compression/on_disk.h"
//----------------------------------------------
void flxr::on_disk::write_data(Container& container, MetaData& meta_data, std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Storing path: " << meta_data.get_name() << "\n";

	meta_data.set_offset(stream.tellg());

	/// @todo We need to actually get this path now
	std::string path = meta_data.get_path();
	meta_data.set_size(path.length());
	if (on_init != nullptr) {
		on_init(meta_data.get_name(), meta_data.get_size());
	}

	write(stream, path);

	// This goes so fast, we really don't need an progress bar
	if (on_update != nullptr) {
		on_update(meta_data.get_size());
	}
	if (on_finish != nullptr) {
		on_finish(meta_data.get_size());
	}
}
//----------------------------------------------
void flxr::on_disk::read_data(Container& container, MetaData& meta_data, std::iostream& dest) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Reading path: " << meta_data.get_name() << "\n";

	stream.seekg(meta_data.get_offset(), std::ios::beg);

	std::string path;
	read(stream, path);
	std::fstream file_stream;
	/// @todo Instead of returning the path we need to open the file and return that;
	file_stream.open(path, std::ios::in | std::ios::binary);
	dest << file_stream.rdbuf();
}
