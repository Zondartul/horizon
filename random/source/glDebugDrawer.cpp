//copied from this post:
//http://sio2interactive.forumotion.net/t599-enabling-bullet-debug-draw-code-included
//and adapted.
#include "glDebugDrawer.h"
#include "GL/gl.h"
#include <stdio.h> //printf debugging
#include "vec.h"
#include "paint.h"

GLDebugDrawer::GLDebugDrawer()
:m_debugMode(0)
{
   
}

void    GLDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
   //      if (m_debugMode > 0)
   {
      /* float tmp[ 6 ] = { from.getX(), from.getY(), from.getZ(),
                    to.getX(), to.getY(), to.getZ() }; */
      
      /* glPushMatrix();
      {         
         glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);         
         glVertexPointer( 3,
                     GL_FLOAT,
                     0,
                     &tmp );
         
         glPointSize( 5.0f );
         glDrawArrays( GL_POINTS, 0, 2 );
         glDrawArrays( GL_LINES, 0, 2 );
      }
      glPopMatrix(); */    
	  setColor((vec3ub){color.getX()*255.0,color.getY()*255.0,color.getZ()*255.0});
	  setAlpha(255);
	  drawLine3D((vec3f){from.getX(), from.getY(), from.getZ()},(vec3f){to.getX(), to.getY(), to.getZ()});
   }
}

void    GLDebugDrawer::setDebugMode(int debugMode)
{
   m_debugMode = debugMode;
}

void    GLDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
   //glRasterPos3f(location.x(),  location.y(),  location.z());
   //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void    GLDebugDrawer::reportErrorWarning(const char* warningString)
{
   printf(warningString);
}

void    GLDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
   {
	   vec3f point = (vec3f){pointOnB.getX(), pointOnB.getY(), pointOnB.getZ()};
	   vec3f normal = (vec3f){normalOnB.getX(),normalOnB.getY(),normalOnB.getZ()};
	   renderContext{
		//printf("draw cp: (%.3f,%.3f,%.3f),(%.3f,%.3f,%.3f),%.3f\n",point.x,point.y,point.z,normal.x,normal.y,normal.z,distance);   
		glDisable(GL_DEPTH_TEST);
		setThickness(3);
		setColor({255,0,0});
		drawPoint3D(point);
		setThickness(1);
		setColor({0,0,255});
		drawLine3D(point,point+normal);
		glEnable(GL_DEPTH_TEST);
	   }
      //btVector3 to=pointOnB+normalOnB*distance;
      //const btVector3&from = pointOnB;
      //glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   
      
      //GLDebugDrawer::drawLine(from, to, color);
      
      //glRasterPos3f(from.x(),  from.y(),  from.z());
      //char buf[12];
      //sprintf(buf," %d",lifeTime);
      //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
   }
}
