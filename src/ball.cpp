//
//  ball.cpp
//  openGlprac
//
//  Created by Akash Tanwar on 11/01/15.
//  Copyright (c) 2015 Akash Tanwar. All rights reserved.
//

#include "ball.h"
#include <pthread.h>
#include <queue>

ball::ball(){
    r=0;
    colour_red=(double)0/255;
    colour_green=(double)255/255;
    colour_blue=(double)255/255;
    cx=0;
    cy=0;
    cz=0;
    m=1;
    dx=0;
    dy=0;
    dz=0;
    is_Collided=false;
}
float ball::ball_mass(){
    return m;
}
float ball::ball_radius(){
    return r;
}
float ball::ball_cx(){
    return cx;
}
float ball::ball_cy(){
    return cy;
}
float ball::ball_cz(){
    return cz;
}
float ball::ball_dx(){
    return dx;
}
float ball::ball_dy(){
    return dy;
}
float ball::ball_dz(){
    return dz;
}
bool ball::ball_isCollided(){
   return is_Collided;
}
void ball::ball_setcX(float val){
    cx = val;
}
void ball::ball_setcY(float val){
    cy = val;
}
void ball::ball_setcZ(float val){
    cz=val;
}
void ball::ball_setRad(float val){
    r = val;
}
void ball::ball_setdX(float val){
    dx = val;
}
void ball::ball_setdY(float val){
    dy = val;
}
void ball::ball_setdZ(float val){
    dz=val;
}
mailbox ball::ball_info(){
    mailbox temp;
    temp.x_c=ball_cx();
    temp.y_c=ball_cy();
    temp.z_c=ball_cz();
    temp.x_v=ball_dx();
    temp.y_v=ball_dy();
    temp.z_v=ball_dz();
    temp.r=ball_radius();
    temp.m=ball_mass();    
    return temp;
}

