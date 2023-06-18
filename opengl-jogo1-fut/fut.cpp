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

float player1X = -20.0f;
float player1Y = 0.0f;

float player2X = 20.0f;
float player2Y = 0.0f;

float bolaX = 0.0f;
float bolaY = 0.0f;

float raioBola = 1.0f;
float raioPlayer = 1.5f;

float bolaVelocidadeX = 0.0f;
float bolaVelocidadeY = 0.0f;
float bolaAceleracao = 0.01f;
float bolaDesaceleracao = 0.005f;
float velocidadeMaxima = 2.0f;

// velocidade player
float velocidade_player = 0.15f;
float diagonal_velocity = velocidade_player / sqrt(2);

void debugLines() {
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-WIDTH/2, 0.0f);
    glVertex2f(WIDTH/2, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, HEIGHT/2);
    glVertex2f(0.0f, -HEIGHT/2);
    glEnd();
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
void display() {  
    glClear(GL_COLOR_BUFFER_BIT);

    Circle player1 = {player1X, player1Y, raioPlayer};
    Circle player2 = {player2X, player2Y, raioPlayer};
    Circle bola = {bolaX, bolaY, raioBola};

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

bool keyStates[256] = { false };
bool specialKeyStates[256] = { false };

void Teclado(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void TecladoUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void TeclasEspeciais(int key, int x, int y) {
    specialKeyStates[key] = true;
}

void TeclasEspeciaisUp(int key, int x, int y) {
    specialKeyStates[key] = false;
}

void idle() {
    Circle player1 = { player1X, player1Y, raioPlayer };
    Circle player2 = { player2X, player2Y, raioPlayer };
    Circle bola = { bolaX, bolaY, raioBola };

    // Função auxiliar para verificar se duas circunferências colidem
    auto checkCollision = [](const Circle& c1, const Circle& c2) {
        float dx = c1.x - c2.x;
        float dy = c1.y - c2.y;
        float distance = sqrt(dx * dx + dy * dy);
        return distance <= c1.radius + c2.radius;
    };

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

        // Acelera a bola
        bolaVelocidadeX += dirX * bolaAceleracao;
        bolaVelocidadeY += dirY * bolaAceleracao;

        // Limita a velocidade máxima da bola
        float velocidade = sqrt(bolaVelocidadeX * bolaVelocidadeX + bolaVelocidadeY * bolaVelocidadeY);
        if (velocidade > velocidadeMaxima) {
            bolaVelocidadeX = (bolaVelocidadeX / velocidade) * velocidadeMaxima;
            bolaVelocidadeY = (bolaVelocidadeY / velocidade) * velocidadeMaxima;
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

        // Acelera a bola
        bolaVelocidadeX += dirX * bolaAceleracao;
        bolaVelocidadeY += dirY * bolaAceleracao;

        // Limita a velocidade máxima da bola
        float velocidade = sqrt(bolaVelocidadeX * bolaVelocidadeX + bolaVelocidadeY * bolaVelocidadeY);
        if (velocidade > velocidadeMaxima) {
            bolaVelocidadeX = (bolaVelocidadeX / velocidade) * velocidadeMaxima;
            bolaVelocidadeY = (bolaVelocidadeY / velocidade) * velocidadeMaxima;
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

    glutPostRedisplay();
}

void initialize() {
    glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
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