#pragma once
#include "Group.h"
#include "Tuple.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

class ObjParser{
public:
    // Group of shapes produced by obj file
    std::vector<Group*> groups{(new Group)};
    // Tracks index of which group a shape in the obj file should be added to based on the g prefix
    int groupsInd = 0;
    // Stores xyz coordinates prefixed by v in obj files
    std::vector<Point> vertices;
    // Stores xyz coordinates prefixed by vn in obj files, for smooth triangles
    std::vector<Vector> normals;

    // Constructor
    ObjParser(std::string file_path_or_content, bool isFile);

    // When f prefix in obj file provides more than 3 vertices,
    // uses the list of vertices provided to generate multiple triangles
    void fanTriangulation(std::vector<int> vertice_inds);

    // Concatenates all groups generated into one larger group
    Group* objToGroup();
};