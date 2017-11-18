#include "Tiro.h"

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    int i;
    GLfloat x, y;
    glColor3f(R,G,B);
    glPointSize(3);
    glBegin(GL_POLYGON);
        for (i = 0; i < 360; i+=20)
        {
            x = radius * cos(M_PI*i/180);
            y = radius * sin(M_PI*i/180);
            glVertex3f(x, y, 0);
        }
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y, GLfloat thetaPlayer, GLfloat thetaGun, GLfloat deltaGun, GLfloat radius, GLfloat scale, GLfloat gunHeight){
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
    GLfloat elapsedTime = currentTime - lastTime;
    lastTime = currentTime;
    int sinal = velocidade != 0 ? velocidade/abs(velocidade) : 0;
    GLfloat yDelta = (velocidade * sin((90 + thetaPlayer + thetaGun) * M_PI/180.0)) * (sinal);
    GLfloat xDelta = (velocidade * cos((90 + thetaPlayer + thetaGun) * M_PI/180.0)) * (sinal);

    gX = x += xDelta * elapsedTime;
    gY = y += yDelta * elapsedTime;

	glPushMatrix();

        glTranslatef(x, y, 0);
        glRotatef(thetaPlayer, 0, 0, 1);
        glScalef(scale, scale, scale);
        //Desenha arma
        glTranslatef(deltaGun, 0, 0);
        glRotatef(thetaGun, 0, 0, 1);
        glTranslatef(0, gunHeight, 0);
        DesenhaCirc(radius, 1, 0, 0); 
    
    glPopMatrix();
}

GLfloat Tiro::ObtemX(){
	return gX + cos((gThetaPlayer) * M_PI/180.0) * gDeltaGun + cos((90 + gThetaPlayer + gThetaGun) * M_PI/180.0) * gGunHeight;
}

GLfloat Tiro::ObtemY(){
	return gY + sin((gThetaPlayer) * M_PI/180.0) * gDeltaGun + sin((90 + gThetaPlayer + gThetaGun) * M_PI/180.0) * gGunHeight;
}

GLfloat Tiro::ObtemRaio(){
	return gRadius;
}