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
}


int Mesh::findVertex(const Cartesian3 vertex)
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
    for (const Faces& face : this->faces)
    {
        DirectedEdges edge0{};
        edge0.from = face.face[0];
        edge0.to = face.face[1];
        this->directedEdges.push_back(edge0);

        DirectedEdges edge1{};
        edge1.from = face.face[1];
        edge1.to = face.face[2];
        this->directedEdges.push_back(edge1);

        DirectedEdges edge2{};
        edge2.from = face.face[2];
        edge2.to = face.face[0];
        this->directedEdges.push_back(edge2);

    }
}


void Mesh::printFaceFile() const
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
