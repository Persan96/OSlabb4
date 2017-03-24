#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "directoryTree.h"
#include <fstream>

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;
    DirectoryTree * DirTree;
    Directories * root;
    int page;
    // Here you can add your own data structures

public:
    FileSystem(Directories * root, Directories * current);
    ~FileSystem();

    void format();

    void moveDownDir(Directories *name, std::vector<std::string> &saved);

    void moveDownAdd(Directories *dir, std::string name, std::string parent);

    void moveAddFile(Directories *dir, std::string parent, std::string name, int tempID);

    void createImage(std::string name);

    void restoreImage(std::string name);
    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */
    void katt(std::string name);

    void copy(std::string path1, std::string path2);
    /* This function creates a file in the filesystem */
    // createFile(...)
    void createFile(std::string path, std::string data);

    /* Creates a folder in the filesystem */
    // createFolderi(...);
    void createFolderi(std::string path);

    /* Removes a file in the filesystem */
    // removeFile(...);
    void removeFile(std::string path);

    /* Removes a folder in the filesystem */
    // removeFolder(...);
    void removeFolder(std::string name, std::string path);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);
    //to be made

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);
    void listDir(std::string path);

    void cd(std::string name);

    void pwd();
    /* Add your own member-functions if needed */

	//print data in page
	void printFileData(std::string path);
};

#endif // FILESYSTEM_H
