#include <utility>

#include "VirtualFS.h"

VirtualFS::VirtualFS() {
    addDir("C:\\", "DOS");
}

VirtualFS::~VirtualFS() = default;

StatData VirtualFS::stat(std::string path) {
    return StatData();
}

std::vector<StatData> VirtualFS::dirList(std::string path) {
    return std::vector<StatData>();
}

void VirtualFS::addFile(std::string path, StatData file) {

}

void VirtualFS::addDir(std::string path, std::string name) {
    FileNode node = pathToNode(std::move(path));
    node.children.push_back({ {std::move(name), "<DIR>", 0}, {} });
}

FileNode VirtualFS::pathToNode(std::string path) {

    for

    return INVALID_FILE;
}

