//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/compression/raw.h"
//----------------------------------------------
void flxr::raw::write_data(Container& container, File& file, std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Compressing: " << file.get_name() << "\n";

	file.set_offset(stream.tellg());

	write(stream, source);
	file.set_size(uint64(stream.tellg()) - file.get_offset());
}
//----------------------------------------------
void flxr::raw::read_data(Container& container, File& file, std::iostream& dest) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Decompressing: " << file.get_name() << "\n";

	stream.seekg(file.get_offset(), std::ios::beg);

	char buffer[file.get_size()];
	stream.read(buffer, file.get_size()), file.get_size();
	dest.write(buffer, file.get_size());

	std::cout << "[D] " << "Offset: " << file.get_offset() << '\n';
	std::cout << "[D] " << "Size: " << file.get_size() << '\n';
}
