
#include "GlutStuff.h"
#include "GLDebugDrawer.h"
#include "btBulletDynamicsCommon.h"
GLDebugDrawer	gDebugDrawer;

#include "CharacterDemo.h"
int main(int argc, char** argv)
{

	CharacterDemo* characterDemo = new CharacterDemo;

	characterDemo->initPhysics();
	characterDemo->getDynamicsWorld()->setDebugDrawer(&gDebugDrawer);

	return glutmain(argc, argv, 640, 480, "Bullet Character Demo. http://www.continuousphysics.com/Bullet/phpBB2/", characterDemo);
}/*

#include "VehicleDemo.h"
int main(int argc,char** argv)
{

        VehicleDemo* vehicleDemo = new VehicleDemo;

        vehicleDemo->initPhysics(); 
		vehicleDemo->getDynamicsWorld()->setDebugDrawer(&gDebugDrawer);

        return glutmain(argc, argv,640,480,"Bullet Vehicle Demo. http://www.continuousphysics.com/Bullet/phpBB2/", vehicleDemo);
}*/

