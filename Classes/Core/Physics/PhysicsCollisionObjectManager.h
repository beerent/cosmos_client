//
//  PhysicsCollisionObjectManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 3/24/14.
//
//

#ifndef Projectios1_PhysicsCollisionObjectManager_h
#define Projectios1_PhysicsCollisionObjectManager_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Interface.hpp"
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else // __ANDROID__ or _WIN32
#include <GLES2/gl2.h>
#endif
#include "Core/OpenSource/Vector.hpp"
#include "Core/StringManager/StringManager.h"
#include "Core/Physics/PhysicsManager.h"

//KKonecko: Is this class even needed?

class PhysicsEntity;

class PhysicsCollisionObjectManager
{
public:
    
private:
    static PhysicsCollisionObjectManager* mInstance;
    
};

#endif
