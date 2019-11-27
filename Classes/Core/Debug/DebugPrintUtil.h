//
//  DebugPrintUtil.h
//  Projectios2
//
//  Created by Kamil Konecko on 3/10/15.
//
//

#ifndef __Projectios2__DebugPrintUtil__
#define __Projectios2__DebugPrintUtil__

#include <stdio.h>
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"

/*
namespace glm
{
    class vec3;
}
*/

namespace DebugPrintUtil
{

    void printGLVector(const glm::vec3 &vec);
    void printGLVector(const glm::vec4 &vec);
    void printGlTransform(const glm::mat4 &mat);
    
}
#endif /* defined(__Projectios2__DebugPrintUtil__) */
