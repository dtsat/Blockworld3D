#pragma once

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include "SOIL.h"
#include "CImg.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

#include "Block.h"

class Plane
{
public:
	/// Attributes
	const char* fullName;
	std::vector<GLuint> pVAO, pVBO;
	GLuint planeTex;
	std::vector<std::vector<int>> heights;
	std::vector<std::vector<GLfloat>> terrainCoords;
	std::vector<GLfloat> topCoords;
	int planeSize;

	/// Constructors
	Plane();

	/// Functions
	void drawPlane();
	void heightMap();
	void buildPoints();
	void cleanUp();
};

