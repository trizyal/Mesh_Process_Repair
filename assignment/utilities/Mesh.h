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
#include <set>

struct Faces
{
    int face[3];

    bool operator == (const Faces &other) const
    {
        if (face[0] == other.face[0] && face[1] == other.face[1] && face[2] == other.face[2])
            return true;
        if (face[0] == other.face[1] && face[1] == other.face[2] && face[2] == other.face[0])
            return true;
        if (face[0] == other.face[2] && face[1] == other.face[0] && face[2] == other.face[1])
            return true;
        if (face[0] == other.face[0] && face[1] == other.face[2] && face[2] == other.face[1])
            return true;
        if (face[0] == other.face[1] && face[1] == other.face[0] && face[2] == other.face[2])
            return true;
        if (face[0] == other.face[2] && face[1] == other.face[1] && face[2] == other.face[0])
            return true;
        if (face[0] == other.face[2] && face[1] == other.face[0] && face[2] == other.face[1])
            return true;

        return false;
    }
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


    //------------------ FILE READING FUNCTIONS ------------------

    // function to read a .tri file and populate the mesh object
    void readTriFile(std::ifstream input_file);

    // function to read a .face file and populate the mesh object
    void readFaceFile(std::ifstream input_file);



    //------------------ COMPUTING FUNCTIONS ------------------

    // function to check if the vertex exists in vertices
    // if it does return the index
    // otherwise emplace_back and return the new index
    int findSaveVertex(Cartesian3 vertex);

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

    // function to count the number of connected edges for a given vertex
    int countEdges(int vertexIndex) const;

    // function to get the directed edges from the vertex
    std::vector<DirectedEdges> getConnectedEdges(int vertexIndex) const;

    // function to get the directed edges index from the vertex
    std::vector<int> getConnectedEdgesIndices(int vertexIndex) const;

    // function to get the edge index from the 2 vertices
    int getEdgeIndex(int from, int to) const;


    // ----------------- GENUS CALCULATION FUNCTIONS -----------------

    // function to calculate the genus of the mesh
    int calculateGenus() const;


    // ----------------- MESH REPAIR FUNCTIONS -----------------

    // function to return edges with 1 incident faces
    std::vector<int> getUnpairedEdges();

    // function to get edges with no other half
    std::vector<int> getUnpairedEdgesFromOtherHalfs();

    // function to get repeated edges in directedEdges
    std::vector<int> getRepeatedEdges();

    // function to group the unpaired edges into holes
    std::vector<std::vector<int>> getHoles();

    // function to get faces that are equal to a given face
    std::vector<int> getEqualFaces(int faceIndex);

    // function to get edges that are equal to a given edge
    std::vector<int> getEqualEdges(int edgeIndex);

    void repairMesh();

    void removeFaceEdges(int faceIndex);

    // function to remove repeated faces
    void removeRepeatedFaces();

    void removeEqualEdges();

    // function to remove non connected edges
    void removeNonConnectedEdges();

    void removeNonConnectedVertices();

    void removeNotConnectedFaces();

    int getEdgeIndexFromVertex(int vertexIndex, std::vector<int> unpairedEdges, std::set<int> &visitedVertices);



    // ----------------- PRINT AND SAVE FUNCTIONS -----------------

    // std print for console
    // does what .face file should look like
    void printFaceFile() const;

    // save the face file
    int saveFaceFile(std::ofstream output_file) const;

    // save face file as obj file or debugging and blender
    int saveObjFile(std::ofstream output_file) const;

    // std print for console
    // does what .diredge file should look like
    void printDiredgeFile() const;

    // save the diredge file
    int saveDiredgeFile(std::ofstream output_file) const;

    // print the directed edges
    void printDirectedEdges() const;

    // print other halfs
    void printOtherHalfs() const;

    // print manifold testing results
    void printManifoldTestResults();

    int saveManifoldTestResults(std::ofstream output_file);

    // save repaired mesh tri file
    int saveRepairedMesh(std::ofstream output_file) const;

};


#endif //MESH_H
