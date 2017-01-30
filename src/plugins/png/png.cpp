/// @note THIS IS A DIRECT PORT FROM FLARE AND NEEDS A LOT OF WORK!!!
#include <cstring>

#include <png.h>

#include "flexy/plugin.h"
#include "flxr/memstream.h"
// #include "typedef.h"

#include "logger.h"

namespace png {

    struct Data {

	int width = 0;
	int height = 0;
	int size = 0;
	unsigned char bytesPerPixel = 0;
	unsigned char *pixels = nullptr;
    };

    Data read(const char *name);
}

png::Data png::read(const char *name) {

	Data data;

	png_structp png_ptr;
	png_infop info_ptr;
	uint32 sig_read = 0;
	int color_type;
	int interlace_type;
	FILE *fp;

	if ((fp = fopen(name, "rb")) == NULL) {
		return data;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return data;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return data;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return data;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	png_uint_32 width;
	png_uint_32 height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);

	uint32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	data.pixels = (byte*) malloc(row_bytes * height);
	data.size = row_bytes * height;

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	for (uint32 i = 0; i < height; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(data.pixels+(row_bytes * i), row_pointers[i], row_bytes);
	}

	data.width = width;
	data.height = height;

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	fclose(fp);

	data.bytesPerPixel = data.size / (data.width * data.height);

	return data;
}

class PNG : public Plugin {

	public:
		std::shared_ptr<std::iostream> process(std::string file_path) override {

			png::Data image = png::read(file_path.c_str());

			uint64 size = image.size + 2*sizeof(int) + sizeof(byte);
			byte* data = new byte[size];
			debug << size << '\n';

			uint32 offset = 0;
			for (unsigned int i = 0; i < sizeof(int32); ++i) {

				data[i] = image.width >> (i*8);
			}
			offset += sizeof(int32);

			for (unsigned int i = 0; i < sizeof(int32); ++i) {

				data[i+offset] = image.height >> (i*8);
			}
			offset += sizeof(int32);

			data[offset] = image.bytesPerPixel;
			offset += sizeof(byte);

			// file->data = data.pixels;
			for (int32 y = 0; y < image.height; y++) {
				for (int32 x = 0; x < image.width*image.bytesPerPixel; x++) {

					data[x + y * (image.width*image.bytesPerPixel) + offset] = image.pixels[x + y * (image.width*image.bytesPerPixel)];
				}
			}

			std::shared_ptr<std::iostream> final_stream = std::make_shared<Memstream<char*>>((char*)data, size);
			delete[] data;

			return final_stream;
		}
};

share_plugin(PNG)
