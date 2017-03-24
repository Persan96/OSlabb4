#ifndef DIRECTORIES_H
#define DIRECTORIES_H
#include <iostream>
#include <string>
#include <vector>
#include "file.h"

class Directories {
public:
	std::string name;
	Directories * parent;
	std::vector<Directories*> children;
	std::vector<File*> files;

	void printDir();

	Directories(std::string name, Directories * parent);
	~Directories();
private:
};

#endif
