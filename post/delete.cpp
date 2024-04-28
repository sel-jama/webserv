#include "Delete.hpp"

void Delete::check_RequestedR(Request &obj){
    struct stat buffer;
    // if(!(obj.getUri().rfind('/', obj.getUri().size() - 1) != std::string::npos))
    //     throw Except();
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

void Delete::Work_with_file(Request &obj, handleCgi &o)
{
    method b;
    std::string check;
    size_t t = obj.path.find("..") + 1;
    // check ../ /.. /../  403
        if(t == obj.path.size())
            throw Except();
        else
        {
                if(std::remove(obj.path.c_str()) != 0)
                    throw Except();
        }

}

void Delete::Work_with_Directory(Request &obj, handleCgi &o)
{
    R_removing(obj);
}

void Delete::R_removing(Request &obj){
    DIR *dir;
    struct dirent *ent;

    // Open directory
    if ((dir = opendir(obj.path.c_str()))) {
        // Iterate through directory entries
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            std::string filepath = obj.path + '/' + filename;
            char *filepath2 = const_cast<char*>(filepath.c_str());
            if(ent->d_type == DT_DIR)
            {
                obj.path = filepath2;
                R_removing(obj);
            }
            if(std::remove(filepath.c_str()) != 0)
                throw Except();//403
        }
    }
    else
        throw Except();//500
    // No index file found
    return ;
}
