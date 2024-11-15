//
// Created by hades on 11/14/24.
//

#include "face2faceindex.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

    std::string line;
    
    // read the first line
    std::getline(file, line);

    // store the number of triangles
    int numTriangles = std::stoi(line);

    // store the vertices
    std::vector<Cartesian3> vertices;
    std::vector<Cartesian3> faces;
    for (int i = 0; i < numTriangles; i++)
    {
        std::getline(file, line);
        std::istringstream iss(line);
        float x, y, z;
        iss >> x >> y >> z;
        vertices.push_back(Cartesian3(x, y, z));
    }

    // objName will be the file name without the extension and the path
    std::string objNameStr = input_filename.substr(input_filename.find_last_of("/\\") + 1);
    objNameStr = objNameStr.substr(0, objNameStr.find_last_of("."));

    // create a TriangleMesh object
    TriangleMesh mesh(objNameStr, numTriangles, vertices);

    int check = mesh.saveFile();
    if (check == 1)
    {
        std::cerr << "Error: could not save file" << std::endl;
        return 1;
    }
    
    std::cout << "File saved successfully" << std::endl;    

    return 0;
}
