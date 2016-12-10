//----------------------------------------------
#include "flexy/config.h"
//----------------------------------------------
using namespace flxr;
//----------------------------------------------
COMPRESSION get_compression_type(ValTree package) {
	const std::string compression = package.query("compression.type").getStr();

	if (compression.compare("ZLIB") == 0) {
		return COMPRESSION::ZLIB;
	} else if (compression.compare("RAW") == 0) {
		return COMPRESSION::RAW;
	} else if (compression.compare("ON_DISK") == 0) {
		return COMPRESSION::ON_DISK;
	} else {
		std::cerr << "Invalid compression type\n";
		exit(-1);
	}
}
