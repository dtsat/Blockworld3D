#include "Plane.h"

Plane::Plane()
{
	heightMap();

	buildPoints();
	
	int texWidth, texHeight;
	unsigned char* texType = 0;

	texType = SOIL_load_image("../textures/grass.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &planeTex);
	glBindTexture(GL_TEXTURE_2D, planeTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texType);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(texType);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < terrainCoords.size(); i++) {
		pVAO.push_back(GLuint());
		pVBO.push_back(GLuint());
	}

	for (int i = 0; i < terrainCoords.size(); i++) {
		glGenVertexArrays(1, &pVAO[i]);
		glGenBuffers(1, &pVBO[i]);

		glBindVertexArray(pVAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, pVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, terrainCoords[i].size()*sizeof(GLfloat), &terrainCoords[i][0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Plane::buildPoints()
{
	std::cout << "\n\nLoading terrain.." << std::endl;
	std::vector<glm::vec3> allPoints;
	for (int w = 0; w < planeSize; w++)
		for (int h = 0; h < planeSize; h++)
		{
			glm::vec3 blockCoords(w, 0, h);
			allPoints.push_back(blockCoords);
		}
	terrainCoords.push_back(std::vector<GLfloat>());

	for (int i = 0; i < planeSize; i++)
	{
		for (int j = 0; j < planeSize; j++)
		allPoints[i].y = heights[i][j];
	}

	int percent = (planeSize / 100), percentage = 0;
	int currentVector = 0;
	int index = 0;

	for (int i = 0; i < planeSize; i++)
	{
		for (int j = 0; j < planeSize; j++) {
			int minHeight;
			if (i == 0 && j == 0 && j < planeSize - 1)
				minHeight = std::min(heights[i][j + 1], heights[i + 1][j]);
			else if (i == 0 && j > 0 && j < planeSize - 1) {
				minHeight = std::min(heights[i][j + 1], heights[i + 1][j]);
				minHeight = std::min(minHeight, heights[i][j - 1]);
			}
			else if (i >= 1 && i < planeSize - 1 && j < planeSize - 1 && j > 0) {
				minHeight = std::min(heights[i][j + 1], heights[i + 1][j]);
				minHeight = std::min(minHeight, heights[i][j - 1]);
				minHeight = std::min(minHeight, heights[i - 1][j]);
			}
			else if (i >= planeSize - 1 && j < planeSize - 1 && j > 0) {
				minHeight = std::min(heights[i][j + 1], heights[i - 1][j]);
				minHeight = std::min(minHeight, heights[i][j - 1]);
			}
			else if (i >= planeSize - 1 && j >= planeSize - 1 && j > 0)
				minHeight = std::min(heights[i][j - 1], heights[i - 1][j]);
			minHeight = std::max(minHeight, 0);
			heights[i][j] = std::max(heights[i][j], 1);
			if (terrainCoords[currentVector].size() >= 1800000) {
				terrainCoords.push_back(std::vector<GLfloat>());
				currentVector++;
			}
			for (int k = minHeight; k < heights[i][j]; k++)
			{
				Block block(allPoints[index].x, k, allPoints[index].z, 1.0);
				for (int l = 0; l < block.vertices.size(); l++)
				{
					terrainCoords[currentVector].push_back(block.vertices[l]);
				}
			}
			Block block2(allPoints[index].x, heights[i][j], allPoints[index].z, 1.0, 1);
			for (int l = 0; l < block2.vertices.size(); l++)
			{
				terrainCoords[currentVector].push_back(block2.vertices[l]);
			}
			
			index++;
			if ( i == planeSize - 1 && j == planeSize - 1)
				std::cout << "100%.." << std::endl;
		}
		if (i % (percent * 5) == 0) {
			std::cout << percentage << "%.." << std::endl;
			percentage += 5;
		}
	}
}

void Plane::heightMap()
{
	srand(time(NULL));
	using namespace std;
	int nbr = rand() % 12 + 1;
	string fullName_str = "../maps/map_";
	fullName_str += std::to_string(nbr);
	fullName_str += ".bmp";
	fullName = fullName_str.c_str();
	cout << fullName;
	//cimg_library::CImg<float> image(fullName);
	cimg_library::CImg<float> image(fullName);
	if (!image)
		cout << "Couldn't load the map" << endl;
	int width = image.width();
	int height = image.height();
	this->planeSize = width;
	for (int i = 0; i < width; i++) {
		heights.push_back(std::vector<int>());
		for (int j = 0; j < height; j++) {
			heights[i].push_back(int());
			float r = image(i, j, 0, 0);
			float g = image(i, j, 0, 1);
			float b = image(i, j, 0, 2);
			float avg = (r + g + b) / 3;
			if (avg < 6)
				heights[i][j] = 0;
			else if (avg < 12)
				heights[i][j] = 1;
			else if (avg < 18)
				heights[i][j] = 2;
			else if (avg < 34)
				heights[i][j] = 3;
			else if (avg < 40)
				heights[i][j] = 4;
			else if (avg < 46)
				heights[i][j] = 5;
			else if (avg < 52)
				heights[i][j] = 6;
			else if (avg < 58)
				heights[i][j] = 7;
			else if (avg < 64)
				heights[i][j] = 8;
			else if (avg < 70)
				heights[i][j] = 9;
			else if (avg < 76)
				heights[i][j] = 10;
			else if (avg < 82)
				heights[i][j] = 11;
			else if (avg < 88)
				heights[i][j] = 12;
			else if (avg < 94)
				heights[i][j] = 13;
			else if (avg < 100)
				heights[i][j] = 14;
			else if (avg < 106)
				heights[i][j] = 15;
			else if (avg < 112)
				heights[i][j] = 16;
			else if (avg < 118)
				heights[i][j] = 17;
			else if (avg < 124)
				heights[i][j] = 18;
			else if (avg < 130)
				heights[i][j] = 19;
			else if (avg < 136)
				heights[i][j] = 20;
			else if (avg < 142)
				heights[i][j] = 21;
			else if (avg < 148)
				heights[i][j] = 22;
			else if (avg < 154)
				heights[i][j] = 23;
			else if (avg < 160)
				heights[i][j] = 24;
			else if (avg < 166)
				heights[i][j] = 25;
			else if (avg < 172)
				heights[i][j] = 26;
			else if (avg < 178)
				heights[i][j] = 27;
			else if (avg < 184)
				heights[i][j] = 28;
			else if (avg < 190)
				heights[i][j] = 29;
			else if (avg < 196)
				heights[i][j] = 30;
			else if (avg < 202)
				heights[i][j] = 31;
			else if (avg < 208)
				heights[i][j] = 32;
			else if (avg < 214)
				heights[i][j] = 33;
			else if (avg < 220)
				heights[i][j] = 34;
			else if (avg < 226)
				heights[i][j] = 35;
			else if (avg < 232)
				heights[i][j] = 36;
			else if (avg < 238)
				heights[i][j] = 37;
			else if (avg < 244)
				heights[i][j] = 38;
			else if (avg < 250)
				heights[i][j] = 39;
			else if (avg < 256)
				heights[i][j] = 40;
		}
	}
}

void Plane::drawPlane()
{
	for (int i = 0; i < terrainCoords.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, planeTex);
		glBindVertexArray(pVAO[i]);
		glDrawArrays(GL_TRIANGLES, 0, 36 * 6250);
		glBindVertexArray(0);
	}
}

void Plane::cleanUp() {
	for (int i = 0; i < pVAO.size(); i++)
		glDeleteVertexArrays(1, &pVAO[i]);
	for (int i = 0; i < pVBO.size(); i++)
		glDeleteVertexArrays(1, &pVBO[i]);
}