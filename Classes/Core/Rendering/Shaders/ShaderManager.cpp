//
//  ShaderManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 10/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Core/Rendering/Shaders/ShaderManager.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/StringManager/StringManager.h"
#include <streambuf>
#include <algorithm>


ShaderManager * ShaderManager::m_instance = NULL;

CONST_STRING_DEF(ShaderManager, TEXTURED_SHADER)
CONST_STRING_DEF(ShaderManager, TEXTURED_LIT_SHADER)
CONST_STRING_DEF(ShaderManager, TEXTURED_LIT_EMISIVE_SHADER)
CONST_STRING_DEF(ShaderManager, FONT_SHADER)
CONST_STRING_DEF(ShaderManager, LINE_SHADER)

ShaderManager::ShaderManager()
{    
	if (m_instance == NULL)
	{
		m_instance = this;
	}
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::reloadShaders()
{
	unloadShaders();
	readProgramDefinitions();
}

void ShaderManager::unloadShaders()
{
	for (StringIDToGluint::iterator it = m_shaderProgramsDT.begin();
		it != m_shaderProgramsDT.end(); ++it)
	{
		glDeleteProgram(it->second);
	}

	m_shaderProgramsDT.clear();
	m_vertexShadersDT.clear();
	m_fragmentShaderDT.clear();
}

GLuint ShaderManager::getProgram(STRING_ID name)
{
	ShaderManager *sm = getInstance();
	StringIDToGluint::const_iterator it = sm->m_shaderProgramsDT.find(name);
	if (it == sm->m_shaderProgramsDT.end())
	{
		Logger::Error("Could not find requested shader " + *StringManager::getStringFromID(name));
	}

	return it->second;
}

void ShaderManager::init(IResourceLoader* resourceLoader)
{
	mResourceLoader = resourceLoader;
	readProgramDefinitions();
}

ShaderManager* ShaderManager::getInstance()
{
	return m_instance;
}

GLuint ShaderManager::createShader(const char* source, GLuint type, GLint length)
{
    GLuint res = glCreateShader(type);
    Renderer::validate();
    
	if (length == 0)
	{
		glShaderSource(res, 1, &source, NULL);
	}
	else
	{
		glShaderSource(res, 1, &source, &length);
	}

    Renderer::validate();
    
    glCompileShader(res);
    Renderer::validate();
    
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
   
    if (compile_ok == GL_FALSE) 
    {

		printError(res);
        Renderer::validate();
        glDeleteShader(res);
        return 0;
    }
        
    return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void ShaderManager::printError(GLuint object)
{
    GLint log_length = 0;
    if (glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else if (glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else {
        fprintf(stderr, "printlog: Not a shader or a program\n");
        return;
    }
    
    char* log = (char*)malloc(log_length);
    
    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);
    
    fprintf(stderr, "%s", log);

	Logger::Error(std::string("Shader Failed To Compile: ") + std::string(log));
    free(log);
}

namespace
{
	bool readDataFromFile(const std::string& path, PropertyCollection& propertyCollection)
	{
		std::ifstream file;
		file.open(path.c_str());
		char buffer[256];

		if (file.good())
		{
			while (file.good())
			{
				file.getline(buffer, 256);
				propertyCollection.addPropertyFromStream(buffer);
			}
			file.close();
			return true;
		}
		else
		{
			file.close();
			Logger::Error(std::string("Failed to load file ") + path);
			return false;
		}
	}
}

void ShaderManager::readProgramDefinitions()
{
	//This maybe switchable
	std::string shaderCollection("ShaderCollection");
    std::string nameLowerStr = shaderCollection;
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
	std::string path = mResourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() + ".txt";
	PropertyCollection propertyCollection;
	if (readDataFromFile(path, propertyCollection))
	{
		const int propertyCount = propertyCollection.getPropertyCount();
		for (int i = 0; i < propertyCount; ++i)
		{
			const PropertyCollection::Property* property = propertyCollection.getPropertyAtIndex(i);
			readInProgram(property->m_name);
		}
	}
}

GLuint ShaderManager::GetShader(GLuint type, STRING_ID name)
{
	StringIDToGluint* shaders = NULL;
	if (type == GL_VERTEX_SHADER)
	{
		shaders = &m_vertexShadersDT;
	}
	else
	{
		shaders = &m_fragmentShaderDT;
	}

	StringIDToGluint::const_iterator it = shaders->find(name);
	if (it == shaders->end())
	{
		GLuint shader = readInShader(name, type);
		(*shaders)[name] = shader;
		return shader;
	}

	return it->second;
}

void ShaderManager::readInProgram(STRING_ID name)
{
    std::string nameLowerStr = *ID_TO_STRING(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
	std::string path = mResourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() + ".txt";
	PropertyCollection propertyCollection;
	if (readDataFromFile(path, propertyCollection))
	{
		STRING_ID vertex = StringManager::UNDEFINED;
		propertyCollection.getValueForProperty(STRING_TO_ID("VERTEX"), vertex);
		GLuint vertexID = GetShader(GL_VERTEX_SHADER, vertex);

		STRING_ID fragment = StringManager::UNDEFINED;
		propertyCollection.getValueForProperty(STRING_TO_ID("FRAGMENT"), fragment);
		GLuint fragmentID = GetShader(GL_FRAGMENT_SHADER, fragment);

		GLuint program = glCreateProgram();
		Renderer::validate();

		glAttachShader(program, fragmentID);
		Renderer::validate();

		glAttachShader(program, vertexID);
		Renderer::validate();

		GLint link_ok = GL_FALSE;
		glLinkProgram(program);
		Renderer::validate();

		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		Renderer::validate();

		if (!link_ok) 
		{
			std::cout << "glLinkProgram:" << stderr << std::endl;
			Logger::Error("Shader linking failed" + *StringManager::getStringFromID(name));
			return;
		}

		m_shaderProgramsDT[name] = program;
	}
}

GLuint ShaderManager::readInShader(STRING_ID name, GLuint shaderType)
{
    std::string nameLowerStr = *StringManager::getStringFromID(name);
    std::transform(nameLowerStr.begin(), nameLowerStr.end(), nameLowerStr.begin(), ::tolower);
	std::string path = mResourceLoader->getResourcePath() + "/" + nameLowerStr.c_str() + ".txt";

	std::ifstream file;
	file.open(path.c_str());
	if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();		
		const char * shaderPtr = str.c_str();
		return createShader(shaderPtr, shaderType, str.length());
	}
	else
	{
		file.close();
		Logger::Warning(Logger::LOADER, std::string("Failed to load file ") + path);
	}

	return 0;
}
