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
		// vect.push_back(strtmp.substr(0, strtmp.find(' ')));
		vect.push_back(strtmp);
	// std::sort(vect.begin(), vect.end());
	// std::vector<std::string>::iterator it=unique(vect.begin(), vect.end());
	return vect;
}
//
// int main()
// {
//     printf("hello this is the test programme for library of text manipulation by lucy\n");
//     auto i= ReadTxt("test.cpp");
// 	copy(i.begin(),i.end(),std::ostream_iterator<std::string>(std::cout,"\n"));
//
// 	return 0;
//
// }
