#include "delete.hpp"

void Delete::check_RequestedR(Request &obj){
    struct stat buffer;
    location obj1 = obj.getMatchedLocation();
    if(obj.path.empty() || obj.method != "DELETE"){
        obj.statusCode = 405; 
        throw std::runtime_error("eror");
    }
    const char *ptr = obj.path.c_str();
    int check = access(ptr, F_OK);
    if (check  == -1) {
        obj.statusCode = 404; 
        throw std::runtime_error("eror");
    }
    int val = stat(ptr, &buffer);
    (void)val;
    if(S_ISDIR(buffer.st_mode))
    {
        if(obj1.root.size()  >= obj.path.size())
        {
            obj.statusCode = 403; 
            throw std::runtime_error("eror");
        }
       Work_with_Directory(obj);
    }
    else if(buffer.st_mode &S_IWUSR){
        if(obj1.root.size()  >= obj.path.size()){
            obj.statusCode = 403;
            throw std::runtime_error("eror");
        }
        Work_with_file(obj);
    }
    else{
        obj.statusCode = 403;
        throw std::runtime_error("eror");
    }
}

void Delete::Work_with_file(Request &obj)
{
    std::string check;
    struct stat buffer;

    int val = stat(obj.path.c_str(), &buffer);
    if(val != 0)
    {
        obj.statusCode = 500;
        throw std::runtime_error("eror");
    }
    if(buffer.st_mode &S_IWUSR)
    {
        obj.statusCode = 204;
        if(std::remove(obj.path.c_str()) != 0){
            obj.statusCode = 500;
            throw std::runtime_error("eror");
        }
    }
    else
    {
        obj.statusCode = 403;
        throw std::runtime_error("eror");
    }
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
                R_removing(filepath, obj);
            int val = stat(filepath.c_str(), &buffer);
            (void)val;
            if(buffer.st_mode & S_IWUSR)
            {
                if(std::remove(filepath.c_str()) != 0 )
                    obj.statusCode = 500;
            }
            else
                obj.statusCode = 403;
        }
    }
    else{
        obj.statusCode = 403;
        throw std::runtime_error("eror");
    }
    closedir(dir);
    if( std::remove(Path.c_str()) != 0){
        if(obj.statusCode == 0)
            obj.statusCode = 500;
        throw std::runtime_error("eror");
    }
    obj.statusCode = 204;
}

void Delete::R_removing(std::string path, Request &obj){
    DIR *dir;
    struct dirent *ent;
    std::string saver;
    struct stat buffer;


    dir = opendir(path.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir))) {
            std::string filpath = ent->d_name;
            if(!std::strcmp(ent->d_name, ".") || !std::strcmp(ent->d_name, ".."))
                continue;
            std::string filepath = path + '/' + ent->d_name;
            char *filepath2 = const_cast<char*>(ent->d_name);
            if(ent->d_type == DT_DIR)
            {
                saver = path;
                path = path + "/" + filepath2;
                R_removing(path, obj);
                path = saver;
            } 
            int val = stat(filepath.c_str(), &buffer);
            if(val != 0)
            {
                obj.statusCode = 500;
                throw std::runtime_error("eror");
            }
            if(buffer.st_mode &S_IWUSR)
            {
                if(std::remove(filepath.c_str()) != 0 )
                    obj.statusCode = 500;
            }
            if(dir == NULL)
                obj.statusCode = 403;
        }
    }
    else
        obj.statusCode = 403;
    closedir(dir);
    return ;
}
