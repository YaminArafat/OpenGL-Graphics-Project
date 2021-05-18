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
GLfloat alpha = 0.0,win=0.0, theta = 0.0,bita=0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false, Rotate = false,cut = false, bit=false;
const float rat = 1.0 * width / height;
unsigned int ID;

float l_height = 19;
float spt_cutoff = 87;
float rot = 0,up= 0;

bool l_on = true;

GLfloat eyeX = 0;
GLfloat eyeY = 8;
GLfloat eyeZ = -23;

GLfloat lookX = 0;
GLfloat lookY = 0;
GLfloat lookZ = 0;



static GLfloat v_box[8][3] =
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

static GLubyte quadIndices[6][4] =
{
    {0,2,6,4},
    {1,5,7,3},
    {0,4,5,1},
    {2,3,7,6},
    {0,1,3,2},
    {4,6,7,5}

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

void drawBox(float R=1,float G=1,float B=1,bool e= false)
{
    glColor3f(R,G,B);
    //GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        //glColor3f(colors[4][0],colors[4][1],colors[4][2]);
        getNormal3p(v_box[quadIndices[i][0]][0], v_box[quadIndices[i][0]][1], v_box[quadIndices[i][0]][2],
                    v_box[quadIndices[i][1]][0], v_box[quadIndices[i][1]][1], v_box[quadIndices[i][1]][2],
                    v_box[quadIndices[i][2]][0], v_box[quadIndices[i][2]][1], v_box[quadIndices[i][2]][2]);

        glVertex3fv(&v_box[quadIndices[i][0]][0]);glTexCoord2f(1,1);
        glVertex3fv(&v_box[quadIndices[i][1]][0]);glTexCoord2f(1,0);
        glVertex3fv(&v_box[quadIndices[i][2]][0]);glTexCoord2f(0,0);
        glVertex3fv(&v_box[quadIndices[i][3]][0]);glTexCoord2f(0,1);
    }
    glEnd();
    //glutSolidSphere (3.0, 20, 16);

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
    glTranslatef(0,0,0);
    glScalef(length,width,width);
    drawBox();
    glPopMatrix();

    // Y-axis
    glPushMatrix();
  //  glTranslatef(0,length/2,0);
    glTranslatef(0,0,0);
    glScalef(width,length,width);
    drawBox();
    glPopMatrix();

    // Z-axis
    glPushMatrix();
//  glTranslatef(0,0,length/2);
    glTranslatef(0,0,0);
    glScalef(width,width,length);
    drawBox();
    glPopMatrix();
}

void flr()
{
    glPushMatrix();
    glTranslatef(-30,0,-25);
    glScalef(60,.3,50);
    drawBox(0.914, 0.588, 0.478);
    glPopMatrix();

///far wall
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-30,0,25);
    glScalef(60,20,.3);
    drawBox();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///left wall
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(30,0,-25);
    glScalef(.3,20,50);
    drawBox();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///r8 wall
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-30,0,-25);
    glScalef(.3,20,50);
    drawBox();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///near wall
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-30,0,-25);
    glScalef(60,20,.3);
    drawBox();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///roof
   // glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-30,20,-25);
    glScalef(60,.3,50);
    drawBox();
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
///border line
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-20,0,-20);
    glScalef(40,5,2);
    drawBox();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
void player()
{
    ///leg
  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.5,0,-23.3);
    glScalef(.25,5,.25);
    drawBox();
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);

  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(.5,0,-23.3);
    glScalef(.25,5,.25);
    drawBox();
    glPopMatrix();
   // glDisable(GL_TEXTURE_2D);
///body
  //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.75,5,-23.7);
    glScalef(2,5,1);
    drawBox();
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
  ///head
    //  glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-.25,10,-23.5);
    glScalef(1,1,1);
    drawBox();
    glPopMatrix();
  //  glDisable(GL_TEXTURE_2D);
}

static void res(int width, int height)
{
    glViewport(0, 0, width, width/rat);
}

void light(float x=0.0)
{
    //GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { x,l_height,3,1.0 };

    glEnable( GL_LIGHT0);

    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);

    /*GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);*/
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-10, 10, -10, 10, 3.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    //glViewport(0, 0, width, height);

    glTranslatef(0,0,Tzval);
    glRotatef(up, 1,0,0);
    glRotatef(rot, 0,1,0);


    flr();
    player();

    axes();

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
    drawBox(1, 1, 1,true);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,l_height,3);
    glScalef(1,.5,5);
    glTranslatef(-0.5,-0.5,-0.5);
    drawBox(1, 1, 1,true);
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

    case 'C':
    case 'c':
        Rotate=!Rotate;
        axis_x=0.0;
        axis_y=0.0;
        break;
    case 'O':
    case 'o':
        uRotate=!uRotate;
        axis_x=0.0;
        axis_y=0.0;
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

    case 'r':
    case 'R':
        bRotate = !bRotate;
        theta=0;
        //uRotate = false;
        axis_x=0.0;
        axis_y=-0.001;
        break;
    case '-':
        Tzval+=0.2;
        break;

    case '+':
        Tzval-=0.2;
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
        case 'v':
        bit=!bit;
        break;

    }

    glutPostRedisplay();
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.5;
     /*   if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0); */
    }
    if(uRotate==true)
    {
        win+=0.2;
        if(win > 90.0)
            uRotate= !uRotate;
    }
    if(Rotate==true)
    {
        win-=0.2;
        if(win<=0)
            Rotate=!Rotate;
    }
    if(cut==true) spt_cutoff=0,l_on=false;
        else spt_cutoff=87,l_on=true;

    if(bit==true) bita+=.2;

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
