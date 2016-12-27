#pragma once
//----------------------------------------------
#include <functional>

// #include "flxr/spec.h"
#include "typedef.h"
#include "logger.h"
//----------------------------------------------
namespace flxr { 
	class MetaData;
	class Compression {
		public:
			virtual ~Compression() {}

			virtual void write_data(MetaData&, std::iostream&,
					std::function<void(const std::string&, const uint64)> = nullptr,
					std::function<void(const uint64)> = nullptr,
					std::function<void(const uint64)> = nullptr) = 0;
			virtual void read_data(MetaData&, std::iostream&) {
				warning << "write_data is not not_implemented\n";
			}
	};
	class Zlib : public flxr::Compression {
		public:
			void write_data(MetaData& meta_data, std::iostream& source,
					std::function<void(const std::string&, const uint64)> on_init = nullptr,
					std::function<void(const uint64)> on_update = nullptr,
					std::function<void(const uint64)> on_finish = nullptr) override;
			void read_data(MetaData& meta_data, std::iostream& dest) override;
	};
	class Raw : public flxr::Compression {
		public:
			void write_data(MetaData& meta_data, std::iostream& source,
					std::function<void(const std::string&, const uint64)> on_init = nullptr,
					std::function<void(const uint64)> on_update = nullptr,
					std::function<void(const uint64)> on_finish = nullptr) override;
			void read_data(MetaData& meta_data, std::iostream& dest) override;
	};
}
