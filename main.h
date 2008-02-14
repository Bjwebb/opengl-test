#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

struct WVector
{
    GLfloat x,y,z;
//     GLfloat r,g,b;
};

typedef struct tagOBJECT {
    WVector pos;
    WVector rot;
    float angle;
    GLuint* Indices;
    WVector* Vertices;
    WVector* Normals;
    int numIndices, numVertices, numNormals;
} OBJECT;

void you_compensate();

void SetupWorld(char* worldfile);
void DrawWorld();

void boundCreate(OBJECT* objs, int num);
bool boundCheck(float minX, float maxX, float minZ, float maxZ);
void wallCreate(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
bool wallCheck(float minX, float maxX, float minZ, float maxZ);

static GLfloat red[]    = { 255,   0,   0 };
static GLfloat green[]  = {   0, 255,   0 };
static GLfloat blue[]   = {   0,   0, 255 };
static GLfloat white[]  = { 255, 255, 255 };
static GLfloat black[]  = {   0,   0,   0 };
static GLfloat yellow[] = { 255, 255,   0 };
static GLfloat purple[] = { 255,   0, 255 };

void buildFont(GLvoid);
void glPrint(const char *fmt, ...);
