//----------------------------------------------
#include "flxr/binary_helper.h"
#include "flxr/write.h"
#include "flxr/crc.h"
//----------------------------------------------
std::ostream& flxr::write_header(std::ostream& stream, Container& container) {
	// write(stream, container.get_header().magic);
	// write(stream, container.get_header().compression);
	// write(stream, container.get_header().file_count);
	write(stream, container.header);

	return stream;
}
//----------------------------------------------
std::ostream& flxr::write_index(std::ostream& stream, Container& container) {
	for(auto file : container.files) {
		write(stream, file.name);
		write(stream, file.offset);
		/// @todo This is just to correctly set the size in the first place
		file.get_data();
		write(stream, file.get_size());
	}

	return stream;
}
//----------------------------------------------
std::ostream& flxr::write_data(std::ostream& stream, Container& container) {
	for(auto file : container.files) {
		write(stream, file.get_data());
	}

	return stream;
}
//----------------------------------------------
std::fstream& flxr::write_crc(std::fstream& stream) {
	uint64 size = stream.tellg();
	stream.clear();
	stream.seekg(0, std::ios::beg);
	uint32 chunk_size = 1024;

	std::cout << "size: " << size << '\n';

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

	std::cout << "crc: " << crc << '\n';

	return stream;
}
//----------------------------------------------
