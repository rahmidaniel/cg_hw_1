//
// Created by danie on 3/30/2022.
//

#ifndef CG_HF1_UTILITY_H
#define CG_HF1_UTILITY_H

#include <random>
#include <list>
#include "framework.h"
#include "debugmalloc.h"

// Important constants
static int worldSize = 1000; // for atom positions
static int circleTessellation = 20; // for circle definition (triangle fan count)
static int lineTessellation = 20; // for line definition (line strip count)
static float dragCoefficient = 0.2;
static float twicePi = 2 * M_PI; // for circle vertex calc
static float coulomb = 1 / (twicePi * 8.854f);
static GPUProgram gpuProgram; // vertex and fragment shaders

// Atom constants
static int massMulti = 100;
static int chargeMulti = 2000;

struct vertex {
    vec2 pos;
    vec4 color; // RGBA - alpha is intensity (Charge/RAND_Max) for atom
};

/**
 * Fills given array with points of a circle
 * @param array - adds vertex positions to this array
 * @param offset - is the center of the circle, with given color
 * @param radius - is 1 by default
 */
void tessellateCircle(std::vector<vertex>& array, vertex offset, float radius = 1.f);
std::vector<vec2> getTessellation();
/*
 * Creates OpenGL context (vao, vbo)
 */
void create(unsigned int& vao, unsigned int& vbo);

/** Generate random number with mt19937
 * Will generate between bounds as: [bLower, bUpper]
 * src = https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
 */
int randomInt(int bUpper = RAND_MAX, int bLower = 1);

/**  Fast exponentiation
 * src = https://medium.com/techzap/fast-exponentiation-for-competitive-programming-c-2639362698f2
 */
int fastExpo(int base, int exp);

static float coulombConst = 8.988f * fastExpo(10, 9); // src = wikipedia

// vertex shader in GLSL: It is a Raw string (C++11) since it contains new line characters
const char * const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

    layout(location = 0) in vec2 pos;
    layout(location = 1) in vec4 color;

    out vec4 outColor;

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix
    uniform bool poincare;      // uniform variable, if true the shader will transforms points to Poincare disk

	void main() {
        // Poincare transformation
        if(poincare){
            float w = sqrt(pos.x * pos.x + pos.y * pos.y + 1);
            float x = pos.x / (w + 1);
            float y = pos.y / (w + 1);
		    gl_Position = vec4(x, y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
        // In case of Euclidean representation
        } else {
            gl_Position = vec4(pos.x, pos.y, 0, 1) * MVP;
        }
		outColor = color;
	}
)";

// fragment shader in GLSL
const char * const fragmentSource = R"(
	#version 330			// Shader 3.3
	precision highp float;	// normal floats, makes no difference on desktop computers

//	uniform vec3 color;		// uniform variable, the color of the primitive
    //TODO: is this needed????
    in vec4 outColor;
    out vec4 fragmentColor;		// computed color of the current pixel

	void main() {
		fragmentColor = outColor;	// computed color is the color of the primitive //TODO: set to temp color, should be outColor
	}
)";

/**
 * Creates equidistant points between 2 given points
 * @param p - line start
 * @param q - line end
 * @param color - color of vertices
 * @return - tessellated points of the segment given
 */
std::vector<vertex> getTessellation(vec2 p, vec2 q, vec4 color);

#endif //CG_HF1_UTILITY_H
