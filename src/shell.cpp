#include <iostream>
#include <sstream>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 15;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
std::string help();

int main(void) {



  //create
  std::string name, name2;
  std::string data;
  std::string path = "", path2 = "";

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output
  Directories root(currentDir, nullptr);
  FileSystem fs(&root, &root);

    bool bRun = true;

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

            case 0: // quit
                bRun = false;
                std::cout << "Exiting\n";
                break;
            case 1: // format
                // Call fileSystem.format()
                fs.format();
                break;
            case 2: // ls
                path = commandArr[1];
                std::cout << "Listing directory" << std::endl;
                fs.listDir(path);
                std::cout << std::endl;
                // Call filesystem.ls()
                break;
            case 3: // create
                name = commandArr[1];
                data = commandArr[2];
                fs.createFile(name, data);
                break;
            case 4: // cat
            //std::cout << "I'm a cat." << std::endl;
                name = commandArr[1];
                fs.katt(name);
                break;
            case 5: // createImage
                name = commandArr[1];
                fs.createImage(name);
                break;
            case 6: // restoreImage
                name = commandArr[1];
                fs.restoreImage(name);
                break;
            case 7: // rm
                name = commandArr[1];
                fs.removeFile(name);
                //fs.removeFolder(name);
                break;
            case 8: // cp
                name = commandArr[1];
                name2 = commandArr[2];
                fs.copy(name, name2);
                break;
            case 9: // append
            //NOT NEEDED
                break;
            case 10: // mv
            //NOT NEEDED
                break;
            case 11: // mkdir
                name = commandArr[1];
                fs.createFolderi(name);
                break;
            case 12: // cd
            name = commandArr[1];
            fs.cd(name);
                break;
            case 13: // pwd
            fs.pwd();
                break;
            case 14: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
        path = "";
        path2 = "";
        name = "";
        name2 = "";
        data = "";
        for(int i = 0; i < 4; i++)
            commandArr[i] = "";
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path> <content>:          Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <file>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}
