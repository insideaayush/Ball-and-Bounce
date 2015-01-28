//
//  ball.h
//  openGlprac
//
//  Created by Akash Tanwar on 11/01/15.
//  Copyright (c) 2015 Akash Tanwar. All rights reserved.
//

#ifndef __openGlprac__ball__
#define __openGlprac__ball__
#include <pthread.h>
#include <stdio.h>
#include <queue>

struct mailbox{
    float x_c;
    float y_c;
    float z_c;
    float x_v;
    float y_v;
    float z_v;
    float r;
    float m;
};

class ball{
private:
    float cx,cy,cz,r,m,dx,dy,dz;
    bool is_Collided;
    //pthread_t id;
     
public:
    std:: queue<mailbox> message;
    ball();
    int index;
    float colour_red;
    float colour_green;
    float colour_blue;
    float ball_mass();  //mass of the ball if required
    float ball_radius();    //radius of the ball
    float ball_cx();    //x coordinate of the centre
    float ball_cy();    //y coordinate of the centre
    float ball_cz();    //z coordinate of the centre
    float ball_dx();    //velocity vector along x axis
    float ball_dy();    //velocity vector along y axis
    float ball_dz();
    void ball_setRad(float val);    //set implies changing the value
    void ball_setcX(float val);
    void ball_setcY(float val);
    void ball_setcZ(float val);
    void ball_setdX(float val);
    void ball_setdY(float val);
    void ball_setdZ(float val);
    bool ball_isCollided();
    mailbox ball_info();
  //  mailbox ball_Message(){return message;};
   // pthread_t ball_id();    //to get the pthread_t id of the ball, not changeable
};
#endif /* defined(__openGlprac__ball__) */
