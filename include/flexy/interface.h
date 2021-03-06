#pragma once
//----------------------------------------------
#include <iostream>
#include <iomanip>

#include "typedef.h"
#include "logger.h"
//----------------------------------------------
/// @todo This is terrible, for the actual tool this needs some work
/// @todo Maybe not use logger here, will result in a lot of log spam
namespace Progress {
	std::string name;
	uint64 total_size;

	/// @todo This can not use message, because it will ruin the log
	void draw(uint64 total_read) {

		float progress = float(total_read)/float(total_size);

		int bar_width = 70;
		std::cout << name << " [";
		int pos = bar_width * progress;
		for (int i = 0; i < bar_width; ++i) {
			if (i < pos) {
				std::cout << "=";
			} else if (i == pos) {
				std::cout << ">";
			} else {
				std::cout << " ";
			}
		}
		std::cout << "] " << int(progress * 100) << "% ";
		if (progress != 1) {
			std::cout << " ";
		}
		std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(total_read)/1000/1000 << " MB / " << float(total_size)/1000/1000 << " MB\r";
		std::cout.flush();
	}

	void setup(const std::string& m_name, uint64 m_total_size) {
		name = m_name;
		total_size = m_total_size;

		draw(0);
	}

	void finish(uint64 compressed_size) {
		std::cout << '\n';
		info << "Final size: " << std::setiosflags(std::ios::fixed) << std::setprecision(1) << float(compressed_size)/1000/1000 << " MB (" << float(compressed_size)/float(total_size)*100 << "%)\n";
	}
}
