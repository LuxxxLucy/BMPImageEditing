
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
    // 声明简单的变量
	auto image = BMPcontainer("test_image/lena512color.bmp");

	std::cout << "make translate"  << std::endl;
	image.translate(50,50);
	image.save_as("test_image/translate.bmp");

    image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "make rotate"  << std::endl;
	image.rotate(0,0,0.5);
	image.save_as("test_image/rotate.bmp");

    image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "make scale"  << std::endl;
	image.scale(0,0,0.2);
	image.save_as("test_image/scale.bmp");

    image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "make shear at x"  << std::endl;
	image.x_shear(50);
	image.save_as("test_image/x_shear.bmp");

    image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "make shear at y"  << std::endl;
	image.y_shear(50);
	image.save_as("test_image/y_shear.bmp");

    image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "mirror at x"  << std::endl;
	image.x_mirror(256);
	image.save_as("test_image/x_mirror.bmp");

    image = BMPcontainer("test_image/lena512color.bmp");
	std::cout << "mirror at y"  << std::endl;
	image.y_mirror(256);
	image.save_as("test_image/y_mirror.bmp");

	return 0;

}
