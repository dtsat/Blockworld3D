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
#include "Block.h"
#include "SOIL.h"
#include "CImg.h"

class Light {
public:
	Block light;
	Light();
	GLuint lVAO, lVBO;
	void buildLight();
	void drawLight();
};

Light::Light() {
	light = Block();
}

void Light::buildLight() {

}

void drawLight() {

}