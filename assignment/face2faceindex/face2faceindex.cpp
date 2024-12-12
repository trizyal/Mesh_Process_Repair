//
// Created by hades on 11/14/24.
//

#include "../utilities/Mesh.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input.tri>" << std::endl;
        return 1;
    }

    // Load the input file
    std::string input_filename = argv[1];

    std::string obj_name = input_filename.substr(input_filename.find_last_of("/\\") + 1);
    obj_name = obj_name.substr(0, obj_name.find_last_of("."));

    
    // parse the input file
    std::ifstream file(input_filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << input_filename << std::endl;
        return 1;
    }

    std::cout<<"File opened successfully"<<std::endl;

    Mesh mesh;
    mesh.readTriFile(std::move(file));
    mesh.objName = obj_name;

    file.close();

    std::string filename = "facefiles/" + obj_name + ".face";

    // create the output directory if it doesn't exist
    std::string output_dir = "facefiles";
    std::string command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream outfile(filename.c_str());
    if (!outfile.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    mesh.saveFaceFile(std::move(outfile));
    outfile.close();

    std::cout<<"File saved successfully"<<std::endl;

/*
    filename = "objfiles/" + obj_name + ".obj";

    output_dir = "objfiles";
    command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream outfile_obj(filename.c_str());
    if (!outfile_obj.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    mesh.saveObjFile(std::move(outfile_obj));
    outfile_obj.close();
*/

    return 0;
}