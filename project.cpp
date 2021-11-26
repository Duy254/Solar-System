#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#define _USE_MATH_DEFINES
#include <math.h>
float Time;
#define MS_IN_THE_ANIMATION_CYCLE		10000

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.

// title of these windows:

const char *WINDOWTITLE = { "OpenGL / GLUT Sample -- Joe Graphics" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b

// initial window size:

const int INIT_WINDOW_SIZE = { 1000 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
        ORTHO,
        PERSP
};
enum Distort{
        NORMAL,
        DISTORT
};

enum Texture{
        TEXTURE,
        NO_TEXTURE,
        DISTORT_TEXTURE
};

// which button:

enum ButtonVals
{
        RESET,
        QUIT
};

enum View{
	OUTSIDE,
	INSIDE,
        SUN,
        EARTH
};
// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
        RED,
        YELLOW,
        GREEN,
        CYAN,
        BLUE,
        MAGENTA,
        WHITE,
        BLACK
};

char * ColorNames[ ] =
{
        (char *)"Red",
        (char *)"Yellow",
        (char *)"Green",
        (char *)"Cyan",
        (char *)"Blue",
        (char *)"Magenta",
        (char *)"White",
        (char *)"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] =
{
        { 1., 0., 0. },		// red
        { 1., 1., 0. },		// yellow
        { 0., 1., 0. },		// green
        { 0., 1., 1. },		// cyan
        { 0., 0., 1. },		// blue
        { 1., 0., 1. },		// magenta
        { 1., 1., 1. },		// white
        { 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


float White[ ] = { 1.,1.,1.,1. };
// utility to create an array from 3 separate values:
float *
Array3( float a, float b, float c )
{
static float array[4];
array[0] = a;
array[1] = b;
array[2] = c;
array[3] = 1.;
return array;
}
// utility to create an array from a multiplier and an array:
float *
MulArray3( float factor, float array0[3] )
{
static float array[4];
array[0] = factor * array0[0];
array[1] = factor * array0[1];
array[2] = factor * array0[2];
array[3] = 1.;
return array;
}


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to use the z-buffer
GLuint	BoxList;				// object display list
GLuint Tex0, Tex1, Tex2, Tex3, Tex4, Tex5, Tex6, Tex7, Tex8, Tex9, Tex10, Tex11;
int             width;
int             height;
int             mercury_width, mercury_height;
int             venus_width, venus_height;
int             earth_width, earth_height;
int             mars_width, mars_height;
int             jupiter_width, jupiter_height;
int             saturn_width, saturn_height;
int             uranus_width, uranus_height;
int             neptune_width, neptune_height;
int             pluto_width, pluto_height;
int             saturn_ring_width, saturn_ring_height;
int             universe_width, universe_height;

double      x, y;


unsigned char *BmpToTexture(char *, int *, int*);
unsigned char *TextureArray0 = BmpToTexture("sun.bmp", &width, &height);
unsigned char *TextureArray1 = BmpToTexture("mercury.bmp", &mercury_width, &mercury_height);
unsigned char *TextureArray2 = BmpToTexture("venus.bmp", &venus_width, &venus_height);
unsigned char *TextureArray3 = BmpToTexture("worldtex.bmp", &earth_width, &earth_height);
unsigned char *TextureArray4 = BmpToTexture("mars.bmp", &mars_width, &mars_height);
unsigned char *TextureArray5 = BmpToTexture("jupiter.bmp", &jupiter_width, &jupiter_height);
unsigned char *TextureArray6 = BmpToTexture("saturn.bmp", &saturn_width, &saturn_height);
unsigned char *TextureArray7 = BmpToTexture("uranus.bmp", &uranus_width, &uranus_height);
unsigned char *TextureArray8 = BmpToTexture("neptune.bmp", &neptune_width, &neptune_height);
unsigned char *TextureArray9 = BmpToTexture("ring.bmp", &saturn_ring_width, &saturn_ring_height);
unsigned char *TextureArray10 = BmpToTexture("universe.bmp", &universe_width, &universe_height);
unsigned char *TextureArray11 = BmpToTexture("pluto.bmp", &pluto_width, &pluto_height);



int		MainWindow;				// window id for main graphics window
float	        Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
int     whichMode;				// set the Distort or Normal mode
int     whichTexture;
double  translate = 30.0;
bool    Light0On;
bool    Light1On;
bool    Light2On;
bool    TextOn;
bool    Frozen;
int       whichView;




// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );
void	Axes( float );
void	HsvRgb( float[3], float [3] );
void    DoDistort( int );
void    DoViewMenu( int );


// main program:

int
main( int argc, char *argv[ ] )
{
        // turn on the glut package:
        // (do this before checking argc and argv since it might
        // pull some command line arguments out)

        glutInit( &argc, argv );


        // setup all the graphics stuff:

        InitGraphics( );


        // create the display structures that will not change:

        InitLists( );


        // init all the global variables used by Display( ):
        // this will also post a redisplay

        Reset( );


        // setup all the user interface stuff:

        InitMenus( );


        // draw the scene once and wait for some interaction:
        // (this will never return)

        glutSetWindow( MainWindow );
        glutMainLoop( );


        // this is here to make the compiler happy:


        return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
        // put animation stuff in here -- change some global variables
        // for Display( ) to find:

        int ms = glutGet(GLUT_ELAPSED_TIME); //milliseconds
        //ms %= MS_IN_THE_ANIMATION_CYCLE;
        Time  = (float) ms / (float)MS_IN_THE_ANIMATION_CYCLE; //[0., 1.]
        // force a call to Display( ) next time it is convenient:
        
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


int	ReadInt( FILE * );
short	ReadShort( FILE * );


struct bmfh
{
        short bfType;
        int bfSize;
        short bfReserved1;
        short bfReserved2;
        int bfOffBits;
} FileHeader;

struct bmih
{
        int biSize;
        int biWidth;
        int biHeight;
        short biPlanes;
        short biBitCount;
        int biCompression;
        int biSizeImage;
        int biXPelsPerMeter;
        int biYPelsPerMeter;
        int biClrUsed;
        int biClrImportant;
} InfoHeader;

const int birgb = { 0 };

unsigned char *
BmpToTexture( char *filename, int *width, int *height )
{

        int s, t, e;		// counters
        int numextra;		// # extra bytes each line in the file is padded with
        FILE *fp;
        unsigned char *texture;
        int nums, numt;
        unsigned char *tp;


        fp = fopen( filename, "rb" );
        if( fp == NULL )
        {
                fprintf( stderr, "Cannot open Bmp file '%s'\n", filename );
                return NULL;
        }

        FileHeader.bfType = ReadShort( fp );


        // if bfType is not 0x4d42, the file is not a bmp:

        if( FileHeader.bfType != 0x4d42 )
        {
                fprintf( stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType );
                fclose( fp );
                return NULL;
        }


        FileHeader.bfSize = ReadInt( fp );
        FileHeader.bfReserved1 = ReadShort( fp );
        FileHeader.bfReserved2 = ReadShort( fp );
        FileHeader.bfOffBits = ReadInt( fp );


        InfoHeader.biSize = ReadInt( fp );
        InfoHeader.biWidth = ReadInt( fp );
        InfoHeader.biHeight = ReadInt( fp );

        nums = InfoHeader.biWidth;
        numt = InfoHeader.biHeight;

        InfoHeader.biPlanes = ReadShort( fp );
        InfoHeader.biBitCount = ReadShort( fp );
        InfoHeader.biCompression = ReadInt( fp );
        InfoHeader.biSizeImage = ReadInt( fp );
        InfoHeader.biXPelsPerMeter = ReadInt( fp );
        InfoHeader.biYPelsPerMeter = ReadInt( fp );
        InfoHeader.biClrUsed = ReadInt( fp );
        InfoHeader.biClrImportant = ReadInt( fp );


        // fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );


        texture = new unsigned char[ 3 * nums * numt ];
        if( texture == NULL )
        {
                fprintf( stderr, "Cannot allocate the texture array!\b" );
                return NULL;
        }


        // extra padding bytes:

        numextra =  4*(( (3*InfoHeader.biWidth)+3)/4) - 3*InfoHeader.biWidth;


        // we do not support compression:

        if( InfoHeader.biCompression != birgb )
        {
                fprintf( stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression );
                fclose( fp );
                return NULL;
        }



        rewind( fp );
        fseek( fp, 14+40, SEEK_SET );

        if( InfoHeader.biBitCount == 24 )
        {
                for( t = 0, tp = texture; t < numt; t++ )
                {
                        for( s = 0; s < nums; s++, tp += 3 )
                        {
                                *(tp+2) = fgetc( fp );		// b
                                *(tp+1) = fgetc( fp );		// g
                                *(tp+0) = fgetc( fp );		// r
                        }

                        for( e = 0; e < numextra; e++ )
                        {
                                fgetc( fp );
                        }
                }
        }

        fclose( fp );

        *width = nums;
        *height = numt;
        return texture;
}

//READ BMP texture file

int
ReadInt( FILE *fp )
{
        unsigned char b3, b2, b1, b0;
        b0 = fgetc( fp );
        b1 = fgetc( fp );
        b2 = fgetc( fp );
        b3 = fgetc( fp );
        return ( b3 << 24 )  |  ( b2 << 16 )  |  ( b1 << 8 )  |  b0;
}


short
ReadShort( FILE *fp )
{
        unsigned char b1, b0;
        b0 = fgetc( fp );
        b1 = fgetc( fp );
        return ( b1 << 8 )  |  b0;
}

//Sphere function
bool	Distort;		// global -- true means to distort the texture
bool    TextureMode;                // global -- turn the texture on or off
struct point {
        float x, y, z;		// coordinates
        float nx, ny, nz;	// surface normal
        float s, t;		// texture coords
};

int		NumLngs, NumLats;
struct point *	Pts;

struct point *
PtsPointer( int lat, int lng )
{
        if( lat < 0 )	lat += (NumLats-1);
        if( lng < 0 )	lng += (NumLngs-1);
        if( lat > NumLats-1 )	lat -= (NumLats-1);
        if( lng > NumLngs-1 )	lng -= (NumLngs-1);
        return &Pts[ NumLngs*lat + lng ];
}



void
DrawPoint( struct point *p )
{
        glNormal3f( p->nx, p->ny, p->nz );
        glTexCoord2f( p->s, p->t );
        glVertex3f( p->x, p->y, p->z );
}

void
MjbSphere( float radius, int slices, int stacks )
{
        struct point top, bot;		// top, bottom points
        struct point *p;

        int a  = rand();
        // set the globals:

        NumLngs = slices;
        NumLats = stacks;

        if( NumLngs < 3 )
                NumLngs = 3;

        if( NumLats < 3 )
                NumLats = 3;


        // allocate the point data structure:

        Pts = new struct point[ NumLngs * NumLats ];


        // fill the Pts structure:

        for( int ilat = 0; ilat < NumLats; ilat++ )
        {
                float lat = -M_PI/2.  +  M_PI * (float)ilat / (float)(NumLats-1);
                float xz = cos( lat );
                float y = sin( lat );
                for( int ilng = 0; ilng < NumLngs; ilng++ )
                {
                        float lng = -M_PI  +  2. * M_PI * (float)ilng / (float)(NumLngs-1);
                        float x =  xz * cos( lng );
                        float z = -xz * sin( lng );
                        p = PtsPointer( ilat, ilng );
                        p->x  = radius * x;
                        p->y  = radius * y;
                        p->z  = radius * z;
                        p->nx = x;
                        p->ny = y;
                        p->nz = z;

                        if( Distort )
                        {
                                p->s = (( cos(lng*Time) + M_PI )  / (2. * M_PI )) ;
                                p->t = (( sin(lat/Time) + M_PI/2. )  / M_PI);
                        }
                        else
                        {
                                p->s = ( lng + M_PI    ) / ( 2.*M_PI );
                                p->t = ( lat + M_PI/2. ) / M_PI;
                        }
                }
        }

        top.x =  0.;		top.y  = radius;	top.z = 0.;
        top.nx = 0.;		top.ny = 1.;		top.nz = 0.;
        top.s  = 0.;		top.t  = 1.;

        bot.x =  0.;		bot.y  = -radius;	bot.z = 0.;
        bot.nx = 0.;		bot.ny = -1.;		bot.nz = 0.;
        bot.s  = 0.;		bot.t  =  0.;


        // connect the north pole to the latitude NumLats-2:

        glBegin( GL_QUADS );
        for( int ilng = 0; ilng < NumLngs-1; ilng++ )
        {
                p = PtsPointer( NumLats-1, ilng );
                DrawPoint( p );

                p = PtsPointer( NumLats-2, ilng );

                DrawPoint( p );

                p = PtsPointer( NumLats-2, ilng+1 );
                DrawPoint( p );

                p = PtsPointer( NumLats-1, ilng+1 );
                DrawPoint( p );
        }
        glEnd( );

        // connect the south pole to the latitude 1:

        glBegin( GL_QUADS );
        for( int ilng = 0; ilng < NumLngs-1; ilng++ )
        {
                p = PtsPointer( 0, ilng );
                DrawPoint( p );

                p = PtsPointer( 0, ilng+1 );
                DrawPoint( p );

                p = PtsPointer( 1, ilng+1 );
                DrawPoint( p );

                p = PtsPointer( 1, ilng );
                DrawPoint( p );
        }
        glEnd( );


        // connect the other 4-sided polygons:

        glBegin( GL_QUADS );
        for( int ilat = 2; ilat < NumLats-1; ilat++ )
        {
                for( int ilng = 0; ilng < NumLngs-1; ilng++ )
                {
                        p = PtsPointer( ilat-1, ilng );
                        DrawPoint( p );

                        p = PtsPointer( ilat-1, ilng+1 );
                        DrawPoint( p );

                        p = PtsPointer( ilat, ilng+1 );
                        DrawPoint( p );

                        p = PtsPointer( ilat, ilng );
                        DrawPoint( p );
                }
        }
        glEnd( );

        delete [ ] Pts;
        Pts = NULL;
}


void glutSolidSphere(GLdouble radius,
                     GLint slices, GLint stacks);
// draw the complete scene:

void glutSolidTorus(GLdouble innerRadius,
                    GLdouble outerRadius,
                    GLint nsides, GLint rings);
void glutSolidCube(GLdouble size);
void glutSolidCone(GLdouble base, GLdouble height,
                   GLint slices, GLint stacks);
void glutSolidDodecahedron(void);
void glutSolidIcosahedron(void);
void glutSolidSphere(GLdouble radius,
                     GLint slices, GLint stacks);

void
SetMaterial( float r, float g, float b, float shininess )
{
glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
glMaterialf ( GL_BACK, GL_SHININESS, 2.f );
glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );
glMaterialfv( GL_FRONT, GL_AMBIENT, Array3( r, g, b ) );
glMaterialfv( GL_FRONT, GL_DIFFUSE, Array3( r, g, b ) );
glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
glMaterialf ( GL_FRONT, GL_SHININESS, shininess );
}
void
SetPointLight( int ilight, float x, float y, float z, float r, float g, float b )
{
glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
glLightfv( ilight, GL_AMBIENT, Array3( 0.1, 0.1, 0.1 ) );
glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
glEnable( ilight );
}
void
SetPointLight2( int ilight, float x, float y, float z, float r, float g, float b )
{
glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
glEnable( ilight );
}

void
SetSpotLight( int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b )
{
glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
glLightfv( ilight, GL_SPOT_DIRECTION, Array3(xdir,ydir,zdir) );
glLightf( ilight, GL_SPOT_EXPONENT, 1. );
glLightf( ilight, GL_SPOT_CUTOFF, 45. );
glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
glEnable( ilight );
}


void
Display( )
{
        if( DebugOn != 0 )
        {
                fprintf( stderr, "Display\n" );
        }


        // set which window we want to do the graphics into:

        glutSetWindow( MainWindow );


        // erase the background:

        glDrawBuffer( GL_BACK );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        if( DepthBufferOn != 0 )
                glEnable( GL_DEPTH_TEST );
        else
                glDisable( GL_DEPTH_TEST );


        // specify shading to be flat:

        glShadeModel( GL_FLAT );


        // set the viewport to a square centered in the window:

        GLsizei vx = glutGet( GLUT_WINDOW_WIDTH *2 ) ;
        GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT * 2)  ;
        GLsizei v = vx < vy ? vx : vy;			// minimum dimension
        GLint xl = ( vx - v ) / 2;
        GLint yb = ( vy - v ) / 2;
        glViewport( xl, yb,  v, v );


        // set the viewing volume:
        // remember that the Z clipping  values are actually
        // given as DISTANCES IN FRONT OF THE EYE
        // USE gluOrtho2D( ) IF YOU ARE DOING 2D !

        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        if( WhichProjection == ORTHO )
                glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
        else
                gluPerspective( 90., 1.,	0.1, 1000.);
        

        if (whichTexture == TEXTURE){
                TextureMode = true;

        }
        else if( whichTexture == NO_TEXTURE){
                TextureMode = false;
                Distort = false;
        }

        if( whichMode == DISTORT)
        {
                Distort = true;
        }
        else
        {
                Distort = false;
        }


        // place the objects into the scene:

        Animate();

        // set the eye position, look-at position, and up-vector:

       //double xeye=earthOrbit*cos(rotateAngle)+EarthRadius*cos(spinAngle);
       //double zeye=earthOrbit*cos(rotateAngle)+EarthRadius*cos(spinAngle);
       ////double xeye=earthOrbit*cos(rotateAngle)+EarthRadius*cos(spinAngle);

	//if( whichView == SUN)
	//{
		//gluLookAt( 0, 0, 20,     0, 0, 20,     0, 1., 0. ); 
	//}
        ////else if (whichView == EARTH){
        ////        gluLookAt(xeye, 0, zeye,  0,0,0   , 0, 1, 0);
        ////}
	//else
	//{
		//gluLookAt( 0., 0., 20.,     0., 0., 0.,     0., 1., 0. ); 
	//}
        //gluLookAt( xeye., 0., zeye,     0., 0., 0.,     0., 1., 0. );
        gluLookAt( 0., 0., 3.,     0., 0., 0.,     0., 1., 0. );


        // rotate the scene:

        glRotatef( (GLfloat)Yrot, 0., 1., 0. );
        glRotatef( (GLfloat)Xrot, 1., 0., 0. );


        // uniformly scale the scene:

        if( Scale < MINSCALE )
                Scale = MINSCALE;
        glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );


        // set the fog parameters:

        if( DepthCueOn != 0 )
        {
                glFogi( GL_FOG_MODE, FOGMODE );
                glFogfv( GL_FOG_COLOR, FOGCOLOR );
                glFogf( GL_FOG_DENSITY, FOGDENSITY );
                glFogf( GL_FOG_START, FOGSTART );
                glFogf( GL_FOG_END, FOGEND );
                glEnable( GL_FOG );
        }
        else
        {
                glDisable( GL_FOG );
        }

       

        // possibly draw the axes:

        if( AxesOn != 0 )
        {
                glColor3fv( &Colors[WhichColor][0] );
                glCallList( AxesList );
        }


        // since we are using glScalef( ), be sure normals get unitized:
        


        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glLightfv( GL_LIGHT0, GL_POSITION, Array3(0, 0, 0) );

        // perform the rotations and scaling about the origin:
        glRotatef( Xrot, 1., 0., 0. );
        glRotatef( Yrot, 0., 1., 0. );
        glScalef( Scale, Scale, Scale );

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glPushMatrix();
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex10);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        //DoRasterString( 0., 1., 0., (char *)"Mercury" );
        glRotatef(90, 1, 0, 0);
        //glRotatef(360*Time*0.5, 0, 1, 0);
        glTranslatef(0,0,0);
        MjbSphere(1000, 500, 500);
        glDisable( GL_TEXTURE_2D);
        glPopMatrix();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        //Drawing object
        //Sun
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glTranslatef(0,0,0);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_BLEND );
        //glColor4f(1, 1, 1, 0.5);
        MjbSphere(15.0, 500, 500);
        MjbSphere(15.1, 500, 500);
        MjbSphere(15.2, 500, 500);
        MjbSphere(15.3, 500, 500);
        MjbSphere(15.5, 500, 500);
        MjbSphere(15.7, 500, 500);
        MjbSphere(15.9, 500, 500);
        MjbSphere(16.1, 500, 500);
        MjbSphere(16.1, 500, 500);
        MjbSphere(16.2, 500, 500);
        glColor4f(1,1,1,1);
        glDisable( GL_BLEND );
        glDisable( GL_TEXTURE_2D);
        glPopMatrix;

        //For Lighting
        glEnable( GL_LIGHTING );
        glEnable( GL_NORMALIZE );
        glEnable( GL_LIGHT0 );
        //glLightModelfv( GL_LIGHT_MODEL_AMBIENT, MulArray3( .3f, White ));
        glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
        SetPointLight(GL_LIGHT0, -3, 6, 0, 1, 1, 1);
        SetPointLight(GL_LIGHT1, 3, 6, -3, 1, 1, 1);
        SetPointLight(GL_LIGHT2, 3, -6, 3, 1, 1, 1);
        SetPointLight(GL_LIGHT3, -3, -6, 0, 1, 1, 1);
         //glDisable ( GL_LIGHTING);
        //Mercury
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glTranslatef(-0,0,0);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 20.0f * sin(angle);
                y = 20.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex1);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time* 1.607, 0, 1, 0);
        glTranslatef(-20,0,0);
        glRotatef(360*Time*0.0183, 0, 1, 0);
        MjbSphere(1.0, 500, 500); //0.2
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Mercury" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();

        ////Venus
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glTranslatef(0,0,0);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 24.5f * sin(angle);
                y = 24.5f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex2);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*1.174, 0, 1, 0);
        glTranslatef(-24.5,0,0);
        glRotatef(360*Time*0.0652, 0, 1, 0);
        MjbSphere(2.5, 500, 500); //0.5
        glDisable( GL_TEXTURE_2D);
        if(TextOn){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Venus" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();

        ////Earth
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 30.0f * sin(angle);
                y = 30.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex3);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time, 0, 1, 0);
        glTranslatef(-30,0,0);
        glRotatef(360*Time*1.674, 0, 1, 0);
        MjbSphere(2.7, 500, 500);  //0.54
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Earth" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();

        ///Mars
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 35.0f * sin(angle);
                y = 35.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex4);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*0.802, 0, 1, 0);
        glTranslatef(-35,0,0);
        glRotatef(360*Time*0.866, 0, 1, 0);
        MjbSphere(1.45,  500, 500); //0.29
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Mars" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();

        ////Jupiter
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 80.0f * sin(angle);
                y = 80.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex5);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*0.434, 0, 1, 0);
        glTranslatef(-80, 0,0);
        glRotatef(360*Time*45.583, 0, 1, 0);
        MjbSphere(8, 500, 500);
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Jupiter" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();

        ////Saturn
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 63.0f * sin(angle);
                y = 63.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
       glBindTexture(GL_TEXTURE_2D, Tex6);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*0.323, 0, 1, 0);
        glTranslatef(-63.0,0,0);
        //glPushMatrix();
        glRotatef(360*Time*36.840, 0, 1, 0);
        MjbSphere(7, 500, 500);
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Saturn" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glRotatef(-360*Time*36.840, 0, 1, 0);
        //glPopMatrix();
        glPushMatrix();
        glRotatef(120, 1, 0, 0);
        //glRotatef(60 * Time, 0, 0, 1);
        glScalef(1, 1, 0.5 );
        glDisable(GL_CULL_FACE);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex9);
        glColor3f(1., 1., 1.);

        glBegin( GL_TRIANGLES );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 8.0f * sin(angle);
                y = 8.0f * cos(angle);
                glTexCoord2f( angle / (2*M_PI), 0); 
               glVertex3f(x, y, 0.0f);
                x = 12.0f * sin(angle);
                y = 12.0f * cos(angle);
                glTexCoord2f( angle / (2*M_PI), 1); 
                glVertex3f(x, y, 0.0f);
                 x = 8.0f * sin(angle + 0.01);
                y = 8.0f * cos(angle + 0.01);
                glTexCoord2f( (angle + 0.01) / (2*M_PI), 0); 
                glVertex3f(x, y, 0.0f);

                 x = 8.0f * sin(angle + 0.01);
                y = 8.0f * cos(angle + 0.01);
                glTexCoord2f( (angle + 0.01) / (2*M_PI), 0); 
                glVertex3f(x, y, 0.0f);
                x = 12.0f * sin(angle);
                y = 12.0f * cos(angle);
                glTexCoord2f( angle / (2*M_PI), 1); 
                glVertex3f(x, y, 0.0f);
                x = 12.0f * sin(angle + 0.01);
                y = 12.0f * cos(angle + 0.01);
                glTexCoord2f( (angle + 0.01) / (2*M_PI), 1); 
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable(GL_CULL_FACE);
        glDisable( GL_TEXTURE_2D);
        glPopMatrix();
        
        glPopMatrix();
        

        ////Uranus
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 115.0f * sin(angle);
                y = 115.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex7);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*0.228, 0, 1, 0);
        glTranslatef(-115,0,0);
        glRotatef(360*Time*14.794, 0, 1, 0);
        MjbSphere(3, 500, 500); //2.185
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Uranus" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();

        //Neptune
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 380.0f * sin(angle);
                y = 380.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex8);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*0.182, 0, 1, 0);
        glTranslatef(-380.0,0,0);
        glRotatef(360*Time * 9, 0, 1, 0);
        MjbSphere(3, 500, 500); //2.91
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Neptune" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();
     
        //Pluto
        glPushMatrix();
        glDisable ( GL_LIGHTING);
        glRotatef(90, 1, 0, 0);
        glBegin( GL_POINTS );
        for (double angle = 0.0f; angle <= (2.0f * M_PI); angle += 0.01f)
        {
                x = 500.0f * sin(angle);
                y = 500.0f * cos(angle);
                glVertex3f(x, y, 0.0f);
        }
        glEnd();
        glEnable ( GL_LIGHTING);
        glEnable( GL_TEXTURE_2D );
        glBindTexture(GL_TEXTURE_2D, Tex11);
        glRotatef(90, 1, 0, 0);
        glRotatef(360*Time*0.182, 0, 1, 0);
        glTranslatef(-500.0,0,0);
        glRotatef(360*Time * 9, 0, 1, 0);
        MjbSphere(1, 500, 500); //2.91
        glDisable( GL_TEXTURE_2D);
        if( TextOn ){
        glDisable( GL_DEPTH_TEST );
        glDisable( GL_LIGHTING );
        glColor4f( 1. ,  1. ,  1., 1.);
        DoRasterString( 0., 1., 0., (char *)"Pluto" );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        }
        glPopMatrix();
     
 
        if( DepthFightingOn != 0 )
        {
                glPushMatrix( );
                        glRotatef( 90.,   0., 1., 0. );
                        glCallList( BoxList );
                glPopMatrix( );
        }


        // draw some gratuitous text that just rotates on top of the scene:

        glDisable( GL_DEPTH_TEST );
        glColor3f( 0., 1., 1. );
        //DoRasterString( 0., 1., 0., (char *)"Table" );


        // draw some gratuitous text that is fixed on the screen:
        //
        // the projection matrix is reset to define a scene whose
        // world coordinate system goes from 0-100 in each axis
        //
        // this is called "percent units", and is just a convenience
        //
        // the modelview matrix is reset to identity as we don't
        // want to transform these coordinates

        glDisable( GL_DEPTH_TEST );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );
        gluOrtho2D( 0., 100.,     0., 100. );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );
        glColor3f( 1., 1., 1. );
        DoRasterString( 5., 5., 0., (char *)"Duy Nguyen - Project Four" );


        // swap the double-buffered framebuffers:

        glutSwapBuffers( );


        // be sure the graphics buffer has been sent:
        // note: be sure to use glFlush( ) here, not glFinish( ) !

        glFlush( );
}


void
DoAxesMenu( int id )
{
        AxesOn = id;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}

void
DoViewMenu(int id)
{
	whichView = id;
	glutSetWindow( MainWindow );
	glutPostRedisplay( );	
}


void
DoColorMenu( int id )
{
        WhichColor = id - RED;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
        DebugOn = id;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
        DepthBufferOn = id;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
        DepthFightingOn = id;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
        DepthCueOn = id;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
        switch( id )
        {
                case RESET:
                        Reset( );
                        break;

                case QUIT:
                        // gracefully close out the graphics:
                        // gracefully close the graphics window:
                        // gracefully exit the program:
                        glutSetWindow( MainWindow );
                        glFinish( );
                        glutDestroyWindow( MainWindow );
                        exit( 0 );
                        break;
                

		

                default:
                        fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
        }

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
        WhichProjection = id;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}

void
DoDistort(int id)
{
        whichMode = id;
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}
void
DoTexture(int id)
{
        whichTexture = id;
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}
// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
        glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

        char c;			// one character to print
        for( ; ( c = *s ) != '\0'; s++ )
        {
                glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24,  c );
        }
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
        glPushMatrix( );
                glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
                float sf = ht / ( 119.05f + 33.33f );
                glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
                char c;			// one character to print
                for( ; ( c = *s ) != '\0'; s++ )
                {
                        glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
                }
        glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
        // get # of milliseconds since the start of the program:

        int ms = glutGet( GLUT_ELAPSED_TIME );

        // convert it to seconds:

        return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
        glutSetWindow( MainWindow );

        int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
        int colormenu = glutCreateMenu( DoColorMenu );
        for( int i = 0; i < numColors; i++ )
        {
                glutAddMenuEntry( ColorNames[i], i );
        }

        int axesmenu = glutCreateMenu( DoAxesMenu );
        glutAddMenuEntry( "Off",  0 );
        glutAddMenuEntry( "On",   1 );

        int depthcuemenu = glutCreateMenu( DoDepthMenu );
        glutAddMenuEntry( "Off",  0 );
        glutAddMenuEntry( "On",   1 );

        int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
        glutAddMenuEntry( "Off",  0 );
        glutAddMenuEntry( "On",   1 );

        int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
        glutAddMenuEntry( "Off",  0 );
        glutAddMenuEntry( "On",   1 );

        int debugmenu = glutCreateMenu( DoDebugMenu );
        glutAddMenuEntry( "Off",  0 );
        glutAddMenuEntry( "On",   1 );

        int projmenu = glutCreateMenu( DoProjectMenu );
        glutAddMenuEntry( "Orthographic",  ORTHO );
        glutAddMenuEntry( "Perspective",   PERSP );

        int viewmenu = glutCreateMenu( DoViewMenu );
	glutAddMenuEntry( "Sun",  SUN );
	glutAddMenuEntry( "Earth",   EARTH );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	
	glutAddMenuEntry( "Outside",   OUTSIDE );	

        int distort = glutCreateMenu( DoDistort );
        glutAddMenuEntry( "ON",  DISTORT );
        glutAddMenuEntry( "OFF", NORMAL );

        int texture = glutCreateMenu( DoTexture );
        glutAddMenuEntry( "Texture",  TEXTURE );
        glutAddMenuEntry( "No Texture", NO_TEXTURE );
        //glutAddMenuEntry( "Distort", DISTORT_TEXTURE );




        int mainmenu = glutCreateMenu( DoMainMenu );
        glutAddSubMenu(   "Axes",          axesmenu);
        glutAddSubMenu(   "Colors",        colormenu);
        glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
        glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
        glutAddSubMenu(   "Depth Cue",     depthcuemenu);
        glutAddSubMenu(   "Projection",    projmenu );
        glutAddSubMenu(   "View",    viewmenu );
        glutAddSubMenu(   "Distort",       distort );
        glutAddSubMenu(   "Texture",       texture );
        glutAddMenuEntry( "Reset",         RESET );
        glutAddSubMenu(   "Debug",         debugmenu);
        glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

        glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
        // request the display modes:
        // ask for red-green-blue-alpha color, double-buffering, and z-buffering:

        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

        // set the initial window configuration:

        glutInitWindowPosition( 0, 0 );
        glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

        // open the window and set its title:

        MainWindow = glutCreateWindow( WINDOWTITLE );
        glutSetWindowTitle( WINDOWTITLE );

        // set the framebuffer clear values:

        glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

        // setup the callback functions:
        // DisplayFunc -- redraw the window
        // ReshapeFunc -- handle the user resizing the window
        // KeyboardFunc -- handle a keyboard input
        // MouseFunc -- handle the mouse button going down or up
        // MotionFunc -- handle the mouse moving with a button down
        // PassiveMotionFunc -- handle the mouse moving with a button up
        // VisibilityFunc -- handle a change in window visibility
        // EntryFunc	-- handle the cursor entering or leaving the window
        // SpecialFunc -- handle special keys on the keyboard
        // SpaceballMotionFunc -- handle spaceball translation
        // SpaceballRotateFunc -- handle spaceball rotation
        // SpaceballButtonFunc -- handle spaceball button hits
        // ButtonBoxFunc -- handle button box hits
        // DialsFunc -- handle dial rotations
        // TabletMotionFunc -- handle digitizing tablet motion
        // TabletButtonFunc -- handle digitizing tablet button hits
        // MenuStateFunc -- declare when a pop-up menu is in use
        // TimerFunc -- trigger something to happen a certain time from now
        // IdleFunc -- what to do when nothing else is going on

        glutSetWindow( MainWindow );
        glutDisplayFunc( Display );
        glutReshapeFunc( Resize );
        glutKeyboardFunc( Keyboard );
        glutMouseFunc( MouseButton );
        glutMotionFunc( MouseMotion );
        glutPassiveMotionFunc( NULL );
        glutVisibilityFunc( Visibility );
        glutEntryFunc( NULL );
        glutSpecialFunc( NULL );
        glutSpaceballMotionFunc( NULL );
        glutSpaceballRotateFunc( NULL );
        glutSpaceballButtonFunc( NULL );
        glutButtonBoxFunc( NULL );
        glutDialsFunc( NULL );
        glutTabletMotionFunc( NULL );
        glutTabletButtonFunc( NULL );
        glutMenuStateFunc( NULL );
        glutTimerFunc( -1, NULL, 0 );
        glutIdleFunc( NULL );

        // init glew (a window must be open to do this):

#ifdef WIN32
        GLenum err = glewInit( );
        if( err != GLEW_OK )
        {
                fprintf( stderr, "glewInit Error\n" );
        }
        else
                fprintf( stderr, "GLEW initialized OK\n" );
        fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
        //generate random points        

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //Sun
        glGenTextures( 1, &Tex0 );
        glBindTexture(GL_TEXTURE_2D, Tex0); //make the Tex0 texture current and set its parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray0 );

        //Mercury
        glGenTextures( 1, &Tex1 );
        glBindTexture(GL_TEXTURE_2D, Tex1);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, mercury_width, mercury_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray1);

        //Venus
        glGenTextures( 1, &Tex2 );
        glBindTexture(GL_TEXTURE_2D, Tex2);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, venus_width, venus_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray2);
        
        //Earth

        glGenTextures( 1, &Tex3);
        glBindTexture(GL_TEXTURE_2D, Tex3);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, earth_width, earth_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray3);
        //Mars
        glGenTextures( 1, &Tex4);
        glBindTexture(GL_TEXTURE_2D, Tex4);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, mars_width, mars_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray4);
        //Jupiter
        glGenTextures( 1, &Tex5);
        glBindTexture(GL_TEXTURE_2D, Tex5);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, jupiter_width, jupiter_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray5);
        //Saturn
        glGenTextures( 1, &Tex6);
        glBindTexture(GL_TEXTURE_2D, Tex6);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, saturn_width, saturn_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray6);


        glGenTextures( 1, &Tex9);
        glBindTexture(GL_TEXTURE_2D, Tex9);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, saturn_ring_width, saturn_ring_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray9);


        //Uranus
        glGenTextures( 1, &Tex7);
        glBindTexture(GL_TEXTURE_2D, Tex7);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, uranus_width, uranus_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray7);

        //Neptune

        glGenTextures( 1, &Tex8);
        glBindTexture(GL_TEXTURE_2D, Tex8);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, neptune_width, neptune_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray8);

        //universe

        glGenTextures( 1, &Tex10);
        glBindTexture(GL_TEXTURE_2D, Tex10);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, universe_width, universe_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray10);

        //pluto

        glGenTextures( 1, &Tex11);
        glBindTexture(GL_TEXTURE_2D, Tex11);  //the text1 texture as the current texture and set its parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) ;
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) ;
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D( GL_TEXTURE_2D, 0, 3, pluto_width, pluto_height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray11);
}
// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
        float dx = BOXSIZE / 2.f;
        float dy = BOXSIZE / 2.f;
        float dz = BOXSIZE / 2.f;
        glutSetWindow( MainWindow );

        // create the object:

        BoxList = glGenLists( 1 );
        glNewList( BoxList, GL_COMPILE );
       
        glEndList();


        // create the axes:

        AxesList = glGenLists( 1 );
        glNewList( AxesList, GL_COMPILE );
                glLineWidth( AXES_WIDTH );
                        Axes( 1.5 );
                glLineWidth( 1. );
        glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
        if( DebugOn != 0 )
                fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

        switch( c )
        {
                case 'o':
                case 'O':
                        WhichProjection = ORTHO;
                        break;

                case 'p':
                case 'P':
                        WhichProjection = PERSP;
                        break;

                case 'q':
                case 'Q':
                case ESCAPE:
                        DoMainMenu( QUIT );	// will not return here
                        break;				// happy compiler   

                case 'f':
	        case 'F':
		Frozen = ! Frozen;
		if( Frozen )
			glutIdleFunc( NULL );
		else
			glutIdleFunc( Animate );
		break;
                
                case '0':
                        TextOn = !TextOn;
                        break;
                case '1':
                        Light1On = !Light1On;
                        break;
                case '2':
                        Light2On = !Light2On;
                        break;


                default:
                        fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
        }

        // force a call to Display( ):

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
        int b = 0;			// LEFT, MIDDLE, or RIGHT

        if( DebugOn != 0 )
                fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );


        // get the proper button bit mask:

        switch( button )
        {
                case GLUT_LEFT_BUTTON:
                        b = LEFT;		break;

                case GLUT_MIDDLE_BUTTON:
                        b = MIDDLE;		break;

                case GLUT_RIGHT_BUTTON:
                        b = RIGHT;		break;

                default:
                        b = 0;
                        fprintf( stderr, "Unknown mouse button: %d\n", button );
        }


        // button down sets the bit, up clears the bit:

        if( state == GLUT_DOWN )
        {
                Xmouse = x;
                Ymouse = y;
                ActiveButton |= b;		// set the proper bit
        }
        else
        {
                ActiveButton &= ~b;		// clear the proper bit
        }
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
        if( DebugOn != 0 )
                fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


        int dx = x - Xmouse;		// change in mouse coords
        int dy = y - Ymouse;

        if( ( ActiveButton & LEFT ) != 0 )
        {
                Xrot += ( ANGFACT*dy );
                Yrot += ( ANGFACT*dx );
        }


        if( ( ActiveButton & MIDDLE ) != 0 )
        {
                Scale += SCLFACT * (float) ( dx - dy );

                // keep object from turning inside-out or disappearing:

                if( Scale < MINSCALE )
                        Scale = MINSCALE;
        }

        Xmouse = x;			// new current position
        Ymouse = y;

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
        ActiveButton = 0;
        AxesOn = 1;
        DebugOn = 0;
        DepthBufferOn = 1;
        DepthFightingOn = 0;
        DepthCueOn = 0;
        Scale  = 1.0;
        WhichColor = WHITE;
        WhichProjection = PERSP;
        whichView = OUTSIDE;
        Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
        if( DebugOn != 0 )
                fprintf( stderr, "ReSize: %d, %d\n", width, height );

        // don't really need to do anything since window size is
        // checked each time in Display( ):

        glutSetWindow( MainWindow );
        glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
        if( DebugOn != 0 )
                fprintf( stderr, "Visibility: %d\n", state );

        if( state == GLUT_VISIBLE )
        {
                glutSetWindow( MainWindow );
                glutPostRedisplay( );
        }
        else
        {
                // could optimize by keeping track of the fact
                // that the window is not visible and avoid
                // animating or redrawing it ...
        }
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
                0.f, 1.f, 0.f, 1.f
              };

static float xy[ ] = {
                -.5f, .5f, .5f, -.5f
              };

static int xorder[ ] = {
                1, 2, -3, 4
                };

static float yx[ ] = {
                0.f, 0.f, -.5f, .5f
              };

static float yy[ ] = {
                0.f, .6f, 1.f, 1.f
              };

static int yorder[ ] = {
                1, 2, 3, -2, 4
                };

static float zx[ ] = {
                1.f, 0.f, 1.f, 0.f, .25f, .75f
              };

static float zy[ ] = {
                .5f, .5f, -.5f, -.5f, 0.f, 0.f
              };

static int zorder[ ] = {
                1, 2, 3, 4, -5, 6
                };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
        glBegin( GL_LINE_STRIP );
                glVertex3f( length, 0., 0. );
                glVertex3f( 0., 0., 0. );
                glVertex3f( 0., length, 0. );
        glEnd( );
        glBegin( GL_LINE_STRIP );
                glVertex3f( 0., 0., 0. );
                glVertex3f( 0., 0., length );
        glEnd( );

        float fact = LENFRAC * length;
        float base = BASEFRAC * length;

        glBegin( GL_LINE_STRIP );
                for( int i = 0; i < 4; i++ )
                {
                        int j = xorder[i];
                        if( j < 0 )
                        {

                                glEnd( );
                                glBegin( GL_LINE_STRIP );
                                j = -j;
                        }
                        j--;
                        glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
                }
        glEnd( );

        glBegin( GL_LINE_STRIP );
                for( int i = 0; i < 5; i++ )
                {
                        int j = yorder[i];
                        if( j < 0 )
                        {

                                glEnd( );
                                glBegin( GL_LINE_STRIP );
                                j = -j;
                        }
                        j--;
                        glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
                }
        glEnd( );

        glBegin( GL_LINE_STRIP );
                for( int i = 0; i < 6; i++ )
                {
                        int j = zorder[i];
                        if( j < 0 )
                        {

                                glEnd( );
                                glBegin( GL_LINE_STRIP );
                                j = -j;
                        }
                        j--;
                        glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
                }
        glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
        // guarantee valid input:

        float h = hsv[0] / 60.f;
        while( h >= 6. )	h -= 6.;
        while( h <  0. )        h += 6.;

        float s = hsv[1];
        if( s < 0. )
                s = 0.;
        if( s > 1. )
                s = 1.;

        float v = hsv[2];
        if( v < 0. )
                v = 0.;
        if( v > 1. )
                v = 1.;

        // if sat==0, then is a gray:

        if( s == 0.0 )
        {
                rgb[0] = rgb[1] = rgb[2] = v;
                return;
        }

        // get an rgb from the hue itself:

        float i = floor( h );
        float f = h - i;
        float p = v * ( 1.f - s );
        float q = v * ( 1.f - s*f );
        float t = v * ( 1.f - ( s * (1.f-f) ) );

        float r, g, b;			// red, green, blue
        switch( (int) i )
        {
                case 0:
                        r = v;	g = t;	b = p;
                        break;

                case 1:
                        r = q;	g = v;	b = p;
                        break;

                case 2:
                        r = p;	g = v;	b = t;
                        break;

                case 3:
                        r = p;	g = q;	b = v;
                        break;

                case 4:
                        r = t;	g = p;	b = v;
                        break;

                case 5:
                        r = v;	g = p;	b = q;
                        break;
        }


        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
}
