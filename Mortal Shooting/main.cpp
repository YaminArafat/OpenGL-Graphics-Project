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
const int width = 800;
const int height = 800;
const float rat = 1.0 * width / height;
GLfloat eyeX = 320;//21.5;//50;
GLfloat eyeY = 50;//25;//50;
GLfloat eyeZ = 170;//300;//5;//100;

GLfloat lookX = 100;//21.5;//50;
GLfloat lookY = 50;//40;//50;
GLfloat lookZ = 80;//30;//0;
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
    glVertex3f(1.0f, 12.0f, -110.0f);
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
            glTranslatef(j+movex[(int)((int)i/10)],-16,i);
        }
        else
        {
            glTranslated(j+movex[(int)((int)i/10)],0,i);
        }
        if(p)
        {
            glTranslatef(0,16,0);
            p=false;
        }
        glScalef(15, 15, 0.5);
        drawCube();
        glPopMatrix();
    }
    for (double i=60,j=270; i<=90; i+=10,j-=30)
    {
        glPushMatrix();
        glTranslated(j+movex[(int)((int)i/10)],0,i);
        if((j+movex[(int)((int)i/10)])>=225.5 && (j+movex[(int)((int)i/10)])<=237.5 && M)
        {
            glTranslatef(0,-16,0);
        }
        if(p)
        {
            glTranslatef(0,16,0);
            p=false;
        }
        glScalef(15, 15, 0.5);
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
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(-5,5,-5,5, 2.0, 1000.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);

    //glRotatef(rot,0,1,0);
    glTranslatef(-plx,0,-plz);
    MAP();
    train_ground();

    glPushMatrix();
    glTranslatef(230,0,155);
    player();
    glPopMatrix();
    //treeInit(15);
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;
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
        eyeY=21;
        eyeZ=155;
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
