#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{

    YAML::Node config = YAML::LoadFile("config.yaml");
    
    const std::string username = config["username"].as<std::string>();
    const std::string password = config["password"].as<std::string>();
    const std::string test1 = config["block1"]["test1"].as<std::string>();
    std::cout<<"user name is " << username << std::endl;    
    std::cout<<"password is " << password << std::endl;    
    std::cout<<"block1->test1 is " << test1 << std::endl;    
    return 0;
}
