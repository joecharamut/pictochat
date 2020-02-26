#include <utility>

#ifndef PROJECT_VIRTUALFS_H
#define PROJECT_VIRTUALFS_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <utility>

struct StatData {
    std::string filename;
    std::string displayName;
    std::string extension;
    std::string displayExtension;
    int bytes;

    StatData(std::string filename, std::string extension, int bytes)
        : filename(std::move(filename)), extension(std::move(extension)), bytes(bytes) {
        this->filename.substr(0, 8);
        displayName = this->filename;
        displayName.resize(8, ' ');

        this->extension.substr(0, 5);
        displayExtension= this->extension;
        displayExtension.resize(5);
    };
};

struct FileNode {
    StatData value;
    std::vector<std::shared_ptr<FileNode>> children;

    FileNode(StatData value, std::vector<std::shared_ptr<FileNode>> children)
        : value(std::move(value)), children(std::move(children)) {};
};

class VirtualFS {
public:
    VirtualFS();
    ~VirtualFS();

    StatData stat(std::string path);
    void addFile(std::string path, StatData file);
    void addDir(std::string path, std::string name);
    std::shared_ptr<FileNode> pathToNode(std::string path);
    std::string nodeToPath(std::shared_ptr<FileNode> node);

private:
    std::shared_ptr<FileNode> fileTree;
};


#endif //PROJECT_VIRTUALFS_H
