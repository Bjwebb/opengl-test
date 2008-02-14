#include "main.h"

float cube_angle = 0.0f;

float you_angle = 0.0f;
float you_x = 0.0f;
float you_z = 3.0f;
float you_vel_angle = 0.0f;
float you_vel_x = 0.0f;
float you_vel_z = 0.0f;
float you_velocity = 10;
float you_turn = 0;
float you_dir = 0;

float piover180 = 0.01745329251994329577;

bool light = true;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };

static GLint T0     = 0;
static GLint Frames = 0;
GLfloat fps = 500;

void quit_app( int code ) {
    // Return to previous resolution etc.
    SDL_Quit( );
    exit( code );
}

void handle_keydown( SDL_keysym* keysym ) {
    switch (keysym->sym) {
        case SDLK_UP:
            you_dir = -1;
            break;
        case SDLK_DOWN:
            you_dir = 1;
            break;
        case SDLK_LEFT:
            you_turn = -1;
            break;
        case SDLK_RIGHT:
            you_turn = 1;
            break;
        case SDLK_KP_PLUS:
            you_velocity++;
//             printf("%f\n", you_velocity);
            break;
        case SDLK_KP_MINUS:
            you_velocity--;
//             printf("%f\n", you_velocity);
            break;
        case SDLK_q:
        case SDLK_ESCAPE:
            quit_app( 0 );
            break;
        case SDLK_l:
            /* 'l' key was pressed
            * this toggles the light
            */
            light = !light;
            if ( !light )
            glDisable( GL_LIGHTING );
            else
            glEnable( GL_LIGHTING );
            break;
        default:
            break;
    }
}

void handle_keyup( SDL_keysym* keysym ) {
    switch (keysym->sym) {
        case SDLK_UP:
        case SDLK_DOWN:
            you_dir = 0;
            break;
        case SDLK_LEFT:
        case SDLK_RIGHT:
            you_turn = 0;
            break;
    }
}


void process_events( void ) {
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_KEYDOWN:
            handle_keydown( &event.key.keysym );
            break;
        case SDL_KEYUP:
            handle_keyup( &event.key.keysym );
            break;
        case SDL_QUIT:
            quit_app( 0 );
            break;
        }

    }
}

void you_compensate( void ) {
    glRotatef( you_angle, 0.0f, 1.0f, 0.0f );
    glTranslatef( -you_x, -1, -you_z);
}

void draw_cube( void ) {
    glRotatef( cube_angle, 1.0f, 0.0f, 0.0f );
     if( ++cube_angle > 360.0f ) {
         cube_angle = 0.0f;
     }

    GLfloat cube[] = {
        // FRONT
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        // BACK
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        // LEFT
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        // RIGHT
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        // TOP
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        // BOTTOM
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f
    };
    
    glVertexPointer(3, GL_FLOAT, 0, cube);
    //glDrawElements( GL_TRIANGLES, 8, GL_UNSIGNED_BYTE, &cube_i );
   glColor3fv( red );
   glNormal3f(0, 0, 1);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glNormal3f(0, 0, -1);
   glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
   glColor3fv( green );
   glNormal3f(-1, 0, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
   glNormal3f(1, 0, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
   glColor3fv( blue );
   glNormal3f(0, 1, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
   glNormal3f(0, -1, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
}

void draw_screen( void ) {
    // Step
    you_angle += ( 10 * you_velocity * you_turn ) / fps;
//     printf("Angle: %f", you_angle);
    
    float you_x_new = you_x - ( (float)sin(you_angle*piover180) * you_velocity * you_dir ) / fps;
    float you_z_new = you_z + ( (float)cos(you_angle*piover180) * you_velocity * you_dir ) / fps;
    if (boundCheck(you_x_new-1, you_x_new+1, you_z_new-2, you_z_new+2)) {
        you_x = you_x_new;
        you_z = you_z_new;
    }
//     printf("Position: (%f, %f)\n", you_x, you_z);
//     printf("%f\n", you_velocity);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen and The Depth Buffer
    /* We don't want to modify the projection matrix. */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity(); you_compensate();
    
    
    /*glBegin( GL_TRIANGLES );                      // Drawing Using Tricube_angles
    glColor3fv( green );
    glVertex3f( 0.0f, 1.0f, 0.0f);
    glColor3fv( red );
    glVertex3f(-1.0f,-1.0f, 0.0f);
    glColor3fv( blue );
    glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd( );*/
    
    // Draw Floor
    glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;
    
    glBegin( GL_QUADS );
    glNormal3f( 0.0f,1.0f, 0.0f);
    glColor3fv( green );
//     GLfloat *col  = new GLfloat[3];
//     col[0] = 0; col[1] = 255; col[2] = 0;
//     glMaterialfv( GL_FRONT, GL_COLOR_INDEXES, col );
    glVertex3f( 10, 0, -10);
    glColor3fv( red );
    glVertex3f(-10, 0, -10);
    glColor3fv( blue );
    glVertex3f( -10, 0, 10);
    glColor3fv( yellow );
    glVertex3f( 10, 0, 10);
    
    glNormal3f( 0.0f,-1.0f, 0.0f);
    glColor3fv( yellow );
    glVertex3f( 10, 0, 10);
    glColor3fv( blue );
    glVertex3f( -10, 0, 10);
    glColor3fv( red );
    glVertex3f(-10, 0, -10);
    glColor3fv( green );
    glVertex3f( 10, 0, -10);
    glEnd( );

    glLoadIdentity(); you_compensate();
    glTranslatef( 0.0f, 1.0f, 6.0f );
    draw_cube();
 
    glLoadIdentity(); you_compensate();
    int numIndices = 8;
    int numVertices = 4;
    WVector* Vertices = new WVector[numVertices];
    Vertices[0].x = 2;
    Vertices[0].y = 2;
    Vertices[0].z = 0;
    Vertices[1].x = -10;
    Vertices[1].y = 2;
    Vertices[1].z = 0;
    Vertices[2].x = -10;
    Vertices[2].y = 0;
    Vertices[2].z = 0;
    Vertices[3].x = 2;
    Vertices[3].y = 0;
    Vertices[3].z = 0;
    GLfloat* Normals = new GLfloat[numVertices*3];
    Normals[0] = 0;
    Normals[1] = -1;
    Normals[2] = 0;
    Normals[3] = 0;
    Normals[4] = -1;
    Normals[5] = 0;
    Normals[6] = 0;
    Normals[7] = -1;
    Normals[8] = 0;
    Normals[9] = 0;
    Normals[10] = -1;
    Normals[11] = 0;
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
    
    wallCreate(-10, 0, 0, 2, 2, 0);
    DrawWorld();
    
    glLoadIdentity(); glTranslatef(0, 0, -1);
    glColor3f(1, 1, 1);
    glRasterPos2f(0.3f, -0.5f);
    glPrint("FPS: %f", fps);

    /* Gather our frame
            printf("%f\n", you_velocity);s per second */
    Frames++;
    GLint t = SDL_GetTicks();
    if (t - T0 >= 100) {
        GLfloat seconds = (t - T0) / 1000.0;
        fps = Frames / seconds;
//         printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
        T0 = t;
        Frames = 0;
    }
    
    SDL_GL_SwapBuffers( );
}

static void setup_opengl( int width, int height ) {
    float ratio = (float) width / (float) height;
    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );
    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );
    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );
    /* Setup our viewport. */
    glViewport( 0, 0, width, height );
    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
    glEnable(GL_LIGHT1);
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
    
    buildFont();
    return;
}

int main( int argc, char* argv[] ) {
    bool fullscreen = false;
    char* world = "monkey.obj";
    for (int i=0; i<argc; i++) {
        if (strcmp(argv[i], "-win") == 0) fullscreen = false;
        if (strcmp(argv[i], "-f") == 0) fullscreen = true;
        if (strcmp(argv[i], "-w") == 0)
            world = argv[i+1];
    }
    SetupWorld(world);

    /* Information about the current video settings. */
    const SDL_VideoInfo* info = NULL;
    /* Dimensions of our window. */
    int width = 0;
    int height = 0;
    /* Color depth in bits of our window. */
    int bpp = 0;
    /* Flags we will pass into SDL_SetVideoMode. */
    int flags = 0;
    
        /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        quit_app( 1 );
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );

    if( !info ) {
        /* This should probably never happen. */
        fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
        quit_app( 1 );
    }

    width = 640;
    height = 480;
    bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    flags = SDL_OPENGL;
    if (fullscreen)
        flags |= SDL_FULLSCREEN;

    /*
     * Set the video mode
     */
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
        quit_app( 1 );
    }

    setup_opengl( width, height );
    bool init = true;
    while( 1 ) {
        process_events( );
        draw_screen( );
        if (init) { glEnable( GL_LIGHTING ); init=false; }
    }
    
    return 0;
}
