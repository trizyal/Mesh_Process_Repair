//
// Created by hades on 11/29/24.
//

#include "Mesh.h"

#include <algorithm>
#include <assert.h>
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


void Mesh::readFaceFile(std::ifstream input_file)
{
    std::string line;

    int numVertices = 0;
    int numFaces = 0;

    while (std::getline(input_file, line))
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
            numVertices++;
        }

        // check if the line starts with the word "Face"
        else if (line.find("Face") == 0)
        {
            // Face 1: 0 1 2
            // read the numbers after the colon
            std::istringstream iss(line);
            // skip till the colon
            iss.ignore(256, ':');
            Faces face{};
            iss >> face.face[0] >> face.face[1] >> face.face[2];
            faces.push_back(face);
            numFaces++;
        }
    }

    this->numTriangles = numFaces;
    this->numUniqueVertices = numVertices;
    this->numFaces = numFaces;

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
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == vertexIndex)
        {
            count++;
        }
    }

    return count;
}


std::vector<DirectedEdges> Mesh::getConnectedEdges(int vertexIndex)
{
    std::vector<DirectedEdges> connectedEdges;
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == vertexIndex)
            connectedEdges.push_back(this->directedEdges[i]);
    }
    return connectedEdges;
}


std::vector<int> Mesh::getConnectedEdgesIndices(int vertexIndex)
{
    std::vector<int> connectedEdges;
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == vertexIndex)
            connectedEdges.push_back(i);
    }
    return connectedEdges;
}

int Mesh::getEdgeIndex(int from, int to)
{
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == from && this->directedEdges[i].to == to)
            return i;
    }
    return -1;
}


int Mesh::countCycles(int vertexIndex)
{
    int cycles = 0;

    int countConnectedEdges = countEdges(vertexIndex);
    if (countConnectedEdges == 0)
    {
        return 0;
    }

    std::vector<DirectedEdges> connectedEdges = getConnectedEdges(vertexIndex);
    std::vector<int> connectedEdgesIndices = getConnectedEdgesIndices(vertexIndex);

    // now that we have the connected edges
    // we can count cycles
    // we will start from the first edge

    DirectedEdges firstEdge = connectedEdges[0];
    int firstEdgeIndex = connectedEdgesIndices[0];
    int firstOutGoingVertex = firstEdge.to;
    int faceIndex = firstEdge.face;

    Faces currentFace = this->faces[faceIndex];

    int nextVertex = firstOutGoingVertex; // for now

    for (int vertex : currentFace.face)
    {
        if (vertex != vertexIndex && vertex != firstOutGoingVertex)
        {
            nextVertex = vertex;
            break;
        }
    }

    int nextEdgeIndex = getEdgeIndex(vertexIndex, nextVertex);

    int count = 1;

    while (nextEdgeIndex != firstEdgeIndex)
    {
        DirectedEdges nextEdge = this->directedEdges[nextEdgeIndex];
        int nextFaceIndex = nextEdge.face;
        Faces nextFace = this->faces[nextFaceIndex];

        // int nextVertex = -1;
        for (int vertex : nextFace.face)
        {
            if (vertex != vertexIndex && vertex != nextEdge.to)
            {
                nextVertex = vertex;
                break;
            }
        }

        nextEdgeIndex = getEdgeIndex(vertexIndex, nextVertex);
        count++;

        if (count > countConnectedEdges)
            break;
    }

    // std::cout << "edges counted: " << count << std::endl;
    if (count == countConnectedEdges)
        cycles = 1;
    if (count > countConnectedEdges)
        cycles = 2;





    // std::cout << "Cycles: " << cycles << std::endl;
    return cycles;
}



std::vector<int> Mesh::vertexManifoldTest()
{
    std::vector<int> nonManifoldVertices;

    // function to loop through all the vertices and check if they have exactly 1 cycle
    std::cout << "Vertex Manifold Test Started" << std::endl;
    // std::cout << "Vertices: " << this->vertices.size() << std::endl;

    for (int i = 0; i < this->vertices.size(); i++)
    {
        std::cout << "Vertex: " << i << std::endl;
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


int Mesh::saveFaceFile(std::ofstream output_file)
{
    if (!output_file.is_open())
    {
        std::cerr << "Error: could not open output file " << std::endl;
        return 1;
    }

    output_file << "# University of Leeds 2024-2025" << std::endl;
    output_file << "# COMP 5893M Assignment 1" << std::endl;
    output_file << "# Student Name: Tejaswa Rizyal" << std::endl;
    output_file << "# Student Number: 201484983" << std::endl;
    output_file << "#"<< std::endl;
    output_file << "# Object Name: " << this->objName << std::endl;
    output_file << "# Vertices=" << this->numUniqueVertices <<" Faces=" << this->numFaces <<std::endl;
    output_file << "#" << std::endl;

    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        output_file << "Vertex " << i << ": " <<std::fixed<<std::setprecision(4)<< this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    for (int i = 0; i < this->numFaces; i++)
    {
        output_file << "Face " << i << ": " << this->faces[i].face[0] << " " << this->faces[i].face[1] << " " << this->faces[i].face[2] << std::endl;
    }

    output_file.close();
    return 0;
}


int Mesh::saveObjFile(std::ofstream output_file)
{
    if (!output_file.is_open())
    {
        std::cerr << "Error: could not open output file " << std::endl;
        return 1;
    }

    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        output_file << "v" <<std::setprecision(4) << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    for (int i = 0 ; i < this->numFaces ; i++)
    {
        output_file << "f " << this->faces[i].face[0] + 1 << " " << this->faces[i].face[1] + 1 << " " << this->faces[i].face[2] + 1 << std::endl;
    }

    return 0;
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


int Mesh::saveDiredgeFile(std::ofstream output_file)
{
    if (!output_file.is_open())
    {
        std::cerr << "Error: could not open output file " << std::endl;
        return 1;
    }

    output_file << "# University of Leeds 2024-2025" << std::endl;
    output_file << "# COMP 5893M Assignment 1" << std::endl;
    output_file << "# Student Name: Tejaswa Rizyal" << std::endl;
    output_file << "# Student Number: 201484983" << std::endl;
    output_file << "#"<< std::endl;
    output_file << "# Object Name: " << this->objName << std::endl;
    output_file << "# Vertices=" << this->numUniqueVertices <<" Faces=" << this->numFaces <<std::endl;
    output_file << "#" << std::endl;


    // write the vertices
    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        output_file << "Vertex " << i << ": " << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    // write the first directed edge for each vertex
    for (int i = 0 ; i < this->numUniqueVertices; i++)
    {
        output_file << "FirstDirectedEdge " << i << " " << this->firstDirectedEdges[i] << std::endl;
    }

    // write the faces
    for (int i = 0; i < this->numFaces; i++)
    {
        output_file << "Face " << i << ": " << this->faces[i].face[0] << " " << this->faces[i].face[1] << " " << this->faces[i].face[2] << std::endl;
    }

    // write the edge other halfs
    for (int i = 0; i < this->numFaces * 3; i++)
    {
        output_file << "OtherHalf " << i << " " << this->otherHalfs[i] << std::endl;
    }

    return 0;
}


