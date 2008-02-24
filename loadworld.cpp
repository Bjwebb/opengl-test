#include "main.h"

OBJECT *Objects;
int numObjects;

float world_x = 0;
float world_z = 0;
float world_angle = 0;

typedef struct tagVERTEX                        // Build Our Vertex Structure
{
    float x, y, z;                          // 3D Coordinates
    // float u, v;                         // Texture Coordinates
} VERTEX;

typedef struct tagFACE {
    int v;                          // 3D Vertices
    // float u, v;                         // Texture Coordinates
} FACE;

float getWorldX() {
    return world_x;
}
float getWorldZ() {
    return world_z;
}
float getWorldAngle() {
    return world_angle;
}

short readstr(FILE *f, char *string) {                 // Read In A String
    do
    {
        fgets(string, 255, f);
        if (feof(f)) return 1;
    } while ((string[0] == '#') || (string[0] == '\n'));
    return 0;
}

OBJECT LoadMtl(char* mtlfile, OBJECT tmpobj) {
    printf("### %d\n", tmpobj.numMtl);
    FILE *filein;                           // File To Work With
    filein = fopen(mtlfile, "rt");                // Open Our File
    tmpobj.mtl = new WMaterial[tmpobj.numMtl];
    bool content = false;
    for (int i=0; i<tmpobj.numMtl; i++) {
        float ar=0, ag=0, ab=0, dr=0, dg=0, db=0, sr=0, sg=0, sb=0;
        float ns=0;
        while (true) {
//            printf("Test\n");
            char oneline[255];
            char t1[255], t2[255], t3[255], t4[255];
            float f2=0, f3=0, f4=0;
            if (readstr(filein,oneline)) break;
            content = true;
            int num = sscanf(oneline, "%s %f %f %f", t1, &f2, &f3, &f4);
            if (strcmp(t1,"newmtl") == 0) {
                sscanf(oneline, "%s %s", t2, t3);
                printf("Material: %s\n", t3);
                tmpobj.mtl[i].name = new char[255];
                strcpy(tmpobj.mtl[i].name, t3);
            }
            if (strcmp(t1,"Ka") == 0) {
                ar = f2;
                ag = f3;
                ab = f4;
            }
            if (strcmp(t1,"Kd") == 0) {
                dr = f2;
                dg = f3;
                db = f4;
            }
            if (strcmp(t1,"Ks") == 0) {
                sr = f2;
                sg = f3;
                sb = f4;
            }
            if (strcmp(t1,"Ns") == 0) {
                ns = f2;
            }
            if (strcmp(t1,"illum") == 0) {
                break;
            }
        }
        tmpobj.mtl[i].ambient[0] = ar;
        tmpobj.mtl[i].ambient[1] = ag;
        tmpobj.mtl[i].ambient[2] = ab;
        tmpobj.mtl[i].ambient[3] = 1;
        tmpobj.mtl[i].diffuse[0] = dr;
        tmpobj.mtl[i].diffuse[1] = dg;
        tmpobj.mtl[i].diffuse[2] = db;
        tmpobj.mtl[i].diffuse[3] = 1;
        tmpobj.mtl[i].specular[0] = sr;
        tmpobj.mtl[i].specular[1] = sg;
        tmpobj.mtl[i].specular[2] = sb;
        tmpobj.mtl[i].specular[3] = 1;
        tmpobj.mtl[i].shininess[0] = ns;
        printf("$ %d\n", content);
        tmpobj.ismtl = content;
    }
    fclose(filein);
    return tmpobj;
}

OBJECT LoadObj(char* objfile, int bits) {
    printf("*** %d", bits);
    FILE *filein;                           // File To Work With
    filein = fopen(objfile, "rt");                // Open Our File
    
    char* mtllib;
    char oneline[255];
    char tmp1[255];
    char tmp2[255];
    readstr(filein,oneline);
    sscanf(oneline, "%s %s", tmp1, tmp2);
    if (strcmp(tmp1, "mtllib") == 0) {
        mtllib = tmp2;
    }
    
    int numvert = 10000;
    char x[10], y[10], z[10];
    int numface = 4*10000;

//     printf("Test 1\n");

    VERTEX* vertex = new VERTEX[numvert];
    int vertloop = 0;
    while (vertloop < numvert)        // Loop Through All The Vertices
    {
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "v") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);
        // Store Values Into Respective Vertices
            vertex[vertloop].x = atof(x);
            vertex[vertloop].y = atof(y);
            vertex[vertloop].z = atof(z);
            vertloop++;
        }
        //else if (vertloop != 0) { break; }
    }

    fclose(filein);
    filein = fopen(objfile, "rt"); 

    VERTEX* normal = new VERTEX[numvert];
    int normloop = 0;
    while (normloop < numvert) {
        if (readstr(filein,oneline)) break;
        char tmp[255];
        sscanf(oneline, "%s ", tmp);
        if (strcmp(tmp, "vn") == 0) {
            sscanf(oneline, "%s %s %s %s", tmp, &x, &y, &z);
        // Store Values Into Respective Vertices
            normal[normloop].x = atof(x);
            normal[normloop].y = atof(y);
            normal[normloop].z = atof(z);
            normloop++;
        }
        //else if (normloop != 0) { break; }
    }
    
    fclose(filein);
    filein = fopen(objfile, "rt"); 
    
//     printf("Test 2\n");
    
    int numgroup = 100; //TODO wtf
    FACE** face;
    face = new FACE*[100];
    for (int i=0; i<100; i++) face[i] = new FACE[40000]; //TODO wtf
    int grouploop = 0;
    int faceloop[numgroup];
//     printf("Test 3\n");
    bool loop = true;
    char** mtls;
    mtls = new char*[100]; // TODO replace these 100s with variable!
    for (int i=0; i<100; i++) mtls[i] = new char[256];
    bool skip = false;
    while (grouploop < numgroup && loop) {
//         printf("Test: Grouploop: %d\n", grouploop);
        faceloop[grouploop] = 0;
        
        char v1[10], v2[10], v3[10], v4[10];
        if (skip) skip = false;
        else {
            if (readstr(filein,oneline)) { break; }
        }
//         printf(oneline);
        char tmp[255];
        char tmp2[255];
        sscanf(oneline, "%s %s", tmp, tmp2);
        if (strcmp(tmp, "usemtl") == 0) {
            strcpy(mtls[grouploop], tmp2);
            printf("mtls[%d]: %s\n", grouploop, mtls[grouploop]);
        }
        else if (strcmp(tmp, "f") == 0) {
            skip = true;
            while (faceloop[grouploop] < numface) {
                if (skip) skip = false;
                else {
                    if (readstr(filein,oneline)) { loop=false; break; }
                }
//                 printf("Test: %d : %s", faceloop[grouploop], oneline);
                sscanf(oneline, "%s %s", tmp, tmp2);
//                 printf("Test: Faceloop 2\n");
                if (strcmp(tmp, "f") == 0) {
                    if (sscanf(oneline, "%s %s %s %s %s", tmp, &v1, &v2, &v3, &v4) == 4) {
                        strcpy (v4, v3);
                    }
                // Store Values Into Respective Vertices
                    int test1, test2;
//                     printf("Test: Faceloop 3\n");
                    if (sscanf(v1, "%d//%d", &test1, &test2) == 2) {
//                         printf("Test: Faceloop \n");
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v1);
                    }
//                     printf("Test: Faceloop 4\n");
                    faceloop[grouploop]++;
                    if (sscanf(v2, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v2);
                    }
                    faceloop[grouploop]++;
                    if (sscanf(v3, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v3);
                    }
                    faceloop[grouploop]++;
                    if (sscanf(v4, "%d//%d", &test1, &test2) == 2) {
                        face[grouploop][faceloop[grouploop]].v = test1;
                    }
                    else {
                        face[grouploop][faceloop[grouploop]].v = atoi(v4);
                    }
                    faceloop[grouploop]++;
                    for (int i=0; i<9; i++) {
                        v4[i] = '0';
                    }
                    v4[9] = '\0'; // TODO make this more pretty; moreover, why do they carry on?
        //             printf("Face: %d %d %d %d\n", face[faceloop-4].v, face[faceloop-3].v, face[faceloop-2].v, face[faceloop-1].v);
                }
                else if (faceloop[grouploop] != 0) {
                    if (strcmp(tmp, "usemtl") == 0) {                                   skip = true;
                    }
                    break;
                }
            }
            grouploop++;
            printf("%d %d\n", grouploop, loop);
        }
    }
    fclose(filein);
//     printf("Test A\n");

    OBJECT tmpobj;
    tmpobj.numMtl = bits;
    tmpobj.numGroups = grouploop;
    tmpobj.numVertices = vertloop;
    tmpobj.numNormals = normloop;
    printf("*********** Groups: %d Vertices: %d Normals: %d\n", tmpobj.numGroups, tmpobj.numVertices, tmpobj.numNormals);
    tmpobj.Vertices = new WVector[tmpobj.numVertices];
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
    
//     printf("Test B\n");
    
    tmpobj.groups = new WGroup[tmpobj.numGroups];
    for (int j=0; j<tmpobj.numGroups; j++) {
        tmpobj.groups[j].numIndices = faceloop[j];
        tmpobj.groups[j].Indices = new GLuint[tmpobj.groups[j].numIndices];
        tmpobj.groups[j].mtlname = mtls[j];
        printf("* %s\n", tmpobj.groups[j].mtlname);
        for (int i=0; i<tmpobj.groups[j].numIndices; i++) {
            tmpobj.groups[j].Indices[i] = face[j][i].v - 1;
        }
    }
    
//     printf("Test C\n");
    printf("Test A\n");
    tmpobj = LoadMtl(tmp2, tmpobj);
    printf("Test Z\n");
//     printf("Test D\n");

    printf("asdf %d\n", tmpobj.ismtl);
    if (tmpobj.ismtl) {
        for (int j=0; j<tmpobj.numGroups; j++) {
            tmpobj.groups[j].mtl = -1;
            printf("\n%s :O %d\n", tmpobj.groups[j].mtlname, tmpobj.numMtl);
            for (int c=0; c<tmpobj.numMtl; c++) {
                printf("%s\n", tmpobj.mtl[c].name);
                if (strcmp(tmpobj.groups[j].mtlname, tmpobj.mtl[c].name) == 0) {
                    tmpobj.groups[j].mtl = c;
                    printf("Success!");
                }
            }
            printf("'m' = %d\n", tmpobj.groups[j].mtl);
        }
    }


    return tmpobj;
}

void DrawWorld() {
    for (int i=0; i<numObjects; i++) {
        glLoadIdentity(); you_compensate();
        glTranslatef( Objects[i].pos.x, Objects[i].pos.y, Objects[i].pos.z );
        glRotatef(Objects[i].rot.x, 1, 0, 0);
        glRotatef(Objects[i].rot.y, 0, 1, 0);
        glRotatef(Objects[i].rot.z, 0, 0, 1);
        
        if (!Objects[i].ismtl) {
            GLfloat a[] = {0,0,0,1};
            GLfloat d[] = {0.8f,0.8f,0.8f,1};
            GLfloat s[] = {0.5f,0.5f,0.5f};
            GLfloat sh[] = {100};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, d);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, sh);
            glColor3fv( blue );
        }

        glVertexPointer( 3, GL_FLOAT, sizeof(WVector), Objects[i].Vertices);
        glNormalPointer( GL_FLOAT, sizeof(WVector), Objects[i].Normals );
        
        for (int j=0; j<Objects[i].numGroups; j++) {
            if (Objects[i].ismtl) {
                if (Objects[i].groups[j].mtl != -1) {
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Objects[i].mtl[Objects[i].groups[j].mtl].ambient);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Objects[i].mtl[Objects[i].groups[j].mtl].diffuse);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Objects[i].mtl[Objects[i].groups[j].mtl].specular);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Objects[i].mtl[Objects[i].groups[j].mtl].shininess);
                    glColor3fv(Objects[i].mtl[Objects[i].groups[j].mtl].diffuse);
                }
            }
            glDrawElements( GL_QUADS, Objects[i].groups[j].numIndices, GL_UNSIGNED_INT, Objects[i].groups[j].Indices );
        }
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
    if (!strcmp(tmp1, "Var") == 0) {
        printf("Sorry, this filetype/version is not supported.");
        exit(1);
    }
    while (true) {
        readstr(filein,oneline);
        sscanf(oneline, "%s %s", tmp1, tmp2);
        if (strcmp(tmp1, "numObjects") == 0) numObjects = atoi(tmp2);
        if (strcmp(tmp1, "X") == 0) world_x = atoi(tmp2);
        if (strcmp(tmp1, "Z") == 0) world_z = atoi(tmp2);
        if (strcmp(tmp1, "Angle") == 0) world_angle = atoi(tmp2);
        if (strcmp(tmp1, "endVar") == 0) break;
    }

    Objects = new OBJECT[numObjects];
    
    int i = 0;
    while (i < numObjects) {
        if (readstr(filein,oneline)) break;
        sscanf(oneline, "%s", tmp1);
//         printf("%s", oneline);
        if (strcmp(tmp1, "Object") == 0) {
            float x=0,y=0,z=0,rx=0,ry=0,rz=0;
            int bits=0;
             printf("Count: %d\n", sscanf(oneline, "%s %s %f %f %f %f %f %f %d", tmp1, tmp2, &x, &y, &z, &rx, &ry, &rz, &bits));
//             printf("Test: %s\n", tmp1);
//             printf("Object: %s\n", tmp2);
            Objects[i] = LoadObj(tmp2, bits);
//             printf("%f %f %f\n\n", x, y, z);
//             printf("%f %f %f\n\n", rx, ry, rz);
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

