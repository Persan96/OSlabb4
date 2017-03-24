#include "directoryTree.h"

DirectoryTree::DirectoryTree(Directories * root, Directories * current) {
	this->root = root;
	this->current = current;
}

DirectoryTree::~DirectoryTree() {

}

Directories * DirectoryTree::getCurrent() const {
	return this->current;
}
Directories * DirectoryTree::getRoot() const {
	return this->root;
}



void DirectoryTree::addDir(std::string name, Directories * parent) {
	this->current->children.push_back(new Directories(name, parent));
}


/*bool DirectoryTree::delDir(std::string name){
bool found = false;
this->temp = this->current;

for(unsigned int i = 0; i < this->current->children.size(); i++){
if(this->current->children[i]->name == name){
this->cd(this->current[i].name);

if(this->current->files.size() == 0)
{
this->goBackToPath();
delete this->current->children[i];
found = true;
}
else
{
this->goBackToPath();
std::cout << "Files in dirctorys" << std::endl;
}
}
}

return found;
}*/

void DirectoryTree::addFilee(std::string name, int page) {
	this->current->files.push_back(new File(name, page));
}

bool DirectoryTree::delFilee(std::string name) {
	bool found = false;
	for (unsigned int i = 0; i < this->current->files.size(); i++) {
		if (this->current->files[i]->name == name) {
			delete this->current->files[i];
			this->current->files[i] = nullptr;
			this->current->files[i] = this->current->files[this->current->files.size()];
			this->current->files.pop_back();
		}
	}
	return found;
}

void DirectoryTree::ls() {
	for (unsigned int i = 0; i < this->current->files.size(); i++)
		this->current->files[i]->printFile();
	for (unsigned int j = 0; j < this->current->children.size(); j++)
		this->current->children[j]->printDir();
}

bool DirectoryTree::cd(std::string name) {
	bool found = false;
	if (name == ".." && this->current != this->root) {
		this->current = this->current->parent;
		found = true;
	}
	else {
		for (unsigned int j = 0; j < this->current->children.size() && found == false; j++)
		{
			if (this->current->children[j]->name == name)
			{
				this->current = this->current->children[j];
				found = true;
			}
		}
	}
	return found;
}

void DirectoryTree::goToPath(std::string name) {
	this->temp = this->current;

	//std::stringstream tempS;
	if (name[0] == '/')
		this->current = this->root;

	std::string tempS = "";
	std::vector<std::string> folders;
	//std::cout << name.size() << std::endl;
	for (unsigned int i = 0; i < name.size(); i++)
	{
		if (name[i] != '/')
		{
			tempS += name[i];
		}
		if (name[i] == '/' || i == name.size() - 1)
		{
			folders.push_back(tempS);
			tempS = "";
		}
	}

	tempS = "";

	for (unsigned int i = 0; i < folders.size(); i++)
	{
		tempS = folders[i];
		if (this->cd(tempS) == false)
		{
			i = folders.size() + 1;
			this->goBackToPath();
			std::cout << "Folder not found. Dir did not change." << std::endl;
		}
	}
}

//USED TO SEND BACK NAME TO PERFORM INSTRUCTION ON, EG: CAT(SENDBACK);
std::string DirectoryTree::goToPathName(std::string name) {
		this->temp = this->current;
		//std::stringstream tempS;
		if (name[0] == '/')
			this->current = this->root;

		std::string tempS = "";
		std::string sendBack = "";

		std::vector<std::string> folders;
		//std::cout << name.size() << std::endl;
		for (unsigned int i = 0; i < name.size(); i++)
		{
			if (name[i] != '/')
			{
				tempS += name[i];
			}
			else if (name[i] == '/')
			{
				folders.push_back(tempS);
				tempS = "";
			}
			if (i == name.size() - 1)
				sendBack = tempS;
		}
		tempS = "";

		for (unsigned int i = 0; i < folders.size(); i++)
		{
			tempS = folders[i];
			if (this->cd(tempS) == false)
			{
				i = folders.size() + 1;
				this->goBackToPath();
				std::cout << "Folder not found. Dir did not change." << std::endl;
			}
		}
			return sendBack;
}

void DirectoryTree::goBackToPath() {
	this->current = this->temp;
}

File *DirectoryTree::copy(std::string name, int page) {
	bool found = false;
	File * copied = nullptr;

	for (unsigned int j = 0; j < this->current->files.size() && found == false; j++)
	{
		if (this->current->files[j]->name == name)
		{
			//SKAPA FIL SOM SKA LÄMNAS TILLBAKS
			copied = new File(this->current->files[j]->name, page);
		}
	}
	return copied;
}

void DirectoryTree::printFileData(std::string name, std::string path)
	{
	bool found = false;
	if(path.size() < 1)
		{
			for(unsigned int i = 0; i < this->current->files.size() && found == false; i++)
				{
					if(this->current->files[i]->name == name)
						{
							this->current->files[i]->printFile();
							found = true;
						}
				}
		}
	else
	{
	this->goToPath(path);
	for(unsigned int i = 0; i < this->current->files.size() && found == false; i++)
		{
			if(this->current->files[i]->name == name)
				{
					this->current->files[i]->printFile();
					found = true;
				}
		}
	}
}
