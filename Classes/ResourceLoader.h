#pragma once
#include "Interface.hpp"

class ResourceLoader : public IResourceLoader
{
public:
	std::string getResourcePath() const;
	void loadPngImage(const std::string& name, int &width, int&height, void** data, int &dataSize);
};