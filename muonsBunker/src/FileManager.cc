#include "FileManager.hh"

#include <iostream>
#include <sys/stat.h>
#include <sys/types.h> 
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>


FileManager::FileManager(){};
FileManager::~FileManager(){};

void FileManager::ReadIniFile(std::string filename) {
  // read ini File in property tree
  pt::ini_parser::read_ini(filename, fptree);
}

void FileManager::CreateResultsDir(){
  // create directory to store results
  std::string name = fptree.get<std::string>("output.foldername");
  std::string path = fptree.get<std::string>("output.pathname");
  std::string pathname = path+name;

  double status = mkdir(pathname.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  
  if(status < 0)
    std::cout << "Failed to create directory for results, maybe directory already exists? " << std::endl;
  else
    std::cout << "Results are stored at "<< pathname << std::endl;
}


void FileManager::WriteIniFile(std::string filename) {
  double method = std::stoi(fptree.get<std::string>("input.method"));
  if( method == 2){
    fptree.erase("gun");
  }

  // save property tree in file
  std::string name = fptree.get<std::string>("output.foldername");
  std::string path = fptree.get<std::string>("output.pathname");
  std::string pathname = path+name;

  pt::ini_parser::write_ini(path+name+"/"+filename, fptree);
}

pt::ptree FileManager::GetPropTree(){
    return fptree;
}





