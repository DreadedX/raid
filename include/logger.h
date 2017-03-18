#pragma once

#include <iostream>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <cstring>
#include <memory>

#ifdef ANDROID
	#include "android/log.h"
#endif

std::stringstream& get_log_string();

namespace logger {

	/// @note Credit: http://stackoverflow.com/questions/27336335/c-cout-with-prefix (Modified)
	class prefixbuf : public std::streambuf {
		private:
			std::string _prefix;
			std::string _location;
			std::streambuf* _sbuf;
			bool need_prefix;

			int sync();
			int overflow(int c);

		public:
			prefixbuf(std::string const& prefix, std::streambuf* sbuf);

			void set_location(std::string location);
	};

	class oprefixstream : public virtual prefixbuf, public std::ostream {
		public:
			oprefixstream(std::string const& prefix, std::ostream& out);
	};

	#ifdef ANDROID
		class androidbuf: public std::streambuf {
			public:
				enum { bufsize = 128 }; // ... or some other suitable buffer size
				androidbuf() { this->setp(buffer, buffer + bufsize - 1); }
			private:
				int overflow(int c) {
					if (c == traits_type::eof()) {
						*this->pptr() = traits_type::to_char_type(c);
						this->sbumpc();
					}
					return this->sync()? traits_type::eof(): traits_type::not_eof(c);
				}
				int sync() {
					if (this->pbase() != this->pptr()) {
						__android_log_print(ANDROID_LOG_INFO,
								"Engine",
								"%s",
								std::string(this->pbase(),
									this->pptr() - this->pbase()).c_str());
						this->setp(buffer, buffer + bufsize - 1);
					}
					return 0;
				}
				char buffer[bufsize];
		};
	#endif

	class Multiplexer {
		public:
			Multiplexer(std::ostream& out1, const std::string& prefix);

			// This doesn't work without pointer, the second one does not get constructor properly
			std::unique_ptr<logger::oprefixstream> _out1 = nullptr;
			std::unique_ptr<logger::oprefixstream> _out2 = nullptr;

			Multiplexer& operator()(const char* file, int line);
	};
}

template <typename T>
logger::Multiplexer& operator<<(logger::Multiplexer& m, const T& t) {
	*m._out1 << t << std::flush;
	*m._out2 << t;

	return m;
}

extern logger::Multiplexer debug;
extern logger::Multiplexer message;
extern logger::Multiplexer warning;

#ifndef LOGGER_H
	#define LOGGER_H
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
	#define debug debug(__FILENAME__, __LINE__)
	/// @todo This one does not work for whatever reason
	// #define message message(__FILENAME__, __LINE__)
	#define warning warning(__FILENAME__, __LINE__)
#endif
