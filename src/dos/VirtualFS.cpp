#include <utility>
#include <iostream>

#include "VirtualFS.h"
#include "../util/Util.h"
#include "../util/Sources.h"

VirtualFS::VirtualFS() {
    fileTree = std::make_shared<FileNode>(
            StatData("", "", -1),
            std::vector<std::shared_ptr<FileNode>>(),
            nullptr
    );
    std::shared_ptr<FileNode> c_drive = std::make_shared<FileNode>(
            StatData("C:", "<DIR>", 0),
            std::vector<std::shared_ptr<FileNode>>(),
            fileTree
    );
    currentDirectory = c_drive;
    fileTree->children.push_back(c_drive);

    // registered commands go here
    addDir("C:", "DOS");

    // add source files under bin dir
    addDir("C:", "BIN");
    std::vector<std::string> sources = Util::splitString(Sources::SOURCES_STR, ";");
    for (int i = 0; i < sources.size(); i += 2) {
        std::string filename = Util::splitString(Util::splitString(sources[i], "/").back(), ".").front();
        addFile("C:\\BIN", StatData(filename, "O", atoi(sources[i+1].c_str())));
    }

    addFile("C:", StatData("soup",    "txt", rand() % 1024));
}

VirtualFS::~VirtualFS() = default;

StatData VirtualFS::stat(const std::string &path) {
    return pathToNode(path)->value;
}

void VirtualFS::addFile(const std::string &path, StatData file) {
    std::shared_ptr<FileNode> node = pathToNode(path);

    if (node) {
        printf("node: %s.%s\n", node->value.filename.c_str(), node->value.extension.c_str());

        //todo: fix filename
        std::string newFilename = Util::toUpperCase(file.filename);

        if (newFilename.size() > 8) {
            std::vector<std::string> children_names;

            for (const auto &c : node->children) {
                if (c->value.extension == file.extension) {
                    children_names.push_back(c->value.filename);
                }
            }

            int suffix = 0;
            do {
                suffix++;
                newFilename = newFilename.substr(0, 6) + "~" + std::to_string(suffix);
            } while (Util::vectorContains(children_names, newFilename));
        }

        StatData fixed = StatData(newFilename, Util::toUpperCase(file.extension), file.bytes);

        node->children.push_back(std::make_shared<FileNode>(
                fixed,
                std::vector<std::shared_ptr<FileNode>>(),
                node
        ));
    } else {
        printf("nullptr\n");
    }
}

void VirtualFS::addDir(const std::string &path, const std::string &name) {
    std::shared_ptr<FileNode> node = pathToNode(path);

    if (node) {
        printf("node: %s.%s\n", node->value.filename.c_str(), node->value.extension.c_str());
        node->children.push_back(std::make_shared<FileNode>(
                StatData(Util::toUpperCase(name), "<DIR>", 0),
                std::vector<std::shared_ptr<FileNode>>(),
                node
        ));
    } else {
        printf("nullptr\n");
    }
}

std::shared_ptr<FileNode> VirtualFS::pathToNode(const std::string &path) {
    std::vector<std::string> pathSplit = Util::splitString(path, "\\");

    for (const auto &str : pathSplit) {
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
        if (data.extension == "<DIR>" && data.filename == Util::toUpperCase(pathSplit[pathIdx])) {
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
    std::string path;

    std::shared_ptr<FileNode> parent = node;
    do {
        path.insert(0, "\\" + parent->value.filename);
    } while ((parent = parent->parent) != nullptr);

    return path.substr(2);
}

void VirtualFS::chdir(std::shared_ptr<FileNode> node) {
    currentDirectory = node;
}

std::string VirtualFS::getcwd() {
    return nodeToPath(currentDirectory);
}
