//
// Created by danie on 3/31/2022.
//

#include "Atom.h"

Atom::Atom() {
    init();
}

void Atom::init() {

    // Positions and radii are normalized
    center.pos = vec2(randomInt(worldSize,-worldSize) / (float)worldSize,randomInt(worldSize,-worldSize) / (float)worldSize);

    int massIntensity = randomInt(massMulti, massMulti / 10);
    int chargeIntensity = randomInt(chargeMulti, chargeMulti / 10);

    radius = (float)chargeIntensity / chargeMulti / 10; // 10 seems like a good middle ground
    int neg = randomInt(10) <= 5 ? -1 : 1; // random negative

    mass = 1.008f * massIntensity;
    qCharge = -1.602f * (1.f/fastExpo(10, 19)) * randomInt(chargeMulti) * neg;

    // Calculate color intensity from charge
    float alphaIntensity = radius * 10;
    if(qCharge > 0) center.color = vec4(alphaIntensity,0,0, 1); // red
    else center.color = vec4(0,0,alphaIntensity,1); // blue
}

void Atom::create() {
    ::create(vao, vbo);
}

void Atom::draw() {
    tessellateCircle(vertices, center, radius);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}

Atom::~Atom() {
    if(vbo != 0 && vao != 0){
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}
