#include "main.h"

char* worldfile;

GLuint* Indices;
WVector* Vertices;
WVector* Normals;
int numIndices, numVertices, numNormals;

typedef struct tagVERTEX                        // Build Our Vertex Structure
{
    float x, y, z;                          // 3D Coordinates
    // float u, v;                         // Texture Coordinates
} VERTEX;

typedef struct tagFACE
{
    int v;                          // 3D Vertices
    // float u, v;                         // Texture Coordinates
} FACE;

typedef struct tagSECTOR                        // Build Our Sector Structure
{
    int numvert;                       // Number Of Triangles In Sector
    VERTEX* vertex;                     // Pointer To Array Of Triangles
    VERTEX* normal;
    int numface;
    FACE* face;
} SECTOR;                               // Call It SECTOR

SECTOR sector1;

short readstr(FILE *f, char *string)                  // Read In A String
{
    do
    {
        fgets(string, 255, f);
        if (feof(f)) return 1;
    } while ((string[0] == '#') || (string[0] == '\n'));
    return 0;
}

void SetupWorld(char* worldfile)                           // Setup Our World
{    
    FILE *filein;                           // File To Work With
    filein = fopen(worldfile, "rt");                // Open Our File

    int numvert = 4000;
    char x[10], y[10], z[10];
    int numface = 4*4000;
    
    sector1.numvert = numvert;
    sector1.vertex = new VERTEX[numvert];
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
            sector1.vertex[vertloop].x = atof(x);
            sector1.vertex[vertloop].y = atof(y);
            sector1.vertex[vertloop].z = atof(z);
//              printf("%f %f %f\n\n", sector1.vertex[vertloop].x, sector1.vertex[vertloop].y, sector1.vertex[vertloop].z);
            vertloop++;
        }
        else if (vertloop != 0) { break; }
    }

    sector1.normal = new VERTEX[numvert];
    int normloop = 0;
    while (normloop < numvert)        // Loop Through All The Vertices
    {
        char oneline[255];
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "vn") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);
//             printf("%d %s: %s", normloop, tmp, oneline);
        // Store Values Into Respective Vertices
            sector1.normal[normloop].x = atof(x);
            sector1.normal[normloop].y = atof(y);
            sector1.normal[normloop].z = atof(z);
//              printf("%f %f %f\n\n", sector1.vertex[normloop].x, sector1.vertex[normloop].y, sector1.vertex[normloop].z);
            normloop++;
        }
        else if (normloop != 0) { break; }
    }
    
    
     fclose(filein);
     filein = fopen(worldfile, "rt"); 
      
    sector1.numface = numface;
    sector1.face = new FACE[numface];
    int faceloop = 0;
    while (faceloop < numface)        // Loop Through All The Vertices
    {
        char v1[10], v2[10], v3[10], v4[10];
//         printf("Test\n");
        char oneline[255];
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s", tmp);
        if (strcmp(tmp, "f") == 0) {
//             printf("Test.1\n");
            if (sscanf(oneline, "%s %s %s %s %s", tmp, &v1, &v2, &v3, &v4) == 4) printf("Test %s ", v4);
//             printf("%d %s: %s", faceloop, tmp, oneline);
        // Store Values Into Respective Vertices
            int test1, test2;
            if (sscanf(v1, "%d//%d", &test1, &test2) == 2) {
//                 printf("Test %d %d\n", test1, test2);
                sector1.face[faceloop].v = test1;
            }
            else {
                sector1.face[faceloop].v = atoi(v1);
            }
            faceloop++;
            if (sscanf(v2, "%d//%d", &test1, &test2) == 2) {
//                 printf("Test %d %d\n", test1, test2);
                sector1.face[faceloop].v = test1;
            }
            else {
                sector1.face[faceloop].v = atoi(v2);
            }
            faceloop++;
            if (sscanf(v3, "%d//%d", &test1, &test2) == 2) {
//                 printf("Test %d %d\n", test1, test2);
                sector1.face[faceloop].v = test1;
            }
            else {
                sector1.face[faceloop].v = atoi(v3);
            }
            faceloop++;
//             printf("%s\n", v4);
            if (sscanf(v4, "%d//%d", &test1, &test2) == 2) {
//                 printf("Test %d %d\n", test1, test2);
                sector1.face[faceloop].v = test1;
            }
            else {
                sector1.face[faceloop].v = atoi(v4);
            }
            faceloop++;
            for (int i=0; i<9; i++) {
                v4[i] = '0';
            }
             v4[9] = '\0'; // TODO make this more pretty; moreover, why do they carry on?
//              printf("%d %d %d %d\n\n", sector1.face[faceloop-4].v, sector1.face[faceloop-3].v, sector1.face[faceloop-2].v, sector1.face[faceloop-1].v);
        }
        else if (faceloop != 0) { break; }
    }
    fclose(filein);
    
    printf("%d\n",vertloop);
    printf("%d\n",faceloop);

    numVertices = vertloop;    
    numIndices = faceloop;
    numNormals = normloop;
    Vertices = new WVector[numVertices];
    Indices = new GLuint[numIndices];
    Normals = new WVector[numNormals];
    
    for (int i=0; i<numVertices; i++) {
        Vertices[i].x = sector1.vertex[i].x;
        Vertices[i].y = sector1.vertex[i].y;
        Vertices[i].z = sector1.vertex[i].z; 
    }
    for (int i=0; i<numNormals; i++) {
        Normals[i].x = sector1.normal[i].x;
        Normals[i].y = sector1.normal[i].y;
        Normals[i].z = sector1.normal[i].z;
    }
    for (int i=0; i<faceloop; i++) {
        Indices[i] = sector1.face[i].v - 1;
    }
//     int evil = 0;
//     int asfd = 8/evil;
    return;
}
void DrawWorld() {
    glRotatef(90, 1, 0, 0);
    glVertexPointer( 3, GL_FLOAT, sizeof(WVector), Vertices);
    glNormalPointer( GL_FLOAT, sizeof(WVector), Normals );
    glDrawElements( GL_QUADS, numIndices, GL_UNSIGNED_INT, Indices );
    return;
}

