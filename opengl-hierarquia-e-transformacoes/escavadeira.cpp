#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

// variaveis globais
GLfloat pos_x_escavadeira = 0;
GLfloat ang_braco1_escavadeira = 45;
GLfloat ang_braco2_escavadeira = 90;
GLfloat ang_braco3_escavadeira = 225;
GLfloat dis_braco2_escavadeira = 9;
GLfloat window_size = 25; // tamanho da janela
int active_axis = 1;

void desenhaBraco1() {
    glBegin(GL_POLYGON);
        glVertex2f(0.3,10.0);
        glVertex2f(1.1,-0.8);
        glVertex2f(-1.1,-0.8);
        glVertex2f(-0.3,10.0);    
    glEnd();

    glColor3f(0.2f,0.2f,0.2f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.3,10.0);
        glVertex2f(1.1,-0.8);
        glVertex2f(-1.1,-0.8);
        glVertex2f(-0.3,10.0);
    glEnd();

  // circulo que indica se o eixo esta ativo
  active_axis == 1 ? glColor3f(0.0,1.0,0.0) : glColor3f(1.0, 0.0, 0.0);
  glTranslatef(0.0, 0.0, 0.0);
  glutSolidSphere(0.30, 10, 10);
}

void desenhaBraco2() {

    glBegin(GL_POLYGON);
        glVertex2f(0.3,8.0);
        glVertex2f(0.6,-0.8);
        glVertex2f(-0.6,-0.8);
        glVertex2f(-0.3,8.0);    
    glEnd();
 
    // draw stroke of previus GL_POLYGON
    glColor3f(0.2f,0.2f,0.2f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0.3,8.0);
        glVertex2f(0.6,-0.8);
        glVertex2f(-0.6,-0.8);
        glVertex2f(-0.3,8.0);
    glEnd();

  // circulo que indica se o eixo esta ativo
  if(active_axis == 2){
    glColor3f(0.0,1.0,0.0);
  } else if (active_axis == 4){
    glColor3f(0.0, 0.0, 1.0);
  } else {
    glColor3f(1.0, 0.0, 0.0);
  }
  glTranslatef(0.0, 0.0, 0.0);
  glutSolidSphere(0.30, 10, 10);

}

void pa () {
  glBegin(GL_POLYGON);
    glVertex2f(0.0,0.0); 
    glVertex2f(1.0,-0.3);
    glVertex2f(2.0,0.0);
    glVertex2f(3.0,1.0);
    glVertex2f(3.5,2.0);
    glVertex2f(3.0,1.0);
    glVertex2f(0.0,2.0);
    glVertex2f(-0.5,1.0);
  glEnd();
  
  // circulo que indica se o eixo esta ativo
  active_axis == 3 ? glColor3f(0.0,1.0,0.0) : glColor3f(1.0, 0.0, 0.0);
  glTranslatef(0.5, 1.0, 0.0);
  glutSolidSphere(0.2, 10, 10);
}

// Função para desenhar a base da escavadeira          
void DesenhaBase()
{
	glLineWidth(2);
  
  glColor3f(0.85f,0.75f,0.0f);

  // base
  glBegin(GL_POLYGON);
    glVertex2f(-2.0, 0.0);
    glVertex2f(2.0, 0.0);
    glVertex2f(2.0, 2.0);
    glVertex2f(-2.0, 0.7);
	glEnd();

  // cabine
  glColor3f(0.2f,0.2f,0.2f);
  glBegin(GL_POLYGON);
    glVertex2f(-2.0f, 0.7f);
    glVertex2f(2.0f, 2.0f);
    glVertex2f(2.0f, 5.0f);
    glVertex2f(-1.0f, 4.0f);
    glVertex2f(-2.0f, 1.0f);

  glEnd();

  glColor3f(0.85f,0.75f,0.0f);
  // retangula atras
  glBegin(GL_POLYGON);
    glVertex2f(2.0, 0.0);
    glVertex2f(3.5, 0.0);
    glVertex2f(3.5, 2.5);
    glVertex2f(2.0, 2.5);
  glEnd();
    
  // esteira
  glColor3f(0.4f,0.4f,0.4f);
  glBegin(GL_POLYGON);
    glVertex2f(-2.5, -0.2);
    glVertex2f(4.0, -0.2);
    glVertex2f(4.5, -0.7);
    glVertex2f(4.0, -1.5);
    glVertex2f(-2.5, -1.5);
    glVertex2f(-3.0, -0.7);
  glEnd(); 

  // contorno esteira 
  glColor3f(0.2f,0.2f,0.2f);
  glLineWidth(8);
  glBegin(GL_LINE_LOOP);
    glVertex2f(-2.5, -0.2);
    glVertex2f(4.0, -0.2);
    glVertex2f(4.5, -0.7);
    glVertex2f(4.0, -1.5);
    glVertex2f(-2.5, -1.5);
    glVertex2f(-3.0, -0.7);
  glEnd();
}

void Desenha(void)
{
    // Muda para o sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);

    // Inicializa a matriz de transformação corrente
    glLoadIdentity();

    // Limpa a janela de visualização com a cor de fundo definida previamente
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o ceu
    glColor3f(0.1f, 0.1f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(-window_size, window_size);
    glVertex2f(window_size, window_size);
    glVertex2f(window_size, -window_size);
    glVertex2f(-window_size, -window_size);
    glEnd();

    // Desenha o "chão"
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(-window_size, -12.0);
    glVertex2f(window_size, -12.0);
    glVertex2f(window_size, -window_size);
    glVertex2f(-window_size, -window_size);
    glEnd();

    // Desenha um objeto modelado com transformações hierárquicas
    glPushMatrix();
    glTranslatef(pos_x_escavadeira, -10.0f, 0.0f);

    glPushMatrix();
    glScalef(2.5f, 2.5f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    DesenhaBase();
    glPopMatrix();

    // Desenha o braço1
    glTranslatef(0.0f, 1.5f, 0.0f);
    glRotatef(ang_braco1_escavadeira, 0.0f, 0.0f, 1.0f);
    glScalef(1.4f, 1.4f, 1.0f);
    glColor3f(0.8f, 0.75f, 0.0f);
    desenhaBraco1();

    // Desenha o braço2
    glTranslatef(0.0f, dis_braco2_escavadeira, 0.0f);
    glRotatef(ang_braco2_escavadeira, 0.0f, 0.0f, 1.0f);
    glColor3f(0.8f, 0.75f, 0.0f);
    desenhaBraco2();

    // Desenha a pá
    glTranslatef(0.0f, 8.0f, 0.0f);
    glRotatef(ang_braco3_escavadeira, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.3f, -1.0f, 0.0f);
    glScalef(1.3f, 1.3f, 1.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    pa();

    glPopMatrix();

    // Executa os comandos OpenGL
    glFlush();
}



void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // Evita a divisão por zero
    if (h == 0) h = 1;

    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);

    // Inicializa o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calcula os valores para a janela de seleção
    float largura, altura;
    float window_size;
    
    if (w <= h)
    {
        largura = 25.0f;
        altura = 25.0f * h / w;
    }
    else
    {
        largura = 25.0f * w / h;
        altura = 25.0f;
    }
    
    gluOrtho2D(-largura, largura, -altura, altura);
    window_size = largura;
}

void TeclasEspeciais(int key, int x, int y)
{
    // Move a base
    const float step = 0.30;
    const float step_angle = 3.0;
    
    if (key == GLUT_KEY_LEFT)
    {
        pos_x_escavadeira -= step;
        pos_x_escavadeira = std::max(-window_size, pos_x_escavadeira);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        pos_x_escavadeira += step;
        pos_x_escavadeira = std::min(window_size, pos_x_escavadeira);
    }

    // Altera o controle de acordo com a tecla pressionada
    switch (key)
    {
        case GLUT_KEY_F1: // muda o controle para o braco 1
            active_axis = 1;
            break;
        case GLUT_KEY_F2: // muda o controle para o braco 2
            active_axis = 2;
            break;
        case GLUT_KEY_F3: // muda o controle para a pa
            active_axis = 3;
            break;
        case GLUT_KEY_F4: // mudar o controle o ajuste da distancia do eixo
            active_axis = 4;
            break;
        case GLUT_KEY_UP: // aumenta o angulo do eixo ativo
            switch (active_axis)
            {
                case 1:
                    ang_braco1_escavadeira = std::max(-30.0f, ang_braco1_escavadeira - step_angle);
                    break;
                case 2:
                    ang_braco2_escavadeira = std::max(-135.0f, ang_braco2_escavadeira - step_angle);
                    break;
                case 3:
                    ang_braco3_escavadeira = std::max(45.0f, ang_braco3_escavadeira - step_angle);
                    break;
                case 4:
                    dis_braco2_escavadeira = std::min(9.0f, dis_braco2_escavadeira + step);
                    break;
            }
            break;
        case GLUT_KEY_DOWN: // diminui o angulo do eixo ativo
            switch (active_axis)
            {
                case 1:
                    ang_braco1_escavadeira = std::min(115.0f, ang_braco1_escavadeira + step_angle);
                    break;
                case 2:
                    ang_braco2_escavadeira = std::min(135.0f, ang_braco2_escavadeira + step_angle);
                    break;
                case 3:
                    ang_braco3_escavadeira = std::min(225.0f, ang_braco3_escavadeira + step_angle);
                    break;
                case 4:
                    dis_braco2_escavadeira = std::max(4.0f, dis_braco2_escavadeira - step);
                    break;
            }
            break;
    }

    glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos de teclas
void Teclado (unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
}
           
// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{   
  // apresenta os controles
  std::cout << "Controles:" << std::endl;
  std::cout << "F1: controla o braco 1" << std::endl;
  std::cout << "F2: controla o braco 2" << std::endl;
  std::cout << "F3: controla a pa" << std::endl;
  std::cout << "F4: controla a distancia do eixo 2" << std::endl;
  std::cout << "Setas para cima: aumenta o angulo do eixo ativo" << std::endl;
  std::cout << "Setas para baixo: diminui o angulo do eixo ativo" << std::endl;
  std::cout << "Setas para esquerda: move a escavadeira para a esquerda" << std::endl;
  std::cout << "Setas para direita: move a escavadeira para a direita" << std::endl;

  std::cout << "Esc: sai do programa" << std::endl;
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);  
	glutInitWindowPosition(5,5);     
	glutInitWindowSize(450,300);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  
	glutCreateWindow("atividade 1"); 
 
	glutDisplayFunc(Desenha);  

	glutReshapeFunc(AlteraTamanhoJanela);    

	glutSpecialFunc(TeclasEspeciais);

	glutKeyboardFunc(Teclado);

	Inicializa(); 
 
	glutMainLoop();

	return 0;
}
