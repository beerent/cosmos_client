#include "ResourceLoader.h"
#include "Core/OpenSource/picopng.cpp"

std::string ResourceLoader::getResourcePath() const
{
	return "Resources";
}

void ResourceLoader::loadPngImage(const std::string& name, int &width, int&height, void** data, int &dataSize)
{
	std::vector<unsigned char> buffer;

	// LEAK LEAK LEAK TODO TODO need to add this to a texture manager
	std::vector<unsigned char>* image = new std::vector<unsigned char>();
	std::string nameWithPath = getResourcePath();
	std::string trbNameWithPath = nameWithPath;
	trbNameWithPath += "/" + name + ".trb";
	loadFile(*image, trbNameWithPath);

	if (image->size() > 0)
	{
		const int sizeDataLength = sizeof(int) * 2;
		int sizeData[2];

		unsigned char* wptr = reinterpret_cast<unsigned char*>(&sizeData[0]);
		unsigned char* rptr = &(*image->begin());
		rptr += image->size() - sizeDataLength;
		for (int i = 0; i < sizeDataLength; ++i)
		{
			*wptr = *rptr;
			++wptr;
			++rptr;
		}		

		width = sizeData[0];
		height = sizeData[1];
		dataSize = image->size() - sizeDataLength;
		unsigned char* bufferStart = &(*image->begin());
		*data = reinterpret_cast<void*>(bufferStart);
		return;
	}

	nameWithPath += "/" + name + ".png";

	loadFile(buffer, nameWithPath);
	unsigned long w, h;
	int error = decodePNG(*image, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());
	width = w;
	height = h;
	unsigned char* bufferStart = &(*image->begin());
	*data = reinterpret_cast<void*>(bufferStart);
	dataSize = image->size();
}