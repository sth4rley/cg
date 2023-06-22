#include <iostream>
#include <GL/glut.h>
#include <cmath>

#define WIDTH 1280
#define HEIGHT 720

struct Circle {
    float x;
    float y;
    float radius;
};

// variáveis globais

// largura e altura do campo
int fieldWidth = 90;
int fieldHeight = 45;

// coordenadas iniciais do player 1
float player1X = -20.0f;
float player1Y = 0.0f;

// coordenadas iniciais do player 2
float player2X = 20.0f;
float player2Y = 0.0f;

// coordenadas iniciais da bola
float bolaX = 0.0f;
float bolaY = 0.0f;

// tamanho da bola e do player
float raioBola = 1.0f;
float raioPlayer = 1.5f;

// velocidade da bola
float bolaVelocidadeX = 0.0f;
float bolaVelocidadeY = 0.0f;

// Aceleração e desaceleração da bola
float bolaAceleracao = 0.01f;
float bolaDesaceleracao = 0.004f;

// velocidade player
float velocidade_player = 0.20f;
float diagonal_velocity = velocidade_player / sqrt(2);

// velocidade do chute
float player1KickStrength = 0.5f; // Intensidade do chute do jogador 1
float player2KickStrength = 0.5f; // Intensidade do chute do jogador 2

// Peso
float player1Weight = 1.0f; // Peso do jogador 1
float player2Weight = 1.0f; // Peso do jogador 2
float bolaWeight = 0.0f; // Peso da bola
// variáveis de controle de teclado
bool keyStates[256] = { false };
bool specialKeyStates[256] = { false };

void debugLines() {

  // eixo x
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-WIDTH/2, 0.0f);
    glVertex2f(WIDTH/2, 0.0f);
    glEnd();
  // eixo y
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, HEIGHT/2);
    glVertex2f(0.0f, -HEIGHT/2);
    glEnd();
}


bool checkPlayerCollision(const Circle& c1, const Circle& c2) {
    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance <= c1.radius + c2.radius;
}

void drawCircle(const Circle& circle, const float& r, const float& g, const float& b) {
    glTranslatef(circle.x, circle.y, 0.0f);
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float degInRad = i * 3.14159 / 180;
        glVertex2f(cos(degInRad) * circle.radius, sin(degInRad) * circle.radius);
    }
    glEnd();
    glTranslatef(-circle.x, -circle.y, 0.0f);

}


void drawField(){
  // draw field, a retangle with rounded corners
  glBegin(GL_POLYGON);
  // translate
  glTranslatef(0.0f, 0.0f, 0.0f);
  //set color
  glColor3f(0.5f, 0.7f, 0.3f);
  // draw the corners
  glVertex2f(-fieldWidth/2, fieldHeight/2); 
  glVertex2f(fieldWidth/2, fieldHeight/2);
  glVertex2f(fieldWidth/2, -fieldHeight/2);
  glVertex2f(-fieldWidth/2, -fieldHeight/2);

  glEnd();

  // draw a circle inside the field
  Circle circle = {0.0f, 0.0f, 7.0f};
  drawCircle(circle, 1.0f, 1.0f, 1.0f);
  
 // draw a circle inside the field
  Circle circle2 = {0.0f, 0.0f, 6.5f};
  drawCircle(circle2, 0.5f, 0.7f, 0.3f);
  
}

void display() {  
    glClear(GL_COLOR_BUFFER_BIT);

    Circle player1 = {player1X, player1Y, raioPlayer};
    Circle player2 = {player2X, player2Y, raioPlayer};
    Circle bola = {bolaX, bolaY, raioBola};

    drawField();

    drawCircle(player1, 0.0f, 0.0f, 1.0f);
    drawCircle(player2, 1.0f, 0.0f, 0.0f);
    drawCircle(bola, 1.0f, 1.0f, 1.0f);

    debugLines();


    glutSwapBuffers();
}


void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect_ratio = (float)w / h;

    if (w <= h) {
        glOrtho(-25.0f, 25.0f, -25.0f / aspect_ratio, 25.0f / aspect_ratio, -1.0, 1.0);
    }
    else {
        glOrtho(-25.0f * aspect_ratio, 25.0f * aspect_ratio, -25.0f, 25.0f, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
}

void Teclado(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void TecladoUp(unsigned char key, int x, int y) {
    keyStates[key] = false;

    // Desativa o estado de chute quando a tecla de chute é liberada
    if (key == 32) { // 32 é o código ASCII para a tecla de espaço
        keyStates[32] = false;
    }
    else if (key == 13) { // 80 é o código ASCII para a tecla "P"
        keyStates[13] = false;
    }
}

void TeclasEspeciais(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void TeclasEspeciaisUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

bool checkCollision(const Circle& c1, const Circle& c2) {

        float dx = c1.x - c2.x;
        float dy = c1.y - c2.y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance <= c1.radius + c2.radius; 
}

void idle() {
    Circle player1 = { player1X, player1Y, raioPlayer };
    Circle player2 = { player2X, player2Y, raioPlayer };
    Circle bola = { bolaX, bolaY, raioBola }; 

    float diagonal_velocity = velocidade_player / sqrt(2);

    if (keyStates['w'] && keyStates['a']) {
        player1Y += diagonal_velocity;
        player1X -= diagonal_velocity;
    }
    else if (keyStates['w'] && keyStates['d']) {
        player1Y += diagonal_velocity;
        player1X += diagonal_velocity;
    }
    else if (keyStates['s'] && keyStates['a']) {
        player1Y -= diagonal_velocity;
        player1X -= diagonal_velocity;
    }
    else if (keyStates['s'] && keyStates['d']) {
        player1Y -= diagonal_velocity;
        player1X += diagonal_velocity;
    }
    else {
        if (keyStates['w'])
            player1Y += velocidade_player;
        if (keyStates['s'])
            player1Y -= velocidade_player;
        if (keyStates['a'])
            player1X -= velocidade_player;
        if (keyStates['d'])
            player1X += velocidade_player;
    }

    if (specialKeyStates[GLUT_KEY_UP] && specialKeyStates[GLUT_KEY_LEFT]) {
        player2Y += diagonal_velocity;
        player2X -= diagonal_velocity;
    }
    else if (specialKeyStates[GLUT_KEY_UP] && specialKeyStates[GLUT_KEY_RIGHT]) {
        player2Y += diagonal_velocity;
        player2X += diagonal_velocity;
    }
    else if (specialKeyStates[GLUT_KEY_DOWN] && specialKeyStates[GLUT_KEY_LEFT]) {
        player2Y -= diagonal_velocity;
        player2X -= diagonal_velocity;
    }
    else if (specialKeyStates[GLUT_KEY_DOWN] && specialKeyStates[GLUT_KEY_RIGHT]) {
        player2Y -= diagonal_velocity;
        player2X += diagonal_velocity;
    }
    else {
        if (specialKeyStates[GLUT_KEY_UP])
            player2Y += velocidade_player;
        if (specialKeyStates[GLUT_KEY_DOWN])
            player2Y -= velocidade_player;
        if (specialKeyStates[GLUT_KEY_LEFT])
            player2X -= velocidade_player;
        if (specialKeyStates[GLUT_KEY_RIGHT])
            player2X += velocidade_player;
    }

    // Verifica se ocorreu uma colisão entre a bola e o jogador 1
    if (checkCollision(player1, bola)) {

        // Atualiza as coordenadas da bola com base no vetor de direção do jogador 1
        float dirX = bolaX - player1X;
        float dirY = bolaY - player1Y;
        float magnitude = sqrt(dirX * dirX + dirY * dirY);
        dirX /= magnitude;
        dirY /= magnitude;
 
        // Verifica se a tecla de chute do jogador 1 (espaço) está pressionada
        if (keyStates[32]) { // 32 é o código ASCII para a tecla de espaço
            // Chuta a bola com a intensidade definida para o jogador 1
            bolaVelocidadeX += dirX * player1KickStrength;
            bolaVelocidadeY += dirY * player1KickStrength;
        }

        else {
            // Acelera a bola
            bolaVelocidadeX += dirX * bolaAceleracao;
            bolaVelocidadeY += dirY * bolaAceleracao;
        }
 
        // Atualiza as coordenadas da bola
        bolaX += bolaVelocidadeX;
        bolaY += bolaVelocidadeY;
    }
    else if (checkCollision(player2, bola)) {
        // Atualiza as coordenadas da bola com base no vetor de direção do jogador 2
        float dirX = bolaX - player2X;
        float dirY = bolaY - player2Y;
        float magnitude = sqrt(dirX * dirX + dirY * dirY);
        dirX /= magnitude;
        dirY /= magnitude;

        if (keyStates[13]) { // é o código ASCII para a tecla "P"
            // Chuta a bola com a intensidade definida para o jogador 2
            bolaVelocidadeX += dirX * player2KickStrength;
            bolaVelocidadeY += dirY * player2KickStrength;
        } else {
            // Acelera a bola
            bolaVelocidadeX += dirX * bolaAceleracao;
            bolaVelocidadeY += dirY * bolaAceleracao;
        } 

        // Atualiza as coordenadas da bola
        bolaX += bolaVelocidadeX;
        bolaY += bolaVelocidadeY;
    }
    else {
        // Desacelera a bola se não houver colisão
        if (bolaVelocidadeX != 0.0f || bolaVelocidadeY != 0.0f) {
            float dirX = bolaVelocidadeX;
            float dirY = bolaVelocidadeY;
            float magnitude = sqrt(dirX * dirX + dirY * dirY);
            dirX /= magnitude;
            dirY /= magnitude;

            // Reduz a velocidade da bola
            bolaVelocidadeX -= dirX * bolaDesaceleracao;
            bolaVelocidadeY -= dirY * bolaDesaceleracao;

            // Verifica se a bola parou de se mover
            float velocidade = sqrt(bolaVelocidadeX * bolaVelocidadeX + bolaVelocidadeY * bolaVelocidadeY);
            if (velocidade < bolaDesaceleracao) {
                bolaVelocidadeX = 0.0f;
                bolaVelocidadeY = 0.0f;
            }

            // Atualiza as coordenadas da bola
            bolaX += bolaVelocidadeX;
            bolaY += bolaVelocidadeY;
        }
    } 
    
    // verifica a colisao entre os 2 jogadores
    if(checkCollision(player1, player2)){
        // Atualiza as coordenadas da bola com base no vetor de direção do jogador 1
        float dirX = player1X - player2X;
        float dirY = player1Y - player2Y;
        float magnitude = sqrt(dirX * dirX + dirY * dirY);
        dirX /= magnitude;
        dirY /= magnitude; 
 
        // Atualiza as coordenadas da bola
        player1X += dirX * velocidade_player / 5;
        player1Y += dirY * velocidade_player / 5;        
    }
    if(checkCollision(player2,  player1)){
      float dirX = player2X - player1X;
      float dirY = player2Y - player1Y;
      float magnitude = sqrt(dirX * dirX + dirY * dirY);
      dirX /= magnitude;
      dirY /= magnitude;

      player2X += dirX * velocidade_player / 3;
      player2Y += dirY * velocidade_player / 3;

    }

    // trata as colisões com as paredes
if (bolaY + raioBola > fieldHeight / 2) {
    bolaY = fieldHeight / 2 - raioBola;
    bolaVelocidadeY *= -1.0f;
}
else if (bolaY - raioBola < -fieldHeight / 2) {
    bolaY = -fieldHeight / 2 + raioBola;
    bolaVelocidadeY *= -1.0f;
}
else if (bolaX + raioBola > fieldWidth / 2) {
    bolaX = fieldWidth / 2 - raioBola;
    bolaVelocidadeX *= -1.0f;
}
else if (bolaX - raioBola < -fieldWidth / 2) {
    bolaX = -fieldWidth / 2 + raioBola;
    bolaVelocidadeX *= -1.0f;
}


    glutPostRedisplay();
}

void initialize() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(5, 5);
    glutCreateWindow("Vem pro fut");

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(Teclado);
    glutKeyboardUpFunc(TecladoUp);
    glutSpecialFunc(TeclasEspeciais);
    glutSpecialUpFunc(TeclasEspeciaisUp);

    initialize();

    glutMainLoop();

    return 0;
}