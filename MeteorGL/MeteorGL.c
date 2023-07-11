// Para compilar 
// g++ MeteorGL.c -lglut -lGLU -lGL
// ou
// gcc MeteorGL.c -lglut -lGLU -lGL

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
    int sentido; // 0 = esquerda, 1 = direita
} Meteoro;

enum EstadoJogo 
{
    MENU,
    JOGANDO,
    GAMEOVER
};

#define MAX_METEOROS 50

enum EstadoJogo estadoJogo = MENU; // Estado inicial do jogo é o menu

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

// funcoes de entrada
void keyboard(unsigned char key, int x, int y) { keyStates[key] = 1; }
void keyboardUp(unsigned char key, int x, int y) { keyStates[key] = 0; }
void specialKeys(int key, int x, int y) { specialKeyStates[key] = 1; }
void specialKeysUp(int key, int x, int y) { specialKeyStates[key] = 0; }
void initKeyboard()
{
    memset(keyStates, 0, sizeof(keyStates));
    memset(specialKeyStates, 0, sizeof(specialKeyStates));
}

void displayText(float x, float y, int r, int g, int b, const char *string) // funcao para escrever texto na tela
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y); // Posição do texto na tela
    for (int i = 0; i < strlen(string); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
}

void desenhaMenu() // Desenha o menu
{
    float menuTextPosX = -50.0f;
    float menuTextPosY = 50.0f;

    // título do menu      
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, "MeteorGL");

    // opções do menu
    menuTextPosY -= 30.0f;
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, "1 - Jogar");
    menuTextPosY -= 20.0f;
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, "2 - Sair");
}

void desenhaGameOver() // Desenha a tela de game over
{
    float menuTextPosX = -50.0f;
    float menuTextPosY = 50.0f;

    // Desenha o título do menu
    // Ajusta o tamanho do texto do título
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, "Você perdeu!");

    // escreve a pontuação do jogador
    menuTextPosY -= 30.0f;
    char texto[30];
    sprintf(texto, "Pontos: %d", pontos);
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, texto);

    // opções do menu
    menuTextPosY -= 30.0f;
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, "1 - Jogar novamente");
    menuTextPosY -= 20.0f;
    displayText(menuTextPosX, menuTextPosY, 255, 255, 255, "2 - Sair");
}

void desenhaPlacar() // Desenha o placar na tela
{
    char texto[30];
    sprintf(texto, "Pontos: %d", pontos);
    displayText(-largura + 10, altura - 20, 255, 255, 255, texto);
}

void desenhaFoguetePlayer()
{
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

/* Funcoes que tratam dos meteoros */

void inicializarMeteoros() // Inicializa os com valores aleatórios
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

void desenhaMeteoros()  // Desenha os meteoros na tela
{
  for (int i = 0; i < MAX_METEOROS; i++)
  {
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(meteoros[i].posX, meteoros[i].posY, 0.0f);
    glRotatef(meteoros[i].angulo, 0.0f, 0.0f, 1.0f);

    glBegin(GL_POLYGON);
      glVertex2f(-meteoros[i].tamanho, -meteoros[i].tamanho);
      glVertex2f(meteoros[i].tamanho, -meteoros[i].tamanho);
      glVertex2f(meteoros[i].tamanho, meteoros[i].tamanho);
      glVertex2f(-meteoros[i].tamanho, meteoros[i].tamanho);
    glEnd();

    glPopMatrix();
  }
}

void movimentaMeteoros() // Movimenta os meteoros na tela
{
    // movimenta os meteoros
    for (int i = 0; i < MAX_METEOROS; i++)
    {
        meteoros[i].posY -= meteoros[i].velocidade;

        if (meteoros[i].posY < -altura) // quando o meteoro passa da tela, ele é reposicionado 
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

void colisoesMeteoros() // Verifica se houve colisão entre o foguete e os meteoros
{
    for (int i = 0; i < MAX_METEOROS; i++)
    {
        if (posicaoFoguetePlayerX > meteoros[i].posX - meteoros[i].tamanho &&
            posicaoFoguetePlayerX < meteoros[i].posX + meteoros[i].tamanho &&
            posicaoFoguetePlayerY > meteoros[i].posY - meteoros[i].tamanho &&
            posicaoFoguetePlayerY < meteoros[i].posY + meteoros[i].tamanho)
        {
            estadoJogo = GAMEOVER; 
        }
    }
}

void colisoesBordaJanela() // trata a colisão do foguete com as bordas da janela
{
    if (posicaoFoguetePlayerX > largura) posicaoFoguetePlayerX = -largura;
    if (posicaoFoguetePlayerX < -largura) posicaoFoguetePlayerX = largura;
    if (posicaoFoguetePlayerY > altura) posicaoFoguetePlayerY = -altura;
    if (posicaoFoguetePlayerY < -altura) posicaoFoguetePlayerY = altura;
}

/* Funcoes que tratam dos comandos do jogo */

void comandosMenu()
{
    if (keyStates['1'] == 1) estadoJogo = JOGANDO; 
    if (keyStates['2'] == 1) exit(0);
}

void comandosGameOver()
{
    if (keyStates['1'] == 1)
    {
      posicaoFoguetePlayerX = posicaoFoguetePlayerY = anguloFoguetePlayer = 0.0f;
      pontos = 0;
      estadoJogo = JOGANDO;
      inicializarMeteoros();
    }

    if (keyStates['2'] == 1) exit(0);
    
}

void comandosJogo() // trata os comandos do jogo
{
    GLfloat velocidade = largura * 0.0004f;
    if (keyStates['w'] == 1)
    {
        // calcula o movimento do foguete baseado no angulo que ele esta virado 
        // 3.1416 / 180.0f = 1 grau
        posicaoFoguetePlayerX += (-velocidade * sin(anguloFoguetePlayer * 3.1416 / 180.0f)); // sen -> componente x
        posicaoFoguetePlayerY += (+velocidade * cos(anguloFoguetePlayer * 3.1416 / 180.0f)); // cos -> componente y 
    }
    if (keyStates['a'] == 1) anguloFoguetePlayer += 0.3f; 
    if (keyStates['d'] == 1) anguloFoguetePlayer -= 0.3f; 
    if (keyStates[27] == 1) estadoJogo = MENU; // ESC 
}

void reshape(GLsizei w, GLsizei h) // trata o redimensionamento da janela
{
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)w / h;
    const float scale = 100.0f;

    if (w <= h) {
        largura = scale;
        altura = scale / aspectRatio;
    }
    else {
        largura = scale * aspectRatio;
        altura = scale;
    }

    gluOrtho2D(-largura, largura, -altura, altura);
}

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    if (estadoJogo == MENU)
    {
        desenhaMenu();
    }
    else if (estadoJogo == JOGANDO)
    {
      desenhaFoguetePlayer();
      desenhaMeteoros();  
      desenhaPlacar();
    } else if (estadoJogo == GAMEOVER) {
        desenhaGameOver();
    }

    glFlush();
}

void idle()
{
    if (estadoJogo == JOGANDO)
    {
        comandosJogo();
        movimentaMeteoros();
        colisoesMeteoros();
        colisoesBordaJanela(); 
    }
    else if (estadoJogo == MENU)
    {
        comandosMenu();
    }
    else if (estadoJogo == GAMEOVER)
    {
        comandosGameOver();
    }
    glutPostRedisplay();
}

void inicializa()
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

    glutDisplayFunc(display); // funcao que sera chamada para desenhar
    glutIdleFunc(idle); // funcao que sera chamada quando nao tiver eventos

    glutReshapeFunc(reshape); // ajusta a janela quando ela for redimensionada

    glutKeyboardFunc(keyboard); // funcao que sera chamada quando uma tecla for pressionada
    glutKeyboardUpFunc(keyboardUp); // funcao que sera chamada quando uma tecla for solta
    glutSpecialFunc(specialKeys); // funcao que sera chamada quando uma tecla especial for pressionada
    glutSpecialUpFunc(specialKeysUp); // funcao que sera chamada quando uma tecla especial for solta 

    inicializa();

    glutMainLoop();

    return 0;
}