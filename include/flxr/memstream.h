#pragma once
//----------------------------------------------
#include <iostream>
#include <vector>
#include <stdint.h>

#include "typedef.h"
//----------------------------------------------
class Membuf : public std::basic_streambuf<char> {
	public:
		Membuf(char* p, size_t l) {
			setg(p, p, p + l);
		}
	protected:
		virtual std::iostream::pos_type seekoff(std::iostream::off_type off, std::ios_base::seekdir way,
				std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override {

			if (way == std::ios_base::cur) gbump(off);
			if (way == std::ios_base::beg) gbump(eback()-gptr()+off);
			// Should go to end of buffer, hopefully offset is not taken into account
			if (way == std::ios_base::end) gbump(egptr()-gptr());

			return gptr() - eback();
		}
		virtual std::iostream::pos_type seekpos(std::iostream::pos_type sp, std::ios_base::openmode which =
				std::ios_base::in | std::ios_base::out) override {

			seekoff(sp, std::ios_base::beg, which);

			return gptr() - eback();
		}
};
//----------------------------------------------
/// @todo Figure out a way to get rid of explicit templating
template <typename T>
class Memstream : public std::iostream {
	public:
		Memstream(T data, size_t l);
		Memstream(T& data);

	private:
		T _data;
		Membuf _buffer;
};
//----------------------------------------------
template<>
Memstream<char*>::Memstream(char* data, size_t l) : std::iostream(&_buffer), _buffer(data, l){
	_data = new char[l];
	std::copy(data, data+l, _data);
	_buffer = Membuf(_data, l);

	rdbuf(&_buffer);
}
//----------------------------------------------
/// @note This will store a copy of the original and invalidate the original
template<>
Memstream<std::string>::Memstream(std::string& data) : std::iostream(&_buffer), _data(std::move(data)), _buffer((char*)_data.data(), _data.length()) {
	rdbuf(&_buffer);
}
