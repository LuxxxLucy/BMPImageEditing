
#include "BMPcontainer.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <vector>
#include <cassert>


int main()
{
    printf("hello this is the test programme for library of image process by lucy\n");
	printf("assignment 06");
// 声明简单的变量
	//auto image = BMPcontainer("test_image/threhold_test.bmp");
	auto image = BMPcontainer("test_image/lena512color.bmp");

	std::cout << "make mean bilateral filter of window size 5"  << std::endl;
	image.make_bilateral_filter(3,10);
	image.save_as("test_image/bilateral_filter.bmp");

	return 0;

}
