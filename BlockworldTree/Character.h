#pragma once
#include "glew.h"		// include GL Extension Wrangler

#include "glfw3.h"  // include GLFW helper library

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"

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
#include "SOIL.h"

using namespace std;


class Character
{
public:
	vector<GLfloat> charPoints, segwayPoints;
	GLuint texture_1, texture_2;
	Character();
	void drawChar();
	void drawSegway();
	void initialization(GLuint vao, GLuint vbo, std::vector<GLfloat> vector);
	GLuint VBO[2], VAO[2];
	void cleanUp();
};