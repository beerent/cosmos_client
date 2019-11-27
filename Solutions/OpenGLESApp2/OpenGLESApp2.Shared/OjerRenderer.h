#pragma once

#include <string>

#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>

#include <OpenGLES/ES2/gl.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif

#include "../../../Classes/Core/Net/IRestConnector.h"
#include "../../../Classes/Core/User/IUserMemory.h"

class ResourceLoader;
class OjerAppEngine2d;

class OjerRenderer
{
public:
	OjerRenderer();
	OjerRenderer(IRestConnector* restConnector, IUserMemory* userMemory);
    ~OjerRenderer();

    void Draw(double delta);
    void UpdateWindowSize(GLsizei width, GLsizei height);
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnKeyUp(const std::string& key);
	void OnKeyDown(const std::string& key);

private:
    GLuint mProgram;
    GLsizei mWindowWidth;
    GLsizei mWindowHeight;

    GLint mPositionAttribLocation;
    GLint mColorAttribLocation;

    GLint mModelUniformLocation;
    GLint mViewUniformLocation;
    GLint mProjUniformLocation;

    GLuint mVertexPositionBuffer;
    GLuint mVertexColorBuffer;
    GLuint mIndexBuffer;

	ResourceLoader* mLoader;
	OjerAppEngine2d* mEngine;
    int mDrawCount;
};
