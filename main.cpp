#define CROW_MAIN

#include "crow_all.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace crow;
using namespace std;

string username = "Jerin";
string password = "1234";


void sendPage(crow::response& res, const std::string filename) {
    ifstream in("../public/" + filename, ifstream::in);
    if (in) {
        if (filename == "checkout.html") {
            res.code = 402;
        }
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", "text/html");
        res.write(contents.str());
    }
    else {
        res.write("Not Found");
    }
    res.end();
}


//Function to write the data to a file
bool writeData(int quanity, string product) {
    ofstream file("data", ios::app); // from some searching found out about ios::app function which allowes to write to a file or will create one for you if there is none of the name
    if (file.is_open()) {
        file << quanity << " " << product << "\n";
        file.close();
        return true;
    }
    else {
        return false;
    }
}

void sendImage(crow::response& res, const std::string filename) {
    ifstream in("../public/images/" + filename, ifstream::in);
    if (in) {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", "image/jpeg");
        res.write(contents.str());
    }
    else {
        res.write("Not Found");
    }
    res.end();
}

void sendStyle(crow::response& res, const std::string& filename) {
    ifstream in("../public/styles/" + filename, ifstream::in);
    if (in) {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", "text/css");
        res.write(contents.str());
    }
    else {
        res.code = 404;
        res.write("Not Found");
    }
    res.end();
}

void sendScript(crow::response& res, const std::string& filename) {
    ifstream in("../public/scripts/" + filename, ifstream::in);
    if (in) {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", "text/javascript");
        res.write(contents.str());
    }
    else {
        res.code = 404;
        res.write("Not Found");
    }
    res.end();
}


bool readData(crow::response& res) {
    ifstream file("data");
    if (file.is_open()) {
        ostringstream data;
        data << file.rdbuf();
        file.close();

        res.set_header("Content-Type", "text/plain");
        res.write(data.str());
        return true;
    }
    else {
        return false;
    }
    
}




int main()
{
    
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")
        ([&](const request& req, response& res) {
        ifstream in("../public/index.html", ifstream::in);
        if (in) {
            ostringstream contents;
            contents << in.rdbuf();
            in.close();
            res.set_header("Content-Type", "text/html");
            res.write(contents.str());
            }
        else {
            
            res.write("Not Found");
        }
        res.end();
        });

   
    CROW_ROUTE(app, "/<string>")
        ([](const crow::request& req, crow::response& res, std::string filename) {
        
        sendPage(res, filename);
        });


    CROW_ROUTE(app, "/addtocart/<int>/<string>").methods(HTTPMethod::POST)
        ([](const crow::request& req, crow::response& res, int quantity, string product) {
        if (writeData(quantity, product)) {
            
            res.set_header("Location", "/");
            res.code = 303; // See Other
        }
        else {
            res.write("Failed to add product to cart!");
        }
        res.end();
        });
           
    CROW_ROUTE(app, "/login").methods(HTTPMethod::POST,HTTPMethod::GET) 
        ([](const request& req, response& res) {
        if (req.method == HTTPMethod::POST) {

            string uname = req.url_params.get("username");
            string upass = req.url_params.get("password");
            string data = "";

            if (uname == username && upass == password) {
                res.code = 202; 
                res.end();
                
            }
            else {
                res.code = 401; 
                res.write("Invalid username or password");
                res.end();
            }
            
        }
        res.end();
        
        });

    CROW_ROUTE(app, "/display/txt")
        ([](const crow::request& req, crow::response& res) {
         readData(res);
         res.end();
        });

    CROW_ROUTE(app, "/images/<string>")
        ([](const crow::request& req, crow::response& res, const string& filename) {
        sendImage(res, filename);
            });
    CROW_ROUTE(app, "/scripts/<string>")
        ([](const crow::request& req, crow::response& res, std::string filename) {
        sendScript(res, filename);
            });
    CROW_ROUTE(app, "/styles/<string>")
        ([](const crow::request& req, crow::response& res, std::string filename) {
        sendStyle(res, filename);
            });





    // Run the server
    app.port(23500).multithreaded().run();

    return 0;
}
