#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include<vector>
#include <iostream>
using namespace std;

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
GLuint xet;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 10000;

double robotx = 0;
double robotz = 0;
double roboty = 3.5;
bool push = true;
bool gameOn = true;
int px = 0;
int pz= 0;
int inv = 1;
bool firstPerson;
vector<double>p = {};
vector<double>coins = {};
int coinsx = 0;
int coinsz = 0;
int jumpUp = 0;
int jumpDown = 0;
bool jump = false;
vector<double>cars = {};
int carsx = 0;
int carsz = 0;
bool coinsbool[5] = { true,true,true,true,true };
bool coin1 = true;
bool coin2 = true;
bool coin3 = true;
bool coin4 = true;
bool coin5 = true;
vector<double>magic = {};
int magicx = 0;
int magicz = 0;

int flyUp = 0;
int flyDown = 0;
int flyStill = 0;
bool fly = false;

double deathroll = 0;
double rotateDeg=0;

double lightElevation = 30;
bool lightUp = false;

double redd = 1;
double greenn = 1;
double bluee = 1;

double text = -50;

int score = 0;
int gameSpeed = 5;

vector<double>presents = {};
double presentsx = 0;
double presentsz = 0;
bool present1 = true;
bool present2 = true;
bool present3 = true;
bool present4 = true;
bool present5 = true;

bool magic1 = true;
bool magic2 = true;


class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0, 13, 30);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_car;
Model_3DS model_moto;
Model_3DS model_robot;
Model_3DS model_coin;
Model_3DS model_cactus;
Model_3DS model_magic;
Model_3DS model_sign;


// Textures
GLTexture tex_ground;
GLTexture xet_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { redd,greenn , bluee, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { redd, greenn, bluee, 1 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, lightElevation, robotz, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat dir[] = { 0,-1,0 };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
}

void print(int x, int y,int z, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y,z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0, 0, -800);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, 70);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, 70);
	glTexCoord2f(1, 15);
	glVertex3f(20, 0, 900);
	glTexCoord2f(0, 15);
	glVertex3f(-20, 0, 900);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderGround2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, xet_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0, 0, -800);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-25, 0, -1100);
	glTexCoord2f(1, 0);
	glVertex3f(25, 0, -1100);
	glTexCoord2f(1, 15);
	glVertex3f(25, 0, 70);
	glTexCoord2f(0, 15);
	glVertex3f(-25, 0, 70);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}



void LoadAssets()
{
	// Loading Model files
	model_moto.Load("Models/train/tr.3ds");
	model_car.Load("Models/car/car.3ds");
	model_robot.Load("Models/robot/robot.3ds");
	//model_coin.Load("Models/coin/coin.3ds");
	model_cactus.Load("Models/cactus/cactus.3ds");
	model_magic.Load("Models/magic/magic.3ds");
	model_sign.Load("Models/sign/sign.3ds");


	// Loading texture files
	tex_ground.Load("Textures/sand2.bmp");
	xet_ground.Load("Textures/road2.bmp");

		loadBMP(&tex, "Textures/desert.bmp", true);
	
	
		loadBMP(&xet, "Textures/blu-sky-3.bmp", true);
	
}

void time(int) {
	if (robotz < -719) {
		gameSpeed = 10;
	}
	if (lightUp ) {
		lightElevation+=3;
		if (lightElevation == 30)
			lightUp = false;
	}
	if (lightUp == false) {
		lightElevation-=3;
		if (lightElevation == 9)
			lightUp = true;
	}
	
	if (gameOn == false) {
		deathroll = 90;
		greenn = 0;
		bluee = 0;
		InitLightSource();
		glLoadIdentity();
		
	}

	if(gameOn){
	robotz = robotz - gameSpeed;
	text = text - gameSpeed;
	}

	if (rotateDeg == 360) {
		rotateDeg = 0;
	}
	rotateDeg += 3;
	if (jumpUp > 0) {
		roboty += 2;
		if (firstPerson) {
			At.y += 2;
			Eye.y += 2;
		}
		jumpUp -= 2;
		if (jumpUp == 0) {
			jumpDown = 6;
			
		}

	}
	if (jumpDown > 0) {
		roboty -= 2;
		if (firstPerson) {
			At.y -= 2;
			Eye.y -= 2;
		}
		jumpDown -= 2;
		if (jumpDown == 0)
			jump = false;
	}
	if (flyUp > 0) {
		roboty += 2;
		if (firstPerson) {
			At.y += 2;
			Eye.y += 2;
		}
		flyUp -= 2;
		if (flyUp == 0)
			flyStill = 60;
	}
	if (flyStill > 0) {
		flyStill -= 2;
		if (flyStill == 0)
			flyDown = 8;
	}
	if (flyDown > 0) {
		roboty -= 2;
		if (firstPerson) {
			At.y -= 2;
			Eye.y -= 2;
		}
		flyDown -= 2;
		if (flyDown == 0)
			fly = false;
	}

	if (gameOn) {
		Eye.z -= gameSpeed;
	    At.z -= gameSpeed;
	}
	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	
		
	for (int i = 0; i < p.size(); i = i + 2) {
		px = p.at(i);
		pz= p.at(i + 1);
		if (px == robotx || (px == 1 && robotx == 0)) {
			if ((robotz - pz)<8.5 && (robotz - pz)>-5) {
				if (fly == false) {
					gameOn = false;
					robotz += 5;
					roboty -= 2;
				}
			}

	}
	}

	for (int i = 0; i < cars.size(); i = i + 2) {
		carsx = cars.at(i);
		carsz = cars.at(i + 1);
		if (carsx == robotx || (carsx == 12 && robotx == 10) || (carsx==-12 && robotx==-10)) {
			if ((robotz - carsz) < 16 && (robotz - carsz) > -8) {
				if (fly == false) {
					gameOn = false;
					robotz += 5;
					roboty -= 2;
				}
			}

		}
	}

	for (int i = 0; i < magic.size(); i = i + 2) {
		magicx = magic.at(i);
		magicz = magic.at(i + 1);
		if (magicx == robotx || (magicx == 1 && robotx == 0)) {
			if ((robotz - magicz) < 1.5 && (robotz - magicz) > -2) {
				fly = true;
				flyUp = 8;
				if (i == 0)
					magic1 = false;
				else if (i == 2)
					magic2 = false;
			}

		}
	}

	
	for (int i = 0; i < presents.size(); i = i + 2) {
		presentsx = presents.at(i);
		presentsz = presents.at(i + 1);
		if (presentsx == robotx || (presentsx == 1 && robotx == 0)) {
			if ((robotz - presentsz) < 5 && (robotz - presentsz) > -3) {
				int z = i / 2;
				score++;
				if (i == 0)
					present1 = false;
				else if (i == 2) {
					present2 = false;
				}
				else if (i == 4)
					present3 = false;
				else if (i == 6)
					present4 = false;
				else if (i == 8)
					present5 = false;

				if (jumpUp == 0 && jumpDown <= 0) {
					jumpUp = 6;
					jump = true;
				}
			}

		}
	}

	for (int i = 0; i < coins.size(); i = i + 2) {
		coinsx = coins.at(i);
		coinsz = coins.at(i + 1);
		if (coinsx == robotx || (coinsx == 1 && robotx == 0)) {
			if (i < 10) {
				if ((robotz - coinsz) < 1 && (robotz - coinsz) > -1) {
					int z = i / 2;
					score++;
					if (i == 0)
						coin1 = false;
					else if (i == 2) {
						coin2 = false;
					}
					else if (i == 4)
						coin3 = false;
					else if (i == 6)
						coin4 = false;
					else if (i == 8)
						coin5 = false;

					if (jumpUp == 0 && jumpDown <= 0) {
						jumpUp = 6;
						jump = true;
					}
				}
			}
			else {
				if (i > 9) {
					
					if ((robotz - coinsz) < 5 && (robotz - coinsz) > -3) {
						
						int z = i / 2;
						score+=2;
						if (i == 10)
							present1 = false;
						else if (i == 12) {
							present2 = false;
						}
						else if (i == 14)
							present3 = false;
						else if (i == 16)
							present4 = false;
						else if (i == 18)
							present5 = false;

						if (jumpUp == 0 && jumpDown <= 0) {
							jumpUp = 6;
							jump = true;
						}
					}
			}

		}
	}

	
}
	glutPostRedisplay();
	glutTimerFunc(100, time, 0);
}
void drawPresents(int x, int z) {
	
	glPushMatrix();
	
	glTranslated(x, 0, z);
	glRotated(rotateDeg, 0, 1, 0);
	glScaled(40, 40, 40);
	//if (presentsAnim) {
		//glRotated(rotateDeg, 0, 1, 0);
	//}

	glPushMatrix();

	glColor3f(1, 1, 0);
	glutSolidCube(0.15);
	glPopMatrix();
	glColor3f(0, 0, 1);

	glPushMatrix();
	glTranslated(0, 0.06, 0);
	glScaled(1, 0.21, 0.2);
	glutSolidCube(0.15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.06, 0);
	glScaled(0.2, 0.21, 1);
	glutSolidCube(0.15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 0.06);
	glScaled(0.2, 1, 0.21);
	glutSolidCube(0.15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -0.06);
	glScaled(0.2, 1, 0.21);
	glutSolidCube(0.15);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.082, 0);
	glutSolidSphere(0.015, 15, 15);
	glPopMatrix();
	glPopMatrix();
}
void drawCoin(double x, double z) {

	
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	glPushMatrix();
	glColor3f(1, 0.9, 0);
	glTranslated(x, 3, z);
	glRotated(rotateDeg, 0, 1, 0);
	gluCylinder(qobj, 3, 0.01, 0.01, 80, 80);
	glPopMatrix();

}

void drawMagic(double x, double z) {
	glPushMatrix();
	glTranslated(x, 0, z);
	glScaled(0.4, 0.4, 0.4);
	model_magic.Draw();
		glPopMatrix();
}

void drawMoto(int x, int z) {
	glPushMatrix();
	glTranslated(x, 2.5, z);
	glScaled(0.05, 0.03, 0.04);
	model_moto.Draw();
	glPopMatrix();

}
void drawSign(int x, int z) {
	glPushMatrix();
	glTranslated(x, 1.5, z);
	glScaled(0.1, 0.1, 0.1);
	model_sign.Draw();
	glPopMatrix();

}

void drawCar(int x, int z) {
	glPushMatrix();
	glTranslated(x, 3, z);
	glScaled(0.4, 0.4, 0.4);
	model_car.Draw();
	glPopMatrix();

}
void drawCactus(int x, int z) {
	glPushMatrix();
	glTranslated(x,1, z);
	glScaled(0.1, 0.08, 0.1);
	model_cactus.Draw();
	glPopMatrix();

}
//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InitLightSource();


	/*GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);*/

	// Draw Ground
	RenderGround();
	RenderGround2();

	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(robotx, roboty, robotz);
	//glutSolidSphere(4, 50, 50);
	glPopMatrix();

	//Draw score
	glColor3f(0, 0, 0);
	char* p0s[20];
	sprintf((char*)p0s, "Score: %d", score);
	print(-20, 8,text, (char*)p0s);
	glColor3f(1, 1, 1);

	

	// Draw magic
	if(magic1)
	drawMagic(1,-420 );
	if (magic2)
		drawMagic(1, -1200);
	if (push) {
		magic.push_back(1);
		magic.push_back(-420);
		magic.push_back(1);
		magic.push_back(-1200);
	}

	
	glColor3f(1, 1, 1);
	// Draw motos Model
	drawMoto(10, -100);
	drawMoto(1, -200);
	drawMoto(10, -300);
	drawMoto(-10, -300);
	drawMoto(1, -400);
	drawMoto(1, -500);
	drawMoto(10, -500);
	drawMoto(-10, -600);
	drawMoto(1, -700);
	drawMoto(-10, -700);
	if (push) {
		p.push_back(10);
		p.push_back(-100);
		p.push_back(1);
		p.push_back(-200);
		p.push_back(10);
		p.push_back(-300);
		p.push_back(-10);
		p.push_back(-300);
		p.push_back(1);
		p.push_back(-400);
		p.push_back(1);
		p.push_back(-500);
		p.push_back(10);
		p.push_back(-500);
		p.push_back(-10);
		p.push_back(-600);
		p.push_back(1);
		p.push_back(-700);
		p.push_back(-10);
		p.push_back(-700);
		
	}
	if(coin1)
	drawCoin(1, -50);
	if (coin2)
		drawCoin(10, -150);
	if (coin3)
		drawCoin(-10, -250);
	if (coin4)
		drawCoin(1, -350);
	if (coin5)
		drawCoin(10, -650);

	if (push) {
		coins.push_back(1);
		coins.push_back(-50);
		coins.push_back(10);
		coins.push_back(-150);
		coins.push_back(-10);
		coins.push_back(-250);
		coins.push_back(1);
		coins.push_back(-350);
		coins.push_back(10);
		coins.push_back(-650);
	}
	glColor3f(1, 1, 1);
	// draw presents
	if (present1)
		drawPresents(-10, -950);
	if (present2)
		drawPresents(10, -1050);
	if (present3)
		drawPresents(-10, -1150);
	if (present4)
		drawPresents(1, -1550);
	if (present5)
		drawPresents(10, -1650);
	glColor3f(1, 1, 1);
	if (push) {
		coins.push_back(-10);
		coins.push_back(-950);
		coins.push_back(10);
		coins.push_back(-1050);
		coins.push_back(-10);
		coins.push_back(-1150);
		coins.push_back(1);
		coins.push_back(-1550);
		coins.push_back(10);
		coins.push_back(-1650);
		
		
	}

	
	//draw Cars
	drawCar(0, -900);
	drawCar(0, -1000);
	drawCar(12, -1000);
	drawCar(-12, -1100);
	drawCar(-12, -1200);
	drawCar(12, -1200);
	drawCar(0, -1300);
	drawCar(0, -1400);
	drawCar(-12, -1400);
	drawCar(12, -1500);
	drawCar(0, -1500);
	drawCar(-12, -1600);
	drawCar(12, -1700);
	drawCar(-12, -1700);
	if (push) {
		cars.push_back(0);
		cars.push_back(-900);
		cars.push_back(0);
		cars.push_back(-1000);
		cars.push_back(12);
		cars.push_back(-1000);
		cars.push_back(-12);
		cars.push_back(-1100);
		cars.push_back(-12);
		cars.push_back(-1200);
		cars.push_back(12);
		cars.push_back(-1200);
		cars.push_back(0);
		cars.push_back(-1300);
		cars.push_back(0);
		cars.push_back(-1400);
		cars.push_back(-12);
		cars.push_back(-1400);
		cars.push_back(12);
		cars.push_back(-1500);
		cars.push_back(0);
		cars.push_back(-1500);
		cars.push_back(-12);
		cars.push_back(-1600);
		cars.push_back(12);
		cars.push_back(-1700);
		cars.push_back(-12);
		cars.push_back(-1700);
	}

	//draw cactus
	drawCactus(15, -150);
	drawCactus(-15, -250);
	drawCactus(15, -350);
	drawCactus(-15, -350);
	drawCactus(15, -450);
	drawCactus(-15, -550);
	drawCactus(15, -650);
	drawCactus(-15, -700);
	//draw signs
	drawSign(20, -750);
	drawSign(-20, -850);
	drawSign(20, -950);
	drawSign(-20, -1050);
	drawSign(20, -1150);
	drawSign(-20, -1250);
	drawSign(-20, -1350);
	drawSign(20, -1450);
	drawSign(-20, -1500);
	drawSign(20, -1600);
	drawSign(-20, -1700);


	

	glPushMatrix();
	
	//glColor3f(0.9,1,0);
	glTranslated(robotx,roboty, robotz);
	glScaled(0.15, 0.15, 0.15);
	//glScaled(0.05, 0.05, 0.05);
	glRotated(deathroll, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	model_robot.Draw();
	glPopMatrix();


	//sky box
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);

	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 720, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	glPushMatrix();

	//GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, -500);
	glRotated(90, 1, 0, 1);

	glBindTexture(GL_TEXTURE_2D, xet);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 1300, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();

	push = false;

	glutSwapBuffers();
}



//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'a':
		if (robotx == 0 || robotx == 10) {
			robotx -= 10;
		
				Eye.x -= 10;
				At.x -= 10;
			
		}
		
		break;
	case 'd':
		if (robotx == 0 || robotx == -10) {
			robotx += 10;
		
				
				Eye.x += 10;
				At.x += 10;
			
		}
		break;
	case 'f':
		if (firstPerson == false) {
			Eye.y -= 10;
			Eye.z -= 35;
			At.z -= 15;
			firstPerson = true;
			
		}
		else {
			Eye.y += 10;
			Eye.z += 35;
			At.z += 15;
			firstPerson = false;
		
		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}
void actM(int button, int state, int x, int y)//mouse function takes 4 parameters: button: which button has been clicked (GLUT_RIGHT_BUTTON or GLUT_LEFT_BUTTON),
											//state wether the button is clicked or released (GLUT_UP or GLUT_DOWN)
											// x and y are the position of the mouse cursor
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//if the left button has been clicked then translate the square to the mouse position
	{
		if (robotx == 0 || robotx == 10) {
			robotx -= 10;

			Eye.x -= 10;
			At.x -= 10;

		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//if the right button has been clicked, translate  the square to the origin (0,0)
	{
		if (robotx == 0 || robotx == -10) {
			robotx += 10;


			Eye.x += 10;
			At.x += 10;

		}
	}

	glutPostRedisplay();//redisplay to update the screen with the new paraeters
}
//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================


//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);
	glutMouseFunc(actM);			//call the mouse function


	glutReshapeFunc(myReshape);

	myInit();

	glutTimerFunc(0, time, 0);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}