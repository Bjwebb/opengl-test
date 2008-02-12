#include "main.h"
const int boxes = 1;
tBoundingBox boxArray[boxes];

void boundCreate() {
    boxArray[0].min.x  = -1;
    boxArray[0].max.x  = 1;
    boxArray[0].min.z  = -7;
    boxArray[0].max.z  = -5; 
}

bool boundCheck(float x, float z, float ) {
//     float minX = x;
//     float maxX = x;
//     float minZ = z;
//     float maxZ = z;
    for (int i=0; i<boxes; i++) {
        if (    x > boxArray[i].min.x &&
                x < boxArray[i].max.x &&
                z > boxArray[i].min.z &&
                z < boxArray[i].max.z ) {
            return false;
        }
    }
    return true;
}
