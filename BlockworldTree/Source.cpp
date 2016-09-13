#include "glew.h"
#include "glfw3.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include "irrKlang.h"

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

#include "Tree.h"
#include "Plane.h"
#include "SOIL.h"
#include "Character.h"

using namespace std;

/// Window object
GLFWwindow* window = 0x00;

/// Shader programs
GLuint world_program = 0, char_program = 0;

/// Position of matrices in shaders
GLuint view_matrix_id = 0, charView_matrix_id = 0;
GLuint model_matrix_id = 0, charModel_matrix_id = 0;
GLuint proj_matrix_id = 0, charProj_matrix_id = 0;
GLuint light_color_id = 0;
GLuint light_pos_id = 0;
GLuint view_pos_id = 0, charView_pos_id = 0;

/// Initial size of window
GLuint HEIGHT = 1280, WIDTH = 720;

/// Delta Time variables
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat cameraSpeed;

/// Music and Sounds
irrklang::ISoundEngine *soundEngine = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine *soundEngine_2 = irrklang::createIrrKlangDevice();

/// Camera & character vectors
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f ,0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos, charPos;
glm::vec3 tempCam;

GLfloat lastX = WIDTH / 2, lastY = HEIGHT / 2;
GLfloat yaw = -90.0f, pitch = 0.0f;

/// Set field of view to 45 degrees
GLfloat fov = 45.0f;

/// Used to register which keys are being pressed
bool keys[1024], firstMouse = true;
double xposi, yposi;

int camMode = 1, camFactor = 2, mapSize;

/// Transformations matrices
glm::mat4 proj_matrix, charProj_matrix;
glm::mat4 view_matrix, charView_matrix;
glm::mat4 model_matrix, charModel_matrix;

vector<Tree> gameTrees;

// Function to handle the keyboard input
void keyPressed(GLFWwindow *_window, int key, int scancode, int action, int mods) {

	/// Close window with escape
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	/// Render with lines
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/// Render with triangles
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (key == GLFW_KEY_1 && action == GLFW_PRESS && camMode != 1) {
		camMode = 1;
		cameraPos = glm::vec3(mapSize / 2, 50.0f, mapSize / 2);
		camFactor = 2;
		
		if (!soundEngine->isCurrentlyPlaying("Music and Sounds/Theme Music.mp3"))
		{
			soundEngine->stopAllSounds();
			soundEngine->play2D("Music and Sounds/Theme Music.mp3", GL_TRUE);
		}
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS && camMode != 2) {
		camMode = 2;
		cameraPos.y += 3.0;
		cameraPos.z -= 2.0;
		camFactor = 5;
		if (!soundEngine->isCurrentlyPlaying("Music and Sounds/Theme Music.mp3"))
		{
			soundEngine->stopAllSounds();
			soundEngine->play2D("Music and Sounds/Theme Music.mp3", GL_TRUE);
		}
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS && camMode != 3) {
		camMode = 3;
		cameraPos.y += 20.0;
		camFactor = 0;
		if (!soundEngine->isCurrentlyPlaying("Music and Sounds/John Farnham - Thunder In Your Heart.mp3"))
		{
			soundEngine->stopAllSounds();
			soundEngine->play2D("Music and Sounds/John Farnham - Thunder In Your Heart.mp3", GL_TRUE);
		}
	}
	
	/// Check if key is pressed/held down/released
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
			soundEngine_2->stopAllSounds();
		}
	}
	return;
}

// Function to control the rotation of the camera around the model
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void cameraMove(vector<vector<int>> map, int size)
{
	/// Camera speed adjusted for Delta Time
	cameraSpeed = 10.0f * deltaTime;

	/// Increase movement speed
	if (keys[GLFW_KEY_LEFT_SHIFT])
		cameraSpeed = 17.5f * deltaTime;

	/// Moving forward
	if (keys[GLFW_KEY_W])
	{
		if (camMode == 3)
			tempCam = cameraFront * glm::vec3(1.0, 1.0, 1.0);
		else
			tempCam = cameraFront * glm::vec3(1.0, 0.0, 1.0);
		glm::vec3 newCam = cameraPos + (cameraSpeed * tempCam);
		if (camMode != 3 && cameraPos.y < map[(int)newCam.x][(int)newCam.z] + camFactor);
		else {
			if ((newCam.x < 1 || newCam.x > size - 1 || newCam.z < 1 || newCam.z > size - 1) && camMode != 3)
				cameraPos = glm::vec3(size / 2, 50.0f, size / 2);
			else
				cameraPos += cameraSpeed * tempCam;
		}
		
		/// Activate the footsteps sounds (One at a time)
		if (camMode != 3)
			while (!soundEngine_2->isCurrentlyPlaying("Music and Sounds/Footsteps Sound.wav"))
				soundEngine_2->play2D("Music and Sounds/Footsteps Sound.wav", GL_FALSE);
	}

	/// Moving backwards
	if (keys[GLFW_KEY_S]) {
		if (camMode == 3)
			tempCam = cameraFront * glm::vec3(1.0, 1.0, 1.0);
		else
			tempCam = cameraFront * glm::vec3(1.0, 0.0, 1.0);
		glm::vec3 newCam = cameraPos - cameraSpeed * tempCam;
		if (camMode != 3 && cameraPos.y < map[(int)newCam.x][(int)newCam.z] + camFactor);
		else {
			if ((newCam.x < 1 || newCam.x > size - 1 || newCam.z < 1 || newCam.z > size - 1) && camMode != 3)
				cameraPos = glm::vec3(size / 2, 50.0f, size / 2);
			else
				cameraPos -= cameraSpeed * tempCam;
		}

		/// Activate the footsteps sounds (One at a time)
		if (camMode != 3)
			while (!soundEngine_2->isCurrentlyPlaying("Music and Sounds/Footsteps Sound.wav"))
				soundEngine_2->play2D("Music and Sounds/Footsteps Sound.wav", GL_FALSE);
	}

	/// Moving to the left
	if (keys[GLFW_KEY_A]) {
		if (camMode == 3)
			tempCam = cameraFront * glm::vec3(1.0, 1.0, 1.0);
		else
			tempCam = cameraFront * glm::vec3(1.0, 0.0, 1.0);
		glm::vec3 newCam = cameraPos - glm::normalize(glm::cross(tempCam, cameraUp)) * cameraSpeed;
		if (camMode != 3 && cameraPos.y < map[(int)newCam.x][(int)newCam.z] + camFactor);
		else {
			if ((newCam.x < 1 || newCam.x > size - 1 || newCam.z < 1 || newCam.z > size - 1) && camMode != 3)
				cameraPos = glm::vec3(size / 2, 50.0f, size / 2);
			else
				cameraPos -= glm::normalize(glm::cross(tempCam, cameraUp)) * cameraSpeed;
		}

		/// Activate the footsteps sounds (One at a time)
		if (camMode != 3)
			while (!soundEngine_2->isCurrentlyPlaying("Music and Sounds/Footsteps Sound.wav"))
				soundEngine_2->play2D("Music and Sounds/Footsteps Sound.wav", GL_FALSE);
	}

	/// Moving to the right
	if (keys[GLFW_KEY_D]) {
		if (camMode == 3)
			tempCam = cameraFront * glm::vec3(1.0, 1.0, 1.0);
		else
			tempCam = cameraFront * glm::vec3(1.0, 0.0, 1.0);
		glm::vec3 newCam = cameraPos + glm::normalize(glm::cross(tempCam, cameraUp)) * cameraSpeed;
		if (camMode != 3 && cameraPos.y < map[(int)newCam.x][(int)newCam.z] + camFactor);
		else {
			if ((newCam.x < 1 || newCam.x > size - 1 || newCam.z < 1 || newCam.z > size - 1) && camMode != 3)
				cameraPos = glm::vec3(size / 2, 50.0f, size / 2);
			else
				cameraPos += glm::normalize(glm::cross(tempCam, cameraUp)) * cameraSpeed;
		}

		/// Activate the footsteps sounds (One at a time)
		if (camMode != 3)
			while (!soundEngine_2->isCurrentlyPlaying("Music and Sounds/Footsteps Sound.wav"))
				soundEngine_2->play2D("Music and Sounds/Footsteps Sound.wav", GL_FALSE);
	}

	if (keys[GLFW_KEY_SPACE]) {
		cameraSpeed = 10.0f * deltaTime;
		cameraPos += glm::vec3(0.0, cameraSpeed * 12.0, 0.0);

		keys[GLFW_KEY_SPACE] = false;

		/// Activate the jumping sound
		if (camMode != 3) {
			soundEngine_2->setSoundVolume(0.5f);
			soundEngine_2->play2D("Music and Sounds/Jump Sound.wav", GL_FALSE);
			soundEngine_2->setSoundVolume(1.0f);
		}
	}
}

// Callback function for resizing the window
void resize_callback(GLFWwindow* window, int h, int w)
{
	HEIGHT = h;
	WIDTH = w;
}

// Function to initialize all values, the window, and callback functions
bool initialize() {
	/// Initialize GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	/// Create a window of size 640x480 and with title "Lecture 2: First Triangle"
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	/// Fullscreen
	// window = glfwCreateWindow(1920, 1080, "Block World 3D", glfwGetPrimaryMonitor(), NULL);
	/// Windowed
	window = glfwCreateWindow(1280, 720, "Block World 3D", nullptr, nullptr);

	/// Hide the game window until the loading is done
	glfwHideWindow(window);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	int w, h;
	glfwGetWindowSize(window, &w, &h);
	/// Register the keyboard callback function
	glfwSetKeyCallback(window, keyPressed);
	/// Register resize callback function
	glfwSetWindowSizeCallback(window, resize_callback);
	/// Register mouse callback
	glfwSetCursorPosCallback(window, mouse_callback);
	/// Disable the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	/// Make the current window the one used
	glfwMakeContextCurrent(window);

	/// Initialize GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	/// Get the current OpenGL version
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/// Enable the depth test i.e. draw a pixel if it's closer to the viewer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	/// Enable face culling for back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	/// Activates main theme song
	soundEngine->play2D("Music and Sounds/Theme Music.mp3", GL_TRUE);

	return true;
}

// Function to get rid of all buffer objects and close the window
bool cleanUp(Plane plane, vector<Tree> gameTrees, Character character) {
	glDisableVertexAttribArray(0);

	/// Properly de-allocate all resources
	plane.cleanUp();
	for (int i = 0; i < gameTrees.size(); i++)
		gameTrees[i].cleanUp();
	character.cleanUp();

	/// Close GL context and any other GLFW resources
	glfwTerminate();

	return true;
}

// Function to load both vertex and fragment shaders
GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path) {
	/// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	/// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	/// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	/// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	/// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	/// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	/// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	/// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glLinkProgram(ProgramID);

	/// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	
	return ProgramID;
}

// Function to make sure the camera always lies on the plane
void checkMovement(vector<vector<int>> map, int size)
{
	if (cameraPos.y <= map[(int)cameraPos.x][(int)cameraPos.z] + camFactor) {
		cameraPos.y = map[(int)cameraPos.x][(int)cameraPos.z] + camFactor;
	}
	if (cameraPos.y > map[(int)cameraPos.x][(int)cameraPos.z] + camFactor) {
		cameraPos.y -= 0.5;
	}
}

// Main function of the program, where the game loop happens
int main() {
	/// Initialize all values
	initialize();
	cout << endl;
	srand(time(NULL));

	///Load the shaders
	world_program = loadShaders("world.vs", "world.fs");
	char_program = loadShaders("char.vs", "char.fs");
	
	/// Create a plane and a character
	Plane plane = Plane();
	Character character = Character();

	/// Set the starting position of both light and camera
	cameraPos = glm::vec3(plane.planeSize / 2, 50.0f, plane.planeSize / 2);
	lightPos = glm::vec3(plane.planeSize / 2, 100.0f, plane.planeSize / 2);

	/// Set the number of trees to appear on the map
	int numOfTrees = plane.planeSize * 5;

	/// Create a vector to avoid having different trees at the same location
	vector<vector<bool>> treePlaced;
	for (int i = 0; i < plane.heights.size(); i++) {
		treePlaced.push_back(vector<bool>());
		for (int j = 0; j < plane.heights[i].size(); j++) {
			treePlaced[i].push_back(false);
		}
	}

	/// Place trees on the map, show progress in %
	std::cout << "\nLoading trees.." << std::endl;
	int percent = (numOfTrees / 100), percentage = 0;
	for (int i = 0; i < numOfTrees; i++) {
		/// Randomize tree placement
		int spotX = rand() % (plane.planeSize);
		int spotZ = rand() % (plane.planeSize);

		/// Change height at the position where the tree was placed
		GLfloat h = plane.heights[spotX][spotZ];

		/// Add trees if all conditions are met
		if (!treePlaced[spotX][spotZ] && spotX < plane.planeSize - 1
			&& spotX > 0 && spotZ < plane.planeSize - 1 && spotZ > 0) {
			GLfloat size = 0.5;
			Tree tree = Tree(spotX + 0.25, h + 0.5, spotZ + 0.25, size);
			gameTrees.push_back(tree);
			plane.heights[spotX][spotZ] += tree.treeHeight - 3;
			treePlaced[spotX][spotZ] = true;
		}

		/// Display progress
		if (i % (percent * 5) == 0) {
			std::cout << percentage << "%.." << std::endl;
			percentage += 5;
		}
		if (i == numOfTrees - 1)
			std::cout << "100%.." << std::endl;
	}

	/// Show the game window when the loading is done
	glfwShowWindow(window);

	
	/// Game loop
	while (!glfwWindowShouldClose(window)) {
		/// Wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (lightPos.y >-20)
			glClearColor(0.4f * ((lightPos.y / 100) + 0.55), 0.65f * ((lightPos.y / 100) + 0.55), 0.8f * ((lightPos.y / 100) + 0.55), 1.0f);
		else
			glClearColor(0.4f * 0.35, 0.65f * 0.35, 0.8f * 0.35, 1.0f);


		/// make the light rotate around the plane like the sun
		lightPos.x = plane.planeSize/ 2 + cos(glfwGetTime() * 0.1f) * (GLfloat)plane.planeSize;
		lightPos.y = sin(glfwGetTime() * 0.1f) * (GLfloat)plane.planeSize;

		/// Used to get delatime each rendering call
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/// update other events like input handling
		glfwPollEvents();
		
		vector<vector<int>> map = plane.heights;
		int size = mapSize = plane.planeSize;

		charModel_matrix = glm::translate(charModel_matrix, glm::vec3(0.0) - charPos);
		if (camMode == 1)
			charPos = cameraPos + glm::vec3(0.0, -0.75, 0.0);

		/// Camera movement function, new values passed into lookAt function and set to the view matrix
		if (camMode == 1)
			checkMovement(map, size);
		if (camMode != 2)
			cameraMove(map, size);

		charModel_matrix = glm::translate(charModel_matrix, charPos);

		/// Set the view matrix for both model and camera
		charView_matrix = view_matrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		
		/// Set viewport for the window
		// Fullscreen
		// glViewport(0, 0, 1920, 1080);
		// Windowed
		glViewport(0, 0, 1280, 720);

		/// Set the projection matrix for both camera and model
		charProj_matrix = proj_matrix = glm::perspective(glm::radians(fov), (GLfloat)HEIGHT / (GLfloat)WIDTH, 0.1f, 1000.0f);

		/// Use the world shaders to render the world
		glUseProgram(world_program);

		/// Get uniform & attribute locations for the world
		view_matrix_id = glGetUniformLocation(world_program, "view_matrix");
		model_matrix_id = glGetUniformLocation(world_program, "model_matrix");
		proj_matrix_id = glGetUniformLocation(world_program, "proj_matrix");
		light_color_id = glGetUniformLocation(world_program, "light_color");
		light_pos_id = glGetUniformLocation(world_program, "light_pos");
		view_pos_id = glGetUniformLocation(world_program, "view_pos");
		glBindAttribLocation(world_program, 0, "in_Position");
		glBindAttribLocation(world_program, 1, "in_Color");
		
		/// Pass the values of the three matrices to the world shaders
		glUniformMatrix4fv(proj_matrix_id, 1, GL_FALSE, glm::value_ptr(proj_matrix));
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, glm::value_ptr(model_matrix));

		glUniform3f(light_color_id , 0.9, 0.9, 0.9);
		glUniform3f(light_pos_id, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(view_pos_id, cameraPos.x, cameraPos.y, cameraPos.z);
		
		/// Draw the trees
		for (int i = 0; i < gameTrees.size(); i++)
			gameTrees[i].drawTree();

		/// Draw the plane
		plane.drawPlane();
		
		/// Use the character shaders to render the world
		glUseProgram(char_program);

		/// Get uniform & attribute locations for the character
		charView_matrix_id = glGetUniformLocation(char_program, "view_matrix");
		charModel_matrix_id = glGetUniformLocation(char_program, "model_matrix");
		charProj_matrix_id = glGetUniformLocation(char_program, "proj_matrix");
		light_color_id = glGetUniformLocation(char_program, "light_color");
		light_pos_id = glGetUniformLocation(char_program, "light_pos");
		charView_pos_id = glGetUniformLocation(char_program, "view_pos");

		glBindAttribLocation(char_program, 0, "in_Position");
		glBindAttribLocation(char_program, 1, "in_Color");

		/// Pass the values of the three matrices to the character shaders
		glUniformMatrix4fv(charProj_matrix_id, 1, GL_FALSE, glm::value_ptr(charProj_matrix));
		glUniformMatrix4fv(charView_matrix_id, 1, GL_FALSE, glm::value_ptr(charView_matrix));
		glUniformMatrix4fv(charModel_matrix_id, 1, GL_FALSE, glm::value_ptr(charModel_matrix));
		glUniform3f(light_color_id, 0.9, 0.9, 0.9);
		glUniform3f(light_pos_id, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(charView_pos_id, cameraPos.x, cameraPos.y, cameraPos.z);

		/// Draw the character with his segway
		if (camMode != 1)
			character.drawChar();
		character.drawSegway();

		/// Put the stuff we've been drawing onto the display with double buffer
		glfwSwapBuffers(window);
	}

	/// Call the cleanup function when game loop ends
	cleanUp(plane, gameTrees, character);

	return 0;
}