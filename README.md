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

## Rendering Example:
The following rendering is scaled down and enhanced with Photoshop. The raw 8000 by 8000 pixel rendering was generated using 4000 iterations per channel (Red, Green and Blue) at 500 samples per channel. The time elapsed was 5 hours and the file size was 513 MB.
<p align='center'>
	<img src='Examples\scaled_example.png' alt='Rendering Example'/>
</p>

## Build Instructions:
Please refer to the [Wiki section](https://github.com/SalinderSidhu/Buddhabrot/wiki) for instructions on compiling and running the application.

## License:
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](/LICENSE.md)
