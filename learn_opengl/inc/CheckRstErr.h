#pragma once 

#include <glad/glad.h>
#include <SDL3/SDL.h>

#ifdef _DEBUG
	#define GL_STRINGIFY(x) #x
	#define GL_CALL(func) func;checkRstErr(GL_STRINGIFY(func), __FILE__, __LINE__);
#else
	#define GL_CALL(func) func;
#endif

static void checkRstErr(const char* szFuncName, const char* szFileName, int iLine)
{
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) 
	{
		SDL_Log("exec opengl function error,%u,%s,%s,%d", errorCode, szFuncName, szFileName, iLine);
	}
}