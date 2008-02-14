// http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=13

#include "main.h"

#include <stdarg.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

GLuint  base; /* Base Display List For The Font Set */

/* function to build our font list */
void buildFont( ) {
    Display *dpy;          /* Our current X display */
    XFontStruct *fontInfo; /* Our font info */

    /* Sotrage for 96 characters */
    base = glGenLists( 96 );

    /* Get our current display long enough to get the fonts */
    dpy = XOpenDisplay( NULL );

    /* Get the font information */
    fontInfo = XLoadQueryFont( dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1" );

    /* If the above font didn't exist try one that should */
    if ( fontInfo == NULL )
    {
        fontInfo = XLoadQueryFont( dpy, "fixed" );
        /* If that font doesn't exist, something is wrong */
        if ( fontInfo == NULL )
        {
            fprintf( stderr, "no X font available?\n" );
            exit(2);
        }
    }

    /* generate the list */
    glXUseXFont( fontInfo->fid, 32, 96, base );

    /* Recover some memory */
    XFreeFont( dpy, fontInfo );

    /* close the display now that we're done with it */
    XCloseDisplay( dpy );

    return;
}

/* Print our GL text to the screen */
void glPrint( const char *fmt, ... ) {
    char text[256]; /* Holds our string */
    va_list ap;     /* Pointer to our list of elements */

    /* If there's no text, do nothing */
    if ( fmt == NULL )
    return;

    /* Parses The String For Variables */
    va_start( ap, fmt );
      /* Converts Symbols To Actual Numbers */
      vsprintf( text, fmt, ap );
    va_end( ap );

    /* Pushes the Display List Bits */
    glPushAttrib( GL_LIST_BIT );

    /* Sets base character to 32 */
    glListBase( base - 32 );

    /* Draws the text */
    glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
    
//     printf("Test: %s\n", text);

    /* Pops the Display List Bits */
    glPopAttrib( );
}
