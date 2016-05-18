#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "display.h"

Display::Display(uint verticesNum, float vertices[], float minmax[], float normals[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    const int width = 800;
    const int height = 600;

    SDL_Window *window = SDL_CreateWindow(
        "Model viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) std::cerr<<"Problem when creating window.";

    SDL_GLContext gl = SDL_GL_CreateContext(window);

    GLenum status = glewInit();

    SDL_Event e;
	bool quit = false;

	glEnable(GL_DEPTH_TEST);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesNum*sizeof(float)*3, vertices, GL_STATIC_DRAW);

	GLuint normalsBuffer;
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesNum*sizeof(float)*3, normals, GL_STATIC_DRAW);

	//SHADERS
	GLint shaderStatus;

	//VERTEX SHADER
    const GLchar *vertexShaderSrc = getShaderContent("shaders/shader.vert");
    GLuint vertexShader = loadShader('v', vertexShaderSrc);

	//FRAGMENT SHADER
    const GLchar *fragmentShaderSrc = getShaderContent("shaders/shader.frag");
	GLuint fragmentShader = loadShader('f', fragmentShaderSrc);

	//SHADER PROGRAM
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//POSITION
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	//NORMALS
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);

	GLint normalAttrib = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttrib);

    //MODEL
    glm::mat4 model;
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	
	//VIEW
	float heightOffset = 5.0f;
	float distanceOffset = 25.0f;

	glm::mat4 view = glm::lookAt(
	    glm::vec3((minmax[0]+minmax[1])/2, ((minmax[2]+minmax[3])/2)+distanceOffset, ((minmax[4]+minmax[5])/2)+heightOffset),
	    glm::vec3((minmax[0]+minmax[1])/2, (minmax[2]+minmax[3])/2, (minmax[4]+minmax[5])/2),
	    glm::vec3(0.0f, 0.0f, 1.0f)
	);
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	//PROJECTION
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 1000.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	bool rotate = true;
	float angle = 0.05f;

	while (!quit){
		while (SDL_PollEvent(&e)){
			switch (e.key.keysym.sym) {
			    case SDLK_ESCAPE: 
			        quit = true;
			        break;
			    case SDLK_p:
			    	rotate = false;
			    	break;
			    case SDLK_o:
			    	rotate = true;
			    	break;
			    case SDLK_KP_PLUS:
			    	angle += 0.01f;
			    	break;
			    case SDLK_KP_MINUS:
			    	angle -= 0.01f;
			    	break;
			}
			if (e.type == SDL_QUIT){
				quit = true;
			}
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (rotate){
	        glm::mat4 centerAndRotate = glm::translate(glm::mat4(1.0f), glm::vec3((minmax[0]+minmax[1])/2, (minmax[2]+minmax[3])/2, 0.0f));
	        centerAndRotate = glm::rotate(centerAndRotate, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	        centerAndRotate = glm::translate(centerAndRotate, glm::vec3(-((minmax[0]+minmax[1])/2), -((minmax[2]+minmax[3])/2), 0.0f));

        	model *= centerAndRotate;
        }

    	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, verticesNum);

		SDL_GL_SwapWindow(window);
	}

	delete[] vertexShaderSrc;
	delete[] fragmentShaderSrc;

	SDL_GL_DeleteContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

GLint Display::loadShader(char type, const char* shaderContent) {
	GLuint shader;
	GLint shaderStatus;

	if (type=='v') {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type=='f') {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(shader, 1, &shaderContent, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		std::cout<<buffer<<std::endl;
		exit(1);
	}

	return shader;
}

const char *Display::getShaderContent(std::string source) {
	std::ifstream file(source.c_str());
	std::string line;

	std::string strContent;

	while (std::getline(file, line))
	{
		strContent += line;
		if(line[0] == '#') {
			strContent+='\n';
		}
	}  

	char* content = new char[strContent.length() + 1];
	strcpy(content, strContent.c_str());
	content = const_cast<char *> (content);

	return content;
}