//
// Created by danie on 3/31/2022.
//

#include "Atom.h"

Atom::Atom() {
    init();
}

void Atom::init() {

    // Positions and radii are normalized

    center.pos = vec2(randomInt(worldSize,-worldSize) / (float)worldSize,
               randomInt(worldSize,-worldSize) / (float)worldSize);

    radius = randomInt( worldSize / 10, worldSize / 40) / (float)worldSize;

    int intensity = randomInt(massMulti);

    int neg = randomInt(10) <= 5 ? -1 : 1; // random negative

    mass = 1.008f * intensity;
    qCharge = -1.602f * (1.f/fastExpo(10, 19)) * randomInt(chargeMulti) * neg;

    // calculate color intensity
    float alphaIntensity = (float)intensity/massMulti;
    if(qCharge > 0) center.color = vec4(alphaIntensity,0,0, 1); // red
    else center.color = vec4(0,0,alphaIntensity,1); // blue
}

void Atom::calculateVertices() {
    vertices.clear(); // reset
    vertices.push_back({center.pos, center.color});
    for(int i = 0; i <= stepNum; i++){
        vec2 pos = vec2(center.pos.x + radius * cos(i * twicePi / stepNum), center.pos.y + radius * sin(i * twicePi / stepNum));
        vertices.push_back({pos, center.color});
        //printf("%0.4f, %0.4f ; %0.f\n", pos.x, pos.y, center.color.x);
    }
}

void Atom::create() {
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

void Atom::draw() {
    calculateVertices();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}

Atom::~Atom() {
    if(vbo != 0 && vao != 0){
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}
