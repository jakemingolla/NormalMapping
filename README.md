# Normal Mapping #

**Jake Mingolla**

**November, 2015**

**Category: Computer Graphics**

**Language(s): C++ / GLSL**

#### About

Implements shadows on a 2-D surface using normal mapping onto a bump image map using shaders. The color of a pixel making up the brick wall is a combination of that specific coordinate on the brick texture and the dot product of the bump normal mapping at the same coordinate with the light vector. The point emanating the light slowly rotates the brick wall, providing dynamic shadows calculated by the shader.

This project began as a lab assignment in Remco Chang's [Comp 175 -- Computer Graphics](http://www.cs.tufts.edu/comp/175) in Spring 2015. The only files I worked on where the fragment and vertex shaders (bump.frag and bump.vert) to understand the GLSL pipeline -- all work handling the management of shader linking and image loading was handled by Mike Shah, the graduate TA for the Computer Graphics class.



#### Dependencies
* OpenGL > 3.x
  * Standard graphics library
  * Earlier version do not provide shader support to the OpenGL libraries
* GLUI
  * Provides user interface support for OpenGL
* GLEW
  * Manages OpenGL extensions (i.e. GLee)
* GLee 5.5
  * Allows for linking of shared libraries by making tedious function pointer management easy.

#### Collaborators
* **Mike Shah** for providing almost all of the support code necessary for loading .ppm image files, linking together shader files in the shaderManager.cpp, and setting up GLee.
* **Remco Chang** for introducing me to GLSL and providing the initial lab files.