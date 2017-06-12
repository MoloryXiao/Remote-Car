#ifndef __POINTANALYSIS_H
#define __POINTANALYSIS_H	  

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


typedef struct
{
  int x,y;
}Point;

// y=x line, explore Point(p)'s location
// return 1 means above
// return 0 means below
int yEx(Point p)
{
  return p.x<p.y;
}
// y=-x+1024 line, explore Point(p)'s location
// return 1 means above
// return 0 means below
int yEx4(Point p)
{
  return -p.x+1024<p.y;
}

// return p's loc on map
// return 1 means up, 2 means down
// return 3 means left, 4 means right
int DirectionGet(Point p)
{
  int dir_1=yEx(p);
  int dir_2=yEx4(p);
  if(dir_1 && dir_2) return 1;      //上
  if(dir_1 && dir_2+1) return 3;    //左
  if(dir_1+1 && dir_2) return 4;    //右
  return 2;   //下
}
// According to p, return speed
// return 0-4 means no v or 1-4 v
// X Y according 0-1024
int SpeedGet(Point p)
{
  int x=p.x,y=p.y;
  
  if(p.x<112 || p.x>912 || p.y<112 || p.y>912) return 4;
  if(p.x<212 || p.x>812 || p.y<212 || p.y>812) return 3;
  if(p.x<312 || p.x>712 || p.y<312 || p.y>712) return 2;
  if(p.x<412 || p.x>612 || p.y<412 || p.y>612) return 1;
  return 0; // p.x:412-612  p.y:412-612
}
#endif