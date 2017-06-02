/*
    * Lu Jialin @ Zhejiang University
*/
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cassert>

std::vector<std::string> ReadTXT( std::string filename ) {
	std::ifstream in(filename);
	std::string strtmp;
	std::vector<std::string> vect;
	while(getline(in, strtmp, '\n'))
		vect.push_back(strtmp);
	return vect;
}
