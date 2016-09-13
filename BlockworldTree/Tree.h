#pragma once

#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include "SOIL.h"

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

class Tree
{
public:
	/// Attributes
	Block trunk;
	Block leaves;
	std::vector<GLfloat> trunkCoords;
	std::vector<glm::vec3> leafTrans;
	std::vector<GLfloat> leafCoords;
	GLuint tVAO[2], tVBO[2];
	GLuint barkTex, leafTex;
	int treeHeight;

	/// Constructors
	Tree();
	Tree(GLfloat x, GLfloat y, GLfloat z, GLfloat s);

	/// Functions
	void drawTree();
	void leavesDraw(int x);
	void cleanUp();
};

