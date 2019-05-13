## Capoc - Cave Point Clouds (and Meshes)
This repository contains source code for Capoc, an OpenGL/wxWindows application for immersive visualization and angle/distance measurements within 3D point clouds and meshes. 

Capoc was specifically developed for visualizing caves and research on 3D models of tectonic features (so-called Digital Outcrop Models) sourced by the means of Structure from Motion. As such, the program also can load and visualize NVM files and includes features for aligning the 3D data with ground control points. After alignment, dip and strike measurements can be readily performed manually on the model cross sections (tomography) or through optimal plane fitting to fragments of the point cloud.

## Usage
A reference guide is not available yet, however we do have an online tutorial:
[http://pliki.jaszczur.org/.capoc/](http://pliki.jaszczur.org/.capoc/)

## Building and installation
Capoc works on both Microsoft Windows and Linux platforms. 

Binaries not requiring an installation are provided for 32-bit and 64-bit Windows; see links at the beginning of the [tutorial](http://pliki.jaszczur.org/.capoc/) 

For building under Linux, the following steps should be generally sufficient. Please install wxWidgets development packages beforehand and make sure you have a working OpenGL installation on your system. We did not test the 

```
git clone https://github.com/mteg/capoc
cd capoc
cmake .
make
```

## License 
This code is released under Apache License version 2.0.

## Code structure
Capoc code is conceptually separated into few loosely connected parts:
* capEngine and accompanying classes (capModel, capNvm, capNvmCam, capCli, capCliThread) load, represent and process 3D models and NVM files. 
* capRender_AncientGL (implementing an abstract class capRenderer) - draws point clouds, meshes and NVM files using legacy OpenGL (display lists, glVertex3f and similar)
* wxCapoc_* classes implement a rudimentary GUI on top of the wxWidgets library and specifically wxGLCanvas class
* utility classes (capServer, capCli, tcpServer)

## References
Capoc uses:
* img.c from Survex for reading Survex 3D files [https://survex.com/](https://survex.com/)
* ply_io.c by Greg Turk [https://people.sc.fsu.edu/~jburkardt/c_src/ply_io/ply_io.html](https://people.sc.fsu.edu/~jburkardt/c_src/ply_io/ply_io.html)
* Nanojpeg from Martin Fiedler [https://keyj.emphy.de/nanojpeg/](https://keyj.emphy.de/nanojpeg/)
* LodePNG from Lode Vandevenne [https://lodev.org/lodepng/](https://lodev.org/lodepng/)
* Commonly used  SVD code from svdecomp.c in XLISP-STAT 2.1 (from Numerical Recipes, adapted by Luke Tierney and David Betz)
 
Within our own code, we also implement a canonical algorithm for registering 3D meshes using singular value decomposition by Besl and McKay (https://ieeexplore.ieee.org/document/121791).

## Authors
* Mateusz Golicz github.com/mteg authored the code
* Jacek Szczygieł provided conceptual guidance, especially for the research features
