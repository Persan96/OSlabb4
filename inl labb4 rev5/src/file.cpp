#include "file.h"

int File::getPageID() const
{
	return this->pageID;
}

void File::printFile() {
	std::cout << this->name << " ";
}

File::File(std::string name, int pageID) {
	this->name = name;
	this->pageID = pageID;
}

File::~File() {

}
