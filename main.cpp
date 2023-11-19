/*
Type your name and student ID here
	- Name:Xu Chenyan
	- Student ID:1155173847
*/

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


GLint programID;
GLuint vaoID[6];
float x_delta = 0.1f;
int x_press_num = 0;
float z_delta =0.1f;
int z_press_num = 0;
int grass_press_number=1;
int rotate_number=0;
float cameraSpeed =0.3;
int s_press_num=0;

glm::vec3 cameraPos = glm::vec3(0.0f,2.0f,5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,-2.0f,-5.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,+0.0f);

float lastX=400;
float lastY=300;
bool firstMouse = true;
float pitch =0.0f;
float yaw =-90.0f;


void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void sendDataToOpenGL() {
    // TODO:
    // create 3D objects and/or 2D objects and/or lines (points) here and bind to VAOs & VBOs
    
    const GLfloat ground[]{

        -1.0f,-0.8f,+1.0f,0.494f,0.922f,0.478f,
        +1.0f,-0.8f,+1.0f,0.494f,0.922f,0.478f,
        +1.0f,-0.8f,-1.0f,0.494f,0.922f,0.478f,
        -1.0f,-0.8f,+1.0f,0.494f,0.922f,0.478f,
        +1.0f,-0.8f,-1.0f,0.494f,0.922f,0.478f,
        -1.0f,-0.8f,-1.0f,0.494f,0.922f,0.478f,

         };
    const GLfloat sheep[]{
        //body
        +0.0f,-0.6f,+0.5f,0.96f,0.96f,0.96f,
        +0.8f,-0.6f,+0.5f,0.96f,0.96f,0.96f,
        +0.8f,-0.6f,0.0f,0.96f,0.96f,0.96f,
        +0.0f,-0.6f,0.0f,0.96f,0.96f,0.96f,
        +0.0f,+0.6f,+0.5f,0.96f,0.96f,0.96f,
        +0.8f,+0.6f,+0.5f,0.96f,0.96f,0.96f,
        +0.8f,+0.6f,0.0f,0.96f,0.96f,0.96f,
        +0.0f,+0.6f,0.0f,0.96f,0.96f,0.96f,
        
        //left front leg
        +0.0f,-0.8f,+0.5f,0.45f,0.30f,0.05f,
        +0.2f,-0.8f,+0.5f,0.45f,0.30f,0.05f,
        +0.2f,-0.8f,+0.3f,0.45f,0.30f,0.05f,
        0.0f,-0.8f,+0.3f,0.45f,0.30f,0.05f,
        +0.0f,-0.6f,+0.5f,0.45f,0.30f,0.05f,
        +0.2f,-0.6f,+0.5f,0.45f,0.30f,0.05f,
        +0.2f,-0.6f,+0.3f,0.45f,0.30f,0.05f,
        0.0f,-0.6f,+0.3f,0.45f,0.30f,0.05f,
        
        // right front leg
        +0.0f,-0.8f,+0.2f,0.45f,0.30f,0.05f,
        +0.2f,-0.8f,+0.2f,0.45f,0.30f,0.05f,
        +0.2f,-0.8f,+0.0f,0.45f,0.30f,0.05f,
        0.0f,-0.8f,+0.0f,0.45f,0.30f,0.05f,
        +0.0f,-0.6f,+0.2f,0.45f,0.30f,0.05f,
        +0.2f,-0.6f,+0.2f,0.45f,0.30f,0.05f,
        +0.2f,-0.6f,+0.0f,0.45f,0.30f,0.05f,
        0.0f,-0.6f,+0.0f,0.45f,0.30f,0.05f,
        
        //left back leg
        +0.6f,-0.8f,+0.5f,0.45f,0.30f,0.05f,
        +0.8f,-0.8f,+0.5f,0.45f,0.30f,0.05f,
        +0.8f,-0.8f,+0.3f,0.45f,0.30f,0.05f,
        +0.6f,-0.8f,+0.3f,0.45f,0.30f,0.05f,
        +0.6f,-0.6f,+0.5f,0.45f,0.30f,0.05f,
        +0.8f,-0.6f,+0.5f,0.45f,0.30f,0.05f,
        +0.8f,-0.6f,+0.3f,0.45f,0.30f,0.05f,
        +0.6f,-0.6f,+0.3f,0.45f,0.30f,0.05f,
        //right back leg
        +0.6f,-0.8f,+0.2f,0.45f,0.30f,0.05f,
        +0.8f,-0.8f,+0.2f,0.45f,0.30f,0.05f,
        +0.8f,-0.8f,+0.0f,0.45f,0.30f,0.05f,
        +0.6f,-0.8f,+0.0f,0.45f,0.30f,0.05f,
        +0.6f,-0.6f,+0.2f,0.45f,0.30f,0.05f,
        +0.8f,-0.6f,+0.2f,0.45f,0.30f,0.05f,
        +0.8f,-0.6f,+0.0f,0.45f,0.30f,0.05f,
        +0.6f,-0.6f,+0.0f,0.45f,0.30f,0.05f,
        //face
        -0.1f,-0.2f,+0.35f,0.45f,0.30f,0.05f,
        -0.0001f,-0.2f,+0.35f,0.45f,0.30f,0.05f,
        -0.0001f,-0.2f,+0.15f,0.45f,0.30f,0.05f,
        -0.1f,-0.2f,+0.15f,0.45f,0.30f,0.05f,
        -0.1f,+0.1999f,+0.35f,0.45f,0.30f,0.05f,
        -0.0001f,+0.1999f,+0.35f,0.45f,0.30f,0.05f,
        -0.0001f,+0.1999f,+0.15f,0.45f,0.30f,0.05f,
        -0.1f,+0.1999f,+0.15f,0.45f,0.30f,0.05f,
        //head
        -0.1f,+0.2f,+0.5f,0.96f,0.96f,0.96f,
        0.0f,+0.2f,+0.5f,0.96f,0.96f,0.96f,
        0.0f,+0.2f,+0.0f,0.96f,0.96f,0.96f,
        -0.1f,+0.2f,+0.0f,0.96f,0.96f,0.96f,
        -0.1f,+0.5f,+0.5f,0.96f,0.96f,0.96f,
        0.0f,+0.5f,+0.5f,0.96f,0.96f,0.96f,
        0.0f,+0.5f,+0.0f,0.96f,0.96f,0.96f,
        -0.1f,+0.5f,+0.0f,0.96f,0.96f,0.96f,
        
    };
    GLushort sheepindice[]{
      0,1,2,
      0,2,3,
      0,1,5,
      0,5,4,
      0,4,7,
      0,7,3,
      6,1,2,
      6,5,1,
      6,3,2,
      6,7,3,
      6,4,5,
      6,7,4,
    
      8,9,10,
      8,10,11,
      8,9,13,
      8,13,12,
      8,12,11,
      8,15,11,
      14,9,10,
      14,13,9,
      14,11,10,
      14,15,11,
      14,12,13,
      14,15,12,
        
        16, 17, 18,
        16, 18, 19,
        16, 17, 21,
        16, 21, 20,
        16, 20, 19,
        16, 23, 19,
        22, 17, 18,
        22, 21, 17,
        22, 19, 18,
        22, 23, 19,
        22, 20, 21,
        22, 23, 20,
        
        24, 25, 26,
        24, 26, 27,
        24, 25, 29,
        24, 29, 28,
        24, 28, 27,
        24, 31, 27,
        30, 25, 26,
        30, 29, 25,
        30, 27, 26,
        30, 31, 27,
        30, 28, 29,
        30, 31, 28,
      
        32, 33, 34,
        32, 34, 35,
        32, 33, 37,
        32, 37, 36,
        32, 36, 35,
        32, 39, 35,
        38, 33, 34,
        38, 37, 33,
        38, 35, 34,
        38, 39, 35,
        38, 36, 37,
        38, 39, 36,
        
        40, 41, 42,
        40, 42, 43,
        40, 41, 45,
        40, 45, 44,
        40, 44, 43,
        40, 47, 43,
        46, 41, 42,
        46, 45, 41,
        46, 43, 42,
        46, 47, 43,
        46, 44, 45,
        46, 47, 44,
        
        48,49,50,
        48,50,51,
        48,49,53,
        48,53,52,
        48,52,51,
        48,55,51,
        54,49,50,
        54,53,49,
        54,51,50,
        54,55,51,
        54,52,53,
        54,55,52,
    };
    
    const GLfloat grass[]{
      -0.9f,-0.8f,-0.9f,0.08f, 0.41f, 0.27f,
      -0.8f,-0.8f,-0.9f,0.08f, 0.41f, 0.27f,
      -0.8f,-0.8f,-0.8f,0.08f, 0.41f, 0.27f,
      -0.9f,-0.8f,-0.8f,0.08f, 0.41f, 0.27f,
      -0.85f,-0.4f,-0.85f,0.08f, 0.41f, 0.27f,
        
        -0.8f,-0.8f,-0.9f,0.08f, 0.41f, 0.27f,
        -0.7f,-0.8f,-0.9f,0.08f, 0.41f, 0.27f,
        -0.7f,-0.8f,-0.8f,0.08f, 0.41f, 0.27f,
        -0.8f,-0.8f,-0.8f,0.08f, 0.41f, 0.27f,
        -0.75f,-0.4f,-0.85f,0.08f, 0.41f, 0.27f,
        
        -0.7f,-0.8f,-0.9f,0.08f, 0.41f, 0.27f,
        -0.6f,-0.8f,-0.9f,0.08f, 0.41f, 0.27f,
        -0.6f,-0.8f,-0.8f,0.08f, 0.41f, 0.27f,
        -0.7f,-0.8f,-0.8f,0.08f, 0.41f, 0.27f,
        -0.65f,-0.4f,-0.85f,0.08f, 0.41f, 0.27f,
    
        
    };
    GLushort grassindice[]{
      0,1,2,
      0,2,3,
      4,0,1,
      4,1,2,
      4,2,3,
      4,1,0,
    
      5,6,7,
      5,7,8,
      9,5,6,
      9,6,7,
      9,7,8,
      9,6,5,
      
      10,11,12,
      10,12,13,
      14,10,11,
      14,11,12,
      14,12,13,
      14,11,10,
    
    };
    const GLfloat grass1[]{
       +0.9f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
      +0.8f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
      +0.8f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
      +0.9f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
      +0.85f,-0.4f,+0.85f,0.08f, 0.41f, 0.27f,
        
        +0.8f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        +0.7f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        +0.7f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        +0.8f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        +0.75f,-0.4f,+0.85f,0.08f, 0.41f, 0.27f,
        
        +0.7f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        +0.6f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        +0.6f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        +0.7f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        +0.65f,-0.4f,+0.85f,0.08f, 0.41f, 0.27f,
    
        
    };
    GLushort grassindice1[]{
      0,1,2,
      0,2,3,
      4,0,1,
      4,1,2,
      4,2,3,
      4,1,0,
    
      5,6,7,
      5,7,8,
      9,5,6,
      9,6,7,
      9,7,8,
      9,6,5,
      
      10,11,12,
      10,12,13,
      14,10,11,
      14,11,12,
      14,12,13,
      14,11,10,
    
    };
    const GLfloat grass2[]{
      -0.9f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
      -0.8f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
      -0.8f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
      -0.9f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
      -0.85f,-0.4f,+0.85f,0.08f, 0.41f, 0.27f,
        
        -0.8f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        -0.7f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        -0.7f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        -0.8f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        -0.75f,-0.4f,+0.85f,0.08f, 0.41f, 0.27f,
        
        -0.7f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        -0.6f,-0.8f,+0.9f,0.08f, 0.41f, 0.27f,
        -0.6f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        -0.7f,-0.8f,+0.8f,0.08f, 0.41f, 0.27f,
        -0.65f,-0.4f,+0.85f,0.08f, 0.41f, 0.27f,
    
        
    };
    GLushort grassindice2[]{
      0,1,2,
      0,2,3,
      4,0,1,
      4,1,2,
      4,2,3,
      4,1,0,
    
      5,6,7,
      5,7,8,
      9,5,6,
      9,6,7,
      9,7,8,
      9,6,5,
      
      10,11,12,
      10,12,13,
      14,10,11,
      14,11,12,
      14,12,13,
      14,11,10,
    
    };
    
    const GLfloat arrow[]{
        -0.5f,2.5f,0.0f,
        +1.0f,0.0f,0.0f,
        +0.5f,2.5f,0.0f,
        +0.0f,0.0f,1.0f,
        +0.0f,+1.5f,+0.0f,
        +0.0f,+1.0f,+0.0f,
    };
    
    glGenVertexArrays(1, &vaoID[0]);
    glBindVertexArray(vaoID[0]);  //first VAO
    
    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    
    glGenVertexArrays(1,&vaoID[1]);
    glBindVertexArray(vaoID[1]);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sheep), sheep, GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    GLuint indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sheepindice),sheepindice, GL_STATIC_DRAW);
    
    glGenVertexArrays(1,&vaoID[2]);
    glBindVertexArray(vaoID[2]);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass), grass, GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grassindice),grassindice, GL_STATIC_DRAW);
    
    glGenVertexArrays(1,&vaoID[3]);
    glBindVertexArray(vaoID[3]);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass1), grass1, GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grassindice1),grassindice1, GL_STATIC_DRAW);
    
    glGenVertexArrays(1,&vaoID[4]);
    glBindVertexArray(vaoID[4]);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grass2), grass2, GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grassindice2),grassindice2, GL_STATIC_DRAW);
    
    glGenVertexArrays(1,&vaoID[5]);
    glBindVertexArray(vaoID[5]);
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrow), arrow, GL_STATIC_DRAW);
    
    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
    
    
    
}

void cooridinatetransform (int index){
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix =glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    if (index==0){
        modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix,glm::vec3(2,2,2));
    }
    if (index==1){
        modelMatrix = glm::translate(modelMatrix,glm::vec3(x_delta * x_press_num, 0.0f, 1.2+z_delta * z_press_num));
        modelMatrix = glm::translate(modelMatrix,glm::vec3(0.3f,0.0f,0.25f));
        modelMatrix= glm::rotate(modelMatrix,glm::radians(60.0f+rotate_number*10),glm::vec3(0.0f,1.0f,0.0f));
        modelMatrix = glm::translate(modelMatrix,glm::vec3(-0.3f,0.0f,-0.25f));
        modelMatrix = glm::scale(modelMatrix,glm::vec3(0.8+0.05*s_press_num,0.4+0.05*s_press_num,0.6+0.05*s_press_num));
    }
    if (index==2||index==3||index==4){
        modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 1.2f));
        modelMatrix= glm::rotate(modelMatrix,glm::radians(50.0f),glm::vec3(0.0f,1.0f,0.0f));
        modelMatrix = glm::scale(modelMatrix,glm::vec3(0.8,0.4+0.02*grass_press_number,0.6));
    }
    if (index==5){
        modelMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix,glm::vec3(0.5,0.5,0.5));
    }
    viewMatrix = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
    projectionMatrix=glm::perspective(glm::radians(45.0f),1.0f,1.0f,100.0f);
    
    GLint modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    GLint viewMatrixUniformLocation = glGetUniformLocation(programID, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniformLocation, 1,GL_FALSE, &viewMatrix[0][0]);
    GLint projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
};
void paintGL(void) {
    // always run
    // TODO:
    // render your objects and control the transformation here
    glClearColor(0.477f, 0.796f, 0.922f, 1.0f);  //specify the background color
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    cooridinatetransform(0);
    glBindVertexArray(vaoID[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    cooridinatetransform(1);
    glBindVertexArray(vaoID[1]);
    glDrawElements(GL_TRIANGLES,252*sizeof(float),GL_UNSIGNED_SHORT,0);
    
    cooridinatetransform(2);
    glBindVertexArray(vaoID[2]);
    glDrawElements(GL_TRIANGLES, 60*sizeof(float),GL_UNSIGNED_SHORT,0 );
   
    cooridinatetransform(3);
    glBindVertexArray(vaoID[3]);
    glDrawElements(GL_TRIANGLES, 60*sizeof(float),GL_UNSIGNED_SHORT,0 );
    
    cooridinatetransform(4);
    glBindVertexArray(vaoID[4]);
    glDrawElements(GL_TRIANGLES, 60*sizeof(float),GL_UNSIGNED_SHORT,0 );
    
    cooridinatetransform(5);
    glBindVertexArray(vaoID[5]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
 
    if (key == GLFW_KEY_A && action == GLFW_PRESS&&x_press_num>=-12) {
        x_press_num -= 1;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS&&x_press_num<=7) {
        x_press_num += 1;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS&&z_press_num>=-6) {
        z_press_num -= 1;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS&&z_press_num<=13) {
        z_press_num += 1;
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        rotate_number += 1;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        rotate_number -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
          cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
          cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
          cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
          cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos,double ypos){
    if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
      
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 70.0f)
            pitch = 70.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    
}
void mouse_button_callback(GLFWwindow* window,int button,int action,int mods){
    if (button==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS&&grass_press_number<=10)
       grass_press_number++;
    if (button==GLFW_MOUSE_BUTTON_RIGHT&&action==GLFW_PRESS&&grass_press_number>=-10)
        grass_press_number--;
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
    glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;
    
	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* do not allow resizing */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback); // TODO
	// TODO: mouse callback, etc.
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
