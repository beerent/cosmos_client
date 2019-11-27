// TextureConverter.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <iostream>
#include <map>

//Windows file support
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <atlstr.h>
#include <cstdlib>
#include "../../../Classes/Core/OpenSource/picopng.cpp"
#pragma comment(lib, "User32.lib")

const std::string MODE_ALL_PNG_TO_BUFFER("ALL_PNG_TO_BUFFER");
const std::string MODE_SINGLE_PNG_TO_BUFFER("SINGLE_PNG_TO_BUFFER");

void convertAllPNGToBuffer(const std::string& path);
void convertPNG(const std::string& path, const std::string& file);
void convertPNGWithOutPath(const std::string& sourcePath, const std::string& outPath);

int main(int argc, char* argv[])
{
	std::cout << "Texture Converter" << std::endl;
	std::map<std::string, std::string> parsedArgs;

	for (int i = 1; i < argc; ++i)
	{
		std::string argStr(argv[i]);
		const int delimiterIndex = argStr.find('=');
		if (delimiterIndex > 0)
		{
			std::string argument = argStr.substr(0, delimiterIndex);
			int postDelimiterIndex = delimiterIndex + 1;
			std::string value = argStr.substr(postDelimiterIndex, argStr.length() - postDelimiterIndex);
			parsedArgs[argument] = value;

			std::cout << "Argument: " << argument.c_str() << " Value: " << value.c_str() << std::endl;
		}
		else
		{
			std::cout << "Can't split " << argStr.c_str() << " argument value pair '=' delimiter not found" << std::endl;
		}
	}

	if (parsedArgs["mode"] == MODE_SINGLE_PNG_TO_BUFFER)
	{
		std::string path = parsedArgs["path"];
		std::string outPath = parsedArgs["outPath"];
		convertPNGWithOutPath(path, outPath);
	}
	else if (parsedArgs["mode"] == MODE_ALL_PNG_TO_BUFFER)
	{
		convertAllPNGToBuffer(parsedArgs["path"]);
	}
	else
	{
		std::cout << "Unknown mode " << std::endl;
		std::cout << "Available modes:" << std::endl;
		std::cout << "MODE_ALL_PNG_TO_BUFFER required params: path" << std::endl;
	}

    return 0;
}

std::string wCharToString(wchar_t* source )
{
	size_t ln = wcslen(source);
	char *str = new char[ln + 1];
	memset(str, 0, ln + 1);
	wcstombs(str, source, ln);
	std::string stdStr(str);
	delete str;
	return stdStr;
}

void convertAllPNGToBuffer(const std::string& path)
{
	std::cout << "Converting all PNGs to texture buffer in path " << path.c_str() << std::endl;
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	size_t convertedChars = 0;
	size_t newsize = strlen(path.c_str()) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	mbstowcs_s(&convertedChars, wcstring, newsize, path.c_str(), _TRUNCATE);

	StringCchCopy(szDir, MAX_PATH, wcstring);
	delete wcstring;
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		std::cout << "No files found in directory " << std::endl;
		return;
	}

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			std::string fileName(wCharToString(ffd.cFileName));
	
			int delimiterIndex = fileName.find('.');
			int postDelimiterIndex = delimiterIndex + 1;
			std::string fileTypeStr = fileName.substr(postDelimiterIndex, fileName.size() - postDelimiterIndex);
			if (fileTypeStr == "png")
			{
				convertPNG(path, fileName);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	std::cout << "Conversion Complete" << std::endl;
}

void convertPNG(const std::string& path, const std::string& file)
{
	std::string nameWithPath = path + "\\" + file;

	std::string outNameWithPath = nameWithPath.substr(0, nameWithPath.length() - 3);
	outNameWithPath += "trb";

	convertPNGWithOutPath(nameWithPath, outNameWithPath);
}

void convertPNGWithOutPath(const std::string& sourcePath, const std::string& outPath)
{
	std::cout << "Converting file " << sourcePath.c_str() << std::endl;

	using std::cout;
	using std::endl;
	using std::string;

	static std::vector<unsigned char> image;
	std::vector<unsigned char> buffer;
	loadFile(buffer, sourcePath);
	unsigned long w, h;
	int error = decodePNG(image, w, h, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());
	if (error != 0)
	{
		cout << "Conversion Failed with error: " << error << endl;
		return;
	}

	const int sizeDataLength = sizeof(int) * 2;
	unsigned char sizeData[sizeDataLength];
	unsigned char* wptr = &sizeData[0];
	int* wptrInt = reinterpret_cast<int*>(wptr);
	*wptrInt = w;
	++wptrInt;
	*wptrInt = h;

	for (int i = 0; i < sizeDataLength; ++i)
	{
		image.push_back(*wptr);
		++wptr;
	}

	writeFile(image, outPath);
}