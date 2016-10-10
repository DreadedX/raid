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
}
