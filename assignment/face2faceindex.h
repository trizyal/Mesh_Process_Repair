//
// Created by hades on 11/14/24.
//

#ifndef FACE2FACEINDEX_H
#define FACE2FACEINDEX_H

#include "../triangle_renderer/Cartesian3.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class TriangleMesh
{
    public:
    std::string objName;
    int numTriangles;
    int numVertices;
    int numFaces;

    std::vector<Cartesian3> vertices;
    std::vector<Cartesian3> faces;

    // Default constructor for TriangleMesh
    TriangleMesh()
    {
        this->numTriangles = 0;
        this->numVertices = 0;
    }

    // Constructor for TriangleMesh
    TriangleMesh(std::string objName, int numTriangles, std::vector<Cartesian3> vertices)
    {
        this->objName = objName;
        this->numTriangles = numTriangles;
        this->numVertices = vertices.size();
        this->vertices = vertices;
        this->numFaces = numTriangles;
    }

    // print the vertices in a file
    void printObjectName()
    {
        std::cout << "Object Name: " << this->objName << std::endl; 
    }

    int saveFile()
    {
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
            file << "Face " << i << " " << i << std::endl;
        }

        file.close();
        return 0;
    }

};


#endif //FACE2FACEINDEX_H
