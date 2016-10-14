#pragma once
//----------------------------------------------
#include <vector>
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
	class Container;
	class File {

		friend std::istream& flxr::read_index(std::istream& stream, flxr::Container& container);

		public:
			File(std::string m_name) : name(m_name) {}

			const std::string& get_name() { return name; }

			const uint64 get_size() {
				return size;
			}
			auto set_size(uint64 m_size) {
				size = m_size;
			}

		private:
			std::string name;
			uint64 size;
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
}
