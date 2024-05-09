#include "delete.hpp"

void Delete::check_RequestedR(Request &obj){
    struct stat buffer;
    location obj1 = obj.getMatchedLocation();

    // std::cout << obj.path << std::endl;
    // if(!(obj.getUri().rfind('/', obj.getUri().size() - 1) != std::string::npos))
    //     throw Except();
    if(obj.path.empty() || obj.method != "DELETE"){
        throw std::runtime_error("eror");
    }
    const char *ptr = obj.path.c_str();
    int check = access(ptr, F_OK);
    if (check  == -1) {
        throw std::runtime_error("eror");
    }
    int val = stat(ptr, &buffer);
    (void)val;
    if(S_ISDIR(buffer.st_mode))
    {
        if(obj1.root.size()  > obj.path.size())
            throw std::runtime_error("eror");
       Work_with_Directory(obj);
    }
    else if(buffer.st_mode &S_IWUSR){
        if(obj1.root.size()  > obj.path.size())
            throw std::runtime_error("eror");
        Work_with_file(obj);
    }
    else
        throw std::runtime_error("eror");
}

void Delete::Work_with_file(Request &obj)
{
    std::string check;
    if(std::remove(obj.path.c_str()) != 0)
            throw std::runtime_error("eror");
}

void Delete::Work_with_Directory(Request &obj){
        first_Delete(obj);
}

void Delete::first_Delete(Request &obj)
{
    DIR *dir;
    struct dirent *ent;
    struct stat buffer;

    Path = obj.path;
    dir = opendir(Path.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir))) {
            std::string filpath = ent->d_name;
            if(!std::strcmp(ent->d_name, ".") || !std::strcmp(ent->d_name, ".."))
                continue;
            std::string filepath = Path + '/' + ent->d_name;
            if(ent->d_type ==  DT_DIR)
            {
                std::cout << filepath << std::endl;
                R_removing(filepath);
            }
            int val = stat(filepath.c_str(), &buffer);
            (void)val;
            if(buffer.st_mode & S_IWUSR)
            {
                if(std::remove(filepath.c_str()) != 0 )
                    throw std::runtime_error("eror");
            }
            else
                throw std::runtime_error("eror");
        }
    }
    else
        throw std::runtime_error("eror");
    if(std::remove(Path.c_str()) != 0 )
        throw std::runtime_error("eror");
}

void Delete::R_removing(std::string path){
    DIR *dir;
    struct dirent *ent;
    std::string saver;
    struct stat buffer;


    dir = opendir(path.c_str());
    if (dir != NULL) {
        std::cout << path << std::endl;
        while ((ent = readdir(dir))) {
            std::cout << ent->d_name   << " ana " << std::endl;
            std::string filpath = ent->d_name;
            if(!std::strcmp(ent->d_name, ".") || !std::strcmp(ent->d_name, ".."))
                continue;
            std::string filepath = path + '/' + ent->d_name;
            std::cout << filepath << std::endl;
            char *filepath2 = const_cast<char*>(ent->d_name);
            if(ent->d_type == DT_DIR)
            {
                saver = path;
                path = path + "/" + filepath2;
                R_removing(path);
                path = saver;
            } 
            int val = stat(filepath.c_str(), &buffer);
            (void)val;
            if(buffer.st_mode &S_IWUSR)
            {
                if(std::remove(filepath.c_str()) != 0 )
                    throw std::runtime_error("eror");
            }
            if(dir == NULL)
                throw std::runtime_error("eror");
        }
    }
    else
        throw std::runtime_error("eror");
    std::cout << "final" << std::endl;
    return ;
}
