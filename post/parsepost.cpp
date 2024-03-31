#include "post.hpp"
#include "../Request/Request.hpp"
#include <ios>
#include <string.h>
#include <sys/_types/_size_t.h>


bool Post::getpost_i(){
    return post_indicate;
}

bool Post::getchunked_i(){
    return chunked_indicate;
}

std::string Post::get_saver(){
    return string_saver;
}

void Post::set_saver(std::string value)
{
    this->string_saver = value;
}

void Post::set_post_i(bool value){
    this->post_indicate = value;
}

void Post::set_chunked_i(bool value){
    this->chunked_indicate = value;
}

std::string Post::get_path(){
    return path;
}

void Post::set_path(std::string value){
    this->path = value;
}

void Post::set_count(size_t value){
    this->reading_count = value;
}

size_t Post::get_last_read(){
    return reading_count;
}

size_t Post::get_content_lenght(){
    return content_lenght;
}

void Post::set_content_lenght(size_t value){
    this->content_lenght = value;
}

int hexa_to_num(std::string ptr)
{
    int value;
    std::stringstream str(ptr);
    str >> std::hex >> value;
    return (value);
}

void Post::load_extension()
{
    extension["audio/aac"] = ".aac";
    extension["application/x-abiword"] = ".abw";
    extension["application/octet-stream"] = ".arc";
    extension["video/x-msvideo"] = ".avi";
    extension["application/vnd.amazon.ebook"] = ".azw";
    extension["application/octet-stream"] = ".bin";
    extension["image/bmp"] = ".bmp";
    extension["application/x-bzip"] = ".bz";
    extension["application/x-bzip2"] = ".bz2";
    extension["application/x-csh"] = ".csh";
    extension["text/css"] = ".css";
    extension["text/csv"] = ".csv";
    extension["application/msword"] = ".doc";
    extension["application/vnd.openxmlformats-officedocument.wordprocessingml.document"] = ".docx";
    extension["application/vnd.ms-fontobject"] = ".eot";
    extension["application/epub+zip"] = ".epub";
    extension["image/gif"] = ".gif";
    extension["text/html"] = ".html";
    extension["image/x-icon"] = ".ico";
    extension["text/calendar"] = ".ics";
    extension["application/java-archive"] = ".jar";
    extension["image/jpeg"] = ".jpeg";
    extension["application/javascript"] = ".js";
    extension["application/json"] = ".json";
    extension["audio/midi"] = ".midi";
    extension["video/mpeg"] = ".mpeg";
    extension["application/vnd.apple.installer+xml"] = ".mpkg";
    extension["application/vnd.oasis.opendocument.presentation"] = ".odp";
    extension["application/vnd.oasis.opendocument.spreadsheet"] = ".ods";
    extension["application/vnd.oasis.opendocument.text"] = ".odt";
    extension["audio/ogg"] = ".oga";
    extension["video/ogg"] = ".ogv";
    extension["application/ogg"] = ".ogx";
    extension["font/otf"] = ".otf";
    extension["image/png"] = ".png";
    extension["application/pdf"] = ".pdf";
    extension["application/vnd.ms-powerpoint"] = ".ppt";
    extension["application/vnd.openxmlformats-officedocument.presentationml.presentation"] = ".pptx";
    extension["application/x-rar-compressed"] = ".rar";
    extension["application/rtf"] = ".rtf";
    extension["application/x-sh"] = ".sh";
    extension["image/svg+xml"] = ".svg";
    extension["application/x-shockwave-flash"] = ".swf";
    extension["application/x-tar"] = ".tar";
    extension["image/tiff"] = ".tiff";
    extension["application/typescript"] = ".ts";
    extension["font/ttf"] = ".ttf";
    extension["application/vnd.visio"] = ".vsd";
    extension["audio/x-wav"] = ".wav";
    extension["audio/webm"] = ".weba";
    extension["video/webm"] = ".webm";
    extension["image/webp"] = ".webp";
    extension["font/woff"] = ".woff";
    extension["font/woff2"] = ".woff2";
    extension["application/xhtml+xml"] = ".xhtml";
    extension["application/vnd.ms-excel"] = ".xls";
    extension["application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"] = ".xlsx";
    extension["application/xml"] = ".xml";
    extension["application/vnd.mozilla.xul+xml"] = ".xul";
    extension["application/zip"] = ".zip";
    extension["audio/3gpp"] = ".3gp";
    extension["video/3gpp"] = ".3gp";
    extension["audio/3gpp2"] = ".3gp2";
    extension["video/3gpp2"] = ".3gp2";
    extension["application/x-7z-compressed"] = ".7z";
}
