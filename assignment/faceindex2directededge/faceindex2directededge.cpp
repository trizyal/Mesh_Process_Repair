//
// Created by hades on 11/26/24.
//

#include "faceindex2directededge.h"

#include <fstream>
#include <iostream>
#include <sstream>

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

    std::vector<Faces3> faces;
    std::vector<DirectedEdge> directedEdges;
    std::vector<Cartesian3> vertices;

    std::string line;

    int numVertices = 0;
    int numFaces = 0;

    while (std::getline(file, line))
    {
        // check if the line starts with the word "Vertex"
        if (line.find("Vertex") == 0)
        {
            // Vertex 1: 0 0 0
            // read the numbers after the colon
            std::istringstream iss(line);
            // skip till the colon
            iss.ignore(256, ':');
            Cartesian3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }

        // check if the line starts with the word "Face"
        else if (line.find("Face") == 0)
        {
            // Face 1: 0 1 2
            // read the numbers after the colon
            std::istringstream iss(line);
            // skip till the colon
            iss.ignore(256, ':');
            Faces3 face{};
            iss >> face.face[0] >> face.face[1] >> face.face[2];
            // std::cout<<face.face[0]<<" "<<face.face[1]<<" "<<face.face[2]<<std::endl;

            faces.push_back(face);
        }
    }

    file.close();

    computeDirectedEdges(faces, directedEdges);

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

    outfile << "# University of Leeds 2024-2025" << std::endl;
    outfile << "# COMP 5893M Assignment 1" << std::endl;
    outfile << "# Student Name: Tejaswa Rizyal" << std::endl;
    outfile << "# Student Number: 201484983" << std::endl;
    outfile << "#"<< std::endl;
    outfile << "# Object Name: " << output << std::endl;
    outfile << "# Vertices=" << vertices.size() <<" Faces=" << faces.size() <<std::endl;
    outfile << "#" << std::endl;

    // write the vertices
    for (int i = 0; i < vertices.size(); i++)
    {
        outfile << "Vertex " << i << ": " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    }

    // write the first directed edge for each vertex
    for (int i = 0; i < vertices.size(); i++)
    {
        // find the first edge that start from this vertex
        for (int j = 0; j < directedEdges.size(); j++)
        {
            if (directedEdges[j].from == i)
            {
                outfile << "FirstDirectedEdge " << i << " " << j << std::endl;
                break;
            }
        }
    }

    // write the faces
    for (int i = 0; i < faces.size(); i++)
    {
        outfile << "Face " << i << ": " << faces[i].face[0] << " " << faces[i].face[1] << " " << faces[i].face[2] << std::endl;
    }

    // write the edge other halfs
    for (int i = 0; i < faces.size() * 3; i++)
    {
        for (int j = 0; j < directedEdges.size(); j++)
        {
            if (directedEdges[j].from == directedEdges[i].to && directedEdges[j].to == directedEdges[i].from)
            {
                outfile << "OtherHalf " << i << " " << j << std::endl;
                break;
            }
        }
    }

    return 0;
}


void computeDirectedEdges(const std::vector<Faces3>& faces, std::vector<DirectedEdge>& directedEdges)
{
    for (const Faces3& face : faces)
    {
        DirectedEdge edge1(face.face[0], face.face[1]);
        DirectedEdge edge2(face.face[1], face.face[2]);
        DirectedEdge edge3(face.face[2], face.face[0]);


        directedEdges.push_back(edge1);
        directedEdges.push_back(edge2);
        directedEdges.push_back(edge3);
    }
}

