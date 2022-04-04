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
static int stepNum = 5; // for circle definition (triangle fan count)
static float twicePi = 2 * M_PI; // for circle vertex calc
static GPUProgram gpuProgram; // vertex and fragment shaders

struct vertex {
    vec2 pos;
    vec4 color; // RGBA - alpha is intensity (Charge/RAND_Max) for atom
};

/* Generate random number with mt19937
 * Will generate between bounds as: [bLower, bUpper]
 * src = https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
 */
int randomInt(int bLower = 1, int bUpper = RAND_MAX);

/* Fast exponentiation
 * src = https://medium.com/techzap/fast-exponentiation-for-competitive-programming-c-2639362698f2
 */
int fastExpo(int base, int exp);

static float coulombConst = 8.9875517923f * fastExpo(10, 9); // src = wikipedia

// vertex shader in GLSL: It is a Raw string (C++11) since it contains new line characters
const char * const vertexSource = R"(
	#version 330				// Shader 3.3
	precision highp float;		// normal floats, makes no difference on desktop computers

    layout(location = 0) in vec2 pos;
    layout(location = 1) in vec4 color;

    out vec4 outColor;

	uniform mat4 MVP;			// uniform variable, the Model-View-Projection transformation matrix

	void main() {
		gl_Position = vec4(pos.x, pos.y, 0, 1) * MVP;		// transform vp from modeling space to normalized device space
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


#endif //CG_HF1_UTILITY_H
