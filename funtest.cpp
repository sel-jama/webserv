#include <iostream>
#include <string>


using namespace std;
string cutOffBodySegment(std::string &request){
    size_t pos = request.find("\r\n\r\n");
    string bodySaver;
    if (pos == string::npos){
        cerr << "bad request" << endl;
        exit(EXIT_FAILURE);
    }
    pos += 4;
    bodySaver = request.substr(pos);
        // int num = request.length() - pos;
    request.erase(pos, request.length() - 1);
    
    return bodySaver;
}

int main() {
    string req = "GET / HTTP/1.1\r\ncontent_length: 32\r\nHeaders...\r\n\r\nThis is the body part";

    cout << "Request before \n"<< req << endl;
    string body = cutOffBodySegment(req);
    cout << "\nafter\n"<<req << endl;
    cout << "Body :" << body;
    return 0;
}