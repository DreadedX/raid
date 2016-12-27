//----------------------------------------------
#include <cassert>

#include <zlib.h>

#include "flxr/binary_helper.h"
#include "flxr/spec.h"

#include "logger.h"
//----------------------------------------------
#define CHUNK 16384
//----------------------------------------------
/// @todo This function is kind of ugly, refactor
void flxr::Zlib::write_data(MetaData& meta_data, std::iostream& source, std::function<void(const std::string&, const uint64)> on_init, std::function<void(const uint64)> on_update, std::function<void(const uint64)> on_finish) {
	auto& stream = meta_data.get_container().get_stream();

	debug << "Compressing: " << meta_data.get_name() << "\n";

	meta_data.set_offset(stream.tellg());

	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, meta_data.get_container().get_compression_level());
	if (ret != Z_OK) {
		exit(-1);
	}

	source.seekg(0, std::ios::end);
	uint64 total_size = source.tellg();
	source.seekg(0, std::ios::beg);
	uint64 total_read = 0;

	if (on_init != nullptr) {
		on_init(meta_data.get_name(), total_size);
	}

	/* compress until end of file */
	do {
		strm.avail_in = source.readsome(reinterpret_cast<char*>(in), CHUNK);
		total_read += strm.avail_in;
		if (source.fail()) {
			(void)deflateEnd(&strm);
			exit(Z_ERRNO);
		}
		if (on_update != nullptr) {
			on_update(total_read);
		}
		/// @todo This can be better
		// flush = source_file.eof() ? Z_FINISH : Z_NO_FLUSH;
		flush = source.tellg() == total_size ? Z_FINISH : Z_NO_FLUSH;
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
			meta_data.set_size(meta_data.get_size() + have);
		} while (strm.avail_out == 0);
		assert(strm.avail_in == 0);     /* all input will be used */

		/* done when last data in file processed */
	} while (flush != Z_FINISH);
	assert(ret == Z_STREAM_END);        /* stream will be complete */

	/* clean up and return */
	(void)deflateEnd(&strm);

	if (on_finish != nullptr) {
		on_finish(meta_data.get_size());
	}
}
//----------------------------------------------
void flxr::Zlib::read_data(MetaData& meta_data, std::iostream& dest) {
	auto& stream = meta_data.get_container().get_stream();

	debug << "Decompressing: " << meta_data.get_name() << "\n";
	/// @todo This should be calculated
	stream.seekg(meta_data.get_offset(), std::ios::beg);

	int ret;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK) {
		warning << "Failed inflateInit\n";
		exit(-1);
	}

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = stream.readsome(reinterpret_cast<char*>(in), CHUNK);
		if (stream.fail()) {
			(void)inflateEnd(&strm);
			warning << "Stream fail\n";
			exit(-1);
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;     /* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					warning << "Z_MEM_ERROR\n";
					exit(-1);
			}
			have = CHUNK - strm.avail_out;
			std::vector<byte> out_vector(out, out+have);
			write(dest, out_vector);
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
}
