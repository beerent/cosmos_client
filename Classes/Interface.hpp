#pragma once
#include "Core/OpenSource/Vector.hpp"
#include <string>

struct IResourceLoader
{
public:
    virtual std::string getResourcePath() const = 0;
    virtual void loadPngImage(const std::string& name, int &width, int&height, void** data, int &dataSize) = 0;
    ~IResourceLoader(){};
};


//#if def this for mac
IResourceLoader* CreateResourceLoader();

