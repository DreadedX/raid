#pragma once
//----------------------------------------------
#include <string>
//----------------------------------------------
bool has_file_extension(const std::string& str) {
	return str.substr(str.find_last_of('/')).find_last_of('.') != std::string::npos;
}
//----------------------------------------------
std::string get_file_extension(const std::string& str) {
	return str.substr(str.find_last_of('.'));
}
//----------------------------------------------
std::string get_file_name(const std::string& str) {
	return str.substr(str.find_first_of('/')+1);
}
//----------------------------------------------
