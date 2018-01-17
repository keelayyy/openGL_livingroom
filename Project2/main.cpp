//Programming Assignment 2
//Kyle James
//Draws a shaded scene of a living room using OpenGL objects
//Controls:
//	z/Z - rotate clock/counterclockwise along x-axis
//	x/X - '' y-axis
//	c/C - '' z-axis
//	r - reset rotation and movement
//	i - snap to x-axis view
//	o - snap to y-axis view
//	p - snap to z-axis view
//	a/q - move left/right along x-axis
//	s/w - '' y-axis
//	d/e - '' z-axis
//	left click - zoom in (using ortho coords)
//	right click - zoom out (using ortho coords)
//	middle click - end program

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

GLfloat zoom = 1.0; //controls zooming in or out of scene
GLfloat rotateAngle = 0; //controls rotation of spinning fan blades
GLfloat Xangle = 0.0; //controls camera rotation around the scene
GLfloat Yangle = 0.0;
GLfloat Zangle = 0.0;
GLfloat Xmove = 0.0;
GLfloat Ymove = 0.0;
GLfloat Zmove = 0.0;
bool check = true;

GLfloat mat_ambient_grey[] = { 0.7f, 0.7f, 0.7f, 1.0f }; //grey (for walls too)
GLfloat mat_diffuse_grey[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat mat_ambient_darkbrown[] = { 0.44f, 0.23f, 0.04f, 1.0f }; //dark brown
GLfloat mat_diffuse_darkbrown[] = { 0.34f, 0.13f, 0.00f, 1.0f };
GLfloat mat_ambient_lightbrown[] = { 0.62f, 0.42f, 0.22f, 1.0f }; //light brown
GLfloat mat_diffuse_lightbrown[] = { 0.62f, 0.42f, 0.12f, 1.0f };
GLfloat mat_ambient_lightblue[] = { 0.24f, 0.74f, 0.89f, 1.0f }; //light blue
GLfloat mat_diffuse_lightblue[] = { 0.14f, 0.64f, 0.79f, 1.0f };
GLfloat mat_ambient_lavender[] = { 0.91f, 0.65f, 0.80f, 1.0f }; //lavender
GLfloat mat_diffuse_lavender[] = { 0.81f, 0.55f, 0.70f, 1.0f };
GLfloat mat_ambient_white[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //white
GLfloat mat_diffuse_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat mat_specular_grey[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //applied to entire scene
GLfloat mat_shininess_grey[] = { 50.0f }; //applied to entire scene

//draw a wall
//parts:	wall (stretched cube)
void wall(GLfloat thickness)
{ // draw thin wall with top = xz-plane, corner at origin 
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_grey);
	glPushMatrix();
	glTranslated(0.5, 0.5*thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

//draw a floor fan with three blades that will rotate clockwise
//parts:	base (stubby cylinder)
//			pole (long thin cylinder)
//			center of blades (sphere)
//			three blades (stretched spheres)
void floorFan() {
	//color: light blue
	//set material color properties to light blue
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lightblue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lightblue);

	//base
	glTranslatef(0.0, 0.02, 0.0);
	glPushMatrix();
	glRotated(-90.0, 1, 0, 0);
	glutSolidCylinder(0.075, 0.02, 100, 100);
	glPopMatrix();

	//pole
	glPushMatrix();
	glRotated(-90.0, 1, 0, 0);
	glutSolidCylinder(0.015, 0.6, 100, 100);
	glPopMatrix();

	//center of blades
	glTranslatef(0.0, 0.6, 0.0);
	glPushMatrix();
	glutSolidSphere(0.025, 100, 100);
	glPopMatrix();

	//three blades
	glTranslatef(0.015, 0.0, 0.0);
	for (int i = 1; i <= 3; i++) {
		GLfloat angle = rotateAngle + i * 2 * 3.14159 / 3;
		glPushMatrix();
		//translate to origin, rotate, and return
		glRotatef(angle * -180 / 3.14159, 1, 0, 0);
		glTranslatef(0.0, sin(angle) * -0.125, cos(angle) * -0.125);
		glScaled(0.05, sin(angle) * 0.9, cos(angle) * 0.9);
		glutSolidSphere(0.15, 100, 100);
		glPopMatrix();
	}
	check = false;
}

//draw a table lamp
//parts:	base (stubby cylinder)
//			pole (long thin cylinder)
//			lampshade (cone)
void tableLamp() {
	//base
	glTranslated(0.0, 0.17, 0.0);
	glPushMatrix();
	//set material color properties to lavender
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lavender);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lavender);
	glRotated(-90.0, 1, 0, 0);
	glScaled(1.0, 1.0, 0.15);
	glutSolidCylinder(0.05, 0.1, 100, 100);
	glPopMatrix();

	//pole
	glPushMatrix();
	//set material color properties to lavender
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lavender);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lavender); 
	glRotated(-90.0, 1, 0, 0);
	glScaled(0.1, 0.1, 1.5);
	glutSolidCylinder(0.15, 0.1, 100, 100);
	glPopMatrix();

	//lampshade
	glTranslatef(0.0, 0.1, 0.0);
	glPushMatrix();
	//set material color properties to white
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glRotated(-90.0, 1, 0, 0);
	glutSolidCone(0.10, 0.125, 100, 100);
	glPopMatrix();
}

//draw a table leg
//parts:	leg (tall, thin cube)
void tableLeg(GLfloat thick, GLfloat len)
{ // draw a table leg with dimension: thick x len x thick 
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

//draw a coffee table
//parts:	top (thin prism - double length than width)
//			four legs (tall, thin prisms)
void coffeeTable(GLfloat topWid, GLfloat topThick, GLfloat legThick, GLfloat legLen)
{
	//set material color properties to brown
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lightbrown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lightbrown);

	//tabletop
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid / 2.0, topThick, topWid); 
	glutSolidCube(1.0);
	glPopMatrix();

	//lower shelf
	glPushMatrix();
	glTranslated(0, legLen / 2, 0);
	glScaled(topWid / 2.0 - 0.05, topThick, topWid - 0.05);
	glutSolidCube(1.0);
	glPopMatrix();

	GLfloat distX = 0.95 * topWid / 4.0 - 0.95 * legThick / 2.0;
	GLfloat distZ = 0.95 * topWid / 2.0 - 0.95 * legThick / 2.0;
	//table legs (offset slightly inside the edges of the tabletop)
	glPushMatrix();
	glTranslated(distX, 0, distZ);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * distZ);
	tableLeg(legThick, legLen);
	glTranslated(-2 * distX, 0, 2 * distZ);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * distZ);
	tableLeg(legThick, legLen);
	glPopMatrix();
}

//draws a sofa. width = z, height = y, depth = x
//parts:	back (cube)
//			2 arms (cubes)
//			2 cushions (cubes)
//			base (cube)
void sofa(GLfloat height, GLfloat width, GLfloat depth) {
	//set material color properties
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_darkbrown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_darkbrown);

	GLfloat backdepth = depth / 4;
	GLfloat armheight = 3 * height / 4;
	GLfloat armwidth = width / 8;
	GLfloat cushionheight = height / 4;
	GLfloat baseheight = height / 3;

	//back
	glPushMatrix();
	glTranslated(-depth / 2 + backdepth / 2, height / 2, 0);
	glScaled(backdepth, height, width);
	glutSolidCube(1.0);
	glPopMatrix();

	////arms
	glPushMatrix();
	glTranslated(0, armheight / 2, 1.1 * (width / 2 - armwidth / 2));
	glScaled(depth, armheight, armwidth);
	glutSolidCube(1.0);
	glPopMatrix(); 

	glPushMatrix();
	glTranslated(0, armheight / 2, 1.1 * (-width / 2 + armwidth / 2));
	glScaled(depth, armheight, armwidth);
	glutSolidCube(1.0);
	glPopMatrix();

	////cushions
	glPushMatrix();
	glTranslated(0, baseheight + cushionheight / 2, width/ -4 - 0.01);
	glScaled(depth * 0.95, cushionheight, width / 2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, baseheight + cushionheight / 2, width / 4 + 0.01);
	glScaled(depth * 0.95, cushionheight, width / 2);
	glutSolidCube(1.0);
	glPopMatrix();

	//base
	glPushMatrix();
	glTranslated(0, baseheight / 2, 0);
	glScaled(depth, baseheight, width);
	glutSolidCube(1.0);
	glPopMatrix();
}

//draws a plate (sphere mostly flattened wrt z-axis)
//rotX - rotation in x-direction
//rotY - rotation in y-direction
//rotZ - rotation in z-direction
//radius - radius of plate (0.0 to 1.0)
void drawPlate(GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat radius) {
	glPushMatrix();
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	glRotatef(rotZ, 0, 0, 1);
	glScalef(1.0, 1.0, 0.01);
	glutSolidSphere(radius, 100, 100);
	glPopMatrix();
}

//draws some china cabinets. width = x, height = y, depth = z
//parts:	back (cube)
//			sides (cubes)
//			top (cube)
//			shelf (cube)
//			bottom drawer (cube)
//			glass doors? (cubes)
void cabinets(GLfloat width, GLfloat height, GLfloat depth) {
	//set color
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_lightbrown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_lightbrown);

	GLfloat backdepth = depth / 15;
	GLfloat sidewidth = width / 12;
	GLfloat topheight = 14 * height / 15;
	GLfloat topthick = sidewidth;
	GLfloat shelfheight = 3 * height / 5;
	GLfloat shelfthick = sidewidth / 2;
	GLfloat interiordepth = depth * 0.95;
	GLfloat drawerheight = height * 0.33;
	GLfloat drawerbottom = height / 24;
	GLfloat drawerdetail_widthoffset = 2 * sidewidth;
	GLfloat drawerdetail_depthoffset = depth * 0.1;
	GLfloat plateradius = width / 8;
	GLfloat plate_centeroffset = width / 6;
	GLfloat teapotsize = plateradius;
	GLfloat doorheight = height * 0.67;

	//back
	glPushMatrix();
	glTranslated(0, height / 2, depth / -2 + backdepth / 2);
	glScaled(width, height, backdepth);
	glutSolidCube(1.0);
	glPopMatrix();

	//sides
	glPushMatrix();
	glTranslated(width / -2 + sidewidth / 2, height / 2, 0);
	glScaled(sidewidth, height, depth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(width / 2 - sidewidth / 2, height / 2, 0);
	glScaled(sidewidth, height, depth);
	glutSolidCube(1.0);
	glPopMatrix();

	//top
	glPushMatrix();
	glTranslated(0, topheight, 0);
	glScaled(width, topthick, depth);
	glutSolidCube(1.0);
	glPopMatrix();

	//shelf
	glPushMatrix();
	glTranslated(0, shelfheight, 0);
	glScaled(width, shelfthick, interiordepth);
	glutSolidCube(1.0);
	glPopMatrix();

	//drawer
	glPushMatrix();
	glTranslated(0, drawerheight / 2 + drawerbottom, 0);
	glScaled(width, drawerheight, interiordepth);
	glutSolidCube(1.0);
	glPopMatrix();

	//extras

	//draw detail on drawer
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_darkbrown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_darkbrown);
	glPushMatrix();
	glTranslated(0, drawerheight / 2 + drawerbottom, 0.005);
	glScaled((width - drawerdetail_widthoffset) * 0.95, drawerheight - drawerdetail_widthoffset, interiordepth);
	glutSolidCube(1.0);
	glPopMatrix();
	
	//drawer handle
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glPushMatrix();
	glTranslated(0, drawerheight / 2 + 0.025, 0.006);
	glScaled(drawerdetail_widthoffset, drawerdetail_widthoffset, interiordepth * 0.5);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();
	
	//plates
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glTranslatef(-plate_centeroffset, shelfheight + plateradius, 0);
	drawPlate(-30, 0, 0, plateradius);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(plate_centeroffset, shelfheight + plateradius, 0);
	drawPlate(-30, 0, 0, plateradius);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(plate_centeroffset, drawerheight + drawerbottom + plateradius, 0);
	drawPlate(-30, 0, 0, plateradius);
	glPopMatrix();
	
	//teapot
	glPushMatrix();
	glTranslatef(-plate_centeroffset, drawerheight + drawerbottom + teapotsize * 0.7, 0);
	glutSolidTeapot(teapotsize);
	glPopMatrix();
	
	//glass door
	glDisable(GL_LIGHTING | GL_LIGHT0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat mat_ambient_glass[] = { 1.0f, 1.0f, 1.0f, 0.3f }; //white, small alpha
	GLfloat mat_diffuse_glass[] = { 1.0f, 1.0f, 1.0f, 0.3f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_glass);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_glass);
	glPushMatrix();
	glTranslatef(0, doorheight - (height - topheight) + drawerbottom, depth / 2 - backdepth / 2);
	glScalef(width - sidewidth * 2, height - drawerheight - drawerbottom, backdepth * 0.97);
	glutSolidCube(1.0);
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING | GL_LIGHT0);
}

//display callback function. Controls creation and display of all scene elements
//	as well as lighting elements
void displaySolid(void)
{
	// set properties of the surface material
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_grey);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_grey);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess_grey);

	// set the light source properties
	GLfloat lightIntensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

	// set the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0 + Xangle * 3.14 / 180, 1.0 + Yangle * 3.14 / 180, 1.0 + Zangle * 3.14 / 180, 
		0.0 + Xmove, 0.25 + Ymove, 0.0 + Zmove, 
		0.0, 1.0, 0.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat winHt = 1.0;  //half-height of the window
	glOrtho(-winHt * 64 / 48.0*zoom, winHt * 64 / 48.0*zoom, -winHt*zoom, winHt*zoom, 0.1, 100.0);
	
	// start drawing
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

	glPushMatrix();
	glTranslated(0.15, 0.02, 0.45);
	sofa(0.3, 0.5, 0.225);  // draw the sofa
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, 0, 0.5);
	coffeeTable(0.5, 0.02, 0.02, 0.15);  // draw the table
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.6, 0, 0.1);
	cabinets(0.4, 0.6, 0.2);  // draw the cabinets
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.15, 0, 0.85);
	floorFan();  // draw the fan
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0.5, 0.0, 0.4);
	tableLamp();  // draw the lamp
	glPopMatrix();

	//display walls
	wall(0.02);  // wall no 1: in xz-plane

	glPushMatrix();
	glRotated(90.0, 0.0, 0.0, 1.0);
	wall((GLfloat)0.02); //wall no 2: in xy-plane
	glPopMatrix();

	glPushMatrix();
	glRotated(-90.0, 1.0, 0.0, 0.0);
	wall(0.02); //wall no 3: in yz-place
	glPopMatrix();

	glFlush();
	glutSwapBuffers(); // display the scene just made
}

void myMouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			zoom = zoom * 0.9;
			glutPostRedisplay(); // draw it again
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			zoom = zoom * 1.1;
			glutPostRedisplay(); // draw it again
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN)
			exit(-1);
		break;
	default:
		break;
	}
}

// Keyboard input processing routine.
void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'z':
		Xangle += 5.0;
		if (Xangle > 360.0) Xangle = 360.0;
		glutPostRedisplay();
		break;
	case 'Z':
		Xangle -= 5.0;
		if (Xangle < 0.0) Xangle = 0;
		glutPostRedisplay();
		break;
	case 'x':
		Yangle += 5.0;
		if (Yangle > 360.0) Yangle = 360.0;
		glutPostRedisplay();
		break;
	case 'X':
		Yangle -= 5.0;
		if (Yangle < 0.0) Yangle = 0;
		glutPostRedisplay();
		break;
	case 'c':
		Zangle += 5.0;
		if (Zangle > 360.0) Zangle = 360.0;
		glutPostRedisplay();
		break;
	case 'C':
		Zangle -= 5.0;
		if (Zangle < 0.0) Zangle = 0;
		glutPostRedisplay();
		break;
	case 'r':
		Xangle = 0; Yangle = 0; Zangle = 0;
		Xmove = 0; Ymove = 0; Zmove = 0;
		glutPostRedisplay();
		break;
	case 'i': //quick snap to x-alignment
		Xangle = 360;
		glutPostRedisplay();
		break;
	case 'o': //quick snap to y-alignment
		Yangle = 360;
		glutPostRedisplay();
		break;
	case 'p': //quick snap to z-alignment
		Zangle = 360;
		glutPostRedisplay();
		break;
	case 'a': //move left along x-axis
		Xmove -= 0.1;
		glutPostRedisplay();
		break;
	case 'q': //move right along x-axis
		Xmove += 0.1;
		glutPostRedisplay();
		break;
	case 's': //move left along y-axis
		Ymove -= 0.1;
		glutPostRedisplay();
		break;
	case 'w': //move right along y-axis
		Ymove += 0.1;
		glutPostRedisplay();
		break;
	case 'd': //move left along z-axis
		Zmove -= 0.1;
		glutPostRedisplay();
		break;
	case 'e': //move right along z-axis
		Zmove += 0.1;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void onIdle()
{
	rotateAngle += 0.005;
	if (rotateAngle > 360) {
		rotateAngle = 0;
	}
	glutPostRedisplay();
}

//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Programming Assignment 2 - Living Room Scene");
	glutDisplayFunc(displaySolid);
	glutMouseFunc(myMouse);                 // register myMouse function
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(onIdle);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);  // background is light gray
	glViewport(0, 0, 640, 480);
	glutMainLoop();
}