//----------------------------------------------
#include <iostream>
#include <vector>
#include <fstream>

#include "typedef.h"
#include "flxr/binary_helper.h"
#include "flxr/crc.h"

using namespace flxr;
//----------------------------------------------
const int32 MAGIC = 844647494; /* FLX2 */
//----------------------------------------------
enum COMPRESSION : byte {
	ZLIB,
};
//----------------------------------------------
struct File {
	File(std::string m_name) : name(m_name) {}

	std::string name;
	/// @todo Calculate this correctly
	uint64 offset = 30;
	std::vector<byte> data;
	uint64 size;

	auto get_size() {
		size = reinterpret_cast<uint64>(data.size());
		return size;
	}

	auto get_data() {
		data = {0x44,0x41,0x54,0x41};

		return data;
	}
};
//----------------------------------------------
struct Container {
	std::string name;
	std::vector<File> files;
	#pragma pack(1)
	struct {
		int32 magic;
		COMPRESSION compression;
		int64 file_count;
	} header;

	void configure(COMPRESSION m_compression) {
		header.magic = MAGIC;
		header.compression = m_compression;
		header.file_count = reinterpret_cast<uint64>(files.size());
	}

	void add_file(File file) {
		files.push_back(std::move(file));
	}
};
//----------------------------------------------
std::ostream& write_header(std::ostream& stream, Container& container) {
	// write(stream, container.get_header().magic);
	// write(stream, container.get_header().compression);
	// write(stream, container.get_header().file_count);
	write(stream, container.header);

	return stream;
}
//----------------------------------------------
std::ostream& write_index(std::ostream& stream, Container& container) {
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
std::ostream& write_data(std::ostream& stream, Container& container) {
	for(auto file : container.files) {
		write(stream, file.get_data());
	}

	return stream;
}
//----------------------------------------------
std::fstream& write_crc(std::fstream& stream) {
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
std::fstream& check_crc(std::fstream& stream) {
	stream.seekg(0, std::ios::end);
	uint64 size = stream.tellg();
	size -= sizeof(crc_t);
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

	std::cout << "crc: " << crc << '\n';

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
/// @todo This is just for testing
std::istream& read_header(std::istream& stream, Container& container) {
	read(stream, container.header);

	if (container.header.magic != MAGIC) {
		std::cout << "Invalid magic number\n";
		exit(-1);
	}

	return stream;
}
//----------------------------------------------
std::istream& read_index(std::istream& stream, Container& container) {
	for (uint64 i = 0; i < container.header.file_count; i++) {

		std::string file_name;
		read(stream, file_name);
		File file(file_name);
		read(stream, file.offset);
		read(stream, file.size);

		container.add_file(file);

		std::cout << file.name << "(offset: " << file.offset << ", size: " << file.size << ")\n";
	}

	return stream;
}
//----------------------------------------------
int main() {
	//-Writing--------------------------------------
	std::cout << "WRITING\n";
	Container container;
	container.name = "test.flx";

	container.add_file(File("test1"));
	container.add_file(File("test2"));

	container.configure(COMPRESSION::ZLIB);

	std::fstream stream;
	stream.open(container.name, std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);

	if(!stream.is_open()) {
		std::cout << "Failed to open file: " << container.name << '\n';
		exit(-1);
	}

	write_header(stream, container);
	write_index(stream, container);
	write_data(stream, container);
	write_crc(stream);

	stream.close();

	//----------------------------------------------
	//-Reading--------------------------------------
	Container read_container;
	std::cout << "READING\n";
	stream.open(container.name, std::ios::in | std::ios::binary);

	check_crc(stream);
	read_header(stream, read_container);
	read_index(stream, read_container);

	stream.close();
	//----------------------------------------------
}
