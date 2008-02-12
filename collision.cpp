#include "main.h"
const int boxes = 1;
tBoundingBox boxArray[boxes];

void boundCreate() {
    boxArray[0].min.x  = -1;
    boxArray[0].max.x  = 1;
    boxArray[0].min.z  = -7;
    boxArray[0].max.z  = -5; 
}

bool boundCheck(float minX, float maxX, float minZ, float maxZ) {
//     float minX = x;
//     float maxX = x;
//     float minZ = z;
//     float maxZ = z;
    for (int i=0; i<boxes; i++) {
        if (    minX > boxArray[i].min.x &&
                maxX < boxArray[i].max.x &&
                minZ > boxArray[i].min.z &&
                maxZ < boxArray[i].max.z ) {
            return false;
        }
    }
    return true;
}
