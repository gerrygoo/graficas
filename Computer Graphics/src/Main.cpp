#include "scene_manager.h" 
#include <GL/glew.h>
#include <GL/freeglut.h>

//#include "time_util.h"


//float angle = 0.0;
//
//void renderLoop() {
//	time_util::tick();
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	// glClearColor(sin(time_util::elapsed_time().count()), 1.0f, 0.5f, 1.0f);
//
//	// Warning old
//
//	glPushMatrix();
//
//	glRotatef(angle++, 0.0f, 0.0f, 1.0f);
//	glBegin(GL_TRIANGLES);
//
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glVertex2f(-1.0f, -1.0f);
//
//	glColor3f(0.0f, 0.0f, 1.0f);
//	glVertex2f(1.0f, -1.0f);
//
//	glColor3f(0.0f, 1.0f, 0.0f);
//	glVertex2f(0.0f, 1.0f);
//
//	glEnd();
//
//	glPopMatrix();
//
//	// end Warning old
//
//	glutSwapBuffers();
//}
//
//void idle() {
//	glutPostRedisplay();
//}

int main(int argc, char* argv[])
{
	//glutInit(&argc, argv);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	//glutInitWindowSize(400, 400);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//glutCreateWindow("Ushia");


	//glutDisplayFunc(renderLoop);
	//glutIdleFunc(idle);

	//glewInit();

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//
	//// color here
	//glClearColor(1.0f, 1.0f, 0.5f, 1.0f);

	//std::cout << glGetString(GL_VERSION) << std::endl;

	//time_util::init();
	//
	//glutMainLoop();

	//scene_manager::start(argc, argv, "Hello, World!", 400, 400);
	scene_manager::start(argc, argv, "Hello, World!", 800, 800);

	return 0;
}