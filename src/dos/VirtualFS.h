#include <utility>

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
        displayExtension = this->extension;
        displayExtension.resize(3, ' ');
    };
};

struct FileNode {
    StatData value;
    std::vector<std::shared_ptr<FileNode>> children;
    std::shared_ptr<FileNode> parent;

    FileNode(StatData value, std::vector<std::shared_ptr<FileNode>> children, std::shared_ptr<FileNode> parent)
        : value(std::move(value)), children(std::move(children)), parent(std::move(parent)) {};
};

class VirtualFS {
public:
    VirtualFS();
    ~VirtualFS();

    StatData stat(const std::string& path);
    void addFile(const std::string& path, StatData file);
    void addDir(const std::string& path, const std::string &name);
    std::shared_ptr<FileNode> pathToNode(const std::string &path);
    std::string nodeToPath(std::shared_ptr<FileNode> node);
    void chdir(std::shared_ptr<FileNode> node);
    std::string getcwd();

private:
    std::shared_ptr<FileNode> fileTree;
    std::shared_ptr<FileNode> currentDirectory;
};


#endif //PROJECT_VIRTUALFS_H
