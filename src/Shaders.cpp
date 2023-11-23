
#include "Shaders.h"

Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);

    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    // Return the transformed vertex
    return Vertex{
        glm::vec3(screenVertex),    // Transformed position
        transformedNormal,          // Transformed normal
        vertex.position             // Original position
    };
}

Fragment stripedPlanetFragmentShader(const Fragment& fragment) {
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    Color fragmentColor = Color(120, 0, 220);

    float intensity = fragment.intensity;

    float xPos = fragment.originalPosition.x;
    float yPos = fragment.originalPosition.y;

    fragmentColor = fragmentColor + Color(0, 0, 255) * 1.2f * std::abs(std::sin(6 * (3.1416f * yPos) + 0.5f) * std::sin(9 * 3.1416f * yPos + 20 * std::pow(yPos, 3)) + 0.6f * std::sin(0.4f * 3.146f * xPos + 3));

    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor * intensity);
    
    return shadedFragment;
}

Fragment earthPlanetFragmentShader(const Fragment& fragment) {
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    float intensity = fragment.intensity;
    FastNoiseLite noise;
    noise.SetSeed(123);  // Set a seed for reproducibility
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    // Scale determines the level of detail in the noise
    float scale = 700.0f;

    // Use Perlin noise to generate elevation
    float elevation = 10.0f * noise.GetNoise(fragment.originalPosition.x * scale, fragment.originalPosition.y * scale, fragment.originalPosition.z * scale);
    elevation += 1.0f;
    elevation *= 0.5f;

    // Threshold for land and water
    float landThreshold = 0.88f;
    float waterThreshold = 0.6f;

    // Determine if the fragment is land or water
    Color fragmentColor;
    if (elevation > landThreshold) {
        // Land color (green)
        fragmentColor = Color(0, 160, 0);
    } else if (elevation > waterThreshold) {
        // Shallow water color (light blue)
        fragmentColor = Color(173, 216, 230);
    } else {
        // Deep water color (dark blue)
        fragmentColor = Color(0, 0, 128);
    }

    float cloudScale = 550.0f;
    float cloudCoverage = noise.GetNoise(fragment.originalPosition.x * cloudScale + fragment.x, fragment.originalPosition.y * cloudScale + fragment.y, fragment.originalPosition.z * cloudScale + fragment.z);
    cloudCoverage += 1.0f;
    cloudCoverage *= 0.5f;

    float cloudThreshold = 0.7f;
    if (cloudCoverage > cloudThreshold) {
        fragmentColor = fragmentColor +  Color(220, 220, 220) * cloudCoverage;
    }

    // Apply variations based on noise for a more natural look
    float noiseValue = noise.GetNoise(fragment.originalPosition.x * 400.0f, fragment.originalPosition.y * 400.0f, fragment.originalPosition.z * 400.0f);
    fragmentColor = fragmentColor * (1.0f + 0.5f * noiseValue);

    // Apply intensity based on elevation for some 3D effect
    intensity *= 1.0f - elevation * 0.1f;
    fragmentColor = fragmentColor * intensity;

    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor);
    
    return shadedFragment;
}

Fragment moonFragmentShader(const Fragment& fragment) {
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    float intensity = fragment.intensity;
    FastNoiseLite noise;
    noise.SetSeed(456);  // Set a different seed for variety
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    // Scale determines the level of detail in the noise
    float scale = 900.0f;

    // Use Perlin noise to generate crater formations
    float craterFormation = noise.GetNoise(fragment.originalPosition.x * scale, fragment.originalPosition.y * scale, fragment.originalPosition.z * scale);
    craterFormation = (craterFormation + 1.0f) * 0.5f;

    // Threshold for craters
    float craterThreshold = 0.8f;

    // Determine if the fragment is part of a crater or not
    Color fragmentColor;
    if (craterFormation > craterThreshold) {
        // Crater color (dark gray)
        fragmentColor = Color(50, 50, 50);
    } else {
        // Moon surface color (light gray)
        fragmentColor = Color(180, 180, 180);
    }

    // Apply variations based on noise for a more natural look
    float noiseValue = noise.GetNoise(fragment.originalPosition.x * 300.0f, fragment.originalPosition.y * 300.0f, fragment.originalPosition.z * 300.0f);
    fragmentColor = fragmentColor * (1.0f + 0.2f * noiseValue);

    // Apply intensity to the crater color
    intensity *= 1.0f - craterFormation * 0.1f;
    fragmentColor = fragmentColor * intensity;

    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor);

    return shadedFragment;
}


Fragment starFragmentShader(const Fragment& fragment) {
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    Color baseColor = Color(255, 40, 0) * 0.5f;
    Color highlightColor = Color(255, 103, 0);

    // Calculate distance from the center of the screen
    float distanceToCenter = glm::length(glm::vec2(fragment.x, fragment.y));

    // Create a FastNoiseLite instance for generating noise
    FastNoiseLite noise;
    noise.SetSeed(123);  // You can set any seed value

    // Scale the coordinates to control the noise pattern
    float scale = 1600.0f;
    float noiseValue = noise.GetNoise(fragment.originalPosition.x * scale, fragment.originalPosition.y * scale, fragment.originalPosition.z * scale);
    noiseValue = 1.0f + 0.5f * noiseValue;

    // Add variations to the intensity based on the noise value
    float intensity = (noiseValue < 0.7f) ? 0.0f : noiseValue;

    // Vary the color based on noiseValue
    Color fragmentColor = baseColor * (1.0f/(intensity + 0.0001f)) + highlightColor * intensity;
    fragmentColor = (noiseValue < 0.7f) ? baseColor : fragmentColor;

    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor);

    return shadedFragment;
}


Fragment testFragmentShader(const Fragment& fragment) {
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    Color fragmentColor = Color(220, 220, 220);

    float intensity = fragment.intensity;
  
    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor * intensity);

    return shadedFragment;
}

Fragment shipFragmentShader(const Fragment& fragment) {
    glm::vec3 fragmentPosition(fragment.x, fragment.y, fragment.z);
    Color fragmentColor = Color(255, 20, 20);

    float intensity = fragment.intensity;

    Fragment shadedFragment = Fragment(fragmentPosition, fragmentColor * intensity);
    
    return shadedFragment;
}