//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/spec.h"
#include "logger.h"
//----------------------------------------------
void flxr::Raw::write_data(MetaData& meta_data, std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	auto& stream = meta_data.get_container().get_stream();

	debug << "Writing: " << meta_data.get_name() << "\n";

	meta_data.set_offset(stream.tellg());

	uint64 initial_positon = source.tellg();
	source.seekg(0, std::ios::end);
	uint64 total_size = uint64(source.tellg()) - initial_positon;
	source.seekg(initial_positon, std::ios::beg);
	if (on_init != nullptr) {
		on_init(meta_data.get_name(), total_size);
	}

	write(stream, source);

	// This goes so fast, we really don't need an progress bar
	meta_data.set_size(uint64(stream.tellg()) - meta_data.get_offset());
	if (on_update != nullptr) {
		on_update(total_size);
	}
	if (on_finish != nullptr) {
		on_finish(meta_data.get_size());
	}
}
//----------------------------------------------
void flxr::Raw::read_data(MetaData& meta_data, std::iostream& dest) {
	auto& stream = meta_data.get_container().get_stream();

	debug << "Reading: " << meta_data.get_name() << "\n";

	stream.seekg(meta_data.get_offset(), std::ios::beg);

	/// @todo This really should be an loop
	std::vector<char> buffer;
	buffer.resize(meta_data.get_size());
	stream.read(buffer.data(), meta_data.get_size()), meta_data.get_size();
	dest.write(buffer.data(), meta_data.get_size());
}
