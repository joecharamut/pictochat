#ifndef PROJECT_FILE_H
#define PROJECT_FILE_H

#include <string>
#include <memory>

class File {
public:
    File(const std::string &path);
    ~File();

    std::shared_ptr<std::string> read(int chars = -1);
    void write(const std::string &data);
    void seek(int offs, int origin);
    int size();

private:
    int pos = 0;
    FILE *filePtr;
};


#endif //PROJECT_FILE_H
