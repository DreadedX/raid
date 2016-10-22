//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/compression/raw.h"
//----------------------------------------------
void flxr::raw::write_data(Container& container, MetaData& meta_data, std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Compressing: " << meta_data.get_name() << "\n";

	meta_data.set_offset(stream.tellg());

	write(stream, source);
	meta_data.set_size(uint64(stream.tellg()) - meta_data.get_offset());
}
//----------------------------------------------
void flxr::raw::read_data(Container& container, MetaData& meta_data, std::iostream& dest) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Decompressing: " << meta_data.get_name() << "\n";

	stream.seekg(meta_data.get_offset(), std::ios::beg);

	char buffer[meta_data.get_size()];
	stream.read(buffer, meta_data.get_size()), meta_data.get_size();
	dest.write(buffer, meta_data.get_size());

	std::cout << "[D] " << "Offset: " << meta_data.get_offset() << '\n';
	std::cout << "[D] " << "Size: " << meta_data.get_size() << '\n';
}
