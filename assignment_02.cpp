
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
	printf("assignment 02");
// 声明简单的变量
	//auto image = BMPcontainer("test_image/threhold_test.bmp");
	auto image = BMPcontainer("test_image/lena512color.bmp");

	std::cout << "make binary"  << std::endl;
	image.make_binary();
	image.save_as("test_image/threhold.bmp");

	std::cout << "make delation"  << std::endl;
	image.make_delation();
	image.save_as("test_image/threhold_delation.bmp");

	image = BMPcontainer("test_image/lena512color.bmp");
	image.make_binary();
	image.make_erosion();
	image.save_as("test_image/threhold_erosion.bmp");

	image = BMPcontainer("test_image/lena512color.bmp");
	image.make_binary();
	image.make_opening();
	image.save_as("test_image/threhold_opening.bmp");

	image = BMPcontainer("test_image/lena512color.bmp");
	image.make_binary();
	image.make_closing();
	image.save_as("test_image/threhold_closing.bmp");
	return 0;

}
