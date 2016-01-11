#include <string>
#include <iostream>
#include <cstdio>
#include <memory>

std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

int main(){
	std::cout<<"0 -\n";
	std::cout<<exec("connmanctl disable wifi");
	std::cout<<"1 -\n";
	std::cout<<exec("connmanctl enable wifi");
	std::cout<<"2 -\n";
	std::cout<<exec("connmanctl scan wifi");
	std::cout<<"3 -\n";

	std::string wifi_connection = exec("connmanctl services | grep linksys");

	wifi_connection.erase(0,25);

	std::cout<<wifi_connection;
	std::cout<<"4 -\n";
	return 0;
}
