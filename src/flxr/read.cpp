//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/spec.h"
#include "flxr/crc.h"
#include "flxr/compression.h"

#include "logger.h"

#include "flxr/exceptions.h"
//----------------------------------------------
void flxr::Container::read_header() {
	read(get_stream(), header);

	if (header.magic != MAGIC) {
		throw flxr::bad_magic_type();
	}

	compression = _get_compression();
}
//----------------------------------------------
void flxr::Container::read_index() {
	auto& stream = get_stream();

	for (uint64 i = 0; i < header.index_size; i++) {

		std::string file_name;
		read(stream, file_name);
		MetaData meta_data(file_name, "", *this);
		uint64 offset;
		read(stream, offset);
		meta_data.set_offset(offset);
		uint64 size;
		read(stream, size);
		meta_data.set_size(size);

		add_file(std::move(meta_data));
	}
}
//----------------------------------------------
void flxr::MetaData::read_data(std::iostream& dest) {
	get_container().get_compression()->read_data(*this, dest);
}
//----------------------------------------------
void flxr::Container::check_crc() {
	auto& stream = get_stream();

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
