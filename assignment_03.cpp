
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
	printf("assignment 03");
// 声明简单的变量
	//auto image = BMPcontainer("test_image/threhold_test.bmp");
	auto image = BMPcontainer("test_image/lena512color.bmp");

	std::cout << "make logarithmic enhance"  << std::endl;
	image.make_logarithmic_enhance();
	image.save_as("test_image/logarithmic_enhance.bmp");

    auto image2 = BMPcontainer("test_image/lena512color.bmp");

	std::cout << "make histogram equalization"  << std::endl;
	image.histogram_equalization();
	image.save_as("test_image/histogram_equalization.bmp");

	return 0;

}
