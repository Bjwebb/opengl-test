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
            if (sscanf(oneline, "%s %s %s %s %s", tmp, &v1, &v2, &v3, &v4) == 4) /*printf("Test %s ", v4)*/;
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
        glColor3fv( blue );
        glRotatef(90, 1, 0, 0);
        if (i == 1) glRotatef(-90, 0, 0, 1);
        glVertexPointer( 3, GL_FLOAT, sizeof(WVector), Objects[i].Vertices);
        glNormalPointer( GL_FLOAT, sizeof(WVector), Objects[i].Normals );
        glDrawElements( GL_QUADS, Objects[i].numIndices, GL_UNSIGNED_INT, Objects[i].Indices );
    }
    return;
}

void SetupWorld(char* worldfile) {
    numObjects = 2;
    Objects = new OBJECT[numObjects];
    Objects[0] = LoadObj("monkey.obj");
    Objects[0].pos.y = 1.0f;
    Objects[0].pos.z = -6.0f;
    Objects[1] = LoadObj("monkey.obj");
    Objects[1].pos.x = -3.0f;
    Objects[1].pos.y = 1.0f;
    Objects[1].pos.z = -3.0f;
    
    boundCreate(Objects,numObjects);
}

