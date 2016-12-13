#include "logger.h"
#undef debug
#undef warning

logger::prefixbuf::prefixbuf(std::string const& prefix, std::streambuf* sbuf) : prefix(prefix), sbuf(sbuf), need_prefix(true) {}

int logger::prefixbuf::sync() {
	return this->sbuf->pubsync();
}

int logger::prefixbuf::overflow(int c) {
	if (c != std::char_traits<char>::eof()) {
		if (this->need_prefix && !this->prefix.empty()) {
			std::string final_prefix;

			time_t rawtime;
			time(&rawtime);
			char *current_time = ctime(&rawtime);
			final_prefix += current_time;

			final_prefix.erase(final_prefix.size()-1);

			final_prefix += " -";
			if (!_location.empty()) {
				final_prefix +=" " + _location + " -";
				_location = "";
			}

			for (uint i = final_prefix.size(); i < 50-prefix.length(); ++i) {
				final_prefix += '-';
			}
			final_prefix += " " + this->prefix + " -";
			final_prefix += ' ';

			this->sbuf->sputn(&final_prefix[0], final_prefix.size());
		}
		this->need_prefix = c == '\n';
	}
	return this->sbuf->sputc(c);
}

void logger::prefixbuf::set_location(std::string location) {
	_location = location;
}

logger::oprefixstream::oprefixstream(std::string const& prefix, std::ostream& out) : prefixbuf(prefix, out.rdbuf()), std::ios(static_cast<std::streambuf*>(this)), std::ostream(static_cast<std::streambuf*>(this)) {}

// std::fstream log;
// Multiplexer::Multiplexer(std::ostream& out1, std::ostream& out2, std::string prefix) : _out1(prefix, out1), _out2(prefix, out2) {
Multiplexer::Multiplexer(std::ostream& out1, std::ostream& out2, std::string prefix) : _out1(out1) {
	if (!_out2.is_open()) {
		_out2.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		_out2 << "Compiled on: " << __DATE__ << " " << __TIME__ << '\n';
	}
}

Multiplexer& Multiplexer::operator()(const char* file, int line) {
	std::string location = std::string(file) + ":" + std::to_string(line);
	// _out1.set_location(location);
	// _out2.set_location(location);
	return *this;
}

Multiplexer debug(std::cout, std::cout, "DEBUG");
Multiplexer message(std::cout, std::cout, "");
Multiplexer warning(std::cout, std::cout, "WARNING");

