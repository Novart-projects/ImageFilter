# Image Processor

## Description

Image processor is a simple console application to filter .bmp images.

## How to build

Go to cmake `cmake-build-debug` and use command `make ImageProcessor`. 
You will have executable binary file `ImageProcessor` at the same folder `cmake-build-debug`.

## How to use
To apply sequence of filters on image use this arguments pattern:

`{application path} {input file path} {output file path}
[-{filter name 1} [filter argument 1] [filter argument 2] ...]
[-{filter name 2} [filter argument 1] [filter argument 2] ...] ...`

To get help info just run image processor without any arguments.

### List of available filters:
`-neg` - make image negative

`-gs` - make a black and white image

`-sharp` - raise sharpness of image

`-edge threshold` - detect edges (`threshold` is a floating point number from 0 to 1)

`-blur sigma` - blur image (`sigma` is a positive number)

`-glass` - make glass effect

`-wave` - make glass wave looking effect

`-paint` - make your picture look like painting

`-sketch` - make your picture look like sketch (based on `-paint`)

`-crop width height` - crop image (`width` and `height` are positive integer numbers)

## Supported file types

Application supports only 24-bit BMP image without compression and color table. Type of used `DIB header` - `BITMAPINFOHEADER`