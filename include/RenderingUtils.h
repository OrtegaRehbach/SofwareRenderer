#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp" // glm::lookAt()
#include <vector>
#include "Color.h"
#include "Vertex.h"
#include "Face.h"
#include "Fragment.h"
#include "Camera.h"
#include "globals.h"

// Render to window
void drawPoint(SDL_Renderer* renderer, float x_position, float y_position, const Color& color = Color(255, 255, 255));
// Fragment generating
std::vector<Fragment> drawLine(const glm::vec3& start, const glm::vec3& end, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawTriangle(const glm::vec3& pointA, const glm::vec3& pointB, const glm::vec3& pointC, const Color& color = Color(255, 255, 255));
std::vector<Fragment> drawTriangle(const std::vector<Vertex>& triangle, const Color& color = Color(255, 255, 255));
std::vector<Fragment> getTriangleFragments(Vertex a, Vertex b, Vertex c, const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const Camera& camera);
// Rendering pipeline
std::vector<glm::vec3> setupVertexBufferObject(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<Face>& faces);
std::vector<std::vector<Vertex>> primitiveAssembly (const std::vector<Vertex>& transformedVertices);
std::vector<Fragment> rasterize(const std::vector<std::vector<glm::vec3>>& assembledVertices);
// Transformation matrixes
glm::mat4 createModelMatrix(const glm::vec3& scaleVector = glm::vec3(1, 1, 1), const glm::vec3& translationVector = glm::vec3(0, 0, 0), const float rotationAngleRadians = 0.0f, const glm::vec3& rotationAxis = glm::vec3(0, 1, 0));
glm::mat4 createViewMatrix(Camera camera);
glm::mat4 createProjectionMatrix(int SCREEN_WIDTH, int SCREEN_HEIGHT);
glm::mat4 createViewportMatrix(int SCREEN_WIDTH, int SCREEN_HEIGHT);
// Related calculations
bool isInsideScreen(const Fragment& fragment, int SCREEN_WIDTH, int SCREEN_HEIGHT);
bool isInsideScreen(int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT);
bool bBoxInsideScreen(int minX, int minY, int maxX, int maxY, int SCREEN_WIDTH, int SCREEN_HEIGHT);
glm::vec3 barycentricCoordinates(const glm::vec3& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);
bool isInsideTriangle(const glm::vec3& barycentricCoordinates);
glm::vec3 findTriangleCentroid(Vertex a, Vertex b, Vertex c);
glm::vec3 calculateTriangleNormal(glm::vec3 A,glm::vec3 B, glm::vec3 C);