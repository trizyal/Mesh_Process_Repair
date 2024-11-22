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
#include <array>

struct Faces3
{
    int face[3];
};

class TriangleMesh
{
    public:
    std::string objName;
    int numTriangles;
    int numVertices;
    int numFaces;

    std::vector<Cartesian3> vertices;
    std::vector<Faces3> faces;

    // Default constructor for TriangleMesh
    TriangleMesh()
    {
        this->numTriangles = 0;
        this->numVertices = 0;
        this->numFaces = 0;
    }

    // Constructor for TriangleMesh
    TriangleMesh(std::string objName, int numTriangles, std::vector<Cartesian3> vertices, std::vector<Faces3> faces)
    {
        this->objName = objName;
        this->numTriangles = numTriangles;
        this->numVertices = vertices.size();
        this->vertices = vertices;
        this->numFaces = numTriangles;
        this->faces = faces;

    }

    // print the vertices in a file
    void printObjectName()
    {
        std::cout << "Object Name: " << this->objName << std::endl; 
    }

    int saveFile() const;

};


#endif //FACE2FACEINDEX_H
