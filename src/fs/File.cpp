#include "File.h"

File::File(const std::string &path) {
    printf("opening file\n");
    filePtr = fopen(path.c_str(), "a+");
    rewind(filePtr);
}

File::~File() {
    printf("closing file\n");
    fclose(filePtr);
}

std::shared_ptr<std::string> File::read(int chars) {
    if (chars == -1) chars = size();

    char *buf = (char *) calloc(chars, sizeof(char));
    fread(buf, sizeof(char), chars, filePtr);
    std::shared_ptr<std::string> str = std::make_shared<std::string>(buf);
    free(buf);
    return str;
}

void File::write(const std::string &data) {
    fwrite(data.c_str(), sizeof(char), data.size(), filePtr);
}

void File::seek(int offs, int origin) {
    fseek(filePtr, offs, origin);
}

int File::size() {
    int size;

    fseek(filePtr, 0, SEEK_END);
    size = ftell(filePtr);
    fseek(filePtr, pos, SEEK_SET);

    return size;
}
