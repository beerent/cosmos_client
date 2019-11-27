//
//  Renderer.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Core/GUI/Components/UIComponent.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/Renderables/RenderableObj.h"
#include "Core/Rendering/IRenderTarget.h"
#include "glm/gtx/quaternion.hpp"
#include "Core/Debug/Logger/Logger.h"
#include "IEngine.h"

Renderer::Renderer()
{
    m_isInitialized = false;
    m_viewPortPosition.x = 0;
    m_viewPortPosition.y = 0;
    m_framebuffer = 0;
    m_renderbuffer = 0;
    glGenRenderbuffers(1, &m_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    mRenderGroupMap[RenderableObj::TRANSPARENT_STR]    = &mTransparentRenderables;
    mRenderGroupMap[RenderableObj::NORMAL]         = &mNormalRenderables;
    mRenderGroupMap[RenderableObj::BACKGROUND]     = &mBackGroundRenderables;

    mNormalRenderables.reserve(1000);
    mTransparentRenderables.reserve(100);
}

void Renderer::SetViewPortSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void Renderer::Initialize()
{

	glViewport(0, 0, m_width, m_height);
#ifdef __APPLE__
    GLuint m_depthRenderbuffer;
    glGenRenderbuffers(1, &m_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH_COMPONENT16,
                          m_width,
                          m_height);
    
    Renderer::validate();
    glGenFramebuffers(1, &m_framebuffer);
    Renderer::validate();
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    Renderer::validate();

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER,
                              m_renderbuffer);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              m_depthRenderbuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);

    Renderer::validate();
    
    glViewport(0,0, m_width, m_height);

	Renderer::validate();

	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Renderer::validate();
#endif
}

void Renderer::addRenderable(IRenderTarget* renderTarget, RenderableObj* renderable)
{
    if(mModelViewProjectionMatrixStack.size() > 0)
    {
        addRenderable(renderTarget, renderable, mModelViewProjectionMatrixStack.top(), m_transformations.top());
    }
    else
    {
        addRenderable(renderTarget, renderable, getViewProjectionMatrix(), glm::mat4());
    }
}

void Renderer::addRenderable(IRenderTarget* renderTarget, RenderableObj* renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
    RenderJobVector * jobs = mRenderGroupMap[renderable->getRenderGroup()];
    RenderJob renderJob;
    renderJob.mRenderable = renderable;
    renderJob.mRenderTarget = renderTarget;
    renderJob.mModelViewProjectionMatrix = modelViewProjectionMatrix;
    renderJob.mModelView = modelView;
    jobs->push_back(renderJob);
}

void Renderer::onBeginRender()
{
    mTransparentRenderables.clear();
    mNormalRenderables.clear();
    mBackGroundRenderables.clear();
    /* Clear the background as white */
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    Renderer::validate();
    glm::mat4 view;
    
    if(m_pActiveCamera != NULL)
    {
        view = m_pActiveCamera->getTransformation();
        view = glm::inverse(view);
    }
    else
    {
        glm::vec3 eye(0.0,2.0,0.0);
        glm::vec3 center(0.0,0.0,-4.0);
        glm::vec3 up(0.0,1.0,0.0);
        view = glm::lookAt(eye,center,up);
    }
    
    float currentAngle = IEngine::getEngine()->getCurrentAngle();
    glm::quat rotation = glm::angleAxis<float>(currentAngle,0.0,0.0,1.0);
    glm::mat4x4 viewRotation = glm::toMat4(rotation);
    
	//this is required to display the background planets correctly. This is similar to the function below
	//and apple's function in ViewController.mm. 
#ifdef __APPLE__
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*m_height/m_width, 0.5f, 3000.0f);
#else
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*m_width / m_height, 0.5f, 3000.0f);
#endif

	m_viewProjectionMatrix  = projection * viewRotation * view;
    
    std::vector<IRenderTarget *>::iterator it = m_renderTargets.begin();
    
    while(it != m_renderTargets.end())
    {
        (*it)->onBeginRender(*this);
        it++;
    }
    
    RenderJobVector::iterator jobIt = mBackGroundRenderables.begin();
    
    while(jobIt != mBackGroundRenderables.end())
    {
        RenderJob &job = (*jobIt);
        job.mRenderTarget->onRender(*this, *job.mRenderable, job.mModelViewProjectionMatrix, job.mModelView);
        jobIt++;
    }
    
    glEnable(GL_DEPTH_TEST);

    jobIt = mNormalRenderables.begin();
    
    while(jobIt != mNormalRenderables.end())
    {
        RenderJob &job = (*jobIt);
        job.mRenderTarget->onRender(*this, *job.mRenderable, job.mModelViewProjectionMatrix, job.mModelView);
        jobIt++;
    }

    jobIt = mTransparentRenderables.begin();
    
    while(jobIt != mTransparentRenderables.end())
    {
        RenderJob &job = (*jobIt);
        job.mRenderTarget->onRender(*this, *job.mRenderable, job.mModelViewProjectionMatrix, job.mModelView);
        jobIt++;
    }
    
    m_renderTargets.clear();
}

void Renderer::onBeginRenderUI(UIComponent* pRoot)
{
    glDisable(GL_DEPTH_TEST);
    
    float currentAngle = IEngine::getEngine()->getCurrentAngle();
    glm::quat rotation = glm::angleAxis<float>(currentAngle,0.0,0.0,1.0);
    glm::mat4x4 viewRotation = glm::toMat4(rotation);
    
    float scale = IEngine::getEngine()->getUIScale();
#ifdef __APPLE__
    //TODO: Figure out why this is revesed for APPLE
    glm::mat4 projection = glm::ortho(0.0f, float(m_height) * scale, float(m_width) * scale, 0.0f,0.0f, 100.0f);
#else
    glm::mat4 projection = glm::ortho(0.0f, float(m_width) * scale, float(m_height) * scale, 0.0f,0.0f, 100.0f);
#endif
    m_viewProjectionMatrix  = viewRotation * projection;
    pushTransformation(pRoot->getTransformation());
    pRoot->onBeginRender(*this);
    popTransformation();
}

glm::mat4 Renderer::getViewProjectionMatrix()
{
    return m_viewProjectionMatrix;
}

void Renderer::AddRenderTarget(IRenderTarget* renderTarget)
{
    STRING_ID renderTargetID = renderTarget->getRenderTargetID();
    if(StringManager::getStringFromID(renderTargetID))
    {
        m_renderTargets.push_back(renderTarget);
    }
    else
    {
        Logger::Error("Bad RenderTarget");
    }
}

void Renderer::pushTransformation(const glm::mat4 &transform, bool overRide)
{
    if(m_transformations.size() > 0 && !overRide)
    {
        m_transformations.push(m_transformations.top() * transform);
        mModelViewProjectionMatrixStack.push(mModelViewProjectionMatrixStack.top() * transform);
    }
    else
    {
        m_transformations.push(transform);
        mModelViewProjectionMatrixStack.push(getViewProjectionMatrix() * transform);
    }
}

void Renderer::popTransformation()
{
    if(m_transformations.size() > 0)
    {
        m_transformations.pop();
        mModelViewProjectionMatrixStack.pop();
    }
}

const glm::mat4& Renderer::getCurrentTransformation()
{
    if(m_transformations.size() < 1)
    {
        assert(false);
    }
    
    return m_transformations.top();
}

const glm::mat4& Renderer::getCurrentModelViewProjectionMatrix()
{
    if(mModelViewProjectionMatrixStack.size() < 1)
    {
        assert(false);
    }
    
    return mModelViewProjectionMatrixStack.top();
}

const glm::vec3& Renderer::getEyePosition()
{
	return m_pActiveCamera->getPosition();
}

bool Renderer::validate()
{
    int err = glGetError();
	while(err != 0)
    {
        switch(err)
        {
            case 1280:
                std::cout<< "Gl Use Program error code on enter rederer: 1280 GL_INVALID_ENUM " << std::endl;
                assert(false);
                break;
            case 1281:
                std::cout<< "Gl Use Program error code on enter rederer: 1281 GL_INVALID_VALUE " << std::endl;
                 assert(false);
                break;
            case 1282:
                std::cout<< "Gl Use Program error code on enter rederer: 1282 GL_INVALID_OPERATION " << std::endl;
                assert(false);
                break;
            case 1283:
                std::cout<< "Gl Use Program error code on enter rederer: 1283 GL_STACK_OVERFLOW " << std::endl;
                assert(false);
                break;
            case 1284:
                std::cout<< "Gl Use Program error code on enter rederer: 1284 GL_STACK_UNDERFLOW " << std::endl;
                assert(false);
                break;
            case 1285:
                std::cout<< "Gl Use Program error code on enter rederer: 1285 GL_OUT_OF_MEMORY " << std::endl;
                assert(false);
                break;
            case 1286:
			{
				std::cout << "Gl Use Program error code on enter rederer: 1286 GL_INVALID_FRAMEBUFFER_OPERATION_EXT " << std::endl;
				glCheckFramebufferStatus(GL_FRAMEBUFFER);
				assert(false);
			}
                break;
            default:
                std::cout<< "Gl Use Program error code on enter rederer: "<< err << std::endl;
                assert(false);
                break;
        }        
        
        err = glGetError();            
    }
    return err != 0 ? false : true;
}

void Renderer::setActiveCamera(Camera* pActiveCamera)
{
    if(m_pActiveCamera != NULL)
    {
        m_pActiveCamera->setIsActiveCamera(false);
    }
    
    m_pActiveCamera = pActiveCamera;
    
    if(m_pActiveCamera != NULL)
    {
        m_pActiveCamera->setIsActiveCamera(true);
    }
}
