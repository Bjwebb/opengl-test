#include "main.h"
int boxes;
tBoundingBox *boxArray;

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

bool boundCheck(float minX, float maxX, float minZ, float maxZ) {
    for (int i=0; i<boxes; i++) {
        if (    maxX > boxArray[i].min.x &&
                minX < boxArray[i].max.x &&
                maxZ > boxArray[i].min.z &&
                minZ < boxArray[i].max.z ) {
            return false;
        }
    }
    return true;
}

void wallCreate(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
    
}

