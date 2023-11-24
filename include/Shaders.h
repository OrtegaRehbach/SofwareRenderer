#pragma once

#include <glm/glm.hpp>
#include "Uniform.h"
#include "Color.h"
#include "Fragment.h"
#include "Vertex.h"
#include "FastNoiseLite.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms);
Fragment stripedPlanetFragmentShader(const Fragment& fragment);
Fragment earthPlanetFragmentShader(const Fragment& fragment);
Fragment moonFragmentShader(const Fragment& fragment);
Fragment starFragmentShader(const Fragment& fragment);
Fragment redPlanetFragmentShader(const Fragment& fragment);
Fragment testFragmentShader(const Fragment& fragment);
Fragment shipFragmentShader(const Fragment& fragment);