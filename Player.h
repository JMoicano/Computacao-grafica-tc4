#ifndef ROBO_H
#define	ROBO_H
#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <list>
#include "Tiro.h"
#include "Circle.h"
// Dimensions
#define legHeight .7
#define legWidth .4
#define gunHeight 1
#define gunWidth .2
#define bodyScale .3
#define headScale .6
#define totalJumpTime 2.0

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
    GLfloat initJumpScale;
    GLfloat lastTime;
    GLfloat lastShot;
    GLfloat elapsedTime;
    GLfloat animacaoArma;
    double velocidade;
    double velocidadeTiro;
    double freqTiro;
    double obstacleHigh;
    double jumpTime;
    bool inJump;
    bool above; 
    bool canMove;
    bool cantMoveAbove;
    bool falling;
    int aboveI;
    list<Tiro*> tiros;

private:
    void DesenhaRect(  GLint height, GLint width,
                    GLfloat R, GLfloat G, GLfloat B);
    void DesenhaCirc(  GLint radius, GLfloat R,
                    GLfloat G, GLfloat B);
    void DesenhaPernas(GLfloat x, GLfloat y, GLfloat gThetaLeg);
    void DesenhaPlayer(GLfloat gX, GLfloat gY, GLfloat radius,
                    GLfloat gThetaLeg, GLfloat gThetaGun, GLfloat gThetaPlayer);
    double dist(Player *, int);
    double dist(Circle *, int);

public:
    Player(GLfloat posX, GLfloat posY, GLfloat r, double vel, double velTiro, double oHigh, double freq = 0){
        gX = posX;
        gY = posY;
        radius = r;
        gThetaLeg = 90;
        gThetaGun = 0;
        gThetaPlayer = 0;
        inJumpScale = 1;
        initJumpScale = 1;
        jumpTime = 0;
        animacaoArma = 1;
        obstacleHigh = oHigh/100.0;
        inJump = false;
        above = false;
        falling = false;
        canMove = true;
        cantMoveAbove = false;
        aboveI = -1;
        velocidade = vel;
        velocidadeTiro = velTiro;
        freqTiro = freq;
        lastShot = 0;
        lastTime = glutGet(GLUT_ELAPSED_TIME);
        srand((unsigned)lastTime);
    };
    void Desenha(){
        DesenhaPlayer(gX, gY, radius, gThetaLeg, gThetaGun, gThetaPlayer);
    };
    void RodaPlayer(int);
    void RodaArma(GLfloat);
    void Move(int);
    void Pula();
    void Atira();
    void Para();
    GLfloat ObtemX();
    GLfloat ObtemY();
    GLfloat tryToMoveX(int);
    GLfloat tryToMoveY(int);
    GLfloat ObtemRaio();
    list<Tiro*> ObtemTiros();
    void RemoveTiro(Tiro*);
    void checkCollision(Circle *, int, bool intern = false);
    void checkCollision(Player *, int);
    void checkCollisionJumpable(Circle *, int, int);
    void anima();

};

#endif	/* ROBO_H */

