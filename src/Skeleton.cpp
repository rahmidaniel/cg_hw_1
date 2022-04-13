//=============================================================================================
// Mintaprogram: Z�ld h�romsz�g. Ervenyes 2019. osztol.
//
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat, BOM kihuzando.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni a printf-et kiveve
// - Mashonnan atvett programresszleteket forrasmegjeloles nelkul felhasznalni es
// - felesleges programsorokat a beadott programban hagyni!!!!!!! 
// - felesleges kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan OpenGL fuggvenyek hasznalhatok, amelyek az oran a feladatkiadasig elhangzottak 
// A keretben nem szereplo GLUT fuggvenyek tiltottak.
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Rahmi Daniel
// Neptun : WIFRTR
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
#include <iostream>
#include "Molecule.h"

// Data
Molecule molecule1;
Molecule molecule2;

// Time
static float lastTick;
static float tickStep = 0.1f;

// World objects
std::vector<vertex> disk;

// OpenGL
unsigned int vao, vbo;

void create(); // for the disk

// Initialization, create an OpenGL context
void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);
    glLineWidth(2.0f); // line pixel width

    // Create world
    create();

    molecule1.create();
    molecule2.create();

    // Create program for the GPU
	gpuProgram.create(vertexSource, fragmentSource, "fragmentColor");
}

void create() {
    create(vao, vbo);

    // Creating the Poincare disk
    tessellateCircle(disk, {0, vec4(0.5f, 0.5f, 0.5f, 1)});
}

// Window has become invalid: Redraw
void onDisplay() {
	glClearColor(0.f, 0.f, 0.f, 1);     // background color
	glClear(GL_COLOR_BUFFER_BIT); // clear frame buffer

    // MVP
	int location = glGetUniformLocation(gpuProgram.getId(), "MVP");	// Get the GPU location of uniform variable MVP
    mat4 scaled = ScaleMatrix(vec2(1,1));
	glUniformMatrix4fv(location, 1, GL_TRUE, &scaled[0][0]);	// Load a 4x4 row-major float matrix to the specified location

    // Poincare flag to false
    location = glGetUniformLocation(gpuProgram.getId(), "poincare");
    glUniform1i(location, false);

    // Poincare disk (in Euclidean space)
    glBufferData(GL_ARRAY_BUFFER, disk.size() * sizeof(vertex), &disk[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, disk.size());

    // Transform to poincare form here
    glUniform1i(location, true);

    molecule1.draw();
    molecule2.draw();

	glutSwapBuffers(); // exchange buffers for double buffering
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
	if (key == ' ') {
        molecule1.init(); molecule1.create();
        molecule2.init(); molecule2.create();
    }

    glutPostRedisplay();
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
}

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;
	printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;

	std::string buttonStat;
	switch (state) {
	case GLUT_DOWN: buttonStat = "pressed"; break;
	case GLUT_UP:   buttonStat = "released"; break;
	}

	switch (button) {
	case GLUT_LEFT_BUTTON:   printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat.c_str(), cX, cY);   break;
	case GLUT_MIDDLE_BUTTON: printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat.c_str(), cX, cY); break;
	case GLUT_RIGHT_BUTTON:  printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat.c_str(), cX, cY);  break;
	}
}

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
    float timeSec = time / 1000.0f;
    float T = timeSec - lastTick; // time between updates
    for(float t = 0.f; t < T; t += tickStep){
        molecule1.react2Molecule(molecule2, tickStep);
        molecule2.react2Molecule(molecule1, tickStep);

    }
        lastTick = timeSec;

    glutPostRedisplay();
}
