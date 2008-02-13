#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

struct WVector
{
    GLfloat x,y,z;
//     GLfloat r,g,b;
};

typedef struct tagOBJECT {
    WVector pos;
    GLuint* Indices;
    WVector* Vertices;
    WVector* Normals;
    int numIndices, numVertices, numNormals;
} OBJECT;

struct tBoundingBox
{
      WVector max;
      WVector min;
};

struct tThinWall
{
      WVector max;
      WVector min;
};

void you_compensate();

void SetupWorld(char* worldfile);
void DrawWorld();
void wallCreate(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

void boundCreate(OBJECT* objs, int num);
bool boundCheck(float minX, float maxX, float minZ, float maxZ);

static GLfloat red[]    = { 255,   0,   0 };
static GLfloat green[]  = {   0, 255,   0 };
static GLfloat blue[]   = {   0,   0, 255 };
static GLfloat white[]  = { 255, 255, 255 };
static GLfloat black[]  = {   0,   0,   0 };
static GLfloat yellow[] = { 255, 255,   0 };
static GLfloat purple[] = { 255,   0, 255 };
