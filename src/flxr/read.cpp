//----------------------------------------------
#include <iomanip>
#include "flxr/binary_helper.h"
#include "flxr/read.h"
#include "flxr/crc.h"
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

	for (uint64 i = 0; i < container.get_header().file_count; i++) {

		std::string file_name;
		read(stream, file_name);
		File file(file_name);
		uint64 size;
		read(stream, size);
		file.set_size(size);

		container.add_file(file);

		/// @todo This needs to be replaced with a callback
		std::cout << file.get_name() << " " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(file.get_size())/1000/1000 << " MB compressed\n";
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
