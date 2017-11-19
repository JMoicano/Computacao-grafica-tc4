#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <list>

#include <GL/glut.h>

#include "Window.h"
#include "Circle.h"
#include "Player.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

Player *player;
Circle *arena[2];
list<Player*> enemies;
list<Circle*> lowObstacles;

int lastXMouse;
int keyFlags[256];

Window *window;

void readParams(char* fileName){
	XMLDocument file;

	string fileNameS = fileName;
	double velTiro, velJogador, velInimigo, velTiroInimigo, freqTiro, alturaObstaculo;

	if(fileNameS[fileNameS.size() - 1] != '/'){
		fileNameS + "/";
	}

	fileNameS.append("config.xml");

	string endereco;

	//Read XML file using tinyxml2
	if(file.LoadFile(fileNameS.c_str())){
		cout << "Erro ao ler arquivo XML" << endl;
	}else{
		XMLElement* rootTag = file.FirstChildElement();
		string rootValue;


		//Read parameters
		for(XMLElement* it = rootTag->FirstChildElement(); it != NULL; it = it->NextSiblingElement()){

			rootValue = it->Value();
			
			//Arena file params
			if(rootValue.compare("arquivoDaArena") == 0){

				endereco = it->Attribute("caminho");
				endereco += it->Attribute("nome");
				endereco += + ".";
				endereco += it->Attribute("tipo");

			}else if(rootValue.compare("jogador") == 0){

				velTiro = it->FloatAttribute("velTiro");
				velJogador = it->FloatAttribute("vel");

			}else if(rootValue.compare("inimigo") == 0){

				velTiroInimigo = it->FloatAttribute("velTiro");
				velInimigo = it->FloatAttribute("vel");
				freqTiro = it->FloatAttribute("freqTiro");

			}else if(rootValue.compare("obstaculo") == 0){

				alturaObstaculo = it->FloatAttribute("altura");

			}
		}

	}

	if(file.LoadFile(endereco.c_str())){
		cout << "Erro ao ler arquivo da arena" << endl;
	}else{
		XMLElement* rootTag = file.FirstChildElement();
		string rootValue;


		//Read circle parameters
		for(XMLElement* it = rootTag->FirstChildElement(); it != NULL; it = it->NextSiblingElement()){

			rootValue = it->Value();
			
			//Circle params
			if(rootValue.compare("circle") == 0){
				string cor = it->Attribute("fill");

				int x, y;
				double radius;

				x = it->IntAttribute("cx");
				y = it->IntAttribute("cy");
				radius = it->FloatAttribute("r");
				Circle *c = new Circle(radius, x, -y, 0);


				if(cor.compare("red") == 0){
					Player *e = new Player(x, -y, radius, velInimigo, velTiroInimigo, alturaObstaculo, freqTiro);
					c->setColor(1, 0, 0);
					enemies.push_back(e);
				}else if(cor.compare("black") == 0){
					c->setColor(0, 0, 0);
					lowObstacles.push_back(c);
				}else if(cor.compare("blue") == 0){
					window = new Window(2 * radius, 2 * radius);
					c->setColor(0, 0, 1);
					arena[0] = c;
				}else if(cor.compare("white") == 0){
					c->setColor(1, 1, 1);
					arena[1] = c;
				}else if(cor.compare("green") == 0){
					player = new Player(x, -y, radius, velJogador, velTiro, alturaObstaculo);
				}
			}
		}
	}
}

void initWindow(void)
{
	for (int i = 0; i < 256; ++i)
	{
		keyFlags[i] = 0;
	}
	// canMove = true;
	// canMoveAbove = true;
	 // select background color 
	glClearColor (1, 1, 1, 0.0);
	 // inicializar sistema de viz. 
	glMatrixMode(GL_PROJECTION);
	glOrtho(arena[0]->getCenterX() - arena[0]->getRadius(), arena[0]->getCenterX() + arena[0]->getRadius(), arena[0]->getCenterY() - arena[0]->getRadius(),arena[0]->getCenterY() + arena[0]->getRadius(), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
	glLoadIdentity();	
}

void keyPress(unsigned char key, int x, int y){
	keyFlags[key] = 1;
}

void keyUp(unsigned char key, int x, int y){
	keyFlags[key] = 0;
}

void passiveMotion(int x, int y){
	player->RodaArma((int)((lastXMouse - x) * player->ObtemRaio() / window->getWidth()) * 8);
	lastXMouse = x;
}
void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		player->Atira();
	}
}

void drawCircle(Circle *circle){
		 // Draw circle as polygon 
	glColor3f (circle->getColorR(), circle->getColorG(), circle->getColorB());
	glBegin(GL_POLYGON);
	glVertex3f(circle->getCenterX(), circle->getCenterY(), 0.0); // center of circle
	int resolution = 64;
	for(int i = 0; i <= resolution;i++) { 
		glVertex3f(
		circle->getCenterX() + (circle->getRadius() * cos(i * 2 * M_PI / resolution) * sin(M_PI / 2)), 
		circle->getCenterY() + (circle->getRadius() * sin(i * 2 * M_PI / resolution) * sin(M_PI / 2)),
		circle->getCenterZ() + (circle->getRadius() * cos(M_PI / 2))
		);
	}
	glEnd();

}

void display(void)
{
	 // Clear pixels 
	glClear (GL_COLOR_BUFFER_BIT);
	
	list <Tiro*> tiros = player->ObtemTiros();

	//Draw arena
	drawCircle(arena[0]);
	drawCircle(arena[1]);

	//Draw obstacles
	for (list<Circle*>::iterator iter = lowObstacles.begin(); iter != lowObstacles.end(); ++iter)
	{
		drawCircle((*iter));
	}
	
	//Draw enemies
	for (list<Player*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter)
	{
		(*iter)->Desenha();
		list <Tiro*> tirosE = (*iter)->ObtemTiros();
		for (list<Tiro*>::iterator i = tirosE.begin(); i != tirosE.end(); ++i)
		{
			(*i)->Desenha();
		}
	}

	//Draw shots
	for (list<Tiro*>::iterator iter = tiros.begin(); iter != tiros.end(); ++iter)
	{
		(*iter)->Desenha();
	}

	//Draw player
	player->Desenha();

	/* Dont Wait! */
	glutSwapBuffers();
}

double dist(Circle *c1, Circle *c2){
	double distance = sqrt(pow(c1->getCenterX() - c2->getCenterX(), 2) + pow(c1->getCenterY() - c2->getCenterY(), 2));
}

bool collisionTiro(Circle *c, Tiro* t, bool intern = false){
	double distance = sqrt(pow(c->getCenterX() - t->ObtemX(), 2) + pow(c->getCenterY() - t->ObtemY(), 2));
	return !(intern ? distance < c->getRadius() - t->ObtemRaio() : distance > c->getRadius() + t->ObtemRaio());
}

bool collisionTiro(Player *p, Tiro *t){
	double distance = sqrt(pow(p->ObtemX() - t->ObtemX(), 2) + pow(p->ObtemY() - t->ObtemY(), 2));
	return !(distance > p->ObtemRaio() + t->ObtemRaio());
}

void idle(void){

	int velocidade = 0;

	if(keyFlags[(int)('w')])
    {
        velocidade = 1;
    }
    if(keyFlags[(int)('s')])
    {
        velocidade = -1;
    }
	if(keyFlags[(int)('a')])
    {
        player->RodaPlayer(1);
    }
    if(keyFlags[(int)('d')])
    {
        player->RodaPlayer(-1);
    }
    if(keyFlags[(int)('p')])
    {
         player->Pula();
    }

	list<Tiro*> tiros = player->ObtemTiros();

	list<Tiro*> remove;

	list<Player*> morre;

	for (list<Tiro*>::iterator iterT = tiros.begin(); iterT != tiros.end(); ++iterT)
	{
		if(collisionTiro(arena[0], *iterT, true) || collisionTiro(arena[1], *iterT)){
			remove.push_back(*iterT);
		}
	}

	player->checkCollision(arena[0], velocidade, true);
	player->checkCollision(arena[1], velocidade);
	for (list<Player*>::iterator iter = enemies.begin(); iter != enemies.end(); ++iter)
	{
		for (list<Tiro*>::iterator iterT = tiros.begin(); iterT != tiros.end(); ++iterT){
			if(collisionTiro(*iter, *iterT)){
				remove.push_back(*iterT);
				morre.push_back(*iter);
			}
		}
		list<Tiro*> removeE;
		list<Tiro*> tirosE = (*iter)->ObtemTiros();
		for (list<Tiro*>::iterator iterT = tirosE.begin(); iterT != tirosE.end(); ++iterT){
			if(collisionTiro(arena[0], *iterT, true) || collisionTiro(arena[1], *iterT)){
				removeE.push_back((*iterT));
			}
			if(collisionTiro(player, *iterT)){
				removeE.push_back(*iterT);
			}
		}
		for (list<Circle*>::iterator iterO = lowObstacles.begin(); iterO != lowObstacles.end(); ++iterO)
		{
			for (list<Tiro*>::iterator iterT = tirosE.begin(); iterT != tirosE.end(); ++iterT){
				if(collisionTiro(*iterO, *iterT)){
					removeE.push_back(*iterT);
				}
			}
		}
		for (std::list<Player*>::iterator iterE = enemies.begin(); iterE != enemies.end(); ++iterE)
		{
			if(iter != iterE){
				(*iterE)->checkCollision(*iter, 1);
				(*iter)->checkCollision(*iterE, 1);
			}
		}
		(*iter)->checkCollision(arena[0], 1, true);
		(*iter)->checkCollision(arena[1], 1);
		(*iter)->checkCollision(player, 1);
		player->checkCollision((*iter), velocidade);
		for (list<Tiro*>::iterator i = removeE.begin(); i != removeE.end(); ++i)
		{
			(*iter)->RemoveTiro((*i));
		}
	}
	 int i = 0;
	for (list<Circle*>::iterator iter = lowObstacles.begin(); iter != lowObstacles.end(); ++iter, ++i)
	{
		for (list<Tiro*>::iterator iterT = tiros.begin(); iterT != tiros.end(); ++iterT){
			if(collisionTiro((*iter), (*iterT))){
				remove.push_back((*iterT));
			}
		}
		for (list<Player*>::iterator iterE = enemies.begin(); iterE != enemies.end(); ++iterE){
			(*iterE)->checkCollisionJumpable((*iter), 1, i);
		}

		player->checkCollisionJumpable((*iter), velocidade, i);
	}

	for (list<Tiro*>::iterator i = remove.begin(); i != remove.end(); ++i)
	{
		player->RemoveTiro((*i));
	}
	for (list<Player*>::iterator i = morre.begin(); i != morre.end(); ++i)
	{
		enemies.remove((*i));
	}
	for (list<Player*>::iterator iterE = enemies.begin(); iterE != enemies.end(); ++iterE){
		(*iterE)->anima();
	}

    player->Move(velocidade);
	
	glutPostRedisplay();
}

int main(int argc, char** argv){
	readParams(argv[1]);
	argc--;
	argv = &argv[1];
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE |	GLUT_RGB);
	glutInitWindowSize (window->getWidth(), window->getHeight());
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("trabalhocg");
	initWindow();
	glutPassiveMotionFunc(passiveMotion);
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}