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
#include "txt_process.hpp"

// ReadColorMap
// RGB2YUV
// RGB2CMY
// RGB2HSV
// Color2Gray
// RearrangeIntensity(channelNumber)
// WriteGrayScaleBMP
// YUV2RGB
// WriteColorBMP

#define BINARY_1 255
#define BINARY_0 1
#define MIN_DISTANCE 0.00001
typedef char BYTE; // one byte
typedef unsigned short WORD; // two byte
typedef unsigned int DWORD; // four byte
typedef long LONG; // eight byte
const double pi = std::acos(-1);
typedef struct tagBITMAPFILEHEADER{
	WORD bfType;      // must always set to "BM" to declare it is a .bmp file
	DWORD bfSize;     // specifies the size of the file in bytes
	WORD bfReserved1; // must always be set to zero
	WORD bfReserved2; // must always be set to zero
	DWORD bfOffBits;  // specifies the offset from start to bitmap data.
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

// BI_RGB for non-compressed
#define BI_RGB 1
typedef struct tagPIXEL{
	BYTE index;
} Pixel;


typedef union tagColoredPIXEL{
	struct{ uint8_t rgbBlue; uint8_t rgbGreen; uint8_t rgbRed; uint8_t rgbReserved;
	};
	struct{ uint8_t yuvY; uint8_t yuvU; uint8_t yuvV; uint8_t yuvReserved;
	};
	uint8_t channels[4];
} CPixel;

std::ostream& operator << (std::ostream& output,tagPIXEL pixel)
{
	//output<<"red "<< (char)pixel.rgbRed<<" green "<<(char)pixel.rgbGreen<<" blue "<< (char)pixel.rgbBlue<<std::endl;
	output << "platte index " << (uint8_t)pixel.index-0;
	return output;
}

std::ostream& operator << (std::ostream& output,tagColoredPIXEL pixel)
{
	//output<<"red "<< (char)pixel.rgbRed<<" green "<<(char)pixel.rgbGreen<<" blue "<< (char)pixel.rgbBlue<<std::endl;
	output << "color of pixel, Red: " << (uint8_t)pixel.rgbRed-0 << " Blue " << (uint8_t)pixel.rgbBlue-0 << " Green " << (uint8_t)pixel.rgbGreen-0 ;
	return output;
}

typedef struct tagBITMAPINFOHEADER{
	DWORD biSize;     // number of bits in this bit map info header
	DWORD biWidth;    // image width
	DWORD biHeight;   // image height
	WORD biPlanes;   // always 1
	WORD biBitCount; // bits per pixel
	DWORD biCompression; // compression type
	DWORD biSizeImage;
	DWORD biXPelsPerMeter;   //
	DWORD biYPelsPerMeter;   //
	DWORD biClrUsed;   // 0 where all colors are used
	DWORD biClrImportant;   // 0 where all colors are  important
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef union tagCOLORPLATTE{
	struct{
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;
	uint8_t rgbReserved;
	};
	struct{
	uint8_t yuvY;
	uint8_t yuvU;
	uint8_t yuvV;
	uint8_t yuvReserved;
	};
	uint8_t channels[4];
} COLORPLATTE;

std::ostream& operator << (std::ostream& output,tagCOLORPLATTE platte)
{
	output<<"red "<< (uint8_t)platte.rgbRed-0<<" green "<<(uint8_t)platte.rgbGreen-0<<" blue "<< (uint8_t)platte.rgbBlue-0 <<std::endl;
	return output;
}

uint8_t fix_range(float f)
{
	if(f>=255)
		return uint8_t(255);
	else if(f<=0)
		return uint8_t(0);
	else
		return uint8_t(f);
}
#define IMAGE_RGB 0
class BMPcontainer{
private:
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	std::vector<Pixel> pixels;
	std::vector<CPixel> cpixels;
	std::vector<COLORPLATTE> colorPlatte;
	std::string fileName;

	std::vector<CPixel> laplacian_result(int x_window_size,int y_window_size);
	void resum_with_weight(std::vector<CPixel> new_pixels,double ratio);
	CPixel make_interpolation(double x_p,double y_p,std::vector<CPixel>& pixels);
public:
	// constructor : load file
	BMPcontainer(std::string filename);
	// constructor : created by pixels information
	BMPcontainer(DWORD width,DWORD height,std::vector<CPixel> v);
	~BMPcontainer();
	// set pixel color by color template index
	void set_pixel_color(DWORD position,WORD postion);
	// set pixel color by color
	void set_pixel_color(DWORD position,WORD channel1,WORD channel2,WORD channel3);
	// save file
	bool save_as(std::string);
	// cout the info header
	void show_info_header();
	// cout the file headerj
	void show_file_header();
	// get pixels for pixel manipulation
	std::vector<CPixel> get_pixels();
	int get_width(){ return (int)this->infoHeader.biWidth; }
	int get_height(){ return (int)this->infoHeader.biHeight; }
	// show the exact channel value of first 10 pixel
	void show_first_10_pixels();
	// convert from RGB to YUV
	void RGBtoYUV();
	// convert from YUV to RGB
	void YUVtoRGB();
	// convert  to Gray scale by index of the desired channel
	void ToGray(int channel);
	// remap pixel value to 0-255
	void RearrangeIntensity();
	void make_binary();
	void make_delation();
	void make_erosion();
	void make_opening();
	void make_closing();
	// find the optimal threhold of the image so that we can do the perfect bianry image
	int findThrehold();
	// use the logarithmic enhancement technique
	void make_logarithmic_enhance();
	// do histogram equalization
	void histogram_equalization();
	// do image mean filter
	void make_mean_filter(int x_window_size,int y_window_size);
	// do laplacian enhancement
	void make_laplacian_enhancement(double ratio);
	// get pure laplacian result

	// bilateral filter of radius
	void make_bilateral_filter(float space_radius,float intensity_radius);


	void translate(int x_offset,int y_offset);
	void rotate(int x_origin,int y_origin, double r);
	void scale(int x_origin,int y_origin,double scale_factor);
	void x_shear(int offset_r);
	void y_shear(int offset_r);
	void x_mirror(int pivot);
	void y_mirror(int pivot);

};



BMPcontainer ReadBMP( std::string filename ) {
	return BMPcontainer(filename);
}

BMPcontainer::BMPcontainer(std::string filename)
{
	this->fileName=filename;
	std::ifstream imgFile(filename);
	if(imgFile.is_open())
	{
		// now start encoding information
		imgFile.seekg(0,std::ios::beg);
		imgFile.read((char *)&(this->fileHeader),sizeof(WORD));
		imgFile.read((char *)&(this->fileHeader)+4,sizeof(BITMAPFILEHEADER)-4);
		imgFile.seekg(14,std::ios::beg);
		imgFile.read((char *)&(this->infoHeader),sizeof(BITMAPINFOHEADER));

		// now start loading color platteo
		COLORPLATTE temp_platte;
		for(int color_count=0;color_count<this->infoHeader.biClrUsed;color_count++)
		{
			imgFile.read((BYTE*)&temp_platte,sizeof(COLORPLATTE));
			this->colorPlatte.push_back(temp_platte);
		}

		int width=this->infoHeader.biWidth;
		int height=this->infoHeader.biHeight;
		// now start decode every pixel
		if(this->colorPlatte.size()!=0)
		{
			Pixel temp_pixel;
			for (int i=0;i<width;i++)
				for (int j=0;j<height;j++)
				{
					imgFile.read((BYTE*)&temp_pixel,sizeof(Pixel));
					this->pixels.push_back(temp_pixel);
				}
		}
		else
		{
			// means it has no platte
			CPixel temp_pixel;
			for (int i=0;i<width;i++)
				for (int j=0;j<height;j++)
				{
					imgFile.read((BYTE*)&temp_pixel,this->infoHeader.biBitCount/8);
					this->cpixels.push_back(temp_pixel);
				}
		}

		// now close the file
		imgFile.close();
	}
	else
		// failed to open the file, need exception handling
		std::cout << "unable to open the file of " << filename;
}

BMPcontainer::BMPcontainer(DWORD width,DWORD height,std::vector<CPixel> v)
{
	this->cpixels=v;
	this->fileHeader.bfType=0x4d42; // note the order
	this->fileHeader.bfSize=width*height*3+54;
	this->fileHeader.bfOffBits=54;
	this->infoHeader.biSize=40;
	this->infoHeader.biWidth=width;
	this->infoHeader.biHeight=height;
	this->infoHeader.biPlanes=1;
	this->infoHeader.biBitCount=24;
	this->infoHeader.biCompression=0;
	// this->infoHeader.biSizeImage= ceil(width/4.0)*4*height;
	this->infoHeader.biSizeImage= width*height*3+2;
	this->infoHeader.biXPelsPerMeter=2834;
	this->infoHeader.biYPelsPerMeter=2834;
	this->infoHeader.biClrUsed=0;
	this->infoHeader.biClrImportant=0;
	// DWORD biSize;     // number of bits in this bit map info header
	// DWORD biWidth;    // image width
	// DWORD biHeight;   // image height
	// WORD biPlanes;   // always 1
	// WORD biBitCount; // bits per pixel
	// DWORD biCompression; // compression type
	// DWORD biSizeImage;
	// DWORD biXPelsPerMeter;   //
	// DWORD biYPelsPerMeter;   //
	// DWORD biClrUsed;   // 0 where all colors are used
	// DWORD biClrImportant;   // 0 where all colors are  important
}

bool BMPcontainer::save_as(std::string filename)
{
	std::ofstream imgFile(filename);
	if(imgFile.is_open())
	{
		// now start encoding information
		imgFile.seekp(0,std::ios::beg);
		imgFile.write((char *)&(this->fileHeader),sizeof(WORD));
		imgFile.write((char *)&(this->fileHeader)+4,sizeof(BITMAPFILEHEADER)-4);

		imgFile.seekp(14,std::ios::beg);
		imgFile.write((char *)&(this->infoHeader),sizeof(BITMAPINFOHEADER));


		imgFile.seekp(this->fileHeader.bfOffBits,std::ios::beg);
		if(this->colorPlatte.size()!=0)
		{
			// now start dumping color platteo
			for(int color_count=0;color_count<this->infoHeader.biClrUsed;color_count++)
				imgFile.write((BYTE*)&(this->colorPlatte[color_count]),sizeof(COLORPLATTE));
			// int width=this->infoHeader.biWidth;
			// int height=this->infoHeader.biHeight;
			for (int i=0,width=this->infoHeader.biWidth,height=this->infoHeader.biHeight;i<width;i++)
				for (int j=0;j<height;j++)
				{
					imgFile.write((BYTE*)&(this->pixels[i*width+j]),sizeof(Pixel));
				}
		}
		else
		{
			// it has no platteo
			for (int i=0,width=this->infoHeader.biWidth,height=this->infoHeader.biHeight;i<width;i++)
				for (int j=0;j<height;j++)
					imgFile.write((BYTE*)&(this->cpixels[i*width+j]),this->infoHeader.biBitCount/8);
		}

		// now close the file: saving complete
		imgFile.close();
		return true;
	}
	else
		std::cout << "unable to open the file of " << filename;
		return false;


}


std::vector<CPixel> BMPcontainer::get_pixels()
{
	if(this->infoHeader.biClrUsed!=0)
		return std::vector<CPixel>(0);
	else
		return this->cpixels;
}
void BMPcontainer::show_file_header()
{
	std::cout << "this is the file header of " <<this->fileName <<std::endl;
	std::cout << "image type " << std::hex << this->fileHeader.bfType <<std::endl;
	std::cout << "byte size " << std::dec << this->fileHeader.bfSize <<std::endl;
	std::cout << "reserve word 1: " << this->fileHeader.bfReserved1 <<std::endl;
	std::cout << "reserve word 2: " << this->fileHeader.bfReserved2 <<std::endl;
	std::cout << "bit offset " << this->fileHeader.bfOffBits <<std::endl;
}

void BMPcontainer::show_info_header()
{
	std::cout << "size of info header " << this->infoHeader.biSize <<std::endl;
	std::cout << "width " << this->infoHeader.biWidth <<std::endl;
	std::cout << "height " << this->infoHeader.biHeight <<std::endl;
	std::cout << "planes " << this->infoHeader.biPlanes <<std::endl;
	std::cout << "bit count " << this->infoHeader.biBitCount <<std::endl;
	std::cout << "compression type " << this->infoHeader.biCompression <<std::endl;
	std::cout << "image size by byte " << this->infoHeader.biSizeImage <<std::endl;
	std::cout << "x resoution " << this->infoHeader.biXPelsPerMeter <<std::endl;
	std::cout << "y resoution " << this->infoHeader.biYPelsPerMeter <<std::endl;
	std::cout << "color used " << this->infoHeader.biClrUsed <<std::endl;
	std::cout << "color important " << this->infoHeader.biClrImportant <<std::endl;
	std::cout << "number of color used " << this->colorPlatte.size() << std::endl;
}

void BMPcontainer::show_first_10_pixels()
{
	copy(this->cpixels.begin(),this->cpixels.begin()+10,std::ostream_iterator<CPixel>(std::cout,"\n"));
}
void BMPcontainer::RGBtoYUV()
{
	for(std::vector<CPixel>::iterator iter=this->cpixels.begin();iter!=this->cpixels.end();iter++)
	{
		uint8_t r= (*iter).channels[2];
		uint8_t g= (*iter).channels[1];
		uint8_t b= (*iter).channels[0];

		//(*iter).yuvY= uint8_t(0.257*r+0.504*g+0.098*b+16);
		//(*iter).yuvU= uint8_t(0.148*r-0.291*g+0.439*b+128);
		//(*iter).yuvV= uint8_t(0.439*r-0.368*g-0.071*b+128);

		(*iter).yuvY= fix_range(0.299*r+0.587*g+0.114*b);
		(*iter).yuvU= fix_range(-0.169*r-0.331*g+0.5*b+128);
		(*iter).yuvV= fix_range(0.5*r-0.419*g-0.081*b)+128;
	}
}

void BMPcontainer::ToGray(int channel)
{
	for(std::vector<CPixel>::iterator iter=this->cpixels.begin();iter!=this->cpixels.end();iter++)
	{
		uint8_t gray_scale= (*iter).channels[channel];

		(*iter).channels[0]= gray_scale;
		(*iter).channels[1]= gray_scale;
		(*iter).channels[2]= gray_scale;
	}
}

void BMPcontainer::YUVtoRGB()
{
	for(std::vector<CPixel>::iterator iter=this->cpixels.begin();iter!=this->cpixels.end();iter++)
	{
		uint8_t y= (*iter).channels[0];
		uint8_t u= (*iter).channels[1];
		uint8_t v= (*iter).channels[2];

	//	(*iter).rgbRed= fix_range(1.164*(y-16)+2.018*(u-128));
	//	(*iter).rgbGreen= fix_range(1.164*(y-16)-0.813*(v-128)-0.391*(u-128));
	//	(*iter).rgbBlue= fix_range(1.164*(y-16)+1.596*(v-128));

		(*iter).rgbRed= fix_range(y+1.13983*(v-128));
		(*iter).rgbGreen= fix_range(y-0.39465*(u-128)-0.58060*(v-128));
		(*iter).rgbBlue= fix_range(y+2.03211*(u-128));
	}

}

void BMPcontainer::RearrangeIntensity()
{
	uint8_t temp[6]={ 0,255,0,255,0,255 };

	for(std::vector<CPixel>::iterator iter=this->cpixels.begin();iter!=this->cpixels.end();iter++)
	{
		temp[0]= temp[0]>(*iter).channels[0]? temp[0]:(*iter).channels[0];
		temp[1]= temp[1]<(*iter).channels[0]? temp[1]:(*iter).channels[0];

		temp[2]= temp[2]>(*iter).channels[1]? temp[2]:(*iter).channels[1];
		temp[3]= temp[3]<(*iter).channels[1]? temp[3]:(*iter).channels[1];

		temp[4]= temp[4]>(*iter).channels[2]? temp[4]:(*iter).channels[2];
		temp[5]= temp[5]<(*iter).channels[2]? temp[5]:(*iter).channels[2];
	}

	for(std::vector<CPixel>::iterator iter=this->cpixels.begin();iter!=this->cpixels.end();iter++)
	{
		uint8_t r= (*iter).channels[0];
		uint8_t g= (*iter).channels[1];
		uint8_t b= (*iter).channels[2];

		(*iter).channels[0]= uint8_t(  (r-temp[1])*1.0/(temp[0]-temp[1])*255     );
		(*iter).channels[1]= uint8_t(  (g-temp[3])*1.0/(temp[2]-temp[3])*255     );
		(*iter).channels[2]= uint8_t(  (b-temp[5])*1.0/(temp[4]-temp[5])*255     );
	}



}


BMPcontainer::~BMPcontainer(void)
{
	// auto it = this->colorPlatte;
	// copy(it.begin(),it.end(),std::ostream_iterator<COLORPLATTE>(std::cout,"\n"));
	// auto it2 = this->pixels;
	// copy(it2.begin(),it2.end(),std::ostream_iterator<Pixel>(std::cout,"\n"));
	std::cout << this->fileName << " is happliy dead" <<std::endl;
}

void BMPcontainer::make_binary()
{

	this->ToGray(0);
	this->RearrangeIntensity();

	if(this->colorPlatte.size()!=0)
	{
		std::vector<Pixel>& pixels=this->pixels;
		std::cout << "color platte process, not done yet, pleasr use a bmp with no platte"<<std::endl;
		return;
	}
	else
	{

		int threhold = this->findThrehold();
		std::cout<< " threhold is " << threhold <<std::endl;
		std::vector<CPixel>& pixels=this->cpixels;
		// threhold=100;

		for(int i=0;i<pixels.size();i++)
		{
			pixels[i].channels[0]= pixels[i].channels[0]<threhold ? BINARY_0:BINARY_1;
			pixels[i].channels[1]= pixels[i].channels[1]<threhold ? BINARY_0:BINARY_1;
			pixels[i].channels[2]= pixels[i].channels[2]<threhold ? BINARY_0:BINARY_1;
		}
	}
}

void BMPcontainer::make_delation()
{
	std::vector<std::pair<int,int> > structuring_element;
	structuring_element.push_back(std::make_pair(0,0));
	structuring_element.push_back(std::make_pair(0,1));
	structuring_element.push_back(std::make_pair(0,-1));
	structuring_element.push_back(std::make_pair(1,0));
	structuring_element.push_back(std::make_pair(-1,0));
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> new_pixels;
	new_pixels.assign(pixels.begin(), pixels.end());
	std::cout << "1" <<std::endl;
	for(int i=0;i<pixels.size();i++)
	{
		pixels[i].channels[0]=BINARY_0;
		pixels[i].channels[1]=BINARY_0;
		pixels[i].channels[2]=BINARY_0;

		for(int j=0;j<=structuring_element.size();j++)
		{
			int offset=i+structuring_element[j].first+structuring_element[j].second*this->infoHeader.biWidth;
			if(offset<0 || offset>=pixels.size()) continue;
			if(new_pixels[offset].channels[0]==BINARY_1)
			{
				pixels[i].channels[0]=BINARY_1;
				pixels[i].channels[1]=BINARY_1;
				pixels[i].channels[2]=BINARY_1;

			}
		}
	}

}

void BMPcontainer::make_erosion()
{
	std::vector<std::pair<int,int> > structuring_element;
	structuring_element.push_back(std::make_pair(0,0));
	structuring_element.push_back(std::make_pair(0,1));
	structuring_element.push_back(std::make_pair(0,-1));
	structuring_element.push_back(std::make_pair(1,0));
	structuring_element.push_back(std::make_pair(-1,0));

	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> new_pixels=this->get_pixels();
	for(int i=0;i<pixels.size();i++)
	{
		pixels[i].channels[0]=BINARY_1;
		pixels[i].channels[1]=BINARY_1;
		pixels[i].channels[2]=BINARY_1;
		for(int j=0;j<=structuring_element.size();j++)
		{
			int offset=i+structuring_element[j].first+structuring_element[j].second*this->infoHeader.biWidth;
			if(offset<0 || offset>=pixels.size()) continue;
			if(new_pixels[offset].channels[0]!=BINARY_1)
			{
				pixels[i].channels[0]=BINARY_0;
				pixels[i].channels[1]=BINARY_0;
				pixels[i].channels[2]=BINARY_0;
			}
		}
	}

}

void BMPcontainer::make_opening()
{
	this->make_erosion();
	this->make_delation();
}

void BMPcontainer::make_closing()
{
	this->make_delation();
	this->make_erosion();
}

int BMPcontainer::findThrehold()
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<int> fore_pixels;
	std::vector<int> back_pixels;

	int target_threhold=1;
	float within_va=100000;
	float between_va=0;

	for(int threhold=1;threhold<255;threhold++)
	{
		fore_pixels.clear();
		back_pixels.clear();

		for(int i=0;i<pixels.size();i++)
		{
			if(pixels[i].channels[0]<threhold)
				back_pixels.push_back(i);
			else
				fore_pixels.push_back(i);
		}

		float back_average=0;
		for(std::vector<int>::iterator it = back_pixels.begin(); it != back_pixels.end(); ++it)
	    	back_average += pixels[*it].channels[0];
		back_average/=back_pixels.size();

		float fore_average=0;
		for(std::vector<int>::iterator it = fore_pixels.begin(); it != fore_pixels.end(); ++it)
	    	fore_average += pixels[*it].channels[0];
		fore_average/=fore_pixels.size();

		double back_accum  = 0.0;
		for(std::vector<int>::iterator it = back_pixels.begin(); it != back_pixels.end(); ++it)
		{
			back_accum  += std::pow(pixels[*it].channels[0]-back_average,2);
		}
		double back_stdev = std::sqrt(back_accum/(back_pixels.size())); //方差

		double fore_accum  = 0.0;
		for(std::vector<int>::iterator it = fore_pixels.begin(); it != fore_pixels.end(); ++it)
		{
			fore_accum  += std::pow(pixels[*it].channels[0]-fore_average,2);
		}
		double fore_stdev = std::sqrt(fore_accum/(fore_pixels.size())); //方差

		float w_f=1.0*fore_pixels.size()/pixels.size();
		float w_b=1.0*back_pixels.size()/pixels.size();

		float temp_within_va= fore_stdev*(w_f)+back_stdev*(w_b);
		float temp_between_va= w_f*w_b*std::pow(fore_average-back_average,2);

		if(temp_between_va>between_va && temp_within_va<within_va )
		{
			between_va=temp_between_va;
			within_va=temp_within_va;
			target_threhold=threhold;
		}

	}
	return target_threhold;
}

void BMPcontainer::make_logarithmic_enhance()
{
	this->RGBtoYUV();
	uint8_t y_max=0;

	for(std::vector<CPixel>::iterator it = this->cpixels.begin(); it != this->cpixels.end(); ++it)
		y_max = (y_max >= (*it).yuvY) ? y_max : (*it).yuvY;

	for(std::vector<CPixel>::iterator it = this->cpixels.begin(); it != this->cpixels.end(); ++it)
		(*it).yuvY = fix_range(( 255 * log((*it).yuvY/255.0+1) / log(y_max/255.0+1) ) );

	this->YUVtoRGB();
}

void BMPcontainer::histogram_equalization()
{
	this->show_first_10_pixels();
	std::map<uint8_t,int> r_map,g_map,b_map;
	for(std::vector<CPixel>::iterator it = this->cpixels.begin(); it != this->cpixels.end(); ++it)
	{
		auto temp= r_map.find((*it).rgbRed);
		if(temp!=r_map.end())
			r_map[(*it).rgbRed]+=1;
		else
			r_map[(*it).rgbRed]=0;

		temp= b_map.find((*it).rgbBlue);
		if(temp!=b_map.end())
			b_map[(*it).rgbBlue]+=1;
		else
			b_map[(*it).rgbBlue]=0;

		temp= g_map.find((*it).rgbGreen);
		if(temp!=g_map.end())
			g_map[(*it).rgbGreen]+=1;
		else
			g_map[(*it).rgbGreen]=0;
	}

	for(std::vector<CPixel>::iterator it = this->cpixels.begin(); it != this->cpixels.end(); ++it)
	{
		int size = this->infoHeader.biWidth*this->infoHeader.biHeight;
		double new_v=0;
		for(std::map<uint8_t,int>::iterator temp = r_map.begin(); temp != r_map.end(); ++temp)
		{
			if((*temp).first <= (*it).rgbRed)
			{
				// new_v += (*temp).first * (*temp).second * 1.0 / size;
				new_v += (*temp).second * 1.0 ;
			}
		}
		(*it).rgbRed=fix_range(new_v/size*255);

		new_v=0;
		for(std::map<uint8_t,int>::iterator temp = g_map.begin(); temp != g_map.end(); ++temp)
		{
			if((*temp).first < (*it).rgbGreen)
			{
				// new_v += (*temp).first * (*temp).second * 1.0 / size;
				new_v += (*temp).second * 1.0 ;
			}
		}
		(*it).rgbGreen=fix_range(new_v/size*255);

		new_v=0;
		for(std::map<uint8_t,int>::iterator temp = b_map.begin(); temp != b_map.end(); ++temp)
		{
			if((*temp).first < (*it).rgbBlue)
			{
				// new_v += (*temp).first * (*temp).second * 1.0 / size;
				new_v += (*temp).second * 1.0 ;
			}
		}
		(*it).rgbBlue=fix_range(new_v/size*255);
	}
	this->show_first_10_pixels();

}
void BMPcontainer::translate(int x_offset,int y_offset)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth*this->infoHeader.biHeight;i++)
	{
		CPixel temp_pix={ 0,0,0,0 };
		pixels.push_back(temp_pix);
	}
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		if( (i+x_offset)>= this->infoHeader.biHeight || (i+x_offset)<0) continue;
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			if( (j+y_offset)>= this->infoHeader.biWidth || (j+y_offset)<0) continue;
			pixels[(i+x_offset)*this->infoHeader.biWidth+j+y_offset]=old_pixels[i*this->infoHeader.biWidth+j];
		}
	}
}
void BMPcontainer::rotate(int x_origin,int y_origin, double r)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double x_previous=i*std::cos(-r)-j*std::sin(-r);
			double y_previous=i*std::sin(-r)+j*std::cos(-r);
			CPixel temp_pix={ 0,0,0,0 };
			pixels.push_back(temp_pix);

			if( y_previous>= this->infoHeader.biWidth || y_previous<0 || x_previous>=this->infoHeader.biHeight || x_previous<0)
			{
				continue;
			}

			pixels[i*this->infoHeader.biWidth+j]=this->make_interpolation(x_previous,y_previous,old_pixels);

		}
	}
}

CPixel BMPcontainer::make_interpolation(double x_p,double y_p,std::vector<CPixel>& pixels)
{
	CPixel pix={0,0,0,0};
	double r=0,g=0,b=0;

	int x,y;
	double sum_regu=0;

	x=std::floor(x_p);
	y=std::floor(y_p);

	if( !(y>= this->infoHeader.biWidth || y<0 || x>=this->infoHeader.biHeight || x<0))
	{
		double distance=std::sqrt(std::pow(x-x_p,2)+std::pow(y-y_p,2));
		if(distance<=MIN_DISTANCE) distance=MIN_DISTANCE;
		sum_regu+=1/distance;
		r+=pixels[x*this->infoHeader.biWidth+y].rgbRed/distance;
		g+=pixels[x*this->infoHeader.biWidth+y].rgbGreen/distance;
		b+=pixels[x*this->infoHeader.biWidth+y].rgbBlue/distance;
	}


	x=std::ceil(x_p);
	y=std::floor(y_p);

	if( !(y>= this->infoHeader.biWidth || y<0 || x>=this->infoHeader.biHeight || x<0))
	{
		double distance=std::sqrt(std::pow(x-x_p,2)+std::pow(y-y_p,2));
		if(distance<=MIN_DISTANCE) distance=MIN_DISTANCE;
		sum_regu+=1/distance;
		r+=pixels[x*this->infoHeader.biWidth+y].rgbRed/distance;
		g+=pixels[x*this->infoHeader.biWidth+y].rgbGreen/distance;
		b+=pixels[x*this->infoHeader.biWidth+y].rgbBlue/distance;
	}

	x=std::ceil(x_p);
	y=std::ceil(y_p);

	if( !(y>= this->infoHeader.biWidth || y<0 || x>=this->infoHeader.biHeight || x<0))
	{
		double distance=std::sqrt(std::pow(x-x_p,2)+std::pow(y-y_p,2));
		if(distance<=MIN_DISTANCE) distance=MIN_DISTANCE;
		sum_regu+=1/distance;
		r+=pixels[x*this->infoHeader.biWidth+y].rgbRed/distance;
		g+=pixels[x*this->infoHeader.biWidth+y].rgbGreen/distance;
		b+=pixels[x*this->infoHeader.biWidth+y].rgbBlue/distance;
	}

	x=std::floor(x_p);
	y=std::ceil(y_p);

	if( !(y>= this->infoHeader.biWidth || y<0 || x>=this->infoHeader.biHeight || x<0))
	{
		double distance=std::sqrt(std::pow(x-x_p,2)+std::pow(y-y_p,2));
		if(distance<=MIN_DISTANCE) distance=MIN_DISTANCE;
		sum_regu+=1/distance;
		r+=pixels[x*this->infoHeader.biWidth+y].rgbRed/distance;
		g+=pixels[x*this->infoHeader.biWidth+y].rgbGreen/distance;
		b+=pixels[x*this->infoHeader.biWidth+y].rgbBlue/distance;
	}
	r/=sum_regu;
	g/=sum_regu;
	b/=sum_regu;

	pix.rgbRed=fix_range(r);
	pix.rgbBlue=fix_range(b);
	pix.rgbGreen=fix_range(g);

	return pix;

}

void BMPcontainer::scale(int x_origin,int y_origin,double scale_factor)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double x_previous=(i-x_origin)/scale_factor+x_origin;
			double y_previous=(j-y_origin)/scale_factor+y_origin;

			CPixel temp_pix={0,0,0,0};
			pixels.push_back(temp_pix);

			if( y_previous>= this->infoHeader.biWidth || y_previous<0 || x_previous>=this->infoHeader.biHeight || x_previous<0)
			{
				continue;
			}

			// std::cout<<x_previous <<" "<<y_previous<<std::endl;
			pixels[i*this->infoHeader.biWidth+j]=this->make_interpolation(x_previous,y_previous,old_pixels);

		}
	}
}
void BMPcontainer::x_shear(int offset_r)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double x_previous=i+1.0*offset_r/this->infoHeader.biHeight*(j-0);
			double y_previous=j;

			CPixel temp_pix={0,0,0,0};
			pixels.push_back(temp_pix);

			if( y_previous>= this->infoHeader.biWidth || y_previous<0 || x_previous>=this->infoHeader.biHeight || x_previous<0)
			{
				continue;
			}

			// std::cout<<x_previous <<" "<<y_previous<<std::endl;
			pixels[i*this->infoHeader.biWidth+j]=this->make_interpolation(x_previous,y_previous,old_pixels);

		}
	}
}
void BMPcontainer::y_shear(int offset_r)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double x_previous=i;
			double y_previous=j+1.0*offset_r/this->infoHeader.biHeight*(i-0);

			CPixel temp_pix={0,0,0,0};
			pixels.push_back(temp_pix);

			if( y_previous>= this->infoHeader.biWidth || y_previous<0 || x_previous>=this->infoHeader.biHeight || x_previous<0)
			{
				continue;
			}

			// std::cout<<x_previous <<" "<<y_previous<<std::endl;
			pixels[i*this->infoHeader.biWidth+j]=this->make_interpolation(x_previous,y_previous,old_pixels);

		}
	}

}
void BMPcontainer::y_mirror(int pivot)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double x_previous=2*pivot-i;
			double y_previous=j;

			CPixel temp_pix={0,0,0,0};
			pixels.push_back(temp_pix);

			if( y_previous>= this->infoHeader.biWidth || y_previous<0 || x_previous>=this->infoHeader.biHeight || x_previous<0)
			{
				continue;
			}

			// std::cout<<x_previous <<" "<<y_previous<<std::endl;
			pixels[i*this->infoHeader.biWidth+j]=this->make_interpolation(x_previous,y_previous,old_pixels);

		}
	}

}
void BMPcontainer::x_mirror(int pivot)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);

	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double x_previous=i;
			double y_previous=2*pivot-j;

			CPixel temp_pix={0,0,0,0};
			pixels.push_back(temp_pix);

			if( y_previous>= this->infoHeader.biWidth || y_previous<0 || x_previous>=this->infoHeader.biHeight || x_previous<0)
			{
				continue;
			}
			pixels[i*this->infoHeader.biWidth+j]=this->make_interpolation(x_previous,y_previous,old_pixels);
		}
	}
}

void BMPcontainer::make_mean_filter(int x_window_size,int y_window_size)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);
	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			int count=0;
			int r=0,g=0,b=0;
			for(int x_offset=-x_window_size;x_offset<=x_window_size;x_offset++)
				for(int y_offset=-y_window_size;y_offset<=y_window_size;y_offset++)
				{
					// accumulate the pixel value in the window
					int x=i+x_offset;
					int y=j+y_offset;
					if( y>= this->infoHeader.biWidth || y<0 || x>=this->infoHeader.biHeight || x<0) continue;
					int index=x*this->infoHeader.biWidth+y;
					r+=old_pixels[index].rgbRed;
					g+=old_pixels[index].rgbGreen;
					b+=old_pixels[index].rgbBlue;
					count+=1;
				}

			// normal the pixel value
			r/=count;
			g/=count;
			b/=count;

			// assign value
			CPixel temp_pix={fix_range(b),fix_range(g),fix_range(r),0};
			pixels.push_back(temp_pix);
		}
	}
}

void BMPcontainer::make_laplacian_enhancement(double ratio)
{
	// get laplacian result
	std::vector<CPixel> laplacian_result_pixels = this->laplacian_result(1,1);
	// rearrange with a ratio
	this->resum_with_weight(laplacian_result_pixels,ratio);
}



void BMPcontainer::resum_with_weight(std::vector<CPixel> new_pixels,double ratio)
{
	std::vector<CPixel>& old_pixels=this->cpixels;
	std::vector<CPixel> pixels;
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			int this_index=i*this->infoHeader.biWidth+j;
			double r = old_pixels[this_index].rgbRed*(1-ratio)+new_pixels[this_index].rgbRed*ratio;
			double g = old_pixels[this_index].rgbGreen*(1-ratio)+new_pixels[this_index].rgbGreen*ratio;
			double b = old_pixels[this_index].rgbBlue*(1-ratio)+new_pixels[this_index].rgbBlue*ratio;
			CPixel temp_pix={fix_range(b),fix_range(g),fix_range(r),0};
			pixels.push_back(temp_pix);
		}
	}
}

std::vector<CPixel> BMPcontainer::laplacian_result(int x_window_size,int y_window_size)
{
	std::vector<CPixel>& old_pixels=this->cpixels;
	std::vector<CPixel> pixels;
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			int count=0;
			int r=0,g=0,b=0;
			for(int x_offset=-x_window_size;x_offset<=x_window_size;x_offset++)
				for(int y_offset=-y_window_size;y_offset<=y_window_size;y_offset++)
				{
					// accumulate the pixel value in the window
					int x=i+x_offset;
					int y=j+y_offset;
					if( y>= this->infoHeader.biWidth || y<0 || x>=this->infoHeader.biHeight || x<0      ) continue;

					int index=x*this->infoHeader.biWidth+y;
					r+=old_pixels[index].rgbRed;
					g+=old_pixels[index].rgbGreen;
					b+=old_pixels[index].rgbBlue;
					count+=1;
				}

			// normalize the pixel value
			int this_index=i*this->infoHeader.biWidth+j;
			r=old_pixels[this_index].rgbRed-(r-count*old_pixels[this_index].rgbRed);
			g=old_pixels[this_index].rgbGreen-(g-count*old_pixels[this_index].rgbGreen);
			b=old_pixels[this_index].rgbBlue-(b-count*old_pixels[this_index].rgbBlue);

			// assign value
			CPixel temp_pix={fix_range(b),fix_range(g),fix_range(r),0};
			pixels.push_back(temp_pix);
		}
	}

	return pixels;
}

static double gaussian_kernel(std::vector<double> x,std::vector<double> y,double radius)
{
	int size = x.size();
	double temp=0;
	for (int i=0;i<size;i++)
	{
		temp+=std::pow( x[i]-y[i], 2);
	}
	return std::exp(-temp/(2*std::pow(radius,2)) );
}

void BMPcontainer::make_bilateral_filter(float space_radius,float intensity_radius)
{
	std::vector<CPixel>& pixels=this->cpixels;
	std::vector<CPixel> old_pixels(pixels);
	pixels.clear();
	for(int i=0;i<this->infoHeader.biWidth;i++)
	{
		for (int j=0;j<this->infoHeader.biHeight;j++)
		{
			double nume_r=0,nume_g=0,nume_b=0,deno=0;
			std::vector<double> space_diff_1;
			space_diff_1.push_back(i);
			space_diff_1.push_back(j);
			// normal the pixel value
			int this_index=i*this->infoHeader.biWidth+j;
			auto this_p=old_pixels[this_index];
			std::vector<double> color_diff_1;
			color_diff_1.push_back(this_p.channels[0]);
			color_diff_1.push_back(this_p.channels[1]);
			color_diff_1.push_back(this_p.channels[2]);
			// loop through every pixel to do
			std::vector<double> space_diff_2;
			std::vector<double> color_diff_2;
			// for(int x=0;x<this->infoHeader.biWidth;x++)
			#define WINDOW_SIZE 10
			for(int x= ( (i-WINDOW_SIZE>=0)? i-WINDOW_SIZE:0) ;x< ( (i+ WINDOW_SIZE<this->infoHeader.biWidth)? i+WINDOW_SIZE:this->infoHeader.biWidth);x++)
			{
				// for (int y=0;y<this->infoHeader.biHeight;y++)
				for(int y= ( (j-WINDOW_SIZE>=0)? j-WINDOW_SIZE:0 );y< ( (j+ WINDOW_SIZE<this->infoHeader.biHeight)? j+WINDOW_SIZE:this->infoHeader.biHeight);y++)
				{
					space_diff_2.push_back(x);
					space_diff_2.push_back(y);
					// normal the pixel value
					int this_index=x*this->infoHeader.biWidth+y;
					auto current_p=old_pixels[this_index];
					color_diff_2.push_back(current_p.channels[0]);
					color_diff_2.push_back(current_p.channels[1]);
					color_diff_2.push_back(current_p.channels[2]);
					double space_k = gaussian_kernel(space_diff_1,space_diff_2,space_radius);
					double intensity_k = gaussian_kernel(color_diff_1,color_diff_2,intensity_radius);
					double temp=space_k*intensity_k;
					deno+=temp;
					nume_r+=this_p.rgbRed*temp;
					nume_g+=this_p.rgbGreen*temp;
					nume_b+=this_p.rgbBlue*temp;

					space_diff_2.clear();
					color_diff_2.clear();
				}
			}

			float r=nume_r / deno;
			float g=nume_g / deno;
			float b=nume_b / deno;

			// assign value
			CPixel temp_pix={fix_range(b),fix_range(g),fix_range(r),0};
			pixels.push_back(temp_pix);
		}
	}

}
