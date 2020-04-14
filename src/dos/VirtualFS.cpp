#include <utility>
#include <iostream>

#include "VirtualFS.h"
#include "../util/Util.h"
#include "../util/Sources.h"
#include "../fs/Filesystem.h"

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
        addFile("C:\\BIN", StatData(filename, "O", atoi(sources[i+1].c_str())), false);
    }

    addFile("C:", StatData("soup", "txt", rand() % 1024), false);

    for (const auto &filePair : Filesystem::files) {
        std::string filename = Util::splitString(filePair.first, "\\").back();
        auto fileSplit = Util::splitString(filename, ".");
        std::string path = filePair.first.substr(0, filePair.first.size() - filename.size());
        addFile(path, StatData(fileSplit[0], fileSplit[1], Filesystem::openFile(filePair.first)->size()), true);
    }
}

VirtualFS::~VirtualFS() = default;

StatData VirtualFS::stat(const std::string &path) {
    return pathToNode(path)->value;
}

void VirtualFS::addFile(const std::string &path, StatData file, bool realFile) {
    printf("trying to add file: (p: %s, f: %s)\n", path.c_str(), (file.filename + "." + file.extension).c_str());
    std::shared_ptr<FileNode> node = pathToNode(path);

    if (node) {
//        printf("node: %s.%s\n", node->value.filename.c_str(), node->value.extension.c_str());

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
        fixed.realFile = realFile;

        node->children.push_back(std::make_shared<FileNode>(
                fixed,
                std::vector<std::shared_ptr<FileNode>>(),
                node
        ));
    } else {
        printf("fs err: node is null\n");
    }
}

void VirtualFS::addDir(const std::string &path, const std::string &name) {
    std::shared_ptr<FileNode> node = pathToNode(path);

    if (node) {
//        printf("node: %s.%s\n", node->value.filename.c_str(), node->value.extension.c_str());
        node->children.push_back(std::make_shared<FileNode>(
                StatData(Util::toUpperCase(name), "<DIR>", 0),
                std::vector<std::shared_ptr<FileNode>>(),
                node
        ));
    } else {
        printf("fs err: node is null\n");
    }
}

std::string VirtualFS::resolvePath(const std::string &path) {
    std::string newPath = path;
    if (!Util::stringStartsWith(newPath, "C:")) {
        newPath = nodeToPath(currentDirectory) + "\\" + newPath;
    }

    auto split = Util::splitString(newPath, "\\");
    std::vector<std::string> newParts;
    for (const auto &part : split) {
        if (part == ".") {
            printf("skip .\n");
            continue;
        } else if (part == "..") {
            printf("erase ..\n");
            newParts.pop_back();
            continue;
        } else {
            newParts.push_back(part);
        }
    }

    std::string consPath;
    for (const auto &pathPart : newParts) {
        consPath += pathPart;
        consPath += "\\";
    }
    consPath.pop_back();

    printf("resolve path: %s -> %s\n", path.c_str(), consPath.c_str());
    return consPath;
}

std::shared_ptr<FileNode> VirtualFS::pathToNode(const std::string &inPath) {
    std::string path = resolvePath(inPath);

    std::vector<std::string> pathSplit = Util::splitString(path, "\\");

//    for (const auto &str : pathSplit) {
//        printf("%s, ", str.c_str());
//    }
//    printf("\n");

    std::shared_ptr<FileNode> currentNode = fileTree;

    int nodeIdx = 0;
    int pathIdx = 0;
    while (true) {
        if (pathIdx >= pathSplit.size()) break;
        if (nodeIdx >= currentNode->children.size()) return nullptr;

        auto nodeSplit = Util::splitString(pathSplit[pathIdx], ".");
        std::string filename = Util::toUpperCase(nodeSplit[0]);
        std::string extension = (nodeSplit.size() == 2 ? Util::toUpperCase(nodeSplit[1]) : "<DIR>");

        StatData data = currentNode->children[nodeIdx]->value;
        if (data.filename == filename && data.extension == extension) {
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
        if (parent->value.extension != "<DIR>") {
            path.insert(0, "." + parent->value.extension);
        }
        path.insert(0, "\\" + parent->value.filename);
    } while ((parent = parent->parent) != nullptr);

    return path.substr(3);
}

void VirtualFS::chdir(std::shared_ptr<FileNode> node) {
    currentDirectory = node;
}

std::string VirtualFS::getcwd() {
    return nodeToPath(currentDirectory);
}
