#pragma once

#include <iostream>
#include <fstream>
#include <streambuf>
#include <ctime>
#include <cstring>

extern std::fstream log;

namespace logger {

	/// @note Credit: http://stackoverflow.com/questions/27336335/c-cout-with-prefix (Modified)
	class prefixbuf : public std::streambuf {
		private:
			std::string prefix;
			std::string _location;
			std::streambuf* sbuf;
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

}

class Multiplexer {
	public:
		Multiplexer(std::ostream& out1, std::ostream& out2, std::string prefix);
		// logger::oprefixstream _out1;
		// logger::oprefixstream _out2;
		std::ostream& _out1;
		// This needs to be a global thing
		std::fstream _out2;

		Multiplexer& operator()(const char* file, int line);

		void flush() {
			_out2.flush();
		}
};

template <typename T>
Multiplexer& operator<<(Multiplexer& m, const T& t) {
	m._out1 << t;
	m._out2 << t;
	return m;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
// Multiplexer& operator<<(Multiplexer& m, std::ostream&(*f)(std::ostream&)) {
// 	m._out1 << f;
// 	m._out2 << f;
// 	return m;
// }
#pragma GCC diagnostic pop

extern Multiplexer debug;
extern Multiplexer message;
extern Multiplexer warning;

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define debug debug(__FILENAME__, __LINE__)
#define warning warning(__FILENAME__, __LINE__)
