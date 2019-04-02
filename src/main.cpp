#include "main.h"
#include "timer.h"
#include "ball.h"
#include "square.h"
#include "ground.h"
#include "pond.h"
#include "trampoline.h"
#include "magnet.h"
#include "triangle.h"
#include "life.h"
#include "lose.h"

#include <math.h> 
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball[10];
Square square;
Ground ground;
Pond pond;
Trampoline t;
Magnet magnet;
Triangle triangle;
Life life[3];
Lose lose,win;

int numlife=3;
int jumpflag = 0;
int magnetflag=0;
int numticks=0;
int loseflag=0;
int winflag=0;
int inwater=0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    for (int i=0; i<10; i++)
        if (ball[i].existence)
            ball[i].draw(VP);

    ground.draw(VP);
    pond.draw(VP);
    t.draw(VP);
    if (magnetflag)
        magnet.draw(VP);
    triangle.draw(VP);
    for (int i=0; i<3; i++)
        if (life[i].existence)
            life[i].draw(VP);
    square.draw(VP);
    if (loseflag)
        lose.draw(VP);
    if (winflag)
        win.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if (up && inwater == 0) {
        jumpflag=1;
        square.speed += 0.035;                   
    }
    if (left && inwater == 0) {
        square.position.x -= 0.1;
    }
    if (right && inwater == 0) {
        square.position.x += 0.1;
        
    }
    if (right && inwater) {
         square.position.x += 0.03;
         square.position.y = - sqrt(pow(0.6,2) - pow(square.position.x-1,2)) -2.8;
         //cout << "position of x is "<<square.position.x<<" and position of y is "<<square.position.y<<endl;
    }
    if (left && inwater) {
        square.position.x -= 0.03;
        square.position.y = - sqrt(pow(0.6,2) - pow(square.position.x-1,2)) -2.8;
    }
   /* if (up && inwater) {
        jumpflag=1;
        square.speed +=0.025;
        inwater=0;
    }*/
};

void tick_elements() {
    numticks++;
    for (int i=0;i<10;i++)        
    {    
        ball[i].tick();
    }
    square.tick();
    triangle.tick();
    //Gravity
    if (jumpflag){
        square.speed -= 0.01;
    }
    //collision between ground and square
    if ( square.position.y <= -2.8 && square.position.x <= 0.4)
    {
        square.position.y = -2.8;
        square.speed=0;
        jumpflag = 0;
        
    }
    if (square.position.y <= -2.8 && square.position.x >= 1.6)
    {
        square.position.y = -2.8;
        square.speed=0;
        jumpflag = 0;
    }
    //square in water;
    if (square.position.x >= 0.45 && square.position.x <= 1.55 && square.position.y <= -2.5)
    {
        inwater=1;
    }
    else 
    {
        inwater=0;
        //cout << "inwater is "<<inwater<<endl;
    }
    if (jumpflag && square.position.x >= 0.45 && square.position.x <= 1.55 && square.position.y <= -3.0)
    {
         square.position.y= - sqrt(pow(0.6,2) - pow(square.position.x-1,2)) -2.8;
    }
    //cout << "jumpflag is "<<jumpflag<<endl;
    //cout << "inwater is "<<inwater<<endl;
    //collision between top and square
    if (square.position.y >= 3.8)
    {
        square.position.y = 3.8;
        square.speed = - square.speed;
    }
    //collision between square and side walls
    if (square.position.x >= 4)
        square.position.x = 4;
    if (square.position.x <= -4)
        square.position.x = -4;
    //collision between square and trampoline
    if (square.position.x <= t.position.x + 0.3 && square.position.x >= t.position.x - 0.3 && square.position.y <= -2.3)
        square.speed+=0.02;
    //collision between circle and square
    for (int i=0;i<10;i++)
        if (ball[i].existence && detect_collision(square.bounding_box(), ball[i].bounding_box()))
        {
            ball[i].existence=0;
            square.speed = -square.speed;
        }
    
    //reflection of circle and side walls
    for (int i=0;i<10;i++)
    {
        if (ball[i].position.x >= 3.2f )
	{ 
            ball[i].position.x = 3.2f;
            ball[i].speed = -ball[i].speed;
        }
        if (ball[i].position.x <=-4.0f)
        {
            ball[i].position.x = -4.0f;
            ball[i].speed = -ball[i].speed;
        }
    }
    //reflection of two circles        
    for (int i=0;i<10;i++)
    {
        
        for (int j=0;j<i;j++)
        {
            if (detect_collision(ball[i].bounding_box(),ball[j].bounding_box()))
            {    
                ball[i].speed = -ball[i].speed;
                ball[j].speed = -ball[j].speed;
             }
        }
        
    }
    //entry magnet
    if ((numticks/500)%2==1)
        magnetflag=1;
    else
        magnetflag=0;
    //magnet effect
    if (magnetflag && square.position.y <= magnet.position.y + 0.3 && square.position.y >= magnet.position.y - 0.3)
    {
        square.position.x+=0.5;
    }
    //boundaries for porcupine
    if (triangle.position.x <= 2)
    {
        triangle.position.x = 2;
        triangle.speed = -triangle.speed;
    }
    if (triangle.position.x >= 3.5)
    {
        triangle.position.x = 3.5;
        triangle.speed = -triangle.speed;
    }
    //collision between square and porcupine
    if (detect_collision(square.bounding_box(), triangle.bounding_box()))
    {
         square.position.y = -2.8;
         square.position.x = 0;
         square.speed = 0;
         //lives--;
         life[numlife-1].existence = false;
         numlife--;
    }
    //lives finished
    if (numlife == 0)
        loseflag=1;
    //if win
    int i;
    for (i=0; i<10; i++)
        if (ball[i].existence)
            break;
    if (i==10)
        winflag=1;


}


/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    int pos[]={-3,3,-2,-1,0,2,1,1,1,-2,0,0,1,3,2,1,3,2,-2,-2 };
    for (int i=0; i<10; i++)
    {
        ball[i] = Ball(pos[i*2],pos[i*2+1],(i%2==0)? COLOR_RED:COLOR_GREEN,(i%3==0)? 0:1, (i%6==0)? 1:0);
        if (i%2 == 0)
            ball[i].speed = -ball[i].speed;
    }
    ball[0].rotation += 45;
    ball[6].rotation += 280;
    for (int i=0; i<3; i++)
        life[i] = Life (i-3, -3.5, COLOR_PINK);
    square = Square(0,-3,COLOR_BLACK);
    ground = Ground(0,-3,COLOR_GROUND);
    pond = Pond(1,-3,COLOR_BLUE);
    t = Trampoline (-2,-3,COLOR_GREY);
    magnet = Magnet (3.5,1.5,COLOR_RED, COLOR_BACKGROUND, COLOR_GREY);
    triangle = Triangle(2,-3,COLOR_BROWN);
    
    lose = Lose(0,0,COLOR_RED);
    win = Lose(0,0,COLOR_GREEN);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;
    
    window = initGLFW(width, height);

    initGL (window, width, height);
    
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
    
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
