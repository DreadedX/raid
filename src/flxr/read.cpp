//----------------------------------------------
#include <vector>

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

	for (int64 i = 0; i < header.index_size; i++) {

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
void flxr::MetaData::read_data(std::vector<byte>& dest, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	get_container().get_compression()->read_data(*this, dest, on_init, on_update, on_finish);
}
//----------------------------------------------
void flxr::MetaData::read_data(std::iostream& dest, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	std::vector<byte> temp;
	read_data(temp, on_init, on_update, on_finish);
	flxr::write(dest, temp);
}
//----------------------------------------------
void flxr::Container::check_crc() {
	auto& stream = get_stream();

	stream.seekg(0, std::ios::end);
	uint64 size = stream.tellg();
	size -= sizeof(crc32);
	stream.clear();
	stream.seekg(0, std::ios::beg);
	uint32 chunk_size = 1024;

	crc32 crc = 0;
	while(size > 0) {

		if (size < chunk_size) {
			chunk_size = size;
		}
		size -= chunk_size;

		char* buffer = new char[chunk_size];
		stream.read(buffer, chunk_size);

		crc = crc32_bitwiser(buffer, chunk_size, crc);
		delete[] buffer;
	}

	std::cout << "CRC: " << crc << '\n';

	crc32 provided_crc;
	read(stream, provided_crc);
	if (crc != provided_crc) {
		throw flxr::bad_file();
	}

	stream.clear();
	stream.seekg(0, std::ios::beg);
}
//----------------------------------------------
