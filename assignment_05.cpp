
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
	printf("assignment 05");
// 声明简单的变量
	//auto image = BMPcontainer("test_image/threhold_test.bmp");
	auto image = BMPcontainer("test_image/lena512color.bmp");

	std::cout << "make mean filter size of (2*2+1)*(2*2+1)"  << std::endl;
	image.make_mean_filter(2,2);
	image.save_as("test_image/mean_filter.bmp");


	image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "make laplacian enhancement"  << std::endl;
	image.make_laplacian_enhancement(0.2);
	image.save_as("test_image/laplacian_enhancement.bmp");
	return 0;

}
