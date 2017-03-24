#ifndef FILE_H
#define FILE_H
#include <string>
#include <iostream>

class File {
private:
public:
	std::string name;
	//std::string data;

	int pageID;

	int getPageID() const;
	void printFile();

	File(std::string name, int page);
	~File();
};

#endif
