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

    // manifold test
    /*
    std::vector<int> nonManifoldEdges = mesh.edgeManifoldTest();
    std::vector<int> nonManifoldVertices = mesh.vertexManifoldTest();

    if (nonManifoldEdges.empty() && nonManifoldVertices.empty())
    {
        std::cout << "\nThe mesh is 2-manifold" << std::endl;
        std::cout << "Genus: " << mesh.calculateGenus() << std::endl;
        std::cout << "No need to repair the mesh" << std::endl;
        return 0;
    }
    */

    mesh.repairMesh();

    std::string filename = "objfiles/" + obj_name + "_repaired.obj";

    std::string output_dir = "objfiles";
    std::string command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream outfile_obj(filename.c_str());
    if (!outfile_obj.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    mesh.saveObjFile(std::move(outfile_obj));
    outfile_obj.close();

    filename = "trifiles/" + obj_name + "_repaired.tri";
    output_dir = "trifiles";
    command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream outfile_tri(filename.c_str());
    if (!outfile_tri.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    mesh.saveRepairedMesh(std::move(outfile_tri));
    outfile_tri.close();

    return 0;
}
