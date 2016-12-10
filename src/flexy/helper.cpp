//----------------------------------------------
#include "flexy/helper.h"
//----------------------------------------------
flxr::COMPRESSION get_compression_type(ValTree package) {
	const std::string compression = package.query("compression.type").getStr();

	if (compression.compare("ZLIB") == 0) {
		return flxr::COMPRESSION::ZLIB;
	} else if (compression.compare("RAW") == 0) {
		return flxr::COMPRESSION::RAW;
	} else if (compression.compare("ON_DISK") == 0) {
		return flxr::COMPRESSION::ON_DISK;
	} else {
		std::cerr << "Invalid compression type\n";
		exit(-1);
	}
}
//----------------------------------------------
