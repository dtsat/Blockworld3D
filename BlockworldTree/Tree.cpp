#include "Tree.h"
#include <iostream>


Tree::Tree()
{
	trunk = Block();
	leaves = Block();

	treeHeight = rand() % 6 + 4;

	int numBark = rand() % 4 + 1;
	int numLeaf = rand() % 4 + 1;

	int texWidth, texHeight;
	unsigned char* barkType = 0;
	unsigned char* leafType = 0;

	switch (numBark)
	{
		case 1:
		{
			barkType = SOIL_load_image("../textures/birch.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 2:
		{
			barkType = SOIL_load_image("../textures/oak.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 3:
		{
			barkType = SOIL_load_image("../textures/jungle.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 4:
		{
			barkType = SOIL_load_image("../textures/spruce.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
	}

	switch (numLeaf)
	{
		case 1:
		{
			leafType = SOIL_load_image("../textures/leaves_birch.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 2:
		{
			leafType = SOIL_load_image("../textures/leaves_jungle.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 3:
		{
			leafType = SOIL_load_image("../textures/leaves_oak.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 4:
		{
			leafType = SOIL_load_image("../textures/leaves_spruce.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
	}

	glGenTextures(1, &barkTex);
	glBindTexture(GL_TEXTURE_2D, barkTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, barkType);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(barkType);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &leafTex);
	glBindTexture(GL_TEXTURE_2D, leafTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, leafType);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(leafType);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < trunk.vertices.size(); i++)
	{
		trunkCoords.push_back(trunk.vertices[i]);
	}

	glm::vec3 translation(0.0f, 0.5f, 0.0f);

	for (int i = 0; i < (treeHeight*trunk.vertices.size()) - 288; i += 5)
	{
		glm::vec3 original(trunkCoords[i], trunkCoords[i + 1], trunkCoords[i + 2]);
		glm::vec3 moved = original + translation;
		trunkCoords.push_back(moved.x);
		trunkCoords.push_back(moved.y);
		trunkCoords.push_back(moved.z);
		glm::vec2 tex(trunkCoords[i + 3], trunkCoords[i + 4]);
		trunkCoords.push_back(tex.x);
		trunkCoords.push_back(tex.y);
		glm::vec3 norm(trunkCoords[i + 5], trunkCoords[i + 6], trunkCoords[i + 7]);
		trunkCoords.push_back(norm.x);
		trunkCoords.push_back(norm.y);
		trunkCoords.push_back(norm.z);
	}

	for (int i = 288 * (treeHeight - 1); i < trunkCoords.size(); i++)
	{
		leafCoords.push_back(trunkCoords[i]);
	}

	int factor = rand() % 3 + 1;

	leavesDraw(factor);

	glGenVertexArrays(1, &tVAO[0]);
	glGenBuffers(1, &tVBO[0]);

	glBindVertexArray(tVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, trunkCoords.size()*sizeof(GLfloat), &trunkCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &tVAO[1]);
	glGenBuffers(1, &tVBO[1]);

	glBindVertexArray(tVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, leafCoords.size()*sizeof(GLfloat), &leafCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Tree::Tree(GLfloat x, GLfloat y, GLfloat z, GLfloat s)
{
	s = 0.5f;
	trunk = Block(x, y, z, s);
	leaves = Block();

	treeHeight = rand() % 6 + 4;

	int numBark = rand() % 4 + 1;
	int numLeaf = rand() % 4 + 1;

	int texWidth, texHeight;
	unsigned char* barkType = 0;
	unsigned char* leafType = 0;

	switch (numBark)
	{
		case 1:
		{
			barkType = SOIL_load_image("../textures/birch.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 2:
		{
			barkType = SOIL_load_image("../textures/oak.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 3:
		{
			barkType = SOIL_load_image("../textures/jungle.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 4:
		{
			barkType = SOIL_load_image("../textures/spruce.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
	}

	switch (numLeaf)
	{
		case 1:
		{
			leafType = SOIL_load_image("../textures/leaves_birch.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 2:
		{
			leafType = SOIL_load_image("../textures/leaves_jungle.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 3:
		{
			leafType = SOIL_load_image("../textures/leaves_oak.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
		case 4:
		{
			leafType = SOIL_load_image("../textures/leaves_spruce.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
			break;
		}
	}

	glGenTextures(1, &barkTex);
	glBindTexture(GL_TEXTURE_2D, barkTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, barkType);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(barkType);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &leafTex);
	glBindTexture(GL_TEXTURE_2D, leafTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, leafType);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(leafType);
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < trunk.vertices.size(); i++)
	{
		trunkCoords.push_back(trunk.vertices[i]);
	}

	glm::vec3 translation(0.0f, 0.5f, 0.0f);

	for (int i = 0; i < (treeHeight*trunk.vertices.size()) - 288; i += 8)
	{
		glm::vec3 original(trunkCoords[i], trunkCoords[i + 1], trunkCoords[i + 2]);
		glm::vec3 moved = original + translation;
		trunkCoords.push_back(moved.x);
		trunkCoords.push_back(moved.y);
		trunkCoords.push_back(moved.z);
		glm::vec2 tex(trunkCoords[i + 3], trunkCoords[i + 4]);
		trunkCoords.push_back(tex.x);
		trunkCoords.push_back(tex.y);
		glm::vec3 norm(trunkCoords[i + 5], trunkCoords[i + 6], trunkCoords[i + 7]);
		trunkCoords.push_back(norm.x);
		trunkCoords.push_back(norm.y);
		trunkCoords.push_back(norm.z);
	}

	for (int i = 288 * (treeHeight - 1); i < trunkCoords.size(); i++)
	{
		leafCoords.push_back(trunkCoords[i]);
	}

	int factor = rand() % 3 + 1;

	leavesDraw(factor);

	glGenVertexArrays(1, &tVAO[0]);
	glGenBuffers(1, &tVBO[0]);

	glBindVertexArray(tVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, trunkCoords.size()*sizeof(GLfloat), &trunkCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &tVAO[1]);
	glGenBuffers(1, &tVBO[1]);

	glBindVertexArray(tVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, leafCoords.size()*sizeof(GLfloat), &leafCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Tree::drawTree()
{
	glBindTexture(GL_TEXTURE_2D, barkTex);
	glBindVertexArray(tVAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36 * treeHeight);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, leafTex);
	glBindVertexArray(tVAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, 20000);
	glBindVertexArray(0);
}

void Tree::leavesDraw(int factor)
{
	glm::vec3 north(0.0f, 0.0f, 0.5f);
	glm::vec3 south(0.0f, 0.0f, -0.5f);
	glm::vec3 west(-0.5f, 0.0f, 0.0f);
	glm::vec3 east(0.5f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 0.5f, 0.0f);
	glm::vec3 northWest(-0.5f, 0.0f, 0.5f);
	glm::vec3 northEast(0.5f, 0.0f, 0.5f);
	glm::vec3 southWest(-0.5f, 0.0f, -0.5f);
	glm::vec3 southEast(0.5f, 0.0f, -0.5f);

	for (int i = 0; i < 288; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + up;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 0; i < 288; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + north;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 0; i < 288; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + south;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 0; i < 288; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + east;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 0; i < 288; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + west;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}

	for (int i = 288; i < 576; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + up;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 288; i < 576; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + north;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 288; i < 576; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + south;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 288; i < 576; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + east;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}
	for (int i = 288; i < 576; i += 8)
	{
		glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

		glm::vec3 moved = original + west;
		leafCoords.push_back(moved.x);
		leafCoords.push_back(moved.y);
		leafCoords.push_back(moved.z);
		glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
		leafCoords.push_back(tex.x);
		leafCoords.push_back(tex.y);
		glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
		leafCoords.push_back(norm.x);
		leafCoords.push_back(norm.y);
		leafCoords.push_back(norm.z);
	}

	if (factor == 2)
	{
		for (int i = 1728; i < 2016; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + up;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 1728; i < 2016; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + north;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 1728; i < 2016; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + south;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 1728; i < 2016; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + east;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 1728; i < 2016; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + west;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 2016; i < 2304; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + north;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 2304; i < 2592; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + south;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 2592; i < 2880; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + east;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 2880; i < 3168; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + west;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
	}
	if (factor == 3)
	{
		for (int i = 576; i < 864; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + north;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 864; i < 1152; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + south;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 1152; i < 1440; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + east;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
		for (int i = 1440; i < 1728; i += 8)
		{
			glm::vec3 original(leafCoords[i], leafCoords[i + 1], leafCoords[i + 2]);

			glm::vec3 moved = original + west;
			leafCoords.push_back(moved.x);
			leafCoords.push_back(moved.y);
			leafCoords.push_back(moved.z);
			glm::vec2 tex(leafCoords[i + 3], leafCoords[i + 4]);
			leafCoords.push_back(tex.x);
			leafCoords.push_back(tex.y);
			glm::vec3 norm(leafCoords[i + 5], leafCoords[i + 6], leafCoords[i + 7]);
			leafCoords.push_back(norm.x);
			leafCoords.push_back(norm.y);
			leafCoords.push_back(norm.z);
		}
	}
}

void Tree::cleanUp() {
	glDeleteVertexArrays(2, tVAO);
	glDeleteBuffers(2, tVBO);
}