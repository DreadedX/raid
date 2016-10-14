//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/read.h"
#include "flxr/crc.h"
//----------------------------------------------
/// @todo This is just for testing
std::istream& flxr::read_header(std::istream& stream, Container& container) {
	read(stream, container.get_header());

	if (container.get_header().magic != MAGIC) {
		std::cout << "Invalid magic number\n";
		exit(-1);
	}

	return stream;
}
//----------------------------------------------
std::istream& flxr::read_index(std::istream& stream, Container& container) {
	for (uint64 i = 0; i < container.get_header().file_count; i++) {

		std::string file_name;
		read(stream, file_name);
		File file(file_name);
		read(stream, file.size);

		container.add_file(file);

		std::cout << file.name << " (size: " << file.size << ")\n";
	}

	return stream;
}
//----------------------------------------------
std::fstream& flxr::check_crc(std::fstream& stream) {
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

	return stream;
}
//----------------------------------------------
