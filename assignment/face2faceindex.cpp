//
// Created by hades on 11/14/24.
//

#include "face2faceindex.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <algorithm>

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
    // std::vector<Cartesian3> faces;
    std::vector<Faces3> faces;

    for (int i = 0; i < numTriangles; i++)
    {
        std::getline(file, line);
        std::istringstream iss(line);
        float x, y, z;
        iss >> x >> y >> z;
        // vertices.push_back(Cartesian3(x, y, z));
        
        Cartesian3 vertex(x, y, z);

        // print the vertex
        // std::cout << "Vertex " << i << ": " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;

        vertices.push_back(vertex);

        Faces3 f;
        
        // the first 3 vertices are the first face
        if (faces.empty() == true)
        {
            if (vertices.size() == 3)
            {
                f.face[0] = 0;
                f.face[1] = 1;
                f.face[2] = 2;
                faces.push_back(f);
            }
        }
        else 
        {
            auto pos = std::find(vertices.begin(), vertices.end(), vertex);
            if (pos != vertices.end())
            {
                auto index = pos - vertices.begin();

                if (i % 3 == 0)
                {
                    f.face[0] = index;
                }
                else if (i % 3 == 1)
                {
                    f.face[1] = index;
                }
                else if (i % 3 == 2)
                {
                    f.face[2] = index;
                    faces.push_back(f);
                }
                
            }
        }
        
        
    }

    // objName will be the file name without the extension and the path
    std::string objNameStr = input_filename.substr(input_filename.find_last_of("/\\") + 1);
    objNameStr = objNameStr.substr(0, objNameStr.find_last_of("."));

    // create a TriangleMesh object
    TriangleMesh mesh(objNameStr, numTriangles, vertices, faces);

    int check = mesh.saveFile();
    if (check == 1)
    {
        std::cerr << "Error: could not save file" << std::endl;
        return 1;
    }
    
    std::cout << "File saved successfully" << std::endl;    

    return 0;
}

int TriangleMesh::saveFile() const
{
    std::cout<<"reached saveFile"<<std::endl;

    std::string filename = "output/" + this->objName + ".face";

    // create the output directory if it doesn't exist
    std::string output_dir = "output";
    std::string command = "mkdir -p " + output_dir;
    system(command.c_str());

    std::ofstream file(filename.c_str());
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    file << "# University of Leeds 2024-2025" << std::endl;
    file << "# COMP 5893M Assignment 1" << std::endl;
    file << "# Student Name: Tejaswa Rizyal" << std::endl;
    file << "# Student Number: 201484983" << std::endl;
    file << "#"<< std::endl;
    file << "# Object Name: " << this->objName << std::endl;
    file << "# Vertices: " << this->numVertices << std::endl;
    file << "#" << std::endl;

    for (int i = 0; i < this->numVertices; i++)
    {
        file << "Vertex " << i << " " << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    for (int i = 0; i < this->numFaces; i++)
    {
        file << "Face " << i << " " << this->faces[i].face[0] << " " << this->faces[i].face[1] << " " << this->faces[i].face[2] << std::endl;
    }

    file.close();
    return 0;
}
