#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include "include/BmpLoader.h"
const int windowWidth = 800;
const int windowHeight = 800;
GLfloat eyeX = 100;//21.5;//50;
GLfloat eyeY = 15;//25;//50;
GLfloat eyeZ = 100;//5;//100;

GLfloat lookX = 100;//21.5;//50;
GLfloat lookY = 50;//40;//50;
GLfloat lookZ = 0;//30;//0;
unsigned int ID;
bool light1= true, light2 = true, light3 =true;
bool ambient =true, diffuse = true, specular = true;
float movex=0,movex2=0,movex3=0,movex4=0,movex5=0;
bool on=true,on2=true,on3=true,on4=true,on5=true;
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

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 150.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void house(void)
{
/// legs
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    for(double i=3.5; i<40.0; i+=12)
    {
        for(double j=5; j<40; j+=10)
        {
            glPushMatrix();
            glTranslatef(i,0,j);
            glScalef(2.0, 10.5, 2.0);
            drawCube();
            glPopMatrix();
        }
    }
    glDisable(GL_TEXTURE_2D);
/// Stairs
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(21.5,0.0,40.0);
    glScalef(15.0, 3.0, 4.0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(21.5,3.0,39.0);
    glScalef(15.0, 4.0, 4.0);
    drawCube(0.1,0.9,0.2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(21.5,7.0,38.0);
    glScalef(15.0, 3.0, 4.0);
    drawCube(1,0,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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
    glBindTexture(GL_TEXTURE_2D,3);
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
    glBindTexture(GL_TEXTURE_2D,3);
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
    glRotatef(30,0,0,1);
    glScalef(22, 1.0, 42.0);
    drawCube();
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(41.5, 60, 0);
    glRotatef(150,0,0,1);
    glScalef(24, 1.0, 42.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
static void traing(void)
{
    glPushMatrix();
    glTranslated(57.5,0,10);
    glScalef(141.5, 0.5, 1.0);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslated(57.5,0,20);
    glScalef(141.5, 0.5, 1.0);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslated(57.5,0,30);
    glScalef(141.5, 0.5, 1.0);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslated(57.5,0,40);
    glScalef(141.5, 0.5, 1.0);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslated(57.5,0,50);
    glScalef(141.5, 0.5, 1.0);
    drawCube();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,12);
    glPushMatrix();
    glTranslatef(57.5+movex,0,10);
    glScalef(15, 15, 0.5);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(75+movex2,0,20);
    glScalef(15, 15, 0.5);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(100+movex3,0,30);
    glScalef(15, 15, 0.5);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(125+movex4,0,40);
    glScalef(15, 15, 0.5);
    drawCube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150+movex5,0,50);
    glScalef(15, 15, 0.5);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
static void MAP(void)
{
/// Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glTranslatef(0,-1,0);
    glScalef(200.0, 1.0, 100.0);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
///// Left Wall
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,9);
//    glPushMatrix();
//    glScalef(1.0, 100.0, 100.0);
//    drawCube();
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
/// Front Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    //glTranslatef(0,0,0);
    glScalef(200.0, 100.0, 1.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Right Wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(199, 0, 0);
    glScalef(1.0, 100.0, 100.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Roof
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,11);
    glPushMatrix();
    glTranslatef(0, 100, 0);
    glScalef(200.0, 1.0, 100.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    glTranslatef(42.5,0,0);
    glScalef(15.0, 0.5, 100.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
/// Road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glTranslatef(0,0,43);
    glScalef(42.5, 0.5, 15.0);
    drawCube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
//   glPushMatrix();
//    glTranslatef(57.5,0,43);
//    glScalef(41.5, 0.5, 15.0);
//    drawCube();
//    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);
//    glPushMatrix();
//    glTranslatef(1,-5,2);
//    glScalef(0.5,0.5,0.5);
    house();
    //glPopMatrix();
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

    glViewport(0, 0, windowHeight, windowWidth);

    MAP();
    traing();

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

        break;

    case '-':

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
    case 'z':
        ambient = !ambient;
        break;
    case 'x':
        diffuse = !diffuse;
        break;
    case 'c':
        specular = !specular;
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
    GLfloat light_position[] = { 199,99,50, 1.0 };

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
    GLfloat light_position2[] = { 0,99,50, 1.0 };

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
    GLfloat light_position3[] = { 100, 99, 50, 1.0 };

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
        if(movex<127 && on)
        {
            movex+=0.1;
        }
        else if (movex>=127)
        {
            on=false;
        }
        if(movex>0 && !on)
        {
            movex-=0.1;
        }
        else if(movex<=0)
        {
            on=true;
        }

        if(movex2<109 && on2)
        {
            movex2+=0.1;
        }
        else if (movex>=109)
        {
            on2=false;
        }
        if(movex2>-17.5 && !on2)
        {
            movex2-=0.1;
        }
        else if(movex2<=-17.5)
        {
            on2=true;
        }

        if(movex3<84 && on3)
        {
            movex3+=0.1;
        }
        else if (movex3>=84)
        {
            on3=false;
        }
        if(movex3>-42.5 && !on3)
        {
            movex3-=0.1;
        }
        else if(movex3<=-42.5)
        {
            on3=true;
        }

        if(movex4<60 && on4)
        {
            movex4+=0.1;
        }
        else if (movex4>=60)
        {
            on4=false;
        }
        if(movex4>-67.5 && !on4)
        {
            movex4-=0.1;
        }
        else if(movex4<=-67.5)
        {
            on4=true;
        }

        if(movex5<35 && on5)
        {
            movex5+=0.1;
        }
        else if (movex5>=35)
        {
            on5=false;
        }
        if(movex5>-92.5 && !on5)
        {
            movex5-=0.1;
        }
        else if(movex5<=-92.5)
        {
            on5=true;
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
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Shooting Lab");

    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\ground5.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\wall7.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\test2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\bb2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\roof.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\roof2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\road.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\road2.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\leg1.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\wall11.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\sky.bmp");
    LoadTexture("F:\\Study\\4.2\\Texture Image\\BMP\\target2.bmp");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutIdleFunc(animate);
    light();

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    glutMainLoop();

    return EXIT_SUCCESS;
}
