#ifndef PROJECT_VIRTUALFS_H
#define PROJECT_VIRTUALFS_H

#include <string>
#include <vector>
#include <map>

struct StatData {
    std::string filename;
    std::string extension;
    int bytes;
};

struct FileNode {
    StatData value;
    std::vector<FileNode> children;
};

class VirtualFS {
public:
    VirtualFS();
    ~VirtualFS();

    StatData stat(std::string path);
    std::vector<StatData> dirList(std::string path);
    void addFile(std::string path, StatData file);
    void addDir(std::string path, std::string name);

private:
    const FileNode INVALID_FILE = { {"NUL", "NUL", -1}, {} };
    FileNode fileTree = { {"C:", "<DIR>", 0}, {} };

    FileNode pathToNode(std::string path);
};


#endif //PROJECT_VIRTUALFS_H
