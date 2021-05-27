///openGl aim lab By Ashikur Rahaman
#include<GL/gl.h>
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
GLfloat alpha = 0.0,win=0.0, theta = 0.0, axis_x=0.0, axis_y=0.5,axis_z = -90.0,c=0.4;
GLboolean fire = false,flag1 = false,flag2 = false,flag3 = false,flag4 = false,flag5 = false,cut = false,jump = false, Njump = false;
const float rat = 1.0 * width / height;
unsigned int ID;

float l_height = 100.50;
float spt_cutoff = 90;
float rot = 0,up= 0;

bool l_on = true;

GLfloat eyeX = 0;
GLfloat eyeY = 15;
GLfloat eyeZ = -120.5;

GLfloat lookX = 1.5;
GLfloat lookY = 10;
GLfloat lookZ = 25;



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
    glTranslatef(c,7,-10);
    glScalef(.2,.5,.2);
    cube(1.000, 0.000, 0.000);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.25,7.15,-10);
    glScalef(.5,.2,.2);
    cube(1.000, 0.000, 0.000);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(c,7.15,-10);
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
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-80,0,60);
    glScalef(20,30,2);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-80,0,-80);
    glScalef(60,30,2);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10,0,-140);
    glScalef(2,30,80);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,0,-140);
    glScalef(2,30,60);
    cube();
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
void player()
{
    ///leg
  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-1,0,-23.3);
    glScalef(.75,5.5,.25);
    cube(0.000, 0.000, 0.545);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);

  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(1.3,0,-23.3);
    glScalef(.75,5.5,.25);
    cube(0.000, 0.000, 0.545);
    glPopMatrix();
   // glDisable(GL_TEXTURE_2D);
   ///shoe
     //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-1.2,.5,-23);
    glScalef(1,1,.1);
    cube(0,0,0);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(1.2,.5,-23);
    glScalef(1,1,.1);
    cube(0,0,0);
    glPopMatrix();
   // glDisable(GL_TEXTURE_2D);

///body
  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-1,5,-23.7);
    glScalef(3,5,1);
    cube(0.467, 0.533, 0.600);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
  ///head
    //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.05,10,-23.5);
    glScalef(1,1,1);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
  ///hair
       //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.05,10.85,-23.45);
    glScalef(1,.3,1);
    cube(0,0,0);
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);

  ///hand
    glPushMatrix();
    glTranslatef(-1.3,7,-23.7);
    glScalef(.3,2.5,.3);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3,7,-23.7);
    glScalef(.3,.3,2.5);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,7,-23.7);
    glScalef(.3,2.5,.3);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2,7,-23.7);
    glScalef(.3,.3,2.5);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3,7,-21.2);
    glScalef(3.5,.3,.3);
    cube(0.824, 0.412, 0.118);
    glPopMatrix();
    ///gun
    glPushMatrix();
    glTranslatef(.40,7,-21.2);
    glScalef(.3,.5,3);
    cube(0.184, 0.310, 0.310);
    glPopMatrix();
}

static void res(int width, int height)
{
    glViewport(0, 0, width, width/rat);
}

void light(float x=0.0)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { x,l_height,3,1.0 };

    glEnable( GL_LIGHT0);

    if(l_on) glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    else glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
    if(l_on) glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    else glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);
    if(l_on) glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    else glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    GLfloat spt_ct[] = {spt_cutoff};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightfv( GL_LIGHT0, GL_SPOT_CUTOFF, spt_ct);

}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10, 10, -10, 10, 3.0, 2000.0);

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
    player();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(axis_x,0,axis_z+10);
    //glRotatef(-rot, 0,1,0);
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
    //axes();

    glPushMatrix();
    light(-30);
    glPopMatrix();

    glPushMatrix();
    light(30);
    glPopMatrix();



///tubelight
    glPushMatrix();
    glTranslatef(20,l_height,3);
    glScalef(1,.5,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1, 1, 1,true);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,l_height,3);
    glScalef(1,.5,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1, 1, 1,true);
    glPopMatrix();

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
        break;
    case 'j':
    case 'J':
        axis_x+=1;
        break;
    case 'k':
    case 'K':
        axis_z-=1;
        Tzval+=1;
        break;
    case 'i':
    case 'I':
        axis_z+=1;
        Tzval-=1;
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
        Tzval+=0.2;
        break;

    case '+':
        Tzval-=0.2;
        break;
        case 'x':
        eyeX-=0.2;
        break;

    case 'z':
        eyeX+=0.2;
        break;
    case 'f':
    case 'F':
        fire=!fire;

        break;
    case 'r':
    case 'R':
        flag1=false;
        flag2=false;
        flag3=false;
        flag4 = false;
        flag5 = false;
        break;
    case '1':
        l_height++;
        break;
    case '2':
        l_height--;
        break;

    case '3':
        spt_cutoff++;
        break;
    case '4':
        spt_cutoff--;
        break;
        case 't':
        cut=!cut;
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

    glutPostRedisplay();

}


/* main function */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Texture-Demo");

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
