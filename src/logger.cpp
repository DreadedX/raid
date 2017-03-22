#include <iomanip>
#include <string>
#include <sstream>
#include "logger.h"
#include "typedef.h"

#undef debug
#undef info
#undef warning

#if __has_include(<experimental/filesystem>)
	#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#else
	#pragma message "Compiler does not support experimental/filesystem"
#endif

logger::prefixbuf::prefixbuf(std::string const& prefix, std::streambuf* sbuf) : _prefix(prefix), _sbuf(sbuf), need_prefix(true) {}

int logger::prefixbuf::sync() {
	return this->_sbuf->pubsync();
}

int logger::prefixbuf::overflow(int c) {
	if (c != std::char_traits<char>::eof()) {
		if (this->need_prefix && !this->_prefix.empty()) {
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

			for (unsigned int i = final_prefix.size(); i < 53-_prefix.length(); ++i) {
				final_prefix += '-';
			}
			final_prefix += " " + this->_prefix + " -";
			final_prefix += ' ';

			this->_sbuf->sputn(&final_prefix[0], final_prefix.size());
		}
		this->need_prefix = c == '\n';
	}
	return this->_sbuf->sputc(c);
}

void logger::prefixbuf::set_location(std::string location) {
	_location = location;
}

logger::oprefixstream::oprefixstream(std::string const& prefix, std::ostream& out) : prefixbuf(prefix, out.rdbuf()), std::ios(static_cast<std::streambuf*>(this)), std::ostream(static_cast<std::streambuf*>(this)) {}

// std::fstream _log;
std::stringstream _log;
logger::Multiplexer::Multiplexer(std::ostream& out1, const std::string& prefix) {

	#ifdef ANDROID
		out1.rdbuf(new logger::androidbuf);

		_out1 = std::make_unique<logger::oprefixstream>("", out1);
	#else
		_out1 = std::make_unique<logger::oprefixstream>(prefix, out1);
	#endif
	_out2 = std::make_unique<logger::oprefixstream>(prefix, _log);

	#if 0
	if (!_log.is_open()) {

		#if __has_include(<experimental/filesystem>)
			if(!fs::exists("logs")) {
				fs::create_directory("logs");
			}
		#endif

		#if 0
			time_t rawtime;
			time(&rawtime);

			_log.open("logs/log-" + std::to_string(rawtime) + ".txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		#else
			_log.open("logs/log-latest.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		#endif

		_log << std::left << std::setw(11) << "Compiled on" << " : " << __DATE__ << " " << __TIME__ << '\n';
		_log << std::left << std::setw(11) << "C++ version" << " : " << __cplusplus << '\n';
		_log << std::left << std::setw(11) << "Build" << " : " << "---" << '\n';
	}
	#endif
}

logger::Multiplexer& logger::Multiplexer::operator()(const char* file, int line) {
#ifndef ANDROID
	std::string location = std::string(file) + ":" + std::to_string(line);
#else
	std::ostringstream os;
	os << line;
	std::string location = std::string(file) + ":" + os.str();
#endif
	_out1->set_location(location);
	_out2->set_location(location);
	return *this;
}

std::stringstream& get_log_string() {
	return _log;
}

/// @todo Make this only print prefix when in debug mode
logger::Multiplexer debug(std::cout, "DEBUG");
logger::Multiplexer info(std::cout, "INFO");
logger::Multiplexer warning(std::cerr, "WARNING");

