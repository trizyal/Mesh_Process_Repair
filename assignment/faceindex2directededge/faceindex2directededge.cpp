//
// Created by hades on 11/26/24.
//

#include "../utilities/Mesh.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input.face>" << std::endl;
        return 1;
    }

    // Load the input file
    std::string input_filename = argv[1];
    std::string output = input_filename.substr(input_filename.find_last_of("/\\") + 1);
    output = output.substr(0, output.find_last_of("."));

    // parse the input file
    std::ifstream file(input_filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << input_filename << std::endl;
        return 1;
    }

    std::cout<<"File opened successfully"<<std::endl;

    Mesh mesh;
    mesh.readFaceFile(std::move(file));
    mesh.objName = output;

    file.close();

    std::string filename = "diredgefiles/" + output + ".diredge";

    // create the output directory if it doesn't exist
    std::string output_dir = "diredgefiles";
    std::string command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream outfile(filename.c_str());
    if (!outfile.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    mesh.saveDiredgeFile(std::move(outfile));

    return 0;
}
