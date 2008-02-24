#include "main.h"

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

int boxes;
tBoundingBox *boxArray;
const int walls = 5;
tThinWall *wallArray = new tThinWall[walls];

void boundCreate(OBJECT* objs, int num) {
    boxArray = new tBoundingBox[num];
    boxes=num;
    for (int j=0; j<num; j++) {
        float minX = objs[j].Vertices[0].x;
        float maxX = objs[j].Vertices[0].x;
        float minZ = objs[j].Vertices[0].z;
        float maxZ = objs[j].Vertices[0].z;
        for (int i=1; i<objs[j].numVertices; i++) {
            if (objs[j].Vertices[i].x < minX) minX = objs[j].Vertices[i].x;
            if (objs[j].Vertices[i].x > maxX) maxX = objs[j].Vertices[i].x;
            if (objs[j].Vertices[i].z < minX) minX = objs[j].Vertices[i].z;
            if (objs[j].Vertices[i].z > maxX) maxX = objs[j].Vertices[i].z;
        }
        boxArray[j].min.x  = minX + objs[j].pos.x;
        boxArray[j].max.x  = maxX + objs[j].pos.x;
        boxArray[j].min.z  = minZ + objs[j].pos.z;
        boxArray[j].max.z  = maxZ + objs[j].pos.z;
    }
}

int boundCheck(float minX, float maxX, float minZ, float maxZ) {
    for (int i=0; i<boxes; i++) {
        // TODO find out why the difference in X and Z
        if (    (maxX+1) > boxArray[i].min.x &&
                (minX-1) < boxArray[i].max.x &&
                (maxZ+2.5f) > boxArray[i].min.z &&
                (minZ-2.5f) < boxArray[i].max.z ) {
            if (i==6)
                return 12;
            else
                return false;
        }
    }
    return wallCheck(minX-2, maxX+2, minZ-2, maxZ+2);
}
//wallCreate(-10, 0, 0, 2, 2, 0);
void wallCreate(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, int walli) {
    int numIndices = 8;
    int numVertices = 4;
    WVector* Vertices = new WVector[numVertices];
    Vertices[0].x = maxX;
    Vertices[0].y = maxY;
    Vertices[0].z = maxZ;
    Vertices[1].x = minX;
    Vertices[1].y = maxY;
    Vertices[1].z = minZ;
    Vertices[2].x = minX;
    Vertices[2].y = minY;
    Vertices[2].z = minZ;
    Vertices[3].x = maxX;
    Vertices[3].y = minY;
    Vertices[3].z = maxZ;
    GLfloat* Normals = new GLfloat[numVertices*3];
//     Normals[0] = 0;
//     Normals[1] = -1;
//     Normals[2] = 0;
//     Normals[3] = 0;
//     Normals[4] = -1;
//     Normals[5] = 0;
//     Normals[6] = 0;
//     Normals[7] = -1;
//     Normals[8] = 0;
//     Normals[9] = 0;
//     Normals[10] = -1;
//     Normals[11] = 0;
    GLuint* Indices = new GLuint[numIndices];
    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 3;
    Indices[4] = 3;
    Indices[5] = 2;
    Indices[6] = 1;
    Indices[7] = 0;
    glColor3fv( green );
    glNormalPointer( GL_FLOAT, sizeof(WVector), Normals);
    glVertexPointer( 3, GL_FLOAT, sizeof(WVector), Vertices);
    glDrawElements( GL_QUADS, numIndices, GL_UNSIGNED_INT, Indices );
    
    wallArray[walli].min.x = minX;
    wallArray[walli].min.y = minY;
    wallArray[walli].min.z = minZ;
    wallArray[walli].max.x = maxX;
    wallArray[walli].max.y = maxY;
    wallArray[walli].max.z = maxZ;
}

bool wallCheck(float minX, float maxX, float minZ, float maxZ) {
    for (int i=0; i<walls; i++) {
        if (maxX > wallArray[i].min.x && minX < wallArray[i].max.x
            && maxZ > wallArray[i].min.z && minZ < wallArray[i].max.z)
            return false;
    }
    return true;
}
