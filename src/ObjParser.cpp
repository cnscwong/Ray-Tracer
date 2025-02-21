#include "ObjParser.h"

// .obj file parser, takes a string or file path based on isFile
ObjParser::ObjParser(std::string file_path_or_content, bool isFile){
    std::istringstream content_stream(file_path_or_content);
    std::ifstream file_contents(file_path_or_content);

    if(isFile && !file_contents){
        throw std::invalid_argument("Obj file does not exist");
    }

    std::string line;
    std::string x_str, y_str, z_str;
    float x, y, z;
    while (isFile ? std::getline(file_contents, line):std::getline(content_stream, line)){
        std::istringstream str_stream(line);
        std::string prefix;
        str_stream >> prefix;   

        // Parsing a vertex in the obj file
        if(prefix == "v"){
            str_stream >> x_str >> y_str >> z_str;
            x = stof(x_str);
            y = stof(y_str);
            z = stof(z_str);
            vertices.push_back(Point(x, y, z));
        }else if(prefix == "f"){
            std::vector<int> vertice_inds = std::vector<int>();
            std::vector<int> normal_inds = std::vector<int>();
            std::string num, temp;

            while(str_stream >> num){
                // Searches string for '/' for any texture vertex and vector normal settings
                if(num.find('/') == std::string::npos){
                    vertice_inds.push_back(stoi(num) - 1);
                }else{
                    // string stream used for splitting the string
                    std::stringstream ss(num);
                    std::getline(ss, temp, '/');
                    vertice_inds.push_back(stoi(temp) - 1);
                    std::getline(ss, temp, '/');
                    std::getline(ss, temp, '/');
                    normal_inds.push_back(stoi(temp) - 1);
                }
            }

            Point p1, p2, p3;
            Vector n1, n2, n3;
            if(vertice_inds.size() < 3){
                throw std::invalid_argument("f prefix requires at least 3 vertices provided");
            }else if(vertice_inds.size() == 3 && normal_inds.size() == 3){
                p1 = vertices.at(vertice_inds.at(0));
                p2 = vertices.at(vertice_inds.at(1));
                p3 = vertices.at(vertice_inds.at(2));
                n1 = normals.at(normal_inds.at(0));
                n2 = normals.at(normal_inds.at(1));
                n3 = normals.at(normal_inds.at(2));
                groups.at(groupsInd)->appendShape(new SmoothTriangle(p1, p2, p3, n1, n2, n3));
            }else if(vertice_inds.size() == 3){
                p1 = vertices.at(vertice_inds.at(0));
                p2 = vertices.at(vertice_inds.at(1));
                p3 = vertices.at(vertice_inds.at(2));
                groups.at(groupsInd)->appendShape(new Triangle(p1, p2, p3));
            }else{
                fanTriangulation(vertice_inds);
            }
        }else if(prefix == "g"){
            std::string name;
            bool found = false;
            if(str_stream >> name){
                for(int i = 0; i < groups.size(); i++){
                    if(groups.at(i)->name == name){
                        found = true;
                        groupsInd = i;
                        break;
                    }
                }

                if(!found){
                    groups.push_back(new Group(name));
                    groupsInd = groups.size() - 1;
                }
            }
        }else if(prefix == "vn"){
            str_stream >> x_str >> y_str >> z_str;
            x = stof(x_str);
            y = stof(y_str);
            z = stof(z_str);
            normals.push_back(Vector(x, y, z));
        }else{
            continue;
        }
    }
}

void ObjParser::fanTriangulation(std::vector<int> vertice_inds){
    for(int ind = 1; ind < vertice_inds.size() - 1; ind++){
        groups.at(groupsInd)->appendShape(new Triangle(vertices.at(vertice_inds.at(0)), vertices.at(vertice_inds.at(ind)), vertices.at(vertice_inds.at(ind + 1))));
    }
}

Group* ObjParser::objToGroup(){
    Group* g = new Group;

    for(int i = 0; i < groups.size(); i++){
        if(groups.at(i)->getShapes().size() != 0){
            g->appendShape(groups.at(i));
        }
    }

    return g;
}