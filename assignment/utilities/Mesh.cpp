//
// Created by hades on 11/29/24.
//

#include "Mesh.h"

#include <algorithm>
#include <iomanip>

void Mesh::readTriFile(std::ifstream input_file)
{
    std::string line;

    // read the first line
    std::getline(input_file, line);
    this->numTriangles = std::stoi(line);

    float x, y, z;
    int index1, index2, index3;
    Cartesian3 vertex1, vertex2, vertex3;
    for (int i = 0 ; i < numTriangles ; i++)
    {
        // each triangle has 3 vertices
        // and each vertex will have a xyz to read
        // so we need to read 3 lines for each triangle

        input_file >> x >> y >> z;
        vertex1 = Cartesian3(x, y, z);
        index1 = findVertex(vertex1);

        input_file >> x >> y >> z;
        vertex2 = Cartesian3(x, y, z);
        index2 = findVertex(vertex2);

        input_file >> x >> y >> z;
        vertex3 = Cartesian3(x, y, z);
        index3 = findVertex(vertex3);

        this->faces.push_back({index1, index2, index3});
        this->numFaces++;
    }
    std::cout << "File read successfully" << std::endl;
    computeDirectedEdges();
    std::cout << "Directed Edges computed successfully" << std::endl;
    computeFirstDirectedEdges();
    std::cout << "First Directed Edges computed successfully" << std::endl;
    computeOtherHalfs();
    std::cout << "Other Halfs computed successfully" << std::endl;
}


int Mesh::findVertex(Cartesian3 vertex)
{
    // find the index of the vertex in the vertices vector
    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        if (this->vertices[i].x == vertex.x && this->vertices[i].y == vertex.y && this->vertices[i].z == vertex.z)
            return i;
    }

    this->vertices.push_back(vertex);
    this->numUniqueVertices++;
    return this->numUniqueVertices - 1;
}


void Mesh::computeDirectedEdges()
{
    for (int i = 0 ; i < this->numFaces; i++)
    {
        DirectedEdges edge0{};
        edge0.from = faces[i].face[0];
        edge0.to = faces[i].face[1];
        edge0.face = i;
        this->directedEdges.push_back(edge0);

        DirectedEdges edge1{};
        edge1.from = faces[i].face[1];
        edge1.to = faces[i].face[2];
        edge1.face = i;
        this->directedEdges.push_back(edge1);

        DirectedEdges edge2{};
        edge2.from = faces[i].face[2];
        edge2.to = faces[i].face[0];
        edge2.face = i;
        this->directedEdges.push_back(edge2);
    }
}


void Mesh::computeFirstDirectedEdges()
{
    // for each vertex, find the first directed edge
    // stores the index for of the edge in directedEdges for each implicit vertex
    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        // find the first edge that start from this vertex
        for (int j = 0; j < this->directedEdges.size(); j++)
        {
            if (this->directedEdges[j].from == i)
            {
                this->firstDirectedEdges.push_back(j);
                break;
            }
        }
    }
}


void Mesh::computeOtherHalfs()
{
    // for each edge, in the directedEdges vector, find the other half
    // stores the index for of the edge in directedEdges for each implicit edge
    for (int i = 0; i < this->numFaces * 3; i++)
    {
        for (int j = 0; j < this->directedEdges.size(); j++)
        {
            if (this->directedEdges[j].from == this->directedEdges[i].to && this->directedEdges[j].to == this->directedEdges[i].from)
            {
                this->otherHalfs.push_back(j);
                break;
            }
        }
    }
}


int Mesh::countIncidentFaces(int edgeIndex)
{
    int count = 0;
    if (edgeIndex < 0 || edgeIndex >= this->directedEdges.size())
        return -1;

    int edgeOtherHalf = this->otherHalfs[edgeIndex];

    if (edgeOtherHalf >= 0 && edgeOtherHalf < this->directedEdges.size())
        count++;

    // find how many times this edge appears in the otherHalf vector
    auto c = std::count(this->otherHalfs.begin(), this->otherHalfs.end(), edgeIndex);
    count += c;

    return count;
}



std::vector<int> Mesh::edgeManifoldTest()
{
    // function to loop through all the edges and check if they have exactly 2 incident faces
    std::vector<int> nonManifoldEdges;

    std::cout << "Edge Manifold Test Started" << std::endl;

    for (int i = 0 ; i < this->directedEdges.size(); i++)
    {
        if (countIncidentFaces(i) != 2)
            nonManifoldEdges.push_back(i);
    }

    return nonManifoldEdges;
}


int Mesh::countEdges(int vertexIndex)
{
    int count = 0;
    std::cout<<vertexIndex<<std::endl;
    for (int i = 0; i < this->vertices.size(); i++)
    {
        std::cout<<vertexIndex<<std::endl;
        if (this->directedEdges[i].to == vertexIndex || this->directedEdges[i].from == vertexIndex)
        {
            count++;
            std::cout << "vertex " << vertexIndex << " edges " << count << std::endl;
        }
    }

    return count;
}


std::vector<DirectedEdges> Mesh::getConnectedEdges(int vertexIndex)
{
    std::vector<DirectedEdges> connectedEdges;
    for (int i = 0; i < this->vertices.size(); i++)
    {
        if (this->directedEdges[i].from == vertexIndex)
            connectedEdges.push_back(this->directedEdges[i]);
    }
    return connectedEdges;
}




int Mesh::countCycles(int vertexIndex)
{
    int count = countEdges(vertexIndex);
    return 1;
}



std::vector<int> Mesh::vertexManifoldTest()
{
    std::vector<int> nonManifoldVertices;

    // function to loop through all the vertices and check if they have exactly 1 cycle
    std::cout << "Vertex Manifold Test Started" << std::endl;

    for (int i = 0; i < this->vertices.size(); i++)
    {
        if (countCycles(i) != 1)
            nonManifoldVertices.push_back(i);
    }

    return nonManifoldVertices;
}




void Mesh::printFaceFile()
{
    std::cout << "# University of Leeds 2024-2025" << std::endl;
    std::cout << "# COMP 5893M Assignment 1" << std::endl;
    std::cout << "# Student Name: Tejaswa Rizyal" << std::endl;
    std::cout << "# Student Number: 201484983" << std::endl;
    std::cout << "#"<< std::endl;
    std::cout << "# Object Name: " << this->objName << std::endl;
    std::cout << "# Vertices=" << this->numUniqueVertices <<" Faces=" << this->numFaces <<std::endl;
    std::cout << "#" << std::endl;

    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        std::cout << "Vertex " << i << ": " <<std::fixed<<std::setprecision(4)<< this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    for (int i = 0; i < this->numFaces; i++)
    {
        std::cout << "Face " << i << ": " << this->faces[i].face[0] << " " << this->faces[i].face[1] << " " << this->faces[i].face[2] << std::endl;
    }
}


void Mesh::printDiredgeFile()
{
    std::cout << "# University of Leeds 2024-2025" << std::endl;
    std::cout << "# COMP 5893M Assignment 1" << std::endl;
    std::cout << "# Student Name: Tejaswa Rizyal" << std::endl;
    std::cout << "# Student Number: 201484983" << std::endl;
    std::cout << "#"<< std::endl;
    std::cout << "# Object Name: " << this->objName << std::endl;
    std::cout << "# Vertices=" << this->numUniqueVertices <<" Faces=" << this->numFaces <<std::endl;
    std::cout << "#" << std::endl;

    // write the vertices
    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        std::cout << "Vertex " << i << ": " << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    // write the first directed edge for each vertex
    for (int i = 0 ; i < this->numUniqueVertices; i++)
    {
        std::cout << "FirstDirectedEdge " << i << " " << this->firstDirectedEdges[i] << std::endl;
    }

    // write the faces
    for (int i = 0; i < this->numFaces; i++)
    {
        std::cout << "Face " << i << ": " << this->faces[i].face[0] << " " << this->faces[i].face[1] << " " << this->faces[i].face[2] << std::endl;
    }

    // write the edge other halfs
    for (int i = 0; i < this->numFaces * 3; i++)
    {
        std::cout << "OtherHalf " << i << " " << this->otherHalfs[i] << std::endl;
    }
}

