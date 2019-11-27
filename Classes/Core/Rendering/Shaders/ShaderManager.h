//
//  ShaderManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 10/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Projectios1_ShaderManager_h
#define Projectios1_ShaderManager_h

#ifdef __APPLE__

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif

#include <map>
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/Shaders/ShaderManager.h"
#include "Interface.hpp"

class ShaderManager
{

public:
    CONST_STRING_DEC(TEXTURED_SHADER)
    CONST_STRING_DEC(TEXTURED_LIT_SHADER)
    CONST_STRING_DEC(TEXTURED_LIT_EMISIVE_SHADER)
	CONST_STRING_DEC(FONT_SHADER)
	CONST_STRING_DEC(LINE_SHADER)
       
	static GLuint getProgram(STRING_ID name);
	ShaderManager();
	~ShaderManager();

	void init(IResourceLoader* resourceLoader);
	static ShaderManager *getInstance();
    
	void reloadShaders();

private:
    static ShaderManager * m_instance;
    static GLuint createShader(const char* source, GLuint type, GLint length = 0);
    static void printError(GLuint object);

	IResourceLoader* mResourceLoader;

	typedef std::map<STRING_ID, GLuint> StringIDToGluint;
	StringIDToGluint m_vertexShadersDT; 
	StringIDToGluint m_fragmentShaderDT;
	StringIDToGluint m_shaderProgramsDT;

	GLuint GetShader(GLuint type, STRING_ID name);
	void readProgramDefinitions();
	void readInProgram(STRING_ID name);
	GLuint readInShader(STRING_ID name, GLuint shaderType);
	void unloadShaders();
};


#endif
