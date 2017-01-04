#pragma once
//----------------------------------------------
#include <fstream>
#include <vector>

#include <iostream>
//----------------------------------------------
namespace flxr {
	//----------------------------------------------
	/// Simplify writing binary data
	template<typename T>
	inline std::ostream& write(std::ostream& stream, const T& value){
		stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
		return stream;
	}
	//----------------------------------------------
	/// Simplify writing binary array
	template<typename T>
	inline std::ostream& write(std::ostream& stream, const T value, uint64_t length){
		stream.write(reinterpret_cast<const char*>(value), length);
		return stream;
	}
	//----------------------------------------------
	/// Simplify writing strings and null terminating them
	template<>
	inline std::ostream& write(std::ostream& stream, const std::string& value){
		stream.write(value.c_str(), value.length());
		write(stream, '\0');
		return stream;
	}
	//----------------------------------------------
	/// Simplify writing other streams
	template<>
	inline std::ostream& write(std::ostream& stream, const std::iostream& value){
		stream << value.rdbuf();
		return stream;
	}
	//----------------------------------------------
	/// Simplify writing vectors
	template<typename T>
	inline std::ostream& write(std::ostream& stream, const std::vector<T>& value){
		stream.write(reinterpret_cast<const char*>(value.data()), value.size() * sizeof(T));
		return stream;
	}
	//----------------------------------------------
	/// Simplify reading binary data
	template<typename T>
	inline std::istream& read(std::istream& stream, T& value){
		stream.read(reinterpret_cast<char*>(&value), sizeof(T));
		return stream;
	}
	//----------------------------------------------
	// Simplify reading strings
	template<>
	inline std::istream& read(std::istream& stream, std::string& value){
		std::getline(stream, value, '\0');
		return stream;
	}
	// Simplify reading vectors
	template<typename T>
	inline std::istream& read(std::istream& stream, std::vector<T>& value){
		stream.read(reinterpret_cast<char*>(value.data()), value.size());
		return stream;
	}
}
