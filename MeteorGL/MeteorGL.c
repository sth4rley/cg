#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

typedef struct
{
    GLfloat posX;
    GLfloat posY;
    GLfloat angulo;
    GLfloat velocidade;
    GLfloat tamanho;
    int sentido;
} Meteoro;

#define MAX_METEOROS 15

enum GameState
{
    MENU,
    PLAYING,
    GAMEOVER
};

enum GameState gameState = MENU; // Estado inicial do jogo é o menu

GLfloat largura = 1024.0f;
GLfloat altura = 768.0f;

Meteoro meteoros[MAX_METEOROS];

// Posição e ângulo do foguete do jogador
GLfloat posicaoFoguetePlayerX = 0.0f;
GLfloat posicaoFoguetePlayerY = 0.0f;
GLfloat anguloFoguetePlayer = 0.0f;

// pontos
int pontos = 0;

int keyStates[256];
int specialKeyStates[256];

void displayText(float x, float y, int r, int g, int b, const char *string)
{
    int j = strlen(string);
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (int i = 0; i < j; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
    }
}

void drawMenu()
{
    // Define as propriedades do texto do menu
    float menuTextPosX = -50.0f;
    float menuTextPosY = 50.0f;
    int menuTextR = 255;
    int menuTextG = 255;
    int menuTextB = 255;

    // Desenha o título do menu
    displayText(menuTextPosX, menuTextPosY, menuTextR, menuTextG, menuTextB, "Meu Jogo");

    // Desenha as opções do menu
    menuTextPosY -= 30.0f;
    displayText(menuTextPosX, menuTextPosY, menuTextR, menuTextG, menuTextB, "1 - Jogar");
    menuTextPosY -= 20.0f;
    displayText(menuTextPosX, menuTextPosY, menuTextR, menuTextG, menuTextB, "2 - Sair");
}

void drawGameOver(){
    // Define as propriedades do texto do menu
    float menuTextPosX = -50.0f;
    float menuTextPosY = 50.0f;
    int menuTextR = 255;
    int menuTextG = 255;
    int menuTextB = 255;

    // Desenha o título do menu
    displayText(menuTextPosX, menuTextPosY, menuTextR, menuTextG, menuTextB, "Game Over");

    // Desenha as opções do menu
    menuTextPosY -= 30.0f;
    displayText(menuTextPosX, menuTextPosY, menuTextR, menuTextG, menuTextB, "1 - Jogar");
    menuTextPosY -= 20.0f;
    displayText(menuTextPosX, menuTextPosY, menuTextR, menuTextG, menuTextB, "2 - Sair");
}

void initKeyboard()
{
    memset(keyStates, 0, sizeof(keyStates));
    memset(specialKeyStates, 0, sizeof(specialKeyStates));
}

void inicializarMeteoros()
{
    for (int i = 0; i < MAX_METEOROS; i++)
    {
        meteoros[i].posX = rand() % (int)(largura * 2) - largura;
        meteoros[i].posY = altura;
        meteoros[i].velocidade = (rand() % 10 + 1) / 100.0f;
        meteoros[i].angulo = rand() % 360;
        meteoros[i].tamanho = rand() % 5 + 1;
        meteoros[i].sentido = rand() % 2;
    }
}

// desenha o placar de pontos do jogo na tela
// os pontos sao contabilizados a cada meteoro que passa pela tela
void desenhaPlacar()
{
    char texto[30];
    sprintf(texto, "Pontos: %d", pontos);
    displayText(-largura + 10, altura - 20, 255, 255, 255, texto);
}

void desenhaFoguetePlayer()
{
    // Desenha um foguete com polígonos utilizando transformações geométricas e hierarquia

    // Corpo do foguete
    glPushMatrix();
    glColor3f(0.7f, 0.7f, 0.7f);
    glTranslatef(posicaoFoguetePlayerX, posicaoFoguetePlayerY, 0.0f);
    glRotatef(anguloFoguetePlayer, 0.0f, 0.0f, 1.0f);
    glScalef(0.6f, 0.6f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-2.0f, -15.0f);
    glVertex2f(2.0f, -15.0f);
    glVertex2f(2.0f, 2.0f);
    glVertex2f(-2.0f, 2.0f);
    glEnd();

    // Cabine
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 5.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 3.0f);
    glVertex2f(-1.0f, 3.0f);
    glEnd();
    glPopMatrix();

    // Asa esquerda
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(-2.0f, -12.5f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 7.0f);
    glVertex2f(-6.0f, 0.0f);
    glEnd();
    glPopMatrix();

    // Asa direita
    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(2.0f, -12.5f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 7.0f);
    glVertex2f(6.0f, 0.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    if (gameState == MENU)
    {
        drawMenu();
    }
    else if (gameState == PLAYING)
    {

        desenhaFoguetePlayer();

        for (int i = 0; i < MAX_METEOROS; i++)
        {
            glPushMatrix();
            glTranslatef(meteoros[i].posX, meteoros[i].posY, 0.0f);
            glRotatef(meteoros[i].angulo, 0.0f, 0.0f, 1.0f);
            glColor3f(0.2f, 0.2f, 0.2f);
            glBegin(GL_POLYGON);
            glVertex2f(-meteoros[i].tamanho, -meteoros[i].tamanho);
            glVertex2f(meteoros[i].tamanho, -meteoros[i].tamanho);
            glVertex2f(meteoros[i].tamanho, meteoros[i].tamanho);
            glVertex2f(-meteoros[i].tamanho, meteoros[i].tamanho);
            glEnd();
            glPopMatrix();
        }
    desenhaPlacar();
    } else if (gameState == GAMEOVER) {
        drawGameOver();
    }

    glFlush();
}

void colisoesBordaJanela()
{
    if (posicaoFoguetePlayerX > largura)
        posicaoFoguetePlayerX = -largura;
    if (posicaoFoguetePlayerX < -largura)
        posicaoFoguetePlayerX = largura;
    if (posicaoFoguetePlayerY > altura)
        posicaoFoguetePlayerY = -altura;
    if (posicaoFoguetePlayerY < -altura)
        posicaoFoguetePlayerY = altura;
}

void colisoesMeteoros()
{
    for (int i = 0; i < MAX_METEOROS; i++)
    {
        if (posicaoFoguetePlayerX > meteoros[i].posX - meteoros[i].tamanho &&
            posicaoFoguetePlayerX < meteoros[i].posX + meteoros[i].tamanho &&
            posicaoFoguetePlayerY > meteoros[i].posY - meteoros[i].tamanho &&
            posicaoFoguetePlayerY < meteoros[i].posY + meteoros[i].tamanho)
        {
            // reinicia o jogo se houver colisão
            // gameState = MENU;
            gameState = GAMEOVER;
            posicaoFoguetePlayerX = 0.0f;
            posicaoFoguetePlayerY = 0.0f;
            anguloFoguetePlayer = 0.0f;
            pontos = 0;
        }
    }
}

void comandosMenu()
{
    if (keyStates['1'] == 1)
    {
        gameState = PLAYING;
    }

    if (keyStates['2'] == 1)
    {
        exit(0);
    }
}

void comandosJogo()
{

    GLfloat velocidade = largura * 0.0004f;
    if (keyStates['w'] == 1)
    {
        GLfloat movimentoX = -velocidade * sin(anguloFoguetePlayer * M_PI / 180.0f);
        GLfloat movimentoY = velocidade * cos(anguloFoguetePlayer * M_PI / 180.0f);

        posicaoFoguetePlayerX += movimentoX;
        posicaoFoguetePlayerY += movimentoY;
    }
    if (keyStates['a'] == 1)
    {
        anguloFoguetePlayer += 0.3f;
    }
    if (keyStates['d'] == 1)
    {
        anguloFoguetePlayer -= 0.3f;
    }
    // volta pra o menu com a tecla ESC
    if (keyStates[27] == 1)
    {
        gameState = MENU;
    }
}

// funcao que faz os meteoros se movimentarem
void movimentaMeteoros()
{
    // movimenta os meteoros
    for (int i = 0; i < MAX_METEOROS; i++)
    {
        meteoros[i].posY -= meteoros[i].velocidade;

        if (meteoros[i].posY < -altura)
        {
            meteoros[i].posX = rand() % (int)(largura * 2) - largura;
            meteoros[i].posY = altura;
            meteoros[i].velocidade = (rand() % 10 + 1) / 100.0f;
            meteoros[i].tamanho = rand() % 5 + 1;

            pontos++; // incrementa os pontos quando o meteoro passa
        }

        meteoros[i].angulo += meteoros[i].velocidade * (meteoros[i].sentido == 0 ? -5.0f : 5.0f);
    }
}

void idle()
{
    if (gameState == PLAYING)
    {
        comandosJogo();
        movimentaMeteoros();
        colisoesMeteoros();
        colisoesBordaJanela();
      
    }
    else if (gameState == MENU)
    {
        comandosMenu();
    }
    else if (gameState == GAMEOVER)
    {
        comandosMenu();
    }
    glutPostRedisplay();
}

void reshape(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)w / h;
    const float scale = 100.0f;

    if (w <= h)
    {
        largura = scale;
        altura = scale / aspectRatio;
    }
    else
    {
        largura = scale * aspectRatio;
        altura = scale;
    }

    gluOrtho2D(-largura, largura, -altura, altura);
}

void keyboard(unsigned char key, int x, int y) { keyStates[key] = 1; }

void keyboardUp(unsigned char key, int x, int y) { keyStates[key] = 0; }

void specialKeys(int key, int x, int y) { specialKeyStates[key] = 1; }

void specialKeysUp(int key, int x, int y) { specialKeyStates[key] = 0; }

void init()
{
    glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
    initKeyboard();
    inicializarMeteoros();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(1024, 768);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("MeteorGL");

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);

    init();

    glutMainLoop();
    return 0;
}
