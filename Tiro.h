#ifndef TIRO_H
#define TIRO_H
#include <GL/glut.h>
#include <iostream>
#include <cmath>

class Tiro
{
private:
	GLfloat gX;
	GLfloat gY;
	GLfloat gThetaPlayer;
	GLfloat gThetaGun;
	GLfloat gDeltaGun;
	GLfloat gGunHeight;
	GLfloat gRadius;
    GLfloat lastTime;
    GLfloat gScale;
	double velocidade;
public:
	Tiro(GLfloat x, GLfloat y, GLfloat thetaPlayer, GLfloat thetaGun, GLfloat deltaGun, GLfloat radius, double velTiro, GLfloat scale, GLfloat gunHeight){
		gX = x;
		gY = y;
		gThetaPlayer = thetaPlayer;
		gThetaGun = thetaGun;
		gDeltaGun = deltaGun;
		gRadius = radius;
		velocidade = velTiro;
		gScale = scale;
		gGunHeight = gunHeight;
        lastTime = glutGet(GLUT_ELAPSED_TIME);
	}
	void DesenhaCirc(GLint, GLfloat, GLfloat, GLfloat);
	void DesenhaTiro(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
	void Desenha(){
		DesenhaTiro(gX, gY, gThetaPlayer, gThetaGun, gDeltaGun, gRadius, gScale, gGunHeight);
	}
	GLfloat ObtemX();
	GLfloat ObtemY();
	GLfloat ObtemRaio();
	~Tiro();
	
};

#endif