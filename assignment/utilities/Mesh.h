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
    int face;
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

    // function to read a .tri file and populate the mesh object
    void readTriFile(std::ifstream input_file);


    // TODO: implement the following function
    // function to read a .face file and populate the mesh object
    void readFaceFile(std::ifstream input_file);

    // function to check if the vertex exists in vertices
    // if it does return the index
    // otherwise emplace_back and return the new index
    int findVertex(Cartesian3 vertex);

    // populate the directedEdges vector
    // for each face, create 3 directed edges
    void computeDirectedEdges();

    // populate the firstDirectedEdges vector
    // for each vertex, find the first directed edge
    // stores the index for of the edge in directedEdges for each implicit vertex
    void computeFirstDirectedEdges();

    // populate the otherHalfs vector
    // for each edge, find the other half
    // stores the index for of the edge in directedEdges for each implicit edge
    void computeOtherHalfs();


    // ----------------- MANIFOLD TESTING FUNCTIONS -----------------

    // function to loop through all the edges and check if they have exactly 2 incident faces
    std::vector<int> edgeManifoldTest();

    // counts the faces this edge exists in
    int countIncidentFaces(int edgeIndex);

    // function to loop through all the vertices and check if they have exactly 1 cycle
    std::vector<int> vertexManifoldTest();

    // function to count the number of cycles for a given vertex
    int countCycles(int vertexIndex);

    int countEdges(int vertexIndex);

    // function to get the directed edges from the vertex
    std::vector<DirectedEdges> getConnectedEdges(int vertexIndex);

    // function to get the directed edges index from the vertex
    std::vector<int> getConnectedEdgesIndices(int vertexIndex);
    int getEdgeIndex(int from, int to);


    // ----------------- PRINT FUNCTIONS -----------------
    // std print for console
    // does what .face file should look like
    void printFaceFile();

    int saveFaceFile(std::ofstream output_file);

    int saveObjFile(std::ofstream output_file);

    // std print for console
    // does what .diredge file should look like
    void printDiredgeFile();

    int saveDiredgeFile(std::ofstream output_file);


    void printDirectedEdges()
    {
        for (DirectedEdges edge : directedEdges)
        {
            std::cout << "From: " << edge.from << " To: " << edge.to << " Face: " << edge.face << std::endl;
        }
    }

};



#endif //MESH_H
