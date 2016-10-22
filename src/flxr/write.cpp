//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/write.h"
#include "flxr/crc.h"

#include "flxr/compression/zlib.h"
#include "flxr/compression/raw.h"
#include "flxr/compression/on_disk.h"
//----------------------------------------------
void flxr::write_header(Container& container) {
	write(container.get_stream(), container.get_header());
}
//----------------------------------------------
void flxr::write_index(Container& container) {
	auto& stream = container.get_stream();

	stream.seekg(sizeof(Container::Header), std::ios::beg);
	for(auto meta_data : container.get_index()) {
		write(stream, meta_data.get_name());
		/// @todo This should be calculated by the reader
		write(stream, meta_data.get_offset());
		write(stream, meta_data.get_size());
	}
	stream.seekg(0, std::ios::end);
}
//----------------------------------------------
/// @todo There must be a better way to do this
void flxr::write_data(Container& container, MetaData& meta_data, std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	switch(container.get_header().compression) {
		case flxr::COMPRESSION::ZLIB:
			flxr::zlib::write_data(container, meta_data, source, on_init, on_update, on_finish);
			break;
		case flxr::COMPRESSION::RAW:
			flxr::raw::write_data(container, meta_data, source, on_init, on_update, on_finish);
			break;
		case flxr::COMPRESSION::ON_DISK:
			flxr::on_disk::write_data(container, meta_data, source, on_init, on_update, on_finish);
			break;
		default:
			std::cerr << "Selected compression algorithm is not implemented\n";
			break;
	}
}
//----------------------------------------------
void flxr::write_crc(Container& container) {
	auto& stream = container.get_stream();

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
