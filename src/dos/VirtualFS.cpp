#include <utility>

#include "VirtualFS.h"
#include "../Util.h"

VirtualFS::VirtualFS() {
    fileTree = std::make_shared<FileNode>(
            StatData("", "", -1),
            std::vector<std::shared_ptr<FileNode>>()
    );

    std::shared_ptr<FileNode> c_drive = std::make_shared<FileNode>(
            StatData("C:", "<DIR>", 0),
            std::vector<std::shared_ptr<FileNode>>()
    );

    fileTree->children.push_back(c_drive);

    addDir("C:", "DOS");
    addFile("C:", StatData("wow", "txt", 4));
    addFile("C:", StatData("kinda", "txt", 8));
    addFile("C:", StatData("cringe", "txt", 16));
    addFile("C:", StatData("ngl", "txt", 32));
}

VirtualFS::~VirtualFS() = default;

StatData VirtualFS::stat(std::string path) {
    return StatData("", "", -1);
}

void VirtualFS::addFile(std::string path, StatData file) {
    std::shared_ptr<FileNode> node = pathToNode(std::move(path));

    if (node) {
        printf("node: %s.%s\n", node->value.filename.c_str(), node->value.extension.c_str());
        node->children.push_back(std::make_shared<FileNode>(file, std::vector<std::shared_ptr<FileNode>>()));
    } else {
        printf("nullptr\n");
    }
}

void VirtualFS::addDir(std::string path, std::string name) {
    std::shared_ptr<FileNode> node = pathToNode(std::move(path));

    if (node) {
        printf("node: %s.%s\n", node->value.filename.c_str(), node->value.extension.c_str());
        node->children.push_back(std::make_shared<FileNode>(StatData(std::move(name), "<DIR>", 0),
                std::vector<std::shared_ptr<FileNode>>()));
    } else {
        printf("nullptr\n");
    }
}

std::shared_ptr<FileNode> VirtualFS::pathToNode(std::string path) {
    std::vector<std::string> pathSplit = Util::splitString(path, "\\");

    for (auto str : pathSplit) {
        printf("%s, ", str.c_str());
    }
    printf("\n");

    std::shared_ptr<FileNode> currentNode = fileTree;

    int nodeIdx = 0;
    int pathIdx = 0;
    while (true) {
        if (pathIdx >= pathSplit.size()) break;
        if (nodeIdx >= currentNode->children.size()) return nullptr;

        StatData data = currentNode->children[nodeIdx]->value;
        if (data.extension == "<DIR>" && data.filename == pathSplit[pathIdx]) {
            currentNode = currentNode->children[nodeIdx];
            nodeIdx = 0;
            pathIdx++;
        } else {
            nodeIdx++;
        }
    }

    return currentNode;
}

std::string VirtualFS::nodeToPath(std::shared_ptr<FileNode> node) {
    return std::string();
}

