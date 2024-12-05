//
// Created by hades on 11/29/24.
//

#include "manifoldtesting.h"
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

    // mesh.printFaceFile();
    // mesh.printDiredgeFile();
    // mesh.printDirectedEdges();


    // we calculate the edges and directed edges and otherHalfs and store them in the Mesh objects

    // for manifold testing
    // A surface mesh is 2-manifold iff:
    // 1. all edges have exactly 2 incident faces
    // 2. no pinch points at vertices
    //     - i.e. single cycle around each vertex
    // 3. no self-intersections

    // print non manifold edges
    mesh.printManifoldTestResults();

    // save the results to a file
    std::string filename = "manifold_test_results/"+ obj_name + ".txt";

    std::string output_dir = "manifold_test_results";
    std::string command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream outfile(filename.c_str());
    if (!outfile.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    mesh.saveManifoldTestResults(std::move(outfile));
    outfile.close();

    return 0;

}