# Simple BMP image processing

> This is a C++ library to process BMP file

supported functions are:

1. **basic usages**:

  - load and save images(color and grayscale)

  - convert to grayscale

  - RGBtoYUV and reverse

  - rearrange pixel value

2. **simple manipulate usages**:

  - make biary image

  - erosion and delation

  - opening and closing

3. **simple transform**:

  - translate
  - scale
  - rotate
  - shear
  - mirror

4. **simple enhancement usages**:

  - logarithmic enahancement
  - histogram equalization
  - image mean filter
  - laplacian enha
  - standard biletral filtering

## basic usage

see `example/example_basic_processing.cpp` for how to use

## simple manipulation

### make binary

寻找合适的threhold

![](./test_image/lena512color.bmp)

![](./test_image/threhold.bmp)

### delation

![](./test_image/threhold_delation.bmp)

### erosion

![](./test_image/threhold_erosion.bmp)

### closing

![](./test_image/threhold_closing.bmp)

### opening

![](./test_image/threhold_opening.bmp)

### logarithmic enahancement

![](./test_image/logarithmic_enhance.bmp)

### histogram equalization

![](./test_image/histogram_equalization.bmp)

### rotate

![](./test_image/rotate.bmp)

### scale

![](./test_image/scale.bmp)

#### mirror

![](./test_image/y_mirror.bmp)

![](./test_image/x_mirror.bmp)

### shear

![](./test_image/x_shear.bmp)

![](./test_image/y_shear.bmp)

#### mean filter

通过调用

```c++
image.make_mean_filter(2,2);
```

![](./test_image/mean_filter.bmp)

mean filter using a (2_2+1)_(2_2+1)=5_% window

#### laplapcian enhancement

window size 3*3

![](./test_image/laplacian_result_window1.bmp)

window size 5*5

![](./test_image/laplacian_result_window2.bmp)

re-sum with the original image with a wight of 0.2

![](./test_image/laplacian_enhancement.bmp)

##### mean filter

by calling

```c++
image.make_bilateral_filter(3,10);
```

![](./test_image/bilateral_filter.bmp)
