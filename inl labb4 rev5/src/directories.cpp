#include "directories.h"

void Directories::printDir(){
  std::cout << this->name << " ";
}

Directories::Directories(std::string name, Directories * parent)
{
  this->name = name;
  this->parent = parent;
}
Directories::~Directories()
{
  for(unsigned int i = 0; i < children.size(); i++)
        delete children[i];

    for(unsigned int i = 0; i < files.size(); i++)
        delete files[i];
}
