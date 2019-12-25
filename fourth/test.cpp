#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <Gl/glu.h>
#include <GL/glext.h>

#include "FreeImage.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath> 
#include <cstdlib>
#include "TextureManager.h"

GLfloat separation= 0.0;
GLfloat count = -1.0;
GLfloat x_light_pos = 0.0;
GLfloat angle = 0.0;
GLint  isLight = 0;
GLfloat light_position[] = { 0.0, 0.0, 1.0};
GLfloat z_light_pos = -1.0;
GLint texture_mode = 1;
GLfloat rotate = 0;
GLfloat rotate_x, rotate_y;
GLint stop = 0;
GLfloat alpha = 1;
GLboolean alphaMode = true;
GLuint triangle;


const char *tex_name[] = 
{
   "picture/stone.bmp",
    "picture/tex1.bmp",
    "picture/tex4.bmp",
    "picture/tex7.bmp",
    "picture/tex3.bmp",
    "picture/tex6.bmp",
    "picture/tex.bmp",
    "picture/tex0.bmp",
};

GLuint textures[8]; 

GLvoid LoadGLTextures()
{
    int width, height;
    glGenTextures(8, &textures[0]);

    for (int i = 0; i < 8; i++)
    {
        while (!TextureManager::Inst()->LoadTexture(tex_name[i], i)){};
        TextureManager::Inst()->BindTexture(i);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    }
}
void DrawTriangle(GLfloat x, GLfloat y, GLfloat z, GLint p)
{
    glNormal3f(1,1,1);
    for(int i = 0; i<p; i+=2)
    {
        glNormal3f(1,1,1);
        glBegin(GL_POLYGON);

        glVertex3f( x/p*i       ,y-y/p*i    ,0          );
        glVertex3f( 0           ,y-y/p*i    ,z/p*i      );
        glVertex3f( 0           ,y-y/p*(i+1),z/p*(i+1)  );
        glVertex3f( x/p*(i+1)   ,y-y/p*(i+1),0          );
        glEnd();
    }

}
void CalculateList()
{
	triangle = glGenLists(1);
	glNewList(triangle, GL_COMPILE);
	DrawTriangle(0.4,0.4,0.4, 15);
	glEndList();
}
void init (void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1.0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat white_light[]={1.0,1.0,1.0};
    GLfloat lmodel_ambient[]={0.7, 0.7, 0.7};

    glFrontFace(GL_CW); 
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); 
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    CalculateList();
    glEnable(GL_NORMALIZE); 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glRotatef(45, -1.0, 1.0, 0.0);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LoadGLTextures();
    
}


void display()
{ 

glMatrixMode(GL_MODELVIEW);
glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
if (isLight)
{
    glPushMatrix();
        light_position[0] = x_light_pos;
        light_position[1] = 0.0;
        light_position[2] = z_light_pos;
        glLoadIdentity();
            glTranslated(x_light_pos,0.0,z_light_pos); 
            glColor3f(0.5, 0.5, 0.5);
            glutWireSphere(0.05,14,14);
            glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glPopMatrix();
}
    
   if (rotate)
    {   
        float angle = rotate / 100;
        glRotatef(rotate, rotate_x, rotate_y, 0.0);
        rotate = 0;
    }

switch(texture_mode)
    {
        case 1:
        {
            glBegin(GL_TRIANGLES);

                glNormal3f(-0.16, -0.16, -0.16);
                glColor4f(1.0, 0.5, 0.0, alpha); //оранжевый
                glVertex3f(0.0 + separation, 0.4 + separation, 0.0 + separation);
                glVertex3f(0.4 + separation, 0.0 + separation, 0.0 + separation);
                glVertex3f(0.0 + separation, 0.0 + separation, 0.4 + separation);
        
                glNormal3f(-0.16, 0.16, -0.16);
                glColor4f(0.0, 0.0, 1.0, alpha); // СИНИЙ
                glVertex3f(0.0 + separation, -0.4 - separation, 0.0 + separation);
                glVertex3f(0.0 + separation, 0.0 - separation, 0.4 + separation);
                glVertex3f(0.4 + separation, 0.0 - separation, 0.0 + separation);
            
                glNormal3f(-0.16, -0.16, 0.16);
                glColor4f(1.0, 0.0, 0.0, alpha);//красный
                glVertex3f(0.0 + separation, 0.4 + separation, 0.0 - separation);
                glVertex3f(0.0 + separation, 0.0 + separation, -0.4 - separation); 
                glColor4f(0.7, 0.7, 0.0, alpha);    
                glVertex3f(0.4 + separation, 0.0 + separation, 0.0 - separation);
                
                glNormal3f(0.16, -0.16, -0.16);
                glColor4f(0.0, 1.0, 0.0, alpha);//зеленый
                glVertex3f(0.0 - separation, 0.4 + separation, 0.0 + separation);
                glVertex3f(0.0 - separation, 0.0 + separation, 0.4 + separation);
                glVertex3f(-0.4 - separation, 0.0 + separation, 0.0 + separation);
            
                glNormal3f(0.16, 0.16, -0.16);
                glColor4f(0.02, 0.69, 0.97,alpha); // blue
                glVertex3f(0.0 - separation, -0.4 - separation, 0.0 + separation);
                glVertex3f(-0.4 - separation, 0.0 - separation, 0.0 + separation);
                glVertex3f(0.0 - separation, 0.0 - separation, 0.4 + separation);
            
                glNormal3f(0.16, -0.16, 0.16);
                glColor4f(1.0, 1.0, 0.0,alpha); // YELLOW
                glVertex3f(0.0 - separation, 0.4 + separation, 0.0 - separation);
                glColor4f(1.0, 0.5, 0.0, alpha); // YELLOW   
                glVertex3f(-0.4 - separation, 0.0 + separation, 0.0 - separation);
                glColor4f(1.0, 0.0, 0.0, alpha); // YELLOW
                glVertex3f(0.0 - separation, 0.0 + separation, -0.4 - separation);
                
                glNormal3f(-0.16, 0.16, 0.16);
                glColor4f(0.73, 0.16, 0.96, alpha); // PURPLE
                glVertex3f(0.0 + separation, -0.4 - separation, 0.0 - separation);
                glVertex3f(0.4 + separation, 0.0 - separation, 0.0 - separation);
                glVertex3f(0.0 + separation, 0.0 - separation, -0.4 - separation);
                
                glNormal3f(0.16, 0.16, 0.16);
                glColor4f(1.0, 0.0, 1.0, alpha); //  
                glVertex3f(0.0 - separation, -0.4 - separation, 0.0 - separation);
                glVertex3f(0.0 - separation, 0.0 - separation, -0.4 - separation);
                glColor4f(1.0, 1.0, 0.0, alpha);
                glVertex3f(-0.4 -separation, 0.0 - separation, 0.0 -separation);
            glEnd();
            break;
        }
        case 2:
        {
            glEnable(GL_TEXTURE_2D);
            TextureManager::Inst()->BindTexture(0);
            glColor4f(1.0, 1.0, 1.0, alpha);
            glBegin(GL_TRIANGLES);

                glNormal3f(-0.16, -0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, 0.4 + separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.4 + separation, 0.0 + separation, 0.0 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 + separation, 0.0 + separation, 0.4 + separation);
        
                glNormal3f(-0.16, 0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, -0.4 - separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 + separation, 0.0 - separation, 0.4 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.4 + separation, 0.0 - separation, 0.0 + separation);
            
                glNormal3f(-0.16, -0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, 0.4 + separation, 0.0 - separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 + separation, 0.0 + separation, -0.4 - separation);  
                glTexCoord2f(0.5, 1.0);   
                glVertex3f(0.4 + separation, 0.0 + separation, 0.0 - separation);
                
                glNormal3f(0.16, -0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, 0.4 + separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 - separation, 0.0 + separation, 0.4 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(-0.4 - separation, 0.0 + separation, 0.0 + separation);
            
                glNormal3f(0.16, 0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, -0.4 - separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(-0.4 - separation, 0.0 - separation, 0.0 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 - separation, 0.0 - separation, 0.4 + separation);

                glNormal3f(0.16, -0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, 0.4 + separation, 0.0 - separation);  
                glTexCoord2f(1.0, 0.0);
                glVertex3f(-0.4 - separation, 0.0 + separation, 0.0 - separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 - separation, 0.0 + separation, -0.4 - separation);
                
                glNormal3f(-0.16, 0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, -0.4 - separation, 0.0 - separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.4 + separation, 0.0 - separation, 0.0 - separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 + separation, 0.0 - separation, -0.4 - separation);
                
                glNormal3f(0.16, 0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, -0.4 - separation, 0.0 - separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 - separation, 0.0 - separation, -0.4 - separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(-0.4 -separation, 0.0 - separation, 0.0 -separation);
            glEnd();   
            break;
        }
        case 3:
        {
            glEnable(GL_TEXTURE_2D);
            glColor4f(1.0, 1.0, 1.0, alpha);
            TextureManager::Inst()->BindTexture(0);
            glBegin(GL_TRIANGLES);

                glNormal3f(-0.16, -0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, 0.4 + separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.4 + separation, 0.0 + separation, 0.0 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 + separation, 0.0 + separation, 0.4 + separation);
            glEnd();

            TextureManager::Inst()->BindTexture(1);
            glBegin(GL_TRIANGLES);
                glNormal3f(-0.16, 0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, -0.4 - separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 + separation, 0.0 - separation, 0.4 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.4 + separation, 0.0 - separation, 0.0 + separation);
            glEnd();
            
            TextureManager::Inst()->BindTexture(2);
            glBegin(GL_TRIANGLES);
                glNormal3f(-0.16, -0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, 0.4 + separation, 0.0 - separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 + separation, 0.0 + separation, -0.4 - separation);  
                glTexCoord2f(0.5, 1.0);   
                glVertex3f(0.4 + separation, 0.0 + separation, 0.0 - separation);
            glEnd(); 

            TextureManager::Inst()->BindTexture(3);
            glBegin(GL_TRIANGLES);
                glNormal3f(0.16, -0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, 0.4 + separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 - separation, 0.0 + separation, 0.4 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(-0.4 - separation, 0.0 + separation, 0.0 + separation);
            glEnd();    

            TextureManager::Inst()->BindTexture(4);
            glBegin(GL_TRIANGLES);
                glNormal3f(0.16, 0.16, -0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, -0.4 - separation, 0.0 + separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(-0.4 - separation, 0.0 - separation, 0.0 + separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 - separation, 0.0 - separation, 0.4 + separation);
            glEnd();    

            TextureManager::Inst()->BindTexture(5);
            glBegin(GL_TRIANGLES);    
                glNormal3f(0.16, -0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, 0.4 + separation, 0.0 - separation);   
                glTexCoord2f(1.0, 0.0);
                glVertex3f(-0.4 - separation, 0.0 + separation, 0.0 - separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 - separation, 0.0 + separation, -0.4 - separation);
            glEnd();

            TextureManager::Inst()->BindTexture(6);
            glBegin(GL_TRIANGLES);    
                glNormal3f(-0.16, 0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 + separation, -0.4 - separation, 0.0 - separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.4 + separation, 0.0 - separation, 0.0 - separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(0.0 + separation, 0.0 - separation, -0.4 - separation);
            glEnd();

            TextureManager::Inst()->BindTexture(7);
            glBegin(GL_TRIANGLES);    
                glNormal3f(0.16, 0.16, 0.16);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(0.0 - separation, -0.4 - separation, 0.0 - separation);
                glTexCoord2f(1.0, 0.0);
                glVertex3f(0.0 - separation, 0.0 - separation, -0.4 - separation);
                glTexCoord2f(0.5, 1.0);
                glVertex3f(-0.4 -separation, 0.0 - separation, 0.0 -separation);
            glEnd(); 
            glDisable(GL_TEXTURE_2D);   
            break;
        }
        case 4:
            glPushMatrix();
            glTranslatef(separation,separation,separation);
            glColor4f(1.0, 0.5, 0.0, alpha); 
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(0.0, 0.0, 1.0, alpha); // СИНИЙ
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(1.0, 0.0, 0.0, alpha);//красный
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(0.0, 1.0, 0.0, alpha);//зеленый
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

                glRotatef(180,1.0,0.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(0.02, 0.69, 0.97,alpha);    // Color Blue
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(0.0,0.0,1.0,alpha);    // Color darkBlue
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(0.8,0.0,0.8,alpha);    // Color purple
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glRotatef(90,0.0,1.0,0.0);

            glTranslatef(separation,separation,separation);
            glColor4f(0.73, 0.16, 0.96, alpha); // PURPLE
            glCallList(triangle);
            glTranslatef(-separation,-separation,-separation);
            glPopMatrix();
        break;
        default:
            break;
    }

    glutSwapBuffers();

}

void animation(void)
{   
    GLfloat a = 1.0;
    GLfloat b = 0.8;
    x_light_pos = a*cos(angle);
    z_light_pos = b*sin(angle);
    if (!stop)
    {angle = angle + 0.0009;
    if (angle > 360)
    {angle = 0;} }
    glutPostRedisplay();
}

void mouse(int button,int state,int x,int y)
{
switch(button)
{
case GLUT_LEFT_BUTTON:
if (state==GLUT_DOWN) separation = 0.07;  break;
case GLUT_RIGHT_BUTTON: if (state==GLUT_DOWN) separation = 0.0; break;
}
} 

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'l':  
                glEnable(GL_LIGHT0); stop = 0; glutIdleFunc(animation); isLight = 1; 
                break;
        case 'd':  
                glDisable(GL_LIGHT0); glutIdleFunc(NULL); stop = 1; glutPostRedisplay(); isLight = 0; 
                break;
        case 's': stop = 1; break;

        case 49:  // 1 
                texture_mode = 1;  glutPostRedisplay();
                break;
        case 50:  // 2
                texture_mode = 2; glutPostRedisplay();
                break;
        case 51: //3
                texture_mode = 3; glutPostRedisplay();
            break;
        case '4':
            texture_mode = 4; separation = 0.07; glutPostRedisplay(); break;
        case 27:
            exit(0);
            break;
        case '5':
            rotate = 1; glutPostRedisplay();
            break;
        case '6':
            rotate = 0;
             glutPostRedisplay();
            break;
        case 'm':
        rotate = 1;
            rotate_x = 1.0;
            rotate_y = 0.0; glutPostRedisplay();
            break;  
        case 'k':
        rotate = 1;
            rotate_x = -1.0;
            rotate_y = 0.0; glutPostRedisplay();
            break;      
        case 'n':
        rotate = 1;
            rotate_x = 0.0;
            rotate_y = 1.0; glutPostRedisplay();
            break; 
        case 'b':
        rotate = 1;
            rotate_x = 0.0;
            rotate_y = -1.0; glutPostRedisplay();
            break; 
        case 'x':
        if(alphaMode){
            alpha = 0.2;
            alphaMode = !alphaMode;
            glDisable(GL_DEPTH_TEST);
            glutPostRedisplay();
        }else{
            alpha = 1.0;
            alphaMode = !alphaMode;
            glEnable(GL_DEPTH_TEST);
            glutPostRedisplay();
        }
        break; 
        default:
            break;
    }
}

void reshape(int w, int h)
{
    if (h == 0 || w == 0) return;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0*(GLfloat)w/(GLfloat)h, 1.0*(GLfloat)w/(GLfloat)h,-1.0,1.0,1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,w,h);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (600, 600); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("octa");

    init();
     glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutMouseFunc(mouse); 
    glutKeyboardFunc(keyboard);
    glutIdleFunc(animation);
    glutReshapeFunc(reshape); 
    glutMainLoop();
    glDisable(GL_DEPTH_TEST);
    return 0; 
}