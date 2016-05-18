#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <string>

class Display {
	private:
		GLint loadShader(char,const char*);
		const char *getShaderContent(std::string);
	public:
		Display(uint, float[], float[], float[]);
};

#endif