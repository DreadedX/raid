//----------------------------------------------
#include <typeinfo>
#include <cassert>
#include <zlib.h>

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
std::ostream& flxr::write_compressed_test(std::ostream& stream, Container& container) {

	std::ifstream file("test.txt", std::ios::in | std::ios::binary);

	for (auto file : container.files) {
		byte* uncompressed_data = file.get_data().data();
		std::cout << "File: " << file.name << " (size: " << file.get_size() << ")\n";
		uint64 compressed_size;
		std::vector<byte> compressed_data;
		compressed_data.resize(20, 0x00);

		compress(compressed_data.data(), &compressed_size, uncompressed_data, file.get_size());
		compressed_data.resize(compressed_size);

		std::cout << "Compressed content: (size: " << compressed_data.size() << ")\n";
		for (auto dat : compressed_data) {
			assert(typeid(dat) == typeid(byte));
			std::cout << int(dat) << '\n';
		}
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
