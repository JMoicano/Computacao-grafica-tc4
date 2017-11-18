#include "Player.h"
#include <math.h>

void Player::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
     glColor3f(R,G,B);
 
     glBegin(GL_QUADS);
          glVertex2f( -width/2.0, 0.0);                        // Top Left
          glVertex2f( -width/2.0, height);               // Bottom Left
          glVertex2f( width/2.0, height);               // Bottom Right
          glVertex2f( width/2.0, 0.0);                // Top Right
     glEnd();

    glColor3f(0,0,0);
    glLineWidth( 2.0 );
    glBegin(GL_LINE_STRIP);
          glVertex2f( -width/2.0, 0.0);
          glVertex2f( -width/2.0, height);
          glVertex2f( width/2.0, height);
          glVertex2f( width/2.0, 0.0);
          glVertex2f( -width/2.0, 0.0);
     glEnd();
}

void Player::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
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
    glColor3f(0,0,0);
    glLineWidth( 2.0 );
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < 360; i+=20)
        {
            x = radius * cos(M_PI*i/180);
            y = radius * sin(M_PI*i/180);
            glVertex3f(x, y, 0);
        }
        x = radius * cos(0);
        y = radius * sin(0);
        glVertex3f(x, y, 0);
    glEnd();
    glPointSize(3);
}

void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat radius, GLfloat thetaLeg, GLfloat thetaGun, GLfloat thetaPlayer)
{
    double R = freqTiro == 0 ? 1 : 0;
    double G = freqTiro == 0 ? 0 : 1;
    if(inJump){
        double jumpTime = (glutGet(GLUT_ELAPSED_TIME) - jumpInitTime)/1000.0;
        if(jumpTime < 2){
            inJumpScale = 1 + (.5 * (jumpTime*(2 - jumpTime)));
        }else{
            inJumpScale = 1;
            inJump = false;
        }
    }
    glPushMatrix();

        glTranslatef(x, y, 0);
        glRotatef(thetaPlayer, 0, 0, 1);
        glScalef( inJumpScale, inJumpScale, 1);

        glPushMatrix();

            // Desenha perna direita
            glTranslatef(.6 * radius, 0, 0);
            glScalef(1, sin(thetaLeg), 1);
            DesenhaRect( legHeight * radius,  legWidth * radius, 0, 0, 0); 
        
        glPopMatrix();

        glPushMatrix();
            
            //Desenha perna esquerda
            glTranslatef(-.6 * radius, 0, 0);
            glScalef(1, -sin(thetaLeg), 1);
            DesenhaRect( legHeight * radius,  legWidth * radius, 0, 0, 0); 
        
        glPopMatrix();
        
        glPushMatrix();

            //Desenha arma
            glTranslatef(.8 * radius, 0, 0);
            glRotatef(thetaGun, 0, 0, 1);
            DesenhaRect(gunHeight * radius, gunWidth * radius, R, G, 0); 
        
        glPopMatrix();

        glPushMatrix();
        
            //Desenha corpo
            glScalef(1, bodyScale, 1);
            DesenhaCirc(radius, R, G, 0);
        
        glPopMatrix();

        glPushMatrix();
        
            //Desenha cabeca
            glScalef(headScale, headScale, 1);
            DesenhaCirc(radius, R, G, 0);
        
        glPopMatrix();
        
    glPopMatrix();

    GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
    elapsedTime = currentTime - lastTime;
    lastTime = currentTime;
}

void Player::RodaPlayer(int signal)
{
    gThetaPlayer += velocidade * signal * 5;
}

void Player::RodaArma(GLfloat inc)
{
    if(gThetaGun+inc <= 45 && gThetaGun+inc >= - 45){
        gThetaGun += inc;
    }
}

GLfloat Player::tryToMoveX(int signal)
{
    GLfloat xDelta = (velocidade * signal * cos((90 + gThetaPlayer) * M_PI/180.0));
    return gX + xDelta * elapsedTime;
}

GLfloat Player::tryToMoveY(int signal)
{
    GLfloat yDelta = (velocidade * signal * sin((90 + gThetaPlayer) * M_PI/180.0));
    return gY + yDelta * elapsedTime;
}

void Player::Move(int signal, bool canMove)
{
    if(!canMove){
        return;
    }

    GLfloat yDelta = (velocidade * signal * sin((90 + gThetaPlayer) * M_PI/180.0));
    GLfloat xDelta = (velocidade * signal * cos((90 + gThetaPlayer) * M_PI/180.0));
    gX += xDelta * elapsedTime;
    gY += yDelta * elapsedTime;
    gThetaLeg += velocidade * signal * .04 * elapsedTime;
}

void Player::Pula()
{
    if(!inJump){
        jumpInitTime = glutGet(GLUT_ELAPSED_TIME);
        inJump = true;
    }

}

void Player::Atira(){
    if(!above && !inJump){
        tiros.push_back(new Tiro(gX, gY, gThetaPlayer, gThetaGun, .8 * radius, .8 * gunWidth * radius, velocidadeTiro, inJumpScale, gunHeight * radius));
    }
}

GLfloat Player::ObtemX(){
    return gX;
}

GLfloat Player::ObtemY(){
    return gY;
}

GLfloat Player::ObtemRaio(){
    return radius;
}

list<Tiro*> Player::ObtemTiros(){
    return tiros;
}

void Player::RemoveTiro(Tiro* t){
    tiros.remove(t);
}

bool Player::EstaPulando(){
    return inJump;
}

void Player::DeterminaAcima(bool acima){
    above = acima;
}

bool Player::EstaAcima(){
    return above;
}