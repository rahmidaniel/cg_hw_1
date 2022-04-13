//
// Created by danie on 3/31/2022.
//

#include <iostream>
#include "utility.h"

int randomInt(int bUpper, int bLower){
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(bLower, bUpper);
    return (int)distribution(rng);
}
int fastExpo(int base, int exp){
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int oneHalf = fastExpo(base,exp / 2);
    return oneHalf * oneHalf * fastExpo(base,exp % 2);
}

void tessellateCircle(std::vector<vertex>& array, vertex offset, float radius) {
    array.clear(); // reset
    array.push_back({offset.pos, offset.color}); // center of the circle
    float phi = twicePi / (float)circleTessellation;
    for(int i = 0; i <= circleTessellation; i++)
        array.push_back({vec2(offset.pos.x + radius * cosf(i * phi), offset.pos.y + radius * sinf(i * phi)), offset.color});
}

void create(unsigned int &vao, unsigned int &vbo) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Map attribute array 0 to the vertex data of the interleaved vbo
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) offsetof(vertex, pos));
    // Map attribute array 1 to the color data of the interleaved vbo
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) offsetof(vertex, color));
}

std::vector<vertex> getTessellation(vec2 p, vec2 q, vec4 color) {
    std::vector<vertex> points;
    vec2 head = q - p; // Difference vector
    // Incrementing points from p to q
    for (int i = 0; i <= lineTessellation; i++) points.push_back({p + head * i / lineTessellation, color});
    return points;
}
