#include <iostream>
#include <unistd.h>
#include <math.h>
#include "ball.h"
#include <cstdlib>
#include <ctime>
#include <pthread.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

using namespace std;

const double DEG2RAD = 3.14159/180;//used variable to draw the circle
double setrad = 0.1;//radii of the balls
pthread_mutex_t dareNot=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dataChangeLock = PTHREAD_MUTEX_INITIALIZER;
ball *B;//the array of size n initialized in main, has n balls
int pauseRender=0;
int mainWindow;

pthread_t *controlAgent;    //Thread Identifier

int n;      //number of balls, maximum that can be added depends on their radii
class walls{    //wall distance from origin
    public:
        double left;
        double right;
        double top;
        double bottom;
        double back;
        double front;
};

struct couple{  //contains properties of a ball pair
   ball b1;
   ball b2;
   int index;
   int a1;
   int a2;
};

void drawCircle(ball b);    
void move(int k);
double dist(ball a,ball b);

walls w;
GLfloat X = 0.0f; // Translate screen to x direction (left or right)
GLfloat Y = 0.0f; // Translate screen to y direction (up or down)
GLfloat Z = 0.0f; // Translate screen to z direction (zoom in or out)
GLfloat rotX = 0.0f; // Rotate screen on x axis 
GLfloat rotY = 0.0f; // Rotate screen on y axis
GLfloat rotZ = 0.0f; // Rotate screen on z axis
GLfloat rotLx = 0.0f; // Translate screen by using the glulookAt function (left or right)
GLfloat rotLy = 0.0f; // Translate screen by using the glulookAt function (up or down)
GLfloat rotLz = 0.0f; // Translate screen by using the glulookAt function (zoom in or out)


/* Moves out balls from each other if their posion coincide initially or in collision*/
void moveOut(int ind1, int ind2){
    while((dist(B[ind1],B[ind2]))<=(B[ind1].ball_radius()+B[ind2].ball_radius())){
        move(ind1);
        move(ind2);
    }
}

/* Takes a Ball Pair and collide them : Barrier Synchronization of threads used*/
void* collideBalls(void* pair){
    
    couple* duo;
    duo=static_cast<couple*>(pair);
    ball a=duo->b1;
    ball b=duo->b2;
    double u1x,u2x,v1x,v2x,u1y,u2y,v1y,v2y,u1z,u2z,v1z,v2z,m1,m2;
    
    pthread_mutex_lock(&dataChangeLock);
    m1=a.ball_mass();   //assigning values
    m2=b.ball_mass();
    u1x=a.ball_dx();
    u1y=a.ball_dy();
    u1z=a.ball_dz();
    u2x=b.ball_dx();
    u2y=b.ball_dy();
    u2z=b.ball_dz();
    v1x=(2*m2*u2x+(m1-m2)*u1x)/(m1+m2);        //colliding equations
    v2x=((m2-m1)*u2x+2*m1*u1x)/(m1+m2);
    v1y=(2*m2*u2y+(m1-m2)*u1y)/(m1+m2);
    v2y=((m2-m1)*u2y+2*m1*u1y)/(m1+m2);
    v1z=(2*m2*u2z+(m1-m2)*u1z)/(m1+m2);
    v2z=((m2-m1)*u2z+2*m1*u1z)/(m1+m2);
    a.ball_setdX(v1x);      //setting Final velocities
    a.ball_setdY(v1y);
    a.ball_setdZ(v1z);
    b.ball_setdX(v2x);
    b.ball_setdY(v2y);
    b.ball_setdZ(v2z);
    int ind1=duo->a1,ind2=duo->a2;      //replacing them back in Ball Array
        B[ind1]=a;
        B[ind2]=b;
    pthread_mutex_unlock(&dataChangeLock);
    moveOut(ind1,ind2); //moves them out if they are fused
    return NULL;
}


/*Handles all the  On-Screen collision Drama */
void* makeTheScene(void* arg)
{
    pthread_mutex_lock(&dareNot);
    ball* treat;
    treat = static_cast<ball*>(arg);
    
    for(int j=0;j<n;j++)
    {
        if((dist(B[treat->index],B[j])<=(B[treat->index].ball_radius()+B[j].ball_radius())) and j!= treat->index){
            couple use;
            use.b1=B[treat->index];
            use.b2=B[j];
            use.a1=treat->index;
            use.a2=j;
            collideBalls((void*)&use);
        }
    }
    
    move(treat->index);
    pthread_mutex_unlock(&dareNot);
    pthread_exit(NULL);
}

/*mailbox is a type that stores ball info: native to ball class*/
double dist(mailbox a,mailbox b){
    return (sqrt(pow(b.x_c-a.x_c,2)+pow(b.y_c-a.y_c,2)+pow(b.z_c-a.z_c,2)));

}

/*Collide and makeThe Scene Functions : One to One Synchronized*/


void collide121(ball a,mailbox b,int k){
    double u1x,u2x,v1x,v2x,u1y,u2y,v1y,v2y,u1z,u2z,v1z,v2z,m1,m2;
    pthread_mutex_lock(&dataChangeLock);
    m1=a.ball_mass();   //assigning values
    m2=b.m;
    u1x=a.ball_dx();
    u1y=a.ball_dy();
    u1z=a.ball_dz();
    u2x=b.x_v;
    u2y=b.y_v;
    u2z=b.z_v;
    v1x=(2*m2*u2x+(m1-m2)*u1x)/(m1+m2);        //colliding equations
    v2x=((m2-m1)*u2x+2*m1*u1x)/(m1+m2);
    v1y=(2*m2*u2y+(m1-m2)*u1y)/(m1+m2);
    v2y=((m2-m1)*u2y+2*m1*u1y)/(m1+m2);
    v1z=(2*m2*u2z+(m1-m2)*u1z)/(m1+m2);
    v2z=((m2-m1)*u2z+2*m1*u1z)/(m1+m2);
    a.ball_setdX(v1x);      //setting Final velocities
    a.ball_setdY(v1y);
    a.ball_setdZ(v1z);
    B[k]=a;
    pthread_mutex_unlock(&dataChangeLock);

    while((dist(B[k].ball_info(),b))<=(B[k].ball_radius()+b.r)) {
        move(k);
    }
    //pthread_exit(NULL);
}
void* makeTheScene121(void* arg)
{
    int k=(long)arg;
    mailbox temp_1, temp_2;
    temp_1.x_c=B[k].ball_cx();
    temp_1.y_c=B[k].ball_cy();
    temp_1.x_v=B[k].ball_dx();
    temp_1.y_v=B[k].ball_dy();
    temp_1.r=B[k].ball_radius();
    temp_1.m=B[k].ball_mass();

    for (int i=0;i<n;i++){
        if(i!=k){
	        pthread_mutex_lock(&dareNot);
            B[i].message.push(temp_1);
            pthread_mutex_unlock(&dareNot);
	}    
    }  
    while(!B[k].message.empty())
    {
        temp_2=B[k].message.front();
        B[k].message.pop();
        if(dist(B[k].ball_info(),temp_2)<=(B[k].ball_radius()+temp_2.r)){
            collide121(B[k],temp_2,k);
            //break;
        }    
    } 
    move(k);
    
    pthread_exit(NULL);    
}





/* Pausing functionality by mouse*/
void processMouse(int button,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON and state == GLUT_DOWN and pauseRender==0){
        pauseRender=1;
    }
    else if(button == GLUT_LEFT_BUTTON and state == GLUT_DOWN and pauseRender==1){
        pauseRender=0;
    }
}

/*CAlcullates distance between balls given ball class*/
double dist(ball a,ball b){
    return (sqrt(pow(b.ball_cx()-a.ball_cx(),2)+pow(b.ball_cy()-a.ball_cy(),2) + pow(b.ball_cz()-a.ball_cz(),2)));
}

/*at every instant of the animation checks the scene 
    and does the next move of every ball(in small time dt)*/
void move(int k)
{
    double r = B[k].ball_radius();
    double cx=B[k].ball_cx();
    double cy=B[k].ball_cy();
    double cz=B[k].ball_cz();
    double dx=B[k].ball_dx();
    double dy=B[k].ball_dy();
    double dz=B[k].ball_dz();
    /*
        several cases of wall collision:
        1. colliding with faces
        2.colliding with two walls simultaneously i.e corner Line

    */
    if(cx<w.right-r and cy<w.top-r and cz < w.front-r and cz>w.back+r and cx>w.left+r and cy>w.bottom+r)
    {
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcY(cy);
        B[k].ball_setcZ(cz);
    }
    else if((cx>=w.right-r and cy<w.top-r and cy >w.bottom+r and cz <w.front-r and cz >w.back+r) or (cx<=w.left+r and cy<w.top-r and cy >w.bottom+r and cz <w.front-r and cz >w.back+r))
    {
        dx=-dx;
        B[k].ball_setdX(dx);
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcY(cy);
        B[k].ball_setcZ(cz);
    }   

    else if((cy>=w.top-r and cx<w.right-r and cx >w.left+r and cz <w.front-r and cz >w.back+r) or (cy<=w.bottom+r and cx<w.right-r and cx >w.left+r and cz <w.front-r and cz >w.back+r))
    {
        dy=-dy;
        B[k].ball_setdY(dy);
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcZ(cz);
        B[k].ball_setcY(cy);
    }
    else if((cz>=w.front-r and cx<w.right-r and cx >w.left+r and cy <w.top-r and cy >w.bottom+r) or (cz<=w.back+r and cx<w.right-r and cx >w.left+r and cy <w.top-r and cy >w.bottom+r))
    {
        dz=-dz;
        B[k].ball_setdZ(dz);
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcZ(cz);
        B[k].ball_setcY(cy);
    }

    else if((cx>=w.right-r and cy>=w.top-r) or (cx>=w.right-r and cy<=w.bottom+r) or (cx<=w.left+r and cy<=w.bottom+r) or (cx<=w.left+r and cy>=w.top-r) )
    {
        dx=-dx;
        dy=-dy;
        B[k].ball_setdY(dy);
        B[k].ball_setdX(dx);
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcY(cy);
        B[k].ball_setcZ(cz);
    }
    else if((cz>=w.front-r and cy>=w.top-r) or (cz>=w.front-r and cy<=w.bottom+r) or (cz<=w.back+r and cy<=w.bottom+r) or (cz<=w.back+r and cy>=w.top-r) )
    {
        dz=-dz;
        dy=-dy;
        B[k].ball_setdY(dy);
        B[k].ball_setdZ(dz);
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcY(cy);
        B[k].ball_setcZ(cz);
    }
    else if((cx>=w.right-r and cz>=w.front-r) or (cx>=w.right-r and cz<=w.back+r) or (cx<=w.left+r and cz<=w.back+r) or (cx<=w.left+r and cz>=w.front-r) )
    {
        dx=-dx;
        dz=-dz;
        B[k].ball_setdZ(dz);
        B[k].ball_setdX(dx);
        cx+=dx;
        cy+=dy;
        cz+=dz;
        B[k].ball_setcX(cx);
        B[k].ball_setcY(cy);
        B[k].ball_setcZ(cz);
    }

}

/*Draws speheres i.e balls*/
void drawCircle(ball b)//draws the circle of the ball on window
{
    
    glPushMatrix();
    glColor3f(b.colour_red,b.colour_green,b.colour_blue);
    glTranslatef(b.ball_cx(), b.ball_cy(), b.ball_cz());
    glTranslatef(0.0,0.0, -2);
    //Lightening Functions and Varaible
    /*
    GLfloat light_diffuse[] = {0.0, 1.0, 1.0, 1.0};   //Blue diffuse light. 
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};   //Infinite light location. */
    // Enable a single OpenGL light. 
    /*
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);*/
    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);
    glutSolidSphere( setrad, 44, 44);
    glPopMatrix();

}

/*Draws the cubic room*/
void drawCube()
{
     int i, j;

     GLfloat color[][3] = {
        { 0.0, 0.0, 0.0 }, 
        { 0.309,0.458,0.792 },  
        { .066,.51,.239 },  
        { 0.94,0.667,0.30},
        { 0.92, 0.25, 0.27 },  
        { .2705, .2705, .2705 } 
     };

     GLfloat cube[][3] = {
        { -1.0, -1.0,  1.0 },
        {  1.0, -1.0,  1.0 },
        {  1.0,  1.0,  1.0 },
        { -1.0,  1.0,  1.0 },
        { -1.0, -1.0, -1.0 },
        {  1.0, -1.0, -1.0 },
        {  1.0,  1.0, -1.0 },
        { -1.0,  1.0, -1.0 }
     };

    /*
     * Specify vertices in counter-clockwise order for each face
     */

    int face[][4] = {
        { 0, 1, 2, 3 },
        { 1, 5, 6, 2 },
        { 5, 4, 7, 6 },
        { 4, 0, 3, 7 },
        { 2, 6, 7, 3 },
        { 4, 5, 1, 0 }
     };

    for ( i = 0; i < 6; i++ )           /* draw all six sides */
    {
        if (i!=0){
        glBegin( GL_POLYGON );
        glColor3f( color[i][0], color[i][1], color[i][2] );
        for ( j = 0; j < 4; j++ )       /* four vertices per side */
        {
            glVertex3fv( cube[face[i][j]] );
        }
        glEnd();
        }   
    }
}

/*Display Function*/
void renderScene()//called every time for rendering the scene and resulting animation
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0.0,0.0, -2);
    glEnable(GL_DEPTH_TEST);
    // glRotatef and glTranslatef
    glRotatef(rotX,1.0,0.0,0.0); // Rotate on x
    glRotatef(rotY,0.0,1.0,0.0); // Rotate on y
    glRotatef(rotZ,0.0,0.0,1.0); // Rotate on z
    glTranslatef(X, Y, Z); 
    drawCube();
    glPopMatrix();
    glFlush();
   controlAgent=new pthread_t[n];
    for(int j=0;j<n;j++)
    {
        drawCircle(B[j]);
        if(pauseRender==0)
         pthread_create(&controlAgent[j],NULL,makeTheScene121,(void*) j);
        
    }
    if(pauseRender==0)
    {
        for(int j=0;j<n;j++)
        {
            pthread_join(controlAgent[j],NULL);
        }
    }
    delete[] controlAgent;
    glutSwapBuffers();    
}


/*Reshape Function*/
void changeSz(int w, int h)//function to take care if the user changes screen size
                            // it keeps the ratios same
{
    if(h==0) h=1;   //if user manipulates the height to 0 the the system shouldn't crash
    double ratio = 1.0*w/h;  //width and height received from openGL
    glMatrixMode(GL_PROJECTION);    //setting up a new matrix
    glLoadIdentity();
    gluPerspective(30,ratio,1,100);
    glViewport(0,0,w,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (rotLx, rotLy, 15 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
}

/*Keyboard Functions*/
void processNormalKeys(unsigned char key, int x, int y) {

    switch (key) {   // x,X,y,Y,z,Z uses the glRotatef() function
        case 'x': // Rotates screen on x axis 
            rotX -= 0.5f;
            break;
        case 'X': // Opposite way 
            rotX += 0.5f;
            break;
        case 'y': // Rotates screen on y axis
            rotY -= 0.5f;
            break;
        case 'Y': // Opposite way
            rotY += 0.5f; 
            break; 
        case 'z': // Rotates screen on z axis
            rotZ -= 0.5f;
            break;
        case 'Z': // Opposite way
            rotZ += 0.5f;
            break;
    // j,J,k,K,l,L uses the gluLookAt function for navigation
        case 'j':
            rotLx -= 0.2f; 
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt (rotLx, rotLy, 15.0 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 'J':
            rotLx += 0.2f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt (rotLx, rotLy, 15.0 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break; 
        case 'k':
            rotLy -= 0.2f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt (rotLx, rotLy, 15.0 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 'K':
            rotLy += 0.2f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt (rotLx, rotLy, 15.0 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 'l':   // It has a special case when the rotLZ becomes 
        // less than -15 the screen is viewed from the opposite side
        // therefore this if statement below does not allow rotLz be less than -15
        if(rotLz + 14 >= 0)
            rotLz -= 0.2f;           
            glMatrixMode(GL_MODELVIEW);    
            glLoadIdentity();  
            gluLookAt (rotLx, rotLy, 15.0 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 'L':
            rotLz += 0.2f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt (rotLx, rotLy, 15.0 + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
        case 'b': // Rotates on x axis by -90 degree
            rotX -= 90.0f;
            break;
        case 'B': // Rotates on x axis by 90 degree
            rotX += 90.0f; 
            break;
        case 'n': // Rotates on y axis by -90 degree
            rotY -= 90.0f;
            break;
        case 'N': // Rotates on y axis by 90 degree
            rotY += 90.0f;
            break;
        case 'm': // Rotates on z axis by -90 degree
            rotZ -= 90.0f; 
            break;
        case 'M': // Rotates on z axis by 90 degree
            rotZ += 90.0f;
            break;
        case 'o': // Default, resets the translations vies from starting view
        case 'O': 
            X = Y = 0.0f;
            Z = 0.0f;
            rotX = 0.0f;
            rotY = 0.0f;
            rotZ = 0.0f;
            rotLx = 0.0f;
            rotLy = 0.0f;
            rotLz = 0.0f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(rotLx, rotLy, 15.0f + rotLz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);  
            break;
    }
    glutPostRedisplay(); // Redraw the scene
    if (key == 's')
    {
        pauseRender=1;
    }
    else if(key == 27)
        exit(0);
    else if(key == 'p')
        pauseRender=0;
    glutPostRedisplay();
}

/*Special Function for special keys*/
void ProcessSpecialKeys(int key, int x, int y) { 

    // The keys below are using the gluLookAt() function for navigation
    switch(key) {
        case GLUT_KEY_LEFT : // Rotate on x axis
            rotX -= 0.4f;
            break;
        case GLUT_KEY_RIGHT : // Rotate on x axis (opposite)
            rotX += 0.4f;
            break;
        case GLUT_KEY_UP : // Rotate on y axis 
            rotY += 0.4f;
            break;
        case GLUT_KEY_DOWN : // Rotate on y axis (opposite)
            rotY -= 0.4f;
            break; 
        case GLUT_KEY_PAGE_UP: // Rotate on z axis
            rotZ -= 0.4f;
            break;
        case GLUT_KEY_PAGE_DOWN:// Rotate on z axis (opposite)
            rotZ += 0.4f;
            break;
    }
    glutPostRedisplay(); // Redraw the scene
}

/*changes speed of all balls by 5% */
void _changeSpeed(int val){
    switch(val){
        case 0:
            for (int i=0;i<n;i++){
                B[i].ball_setdX(B[i].ball_dx()*1.05);
                B[i].ball_setdY(B[i].ball_dy()*1.05);
                B[i].ball_setdY(B[i].ball_dz()*1.05);
            }
            break;
        case 1:
            for (int i=0;i<n;i++){
                B[i].ball_setdX(B[i].ball_dx()*0.95);
                B[i].ball_setdY(B[i].ball_dy()*0.95);
                B[i].ball_setdY(B[i].ball_dz()*0.95);
            }
            break;
        default:
            break;

    }


}
/*Right click menu for speed change*/
void _initMenu(){
    glutCreateMenu (_changeSpeed);//steves_chippy); /* Create the second menu & add items */ 
    glutAddMenuEntry ("High", 0);
    glutAddMenuEntry ("Low", 1);
    
    glutAttachMenu (GLUT_RIGHT_BUTTON);

}

//Main Function
int main(int argc,  char **argv)
{
    srand (static_cast <unsigned> (time(0)));
    cin>>n;
    B=new ball[n];
    w.left = -1.0;
    w.right = 1.0;
    w.top = 1.0;
    w.bottom = -1.0;
    w.front=-1.0;
    w.back=-3.0;
    for(int k=0;k<n;k++){
    
        // it finds randomn velocity and centre positions for all the balls and adds them to our array
        
        int LO=-(1.0/setrad);//maximum number of balls that can come in negative x axis
        int HI=1.0/setrad;//maximum number of balls that can come in positive x axis
        if(HI ==0) HI=1;
        double spLO=-1.0;
        double spHI = 1.0;
        //random velocity generation
        double p = spLO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(spHI-spLO)));//randomn velocity vector
        double q = spLO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(spHI-spLO)));
        double t = spLO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(spHI-spLO)));
        double a,b,c;
        double r=setrad;
        //random postion values keeping in mind the range
        while(a<w.right-r and b<w.top-r and c< w.front-r and c>w.back+r and a>w.left+r and b>w.bottom+r){
            a=((pow(-1,rand() % 100))*(rand() % 100)*.01);
            b=((pow(-1,rand() % 100))*(rand() % 100)*.01);
            c=((pow(-1,rand() % 100))*(rand() % 100)*.01);
        }
        B[k].ball_setcX(a);
        B[k].ball_setcY(b);
        B[k].ball_setcZ(c);
        B[k].ball_setdX(p*.005);
        B[k].ball_setdY(q*.005);
        B[k].ball_setdZ(t*.005);
        B[k].ball_setRad(setrad);
        B[k].index = k;
    }
    glutInit(&argc, argv);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA|GLUT_ALPHA);
    glutInitWindowSize(800,800);
    mainWindow= glutCreateWindow("ScreenSaver");
    _initMenu();
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSz);
    glutIdleFunc(renderScene);//so that when our screensaver is idle it still renders
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(ProcessSpecialKeys);
    glutMouseFunc(processMouse);
    glutMainLoop();//holds our window until closing
    delete[] B;//freeing the allocated memory
    return 0;
}
