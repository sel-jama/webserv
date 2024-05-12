/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-jama <sel-jama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 16:13:01 by sel-jama          #+#    #+#             */
/*   Updated: 2024/05/11 15:38:10 by sel-jama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Method.hpp"
#include "../Request/Request.hpp"

bool Method::loacationHasCgi(Request &req, handleCgi &cgi) {
	try{
		cgi.setScriptName(req.fileName);
		cgi.validateCgi(req);
		return true;
	}
	catch(const std::runtime_error &e){
		return false;
	}
	return true;
}

void Method::validateAll(Request &req) const{
	if (!(req.pathStatus.st_mode& S_IRUSR)){
		req.statusCode = 403;
		throw std::runtime_error("forbiden : permission denied");
	}
}

//handle GET Method
void Method::GetDataForClient(Request &req){
	defineResourceType(req);
	if (type == "file"){
		//if location does not have cgi
		validateAll(req);
		if (loacationHasCgi(req, cgi)){
			cgi.executeCgiScript(req);
		}
		else{
			req.filePath = req.path;
			req.responseContentLen =  req.pathStatus.st_size;
		}
	}
	else
		handleDirectory(req);

	this->response = content;
}

std::string Method::readContent(Request &req){
	std::ifstream file(req.path.c_str());
	if (!file.is_open())
	{
		std::remove(req.path.c_str());
		req.statusCode = 500;
		throw std::runtime_error("set error page : failed to read requested content");
		std::cerr << "failed to read content" << std::endl;
	}

	std::ostringstream content;
	content << file.rdbuf();
	file.close();
	return content.str();
}

void Method::defineResourceType(Request &req){
	if (S_ISDIR(req.pathStatus.st_mode))
		type = "directory";
	else
		type = "file";
}

void Method::handleDirectory(Request &req){
	handleCgi cgi;
	struct stat status;
	if (!stat(req.path.c_str(), &status) && !(status.st_mode &S_IRUSR)){
		req.statusCode = 403;
		throw std::runtime_error("directory permission dinied");
	}
	if (isDirHasIndexFiles(req) == false)
		autoIndexing(req);
	else{
		req.path += (!req.path.empty() && req.path.at(req.path.length()-1) == '/') ? "" : "/";
		req.path += req.fileName;
		if (stat(req.path.c_str(), &status) != 0){
			req.statusCode = 500;
			throw std::runtime_error("stat failed");
		}
		if (S_ISDIR(status.st_mode))
			handleDirectory(req);
		else{
			validateAll(req);
			if (loacationHasCgi(req, cgi))
				cgi.executeCgiScript(req);
			else
				this->content = readContent(req);
		}
	}
}

bool Method::isDirHasIndexFiles(Request &req) const{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(req.path.c_str()))) {
		while ((ent = readdir(dir)) != NULL) {
			std::string filename = ent->d_name;

			for (std::vector<std::string>::iterator it = req.matchedLocation.index.begin(); it != req.matchedLocation.index.end(); ++it){
				if (filename == *it)  { //compare with index files in location
					std::string path = req.path;
					path += (!req.path.empty() && path.at(path.length()-1) == '/') ? "" : "/";
					path += filename;
					struct stat file;
    				if (stat(path.c_str(), &file) != 0){
						req.statusCode = 500;
						throw std::runtime_error("error stat");
					}
    
    				bool readable = (file.st_mode & S_IRUSR) != 0;
					if (!readable)
						break;
					req.fileName = *it;
					closedir(dir);
					return true;
				}
			}
		}
		closedir(dir);
	} else {
		req.statusCode = 500;
	}

	// No index file found
	return false;
}

void Method::autoIndexing(Request &req){
	(void)req;
	if (req.getMatchedLocation().autoindex == "on")
		directoryListing(req);
	else{
		req.statusCode = 403;
		throw std::runtime_error("403 Forbiden");
	}
}

void Method::directoryListing(Request &req){
	std::stringstream page;

	DIR* directory = opendir(req.path.c_str());
	if (directory == NULL)
	{
		req.statusCode = 500;
		throw std::runtime_error("Error opening directory");
	}

	page << "<html><head><title>Index of " << req.path.c_str() << "</title></head><body><h1>Index of " << req.path.c_str() << "</h1><ul>" << std::endl;

	req.fileName += (!req.fileName.empty() && req.fileName.at(req.fileName.length()-1) == '/') ? "" : "/";

	struct dirent* entry;
	while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			page << "<li><a href=\"" << req.fileName + entry->d_name << "\">" << entry->d_name << "</a></li>" << std::endl;
		}
	}

	page << "</ul></body></html>" << std::endl;
	closedir(directory);
	this->content = page.str();
}



