#include "glut.h"
#include <stdio.h>

#define DRAW_GRID false
#define WIDTH 40
#define HEIGHT 40
int WINDOW_W = 500;
int WINDOW_H = 500;

typedef struct pair {
	int x, y;
} Pair;

typedef struct pairf {
	float x, y;
} Pairf;

bool textura[HEIGHT][WIDTH];
Pair lastClick;
bool shift;
bool pressedLeft;

void initialize() {

	for (int i = 0; i < HEIGHT; i++)
		for(int j = 0; j < WIDTH; j++)
			textura[i][j] = 0;
	shift = 0;
	lastClick.x = 0;
	lastClick.y = 0;
	pressedLeft = 0;
}

Pairf convertCoord(int, int);

Pair getClick(float, float);

void display();

void mouse(int, int, int, int);

void mouseMove(int, int);

void keyboard(unsigned char, int, int);

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitWindowPosition(750, 200);

	glutCreateWindow("Textura");

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);

	gluOrtho2D(-1, 1, -1, 1);

	initialize();

	glutMainLoop();

	return 0;
}

void display() {

	glClearColor(1, 1, 1, 1);
	glClear( GL_COLOR_BUFFER_BIT );

	WINDOW_W = glutGet(GLUT_WINDOW_WIDTH);
	WINDOW_H = glutGet(GLUT_WINDOW_HEIGHT);

	glColor3f(0, 0, 0);

	if (DRAW_GRID) {
		glBegin(GL_LINES);
			for (int i = 1; i <= (WIDTH-1); i++) {
				glVertex2f(-1 + i*2./WIDTH, -1);
				glVertex2f(-1 + i*2./WIDTH, 1);
			}
			for (int i = 1; i <= (HEIGHT-1); i++) {
				glVertex2f(-1, -1 + i*2./HEIGHT);
				glVertex2f(1, -1 + i*2./HEIGHT);
			}
		glEnd();
	}

	glBegin(GL_QUADS);
		for(int i = 0; i < HEIGHT; i++)
			for(int j = 0; j < WIDTH; j++)
				if (textura[i][j]) {
					glVertex2f(-1 + j*2./WIDTH, 1 - i*2./HEIGHT);
					glVertex2f(-1 + (j+1)*2./WIDTH, 1 - i*2./HEIGHT);
					glVertex2f(-1 + (j+1)*2./WIDTH, 1 - (i+1)*2./HEIGHT);
					glVertex2f(-1 + j*2./WIDTH, 1 - (i+1)*2./HEIGHT);
				}
				
	glEnd();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {

	Pairf p = convertCoord(x, y);
	Pair click = getClick(p.x, p.y);
	//printf("%f : %f\n", p.x, p.y);

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
			lastClick = click;
		else
			if (lastClick.x == click.x && lastClick.y == click.y)
				if (click.x >= 0 && click.x < WIDTH && click.y >= 0 && click.y < HEIGHT)
					textura[click.y][click.x] = 1;
	} else {
		if (state == GLUT_DOWN)
			lastClick = click;
		else
			if (lastClick.x == click.x && lastClick.y == click.y)
				if (click.x >= 0 && click.x < WIDTH && click.y >= 0 && click.y < HEIGHT)
					textura[click.y][click.x] = 0;
	}

	if ((glutGetModifiers() & GLUT_ACTIVE_SHIFT) == GLUT_ACTIVE_SHIFT) {
		shift = 1;
		if (click.x >= 0 && click.x < WIDTH && click.y >= 0 && click.y < HEIGHT)
			if (button == GLUT_LEFT_BUTTON)
				textura[click.y][click.x] = 1;
			else
				textura[click.y][click.x] = 0;
	} else
		shift = 0;

	if (button == GLUT_LEFT_BUTTON)
		pressedLeft = 1;
	else
		pressedLeft = 0;

	glutPostRedisplay();
}

void mouseMove(int x, int y) {

	if (shift == 1) {
		Pairf p = convertCoord(x, y);
		Pair click = getClick(p.x, p.y);
		if (click.x >= 0 && click.x < WIDTH && click.y >= 0 && click.y < HEIGHT)
			if (pressedLeft == 1)
				textura[click.y][click.x] = 1;
			else
				textura[click.y][click.x] = 0;
		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {

	if (key == 13 /*ENTER*/) {

		FILE *fTexture;
		fTexture = fopen("Texture.bin", "wt");
		if (fTexture == NULL) {printf("Erro ao criar arquivo\n");	exit(1);}
		fprintf(fTexture, "%d %d\n", WIDTH, HEIGHT);
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++)
				fprintf(fTexture, "%d ", textura[i][j]);
			fprintf(fTexture, "\n");
		}
		fclose(fTexture);
		printf("Texture.bin foi criado com sucesso.\n\n");

	} else if (key == 27 /*ESC*/) {

		for (int i = 0; i < HEIGHT; i++)
			for(int j = 0; j < WIDTH; j++)
				textura[i][j] = 0;
		glutPostRedisplay();
		printf("Buffer Limpo.\n\n");
	}
}

Pairf convertCoord(int x, int y) {
	Pairf p;
	p.x = -1 + 2*((float) x / WINDOW_W);
	p.y = 1 - 2*((float) y / WINDOW_H);
	return p;
}

Pair getClick(float x, float y) {
	Pair p;
	p.x = (int) ((x+1)*WIDTH/2);
	p.y = (int) ((-y+1)*HEIGHT/2);
	return p;
}