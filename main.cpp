#include<cstdio>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include<string.h>
#include<windows.h>
#include<MMSystem.h>
float PI=3.1416;
using namespace std;

float r=-1;

int score1=0;
int score2=0;

void output(int x,int y,float r,float g,float b,char *strings)
{
    glColor3f(r,g,b);
    glRasterPos2f(x,y);
    int len, i;
    len = (int)strlen(strings);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter( (void*)2 , strings[i]);
    }
}


float xm=0.0f;
float ym=-.0f;
float radius=.05f;

int hold1;
int hold2;
float baseMove=.1;

float ax1=baseMove;
float ax2=.5+baseMove;
float ay1=1;
float ay2=.9;

float bx1=baseMove;
float bx2=.5+baseMove;
float by1=-1;
float by2=-0.9;

GLfloat positionx=.35;//x axis
GLfloat speedx = 0.0f; //how much it will move
GLfloat positiony =-.85;//x axis
GLfloat speedy = 0.0f; //how much it will move


void sound()
{
    sndPlaySound("jump-sound.wav",SND_ASYNC);
}

void boxCollusion(float x1,float x2,float y1,float y2)
{
    if((positionx>=x1-radius&&positionx<=x2+radius)&&(positiony>=y1-radius&&positiony<=y2+radius))
    {
        speedy*=-1;
        sound();
    }
}


void ball()
{
    int triangleAmountm = 60; //# of triangles used to draw circle
    glColor3ub(255, 245, 157  );
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xm, ym); // center of circle
    for(int i = 0; i <= 60; i++)
    {
        glVertex2f(
            xm + (radius * cos(i *2*  PI / triangleAmountm)),
            ym + (radius * sin(i *2*  PI / triangleAmountm))
        );
    }
    glEnd();
}


void box(float x1,float x2,float y1,float y2)
{
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.0f, 1.0f);
    glVertex2f(x1,y1);
    glVertex2f(x1,y2);
    glVertex2f(x2,y2);
    glVertex2f(x2,y1);
    glEnd();
}


void base1()
{

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.0f, 1.0f);
    glVertex2f(bx1,by1);
    glVertex2f(bx1,by2);
    glVertex2f(bx2,by2);
    glVertex2f(bx2,by1);
    glEnd();
}


void base2()
{

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.0f, 1.0f);
    glVertex2f(ax1,ay1);
    glVertex2f(ax1,ay2);
    glVertex2f(ax2,ay2);
    glVertex2f(ax2,ay1);
    glEnd();
}


void endDisplay()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int winer;
    if (score1>score2)
    {
        winer=1;
    }
    else
    {
        winer=2;
    }
    char str[16]= {'G','a','m','e',' ','O','v','e','r','e',' ',char(48+winer),' ','W','i','n'};
    output(0,0,0,0,0,str);

    glFlush();

}
void display()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslated(positionx,positiony,0.0f);
    ball();
    glPopMatrix();
    box(r,r+.5,-.05,.05);
    glPushMatrix();
    base1();
    base2();
    glPopMatrix();

    char str[21]= {'s','c','o','r','e','(','1',')',':',char(score1+48),char(32),'s','c','o','r','e','(','2',')',':',char(score2+48)};
    output(-1,0,0,0,0,str);

    glFlush();

}

void update(int value)
{
    positionx += speedx;
    positiony += speedy;

    r+=.035f;
    if (r>=1)
    {
        r=-1.5;
    }

    boxCollusion(r,r+.5,-.05,.05);
    float border=.90f;
    if (positionx>=border)
    {
        speedx*=-1;
        sound();
    }
    if (positionx<=-border)
    {
        speedx*=-1;
        sound();
    }
    if ((positiony>=ay2-radius)&&(positionx>=ax1)&&(positionx<=ax2))
    {
        speedy*=-1;
        sound();
    }
    if ((positiony<=by2+radius)&&(positionx>=bx1)&&(positionx<=bx2))
    {
        speedy*=-1;
        sound();
    }

    if(positiony>.9)
    {
        hold2=1;
        positionx=((ax1+ax2)/2);
        speedx = 0.0f; //how much it will move
        positiony = .85f;//x axis
        speedy = 0.0f;

        score1++;
    }

    if(positiony<-.9)
    {
        hold1=1;
        positionx=((bx1+bx2)/2);
        speedx = 0.0f; //how much it will move
        positiony = -.85f;//x axis
        speedy = 0.0f;

        score2++;
    }

    int maxScore=9;
    if(score1==maxScore||score2==maxScore)
    {
        glutDisplayFunc(endDisplay);
    }


    glutPostRedisplay();
    glutTimerFunc(1, update, 0); //defines after what time the object will move
}


void handleKeypress(unsigned char key, int x, int y)
{
    //for base2
    if(key=='a')
    {
        if (bx1>=-1)
        {
            bx1-=baseMove;
            bx2-=baseMove;
            if (hold1==1)
            {
                positionx-=baseMove;
            }
        }
    }
    if(key=='d')
    {
        if(bx2<=1)
        {
            bx1+=baseMove;
            bx2+=baseMove;
            if (hold1==1)
            {
                positionx+=baseMove;
            }
        }
    }
    if(key=='w')
    {
        if (hold1==1)
        {
            hold1=0;
            speedx=-.01;
            speedy=.01;
        }
    }


//for base1

    if(key=='j')
    {
        if (ax1>=-1)
        {
            ax1-=baseMove;
            ax2-=baseMove;
            if (hold2==1)
            {
                positionx-=baseMove;
            }
        }
    }
    if(key=='l')
    {
        if(ax2<=1)
        {
            ax1+=baseMove;
            ax2+=baseMove;
            if (hold2==1)
            {
                positionx+=baseMove;
            }
        }
    }
    if(key=='i')
    {
        if (hold2==1)
        {
            hold2=0;
            speedx=.01;
            speedy=-.01;
        }
    }





}

int main(int argc, char** argv)
{
    hold1=1;

    glutInit(&argc, argv);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Game");
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);

    glutTimerFunc(1, update, 0);

    glutMainLoop();
    return 0;
}
