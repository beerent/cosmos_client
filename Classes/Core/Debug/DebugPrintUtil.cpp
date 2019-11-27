//
//  DebugPrintUtil.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 3/10/15.
//
//

#include "DebugPrintUtil.h"
#include "Core/Debug/Logger/Logger.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>


namespace DebugPrintUtil
{

    void printGLVector(const glm::vec3 &vec)
    {
        std::stringstream ss;
        ss << "vec x: " << vec.x << " y: " << vec.y << " z: " << vec.z <<"\n";
        Logger::Log(Logger::MATH_3D, ss.str());
    }

    void printGLVector(const glm::vec4 &vec)
    {
        std::stringstream ss;
        ss << "vec x: " << vec.x << " y: " << vec.y << " z: " << vec.z << " w: " << vec.w <<"\n";
        Logger::Log(Logger::MATH_3D, ss.str());
    }
    
    void printGlTransform(const glm::mat4 &mat)
    {
        std::stringstream ss;

        glm::vec4 vec(0,0,0,1);
        
        ss <<  "mat4\n";
        ss << std::setprecision(4) << std::fixed;
        
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                ss << mat[i][j];
                if(j < 4)
                {
                    ss << ", ";
                }
            }
            ss << "\n";
        }
        
        ss << "vec x: " << vec.x << " y: " << vec.y << " z: " << vec.z << " w: " << vec.w <<"\n";
        vec = mat * vec;
        ss << "vec x: " << vec.x << " y: " << vec.y << " z: " << vec.z << " w: " << vec.w <<"\n";
        
        //ss << "vec x " << vec.x << "y " << vec.y << "z " << vec.z << "w " << vec.w <<"\n";
        Logger::Log(Logger::MATH_3D, ss.str());
    }
}