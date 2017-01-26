#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///////////////////////////////////////////////

#define PI 3.14159
#define LIMIT 9000

typedef struct {
  
  GLfloat radius;
  GLfloat *x;
  GLfloat *y;
  GLfloat xSpeed;
  GLfloat ySpeed;
  //float life;

} particleDrop;

//bool default_color = true;
//bool random_colors = true;
bool starry_sky = false;
bool redish = false;
bool greenish = false;
bool blueish = false;
//bool trail = true;

GLfloat blue_diff_material[] = {0.0,0.0,1.0,1.0};
GLfloat blue_specular_material[] = {0.0,0.0,0.5,0.5};

GLfloat image_scale = 5.0;

GLfloat len, y_acceleration = 0.3, x_acceleration = 0.18;

GLfloat wind_speed = 0.0;

//float delta = 0.01;

float particle_apparition_probability = 1.0;
int particle_apparition_rate = 10;
int splash_rate = 10;

int NR_PARTICLES = 1;
particleDrop *drop;

int trail_len = 2;

GLint width, height;

//////////////////////////////////////////////

void removeParticle(unsigned int index)
{
  drop[index] = drop[NR_PARTICLES-1];
  NR_PARTICLES--;
}

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

void initializeNewDrop(particleDrop &drop0)
{
  drop0.radius = 2.0f + 5.0f*myRandom();
  drop0.x = (GLfloat *) malloc (sizeof(GLfloat) * trail_len);
  drop0.y = (GLfloat *) malloc (sizeof(GLfloat) * trail_len);

  drop0.x[0] = ((GLfloat)width * (GLfloat)myRandom());
  drop0.y[0] = ((GLfloat)height - len + len*(GLfloat)myRandom());
  for(int i=1;i<trail_len;i++)
  {
    drop0.x[i] = drop0.x[0];
    drop0.y[i] = drop0.y[0];
  }

  drop0.xSpeed = 0.0;
  drop0.ySpeed = 0.5 * myRandom();
}


void splashDrop(particleDrop drop0)
{
  int drops_nr = (int)(splash_rate * myRandom()) + 1;
  //increaseParticleArray(drops_nr);
  if(drops_nr + NR_PARTICLES > LIMIT)
    printf("%s\n", "Won't make new particles");
  else
  {
    NR_PARTICLES = NR_PARTICLES + drops_nr;

    GLfloat velocity = -drop0.ySpeed*(75 - y_acceleration)*myRandom()/100;

    for(int i = NR_PARTICLES-1; i>NR_PARTICLES-1-drops_nr; i--)
    {
      GLfloat angle = 120*myRandom() + 30;
      drop[i].radius = drop0.radius / 2;
      drop[i].ySpeed = velocity * sin(angle * PI/180.0);
      drop[i].xSpeed = velocity * cos(angle * PI/180.0);
      //
      drop[i].x = (GLfloat *) malloc (sizeof(GLfloat) * trail_len);
      drop[i].y = (GLfloat *) malloc (sizeof(GLfloat) * trail_len);

      drop[i].y[0] = 0.0;
      drop[i].x[0] = drop0.x[0];

      for(int i=1;i<trail_len;i++)
      {
        drop[i].x[i] = drop[i].x[0];
        drop[i].y[i] = drop[i].y[0];
      }
      //
      //drop[i].life = 200.0;
    }
  }
}

//////////////////////////////////////////////////////////////////////////

void move()
{
  for(int i=0; i<NR_PARTICLES; i++)
  {

    drop[i].ySpeed = drop[i].ySpeed + y_acceleration;
    if(drop[i].xSpeed < wind_speed - 0.001 || drop[i].xSpeed > wind_speed + 0.001)
      drop[i].xSpeed = drop[i].xSpeed + x_acceleration * wind_speed;
/*    if(drop[i].xSpeed > wind_speed + 0.001)
      drop[i].xSpeed = drop[i].xSpeed - x_acceleration * wind_speed;*/
    drop[i].y[0] = drop[i].y[0] - drop[i].ySpeed;
    drop[i].x[0] = drop[i].x[0] + drop[i].xSpeed;

    //drop[i].life -= delta; 
  }

  if(myRandom() < particle_apparition_probability)
  {
    int nr_new_particles = (int)(myRandom()*particle_apparition_rate) + 1;
    //increaseParticleArray(nr_new_particles);
    if(nr_new_particles + NR_PARTICLES > LIMIT)
      printf("%s\n", "Won't make new particles");
    else
    {
      NR_PARTICLES = NR_PARTICLES + nr_new_particles;
      for(int i = NR_PARTICLES-1; i>NR_PARTICLES-1-nr_new_particles; i--)
      {
        initializeNewDrop(drop[i]);
      }
    }
  }

  glutPostRedisplay();
}

///////////////////////////////////////////////

void menu (int menuentry) {
  switch (menuentry) {
  
  case 1: y_acceleration += 0.3;
          break;
  
  case 2: if(y_acceleration >= 0.3)
            y_acceleration -= 0.3;
          else printf("%s\n", "Can't set negative gravity");
          break;
  
  case 3: wind_speed += 0.5;
          break;
  
  case 4: wind_speed -= 0.5;
          break;
  
  case 5: particle_apparition_rate += 2;
          break;

  case 6: if(particle_apparition_rate >= 2)
            particle_apparition_rate -= 2;
          break;
  
  case 7: splash_rate += 2;
          break;
  
  case 8: if(splash_rate >= 2)
            splash_rate -= 2;
          break;
  
  case 9: starry_sky = !starry_sky;
          break;
  
  case 10: redish = !redish;
           break;

  case 11: greenish = !greenish;
           break;

  case 12: redish = false;
           greenish = false;
           blueish = false;
           break;

  
  case 13: exit(0);
  }
}

///////////////////////////////////////////////

void init()
{
  glClearColor(0.0,0.0,0.0,0.0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  width = glutGet(GLUT_WINDOW_WIDTH);
  height = glutGet(GLUT_WINDOW_HEIGHT);

  glutCreateMenu (menu);
  glutAddMenuEntry ("Increment gravity", 1);
  glutAddMenuEntry ("Decrement gravity", 2);
  glutAddMenuEntry ("Increment wind", 3);
  glutAddMenuEntry ("Decrement wind", 4);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Increment spawn rate", 5);
  glutAddMenuEntry ("Decrement spawn rate", 6);
  glutAddMenuEntry ("Increment splash", 7);
  glutAddMenuEntry ("Decrement splash", 8);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Toggle starry sky", 9);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Toggle redish", 10);
  glutAddMenuEntry ("Toggle greenish", 11);
  glutAddMenuEntry ("Toggle default", 12);
  glutAddMenuEntry ("", 999);
  glutAddMenuEntry ("Quit", 13);
  glutAttachMenu (GLUT_RIGHT_BUTTON);

  drop = (particleDrop *) malloc (sizeof(particleDrop) * LIMIT);

  len = (GLfloat)height * 15.0 / 100.0;

  srand (time(NULL));

  for(int i=0; i<NR_PARTICLES; i++)
  {
    initializeNewDrop(drop[i]);
    //drop[i].life = 200.0;
  }
}

///////////////////////////////////////////////

void drawDrop(particleDrop &drop0, unsigned int index)
{
 
  GLfloat x = drop0.x[0];
  GLfloat y = drop0.y[0];
  
  //glColor3f(0.0, 0.0, 1.0); // blue

  GLfloat radius = drop0.radius;
  GLfloat p1, p2;

  glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diff_material);

  GLfloat random_specular_material[] = {0.0,0.0,0.0,0.8};

  if(redish)
    random_specular_material[0] = myRandom();
  if(greenish)
    random_specular_material[1] = myRandom();
  if(blueish)
    random_specular_material[2] = myRandom();

  int temp_len = trail_len-1;
/*
  if(!trail)
  {
    temp_len = 1;
  }
*/
  if(starry_sky)
    temp_len ++;

  if(redish || greenish || blueish)
  {
    glMaterialfv(GL_FRONT, GL_SPECULAR, random_specular_material);
  }
  else
    glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular_material);

  for(int i=0;i<temp_len;i++)
  {
  
    glBegin(GL_POLYGON);
      p1 = drop0.x[i] + (radius - 0.005*i) * cos(359 * PI/180.0f);
      p2 = drop0.y[i] + (radius - 0.005*i) * sin(359 * PI/180.0f);
      for(int j=0; j<360; j+=60)
      {
        glVertex3f(p1, p2, 0.0);
        p1 = drop0.x[i] + (radius - 0.005*i) * cos(j * PI/180.0f);
        p2 = drop0.y[i] + (radius - 0.005*i) * sin(j * PI/180.0f);
        glVertex3f(p1, p2, 0.0);
      }
    glEnd();
  }

  if(drop0.y[0] <= 5.0)
  {
    
    if(drop0.ySpeed > 11.0 && NR_PARTICLES<LIMIT)
      splashDrop(drop0);
      
    removeParticle(index);
  }

  if(drop0.x[0] >= width + 100.0 || drop0.x[0] < -100.0 || drop0.y[0] > height + 100)
  {
    removeParticle(index);
  }
}

void display()
{
/* called when opengl needs to update the display */
  glClearColor(0.0, 0.0, 0.0, 0); // background color
  glClear(GL_COLOR_BUFFER_BIT); // clear the screen
  glShadeModel(GL_SMOOTH);

  GLfloat light[] = {1.0,1.0,0.0,1.0};

  glLightfv(GL_LIGHT0, GL_AMBIENT, light);

  glLoadIdentity();

  gluLookAt(1.0, 1.0, 1.0,  // position of camera
            1.0, 1.0, 0.0,  // point at which camera looks
            0.0, 1.0, 0.0); // up direction of camera

  for(int i=0; i<NR_PARTICLES; i++)
    drawDrop(drop[i],i);
  
  glutSwapBuffers();

  //glFlush();
}

///////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y)
{
  if(key == 27) exit(0);
}

///////////////////////////////////////////////

void reshape(int width, int height)
{
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0,(GLsizei)width, -1.0,(GLsizei)height, -1.0,1.0);
  glMatrixMode(GL_MODELVIEW);
}

/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(1920, 1020);
  glutCreateWindow("PARTICLES");
  init();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(move);
  glutMainLoop();
  return 0;
}