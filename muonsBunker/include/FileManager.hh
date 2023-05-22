#ifndef FILEMANAGER_HH
#define FILEMANAGER_HH

#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

// use Meyers singleton as this is threadsafe
class FileManager {
public:
  static FileManager& GetFileManager(){
    static FileManager fileManInstance;
    return fileManInstance;
  }

  void ReadIniFile(std::string); // needs to be called first!
  void CreateResultsDir();
  pt::ptree GetPropTree();
  template <typename T>
  void AddValuePropTree(const std::string& key,const T& val);
  void WriteIniFile(std::string);


private:
  FileManager();
  ~FileManager();
  FileManager(const FileManager&) = delete;
  FileManager& operator=(const FileManager&) = delete; 

  pt::ptree fptree;

};

template <typename T>
void FileManager::AddValuePropTree(const std::string& key,const T& val){
  fptree.put(key, val);  
}




#endif