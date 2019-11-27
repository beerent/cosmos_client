//
// This file is used by the template to render a basic scene using GL.
//
#include "pch.h"

#include "OjerRenderer.h"
#include "MathHelper.h"
#include "OjerAppEngine2d.h"
#include "ResourceLoader.h"
// These are used by the shader compilation methods.
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>


GLuint CompileShader(GLenum type, const std::string &source)
{
    GLuint shader = glCreateShader(type);

    const char *sourceArray[1] = { source.c_str() };
    glShaderSource(shader, 1, sourceArray, NULL);
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == 0)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<GLchar> infoLog(infoLogLength);
        glGetShaderInfoLog(shader, (GLsizei)infoLog.size(), NULL, infoLog.data());

        std::string errorMessage = std::string("Shader compilation failed: ");
        errorMessage += std::string(infoLog.begin(), infoLog.end()); 

        throw std::runtime_error(errorMessage.c_str());
    }

    return shader;
}

GLuint CompileProgram(const std::string &vsSource, const std::string &fsSource)
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        throw std::runtime_error("Program creation failed");
    }

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

    if (vs == 0 || fs == 0)
    {
        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vs);
    glDeleteShader(vs);

    glAttachShader(program, fs);
    glDeleteShader(fs);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == 0)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::vector<GLchar> infoLog(infoLogLength);
        glGetProgramInfoLog(program, (GLsizei)infoLog.size(), NULL, infoLog.data());

        std::string errorMessage = std::string("Program link failed: ");
        errorMessage += std::string(infoLog.begin(), infoLog.end()); 

        throw std::runtime_error(errorMessage.c_str());
    }

    return program;
}

OjerRenderer::OjerRenderer(IRestConnector* restConnector, IUserMemory* userMemory) :
    mWindowWidth(0),
    mWindowHeight(0),
    mDrawCount(0)
{
	mEngine = new OjerAppEngine2d();
	mEngine->SetRestConnector(restConnector);
	mEngine->SetUserMemory(userMemory);

	mLoader = new ResourceLoader();
}

OjerRenderer::OjerRenderer() : 
	mWindowWidth(0), 
	mWindowHeight(0), 
	mDrawCount(0)
{
	mEngine = new OjerAppEngine2d();
	mLoader = new ResourceLoader();
}

OjerRenderer::~OjerRenderer()
{
	delete mEngine;
	delete mLoader;
}


void OjerRenderer::Draw(double delta)
{
	static bool init = false;
	if (init == false)
	{
		mEngine->Initialize(mWindowWidth, mWindowHeight, mLoader, 1.0f);
		mEngine->OnRotate(DeviceOrientation::DeviceOrientationPortrait);
		init = true;
	}
	
	mEngine->Render();
	mEngine->OnDeltaTime(delta / 1000.0f);
}

void OjerRenderer::UpdateWindowSize(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    mWindowWidth = width;
    mWindowHeight = height;
	mEngine->SetDimentions(mWindowWidth, mWindowHeight);
}

void OjerRenderer::OnMouseDown(int x, int y)
{
	mEngine->OnFingerDown(ivec2(x, y));
}

void OjerRenderer::OnMouseUp(int x, int y)
{
	mEngine->OnFingerUp(ivec2(0, 0),ivec2(x, y));
}

void OjerRenderer::OnKeyDown(const std::string& key) {
	if (key == "Back") {
		mEngine->GetKeyboardManager()->OnDeletePressed();
	} else if (key == "Enter") {
		mEngine->GetKeyboardManager()->OnEnterPressed();
	} else {
		mEngine->GetKeyboardManager()->OnCharacterPressed(key[0]);
	}
	//mEngine->OnKeyDown(key);
}

void OjerRenderer::OnKeyUp(const std::string& key) {
	//mEngine->OnKeyUp(key);
}
