#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include "include/BmpLoader.h"
#include <bits/stdc++.h>
using namespace std;
const double PI = 3.14159265389;

int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt=1;
int animat = 0;
const int L=20;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 20;
double heli=0.0,hlx=0,hly=0,hlz=0,hrot=0;


GLfloat ctrlpoints[L+1][3] =
{
    { 0.0, 0.0, 0.0}, { 0.1, 0.2, 0.0},
    { 0.2, 0.5, 0.0},{ 0.3, 0.8, 0.0},
    {0.4, 1.1, 0.0}, {0.5, 1.5, 0.0},
    {3.0, 2.5, 0.0},{3.5, 2.5, 0.0},
    {4.0, 2.5, 0.0}, {4.5, 2.5, 0.0},
    {7.0, 2.5, 0.0},{7.5, 2.5, 0.0},
    {8.0, 2.0, 0.0},{8.5, 2.0, 0.0},
    {8.6, 1.5, 0.0},{8.7, 1.0, 0.0},
    {8.9, 0.5, 0.0},{9.0, 0.0, 0.0},
    {9.1, -0.2, 0.0},{9.2, -0.5, 0.0},
    //{9.5, 1.0, 0.0}
};
float wcsClkDn[3],wcsClkUp[3];
///////////////////////////////
class point1
{
public:
    point1()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];
int flag=0;
GLint viewport[4]; //var to hold the viewport info
GLdouble modelview[16]; //var to hold the modelview info
GLdouble projection[16]; //var to hold the projection matrix info

//////////////////////////
void scsToWcs(float sx,float sy, float wcsv[3] );
void processMouse(int button, int state, int x, int y);
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);
///////////////////////////

void scsToWcs(float sx,float sy, float wcsv[3] )
{

    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

    winX = sx;
    winY = (float)viewport[3] - (float)sy;
    winZ = 0;

    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX;
    wcsv[1]=worldY;
    wcsv[2]=worldZ;


}
//control points
long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}
void BezierCurve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}
void bottleBezier()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints[19][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}
const int width = 800;
const int height = 800;
const float rat = 1.0 * width / height;
GLfloat eyeX = 100;//21.5;//50;
GLfloat eyeY = 50;//25;//50;
GLfloat eyeZ = 150;//300;//5;//100;

GLfloat lookX = 100;//21.5;//50;
GLfloat lookY = 50;//40;//50;
GLfloat lookZ = 500;//30;//0;
GLfloat tmpx,tmpy,tmpz,m=0;
unsigned int ID;
bool light1= true, light2 = true, light3 =true,p=false,M=false;
bool ambient =true, diffuse = true, specular = true;
double movex[15],plx,plz,prot=0,fanim=0;//movex=0,movex2=0,movex3=0,movex4=0,movex5=0;
bool on=true,on2=true,on3=true,on4=true,on5=true,on6=true,on7=true,on8=true,on9=true,view=true;
static GLfloat v_cube[8][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.0, 1.0},
    {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0}

};

static GLubyte quadIndices[6][4] =
{
    {0, 2, 6, 4},
    {1, 5, 7, 3},
    {0, 4, 5, 1},
    {2, 3, 7, 6},
    {0, 1, 3, 2},
    {4, 6, 7, 5}
};
static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}
//void my_mouse(int button, int state, int x, int y)
//{
//    switch (button)
//    {
//    case GLUT_LEFT_BUTTON:
//        if (state == GLUT_DOWN)
//        {
//
//        }
//        //glutIdleFunc(spinDisplay_left);
//        else if(state == GLUT_UP)
//        {
//
//        }
//            glutIdleFunc(NULL);
//        break;
//    case GLUT_RIGHT_BUTTON:
//        if (state == GLUT_DOWN)
//        {
//
//        }
//        //glutIdleFunc(spinDisplay_right);
//        else if(state == GLUT_UP)
//        {
//
//        }
//            glutIdleFunc(NULL);
//        //glutIdleFunc(NULL); // make idle function inactive
//        break;
//    default:
//        break;
//    }
//}
void drawtree(int n)
{
    if(n>0)
    {
        //////////////////////////
        //stack of matrices
        glPushMatrix();

        glTranslatef(100,30.0,50);
        glRotatef(45, 0.0, 0.0, 1.0);

        // 1/sqrt(2)=0.707
        glScalef(0.707,0.707,0.707);

        //recursion
        drawtree(n-1);

        glPopMatrix();
        ////////////////////////////

        glPushMatrix();
        glTranslatef(-100,30.0,50);
        glRotatef(-45, 0.0, 0.0, 1.0);
        glScalef(0.707,0.707,0.707);
        drawtree(n-1);
        glPopMatrix();
        //draw a cube
        glutSolidCube(1);
    }
}

void treeInit(int n)
{
    // draw a red tree
    glColor3f(1.0, 0.0, 0.0);
    drawtree(n);
}

void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}
void drawCube(float colorRed=1, float colorGreen=1, float colorBlue=1, bool on=false)
{
    GLfloat no_mat[] = { 0, 0, 0, 1.0 };
    GLfloat mat_ambient[] = { colorRed, colorGreen, colorBlue, 1.0 };
    GLfloat mat_diffuse[] = { colorRed,colorGreen, colorBlue, 1.0 };
    GLfloat mat_specular[] = { 1, 1, 1, 1.0 };
    GLfloat mat_shininess[] = {50};

    if(ambient)
    {
        glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    }
    else
    {
        glMaterialfv( GL_FRONT, GL_AMBIENT, no_mat);
    }

    if(diffuse)
    {
        glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);

    }
    else
    {
        glMaterialfv( GL_FRONT, GL_DIFFUSE, no_mat);
    }

    if(specular)
    {
        glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    }
    else
    {
        glMaterialfv( GL_FRONT, GL_SPECULAR, no_mat);
    }

    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
                    v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
                    v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        glTexCoord2f(0,1);
        glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }
    glEnd();

}
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, width/rat);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 150.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void player(void)
{
    //glRotatef(prot,0,1,0);
    glTranslatef(plx,0,plz);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,19);
    glPushMatrix();
    glTranslatef(0,16,0);
    glScalef(3,10,3);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,20);
    glPushMatrix();
    glTranslatef(0,9,1);
    glScalef(1,6,1);
    drawCube();
    glPopMatrix();

    ///{
    //glPushMatrix();
    //glTranslatef(0,0,-2);
    //glRotatef(10,1,0,0);
    glPushMatrix();
    glTranslatef(2,9,1);
    glScalef(1,6,1);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,1,1);
    glScalef(1,7,1);
    drawCube();
    glPopMatrix();
    //glPopMatrix();
    ///}

    glPushMatrix();
    glTranslatef(0,1,1);
    glScalef(1,7,1);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0,0,-2);
    glScalef(1,1,3);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,0,-2);
    glScalef(1,1,3);
    drawCube();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,19);
    glPushMatrix();
    glTranslatef(-2,21,0);
    glScalef(1,5,1);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4,21,0);
    glScalef(1,5,1);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,21);
    glPushMatrix();
    glTranslatef(-1.5,19,0);
    glRotatef(250,1,0,0);
    glRotatef(-360,0,1,0);
    glScalef(1,7,1);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4,19,0);
    glRotatef(250,1,0,0);
    glRotatef(360,0,1,0);
    glScalef(1,7,1);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5,27,0.5);
    glScalef(2,3,2);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glRotatef(fanim,1,0,0);
    glPushMatrix();
    glTranslatef(1,20,-10);
    glScalef(0.5,0.5,10);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    //glTranslatef(1,20,-100);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(1.0f, 20.0f, -10.0f);
    glVertex3f(1.0f, 15.0f, -180.0f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1,17.5,-7);
    glRotatef(45,1,0,0);
    glScalef(0.5,4,0.5);
    drawCube();
    glPopMatrix();
    glPopMatrix();
}
static void house(void)
{
/// legs
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,9);
//    for(double i=3.5; i<40.0; i+=12)
//    {
//        for(double j=5; j<40; j+=10)
//        {
//            glPushMatrix();
//            glTranslatef(i,0,j);
//            glScalef(2.0, 10.5, 2.0);
//            drawCube();
//            glPopMatrix();
//        }
//    }
//    glDisable(GL_TEXTURE_2D);
/// Stairs
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,3);
//    glPushMatrix();
//    glTranslatef(21.5,0.0,40.0);
//    glScalef(15.0, 3.0, 4.0);
//    drawCube();
//    glPopMatrix();
//    glPushMatrix();
//    glTranslatef(21.5,3.0,39.0);
//    glScalef(15.0, 4.0, 4.0);
//    drawCube(0.1,0.9,0.2);
//    glPopMatrix();
//    glPushMatrix();
//    glTranslatef(21.5,7.0,38.0);
//    glScalef(15.0, 3.0, 4.0);
//    drawCube(1,0,1);
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
/// Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glPushMatrix();
    glTranslatef(1.5,10,1.0);
    glScalef(40.0, 1.0, 40.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Left Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(1.5,10,1.0);
    glScalef(1.0, 50.0, 40.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Front Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(1.5,10,1.0);
    glScalef(40.0, 50.0, 1.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Back Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(1.5,10,40);
    glScalef(20.0, 50.0, 1.0);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(36.5,10,40);
    glScalef(5.0, 50.0, 1.0);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(21.5,40,40);
    glScalef(15, 20.0, 1.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Door
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,15);
    glPushMatrix();
    glTranslatef(21.5,10,40);
    glRotatef(90,0,1,0);
    glScalef(15, 30.0, 1.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

/// Right Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(40.5, 10, 1);
    glScalef(1.0, 18.0, 40.0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(40.5, 28, 1);
    glScalef(1.0, 14.0, 10.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Window
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,15);
    glPushMatrix();
    glTranslatef(40.5, 28, 11);
    glRotatef(-90,0,1,0);
    glScalef(1.0, 14.0, 15.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(40.5, 28, 26);
    glScalef(1.0, 14.0, 14.0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(40.5, 42, 1);
    glScalef(1.0, 18.0, 40.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Roof
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(1.5, 60, 0);
    glRotatef(33,0,0,1);
    glScalef(26, 1.0, 42.0);
    drawCube();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(41.5, 61, 0);
    glRotatef(145,0,0,1);
    glScalef(22.5, 1.0, 42.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
static void train_ground(void)
{
    for (double i=10; i<=90; i+=10)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,9);
        glPushMatrix();
        glTranslated(165,0,i);
        glScalef(135, 0.5, 1.0);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,12);
    for (double i=10,j=165; i<=50; i+=10,j+=30)
    {
        glPushMatrix();
        if((j+movex[(int)((int)i/10)])>=225.5 && (j+movex[(int)((int)i/10)])<=237.5 && M)
        {
            glTranslatef(j+movex[(int)((int)i/10)],-20,i);
        }
        else
        {
            glTranslated(j+movex[(int)((int)i/10)],0,i);
        }
        if(p)
        {
            glTranslatef(0,20,0);
            p=false;
        }
        glScalef(15, 20, 0.5);
        drawCube();
        glPopMatrix();
    }
    for (double i=60,j=270; i<=90; i+=10,j-=30)
    {
        glPushMatrix();
        glTranslated(j+movex[(int)((int)i/10)],0,i);
        if((j+movex[(int)((int)i/10)])>=225.5 && (j+movex[(int)((int)i/10)])<=237.5 && M)
        {
            glTranslatef(0,-20,0);
        }
        if(p)
        {
            glTranslatef(0,20,0);
            p=false;
        }
        glScalef(15, 20, 0.5);
        drawCube();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
    for (int i=10; i<=100; i+=10)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,3);
        glPushMatrix();
        glTranslated(165,0,i);
        glScalef(1, 40, 1);
        drawCube();
        glPopMatrix();
        if((i/10)%2)
        {
            glPushMatrix();
            glTranslated(165,39.5,i);
            glScalef(i-5, 0.5, 0.5);
            drawCube();
            glPopMatrix();
        }
        glDisable(GL_TEXTURE_2D);
        if((i/10)%2)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,13);
            glPushMatrix();
            glTranslated(165+i-5,30,i);
            glScalef(10, 10, 0.5);
            drawCube();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,3);
        glPushMatrix();
        glTranslated(299,0,i);
        glScalef(1, 40, 1);
        drawCube();
        glPopMatrix();
        if((i/10)%2==0)
        {
            glPushMatrix();
            glTranslated(299-105+i,39.5,i);
            glScalef(105-i, 0.5, 0.5);
            drawCube();
            glPopMatrix();
        }
        glDisable(GL_TEXTURE_2D);
        if((i/10)%2==0)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,13);
            glPushMatrix();
            glTranslated(299-105+i-7.5,30,i);
            glScalef(10, 10, 0.5);
            drawCube();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glTranslatef(190,0,140);
    glScalef(85,10,5);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    for(double i=211.25,k=16; i<275; i+=21.25)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,3);
        glPushMatrix();
        glTranslatef(i,0,170);
        glScalef(1,9,1);
        drawCube();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,k++);
        glPushMatrix();
        glTranslatef(i-5,9,165);
        glScalef(10,1,10);
        drawCube();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,14);
    glPushMatrix();
    glTranslatef(195,0,200);
    glScalef(75,30,1);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,15);
    glPushMatrix();
    glTranslatef(190,30,200);
    glRotatef(-45,1,0,0);
    glScalef(85,30,1);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,9);
        glPushMatrix();
        glTranslatef(195,0,180);
        glScalef(1,53,1);
        drawCube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(195+37.5,0,180);
        glScalef(1,53,1);
        drawCube();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(195+37.5+37.5,0,180);
        glScalef(1,53,1);
        drawCube();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}
static void MAP(void)
{
/// Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glTranslatef(0,-1,0);
    glScalef(300.0, 1.0, 300.0);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    {
/// Left Wall
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,9);
//    glPushMatrix();
//    glScalef(1.0, 100.0, 100.0);
//    drawCube();
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
/// Front Wall
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,10);
//    glPushMatrix();
//    //glTranslatef(0,0,0);
//    glScalef(200.0, 100.0, 1.0);
//    drawCube();
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
/// Right Wall
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,10);
//    glPushMatrix();
//    glTranslatef(199, 0, 0);
//    glScalef(1.0, 100.0, 100.0);
//    drawCube();
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
    }
/// Roof
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,11);
    glPushMatrix();
    glTranslatef(-100, 100, -100);
    glScalef(500.0, 1.0, 500.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    glTranslatef(135,0,0);
    glScalef(30.0, 0.5, 300.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glTranslatef(0,0,135);
    glScalef(135, 0.5, 30.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
//   glPushMatrix();
//    glTranslatef(57.5,0,43);
//    glScalef(41.5, 0.5, 15.0);
//    drawCube();
//    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0,-10,0);
    glScalef(1,0.6,1);
    house();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(70,-10,0);
    glScalef(1,0.6,1);
    house();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-10,135);
    glRotatef(90,0,1,0);
    glScalef(1,0.6,1);
    house();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(0,0,235);
    glScalef(60,70,60);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0,165);
    glScalef(60,70,60);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
static void copter(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
    const double a = t*90.0;
     glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,22);
    if(wired)
    {
        glPolygonMode( GL_FRONT, GL_LINE ) ;
        glPolygonMode( GL_BACK, GL_LINE ) ;

    }
    else
    {
        glPolygonMode( GL_FRONT,GL_FILL ) ;
        glPolygonMode( GL_BACK, GL_FILL ) ;
    }

    glPushMatrix();

    if(animat)
        glRotated(a,0,0,1);

    glRotatef( anglex, 1.0, 0.0, 0.0);
    glRotatef( angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
    glRotatef( anglez, 0.0, 0.0, 1.0);

    //glRotatef( 90, 0.0, 1.0, 0.0);
    //glColor3f(1,0,0);
    glTranslatef(200,20,247);
    glScalef(6,6,5);
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    //matColor(1,1,1,20);   // front face color
    //matColor(0,0.2,0.0,20,1);  // back face color


    bottleBezier();


//    if(shcpt)
//    {
//        matColor(0.0,0.0,0.9,20);
//        showControlPoints();
//    }

    glPopMatrix();


//glPushMatrix();
    //glRotatef()
    glPushMatrix();
    glTranslatef(230,40,248);
    glRotatef(heli,0,1,0);
    glScalef(20,0.2,2);
    drawCube(1,1,1);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(230,40,248);
    //glRotatef(180,0,)
    glRotatef(heli,0,1,0);
    glScalef(2,0.2,20);
    drawCube(1,1,1);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(230,40,248);
    glRotatef(180,0,0,1);
    glRotatef(heli,0,-1,0);
    glScalef(20,0.2,2);
    drawCube(1,1,1);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(230,40,248);
    glRotatef(180,1,0,0);
    glRotatef(heli,0,-1,0);
    glScalef(2,0.2,20);
    drawCube(1,1,1);

    glPopMatrix();

    //glPopMatrix();


    double xxxx=-50;
    glPushMatrix();
    //
    glTranslatef(-20,280,209);
    glRotatef(90,1,0,0);

    glPushMatrix();
    glTranslatef(245+xxxx,40,250);
    glRotatef(heli,0,1,0);
    glScalef(15,0.2,2);
    drawCube(1,1,1);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(245+xxxx,40,250);
    //glRotatef(180,0,)
    glRotatef(heli,0,1,0);
    glScalef(2,0.2,15);
    drawCube(1,1,1);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(245+xxxx,40,250);
    glRotatef(180,0,0,1);
    glRotatef(heli,0,-1,0);
    glScalef(15,0.2,2);
    drawCube(1,1,1);

    glPopMatrix();
    glPushMatrix();
    glTranslatef(245+xxxx,40,250);
    glRotatef(180,1,0,0);
    glRotatef(heli,0,-1,0);
    glScalef(2,0.2,15);
    drawCube(1,1,1);

    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(180,22,246);
    glScalef(25,3,3);
    drawCube(0.4,0.4,0.2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(180,25,246.5);
    glRotatef(45,0,0,1);
    glScalef(2,10,2);
    drawCube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(230,34,247);
    glScalef(1,6,1);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(220,1,240);
    glScalef(20,1,1);
    drawCube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(220,1,254);
    glScalef(20,1,1);
    drawCube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(225,1,240);
    glScalef(1,9,1);
    drawCube(1,1,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(235,1,240);
    glScalef(1,9,1);
    drawCube(1,1,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(225,1,254);
    glScalef(1,9,1);
    drawCube(1,1,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(235,1,254);
    glScalef(1,9,1);
    drawCube(1,1,1);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(170,0,210);
    glScalef(120,1,80);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}
static void buildstair(double k)
{
        glPushMatrix();
        glPushMatrix();
        glTranslatef(100.0,k,255.0);
        //glRotatef(-30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();

        for(double i=255;i<=285;i+=5)
        {
            glPushMatrix();
            glTranslatef(100.0,k,i);
            glScalef(4,1,1);
            drawCube();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(104.0,k,255.0);
        //glRotatef(-30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();
        glRotatef(-30,1,0,0);
        glPopMatrix();

        glPushMatrix();
        //glRotatef(-30,1,0,0);
        glPushMatrix();
        glTranslatef(105.0,k+30,255.0);
        //glRotatef(30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();

        for(double i=255;i<=285;i+=5)
        {
            glPushMatrix();
            glTranslatef(105.0,k,i);
            glScalef(4,1,1);
            drawCube();
            glPopMatrix();
        }

        glPushMatrix();
        glTranslatef(109.0,k+30,255.0);
        //glRotatef(30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();
        glPopMatrix();
}
static void newbuild(void)
{
    double gg=0,kg=0;
    for(double i=0.0; i<=90.0; i+=30.0)
    {


        gg=i;
        kg=gg+18;
        if(i<90.0)
            {
                ///flr
                glPushMatrix();
        glTranslatef(80.0,i,240.0);
        glScalef(20.0,1.0,50.0);
        drawCube(1,1,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(110.0,i,240.0);
        glScalef(20.0,1.0,50.0);
        drawCube(1,1,1);
        glPopMatrix();
        glPushMatrix();
        ///r8wall
        glTranslatef(80.0,i,240.0);
        glScalef(1.0,30.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(80.0,i,250.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(80.0,i+20,250.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(80.0,i,260.0);
        glScalef(1.0,30.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(80.0,i,270.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(80.0,i+20,270.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(80.0,i,280.0);
        glScalef(1.0,30.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();
        ///leftwall
        glPushMatrix();
        glTranslatef(129.0,i,240.0);
        glScalef(1.0,30.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(129.0,i,250.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(129.0,i+20,250.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(129.0,i,260.0);
        glScalef(1.0,30.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(129.0,i,270.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(129.0,i+20,270.0);
        glScalef(1.0,10.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(129.0,i,280.0);
        glScalef(1.0,30.0,10.0);
        drawCube(1,0,1);
        glPopMatrix();
        ///backwall
        glPushMatrix();
        glTranslatef(80.0,i,289.0);
        glScalef(50.0,30.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        ///frntwall
        glPushMatrix();
        glTranslatef(80.0,i,240.0);
        glScalef(5.0,30.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(85.0,i,240.0);
        glScalef(10.0,10.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(85.0,i+20,240.0);
        glScalef(10.0,10.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(95.0,i,240.0);
        glScalef(5.0,30.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(110.0,i,240.0);
        glScalef(5.0,30.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(115.0,i,240.0);
        glScalef(10.0,10.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(115.0,i+20,240.0);
        glScalef(10.0,10.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(125.0,i,240.0);
        glScalef(5.0,30.0,1.0);
        drawCube(0,0,1);
        glPopMatrix();
        ///mid2
        glPushMatrix();
        glTranslatef(110.0,i,250.0);
        glRotatef(90,0,1,0);
        glScalef(1.0,20.0,5.0);
        drawCube(0,1,1);
        glPopMatrix();
        ///mid1
        glPushMatrix();
        glTranslatef(99.0,i,255.0);
        glScalef(1.0,30.0,35.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(99.0,i,250.0);
        glRotatef(-90,0,1,0);
        glScalef(1.0,20.0,5.0);
        drawCube(0,1,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(99.0,i+20,250.0);
        glScalef(1.0,10.0,5.0);
        drawCube(0,1,0);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(99.0,i,240.0);
        glScalef(1.0,30.0,10.0);
        drawCube(0,0,1);
        glPopMatrix();
        ///mid2
        glPushMatrix();
        glTranslatef(110.0,i,255.0);
        glScalef(1.0,30.0,35.0);
        drawCube(0,0,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(110.0,i,240.0);
        glScalef(1.0,30.0,10.0);
        drawCube(0,0,1);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(110.0,i+20,250.0);
        glScalef(1.0,10.0,5.0);
        drawCube(0,1,0);
        glPopMatrix();
        ///stair
        glPushMatrix();
        glTranslatef(100.0,i,255.0);
        glRotatef(-30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(104.0,i,255.0);
        glRotatef(-30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(105.0,i+30,255.0);
        glRotatef(30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(109.0,i+30,255.0);
        glRotatef(30,1,0,0);
        glScalef(1.0,1.0,30.0);
        drawCube(0,0,0);
        glPopMatrix();
        for(double j=255; j<=275; j+=5,gg+=3)
        {
            glPushMatrix();
            glTranslatef(100,gg,j);
            glScalef(5.0, 1.0, 1.0);
            drawCube();
            glPopMatrix();
        }
        for(double j=275; j>=255; j-=5,kg+=3)
        {
            glPushMatrix();
            glTranslatef(105,kg,j);
            glScalef(5.0, 1.0, 1.0);
            drawCube();
            glPopMatrix();
        }
        ///mid
        glPushMatrix();
        glTranslatef(100.0,i,240.0);
        glScalef(10.0,1.0,15.0);
        drawCube(1,0,0);
        glPopMatrix();
            ///stairmid
        glPushMatrix();
        glTranslatef(100.0,i+15,280.0);
        glScalef(10.0,1.0,10.0);
        drawCube(1,1,0);
        glPopMatrix();

        }
        else
        {
            glPushMatrix();
        glTranslatef(80.0,i,240.0);
        glScalef(50.0,1.0,50.0);
        drawCube(1,1,1);
        glPopMatrix();

        }

    }
}
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(-5,5,-5,5, 2.0, 1000.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);

    //glRotatef(hrot/10,0,1,0);

    glTranslatef(-plx,0,-plz);
    glTranslatef(-hlx,-hly,-hlz);
    MAP();
    train_ground();

    glPushMatrix();
    glTranslatef(230,0,175);
    player();
    glPopMatrix();



    glPushMatrix();
    //glRotatef(hrot,0,1,0);
    glTranslatef(hlx,hly,hlz);

    copter();
    glPopMatrix();
    newbuild();
    //treeInit(15);
    glutSwapBuffers();
}
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back, float ambFactor, float specFactor)
{

    const GLfloat mat_ambient[]    = { kdr*ambFactor, kdg*ambFactor, kdb*ambFactor, 1.0f };
    const GLfloat mat_diffuse[]    = { kdr, kdg, kdb, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f*specFactor, 1.0f*specFactor, 1.0f*specFactor, 1.0f };
    const GLfloat high_shininess[] = { shiny };
    if(frnt_Back==0)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==1)
    {
        glMaterialfv(GL_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_BACK, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==2)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
    }

}
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
    case 'n':
        animat=!animat;
        break;

    case '4':
        //case 'S':
        shcpt=!shcpt;
        break;

    case '5':
        //case 'W':
        wired=!wired;
        break;

    case '6':
        anglex = ( anglex + 3 ) % 360;
        break;
//    case 'X':
//        anglex = ( anglex - 3 ) % 360;
//        break;

    case '8':
        angley = ( angley + 3 ) % 360;
        break;
//    case 'Y':
//        angley = ( angley - 3 ) % 360;
//        break;

    case '7':
        anglez = ( anglez + 3 ) % 360;
        break;
//    case 'Z':
//        anglez = ( anglez - 3 ) % 360;
//        break;
    case 't':
        lookY += 1.5;
        break;
    case 'g':
        lookY -= 1.5;
        break;
    case 'h':
        lookX += 1.5;
        break;
    case 'f':
        lookX -= 1.5;
        break;
    case 'u':
        lookZ += 1.5;
        break;
    case 'y':
        lookZ -= 1.5;
        break;
    case 'w':
        eyeY += 0.5;
        break;
    case 's':
        eyeY -= 0.5;
        break;
    case 'a':
        eyeX += 0.5;
        break;
    case 'd':
        eyeX -= 0.5;
        break;
    case 'e':
        eyeZ += 0.5;
        break;
    case 'r':
        eyeZ -= 0.5;
        break;
    case '+':
        prot++;
        //lookX-=5;
        //eyeX-=5;
        break;
    case 'm':
    {
        if(fanim)
        {
            if(fanim==.4)
                fanim=-.4;
            else
                fanim=.4;
        }
        else
            fanim=.4;
    }
    M=true;
    break;
    case '-':
        prot--;
        //lookX+=5;
        //eyeX+=5;
        break;
    case '1':
        if(light1)
        {
            glEnable(GL_LIGHT0);
            light1 = false;
        }

        else
        {
            glDisable(GL_LIGHT0);
            light1 = true;
        }
        break;
    case '2':
        if(light2)
        {
            glEnable(GL_LIGHT1);
            light2 = false;
        }

        else
        {
            glDisable(GL_LIGHT1);
            light2 = true;
        }
        break;
    case '3':
        if(light3)
        {
            glEnable(GL_LIGHT2);
            light3 = false;
        }

        else
        {
            glDisable(GL_LIGHT2);
            light3 = true;
        }
        break;
    case 'p':
        p= true;
        M=false;
        break;
    case 'z':
        ambient = !ambient;
        break;
    case 'x':
        diffuse = !diffuse;
        break;
    case 'c':
        specular = !specular;
        break;
    case 'v':
        eyeX=233;
        eyeY=23;
        eyeZ=175;
        lookX=230;
        lookY=50;
        lookZ=0;
        break;
    case 'b':
        eyeX=320;
        eyeY=50;
        eyeZ=170;
        lookX=100;
        lookY=50;
        lookZ=80;
        break;
    case 'l':
        plx++;
        //eyeX++;
        break;
    case 'j':
        plx--;
        //eyeX--;
        break;
    case 'i':
        plz--;
        //eyeZ++;
        break;
    case 'k':
        plz++;
        //eyeZ--;
        break;
    case 'L':
        hlx++;
        //eyeX++;
        break;
    case 'J':
        hlx--;
        //eyeX--;
        break;
    case 'I':
        hly++;
        //eyeZ++;
        break;
    case 'K':
        hly--;
        //eyeZ--;
        break;
    case 'P':
        hlz++;
        //eyeZ++;
        break;
    case 'O':
        hlz--;
        //eyeZ--;
        break;
//    case 'M':
//        hrot=90;
//        //eyeZ++;
//        break;
//    case 'N':
//        hrot=90;
//        //eyeZ--;
//        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 298,99,150, 1.0 };

    if(ambient)
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);

    }
    else
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
    }

    if(diffuse)
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    }
    else
    {
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
    }

    if(specular)
    {
        glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    }
    else
    {
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }

    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

    GLfloat light_ambient2[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse2[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position2[] = { 0,99,150, 1.0 };

    if(ambient)
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient2);

    }
    else
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
    }

    if(diffuse)
    {
        glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse2);

    }
    else
    {
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
    }

    if(specular)
    {
        glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular2);
    }
    else
    {
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
    }

    glLightfv( GL_LIGHT1, GL_POSITION, light_position2);

    GLfloat light_ambient3[]  = {0.5, 0.3, 0.7, 1.0};
    GLfloat light_diffuse3[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular3[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position3[] = { 230, 100, 160, 1.0 };

    //glEnable( GL_LIGHT2);
    if(ambient)
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient3);

    }
    else
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
    }

    if(diffuse)
    {
        glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse3);

    }
    else
    {
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
    }

    if(specular)
    {
        glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular3);
    }
    else
    {
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
    }

    glLightfv( GL_LIGHT2, GL_POSITION, light_position3);

    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 60.0);
}
/* Program entry point */
void animate()
{
    heli+=2;
    if(heli>360)
        heli=0;
    {
        if(movex[1]<120 && on)
        {
            movex[1]+=0.1;
        }
        else if (movex[1]>=120)
        {
            on=false;
        }
        if(movex[1]>0 && !on)
        {
            movex[1]-=0.1;
        }
        else if(movex[1]<=0)
        {
            on=true;
        }

        if(movex[2]<90 && on2)
        {
            movex[2]+=0.1;
        }
        else if (movex[2]>=90)
        {
            on2=false;
        }
        if(movex[2]>-30 && !on2)
        {
            movex[2]-=0.1;
        }
        else if(movex[2]<=-30)
        {
            on2=true;
        }

        if(movex[3]<60 && on3)
        {
            movex[3]+=0.1;
        }
        else if (movex[3]>=60)
        {
            on3=false;
        }
        if(movex[3]>-60 && !on3)
        {
            movex[3]-=0.1;
        }
        else if(movex[3]<=-60)
        {
            on3=true;
        }

        if(movex[4]<30 && on4)
        {
            movex[4]+=0.1;
        }
        else if (movex[4]>=30)
        {
            on4=false;
        }
        if(movex[4]>-90 && !on4)
        {
            movex[4]-=0.1;
        }
        else if(movex[4]<=-90)
        {
            on4=true;
        }

        if(movex[5]<0 && on5)
        {
            movex[5]+=0.1;
        }
        else if (movex[5]>=0)
        {
            on5=false;
        }
        if(movex[5]>-120 && !on5)
        {
            movex[5]-=0.1;
        }
        else if(movex[5]<=-120)
        {
            on5=true;
        }

        if(movex[6]<15 && on6)
        {
            movex[6]+=0.1;
        }
        else if (movex[6]>=15)
        {
            on6=false;
        }
        if(movex[6]>-105 && !on6)
        {
            movex[6]-=0.1;
        }
        else if(movex[6]<=-105)
        {
            on6=true;
        }

        if(movex[7]<45 && on7)
        {
            movex[7]+=0.1;
        }
        else if (movex[7]>=45)
        {
            on7=false;
        }
        if(movex[7]>-75 && !on7)
        {
            movex[7]-=0.1;
        }
        else if(movex[7]<=-75)
        {
            on7=true;
        }

        if(movex[8]<75 && on8)
        {
            movex[8]+=0.1;
        }
        else if (movex[8]>=75)
        {
            on8=false;
        }
        if(movex[8]>-45 && !on8)
        {
            movex[8]-=0.1;
        }
        else if(movex[8]<=-45)
        {
            on8=true;
        }

        if(movex[9]<105 && on9)
        {
            movex[9]+=0.1;
        }
        else if (movex[9]>=105)
        {
            on9=false;
        }
        if(movex[9]>-15 && !on9)
        {
            movex[9]-=0.1;
        }
        else if(movex[9]<=-15)
        {
            on9=true;
        }
    }
//    if (flagrotate == true)
//    {
//        rotMin+= 0.02;
//        rotHour+=0.002;
//
//    }
//    if (bRotate == true)
//    {
//        theta += 0.5;
//        if(theta > 360.0)
//            theta -= 360.0*floor(theta/360.0);
//    }

//    if (uRotate == true)
//    {
//        alpha += 0.2;
////        if(alpha > 360.0)
////            alpha -= 360.0*floor(alpha/360.0);
//    }
    glutPostRedisplay();
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Mortal Shooting");

    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\grass.bmp");

    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\wall7.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\test2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\bb2.bmp");

    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\roof.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\roof2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\road.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\road2.bmp");

    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\leg1.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\building2.bmp");

    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\sky.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\target2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\dart4.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\guns9.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\wood.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\guns6.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\guns3.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\equipm.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\shirt2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\pant.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\skin3.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\army2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\helipad.bmp");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutIdleFunc(animate);
    //glutMouseFunc(my_mouse);
    light();

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    glutMainLoop();

    return EXIT_SUCCESS;
}
