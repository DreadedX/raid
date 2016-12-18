#pragma once

#include <iostream>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <cstring>
#include <memory>

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
	*m._out1 << t;
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
