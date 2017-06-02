
#include "BMPcontainer.cpp"
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
	auto image = BMPcontainer("test_image/lena512color.bmp");
	image.show_first_10_pixels();
	image.save_as("test_image/lena_save_test.bmp");

	image.RGBtoYUV();
	image.save_as("test_image/YUV.bmp");
	std::vector<CPixel> pix = image.get_pixels();
	image.show_first_10_pixels();

	image.YUVtoRGB();
	image.save_as("test_image/YUVtoRGB.bmp");
	image.show_first_10_pixels();

	image.RGBtoYUV();
	image.ToGray(0);
	image.save_as("test_image/gray.bmp");

	image.RearrangeIntensity();
	image.save_as("test_image/rearrange.bmp");

	//image.save_as("test_image/color_test_of_save.bmp");
	//image.show_file_header();
    // image.show_info_header();


	pix = image.get_pixels();
	//copy(pix.begin(),pix.begin()+10,std::ostream_iterator<CPixel>(std::cout,"\n"));
	std::vector<CPixel> pix_now(256*256);
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
		{
			CPixel temp_pix={ 100,100,100,0 };
			pix_now.push_back(temp_pix);
		}
	image = BMPcontainer(256,256,pix_now);
	image.save_as("test_image/write_gray.bmp");

	pix_now.clear();
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
		{
			CPixel temp_pix={ 255,0,0,0 };
			pix_now.push_back(temp_pix);
		}
	image = BMPcontainer(256,256,pix_now);
	image.save_as("test_image/write_color.bmp");






	return 0;

}
