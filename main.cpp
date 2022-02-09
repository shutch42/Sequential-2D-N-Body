#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "body.h"

#define PI 3.1415926

using namespace std;

vector<Body> bodies;
int currIndex;

void loop (int id) {
	double dt = .00003;
	double G = 6.674e-11;
	double r, F, Fx, Fy, ax, ay, theta, x1, x2, y1, y2, m1, m2; 
	for(int j = 0; j < bodies.size(); j++) {
		for(int k = 0; k < bodies.size(); k++) {
			if(j != k) {
				x1 = bodies[j].getX();
				x2 = bodies[k].getX();
				y1 = bodies[j].getY();
				y2 = bodies[k].getY();
				m1 = bodies[j].getMass();
				m2 = bodies[k].getMass();
				
				r = sqrt(pow((x2-x1),2) + pow((y2-y1), 2));
				F = G*m1*m2/pow(r, 2);
				
				theta = atan2((y2-y1),(x2-x1));
				Fx = F*cos(theta);
				Fy = F*sin(theta);
				ax = Fx/m1;
				ay = Fy/m1;

				bodies[j].setVx(bodies[j].getVx() + ax*dt);
				bodies[j].setVy(bodies[j].getVy() + ay*dt);
				bodies[j].setX(bodies[j].getX() + bodies[j].getVx()*dt);
				bodies[j].setY(bodies[j].getY() + bodies[j].getVy()*dt);
			}
		}
	}
	glutPostRedisplay();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);	
	 for(int i = 0; i < bodies.size(); i++) {
		 glVertex2i(bodies[i].getX(), bodies[i].getY());
	 }
	glEnd();
	glutTimerFunc((1/60)*1000, loop, 0);
	glFlush();
}

int getRand(int min, int max) {
	return min + rand() % (max - min);	
}

void loadGalaxy() {
	for(double i = 0; i < 2*PI; i += .01) {
		Body tmp(1e8, getRand(150, 300)*cos(i), getRand(150, 300)*sin(i), getRand(100, 250)*cos(i+PI/2), getRand(100, 250)*sin(i+PI/2));
		bodies.push_back(tmp);
	}
	Body center(1e20, 0, 0, 0, 0);
	bodies.push_back(center);
}

int main(int argc, char** argv) {
	srand(time(0));
	loadGalaxy();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Sequential N-Body");

	glClearColor(0,0,0,1);
	glColor3f(0,1,0);
	glPointSize(1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-780, 780, -420, 420);

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
