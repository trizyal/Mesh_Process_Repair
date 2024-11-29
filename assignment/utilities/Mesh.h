//
// Created by hades on 11/29/24.
//

#ifndef MESH_H
#define MESH_H

#include "../../triangle_renderer/Cartesian3.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>

struct Faces
{
    int face[3];
};

struct DirectedEdges
{
    int from, to;
};

class Mesh {
  public:
    std::string objName;
    int numTriangles;
    int numUniqueVertices;
    int numFaces;

    // vector to store all the unique vertices
    std::vector<Cartesian3> vertices;

    // vector to store all the faces
    std::vector<Faces> faces;

    // vector to store all the directed edges
    std::vector<DirectedEdges> directedEdges;

    // vector to store the first directed edge for each vertex
    // stores the index for of the edge in directedEdges for each implicit vertex
    std::vector<int> firstDirectedEdges;


    // vector to store the other half of the directed edges
    // stores the index for of the edge in directedEdges for each implicit edge
    std::vector<int> otherHalfs;

    // constructor
    Mesh()
    {
        numTriangles = 0;
        numUniqueVertices = 0;
        numFaces = 0;
    }

    // function to read a file and populate the mesh object
    // returns 0 if successful, 1 otherwise
    void readTriFile(std::ifstream input_file);

    // function to check if the vertex exists in vertices
    // if it does return the index
    // otherwise emplace_back and return the new index
    int findVertex(Cartesian3 vertex);

    // populate the directedEdges vector
    void computeDirectedEdges();

    // std print for console
    // does what .face file should look like
    void printFaceFile() const;

};



#endif //MESH_H
