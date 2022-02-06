# LUX
Volume modeling and rendering using C++. Many different mathematical ideas were implemented in C++ to explore different techniques in volume modeling and rendering. 

# Topics
* Scalar & vector fields
* Implicit functions, signed distance functions and different mathematical operations on them
* Level set, which can be read and converted from an obj file
* 3D Grid for different volumetric operations
* Semi-Lagrangian advection, gridless advection
* Fractal summed Perlin noise, terrain noise
* Ray marching for rendering & deep shadow maps for lighting

# Dependencies
* The .obj files and code in *vmrcode* directory were provided by [Dr. Jerry Tessendorf](https://people.cs.clemson.edu/~jtessen/)
* [tinyexr](https://github.com/syoyo/tinyexr) was used to render into exr files
* The project was compiled and built using Visual Studio 2017 on Windows
