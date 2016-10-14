//----------------------------------------------
#include <cassert>
#include <zlib.h>

#include "flxr/binary_helper.h"
#include "flxr/write.h"
#include "flxr/crc.h"
//----------------------------------------------
void flxr::write_header(Container& container) {
	write(container.get_stream(), container.get_header());
}
//----------------------------------------------
void flxr::write_index(Container& container) {
	auto& stream = container.get_stream();

	stream.seekg(sizeof(Container::Header), std::ios::beg);
	for(auto file : container.get_files()) {
		write(stream, file.get_name());
		write(stream, file.get_size());
	}
	stream.seekg(0, std::ios::end);
}
//----------------------------------------------
/// @todo This function is kind of ugly, refactor
#define CHUNK 16384
void flxr::write_data(Container& container, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	auto& stream = container.get_stream();

	std::cout << "[D] " << "Compressing files\n";
	for (auto& file : container.get_files()) {
		/// @todo This does not, at all, check if the file is correctly opened
		/// @todo Make this a std::fstream
		/// @todo Get this from the file in the container (maybe)
		FILE *source = fopen(file.get_name().c_str(), "r+");

		int ret, flush;
		unsigned have;
		z_stream strm;
		unsigned char in[CHUNK];
		unsigned char out[CHUNK];

		/* allocate deflate state */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		ret = deflateInit(&strm, container.get_compression_level());
		if (ret != Z_OK) {
			exit(-1);
		}

		fseek(source, 0, SEEK_END);
		uint64 total_size = ftell(source);
		fseek(source, 0, SEEK_SET);
		uint64 total_read = 0;

		/// @todo Replace this with some kind of callback function so that people can hook in there own progress report
		if (on_init != nullptr) {
			on_init(file.get_name(), total_size);
		}
		// draw_progress_bar(file.get_name(), float(total_read)/float(total_size));

		/* compress until end of file */
		do {
			strm.avail_in = fread(in, 1, CHUNK, source);
			total_read += strm.avail_in;
			if (on_update != nullptr) {
				on_update(total_read);
			}
			if (ferror(source)) {
				(void)deflateEnd(&strm);
				exit(Z_ERRNO);
			}
			flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
			strm.next_in = in;

			/* run deflate() on input until output buffer not full, finish
			   compression if all of source has been read in */
			do {
				strm.avail_out = CHUNK;
				strm.next_out = out;
				ret = deflate(&strm, flush);    /* no bad return value */
				assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
				have = CHUNK - strm.avail_out;
				std::vector<byte> out_vector(out, out+have);
				write(stream, out_vector);
				file.set_size(file.get_size() + have);
			} while (strm.avail_out == 0);
			assert(strm.avail_in == 0);     /* all input will be used */

			/* done when last data in file processed */
		} while (flush != Z_FINISH);
		assert(ret == Z_STREAM_END);        /* stream will be complete */

		/* clean up and return */
		(void)deflateEnd(&strm);

		if (on_finish != nullptr) {
			on_finish(file.get_size());
		}
	}
}
//----------------------------------------------
void flxr::write_crc(Container& container) {
	auto& stream = container.get_stream();

	uint64 size = stream.tellg();
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

	write(stream, crc);
}
//----------------------------------------------
