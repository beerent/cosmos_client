//
//  Renderer.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include <iostream>
#include <vector>
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/OpenSource/Vector.hpp"
#include "Core/Rendering/Shaders/ShaderManager.h"
#include <Stack>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Entity/Entities/Camera.h"

class UIComponent;
class Renderable;
class IRenderTarget;

// Coordinate system
// The in game coordinate system does not match the coordinate system in blender.

// In game - Right-Handed Cartesian Coordinates
// up/down                     y axis +/-  Green
// right/left                  x axis +/-  Red
// back/forward                z axis -/+  Blue

// In blender - Right-Handed Z up coordinate system common to most CAD software
// up/down                     z axis +/-  Blue
// right/left                  x axis +/-  Red
// back/forward                y axis +/-  Green

class Renderer
{
    private:
        std::vector<IRenderTarget* > m_renderTargets;
        bool   m_isInitialized;
        GLint  m_width;
        GLint  m_height;    
        GLuint m_framebuffer;
        GLuint m_renderbuffer;
    
        ivec2 m_viewPortSize;
        ivec2 m_viewPortPosition;
    
        glm::mat4 m_viewProjectionMatrix;
        Camera *m_pActiveCamera = NULL;
    
        std::stack<glm::mat4> m_transformations;
        std::stack<glm::mat4> mModelViewProjectionMatrixStack;
   
    
        struct RenderJob
        {
            IRenderTarget *mRenderTarget;
            RenderableObj *mRenderable;
            glm::mat4 mModelViewProjectionMatrix;
            glm::mat4 mModelView;
        };
    
        typedef std::vector<RenderJob> RenderJobVector;
        typedef std::map<STRING_ID, RenderJobVector*> RenderGroupMap;

        RenderGroupMap mRenderGroupMap;
        RenderJobVector mBackGroundRenderables;
        RenderJobVector mTransparentRenderables;
        RenderJobVector mNormalRenderables;
    
    public:
        Renderer();
    
        void SetViewPortSize(int width, int height);
        int getViewPortHeight() const {return m_height;}
        int getViewPortWidth() const {return m_width;}
        void Initialize();
        void onBeginRender();
        void AddRenderTarget(IRenderTarget* renderTarget);
        void addRenderable(IRenderTarget* renderTarget, RenderableObj* renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);
        void addRenderable(IRenderTarget* renderTarget, RenderableObj* renderable);

        void printLog(GLuint object);
        void pushTransformation(const glm::mat4 &transform, bool overRide = false);
        void popTransformation();
        const glm::mat4& getCurrentTransformation();
        const glm::mat4& getCurrentModelViewProjectionMatrix();
        glm::mat4 getViewProjectionMatrix();
        static bool validate();
        void onBeginRenderUI(UIComponent* pRoot);
        void setActiveCamera(Camera* pActiveCamera);
		const glm::vec3& getEyePosition();
};

