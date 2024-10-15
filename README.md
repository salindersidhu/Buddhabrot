# Buddhabrot Fractal Generator

[![Contributors](https://img.shields.io/github/contributors/salindersidhu/Buddhabrot?style=for-the-badge)](https://github.com/salindersidhu/Buddhabrot/graphs/contributors) [![Visitors](https://api.visitorbadge.io/api/visitors?path=https%3A%2F%2Fgithub.com%2Fsalindersidhu%2FBuddhabrot&countColor=%23263759)](https://visitorbadge.io/status?path=https%3A%2F%2Fgithub.com%2Fsalindersidhu%2FBuddhabrot) [![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=for-the-badge)](/LICENSE.md)

## Overview

This application generates a Buddhabrot fractal image in a PPM (Portable Pixel Map) file. The application provides several options to customize the fractal such as iterations for each colour channel, the number of samples to collect and the dimensions of the image. The generated PPM file can be viewed and edited using popular image editing software such as GIMP and Photoshop.

[Click here](https://en.wikipedia.org/wiki/Buddhabrot) for more information about the Buddhabrot rendering technique.

## Prerequisite Software

| Software                       | Version |
| :----------------------------- | :------ |
| G++ or Microsoft Visual Studio | 2015+   |

## Features

The following renderings are scaled down and enhanced with Photoshop.

<p align="left">
	<img src="https://user-images.githubusercontent.com/12175684/40275844-30867452-5bc9-11e8-9382-85365f99d70f.png" height="420" width="420" alt="Rendering A"/>
	<img src="https://user-images.githubusercontent.com/12175684/40275843-25a7c040-5bc9-11e8-9c11-b1537635e4f5.png" height="420" width="420" alt="Rendering B"/>
</p>

The first image was originally **1000 x 1000 pixels** generated using **20 iterations** per colour channel at **50 samples** per channel.

The second image was originally **8000 x 8000 pixels** generated using **4000 iterations** per colour channel at **500 samples** per channel.

## Getting Started

You will need to setup Microsoft Visual Studio for Windoes or G++ for Linx or Mac.

### Windows

1. Open `Buddhabrot.sln` in Microsoft Visual Studio 2015.

2. Change `Solutions Configurations` to Release.

3. Change the `Solution Platform` to `x64`.

4. Right click `Buddhabrot` in the Solution Explorer and select `Build` to build `Buddhabrot.exe`.

### Mac or Linux

1. Navigate to the Buddhabrot source code using the following command:

```bash
cd Buddhabrot
```

2. Compile Buddhabrot using the following command:

```bash
make
```

## Running

### Windows

Double click `Buddhabrot.exe` in the `x64\Release` folder.

### Mac or Linux

Run Buddhabrot using the following command:

```bash
./Buddhabrot
```

## Contributing

Please see our [Contributing Guide](/CONTRIBUTING.md) for more info.
