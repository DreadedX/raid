//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/spec.h"
#include "flxr/crc.h"

#include "flxr/compression.h"

#include "flxr/exceptions.h"
//----------------------------------------------
void flxr::Container::write_header() {
	write(get_stream(), header);
}
//----------------------------------------------
void flxr::Container::write_index() {
	auto& stream = get_stream();

	stream.seekg(sizeof(Container::Header), std::ios::beg);
	for(auto meta_data : get_index()) {
		write(stream, meta_data.get_name());
		write(stream, meta_data.get_offset());
		write(stream, meta_data.get_size());
	}
	stream.seekg(0, std::ios::end);
}
//----------------------------------------------
void flxr::MetaData::write_data(std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	get_container().get_compression()->write_data(*this, source, on_init, on_update, on_finish);
}
//----------------------------------------------
void flxr::Container::write_crc() {
	auto& stream = get_stream();

	uint64 size = stream.tellg();
	stream.clear();
	stream.seekg(0, std::ios::beg);
	uint32 chunk_size = 1024;

	crc_t crc = crc_init();
	while(size > 0) {
		if (size < chunk_size) {
			chunk_size = size;
		}
		size -= chunk_size;
		char buffer[chunk_size];
		stream.read(buffer, chunk_size);
		crc = crc_update(crc, buffer, chunk_size);
	}
	crc = crc_finalize(crc);

	write(stream, crc);
}
//----------------------------------------------
