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
		RAW,

		ON_DISK = 255,
	};
	//----------------------------------------------
	class Container;
	class MetaData {

		public:
			MetaData(const std::string name, const std::string path, Container& container) : _name(name), _path(path), _container(container) {}

			const auto& get_name() { return _name; }
			const auto get_offset() { return _offset; }
			const auto get_size() { return _size; }
			const auto& get_path() { return _path; }
			auto& get_container() { return _container; }

			void set_size(uint64 size) { _size = size; }
			void set_offset(uint64 offset) { _offset = offset; }
			// void set_path(const std::string& path) { _path = path; }

		private:
			const std::string _name;
			uint64 _size = 0;
			uint64 _offset = 0;
			/// @todo I do not like this being in something that can use other sources than files
			const std::string _path;
			Container& _container;
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
			
			void empty_file() {
				stream.close();
				stream.open(name, std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
			}

			void configure(COMPRESSION m_compression, int m_compression_level) {
				header.magic = MAGIC;
				header.compression = m_compression;
				header.index_size = reinterpret_cast<uint64>(index.size());

				compression_level = m_compression_level;
			}

			void add_file(MetaData meta_data) {
				index.push_back(std::move(meta_data));
			}

			auto& get_stream() { return stream; }
			auto& get_index() { return index; }
			auto& get_header() { return header; }
			auto& get_compression_level() { return compression_level; }

			#pragma pack(1)
			struct Header {
				int32 magic;
				COMPRESSION compression;
				int64 index_size;
			};

		private:
			const std::string name;
			std::fstream stream;
			std::vector<MetaData> index;
			int compression_level;

			Header header;
	};
}
