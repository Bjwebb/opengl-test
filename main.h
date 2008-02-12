/*
    Parse materials
*/

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

void SetupWorld(char* worldfile);
void DrawWorld();
void boundCreate();
bool boundCheck(float minX, float maxX, float minZ, float maxZ);

