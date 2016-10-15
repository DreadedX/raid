#pragma once
//----------------------------------------------
#include <iostream>
#include <vector>
#include <fstream>

#include "typedef.h"
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	// const int32 MAGIC = 0x32584C46; #<{(| FLX2 |)}>#
	const int32 MAGIC = 0x62584C46; /* FLXb is the work in progress version of FLX2 */
	//----------------------------------------------
	enum COMPRESSION : byte {
		ZLIB,
	};
	//----------------------------------------------
	class File {

		public:
			File(std::string m_name) : name(m_name) {}

			const auto& get_name() { return name; }
			const auto get_offset() { return offset; }
			const auto get_size() { return size; }

			void set_size(uint64 m_size) { size = m_size; }
			void set_offset(uint64 m_offset) { offset = m_offset; }

		private:
			std::string name;
			uint64 size;
			uint64 offset;
	};
	//----------------------------------------------
	class Container {
		public:
			Container(std::string m_name) : name(m_name) {
				stream.open(m_name, std::ios::out | std::ios::in | std::ios::binary);

				if(!stream.is_open()) {
					std::cout << "Trying to create: " << m_name << '\n';
					stream.open(name, std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
					if(!stream.is_open()) {
						std::cerr << "Failed to open file: " << m_name << '\n';
						exit(-1);
					}
				}
			}
			~Container() {
				stream.close();
			}
			
			void clear_file() {
				stream.close();
				stream.open(name, std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
			}

			void configure(COMPRESSION m_compression, int m_compression_level) {
				header.magic = MAGIC;
				header.compression = m_compression;
				header.file_count = reinterpret_cast<uint64>(files.size());

				compression_level = m_compression_level;
			}

			void add_file(File file) {
				files.push_back(std::move(file));
			}

			auto& get_stream() { return stream; }
			auto& get_files() { return files; }
			auto& get_header() { return header; }
			auto& get_compression_level() { return compression_level; }

			#pragma pack(1)
			struct Header {
				int32 magic;
				COMPRESSION compression;
				int64 file_count;
			};

		private:
			std::string name;
			std::fstream stream;
			std::vector<File> files;
			int compression_level;

			Header header;
	};
}
