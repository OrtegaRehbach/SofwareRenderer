#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <array>
#include <sstream>
#include <functional>
#include <random>

#include "globals.h"
#include "ObjLoader.h"
#include "Vertex.h"
#include "Face.h"
#include "Uniform.h"
#include "RenderingUtils.h"
#include "Shaders.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::array<std::array<float, SCREEN_WIDTH>, SCREEN_HEIGHT> zbuffer;
SDL_Window* window;
SDL_Renderer* renderer;
const int* globalScreenHeight;
const int* globalScreenWidth;

Uniforms uniforms;

// Define the type of function to store in activeShader
using ShaderFunction = std::function<Fragment(const Fragment& fragment)>;

// Global variable to store active fragment shader function
ShaderFunction activeShader;


bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Render Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    globalScreenHeight = &SCREEN_HEIGHT;
    globalScreenWidth = &SCREEN_HEIGHT;

    return true;
}

void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Fill the z-buffer
    for (auto &row : zbuffer) {
        std::fill(row.begin(), row.end(), 99999.0f);
    }
}

void point(Fragment fragment) {
    if (isInsideScreen(fragment, SCREEN_WIDTH, SCREEN_HEIGHT) && 
        fragment.z < zbuffer[fragment.y][fragment.x]) {
        // Draw the fragment on screen
        drawPoint(renderer, fragment.x, fragment.y, fragment.color);
        // Update the zbuffer for value for this position
        zbuffer[fragment.y][fragment.x] = fragment.z;
    }
}

void render(std::vector<glm::vec3> vertexBufferObject, Camera camera) {
    // 1. Vertex Shader
    std::vector<Vertex> transformedVertices;
    for (int i = 0; i < vertexBufferObject.size(); i += 2) {
        Vertex vertex = Vertex(vertexBufferObject[i], vertexBufferObject[i + 1]);
        Vertex transformedVertex = vertexShader(vertex, uniforms);
        transformedVertices.push_back(transformedVertex);
    }

    // 2. Primitive Assembly
    std::vector<std::vector<Vertex>> triangles = primitiveAssembly(transformedVertices);

    // 3. Rasterization
    std::vector<Fragment> fragments;
    for (std::vector<Vertex> triangle : triangles) {
        std::vector<Fragment> rasterizedTriangle = getTriangleFragments(triangle[0], triangle[1], triangle[2], SCREEN_WIDTH, SCREEN_HEIGHT, camera);

        fragments.insert(
            fragments.end(),
            rasterizedTriangle.begin(),
            rasterizedTriangle.end()
        );
    }
    
    // 4. Fragment Shader
    for (Fragment fragment : fragments) {
        Fragment transformedFragment = activeShader(fragment);
        point(transformedFragment);
    }
}

float generateRandomNormal(float mean = 0.0f, float stddev = 1.0f) {
    // Set up a random engine and distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> distribution(mean, stddev);

    // Generate and return a random value
    return distribution(gen);
}

std::vector<glm::vec3> generateStars() {
    int amount = 1000;
    float radius = 500.0f;
    std::vector<glm::vec3> starVertices;
    for (int i = 0; i < amount; i++) {
        float x = generateRandomNormal();
        float y = generateRandomNormal();
        float z = generateRandomNormal();
        if (x == 0 && y == 0 && z == 0) {
            i--;
            continue;
        }
        glm::vec3 starVertex(x, y, z);
        starVertex = glm::normalize(starVertex) * radius;
        starVertices.push_back(starVertex);
    }
    return starVertices;
}

void drawStars(const std::vector<glm::vec3>& starVertices, const Uniforms& uniforms) {
    for (auto& star : starVertices) {
        // Apply transformations to the star using the matrices from the uniforms
        glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(star, 1.0f);
        // Perspective divide
        glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;
        // Apply the viewport transform
        glm::vec4 screenStar = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);

        Fragment starFragment{
            glm::vec3(screenStar.x, screenStar.y, 9999.0f),
            Color(),
            1.0f,
            glm::vec3(screenStar),
            star
        };
        point(starFragment);
    }
}

int main() {
    // Initialize SDL
    if (!init()) { return 1; }
    
    // Read from .obj file and store the vertices/faces
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;

    bool loadModel = true;
    if (loadModel) {
        loadOBJ("../models/sphere.obj", vertices, normals, faces);
    }

    std::vector<glm::vec3> shipVertices;
    std::vector<glm::vec3> shipNormals;
    std::vector<Face> shipFaces;

    if (loadModel) {
        loadOBJ("../models/Lab3_Ship.obj", shipVertices, shipNormals, shipFaces);
    }

    Camera camera = {glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)};
    const float cameraMovementSpeed = 0.1f;
    const float horizontalRotationSpeed = 0.02f;

    std::vector<glm::vec3> stars = generateStars();

    std::vector<glm::vec3> VBO = setupVertexBufferObject(vertices, normals, faces);
    std::vector<glm::vec3> VBO_ship = setupVertexBufferObject(shipVertices, shipNormals, shipFaces);

    float rotation = 0.0f;
    float moonRotation = 0.0f;
    Uint32 frameStart, frameTime;
    float orbitAngle = 0.0f;

    // Render loop
    bool running = true;
    SDL_Event event;
    while (running) {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                running = false;
            // Camera movement
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_w) {
                    // "W" key was pressed
                    camera.MoveForward(cameraMovementSpeed);
                }
                else if (event.key.keysym.sym == SDLK_s) {
                    // "S" key was pressed
                    camera.MoveBackward(cameraMovementSpeed);
                }
                else if (event.key.keysym.sym == SDLK_d) {
                    // "D" key was pressed
                    camera.MoveRight(cameraMovementSpeed);
                }
                else if (event.key.keysym.sym == SDLK_a) {
                    // "A" key was pressed
                    camera.MoveLeft(cameraMovementSpeed);
                }
                else if (event.key.keysym.sym == SDLK_q) {
                    // Rotate the camera left (orbit)
                    camera.Rotate(1.0f, 0.0f);
                }
                else if (event.key.keysym.sym == SDLK_e) {
                    // Rotate the camera right (orbit)
                    camera.Rotate(-1.0f, 0.0f);
                }
            }
        }        

        // Clear the buffer
        clear();

        // Get the rotation quaternion
        glm::quat cameraRotation = camera.getCameraRotation();

        // Calculate matrixes for rendering
        uniforms.view = createViewMatrix(camera);
        uniforms.projection = createProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
        uniforms.viewport = createViewportMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

        // Star sphere model matrix
        uniforms.model = createModelMatrix(glm::vec3(1000), glm::vec3(0));
        drawStars(stars, uniforms);

        // Render big planet
        uniforms.model = createModelMatrix(glm::vec3(1), glm::vec3(0, 0, 0), rotation += 0.06f);
        activeShader = earthPlanetFragmentShader;  // Set active shader to striped planet shader
        render(VBO, camera);    // Big planet
        
        
        // Render small planet
        // Orbiting values
        float orbitRadius = 1.5f;
        float xPos = orbitRadius * std::cos(orbitAngle);
        float zPos = orbitRadius * std::sin(orbitAngle);

        // New model matrix for small planet
        uniforms.model = createModelMatrix(glm::vec3(0.4), glm::vec3(xPos, 0, zPos), moonRotation -= 0.4f);

        activeShader = moonFragmentShader;  // Set active shader to test fragment shader
        render(VBO, camera);    // Small orbiting planet

        orbitAngle += 0.15f;    // Increase orbit angle


        // Render ship
        glm::vec3 targetOffset = glm::vec3(0, 0.4, 0);
        uniforms.model = createModelMatrix(glm::vec3(0.1), camera.targetPosition - targetOffset, 1.57);
        // Apply the camera's rotation to the ship's model matrix
        uniforms.model *= glm::mat4_cast(cameraRotation);

        activeShader = shipFragmentShader;
        render(VBO_ship, camera);

        // Present the framebuffer to the screen
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        // Calculate frames per second and update window title
        if (frameTime > 0) {
            std::ostringstream titleStream;
            titleStream << "FPS: " << static_cast<int>(1000.0 / frameTime);  // Milliseconds to seconds
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    quit();
    exit(0);
}
