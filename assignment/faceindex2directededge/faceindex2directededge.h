//
// Created by hades on 11/26/24.
//

#ifndef FACEINDEX2DIRECTEDEDGE_H
#define FACEINDEX2DIRECTEDEDGE_H

#include "../../triangle_renderer/Cartesian3.h"

#include <vector>
#include <string>
#include <iostream>

struct Faces3
{
    int face[3];
};

struct DirectedEdge
{
    int from, to;

    // Default constructor for DirectedEdge
    DirectedEdge()
    {
        this->from = -1;
        this->to = -1;
    }

    // Parameterized constructor for DirectedEdge
    DirectedEdge(int from, int to)
    {
        this->from = from;
        this->to = to;
    }
};

struct OtherHalf
{
    int edgeIndex;
    int otherHalfIndex;
};



void computeDirectedEdges(const std::vector<Faces3>& faces, std::vector<DirectedEdge> &directedEdges);




#endif //FACEINDEX2DIRECTEDEDGE_H
