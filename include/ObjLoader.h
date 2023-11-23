#pragma once

#include <vector>
#include <string>
#include "Vertex.h"
#include "Face.h"

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec3>& out_normals, std::vector<Face>& out_faces);
