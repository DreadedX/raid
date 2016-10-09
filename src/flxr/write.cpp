//----------------------------------------------
#include <iostream>
#include <vector>
#include <fstream>

#include "typedef.h"
#include "flxr/binary_helper.h"
#include "flxr/crc.h"

using namespace flxr;
//----------------------------------------------
enum COMPRESSION : byte {
	ZLIB,
};
//----------------------------------------------
class File {
	public:
		File(std::string m_name) : name(m_name) {
			offset = 30;
		}

		auto get_name() { return name; }
		auto get_offset() { return offset; }
		auto get_size() { return reinterpret_cast<uint64>(data.size()); }

		auto get_data() {
			data = {0x44,0x41,0x54,0x41};

			return data;
		}

	private:
		std::string name;
		uint64 offset;
		std::vector<byte> data;
};
//----------------------------------------------
class Container {
	public:
		Container(std::string m_name) : name(m_name) {}

		auto get_name() { return name; }
		auto get_file_count() { return reinterpret_cast<uint64>(files.size()); }
		auto get_files() { return files; }

		void add_file(File file) { files.push_back(std::move(file)); }

	private:
		std::string name;
		std::vector<File> files;
};
//----------------------------------------------
const int32 MAGIC = 844647494; /* FLX2 */
//----------------------------------------------
std::ostream& write_header(std::ostream& stream, Container& container) {
	write(stream, MAGIC);
	write(stream, COMPRESSION::ZLIB);
	write(stream, container.get_file_count());

	for(auto file : container.get_files()) {
		write(stream, file.get_name());
		write(stream, file.get_offset());
		write(stream, file.get_size());
	}

	return stream;
}
//----------------------------------------------
std::ostream& write_data(std::ostream& stream, Container& container) {
	for(auto file : container.get_files()) {
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
std::istream& read_header(std::istream& stream) {
	int32 magic;
	read(stream, magic);
	std::cout << magic << '\n';
	/// @todo Add error handling
	if (magic != MAGIC) {
		std::cout << "Invalid magic number\n";
		exit(-1);
	}

	COMPRESSION compression;
	read(stream, compression);
	std::cout << compression << '\n';

	uint64 file_count;
	read(stream, file_count);
	std::cout << file_count << '\n';

	for (uint64 i = 0; i < file_count; i++) {

		std::string file_name;
		read(stream, file_name);
		uint64 offset;
		read(stream, offset);
		uint64 size;
		read(stream, size);

		std::cout << file_name << "(offset: " << offset << ", size: " << size << ")\n";
	}

	return stream;
}
//----------------------------------------------
int main() {
	//-Writing--------------------------------------
	std::cout << "WRITING\n";
	Container container("test.flx");

	container.add_file(File("test1"));
	container.add_file(File("test2"));

	std::fstream stream;
	stream.open(container.get_name(), std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);

	if(!stream.is_open()) {
		std::cout << "Failed to open file: " << container.get_name() << '\n';
		exit(-1);
	}

	write_header(stream, container);
	write_data(stream, container);
	write_crc(stream);

	stream.close();

	//----------------------------------------------
	//-Reading--------------------------------------
	std::cout << "READING\n";
	stream.open(container.get_name(), std::ios::in | std::ios::binary);

	check_crc(stream);

	read_header(stream);

	stream.close();
	//----------------------------------------------
}
