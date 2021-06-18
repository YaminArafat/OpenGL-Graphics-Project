///openGl aim lab By Ashikur Rahaman
#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include "include/BmpLoader.h"

using namespace std;
const int width = 1000;
const int height = 1000;
double Txval=0,Tyval=0,Tzval=0;
GLfloat alpha = 0.0,win=0.0, fie = 0.0, axis_x=50, axis_y=10,axis_z = -100.0,c=0.4,bita=0.0;
GLboolean fire = false,flag1 = false,flag2 = false,flag3 = false,flag4 = false,flag5 = false,flag6 = false,cut = false,jump = false, Njump = false, bit= false,door=false, amb=true;
const float rat = 1.0 * width / height;
unsigned int ID;

float l_height = 100.50;
float spt_cutoff = 90;
float rot = 0,up= 0;

bool l_on = true;

GLfloat eyeX = -5;
GLfloat eyeY = 15;
GLfloat eyeZ = -155;

GLfloat lookX = -5;
GLfloat lookY = 10;
GLfloat lookZ = 100;
///curve
int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt=0;
//int animat = 0;
const int L=30;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 50;				//number of slices along x-direction
const int ntheta = 20;
const double PI = 3.14159265389;


static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};


static GLubyte c_ind[6][4] =
{
    {3,1,5,7},  //front
    {6,4,0,2},  //back
    {2,3,7,6},  //top
    {1,0,4,5},  //bottom
    {7,5,4,6},  //right
    {2,0,1,3}   //left
};

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1,
                        GLfloat x2, GLfloat y2, GLfloat z2,
                        GLfloat x3, GLfloat y3, GLfloat z3)
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
void set_mat_prop(float colR=0.5, float colG=0.5, float colB=0.5, bool em=false, float shine=128)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { colR, colG, colB, 1.0 };
    GLfloat mat_diffuse[] = { colR, colG, colB, 1.0 };
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat mat_emission[] = {colR, colG, colB, 1.0};
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(em)
        glMaterialfv( GL_FRONT, GL_EMISSION, mat_emission);
    else
        glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);
}


void cube(float colR=1, float colG=1, float colB=1,
          bool em=false, float shine=128)
{
    set_mat_prop(colR,colG,colB,em,shine);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glTexCoord2f(0,1);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
    }
    glEnd();
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

void axes()
{
    float length = 10;
    float width = 0.3;

    // X-axis
    glPushMatrix();
  //  glTranslatef(length/2,0,0);
    glTranslatef(0,.5,0);
    glScalef(length,width,width);
    cube(1,0,0);
    glPopMatrix();

    // Y-axis
    glPushMatrix();
  //  glTranslatef(0,length/2,0);
    glTranslatef(0,0,0);
    glScalef(width,length,width);
    cube(0,1,0);
    glPopMatrix();

    // Z-axis
    glPushMatrix();
//  glTranslatef(0,0,length/2);
    glTranslatef(0,0.5,0);
    glScalef(width,width,length);
    cube(0,0,1);
    glPopMatrix();
}

void crosair()
{
    glPushMatrix();
    glTranslatef(c,7,-2);
    glScalef(.2,.5,.2);
    cube(1.000, 0.000, 0.000);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.25,7.15,-2);
    glScalef(.5,.2,.2);
    cube(1.000, 0.000, 0.000);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(c,7.15,-2);
    glScalef(.1,.1,.1);
    cube(1.000, 0.000, 0.000);
    glPopMatrix();
}

void wall()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,11);
    glPushMatrix();
    glTranslatef(-80,0,-80);
    glScalef(2,30,140);
    cube(0,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-80,0,60);
    glScalef(20,30,2);
    cube(0,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-80,0,-80);
    glScalef(60,30,2);
    cube(0,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10,0,-110);
    glScalef(2,30,50);
    cube(0,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,0,-100);
    glScalef(30,4,2);
    cube(0,1,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,0,-98);
    glScalef(30,6,2);
    cube(1,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,0,-130);
    glScalef(2,30,50);
    cube(0,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,0,-130);
    glScalef(60,30,2);
    cube(1,1,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(22,4,-100);
    glRotatef(bita,0,0,1);
    glScalef(2,4,2);
    cube(0,1,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(25,5,-98);
    glRotatef(bita,1,0,0);
    glScalef(2,6,2);
    cube(1,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(35,0,-110);
    glScalef(2,30,50);
    cube(.5,.5,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60,0,-130);
    glScalef(2,30,70);
    cube(1,1,0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}
void flr()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glTranslatef(-60,0,-60);
    glScalef(120,.3,120);
    cube(.5, 0.627, 0.478);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

///field
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glTranslatef(-1000,-.5,-1000);
    glScalef(2000,.3,2000);
    cube(.5, 0.627, 0.478);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///far wall field
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-1000,0,1000);
    glScalef(2000,100,.3);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///far left field
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(1000,0,-1000);
    glScalef(.3,100,2000);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///far r8 field
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(-1000,0,-1000);
    glScalef(.3,100,2000);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///far near field
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(-1000,0,-1000);
    glScalef(2000,100,.3);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///far wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-60,0,60);
    glScalef(120,30,.3);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///left wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(60,0,-60);
    glScalef(.3,30,120);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///r8 wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glTranslatef(-60,0,-50);
    glScalef(.3,30,110);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///near wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glPushMatrix();
    glTranslatef(-60,0,-60);
    glScalef(120,30,.3);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///roof
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(-1000,100,-1000);
    glScalef(2000,.3,2000);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///border line
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(-20,0,-30);
    glScalef(40,2.5,1);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void bot(float pos_x,float pos_y,float pos_z)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    glTranslatef(pos_x,pos_y,pos_z);
    glScalef(5,12,.3);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
///curve beizure

GLfloat ctrlpoints[L+1][3] =
{
    { 0.0, 0.0, 0.0}, { 0, 1, 0.0},
    { 0, 1, 0.0},{ 0, 3, 0.0},
    {0, 4, 0.0}, {0, 5, 0.0},
    {0, 6, 0.0},{0, 7, 0.0},
    {.4, 8, 0.0}, {.8, 9, 0.0},
    {1.2, 10, 0.0},{1.6, 9, 0.0},
    {2, 8, 0.0},{2, 7, 0.0},
    {2, 6, 0.0},{2, 5, 0.0},
    {2,4, 0.0},{2,3, 0.0},
    {2,2, 0.0},{2, 1, 0.0},
    {2, 0, 0.0}
};

GLfloat ctrlpoint[L+1][3] =
{
    {0,  5, 0.0},{ 1, 5, 0.0},
    {2,  5, 0.0},{ 3, 5, 0.0},
    {4,  5, 0.0},{5,  5, 0.0},
    {6,  5, 0.0},{7,  5, 0.0},
    {8,  2, 0.0},{9,  2, 0.0},
    {10, 2, 0.0},{11, 2, 0.0},
    {12, 2, 0.0},{13, 2, 0.0},
    {14, 2, 0.0},{15, 2, 0.0},
    {16, 2, 0.0},{17, 2, 0.0},
    {18, 2, 0.0},{19, 2, 0.0},
    {18, 2.5, 0.0},{17, 2.5, 0.0},
    {16, 3, 0.0},{15, 3.5, 0.0},
    {15, 3.5, 0.0},{18, 4, 0.0},
    {20, 4, 0.0},{21, 2, 0.0},
    {22, 1, 0.0},{23, .5, 0.0},
    {24, 0, 0.0}
};

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

void BezierCurve ( double t,  float xy[2],float q=1, float colR=1, float colG=1, float colB=1,bool em=false, float shine=128 )
{
    set_mat_prop(colR,colG,colB,em,shine);

    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        if(q){
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];
        }
        else {
            x+=coef*ctrlpoint[i][0];
            y+=coef*ctrlpoint[i][1];
        }

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

void bottleBezier(float q=1)
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    const float startx=0.0,endx=0.0;

    if(q)
        const float startx = 0, endx = ctrlpoints[L][0];
    if(!q)
        const float startx = 0, endx = ctrlpoint[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
   // BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy, q,0.000, 0.502, 0.502);
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
                getNormal3p(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
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


void player()
{
    ///leg
  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-1,0,0);
    glScalef(.75,5.5,.55);
    cube(0.000, 0.000, 0.545);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);

  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(1.3,0,0);
    glScalef(.75,5.5,.55);
    cube(0.000, 0.000, 0.545);
    glPopMatrix();
   // glDisable(GL_TEXTURE_2D);
   ///shoe
     //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-1.2,.5,.6);
    glScalef(1,1,.1);
    cube(0,0,0);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(1.2,.5,.6);
    glScalef(1,1,.1);
    cube(0,0,0);
    glPopMatrix();
   // glDisable(GL_TEXTURE_2D);

///body
  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-1,5,-.2);
    glScalef(3,5,1);
    cube(0.467, 0.533, 0.600);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
  ///head
    //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.05,10,-.25);
    glScalef(1,1,1);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
  ///hair
       //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.05,10.85,-.25);
    glScalef(1,.3,1);
    cube(0,0,0);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);

  glPushMatrix();
  if(fire==false)
  glRotatef(-5,axis_z,0,0);
  ///hand
    glPushMatrix();
    glTranslatef(-1.3,7,-.5);
    glScalef(.3,2.5,.3);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3,7,-.5);
    glScalef(.3,.3,2.5);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,7,-.5);
    glScalef(.3,2.5,.3);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,7,-.5);
    glScalef(.3,.3,2.5);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3,7,1.7);
    glScalef(3.5,.3,.3);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();
    ///gun
    glPushMatrix();
    glTranslatef(.40,7,1.7);
    glScalef(.3,.5,3);
    cube(0.184, 0.310, 0.310);
    glPopMatrix();
 glPopMatrix();
}

static void res(int width, int height)
{
    glViewport(0, 0, width, width/rat);
}

void light(float x=0.0)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1, 1, 1, 1};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { x,l_height,0,1.0 };

    glEnable( GL_LIGHT0);

    if(l_on && amb) glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    else glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
    if(l_on) glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    else glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);
    if(l_on) glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    else glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat spot_direction[] = { 0, -1, 0,1 };
    GLfloat spt_ct[] = {spt_cutoff};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightfv( GL_LIGHT0, GL_SPOT_CUTOFF, spt_ct);

}
///curve part



/* GLUT callback Handlers */



double ex=0, ey=0, ez=15, lx=0,ly=0,lz=0, hx=0,hy=1,hz=0;

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


void showControlPoints()
{
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlpoint[i][0]);
    glEnd();
}
void rocket()
{
    ///rocket leg
    glPushMatrix();
    glTranslatef(47,-1,-100);
    glScalef(1,12,1);
    glRotatef(65,0,0,1);
    cube(0.561, 0.737, 0.561);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(54,3,-100);
    glScalef(1,12,1);
    glRotatef(-35,0,0,1);
    cube(0.561, 0.737, 0.561);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(48,1,-104);
    glScalef(1,12,1);
    glRotatef(-35,1,0,1);
    cube(0.561, 0.737, 0.561);
    glPopMatrix();
    glPushMatrix();
    glRotatef( anglex, 1.0, 0.0, 0.0);
    glRotatef( angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
    glRotatef( anglez, 0.0, 0.0, 1.0);

    glRotatef( 90, 0.0, 0.0, 1.0);
  //  glTranslated(-3.5,0,0);
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    matColor(0.9,0.5,0.1,20);   // front face color
    matColor(0.0,0.5,0.8,20,1);  // back face color

///rocket
    glTranslatef(10,-50,-100);
    bottleBezier(0);

    if(shcpt)
    {
        matColor(0.0,0.0,0.9,20);
        showControlPoints();
    }

    glPopMatrix();



}
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10, 10, -5, 15, 3.0, 2000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    //glViewport(0, 0, width, height);

    glTranslatef(-axis_x,0,Tzval);
    glRotatef(up, 1,0,0);
    glRotatef(rot, 0,1,0);


    wall();
    flr();
    glPushMatrix();
    //glRotatef(-rot, 0,1,0);
    glTranslatef(axis_x,axis_y,axis_z);

        if(fie==0){
            glRotatef(0, 0,axis_z,0);

    }
    if(fie==1){
            glRotatef(90, 0,axis_z,0);

    }
    if(fie==2){
            glRotatef(-90, 0,axis_z,0);

    }
    if(fie==3){
            glRotatef(180, 0,axis_z,0);
    }
    player();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(axis_x,0,axis_z+10);
    crosair();
    glPopMatrix();

    glPushMatrix();
    if(fire&& -2.5<= axis_x && -2.5+5>=axis_x )
    {
        glRotatef(90, 1,0,0);
        flag1=true;
    }
    if(flag1)
        glRotatef(90, 1,0,0);
    bot(-2.5,0,23);
    glPopMatrix();

    glPushMatrix();
        if(fire&& 10<= axis_x && 10+5>=axis_x )
    {
        glRotatef(90, 1,0,0);
        flag2=true;
       // fire = false;
    }
    if(flag2)
        glRotatef(90, 1,0,0);
    bot(10,0,20);
    glPopMatrix();

    glPushMatrix();
        if(fire&& -15<= axis_x && -15+5>=axis_x )
    {
        glRotatef(90, 1,0,0);
        flag3=true;
       // fire = false;
    }
    if(flag3)
        glRotatef(90, 1,0,0);
    bot(-15,0,20);
    glPopMatrix();

    glPushMatrix();
    if(fire&& -70<= axis_x && -70+5>=axis_x )
    {
        glRotatef(90, 1,0,0);
        flag4=true;
       // fire = false;
    }
    if(flag4)
        glRotatef(90, 1,0,0);
    bot(-70,0,20);
    glPopMatrix();

    glPushMatrix();
    if(fire&& -10<= axis_x && -10+5>=axis_x )
    {
        glRotatef(-90, 1,0,0);
        flag5=true;
       // fire = false;
    }
    if(flag5)
        glRotatef(-90, 1,0,0);
    bot(-10,0,-70);
    glPopMatrix();

    glPushMatrix();
    if(fire&& 25<= axis_x && 25+5>=axis_x )
    {
        glRotatef(-90, 1,0,0);
        flag6=true;
       // fire = false;
    }
    if(flag6)
        glRotatef(-90, 1,0,0);
    bot(25,0,-70);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,12);
    glPushMatrix();
    glTranslatef(40,0,-130);
    if(door)glRotatef(90,0,1,0);
    glScalef(20,30,1);
    cube(1,1,0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    //axes();

    glPushMatrix();
    light(-100);
    glPopMatrix();

    glPushMatrix();
    light(100);
    glPopMatrix();



///tubelight
    glPushMatrix();
    glTranslatef(100,l_height,3);
    glScalef(1,.5,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1, 1, 1,true);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-100,l_height,3);
    glScalef(1,.5,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1, 1, 1,true);
    glPopMatrix();
///curve properties


 const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
    const double a = t*90.0;


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

    glRotatef(bita/5,1,0,0);
    rocket();

    glutSwapBuffers();

}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        exit(0);
        break;
    case 'A':
    case 'a':
        rot++;
        break;
    case 'D':
    case 'd':
        rot--;
        break;
    case 'W':
    case 'w':
        up--;
        break;
    case 'S':
    case 's':
        up++;
        break;

    case 'l':
    case 'L':
        axis_x-=1;
        fie=1;
        break;
    case 'j':
    case 'J':
        axis_x+=1;
        fie=2;
        break;
    case 'k':
    case 'K':
        axis_z-=1;
        Tzval+=1;
        fie=3;
        break;
    case 'i':
    case 'I':
        axis_z+=1;
        Tzval-=1;
        fie=0.0;
        break;
    case 'h':
    case 'H':
        jump=!jump;
        break;
    case 'b':
    case 'B':
        Njump=!Njump;
        break;

    case '-':
        Tzval+=1;
        break;

    case '+':
        Tzval-=1;
        break;
    case 'f':
    case 'F':
        fire=!fire;

        break;
    case 'r':
    case 'R':
        flag1 = false;
        flag2 = false;
        flag3 = false;
        flag4 = false;
        flag5 = false;
        flag6 = false;
        break;
    case '1':
        l_height++;
        break;
    case '2':
        l_height--;
        break;
        case 't':
        cut=!cut;
        break;
    case 'v':
        bit=!bit;
        l_on=!l_on;
        break;
    case 'o':
        door=!door;
        break;
    ///curve command
    case '3':
        shcpt=!shcpt;
        break;

    case '4':
        wired=!wired;
        break;

    case 'x':
        anglex = ( anglex + 3 ) % 360;
        break;
    case 'X':
        anglex = ( anglex - 3 ) % 360;
        break;

    case 'y':
        amb=!amb;
        break;
    case 'z':
        anglez = ( anglez + 3 ) % 360;
        break;
    case 'Z':
        anglez = ( anglez - 3 ) % 360;
        break;


    }

    glutPostRedisplay();
}

void animate()
{
    if(jump)
    {
        for(int i=0;i<20;i++)
            axis_y+=.1;
        jump=!jump;

    }
        if(Njump)
    {
        for(int i=0;i<20;i++)
            axis_y-=.1;
        Njump=!Njump;

    }

    if(cut==true) spt_cutoff=0,l_on=false;
        else spt_cutoff=90,l_on=true;

    if(bit==true) bita+=.2;

    glutPostRedisplay();

}
///curve func

const GLfloat light_ambient[]  = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 1.0f };

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


/* main function */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Aim-lab");

    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\wall1.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\brick2.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\wall3.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\wall4.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\wall5.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\wall6.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\bot1.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\sand.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\floor.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\wall10.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\brick.bmp");
    LoadTexture("E:\\lecture slide\\4-2\\4208-graphics lab\\workplace shhooting game\\Door1.bmp");

    //myInit();
    glutKeyboardFunc(key);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
//    glutIdleFunc(idle);

//    glClearColor(1,1,1,1);
    glutReshapeFunc(res);
    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);


    printf("Use 'r' to rotate the fan, 'd' to look right, and 'a' to look left.\n");
    printf("Use 'w' to go up, 's' to go down,'+' to zoom in, '-' to zoom out,'v' to start and stop the watch.\n");
    printf("Use 'o' to open the window, 'c' to stop the light,'t' to turn on or of the light.\n");
    glutMainLoop();

    return EXIT_SUCCESS;
}
