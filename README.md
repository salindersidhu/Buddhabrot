# Buddhabrot Fractal Generator

## Description:
This application generates a Buddhabrot fractal image in a PPM (Portable Pixel Map) file. The application provides several options to customize the fractal such as iterations for each colour channel, the number of samples to collect and the dimensions of the image. The generated PPM file can be viewed and edited using popular image editing software such as GIMP and Photoshop.

For more information about the Buddhabrot rendering technique, please refer to the following article on [WIKI](https://en.wikipedia.org/wiki/Buddhabrot).

## Supports:
- Microsoft Windows 7, 8, 8.1 and 10
- Linux based distributions
- Mac OS X

## Dependencies:
- **Compiler**
	- G++ or Microsoft Visual Studio 2015 [Community Edition](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx)

## Rendering Samples:
The following renderings are scaled down and enhanced with Photoshop.
<p align="left">
	<img src="https://user-images.githubusercontent.com/12175684/40275844-30867452-5bc9-11e8-9382-85365f99d70f.png" height="420" width="420" alt="Rendering A"/>
	<img src="https://user-images.githubusercontent.com/12175684/40275843-25a7c040-5bc9-11e8-9c11-b1537635e4f5.png" height="420" width="420" alt="Rendering B"/>
</p>

The first image was originally **1000 x 1000 pixels** generated using **20 iterations** per colour channel at **50 samples** per channel. The image size was 1.33 MB.

The second image was originally **8000 x 8000 pixels** generated using **4000 iterations** per colour channel at **500 samples** per channel. The image size was 513 MB!

## Build Instructions:
Please refer to the [Wiki section](https://github.com/SalinderSidhu/Buddhabrot/wiki) for instructions on compiling and running the application.

## License:
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](/LICENSE.md)
