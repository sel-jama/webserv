#include "Delete.hpp"

void Delete::check_RequestedR(Request obj){
    struct stat buffer;
    if(!(obj.getUri().rfind('/', obj.getUri().size() - 1) != std::string::npos))
        throw Except();
    if(obj.path.empty()){
        throw Except();
    }
    const char *ptr = obj.path.c_str();
    int check = access(ptr, F_OK);
    if (check  == -1) {
        throw Except();
    }
    int val = stat(ptr, &buffer);
    if(S_ISDIR(buffer.st_mode))
       Type = "Directory";
    else
        Type = "File";
}

