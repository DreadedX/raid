//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/read.h"
#include "flxr/crc.h"
#include "flxr/compression/zlib.h"
#include "flxr/compression/raw.h"
#include "flxr/compression/on_disk.h"
//----------------------------------------------
void flxr::read_header(Container& container) {
	read(container.get_stream(), container.get_header());

	if (container.get_header().magic != MAGIC) {
		std::cerr << "Invalid magic number\n";
		exit(-1);
	}
}
//----------------------------------------------
void flxr::read_index(Container& container) {
	auto& stream = container.get_stream();

	for (uint64 i = 0; i < container.get_header().index_size; i++) {

		std::string file_name;
		read(stream, file_name);
		MetaData meta_data(file_name);
		uint64 offset;
		read(stream, offset);
		meta_data.set_offset(offset);
		uint64 size;
		read(stream, size);
		meta_data.set_size(size);

		container.add_file(std::move(meta_data));
	}
}
//----------------------------------------------
/// @todo There must be a better way to do this
void flxr::read_data(Container& container, MetaData& meta_data, std::iostream& dest) {
	switch(container.get_header().compression) {
		case flxr::COMPRESSION::ZLIB:
			flxr::zlib::read_data(container, meta_data, dest);
			break;
		case flxr::COMPRESSION::RAW:
			flxr::raw::read_data(container, meta_data, dest);
			break;
		case flxr::COMPRESSION::ON_DISK:
			flxr::on_disk::read_data(container, meta_data, dest);
			break;
		default:
			std::cerr << "Selected compression algorithm is not implemented\n";
			break;
	}
}
//----------------------------------------------
void flxr::check_crc(Container& container) {
	auto& stream = container.get_stream();

	stream.seekg(0, std::ios::end);
	uint64 size = stream.tellg();
	size -= sizeof(crc_t);
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

	crc_t provided_crc;
	read(stream, provided_crc);
	if (crc != provided_crc) {
		std::cout << "File is corrupt\n";
		exit(-1);
	}

	stream.clear();
	stream.seekg(0, std::ios::beg);
}
//----------------------------------------------
