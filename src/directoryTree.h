#ifndef DIRECTORYTREE_H
#define DIRECTORYTREE_H
#include "directories.h"

#include <sstream>

class DirectoryTree
{
  private:

  public:
    Directories * root;
    Directories * current;
    Directories * temp;
    DirectoryTree(Directories * root, Directories * current);
    ~DirectoryTree();

    Directories *getCurrent() const;
    Directories *getRoot() const;

	void goToPath(std::string name);
  std::string goToPathName(std::string name);
	void goBackToPath();
	void addDir(std::string name, Directories * parent);
	//bool delDir(std::string name);
	void addFilee(std::string name, int page);
	bool delFilee(std::string name);
	void ls();
	bool cd(std::string name);
	File *copy(std::string name, int page);
	void printFileData(std::string name, std::string path);
	//void paste(File file);
};

#endif
