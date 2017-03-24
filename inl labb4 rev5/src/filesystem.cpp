
#include "filesystem.h"

FileSystem::FileSystem(Directories * root, Directories * current) {
  DirTree = new DirectoryTree(root, current);
  std::string currentDir = "/";    // current directory, used for output
  this->root = new Directories(currentDir, nullptr);
  this->page = 0;
}

FileSystem::~FileSystem() {
  delete DirTree;
  delete root;
}

void FileSystem::format()
{
  this->mMemblockDevice.reset();
  delete DirTree;
  delete root;
  this->page = 0;
  std::string currentDir = "/";    // current directory, used for output
  root = new Directories(currentDir, nullptr);
  DirTree = new DirectoryTree(root,root);
}

void FileSystem::createImage(std::string name){
  this->DirTree->current = this->DirTree->root;
  std::ofstream myfile;
  std::vector<std::string> saved;
  myfile.open(name);
  moveDownDir(this->DirTree->root, saved);
  myfile << saved.size() << std::endl;
  for(unsigned int i = 0; i < saved.size(); i++)
    myfile << saved[i];
  myfile << this->page << std::endl;
  for(int i = 0; i < this->page; i++)
  {
    //remyfile << i << endl;
    myfile << mMemblockDevice.readBlock(i).toString() << std::endl;
  }
  myfile.close();
}

void FileSystem::moveDownDir(Directories * name, std::vector<std::string> &saved){
  std::stringstream ss;
  if(name->parent != nullptr)
    ss << name->name << std::endl << name->parent->name << std::endl; //Write filename and parent, parent to locate where to create dir next start
  else
    ss << name->name << std::endl;
  ss << name->files.size() << std::endl;  //write number of files for a loop to pick all info up on restoreImage
  for(unsigned int i = 0; i < name->files.size(); i++)
    ss << name->files[i]->name << std::endl << name->files[i]->pageID << std::endl; //write name and page to be able to remake files later, pageID for the data of the file
  saved.push_back(ss.str());
  if(name){
    for(unsigned int i = 0; i < name->children.size(); i++){
      moveDownDir(name->children[i], saved); //Go down, left to right, of the tree to get all directories
    }
  }
 }

void FileSystem::moveDownAdd(Directories * dir, std::string name, std::string parent){
  if(dir->name == parent){
    this->DirTree->current = dir;
    this->DirTree->addDir(name, dir);
    this->DirTree->current = this->DirTree->root;
  }
  for(unsigned int i = 0; i < dir->children.size(); i++)
    moveDownAdd(dir->children[i], name, parent);
}

void FileSystem::moveAddFile(Directories *dir, std::string parent, std::string name, int tempID){
  if(dir->name == parent){
    this->DirTree->current = dir;
    this->DirTree->addFilee(name, tempID);
    this->DirTree->current = this->DirTree->root;
  }
  for(unsigned int i = 0; i < dir->children.size(); i++)
    moveAddFile(dir->children[i], parent, name, tempID);
}

void FileSystem::restoreImage(std::string name){
  this->DirTree->current = this->DirTree->root;
  std::ifstream input(name);
  int dirSize = 0, fileSize = 0, tempID = 0;
  std::string name1, name2, name3;
  input >> dirSize;
  //std::cout << dirSize << std::endl;
  //ROOT FILE MANAGEMENT
  input >> name1;
  //std::cout << name1 << std::endl;
  input >> fileSize;
  //std::cout << fileSize << std::endl;
  for(int k = 0; k < fileSize; k++){
    input >> name1;
    //std::cout << name1 << std::endl;
    input >> tempID;
    //std::cout << tempID << std::endl;
    this->DirTree->addFilee(name1, tempID);
  }
  this->DirTree->current = this->DirTree->root;
      //NORMAL FILE MANAGEMENT AND DIRECTORY MANAGEMENT
  for(int i = 0; i < dirSize-1; i++){ ///i = 1;
    input >> name1; ///this name gets first data
    //std::cout << name1 << std::endl;
    input >> name2; //CREATE DIRECTORY UNDER THIS DIRECTORY IF CURRENT DIR IS PARENT
    //std::cout << name2 << std::endl;
    moveDownAdd(this->DirTree->root, name1, name2);


    //ADD FILES TO THE CREATED DIRECTORY IF THERE ARE ANY
    input >> fileSize;
    //std::cout << fileSize << std::endl;
    for(int k = 0; k < fileSize; k++){
      input >> name3;
      //std::cout << name3 << std::endl;
      input >> tempID;
      //std::cout << tempID << std::endl;
      moveAddFile(this->DirTree->root, name1, name3, tempID);
    }
    this->DirTree->current = this->DirTree->root;
  }
    //ADD PAGES
    input >> this->page;
    //TEST
    //std::cout << this->page << std::endl;
    //ADD ALL MEMBLOCKS
    std::string inputdata;
    for(int i = 0; i < this->page; i++){
      input >> inputdata;
      if(inputdata.size() < 512){
        for (unsigned int i = inputdata.size(); i < 512; i++)
        inputdata += ' ';
      }
      //std::cout << inputdata << std::endl;
      //std::cout << name << std::endl;
      this->mMemblockDevice[i].writeBlock(inputdata);

      //TEST
      /*for(int i = 0; i < this->page; i++)
      {
        //remyfile << i << endl;
        std::cout << mMemblockDevice.readBlock(i).toString() << std::endl;
      }*/
    }

    //RESTORE TO root
    this->DirTree->current = this->DirTree->root;
}

void FileSystem::katt(std::string name){
  //FIX
  this->printFileData(name);
}



void FileSystem::copy(std::string path1, std::string path2){
	//File *fileCp = this->DirTree->copy(name1, this->page);
	std::string tempData;
  bool found = false;
  std::string name1 = this->DirTree->goToPathName(path1);
	for (unsigned int i = 0; i < this->DirTree->getCurrent()->files.size() && found == false; i++)
	{
		if (this->DirTree->getCurrent()->files[i]->name == name1)
		{
			tempData = this->mMemblockDevice[this->DirTree->getCurrent()->files[i]->pageID].readBlock().toString();
			//i = this->DirTree->getCurrent()->files.size() + 1;
			this->DirTree->goBackToPath();
			this->createFile(path2, tempData);
			found = true;
		}
	}
  this->DirTree->goBackToPath();
}



void FileSystem::createFile(std::string path, std::string data){
  bool found = false;

  std::string name = this->DirTree->goToPathName(path);

  for (unsigned int i = 0; i < this->DirTree->getCurrent()->children.size(); i++)
  {
    if (this->DirTree->getCurrent()->children[i]->name == name){
      std::cout << "folder with same name exists." << std::endl;
      found = true;
    }
  }
  for (unsigned int i = 0; i < this->DirTree->getCurrent()->files.size(); i++)
	{
		if (this->DirTree->getCurrent()->files[i]->name == name){
      std::cout << "File already exists." << std::endl;
      found = true;
    }
  }
  //IF name not taken
  if(found == false){
    if(data.size() < 512){
      for (unsigned int i = data.size(); i < 512; i++)
      data += ' ';
    }
    else if(data.size() == 512){
      std::cout << "Loaded full block." << std::endl;
    }
    else{
      for(int i = 0; i < 512; i++)
      data += ' ';
    }

    this->mMemblockDevice[this->page].writeBlock(data);

    this->DirTree->addFilee(name, this->page);
    this->page++;
  }

  this->DirTree->goBackToPath();
}

void FileSystem::createFolderi(std::string path = "."){
  bool found = false;

  std::string name = this->DirTree->goToPathName(path);

  for (unsigned int i = 0; i < this->DirTree->getCurrent()->children.size(); i++)
  {
    if (this->DirTree->getCurrent()->children[i]->name == name){
      std::cout << "folder with same name exists." << std::endl;
      found = true;
    }
  }
  for (unsigned int i = 0; i < this->DirTree->getCurrent()->files.size(); i++)
	{
		if (this->DirTree->getCurrent()->files[i]->name == name){
      std::cout << "File with same name exists." << std::endl;
      found = true;
    }
  }
  //IF name not taken
  if(found == false){
    this->DirTree->addDir(name, DirTree->getCurrent());
  }

  this->DirTree->goBackToPath();
}

void FileSystem::removeFile(std::string path){
  std::string name = this->DirTree->goToPathName(path);
  this->DirTree->delFilee(name);
  this->DirTree->goBackToPath();
}

//void FileSystem::removeFolder(std::string name){
//  this->DirTree->delDir(name);
//}

void FileSystem::listDir(std::string path){
  this->DirTree->goToPath(path);
  this->DirTree->ls();
  this->DirTree->goBackToPath();
}

void FileSystem::cd(std::string name){
  this->DirTree->goToPath(name);
}

void FileSystem::pwd(){
	std::vector<std::string> path;

	Directories * temp = DirTree->getCurrent();

	while (temp != DirTree->getRoot())
	{
		path.push_back(temp->name);
		temp = temp->parent;
	}
	if (temp == DirTree->getRoot())
		path.push_back("");
	//  path.push_back("");
	for (int i = path.size() - 1; i >= 0; i--) {
		std::cout << path[i] << "/";
	}

	std::cout << std::endl;
}

void FileSystem::printFileData(std::string path)
{
  bool found = false;
  std::string name = this->DirTree->goToPathName(path);
  //std::cout << this->DirTree->goToPathName(path);
  for(unsigned int i = 0; i < this->DirTree->getCurrent()->files.size() && found == false; i++)
  {
    if(this->DirTree->getCurrent()->files[i]->name == name)
    {
      //this->current->files[i]->printData();
      std::cout << this->mMemblockDevice[this->DirTree->getCurrent()->files[i]->pageID].readBlock().toString() << std::endl;
      found = true;
    }
    /*else
    { std::cout << "No file with that name found." << std::endl; }
   */}
this->DirTree->goBackToPath();
}



  /*bool found = false;
  if(path.size() < 1)
  {
	for(unsigned int i = 0; i < this->DirTree->getCurrent()->files.size() && found == false; i++)
	{
  	if(this->DirTree->getCurrent()->files[i]->name == name)
  	{
    	//this->current->files[i]->printData();
    	std::cout << this->mMemblockDevice[this->DirTree->getCurrent()->files[i]->pageID].readBlock().toString() << std::endl;
    	found = true;
  	}
	}
  }
  else
  {
  	this->DirTree->goToPath(path);
  	for(unsigned int i = 0; i < this->DirTree->getCurrent()->files.size() && found == false; i++)
  	{
    	if(this->DirTree->getCurrent()->files[i]->name == name)
    	{
      	//this->current->files[i]->printData();
      	std::cout << this->mMemblockDevice[this->DirTree->getCurrent()->files[i]->pageID].readBlock().toString() << std::endl;
      	found = true;
    	}
      else
      { std::cout << "No file with that name found." << std::endl; }
	   }
  this->DirTree->goBackToPath();
}*/
