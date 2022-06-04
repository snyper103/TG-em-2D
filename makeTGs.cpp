#include "makeTGs.h"
#include "stdld.h"
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define PI 3.14159265358979323846

static long font = (long)GLUT_BITMAP_9_BY_15; // Seleção da fonte.
lista* li;  // Lista
int WIDTH = 600, HEIGHT = 600, numCtrlPoints;   // Tamanho inical da tela e numero de pontos de controle
bool enable = false; // Variavel incumbida de ativar o mouse
info poligono; // Elemento da lista, poligono.

// Inicia janela
void myInit(void)
{
   glClearColor(1.0188, 1.0188, 0.9624, 0.0);
}
// Função que imprime na janela o bitmap da string.
void writeBitmapString(void *font, char* string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
// Função que faz os traços dentro da janela.
void drawScene()
{
   char strNum[10];
   short aux;
   info poligonos;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();

   // Imprime o su-eixo e eixo X.
   for ( register int i = 0; i < 21; ++i )
   {
      if ( -10+i == 0)
      {
         glColor3d(0.0, 0.0, 0.0);  // Preto
         glLineWidth(1.5);
         glBegin(GL_LINES);
            glVertex3d(-100.0, -100.0 + (float)i*10.0, 0.0);
            glVertex3d( 100.0, -100.0 + (float)i*10.0, 0.0);  
         glEnd();
      }
      
      else  // sub-eixo X e y
      {
         glColor3d(0.8627, 0.8627, 0.8627); // cinza claro
         glLineWidth(0.75);
         glLineStipple(2.5, 0xAAAA);
         glEnable(GL_LINE_STIPPLE);
         glBegin(GL_LINES);
            glVertex3d(-100.0 + (float)i*10.0, -100.0, 0.0);   // Sub-eixo X
            glVertex3d(-100.0 + (float)i*10.0, 100.0, 0.0); 

            glVertex3d(-100.0, -100.0 + (float)i*10.0, 0.0);   // Sub-eixo Y
            glVertex3d(100.0, -100.0 + (float)i*10.0, 0.0);
         glEnd();
         glDisable(GL_LINE_STIPPLE);
      }
   }
   

   // Imprime os pontos do eixo X.
   glColor3d(0.0, 0.0, 0.0);  // Preto
   for ( register int i = 0; i < 21; ++i )
   {
      if ( -10+i == 0 )
      {
         glPointSize(10.0);
         glBegin(GL_POINTS);
            glVertex3d(0.0, 0.0, 0.0);
         glEnd();
      }
      
      else
      {
         glLineWidth(1.5);
         glBegin(GL_LINES);
            glVertex3d(-100.0+(float)i*10.0, -2.0, 0.0);
            glVertex3d(-100.0+(float)i*10.0, 2.0, 0.0);
         glEnd();
      }
   }

   // Rotula os pontos do eixo X.
   for ( register int i = 0; i < 21; ++i )
   {
      sprintf(strNum, "%i", -10+i);
      
      if ( -10+i == 0 )
         glRasterPos3d(-99.0+(float)i*10.0, -5, 0.0);

      else
         glRasterPos3d(-100.0+(float)i*10.0, -5, 0.0);
      
      writeBitmapString((void*)font, strNum); 
   }

   // Imprime o eixo Y.
   glLineWidth(1.5);
   glBegin(GL_LINES);
      glVertex3d(0.0, -100.0, 0.0);
      glVertex3d(0.0, 100.0, 0.0);  
   glEnd();

   // Imprime os pontos do eixo Y.
   glLineWidth(1.5);
   glBegin(GL_LINES);
      for ( register int i = 0; i < 21; ++i )
         if ( -100+i*10 != 0 ) 
         {
            glVertex3d(-2.0, -100.0 + (float)i*10.0, 0.0);
            glVertex3d(2.0, -100.0 + (float)i*10.0, 0.0);
         }
   glEnd();

   // Rotula os pontos do eixo Y.
   for ( register int i = 0; i < 21; ++i )
   {
      sprintf(strNum, " %i", -10+i);

      if ( -10+i != 0 )
      {
         glRasterPos3d(0.0, -100.0 + (float)i*10.0, 0.0);
         writeBitmapString((void*)font, strNum); 
      }
   }
   
   for ( register short i = 0; i < listaTamanho(li); ++i )
   {
      consultaListaPos(li, i+1, &poligonos);
      
      glShadeModel(GL_FLAT);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

      aux = 1;
      glColor3dv(poligonos.vetColorEdge);    // Troca cor da poligono
      glLineWidth(2.5);   // Grosura da linha
      glBegin(GL_LINES);
         for ( register short j = 0; j < poligonos.numCtrlPoints; ++j )
         {
            glVertex3dv(poligonos.ctrlpoints[j]);
            glVertex3dv(poligonos.ctrlpoints[aux]);
            ++aux;
            aux %= poligonos.numCtrlPoints;
         }
         
      glEnd();

      // O código abaixo imprime os pontos de controle.
      glPointSize(7.0);   // Tamanho do ponto.
      glColor3dv(poligonos.vetColorPoint);    // Troca cor dos pontos de controle
      glBegin(GL_POINTS);
         for ( register short j = 0; j < poligonos.numCtrlPoints; ++j ) 
            glVertex3dv(poligonos.ctrlpoints[j]);
      glEnd();
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_BLEND);
   }
   
   if ( enable )
   {
      glPointSize(7.0);   // Tamanho do ponto.
      glColor3d(0.0, 0.0, 0.0);    // Troca cor dos pontos de controle
      glBegin(GL_POINTS);
         for ( register short j = 0; j < poligono.numCtrlPoints; ++j ) 
            glVertex3dv(poligono.ctrlpoints[j]);
      glEnd();
   }

   glPopMatrix();
   glutSwapBuffers();
   glFlush();
}
// Redimensiona a janela comforme seu tamanho
void reshape(int w, int h)
{
   WIDTH = w;
   HEIGHT = h;

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   if ( w <= h )
      glOrtho(-110.0, 110.0, -110.0*(GLfloat)h/(GLfloat)w, 110.0*(GLfloat)h/(GLfloat)w, -110.0, 110.0);
   
   else
      glOrtho(-110.0*(GLfloat)w/(GLfloat)h, 110.0*(GLfloat)w/(GLfloat)h, -110.0, 110.0, -110.0, 110.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
// Cria o menu de interação no terminal.
void printInteraction(void)
{
   system("cls || clear");
   printf("\t########## MENU ##########\n"
          "Pressione 1 para inserir poligono por teclado.\n"
          "Pressione 2 para inserir poligono por mouse.\n"
          "Pressione 3 para editar poligono.\n"
          "Pressione 4 para remover poligono.\n"
          "Pressione 5 para resetar.\n"
          "Pressione 6 para fazer transfomacao geometrica.\n"
          "Pressione 0 para sair.\n"
          "###############################################\n");
}

// Verifica se não há nenhum ponto repetido
bool verificaPontos(info dado, short point)
{
   short tam = listaTamanho(li);

   if ( tam > 0 )
   {
      info* curve = (info*)malloc(sizeof(info)*tam);

      for ( register short i = 0; i < tam; ++i )
         consultaListaPos(li, i+1, &curve[i]);
      
      for ( register short i = 0; i < tam; ++i )
         for ( register short j = 0; j < curve[i].numCtrlPoints; ++j )
            for ( register short l = 0; l < point; ++l )
               if ( (dado.ctrlpoints[l][0] == curve[i].ctrlpoints[j][0]) && (dado.ctrlpoints[l][1] == curve[i].ctrlpoints[j][1]) )
                  return false;

      free(curve);
   }

   for ( register short i = 0; i < point-1; ++i )
      if ( (dado.ctrlpoints[i][0] == dado.ctrlpoints[i+1][0]) && (dado.ctrlpoints[i][1] == dado.ctrlpoints[i+1][1]) )
         return false;
   
   return true;
}
// Define a cor de cada ponto de controle e aresta do poligono.
void colorizaPoligono(short ID)
{
   switch (ID)
   {
   case 1:
      poligono.vetColorPoint[0] = 0.5422;
      poligono.vetColorPoint[1] = 0.40;
      poligono.vetColorPoint[2] = 0.0578;

      poligono.vetColorEdge[0] = 0.1176;
      poligono.vetColorEdge[1] = 0.3588;
      poligono.vetColorEdge[2] = 0.5235;
      break;

   case 2:
      poligono.vetColorPoint[0] = 0.4628;
      poligono.vetColorPoint[1] = 0.3031;
      poligono.vetColorPoint[2] = 0.2341;

      poligono.vetColorEdge[0] = 0.6067;
      poligono.vetColorEdge[1] = 0.0933;
      poligono.vetColorEdge[2] = 0.30;
      break;

   case 3:
      poligono.vetColorPoint[0] = 0.0;
      poligono.vetColorPoint[1] = 0.5090;
      poligono.vetColorPoint[2] = 0.4910;

      poligono.vetColorEdge[0] = 0.7520;
      poligono.vetColorEdge[1] = 0.0;
      poligono.vetColorEdge[2] = 0.2480;
      break;

   case 4:
      poligono.vetColorPoint[0] = 0.4808;
      poligono.vetColorPoint[1] = 0.3894;
      poligono.vetColorPoint[2] = 0.1298;

      poligono.vetColorEdge[0] = 0.5148;
      poligono.vetColorEdge[1] = 0.4852;
      poligono.vetColorEdge[2] = 0.0;
      break;
   
   default:
      poligono.vetColorPoint[0] = 0.8164;
      poligono.vetColorPoint[1] = 0.1836;
      poligono.vetColorPoint[2] = 0.0;

      poligono.vetColorEdge[0] = 0.2339;
      poligono.vetColorEdge[1] = 0.2982;
      poligono.vetColorEdge[2] = 0.4678;
      break;
   }
}

void leTriangulo(short numPolygon)
{
   short aux = -1;

   for ( register short i = 0; i < 3; ++i )
      do
      {
         if ( aux == i )
            printf("\t#####   ERRO!   #####\n"
                   "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n");

         aux = i;
         printf("Qual a coordenada do ponto %hi (x e y): ", i+1);
         scanf(" %lf %lf", &poligono.ctrlpoints[i][0], &poligono.ctrlpoints[i][1]);
         poligono.ctrlpoints[i][0] *= 10.0;
         poligono.ctrlpoints[i][1] *= 10.0;
         poligono.ctrlpoints[i][2] = 0.0;
         poligono.numCtrlPoints = i+1;
         enable = true;
         drawScene();
         enable = false;
      } while ( (poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
                (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
                !verificaPontos(poligono, i+1) );

   colorizaPoligono(numPolygon);

   poligono.ID = numPolygon;
}
void leQuadrado(short numPolygon)
{
   short aux = -1;

   for ( register short i = 0; i < 4; ++i )
      do
      {
         if ( aux == i )
            printf("\t#####   ERRO!   #####\n"
                   "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n");

         aux = i;
         printf("Qual a coordenada do ponto %hi (x e y): ", i+1);
         scanf(" %lf %lf", &poligono.ctrlpoints[i][0], &poligono.ctrlpoints[i][1]);
         poligono.ctrlpoints[i][0] *= 10.0;
         poligono.ctrlpoints[i][1] *= 10.0;
         poligono.ctrlpoints[i][2] = 0.0;
         poligono.numCtrlPoints = i+1;
         enable = true;
         drawScene();
         enable = false;
      } while ( (poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
                (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
                !verificaPontos(poligono, i+1) );

   colorizaPoligono(numPolygon);

   poligono.ID = numPolygon;
}
void leHexagono(short numPolygon)
{
   short aux =-1;

   for ( register short i = 0; i < 6; ++i )
      do
      {
         if ( aux == i )
            printf("\t#####   ERRO!   #####\n"
                   "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n");

         aux = i;
         printf("Qual a coordenada do ponto %hi (x e y): ", i+1);
         scanf(" %lf %lf", &poligono.ctrlpoints[i][0], &poligono.ctrlpoints[i][1]);
         poligono.ctrlpoints[i][0] *= 10.0;
         poligono.ctrlpoints[i][1] *= 10.0;
         poligono.ctrlpoints[i][2] = 0.0;
         poligono.numCtrlPoints = i+1;
         enable = true;
         drawScene();
         enable = false;
      } while ( (poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
                (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
                !verificaPontos(poligono, i+1) );

   colorizaPoligono(numPolygon);

   poligono.ID = numPolygon;
}
bool fazPoligonoRegular(short numPolygon, short numEdge, double sizEdge, double x, double y)
{
   double r, theta;
   bool ehRepetido = false;

   r = sizEdge/(2.0*sin(PI/(double)numEdge));
   theta = (2.0*PI)/(double)numEdge;

   poligono.numCtrlPoints = numEdge;
   poligono.ID = numPolygon;

   if ( numEdge == 4 )
   {
      poligono.ctrlpoints[0][0] = (x-(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[0][1] = (y-(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[0][2] = 0.0;

      poligono.ctrlpoints[1][0] = (x-(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[1][1] = (y+(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[1][2] = 0.0;

      poligono.ctrlpoints[2][0] = (x+(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[2][1] = (y+(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[2][2] = 0.0;

      poligono.ctrlpoints[3][0] = (x+(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[3][1] = (y-(sizEdge/2.0)) * 10.0;;
      poligono.ctrlpoints[3][2] = 0.0;
   }
   
   else
      for ( register short i = 0; i < numEdge; ++i )
      {
         poligono.ctrlpoints[i][0] = (r*cos((double)i*theta) + x) * 10.0;;
         poligono.ctrlpoints[i][1] = (r*sin((double)i*theta) + y) * 10.0;;
         poligono.ctrlpoints[i][2] = 0.0;
      }
   
   for ( register short i = 0; i < numEdge; ++i )
      if ((poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
          (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
          !verificaPontos(poligono, i+1))
      {
         ehRepetido = true;
         break;
      }
   
   colorizaPoligono(numPolygon);

   return ehRepetido;
}
// Faz a leitura dos pontos de controle do poligono de inicializa os dados de maneira correta para serem inseridos na lista.
bool lePoligono(short numPolygon)
{
   short opSW, opIF;
   bool fezDesenho = false;
   double x = 0.0, y = 0.0, tam = 0.0;

   do
   {
      printf("\t\t\tPoligono %hi\n"
             "   #######   ...INSERIR POLIGONO POR TECLADO...   #######\n"
            "[1] - Inserir poligono por pontos de controle.\n"
            "[2] - Inserir poligono pelo ponto central.\n"
            "[0] - Voltar.\n"
            "-------------------------------------------------------\n"
            "Pressione o numero representante da opcao desejada: ", listaTamanho(li)+1);
      scanf(" %hi%*[^\n]s", &opSW);

      switch ( opSW )
      {
      case 0:
         fezDesenho = true;
         poligono.numCtrlPoints = -1;
         break;

      case 1:
         system("cls || clear");
         printf("\t\t\tPoligono %hi\n"
            "   #######   ...INSERIR POLIGONO POR PONTOS DE CONTROLE...   #######\n"
            "[1] - Inserir triangulo.\n"
            "[2] - Inserir quadrado.\n"
            "[3] - Inserir hexagono.\n"
            "[0] - Voltar.\n"
            "-------------------------------------------------------\n"
            "Pressione o numero representante da opcao desejada: ", listaTamanho(li)+1);
         scanf(" %hi%*[^\n]s", &opIF);

         if ( opIF == 1 )
         {
            system("cls || clear");
            leTriangulo(numPolygon);
            opSW = 0;
            fezDesenho = true;
         }

         else
         {
            if ( opIF == 2 )
            {
               system("cls || clear");
               leQuadrado(numPolygon);
               opSW = 0;
               fezDesenho = true;
            }

            else
            {
               if ( opIF == 3 )
               {
                  system("cls || clear");
                  leHexagono(numPolygon);
                  opSW = 0;
                  fezDesenho = true;
               }
               else
                  if ( opIF == 0 )
                  {
                     fezDesenho = true;
                     poligono.numCtrlPoints = -1;
                  }
            }
         }
         break;

      case 2:
         system("cls || clear");
         printf("\t\t\tPoligono %hi\n"
            "   #######   ...INSERIR POLIGONO PELO PONTO CENTRAL...   #######\n"
            "[1] - Inserir triangulo.\n"
            "[2] - Inserir quadrado.\n"
            "[3] - Inserir hexagono.\n"
            "[0] - Voltar.\n"
            "-------------------------------------------------------\n"
            "Pressione o numero representante da opcao desejada: ", listaTamanho(li)+1);
         scanf(" %hi%*[^\n]s", &opIF);

         if ( opIF == 1 )
         {
            system("cls || clear");
            printf("Qual o tamanho da aresta: ");
            scanf(" %lf", &tam);

            printf("Qual a coordenada do ponto central (x e y): ");
            scanf(" %lf %lf", &x, &y);

            if ( (x > 10.0 || x < -10.0) || (y > 10.0 || y < -10.0) )
            {
               system("cls || clear");
               printf("\t#####   ERRO!   #####\n"
                      "Ponto central excede limite da tela ou repetido, insira um novo.\n"
                      "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );

               fezDesenho = false;
               break;
            }
            
            if ( !fazPoligonoRegular(numPolygon, 3, tam, x, y) )
            {
               opSW = 0;
               fezDesenho = true;
            }

            else
            {
               system("cls || clear");
               printf("\t#####   ERRO!   #####\n"
                      "Pontos de controle excedem limites da tela ou repetidos, insira um novo.\n"
                      "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );
               fezDesenho = false;
            }
         }

         else
         {
            if ( opIF == 2 )
            {
               system("cls || clear");
               printf("Qual o tamanho da aresta: ");
               scanf(" %lf", &tam);

               printf("Qual a coordenada do ponto central (x e y): ");
               scanf(" %lf %lf", &x, &y);

               if ( (x > 10.0 || x < -10.0) || (y > 10.0 || y < -10.0) )
               {
                  system("cls || clear");
                  printf("\t#####   ERRO!   #####\n"
                        "Ponto central excede limite da tela ou repetido, insira um novo.\n"
                        "Pressione enter para continuar.\n");
                  while ( (getchar()) != '\n' );
                  getchar();

                  fezDesenho = false;
                  break;
               }
               
               if ( !fazPoligonoRegular(numPolygon, 4, tam, x, y) )
               {
                  opSW = 0;
                  fezDesenho = true;
               }

               else
               {
                  system("cls || clear");
                  printf("\t#####   ERRO!   #####\n"
                        "Pontos de controle excedem limites da tela ou repetidos, insira um novo.\n"
                        "Pressione enter para continuar.\n");
                  while ( (getchar()) != '\n' );
                  getchar();
                  fezDesenho = false;
               }
            }

            else
            {
               if ( opIF == 3 )
               {
                  system("cls || clear");
                  printf("Qual o tamanho da aresta: ");
                  scanf(" %lf", &tam);

                  printf("Qual a coordenada do ponto central (x e y): ");
                  scanf(" %lf %lf", &x, &y);

                  if ( (x > 10.0 || x < -10.0) || (y > 10.0 || y < -10.0) )
                  {
                     system("cls || clear");
                     printf("\t#####   ERRO!   #####\n"
                           "Ponto central excede limite da tela ou repetido, insira um novo.\n"
                           "Pressione enter para continuar.\n");
                     while ( (getchar()) != '\n' );
                     getchar();

                     fezDesenho = false;
                     break;
                  }
                  
                  if ( !fazPoligonoRegular(numPolygon, 6, tam, x, y) )
                  {
                     opSW = 0;
                     fezDesenho = true;
                  }

                  else
                  {
                     system("cls || clear");
                     printf("\t#####   ERRO!   #####\n"
                           "Pontos de controle excedem limites da tela ou repetidos, insira um novo.\n"
                           "Pressione enter para continuar.\n");
                     while ( (getchar()) != '\n' );
                     getchar();
                     fezDesenho = false;
                  }
               }

               else
                  if ( opIF == 0 )
                  {
                     fezDesenho = true;
                     poligono.numCtrlPoints = -1;
                  }
            }
         }
         break;
      
      default:
         system("cls || clear");
         printf("\t#####   ERRO!   #####\n"
                "Opcao digitada incorreta, por favor digite uma opcao valida.\n"
                "Pressione enter para continuar.\n");
         while ( (getchar()) != '\n' );
         getchar();

         fezDesenho = false;
         break;
      }

      system("cls || clear");
   } while ( opSW != 0 );

   return fezDesenho;
}

// Edita um poligono via terminal.
info editaPoligono(short ID, short numEdge)
{
   short aux = -1;
   removeMeio(li, ID);

   switch ( numEdge )
   {
   case 3:
      poligono.numCtrlPoints = 3;

      system("cls || clear");
      printf("\t\tPoligono %hi\n"
             "#######   ...EDITAR TRIANGULO...   #######\n", ID);

      for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
         do
         {
            if ( aux == i )
               printf("\t#####   ERRO!   #####\n"
                     "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n");

            aux = i;
            printf("Qual a coordenada do ponto %hi (x e y): ", i+1);
            scanf(" %lf %lf", &poligono.ctrlpoints[i][0], &poligono.ctrlpoints[i][1]);
            poligono.ctrlpoints[i][0] *= 10.0;
            poligono.ctrlpoints[i][1] *= 10.0;
            poligono.ctrlpoints[i][2] = 0.0;
         } while ( (poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
                   (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
                   !verificaPontos(poligono, i+1) );
      break;
   
   case 4:
      poligono.numCtrlPoints = 4;

      system("cls || clear");
      printf("\t\tPoligono %hi\n"
             "#######   ...EDITAR QUADRADO...   #######\n", ID);

      for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
         do
         {
            if ( aux == i )
               printf("\t#####   ERRO!   #####\n"
                     "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n");

            aux = i;
            printf("Qual a coordenada do ponto %hi (x e y): ", i+1);
            scanf(" %lf %lf", &poligono.ctrlpoints[i][0], &poligono.ctrlpoints[i][1]);
            poligono.ctrlpoints[i][0] *= 10.0;
            poligono.ctrlpoints[i][1] *= 10.0;
            poligono.ctrlpoints[i][2] = 0.0;
         } while ( (poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
                   (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
                   !verificaPontos(poligono, i+1) );
      break;
   
   case 6:
      poligono.numCtrlPoints = 6;

      system("cls || clear");
      printf("\t\tPoligono %hi\n"
             "#######   ...EDITAR HEXAGONO...   #######\n", ID);

      for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
         do
         {
            if ( aux == i )
               printf("\t#####   ERRO!   #####\n"
                     "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n");

            aux = i;
            printf("Qual a coordenada do ponto %hi (x e y): ", i+1);
            scanf(" %lf %lf", &poligono.ctrlpoints[i][0], &poligono.ctrlpoints[i][1]);
            poligono.ctrlpoints[i][0] *= 10.0;
            poligono.ctrlpoints[i][1] *= 10.0;
            poligono.ctrlpoints[i][2] = 0.0;
         } while ( (poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
                   (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
                   !verificaPontos(poligono, i+1) );
      break;

   default:
      printf("\t#####   ERRO!   #####\n"
             "Pressione enter para continuar.\n");
      while ( (getchar()) != '\n' );
      getchar();

      poligono.numCtrlPoints = 0;

      poligono.ctrlpoints[0][0] = 0.0;
      poligono.ctrlpoints[0][1] = 0.0;
      poligono.ctrlpoints[0][2] = 0.0;
      break;
   }

   colorizaPoligono(ID);

   poligono.ID = ID;

   return poligono;
}

// Tranformaçoes geométricas.
bool transladaPoligono(double x, double y)
{
   bool pontoErrado = false;

   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      poligono.ctrlpoints[i][0] += x;
      poligono.ctrlpoints[i][1] += y;

      if ((poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
          (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
          !verificaPontos(poligono, i+1))
      {
         pontoErrado = true;
         break;
      }
   }
   
   return pontoErrado;
}
bool alteraEscalaPoligono(double escala)
{
   bool pontoErrado = false;
   double centro[2];

   centro[0] = 0.0;
   centro[1] = 0.0;
   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      centro[0] += poligono.ctrlpoints[i][0];
      centro[1] += poligono.ctrlpoints[i][1];
   }
   centro[0] /= poligono.numCtrlPoints;
   centro[1] /= poligono.numCtrlPoints;

   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      poligono.ctrlpoints[i][0] *= escala;
      poligono.ctrlpoints[i][1] *= escala;
   }

   centro[0] -= escala*centro[0];
   centro[1] -= escala*centro[1];

   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      poligono.ctrlpoints[i][0] += centro[0];
      poligono.ctrlpoints[i][1] += centro[1];

      if ((poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
          (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
          !verificaPontos(poligono, i+1))
      {
         pontoErrado = true;
         break;
      }
   }

   return pontoErrado;
}
void refletePoligono(short op)
{
   switch ( op )
   {
   case 1:
      for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
         poligono.ctrlpoints[i][0] *= -1.0;
      break;
   case 2:
      for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
         poligono.ctrlpoints[i][1] *= -1.0;
      break;
   case 3:
      for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
      {
         poligono.ctrlpoints[i][0] *= -1.0;
         poligono.ctrlpoints[i][1] *= -1.0;
      }
      break;
   default:
      break;
   }
}
bool rotacionaPoligono(double ang)
{
   bool pontoErrado = false;
   double theta = ang*PI/180.0, centro[2], x, y;

   centro[0] = 0.0;
   centro[1] = 0.0;
   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      centro[0] += poligono.ctrlpoints[i][0];
      centro[1] += poligono.ctrlpoints[i][1];
   }
   centro[0] /= poligono.numCtrlPoints;
   centro[1] /= poligono.numCtrlPoints;
   
   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      x = poligono.ctrlpoints[i][0];
      y = poligono.ctrlpoints[i][1];

      poligono.ctrlpoints[i][0] = centro[0] + (x-centro[0])*cos(theta) - (y-centro[1])*sin(theta);   
      poligono.ctrlpoints[i][1] = centro[1] + (x-centro[0])*sin(theta) + (y-centro[1])*cos(theta);

      if ((poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
          (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
          !verificaPontos(poligono, i+1))
      {
         pontoErrado = true;
         break;
      }
   }

   return pontoErrado;
}
bool cisalhaPoligono(double coefX, double coefY)
{
   bool pontoErrado = false;
   double centro[2], x, y;

   centro[0] = 0.0;
   centro[1] = 0.0;
   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      centro[0] += poligono.ctrlpoints[i][0];
      centro[1] += poligono.ctrlpoints[i][1];
   }
   centro[0] /= poligono.numCtrlPoints;
   centro[1] /= poligono.numCtrlPoints;

   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      x = poligono.ctrlpoints[i][0];
      y = poligono.ctrlpoints[i][1];

      if ( WIDTH <= HEIGHT )
      {
         poligono.ctrlpoints[i][0] = x+coefX*(y-110.0*HEIGHT/WIDTH);   
         poligono.ctrlpoints[i][1] = y+coefY*(x-110.0);
      }
      else
      {
         poligono.ctrlpoints[i][0] = x+coefX*(y-110.0);   
         poligono.ctrlpoints[i][1] = y+coefY*(x-110.0*WIDTH/HEIGHT);
      }
   }

   if ( WIDTH <= HEIGHT )
   {
      x = centro[0];
      y = centro[1];

      centro[0] -= (x+coefX*(y-110.0*HEIGHT/WIDTH));
      centro[1] -= (y+coefY*(x-110.0));
   }
   else
   {
      x = centro[0];
      y = centro[1];

      centro[0] -= (x+coefX*(y-110.0));
      centro[1] -= (y+coefY*(x-110.0*WIDTH/HEIGHT));
   }

   for ( register short i = 0; i < poligono.numCtrlPoints; ++i )
   {
      poligono.ctrlpoints[i][0] += centro[0];
      poligono.ctrlpoints[i][1] += centro[1];

      if ((poligono.ctrlpoints[i][0] > 100.0 || poligono.ctrlpoints[i][0] < -100.0) ||
          (poligono.ctrlpoints[i][1] > 100.0 || poligono.ctrlpoints[i][1] < -100.0) ||
          !verificaPontos(poligono, i+1))
      {
         pontoErrado = true;
         break;
      }
   }

   return pontoErrado;
}
void transfomacaoGeometrica()
{
   short ID, op;
   double x, y, escala, ang, coef;
   info backupPoligono;

   do
   {
      system("cls || clear");
      printf("   #######   ...TRANSFOMACAO GEOMETRICA...   #######\n"
             "[1] - Translada poligono.\n"
             "[2] - Alterar escala do poligono.\n"
             "[3] - Reflete poligono.\n"
             "[4] - Rotaciona poligono.\n"
             "[5] - Cisalha poligono.\n"
             "[0] - Voltar.\n"
             "-------------------------------------------------------\n"
             "Pressione o numero representante da opcao desejada: ");
      scanf(" %hi%*[^\n]s", &op);

      switch ( op )
      {
      case 0:
         break;
      case 1:
         system("cls || clear");
         printf("   #######   ...TRANSLACAO DE FIGURA GEOMETRICA...   #######\n"
                "-------------------------------------------------------\n"
                "Qual poligono deseja transladar (1 a %hi): ", listaTamanho(li));
         scanf(" %hi", &ID);

         if ( ID >= 1 && ID <= listaTamanho(li) )
         {
            printf("######################################################\n"
                  "\t\t\tPoligono %hi\n"
                  "-------------------------------------------------------\n"
                  "Digite a magnitude do deslocamento (x e y): ", ID);
            scanf(" %lf %lf", &x, &y);

            x *= 10.0;
            y *= 10.0;

            consultaListaPos(li, ID, &poligono);
            if ( transladaPoligono(x, y) )
            {
               system("cls || clear");
               printf("\t#####   ERRO!   #####\n"
                  "pontos excedem tamanho da tela ou repetidos, por favor o insira novamente.\n"
                  "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );
            }

            else
            {
               op = 0;
               removeMeio(li, ID);
               insereListaMeio(li, poligono);
            }
         }

         else
         {
            system("cls || clear");
            printf("\t#####   ERRO!   #####\n"
                     "Poligono invalido, digite um novo"
                     "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }
         break;
      case 2:
         system("cls || clear");
         printf("   #######   ...ESCALACAO DE FIGURA GEOMETRICA...   #######\n"
                "-------------------------------------------------------\n"
                "Qual poligono deseja alterar a escala (1 a %hi): ", listaTamanho(li));
         scanf(" %hi", &ID);

         if ( ID >= 1 && ID <= listaTamanho(li) )
         {
            printf("######################################################\n"
                  "\t\t\tPoligono %hi\n"
                  "-------------------------------------------------------\n"
                  "Digite em quantas vezes deseja alterar escala: ", ID);
            scanf(" %lf", &escala);

            consultaListaPos(li, ID, &poligono);
            if ( alteraEscalaPoligono(escala) )
            {
               system("cls || clear");
               printf("\t#####   ERRO!   #####\n"
                  "pontos excedem tamanho da tela ou repetidos, por favor o insira novamente.\n"
                  "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );
            }

            else
            {
               op = 0;
               removeMeio(li, ID);
               insereListaMeio(li, poligono);
            }
         }

         else
         {
            system("cls || clear");
            printf("\t#####   ERRO!   #####\n"
                     "Poligono invalido, digite um novo"
                     "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }
         break;
      case 3:
         system("cls || clear");
         printf("   #######   ...REFLEXAO DE FIGURA GEOMETRICA...   #######\n"
               "[1] - Refletir apenas eixo X.\n"
               "[2] - Refletir apenas eixo Y.\n"
               "[3] - Refletir eixo X e Y.\n"
               "[0] - Voltar.\n"
               "-------------------------------------------------------\n"
               "Pressione o numero representante da opcao desejada: ");
         scanf(" %hi", &op);
         
         if ( op < 0 || op > 3 )
         {
            system("cls || clear");
            printf("\t#####   ERRO!   #####\n"
                   "Opcao invalida, digite uma nova"
                   "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }

         else
            if ( op > 0 && op < 4 )
            {
               printf("######################################################\n"
                      "Qual poligono deseja alterar a escala (1 a %hi): ", listaTamanho(li));
               scanf(" %hi", &ID);
               consultaListaPos(li, ID, &poligono);

               if ( ID >= 1 && ID <= listaTamanho(li) )
               {
                  refletePoligono(op);
                  op = 0;
                  removeMeio(li, ID);
                  insereListaMeio(li, poligono);
               }
               
               else
               {
                  system("cls || clear");
                  printf("\t#####   ERRO!   #####\n"
                           "Poligono invalido, digite um novo"
                           "Pressione enter para continuar.\n");
                  while ( (getchar()) != '\n' );
                  getchar();
               }
            }
         break;
      case 4:
         system("cls || clear");
         printf("   #######   ...ROTACAO DE FIGURA GEOMETRICA...   #######\n"
                "-------------------------------------------------------\n"
                "Qual poligono deseja alterar a escala (1 a %hi): ", listaTamanho(li));
         scanf(" %hi", &ID);

         if ( ID >= 1 && ID <= listaTamanho(li) )
         {
            printf("######################################################\n"
                  "\t\t\tPoligono %hi\n"
                  "-------------------------------------------------------\n"
                  "Digite o angulo de rotacao (em graus): ", ID);
            scanf(" %lf", &ang);

            consultaListaPos(li, ID, &poligono);
            consultaListaPos(li, ID, &backupPoligono);
            removeMeio(li, ID);

            if ( rotacionaPoligono(ang) )
            {
               system("cls || clear");
               printf("\t#####   ERRO!   #####\n"
                  "pontos excedem tamanho da tela ou repetidos, por favor o insira novamente.\n"
                  "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );
               insereListaMeio(li, backupPoligono);
            }

            else
            {
               op = 0;
               insereListaMeio(li, poligono);
            }
         }

         else
         {
            system("cls || clear");
            printf("\t#####   ERRO!   #####\n"
                     "Poligono invalido, digite um novo"
                     "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }
         break;
      case 5:
         system("cls || clear");
         printf("   #######   ...CISALHAMENTO DE FIGURA GEOMETRICA...   #######\n"
               "[1] - Cisalhar apenas eixo X.\n"
               "[2] - Cisalhar apenas eixo Y.\n"
               "[3] - Cisalhar eixo X e Y.\n"
               "[0] - Voltar.\n"
               "-------------------------------------------------------\n"
               "Pressione o numero representante da opcao desejada: ");
         scanf(" %hi", &op);

         if ( op < 0 || op > 3 )
         {
            system("cls || clear");
            printf("\t#####   ERRO!   #####\n"
                   "Opcao invalida, digite uma nova"
                   "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }

         else
         {
            if ( op > 0 && op < 4 )
            {
               printf("-------------------------------------------------------\n"
                     "Qual poligono deseja fazer o cisalhamento (1 a %hi): ", listaTamanho(li));
               scanf(" %hi", &ID);

               if ( ID >= 1 && ID <= listaTamanho(li) )
               {
                  printf("######################################################\n"
                        "\t\t\tPoligono %hi\n"
                        "-------------------------------------------------------\n"
                        "Digite o coeficiente (X%%): ", ID);
                  scanf(" %lf", &coef);

                  coef /= 100.0;
                  consultaListaPos(li, ID, &backupPoligono);
                  consultaListaPos(li, ID, &poligono);
                  removeMeio(li, ID);

                  switch ( op )
                  {
                  case 1:
                     if ( cisalhaPoligono(coef, 0.0) )
                     {
                        system("cls || clear");
                        printf("\t#####   ERRO!   #####\n"
                           "pontos excedem tamanho da tela ou repetidos, por favor o insira novamente.\n"
                           "Pressione enter para continuar.\n");
                        while ( (getchar()) != '\n' );
                        getchar();
                        insereListaMeio(li, backupPoligono);
                     }

                     else
                     {
                        op = 0;
                        insereListaMeio(li, poligono);
                     }
                     break;
                  case 2:
                     if ( cisalhaPoligono(0.0, coef) )
                     {
                        system("cls || clear");
                        printf("\t#####   ERRO!   #####\n"
                           "pontos excedem tamanho da tela ou repetidos, por favor o insira novamente.\n"
                           "Pressione enter para continuar.\n");
                        while ( (getchar()) != '\n' );
                        getchar();
                        insereListaMeio(li, backupPoligono);
                     }

                     else
                     {
                        op = 0;
                        insereListaMeio(li, poligono);
                     }
                     break;
                  case 3:
                     if ( cisalhaPoligono(coef, coef) )
                     {
                        system("cls || clear");
                        printf("\t#####   ERRO!   #####\n"
                           "pontos excedem tamanho da tela ou repetidos, por favor o insira novamente.\n"
                           "Pressione enter para continuar.\n");
                        while ( (getchar()) != '\n' );
                        getchar();
                        insereListaMeio(li, backupPoligono);
                     }

                     else
                     {
                        op = 0;
                        insereListaMeio(li, poligono);
                     }
                     break;
                  default:
                     break;
                  }
               }
               else
               {
                  system("cls || clear");
                  printf("\t#####   ERRO!   #####\n"
                           "Poligono invalido, digite um novo"
                           "Pressione enter para continuar.\n");
                  while ( (getchar()) != '\n' );
                  getchar();
               }
            }
         }
         break;
      default:
         system("cls || clear");
         printf("\t#####   ERRO!   #####\n"
                "Opcao digitada incorreta, por favor digite uma opcao valida.\n"
                "Pressione enter para continuar.\n");
         while ( (getchar()) != '\n' );
         getchar();
         break;
      }
   } while ( op != 0 );
}

// Função para lidar com os inputs do mouse.
void mouseInput(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && enable )
   {
      if ( WIDTH <= HEIGHT )
      {
         poligono.ctrlpoints[numCtrlPoints][0] = ( (2.0*110.0*x)/WIDTH ) - 110.0;
         poligono.ctrlpoints[numCtrlPoints][1] = (( (2.0*110.0*HEIGHT/WIDTH)*(y-HEIGHT) )/(-HEIGHT) ) - 110.0*HEIGHT/WIDTH;
         poligono.ctrlpoints[numCtrlPoints][2] = 0.0;
      }

      else
      {
         poligono.ctrlpoints[numCtrlPoints][0] = ( (2.0*110.0*WIDTH/HEIGHT*x)/WIDTH ) - 110.0*WIDTH/HEIGHT;
         poligono.ctrlpoints[numCtrlPoints][1] = (( (2.0*110.0)*(y-HEIGHT) )/(-HEIGHT) ) - 110.0;
         poligono.ctrlpoints[numCtrlPoints][2] = 0.0;
      }

      if ( (poligono.ctrlpoints[numCtrlPoints][0] <= 100.0 && poligono.ctrlpoints[numCtrlPoints][0] >= -100.0) &&
           (poligono.ctrlpoints[numCtrlPoints][1] <= 100.0 && poligono.ctrlpoints[numCtrlPoints][1] >= -100.0) &&
            verificaPontos(poligono, numCtrlPoints+1) && numCtrlPoints < 6 )
      {
         printf("A coordenada do ponto %hi (x e y): %.2f\t%.2f\n", numCtrlPoints+1, poligono.ctrlpoints[numCtrlPoints][0]/10.0, poligono.ctrlpoints[numCtrlPoints][1]/10.0);
         
         ++poligono.numCtrlPoints;
         ++numCtrlPoints;
         
         glutPostRedisplay();
      }

      else
      {
         if ( numCtrlPoints >= 6 )
            printf("\t#####   ERRO!   #####\n"
                   "Numero maximo de pontos de controle atingido, por favor conclua com botao direito.\n");
         
         else
            printf("\t#####   ERRO!   #####\n"
                   "Ponto excede limite da tela ou repetido, insira um novo.\n"
                   "%.2f\t%.2f\n", poligono.ctrlpoints[numCtrlPoints][0], poligono.ctrlpoints[numCtrlPoints][1]);
      }
   }   

   if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && enable )
   {
      colorizaPoligono(listaTamanho(li)+1);
      
      poligono.ID = listaTamanho(li)+1;

      if ( numCtrlPoints == 3 || numCtrlPoints == 4 || numCtrlPoints == 6 )
         insereListaFinal(li, poligono);

      else
      {
         system("cls || clear");
         printf("\t#####   ERRO!   #####\n"
                "Numero de pontos inseridos invalido, por favor insira um novamente.\n"
                "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );
      }

      poligono.numCtrlPoints = 0;
      numCtrlPoints = 0;
      enable = false;

      printInteraction();
      glutPostRedisplay();
   }
}
// Função para lidar com os inputs do teclado.
void keyInput(unsigned char key, int x, int y)
{
   short ID, tam;

   switch (key) 
   {
      case '0':
         liberaLista(li);
         system("cls || clear");
         glutLeaveMainLoop();
         break;
      case '1':
         system("cls || clear");

         if ( (listaTamanho(li)+1) > 5 )
         {
            printf("\t#####   ERRO!   #####\n"
                   "O limite maximo de poligonos ja foi atingindo,"
                   " por favor remova um poligono antes de inserir o proximo\n"
                   "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }
         
         else
         {
            if ( lePoligono(listaTamanho(li)+1) && poligono.numCtrlPoints != -1 )
               insereListaFinal(li, poligono);

            else
               if ( poligono.numCtrlPoints != -1 )
               {
                  printf("\t#####   ERRO!   #####\n"
                     "ponto excede tamanho da tela ou repetido, por favor o insira novamente.\n"
                     "Pressione enter para continuar.\n");
                  while ( (getchar()) != '\n' );
                  getchar();
               }
         }
         
         printInteraction();
         glutPostRedisplay();
         break;
      case '2':
         system("cls || clear");
         if ( listaTamanho(li)+1 < 6 )
         {
            poligono.numCtrlPoints = 0;
            numCtrlPoints = 0;
            enable = true;

            printf("Clique com botao esquerdo para inserir pontos de controle (3, 4 ou 6).\n"
                  "Clique com botao direito para terminar a insercao.\n"
                  "\t#####\tPoligono %hi\t#####\n", listaTamanho(li)+1);
         }

         else
         {
               printf("\t#####   ERRO!   #####\n"
                     "O limite maximo de poligonos ja foi atingindo,"
                     " por favor remova um poligono antes de inserir o proximo\n"
                     "Pressione enter para continuar.\n");
               while ( (getchar()) != '\n' );

               printInteraction();
               glutPostRedisplay();
         }
         break;
      case '3':
         system("cls || clear");
         printf("Qual poligono deseja editar (1 a %hi): ", listaTamanho(li));
         scanf(" %hi", &ID);

         if ( ID > 0 && ID <= listaTamanho(li) )
         {
            consultaListaPos(li, ID, &poligono);
            insereListaMeio(li, editaPoligono(ID, poligono.numCtrlPoints));
         }

         else
         {
            printf("\t#####   ERRO!   #####\n"
                     "Poligono invalido, digite um novo"
                     "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }

         printInteraction();
         glutPostRedisplay();
         break;
      case '4':
         system("cls || clear");

         printf("Qual poligono deseja remover (1 a %hi): ", listaTamanho(li));
         scanf(" %hi", &ID);

         if ( ID > 0 && ID <= listaTamanho(li) )
         {
            removeMeio(li, ID);

            tam = listaTamanho(li);
            for ( register short i = ID; i <= tam; ++i )
            {
               consultaListaPos(li, ID, &poligono);
               poligono.ID=i;
               colorizaPoligono(i);
               removeMeio(li, i+1);
               insereListaFinal(li, poligono);
            }
         }

         else
         {
            printf("\t#####   ERRO!   #####\n"
                     "Poligono invalido, digite um novo"
                     "Pressione enter para continuar.\n");
            while ( (getchar()) != '\n' );
            getchar();
         }
         
         printInteraction();
         glutPostRedisplay();
         break;
      case '5':
         system("cls || clear");

         printf("   #######   ...RESETAR...   #######\n"
               "Tem certeza de que deseja limpar a tela, isso apagara\ntodos os poligonos da tela.\n"
               "[1] - Sim.\n"
               "[0] - Voltar.\n"
               "-------------------------------------------------------\n"
               "Pressione o numero representante da opcao desejada: ");
         scanf(" %hi%*[^\n]s", &ID);

         if ( ID == 1 )
         {
            liberaLista(li);
            criaLista(&li);
         }

         glutPostRedisplay();
         printInteraction();
         break;
      case '6':
         transfomacaoGeometrica();

         glutPostRedisplay();
         printInteraction();
         break;
      default:
         system("cls || clear");
         printf("\t#####   ERRO!   #####\n"
                "Opcao digitada incorreta, por favor digite uma opcao valida.\n"
                "Pressione enter para continuar.\n");
         while ( (getchar()) != '\n' );
         getchar();

         printInteraction();
         glutPostRedisplay();
         break;
  }
}

// Executa o programa feito.
void display(int argc, char** argv)
{
   criaLista(&li);
   
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Polygon Maker");
   myInit();

   glutDisplayFunc(drawScene); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseInput);
   glutMainLoop();
}
