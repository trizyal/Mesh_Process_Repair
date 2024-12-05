//
// Created by hades on 12/5/24.
//

#include "../utilities/Mesh.h"

#include <iostream>

int main(int argc, char *argv[])
{
    // first we read the file and store the vertices and faces in the TriangleMesh object
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

    int genus = mesh.calculateGenus();

    std::cout << "Genus of the mesh is: " << genus << std::endl;

    return 0;
}
