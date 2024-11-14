//
// Created by hades on 11/14/24.
//

#include "face2faceindex.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input.tri>" << std::endl;
        return 1;
    }

    // Load the input file
    std::string input_filename = argv[1];
    
    // parse the input file
    std::ifstream file(input_filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << input_filename << std::endl;
        return 1;
    }

    std::cout<<"File opened successfully"<<std::endl;

    // print the file
    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }

    return 0;
}
