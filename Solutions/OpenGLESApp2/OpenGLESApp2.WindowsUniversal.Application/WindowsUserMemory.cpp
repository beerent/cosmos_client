#include "WindowsUserMemory.h"
#include <fstream>
#include <iostream>

void WindowsUserMemory::StoreUsername(const std::string& username) {
	std::ofstream fs("cosmosusername", std::ios_base::app);
	fs << username;
	fs.close();
	RetrieveUsername();
}

void WindowsUserMemory::StorePassword(const std::string& password) {
	std::ofstream fs("cosmospassword", std::ios_base::app);

	if (!fs) {
		std::cerr << "Cannot open the output file." << std::endl;
		return;
	}

	fs << password;
	fs.close();
}

std::string WindowsUserMemory::RetrieveUsername() {
	std::ifstream ifs("cosmosusername");
	if (!ifs) {
		return "";
	}

	std::string username((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return username;
}

std::string WindowsUserMemory::RetrievePassword() {
	std::ifstream ifs("cosmospassword");
	if (!ifs) {
		return "";
	}

	std::string password((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return password;
}