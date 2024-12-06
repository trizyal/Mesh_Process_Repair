//
// Created by hades on 11/29/24.
//

#include "Mesh.h"

#include <algorithm>
#include <iomanip>


//----------------------------------------------------------------------------------
//
// functions to read the files
//
//----------------------------------------------------------------------------------

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
        index1 = findSaveVertex(vertex1);

        input_file >> x >> y >> z;
        vertex2 = Cartesian3(x, y, z);
        index2 = findSaveVertex(vertex2);

        input_file >> x >> y >> z;
        vertex3 = Cartesian3(x, y, z);
        index3 = findSaveVertex(vertex3);

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



//----------------------------------------------------------------------------------
//
// functions to compute different data
//
//----------------------------------------------------------------------------------

int Mesh::findSaveVertex(Cartesian3 vertex)
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
    // initialize the otherHalfs vector
    this->otherHalfs = std::vector<int>(this->numFaces * 3, -1);


    // for each edge, in the directedEdges vector, find the other half
    // stores the index for of the edge in directedEdges for each implicit edge
    for (int i = 0; i < this->numFaces * 3; i++)
    {
        for (int j = 0; j < this->numFaces * 3; j++)
        {
            if (this->directedEdges[j].from == this->directedEdges[i].to && this->directedEdges[j].to == this->directedEdges[i].from)
            {
                // this->otherHalfs.push_back(j);
                this->otherHalfs[i] = j;
                break;
            }
        }
    }
}


//----------------------------------------------------------------------------------
//
// functions to test for manifold
//
//----------------------------------------------------------------------------------


int Mesh::countIncidentFaces(int edgeIndex)
{
    int count = 1;
    if (edgeIndex < 0 || edgeIndex >= this->directedEdges.size())
        return -1;

    int edgeOtherHalf = this->otherHalfs[edgeIndex];

    if (edgeOtherHalf >= 0 && edgeOtherHalf < this->directedEdges.size())
        count++;

    // find how many times this edge appears in the otherHalf vector
    // auto c = std::count(this->otherHalfs.begin(), this->otherHalfs.end(), edgeIndex);
    // count += c;

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



int Mesh::countEdges(const int vertexIndex) const
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



std::vector<DirectedEdges> Mesh::getConnectedEdges(const int vertexIndex) const
{
    std::vector<DirectedEdges> connectedEdges;
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == vertexIndex)
            connectedEdges.push_back(this->directedEdges[i]);
    }
    return connectedEdges;
}



std::vector<int> Mesh::getConnectedEdgesIndices(const int vertexIndex) const
{
    std::vector<int> connectedEdges;
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == vertexIndex)
            connectedEdges.push_back(i);
    }
    return connectedEdges;
}



int Mesh::getEdgeIndex(const int from, const int to) const
{
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (this->directedEdges[i].from == from && this->directedEdges[i].to == to)
            return i;
    }
    return -1;
}



int Mesh::countCycles(const int vertexIndex)
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

    return cycles;
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




//----------------------------------------------------------------------------------
//
// functions to calculate the genus
//
//----------------------------------------------------------------------------------

int Mesh::calculateGenus() const
{
    // Eulers formula for a 2-manifold

    int X = this->numUniqueVertices - static_cast<int>(this->directedEdges.size()/2) + this->numFaces;

    int genus = std::max((2-X)/2, 0);

    return genus;
}



// ----------------------------------------------------------------------------------
//
// functions to repair the mesh
//
// ----------------------------------------------------------------------------------

std::vector<int> Mesh::getEqualFaces(int faceIndex)
{
    std::vector<int> equalFaces;
    Faces face = this->faces[faceIndex];

    for (int i = 0; i < this->numFaces; i++)
    {
        if (i == faceIndex)
            continue;

        Faces otherFace = this->faces[i];

        if (face == otherFace)
            equalFaces.push_back(i);
    }
    return equalFaces;
}


std::vector<int> Mesh::getEqualEdges(int edgeIndex)
{
    std::vector<int> equalEdges;
    DirectedEdges edge = this->directedEdges[edgeIndex];

    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (i == edgeIndex)
            continue;

        DirectedEdges otherEdge = this->directedEdges[i];

        if (edge.from == otherEdge.from && edge.to == otherEdge.to)
            equalEdges.push_back(i);
    }
    return equalEdges;
}



// edges with 1 incident faces
std::vector<int> Mesh::getUnpairedEdges()
{
    std::vector<int> unpairedEdges;

    std::cout << "Unpaired Edges Test Started" << std::endl;

    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        if (countIncidentFaces(i) == 1)
            unpairedEdges.push_back(i);
    }

    return unpairedEdges;
}



// edges with no other half
std::vector<int> Mesh::getUnpairedEdgesFromOtherHalfs()
{
    std::vector<int> unpairedEdges;
    std::set<int> visitedEdges;

    std::cout << "Other Half Unpaired Edges Test Started" << std::endl;

    for (int i = 0; i < this->otherHalfs.size(); i++)
    {
        int index = this->otherHalfs[i];

        if (this->otherHalfs[i] == -1)
            unpairedEdges.push_back(i);

        visitedEdges.insert(index);
    }

    return unpairedEdges;
}


// edges that are repeated in the otherHalfs
std::vector<int> Mesh::getRepeatedEdges()
{
    std::vector<int> repeatedEdges;
    std::set<int> visitedEdges;

    for (int i = 0; i < this->otherHalfs.size(); i++)
    {
        if (this->otherHalfs[i] == -1)
            continue;

        int index = this->otherHalfs[i];

        if (visitedEdges.count(index) == 1)
            repeatedEdges.push_back(i);

        visitedEdges.insert(index);
    }

    return repeatedEdges;
}


void Mesh::removeEqualEdges()
{
    std::set<int> removedEdges;
    for (int i = 0 ; i < directedEdges.size() ; i++)
    {
        if (removedEdges.count(i) == 1)
        {
            std::cout << "Edge " << i << " already removed" << std::endl;
            continue;
        }

        std::vector<int> equalEdges = getEqualEdges(i);

        if (equalEdges.empty())
            continue;

        for (int equalEdge : equalEdges)
        {
            std::cout << "edge " << i << " equal to edge " << equalEdge << std::endl;
            removedEdges.insert(equalEdge);

            std::cout << "Removing edge (removeEqualEdges()) " << equalEdge << std::endl;
            // remove the edge
            directedEdges.erase(directedEdges.begin() + equalEdge);
            otherHalfs.erase(otherHalfs.begin() + equalEdge);
        }
    }
}



void Mesh::removeFaceEdges(int faceIndex)
{
    int previousSize = directedEdges.size();
    for (int i = 0; i < previousSize; i++)
    {
        if (this->directedEdges[i].face == faceIndex)
        {
            std::cout << "Removing edge " << i << std::endl;
            this->directedEdges.erase(this->directedEdges.begin() + i);
            this->otherHalfs.erase(this->otherHalfs.begin() + i);
        }
    }

    for (int i = 0 ; i < otherHalfs.size() ; i++)
    {
        if (this->otherHalfs[i]  >= directedEdges.size())
        {
            std::cout << "Setting other half -1 for " << i << std::endl;
            this->otherHalfs[i] = -1;
        }
    }
}



void Mesh::removeRepeatedFaces()
{
    std::set<int> removedFaces;
    for (int i = 0; i < this->numFaces; i++)
    {
        if (removedFaces.count(i) == 1)
        {
            std::cout << "Face " << i << " already removed" << std::endl;
            continue;
        }

        std::vector<int> equalFaces = getEqualFaces(i);

        if (equalFaces.empty())
            continue;

        for (int equalFace : equalFaces)
        {
            std::cout << "Removing face " << equalFace << std::endl;
            this->faces.erase(this->faces.begin() + equalFace);
            removeFaceEdges(equalFace);
            removedFaces.insert(equalFace);
            this->numFaces--;
        }
    }
}




std::vector<std::vector<int>> Mesh::getHoles()
{
    std::set<int> visitedEdges;
    std::vector<std::vector<int>> holes;
    std::vector<int> unpairedEdges = getUnpairedEdges(); // 1 incident face
    std::vector<int> unpairedEdgesOtherHalfs = getUnpairedEdgesFromOtherHalfs(); // no other half
    std::vector<int> repeatedEdges = getRepeatedEdges(); // repeats in otherHalfs



    holes.push_back(unpairedEdgesOtherHalfs);

    return holes;
}


void Mesh::repairMesh()
{
    removeRepeatedFaces();
    // removeEqualEdges();

    // printDirectedEdges();
    // printOtherHalfs();

    printDirectedEdges();
    printOtherHalfs();

    std::vector<std::vector<int>> holes = getHoles();

    std::vector<int> hole = holes[0];

    for (int edge : hole)
    {
        std::cout << "Edge " << edge << std::endl;
    }

    if (hole.size() == 3)
    {
        int edge0Index = hole[0];
        int edge1Index = hole[1];
        int edge2Index = hole[2];

        DirectedEdges edge0 = this->directedEdges[edge0Index];
        DirectedEdges edge1 = this->directedEdges[edge1Index];
        DirectedEdges edge2 = this->directedEdges[edge2Index];

        // the vertices of the hole
        int vertex0 = edge0.from;
        int vertex1 = edge1.from;
        int vertex2 = edge2.from;

        DirectedEdges newEdge0 = {vertex0, vertex1, this->numFaces};
        DirectedEdges newEdge1 = {vertex1, vertex2, this->numFaces};
        DirectedEdges newEdge2 = {vertex2, vertex0, this->numFaces};

        this->directedEdges.push_back(newEdge0);
        this->directedEdges.push_back(newEdge1);
        this->directedEdges.push_back(newEdge2);

        this->faces.push_back({vertex0, vertex1, vertex2});
        this->numFaces++;
    }
    else
    {

    }

    this->directedEdges.clear();
    this->firstDirectedEdges.clear();
    this->otherHalfs.clear();

    computeDirectedEdges();
    computeFirstDirectedEdges();
    computeOtherHalfs();

    printDirectedEdges();
    printOtherHalfs();
}





//--------------------------------------------------------------------------------------------
//
// functions to print and save the files
//
//--------------------------------------------------------------------------------------------



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


int Mesh::saveFaceFile(std::ofstream output_file) const
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


int Mesh::saveObjFile(std::ofstream output_file) const
{
    if (!output_file.is_open())
    {
        std::cerr << "Error: could not open output file " << std::endl;
        return 1;
    }

    for (int i = 0; i < this->numUniqueVertices; i++)
    {
        output_file << "v " <<std::setprecision(4) << this->vertices[i].x << " " << this->vertices[i].y << " " << this->vertices[i].z << std::endl;
    }

    for (int i = 0 ; i < this->numFaces ; i++)
    {
        output_file << "f " << this->faces[i].face[0] + 1 << " " << this->faces[i].face[1] + 1 << " " << this->faces[i].face[2] + 1 << std::endl;
    }

    return 0;
}




void Mesh::printDiredgeFile() const
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


int Mesh::saveDiredgeFile(std::ofstream output_file) const
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


void Mesh::printDirectedEdges() const
{
    for (int i = 0; i < this->directedEdges.size(); i++)
    {
        std::cout << "Directed Edge " << i << ": " << this->directedEdges[i].from << " " << this->directedEdges[i].to << " " << this->directedEdges[i].face << std::endl;
    }
}


void Mesh::printOtherHalfs() const
{
    for (int i = 0; i < this->numFaces * 3; i++)
    {
        std::cout << "Other Half " << i << ": " << this->otherHalfs[i] << std::endl;
    }
}


void Mesh::printManifoldTestResults()
{
    std::vector<int> nonManifoldEdges = edgeManifoldTest();
    std::vector<int> nonManifoldVertices = vertexManifoldTest();

    if (nonManifoldEdges.empty() && nonManifoldVertices.empty())
        std::cout << "\nThe mesh is 2-manifold" << std::endl;
    else
    {
        std::cout << "The mesh is not 2-manifold" << std::endl;
        std::cout << "Non manifold edges and vertices: " << std::endl;
        for (const int edge : nonManifoldEdges)
        {
            std::cout << "Edge " << edge << std::endl;
        }
        for (const int vertex : nonManifoldVertices)
        {
            std::cout << "Vertex " << vertex << std::endl;
        }
    }
}


int Mesh::saveManifoldTestResults(std::ofstream output_file)
{
    if (!output_file.is_open())
    {
        std::cerr << "Error: could not open output file " << std::endl;
        return 1;
    }

    output_file << "# Manifold testing results for " << this->objName << ".tri" << std::endl;

    std::vector<int> nonManifoldEdges = edgeManifoldTest();
    std::vector<int> nonManifoldVertices = vertexManifoldTest();

    if (nonManifoldEdges.empty() && nonManifoldVertices.empty())
        output_file << "\nThe mesh is 2-manifold" << std::endl;
    else
    {
        output_file << "The mesh is not 2-manifold" << std::endl;
        output_file << "Non manifold edges and vertices: " << std::endl;
        for (const int edge : nonManifoldEdges)
        {
            output_file << "Edge " << edge << std::endl;
        }
        for (const int vertex : nonManifoldVertices)
        {
            output_file << "Vertex " << vertex << std::endl;
        }
    }

    return 0;
}

int Mesh::saveRepairedMesh(std::ofstream output_file) const
{
    // save the repaired mesh as a .tri file
    if (!output_file.is_open())
    {
        std::cerr << "Error: could not open output file " << std::endl;
        return 1;
    }

    output_file << this->numTriangles << std::endl;

    for (Faces face : this->faces)
    {
        output_file << this->vertices[face.face[0]].x << " " << this->vertices[face.face[0]].y << " " << this->vertices[face.face[0]].z << std::endl;
        output_file << this->vertices[face.face[1]].x << " " << this->vertices[face.face[1]].y << " " << this->vertices[face.face[1]].z << std::endl;
        output_file << this->vertices[face.face[2]].x << " " << this->vertices[face.face[2]].y << " " << this->vertices[face.face[2]].z << std::endl;
    }
    return 0;
}






