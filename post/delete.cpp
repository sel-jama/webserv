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
    
    if(std::remove("file.txt2") != 0)
            throw Except();
}

void Delete::Work_with_Directory()
{
    first_Delete();
}

void Delete::first_Delete()
{
    DIR *dir;
    struct dirent *ent;

    Path = "folder";
    dir = opendir(Path.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir))) {
            std::string filpath = ent->d_name;
            if(!std::strcmp(ent->d_name, ".") || !std::strcmp(ent->d_name, ".."))
                continue;
            std::string filepath = Path + '/' + ent->d_name;
            if(ent->d_type ==  DT_DIR)
                R_removing(filepath);
            if(std::remove(filepath.c_str()) != 0)
                throw Except();
        }
    }
}

void Delete::R_removing(std::string path){
    DIR *dir;
    struct dirent *ent;
    std::string saver;

    std::cout << path << " <============"<< std::endl;
    if (dir != NULL) {
        while ((ent = readdir(dir))) {
            std::cout << ent->d_name   << " ana "<< std::endl;
            std::string filpath = ent->d_name;
            if(!std::strcmp(ent->d_name, ".") || !std::strcmp(ent->d_name, ".."))
                continue;
            std::string filepath = path + '/' + ent->d_name;
            char *filepath2 = const_cast<char*>(ent->d_name);
            if(ent->d_type == DT_DIR)
            {
                saver = path;
                path = path + "/" + filepath2;
                R_removing(path);
                path = saver;
            } 
            if(std::remove(filepath.c_str()) != 0)
                throw Except();
        }
    }
    if(dir == NULL)
        throw Except();
    std::cout << "final" << std::endl;
    return ;
}
