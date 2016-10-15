//----------------------------------------------
#include <iomanip>
#include <cassert>
#include <zlib.h>

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
		uint64 offset;
		read(stream, offset);
		file.set_size(offset);

		container.add_file(std::move(file));
	}
}
//----------------------------------------------
#define CHUNK 16384
// void inf(Container& container)
// {
// 	auto& stream = container.get_stream();
//
// 	std::cout << "[D] " << "Decompressing files\n";
// 	for (auto& file : container.get_files()) {
// 		int ret;
// 		unsigned have;
// 		z_stream strm;
// 		unsigned char in[CHUNK];
// 		unsigned char out[CHUNK];
//
// 		#<{(| allocate inflate state |)}>#
// 		strm.zalloc = Z_NULL;
// 		strm.zfree = Z_NULL;
// 		strm.opaque = Z_NULL;
// 		strm.avail_in = 0;
// 		strm.next_in = Z_NULL;
// 		ret = inflateInit(&strm);
// 		if (ret != Z_OK) {
// 			exit(-1);
// 		}
//
// 		#<{(| decompress until deflate stream ends or end of file |)}>#
// 		do {
// 			strm.avail_in = fread(in, 1, CHUNK, source);
// 			if (ferror(source)) {
// 				(void)inflateEnd(&strm);
// 				return Z_ERRNO;
// 			}
// 			if (strm.avail_in == 0)
// 				break;
// 			strm.next_in = in;
//
// 			#<{(| run inflate() on input until output buffer not full |)}>#
// 			do {
// 				strm.avail_out = CHUNK;
// 				strm.next_out = out;
// 				ret = inflate(&strm, Z_NO_FLUSH);
// 				assert(ret != Z_STREAM_ERROR);  #<{(| state not clobbered |)}>#
// 				switch (ret) {
// 					case Z_NEED_DICT:
// 						ret = Z_DATA_ERROR;     #<{(| and fall through |)}>#
// 					case Z_DATA_ERROR:
// 					case Z_MEM_ERROR:
// 						(void)inflateEnd(&strm);
// 						return ret;
// 				}
// 				have = CHUNK - strm.avail_out;
// 				if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
// 					(void)inflateEnd(&strm);
// 					return Z_ERRNO;
// 				}
// 			} while (strm.avail_out == 0);
//
// 			#<{(| done when inflate() says it's done |)}>#
// 		} while (ret != Z_STREAM_END);
//
// 		#<{(| clean up and return |)}>#
// 		(void)inflateEnd(&strm);
// 		return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
// 	}
// }
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
