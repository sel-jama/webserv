#include "delete.hpp"

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

void Delete::Work_with_file()
{
    method b;
    std::string check;
    // size_t t = obj.path.find("..") + 1;
    // // check ../ /.. /../  403
    //     if(t == obj.path.size())
    //         throw Except();
        // else
        // {d
                if(std::remove("file.txt2") != 0)
                    throw Except();
        // } 

}

void Delete::Work_with_Directory()
{
    R_removing();
}

void Delete::R_removing(){
    DIR *dir;
    struct dirent *ent;

    // Open directory
    if(variable == 0)
    {
        variable = 1;
        Path = "/goinfre/yboucha/webserv/post/folder/";
        std::cout << Path << std::endl;
    }
    dir = opendir(Path.c_str());
    if (dir != NULL) {
        dir = opendir(Path.c_str());
        // Iterate through directory entries
        while ((ent = readdir(dir)) != NULL) {
            std::string filpath = ent->d_name;
            std::cout << ent->d_name  << std::endl;
            std::string filepath = Path + '/' + ent->d_name;
            char *filepath2 = const_cast<char*>(filpath.c_str());
            // std::cout << filpath.c_str()  << "hi " << std::endl;
            if(ent->d_type == DT_DIR)
            {
                Path = filepath2;
                R_removing();
            }
            if(std::remove(filpath.c_str()) != 0)
                throw Except();//403
        }
    }
    else
    {
        std::cout << "final"<< std::endl;
        throw Except();//500
    }
    // No index file found
    return ;
}
