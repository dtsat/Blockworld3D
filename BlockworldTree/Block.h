#pragma once

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>

class Block
{
public:
	/// Attributes
	std::vector<GLfloat> vertices;

	/// Constructors
	Block();
	Block(GLfloat x, GLfloat y, GLfloat z, GLfloat s);
	Block(GLfloat x, GLfloat y, GLfloat z, GLfloat s, int i);
};

