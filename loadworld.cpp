#include "main.h"

OBJECT *Objects;
int numObjects;

typedef struct tagVERTEX                        // Build Our Vertex Structure
{
    float x, y, z;                          // 3D Coordinates
    // float u, v;                         // Texture Coordinates
} VERTEX;

typedef struct tagFACE {
    int v;                          // 3D Vertices
    // float u, v;                         // Texture Coordinates
} FACE;

short readstr(FILE *f, char *string) {                 // Read In A String
    do
    {
        fgets(string, 255, f);
        if (feof(f)) return 1;
    } while ((string[0] == '#') || (string[0] == '\n'));
    return 0;
}

OBJECT LoadObj(char* worldfile) {    
    FILE *filein;                           // File To Work With
    filein = fopen(worldfile, "rt");                // Open Our File

    int numvert = 4000;
    char x[10], y[10], z[10];
    int numface = 4*4000;

    VERTEX* vertex = new VERTEX[numvert];
    int vertloop = 0;
    while (vertloop < numvert)        // Loop Through All The Vertices
    {
        char oneline[255];
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "v") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);
//             printf("%d %s: %s", vertloop, tmp, oneline);
        // Store Values Into Respective Vertices
            vertex[vertloop].x = atof(x);
            vertex[vertloop].y = atof(y);
            vertex[vertloop].z = atof(z);
            vertloop++;
        }
        else if (vertloop != 0) { break; }
    }

    VERTEX* normal = new VERTEX[numvert];
    int normloop = 0;
    while (normloop < numvert) {
        char oneline[255];
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "vn") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);
//             printf("%d %s: %s", normloop, tmp, oneline);
        // Store Values Into Respective Vertices
            normal[normloop].x = atof(x);
            normal[normloop].y = atof(y);
            normal[normloop].z = atof(z);
            normloop++;
        }
        else if (normloop != 0) { break; }
    }
    
    
     fclose(filein);
     filein = fopen(worldfile, "rt"); 
      
    FACE* face = new FACE[numface];
    int faceloop = 0;
    while (faceloop < numface) {
        char v1[10], v2[10], v3[10], v4[10];
        char oneline[255];
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s", tmp);
        if (strcmp(tmp, "f") == 0) {
            if (sscanf(oneline, "%s %s %s %s %s", tmp, &v1, &v2, &v3, &v4) == 4) {
                strcpy (v4, v3);
                // printf("Test %s \n", v4);
            }
        // Store Values Into Respective Vertices
            int test1, test2;
            if (sscanf(v1, "%d//%d", &test1, &test2) == 2) {
                face[faceloop].v = test1;
            }
            else {
                face[faceloop].v = atoi(v1);
            }
            faceloop++;
            if (sscanf(v2, "%d//%d", &test1, &test2) == 2) {
                face[faceloop].v = test1;
            }
            else {
                face[faceloop].v = atoi(v2);
            }
            faceloop++;
            if (sscanf(v3, "%d//%d", &test1, &test2) == 2) {
                face[faceloop].v = test1;
            }
            else {
                face[faceloop].v = atoi(v3);
            }
            faceloop++;
//             printf("%s\n", v4);
            if (sscanf(v4, "%d//%d", &test1, &test2) == 2) {
                face[faceloop].v = test1;
            }
            else {
                face[faceloop].v = atoi(v4);
            }
            faceloop++;
            for (int i=0; i<9; i++) {
                v4[i] = '0';
            }
            v4[9] = '\0'; // TODO make this more pretty; moreover, why do they carry on?
            printf("Face: %d %d %d %d\n", face[faceloop-4].v, face[faceloop-3].v, face[faceloop-2].v, face[faceloop-1].v);
        }
        else if (faceloop != 0) { break; }
    }
    fclose(filein);
    
    // DEBUG
//     printf("%d\n",vertloop);
//     printf("%d\n",faceloop);

    OBJECT tmpobj;
    tmpobj.numVertices = vertloop;    
    tmpobj.numIndices = faceloop;
    tmpobj.numNormals = normloop;
    tmpobj.Vertices = new WVector[tmpobj.numVertices];
    tmpobj.Indices = new GLuint[tmpobj.numIndices];
    tmpobj.Normals = new WVector[tmpobj.numNormals];
    
    for (int i=0; i<tmpobj.numVertices; i++) {
        tmpobj.Vertices[i].x = vertex[i].x;
        tmpobj.Vertices[i].y = vertex[i].y;
        tmpobj.Vertices[i].z = vertex[i].z; 
    }
    for (int i=0; i<tmpobj.numNormals; i++) {
        tmpobj.Normals[i].x = normal[i].x;
        tmpobj.Normals[i].y = normal[i].y;
        tmpobj.Normals[i].z = normal[i].z;
    }
    for (int i=0; i<tmpobj.numIndices; i++) {
        tmpobj.Indices[i] = face[i].v - 1;
    }
//     int evil = 0;
//     int asfd = 8/evil;
    return tmpobj;
}

void DrawWorld() {
    for (int i=0; i<numObjects; i++) {
        glLoadIdentity(); you_compensate();
        glTranslatef( Objects[i].pos.x, Objects[i].pos.y, Objects[i].pos.z );
        glRotatef(Objects[i].rot.x, 1, 0, 0);
        glRotatef(Objects[i].rot.y, 0, 1, 0);
        glRotatef(Objects[i].rot.z, 0, 0, 1);
        glColor3fv( blue ); // TODO
        glVertexPointer( 3, GL_FLOAT, sizeof(WVector), Objects[i].Vertices);
        glNormalPointer( GL_FLOAT, sizeof(WVector), Objects[i].Normals );
        glDrawElements( GL_QUADS, Objects[i].numIndices, GL_UNSIGNED_INT, Objects[i].Indices );
    }
    return;
}

void SetupWorld(char* worldfile) {
    FILE *filein;                           // File To Work With
    filein = fopen(worldfile, "rt");                // Open Our File
    
    char oneline[255];
    char tmp1[255];
    char tmp2[255];
    readstr(filein,oneline);
    sscanf(oneline, "%s %s", tmp1, tmp2);
    printf("numObjects: %s\n", tmp2);
    if (strcmp(tmp1, "numObjects") == 0) numObjects = atoi(tmp2);
    printf("numObjects: %d\n\n", numObjects);
    Objects = new OBJECT[numObjects];
    
    int i = 0;
    while (i < numObjects) {
        if (readstr(filein,oneline)) break;
        sscanf(oneline, "%s", tmp1);
        printf("%s", oneline);
        if (strcmp(tmp1, "Object") == 0) {
            float x=0,y=0,z=0,rx=0,ry=0,rz=0,rangle=0;
            printf("Count: %d\n", sscanf(oneline, "%s %s %f %f %f %f %f %f", tmp1, tmp2, &x, &y, &z, &rx, &ry, &rz));
            printf("Test: %s\n", tmp1);
            printf("Object: %s\n", tmp2);
            Objects[i] = LoadObj(tmp2);
            printf("%f %f %f\n\n", x, y, z);
            printf("%f %f %f\n\n", rx, ry, rz);
            Objects[i].pos.x = x;
            Objects[i].pos.y = y;
            Objects[i].pos.z = z;
            Objects[i].rot.x = rx;
            Objects[i].rot.y = ry;
            Objects[i].rot.z = rz;
            i++;
        }
        else if (i != 0) { break; }
    }

    boundCreate(Objects,numObjects);
}

