//
//  Primative.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef Projectios1_Primative_h
#define Projectios1_Primative_h

#include "Core/OpenSource/Vector.hpp"
#include "Core/Entity/Entities/Entity.h"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/StringManager/StringManager.h"

class Primative : public Entity{
public:
    static const StringManager::StringID CLASS_ID_PRIMATIVE;
    
    public:    
        Primative();
        virtual void Render();
        virtual void SetPosition(const Vector3<double> &position);  
    
    
    private:
};
#endif
