#ifndef ROBO_H
#define	ROBO_H
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <list>
#include "Tiro.h"
// Dimensions
#define legHeight .7
#define legWidth .4
#define gunHeight 1
#define gunWidth .2
#define bodyScale .3
#define headScale .6

using namespace std;

class Player {
    GLfloat gX;
    GLfloat gY;
    GLfloat gThetaLeg;
    GLfloat gThetaGun;
    GLfloat gThetaPlayer;
    GLfloat radius;
    GLfloat inJumpScale;
    GLfloat jumpInitTime;
    GLfloat lastTime;
    GLfloat elapsedTime;
    double velocidade;
    double velocidadeTiro;
    double freqTiro;
    bool inJump;
    bool above;
    list<Tiro*> tiros;

private:
    void DesenhaRect(  GLint height, GLint width,
                    GLfloat R, GLfloat G, GLfloat B);
    void DesenhaCirc(  GLint radius, GLfloat R,
                    GLfloat G, GLfloat B);
    void DesenhaPernas(GLfloat x, GLfloat y, GLfloat gThetaLeg);
    void DesenhaPlayer(GLfloat gX, GLfloat gY, GLfloat radius,
                    GLfloat gThetaLeg, GLfloat gThetaGun, GLfloat gThetaPlayer);

public:
    Player(GLfloat posX, GLfloat posY, GLfloat r, double vel, double velTiro, double freq = 0){
        gX = posX;
        gY = posY;
        radius = r;
        gThetaLeg = 90;
        gThetaGun = 0;
        gThetaPlayer = 0;
        inJumpScale = 1;
        inJump = false;
        above = false;
        velocidade = vel;
        velocidadeTiro = velTiro;
        freqTiro = freq;
        lastTime = glutGet(GLUT_ELAPSED_TIME);
    };
    void Desenha(){
        DesenhaPlayer(gX, gY, radius, gThetaLeg, gThetaGun, gThetaPlayer);
    };
    void RodaPlayer(int);
    void RodaArma(GLfloat);
    void Move(int, bool);
    void Pula();
    void Atira();
    void DeterminaAcima(bool);
    GLfloat ObtemX();
    GLfloat ObtemY();
    GLfloat tryToMoveX(int);
    GLfloat tryToMoveY(int);
    GLfloat ObtemRaio();
    bool EstaPulando();
    bool EstaAcima();
    list<Tiro*> ObtemTiros();
    void RemoveTiro(Tiro*);

};

#endif	/* ROBO_H */

