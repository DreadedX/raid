//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/read.h"
#include "flxr/crc.h"
#include "flxr/compression/zlib.h"
#include "flxr/compression/raw.h"
#include "flxr/compression/on_disk.h"

#include "flxr/exceptions.h"
//----------------------------------------------
void flxr::read_header(Container& container) {
	read(container.get_stream(), container.get_header());

	if (container.get_header().magic != MAGIC) {
		throw flxr::bad_magic_type();
	}
}
//----------------------------------------------
void flxr::read_index(Container& container) {
	auto& stream = container.get_stream();

	for (uint64 i = 0; i < container.get_header().index_size; i++) {

		std::string file_name;
		read(stream, file_name);
		MetaData meta_data(file_name, "", container);
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
void flxr::read_data(MetaData& meta_data, std::iostream& dest) {
	switch(meta_data.get_container().get_header().compression) {
		case flxr::COMPRESSION::ZLIB:
			flxr::zlib::read_data(meta_data, dest);
			break;
		case flxr::COMPRESSION::RAW:
			flxr::raw::read_data(meta_data, dest);
			break;
		default:
			// We need to check beforehand if the compression type is even valid
			/// @todo Add function to check if compression type is valid, and maybe check other header stuff
			throw flxr::not_implemented();
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
		throw flxr::bad_file();
	}

	stream.clear();
	stream.seekg(0, std::ios::beg);
}
//----------------------------------------------
