#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ao/ao.h>
#include <mpg123.h>
// #include <thread> 


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define F first
#define S second
#define PB push_back
#define MP make_pair
#define BITS 8

using namespace std;
struct COLOR {
    float r;
    float g;
    float b;
};
typedef struct COLOR COLOR;
COLOR red = {1,0,0};
COLOR green = {0,1,0};
COLOR black = {0,0,0};
COLOR white = {1,1,1};
COLOR blue = {0,0,1};
COLOR lightpink = {255/255.0,122/255.0,173/255.0};
COLOR darkpink = {255/255.0,51/255.0,119/255.0};
COLOR gold = {255.0/255.0,223.0/255.0,0.0/255.0};
COLOR lightred = {255.0/255.0,179.0/255.0,179.0/255.0};
COLOR lightgreen = {153.0/255.0,255.0/255.0,153.0/255.0};
COLOR bluegreen = {0,1,1};
int said = 0;

int ERRORHIT = 5;
int ERRORCATCH = 5;
int gameover = 0;
int xxx = 0;
int xredbuck = 0,flred=0,redpos = -345,flred1=0,flred2=0;
int xgreenbuck = 0,flgreen=0,greenpos = 345,flgreen1=0,flgreen2=0;
int bulletpos, xbul = -340,flbul = 0,ybul,once = 0;
int canShoot = 1;
float angle;
double mouse_x,mouse_y;
int score = 0;
int c = 80;
int highlight_red = 0;
int highlight_green = 0;

float x_change = 0; //For the camera pan
float y_change = 0; //For the camera pan
double mouse_pos_x, mouse_pos_y;
double new_mouse_pos_x, new_mouse_pos_y;
double mouse_initial_x,mouse_initial_y;
int legal = 1;
int BRICKSPEED = 5,BRICKWAITTIME=50;

std::vector< pair<pair<float,float> , COLOR> > brickpos;
std::vector< pair< pair<float,float> , pair<float,COLOR> > > bulpos;
std::vector<pair<pair<float,float>, float> > mirror;
map<string,int> sevenseg;
float x_zoom=400.0f;
float y_zoom=400.0f;
int countBricks = 0,countBul=0;
int laserangle = 0;

int bulletvel = 15;
int bulletvel_black = 5;
int bulletvel_red = 18;
int gunvel = 10;
float zoom_camera = 1;


int ylazer = 0,fllaz,lazpos = 0,lazpos2 = 0;
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

VAO *redbuckO,*redbuck2O,*greenbuckO,*greenbuck2O,*whitelineO,*blacklineO,*lazerO1,*lazerO2,*redbrick,*greenbrick,*blackbrick,*mirrorO1,*mirrorO2,*mirrorO3,*mirrorO4,*spc_redbrick,
*gold_brick; 
VAO *circle1,*bulletO,*bullet1,*bullet2;
VAO *topO, *bottomO, *middleO, *middle1O,*middle2O,*right1O,*right2O,*left1O,*left2O;
VAO *topO_big, *bottomO_big, *middleO_big, *middle1O_big,*middle2O_big,*right1O_big,*right2O_big,*left1O_big,*left2O_big;
map<string, pair<float,float> > dimension;



struct GLMatrices {
  glm::mat4 projection;
  glm::mat4 model;
  glm::mat4 view;
  GLuint MatrixID;
} Matrices;

GLuint programID;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open())
  {
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> VertexShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

  // Link the program
  fprintf(stdout, "Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
  glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
//    exit(EXIT_SUCCESS);
}


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

GLFWwindow* initGLFW (int width, int height);
void initGL (GLFWwindow* window, int width, int height);
/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

mpg123_handle *mh;
unsigned char *buffer;
size_t buffer_size;
size_t done;
int err;

int driver;
ao_device *dev;

ao_sample_format format;
int channels, encoding;
long rate;

void audio_init() {
    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = 3000;
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, "./subway.mp3");
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
}

void audio_play() {
    /* decode and play */
    if (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
        ao_play(dev, (char*) buffer, done);
    else mpg123_seek(mh, 0, SEEK_SET);
}

void audio_close() {
    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}
/**************************
 * Customizable functions *
 **************************/
void setStrokes(char val){
    sevenseg["top"]=0;
    sevenseg["bottom"]=0;
    sevenseg["middle"]=0;
    sevenseg["left1"]=0;
    sevenseg["left2"]=0;
    sevenseg["right1"]=0;
    sevenseg["right2"]=0;
    sevenseg["middle1"]=0;
    sevenseg["middle2"]=0;
    if(val=='0' || val=='2' || val=='3' || val=='5' || val=='6'|| val=='7' || val=='8' || val=='9' || val=='P' || val=='I' || val=='O' || val=='N' || val=='T' || val=='S' || val=='E'){
        sevenseg["top"]=1;
    }
    if(val=='2' || val=='3' || val=='4' || val=='5' || val=='6' || val=='8' || val=='9' || val=='P' || val=='S' || val=='Y' || val=='E'){
        sevenseg["middle"]=1;
    }
    if(val=='0' || val=='2' || val=='3' || val=='5' || val=='6' || val=='8' || val=='9' || val=='O' || val=='S' || val=='I' || val=='Y' || val=='U' || val=='L' || val=='E' || val=='W'){
        sevenseg["bottom"]=1;
    }
    if(val=='0' || val=='4' || val=='5' || val=='6' || val=='8' || val=='9' || val=='P' || val=='O' || val=='N' || val=='S' || val=='Y' || val=='U' || val=='L' || val=='E' || val=='W'){
        sevenseg["left1"]=1;
    }
    if(val=='0' || val=='2' || val=='6' || val=='8' || val=='P' || val=='O' || val=='N' || val=='U' || val=='L' || val=='E' || val=='W'){
        sevenseg["left2"]=1;
    }
    if(val=='0' || val=='1' || val=='2' || val=='3' || val=='4' || val=='7' || val=='8' || val=='9' || val=='P' || val=='O' || val=='N' || val=='Y' || val=='U' || val=='W'){
        sevenseg["right1"]=1;
    }
    if(val=='0' || val=='1' || val=='3' || val=='4' || val=='5' || val=='6' || val=='7' || val=='8' || val=='9' || val=='O' || val=='N' || val=='S' || val=='Y' || val=='U' || val=='W'){
        sevenseg["right2"]=1;
    }
    if(val=='I' || val=='T'){
        sevenseg["middle1"]=1;
    }
    if(val=='I' || val=='T' || val=='W'){
        sevenseg["middle2"]=1;
    }
    // cout<<"yes sevenseg defined\n";
    // cout<<"score: "<<val<<endl;
    return;
}


float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;
void mousescroll(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset==-1) { 
        zoom_camera /= 1.1; //make it bigger than current size
    }
    else if(yoffset==1){
        zoom_camera *= 1.1; //make it bigger than current size
    }
    if (zoom_camera<=1) {
        zoom_camera = 1;
    }
    if (zoom_camera>=4) {
        zoom_camera=4;
    }
    if(x_change-400.0f/zoom_camera<-400)
        x_change=-400+400.0f/zoom_camera;
    else if(x_change+400.0f/zoom_camera>400)
        x_change=400-400.0f/zoom_camera;
    if(y_change-400.0f/zoom_camera<-400)
        y_change=-400+400.0f/zoom_camera;
    else if(y_change+400.0f/zoom_camera>400)
        y_change=400-400.0f/zoom_camera;
    Matrices.projection = glm::ortho((float)(-400.0f/zoom_camera+x_change), (float)(400.0f/zoom_camera+x_change), (float)(-400.0f/zoom_camera+y_change), (float)(400.0f/zoom_camera+y_change), 0.1f, 500.0f);
}

void check_pan(){
    if(x_change-400.0f/zoom_camera<-400)
        x_change=-400+400.0f/zoom_camera;
    else if(x_change+400.0f/zoom_camera>400)
        x_change=400-400.0f/zoom_camera;
    if(y_change-400.0f/zoom_camera<-400)
        y_change=-400+400.0f/zoom_camera;
    else if(y_change+400.0f/zoom_camera>400)
        y_change=400-400.0f/zoom_camera;
}

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_UP:
                mousescroll(window,0,+1);
                check_pan();
                break;
            case GLFW_KEY_DOWN:
                mousescroll(window,0,-1);
                check_pan();
                break;
            case GLFW_KEY_LEFT_CONTROL:
                  flred1 = 0;
                  flred2 = 0;
                  flred = 0;
                  break;
            case GLFW_KEY_LEFT_ALT:
                  flgreen1 = 0;
                  flgreen2 = 0;
                  flgreen = 0;
                  break;
            case GLFW_KEY_RIGHT_CONTROL:
                  flred1 = 0;
                  flred2 = 0;
                  flred = 0;
                  break;
            case GLFW_KEY_RIGHT_ALT:
                  flgreen1 = 0;
                  flgreen2 = 0;
                  flgreen = 0;
                  break;
            case GLFW_KEY_LEFT:
                  flred2 = 0;
                  flgreen2 = 0;
                  flgreen = 0;
                  flred = 0;
                  break;
            case GLFW_KEY_RIGHT:
                  flgreen2 = 0;
                  flred2 = 0;
                  flgreen = 0;
                  flred = 0;
                  break;
            case GLFW_KEY_X:
                  flgreen = 0;
                  break;
            case GLFW_KEY_C:
                  flgreen = 0;
                  break;
            case GLFW_KEY_S:
                  fllaz = 0;
                  break;
            case GLFW_KEY_F:
                  fllaz = 0;
                  break;
            case GLFW_KEY_E:
                  fllaz = 0;
                  break;
            case GLFW_KEY_SPACE:
                flbul = 1;
                once = 1;
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            case GLFW_KEY_LEFT_CONTROL:
            	  flred1 = 1;
                  break;
            case GLFW_KEY_LEFT_ALT:
            	  flgreen1 = 1;
                  break;
            case GLFW_KEY_RIGHT_CONTROL:
            	  flred1 = 1;
                  break;
            case GLFW_KEY_RIGHT_ALT:
            	  flgreen1 = 1;
                  break;
            case GLFW_KEY_LEFT:
                  flgreen2 = -1;
                  flred2 = -1;
            	  //cout<<"1: "<<flred1<<endl<<"2: "<<flred2<<endl;
                  if(flred1 && flred2!=0) flred = flred2;
                  else flred = 0;
                  if(flgreen1 && flgreen2!=0) flgreen = flgreen2;
                  else flgreen = 0;
                  x_change-=10;
                  check_pan();
                  break;
            case GLFW_KEY_RIGHT:
                  flgreen2 = 1;
                  flred2 = 1;
                  if(flgreen1 && flgreen2!=0) flgreen = flgreen2;
                  else flgreen = 0;
                  if(flred1 && flred2!=0) flred = flred2;
                  else flred = 0;
                  x_change+=10;
                  check_pan();
                  break;
            case GLFW_KEY_N:
                  if(BRICKSPEED>2) BRICKSPEED -= 2;
                  break;
            case GLFW_KEY_M:
                  if(BRICKSPEED<30) BRICKSPEED += 2;
                  break;
            case GLFW_KEY_S:
                   fllaz = 1;
                  break;
            case GLFW_KEY_F:
                   fllaz = -1;
                  break;
            case GLFW_KEY_A:
                  if(angle < 80) angle += 10.0;
                  break;
            case GLFW_KEY_D:
                  if(angle > -80) angle -= 10.0;
                  break;
            case GLFW_KEY_P:
            	if(gameover)
            	{
            		gameover = 0;
            		bulpos.clear();
            		brickpos.clear();
            		mirror.clear();
            		ERRORHIT = 5;
					ERRORCATCH = 5;
					gameover = 0;
					xxx = 0;
					xredbuck = 0,flred=0,redpos = -345,flred1=0,flred2=0;
					xgreenbuck = 0,flgreen=0,greenpos = 345,flgreen1=0,flgreen2=0;
					bulletpos, xbul = -340,flbul = 0,ybul,once = 0;
					canShoot = 1;
					score = 0;
					x_change = 0; //For the camera pan
					y_change = 0; //For the camera pan
					legal = 1;
					BRICKSPEED = 5,BRICKWAITTIME=50;
					x_zoom=400.0f;
					y_zoom=400.0f;
					countBricks = 0,countBul=0;
					laserangle = 0;
					bulletvel = 15;
					bulletvel_black = 5;
					bulletvel_red = 30;
					gunvel = 10;
					zoom_camera = 1;
          said = 0;
					ylazer = 0,fllaz,lazpos = 0,lazpos2 = 0;
				    audio_init();

            	}
            	break;

            default:
                break;

        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
  switch (key) {
    case 'Q':
    case 'q':
            quit(window);
            break;
    default:
      break;
  }
}
int mouse_clicked = 0,keyboard_pressed = 0;
int right_mouse_clicked = 0;
void mouse_click(){
    mouse_clicked=1;
    keyboard_pressed=0;
}


void mouse_release(GLFWwindow* window, int button){ 
    mouse_clicked=0;
    float ratio_zoom = x_zoom/y_zoom;
    glfwGetCursorPos(window,&mouse_x,&mouse_y);
    if((mouse_initial_x*ratio_zoom - x_zoom) > (greenpos - 100*0.5) && (ratio_zoom*mouse_initial_x - x_zoom) < (greenpos + 100*0.5)
      && (-mouse_initial_y+y_zoom) > (-345 -100 *0.5) && (-mouse_initial_y+y_zoom) < (-345 +100 *0.5))
    {
        if((ratio_zoom*mouse_x - x_zoom) > -x_zoom && (ratio_zoom*mouse_x - x_zoom) < x_zoom)
      {    greenpos =  ratio_zoom*mouse_x - x_zoom; highlight_green = 1;}

    }
    else if((mouse_initial_x*ratio_zoom - x_zoom) > (redpos -100*0.5) && (ratio_zoom*mouse_initial_x - x_zoom) < (redpos +100*0.5)
      && (-mouse_initial_y+y_zoom) > (-345 -100*0.5) && (-mouse_initial_y+y_zoom) < (-345 +100*0.5))
    {
        if((ratio_zoom*mouse_x - x_zoom) > -x_zoom && (ratio_zoom*mouse_x - x_zoom) < x_zoom)
          {redpos = ratio_zoom*mouse_x - x_zoom;
          highlight_red = 1; }
    }
    else
    {
      // /cout<<"yes in else\n";
      flbul = 1;
      if(canShoot>=10 && flbul)
        {
          countBul++;
          bulpos.PB(MP(MP(xbul+15,lazpos2+10*fllaz),MP(angle,white)));
          for (int i = 0; i < max(0,countBul-1); ++i)
          {
            if(bulpos[i].F.F >= 420 || bulpos[i].F.S< -440 || bulpos[i].F.S > 440) bulpos.erase(bulpos.begin() + i),countBul--;
          }
          canShoot = 0;
          flbul = 0;
        }
        else
        {
          flbul = 0;
        }
    }
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) {
               glfwGetCursorPos(window, &mouse_initial_x, &mouse_initial_y);
                   float ratio_zoom = x_zoom/y_zoom;

               if((mouse_initial_x*ratio_zoom - x_zoom) > (redpos -100*0.5) && (ratio_zoom*mouse_initial_x - x_zoom) < (redpos +100*0.5)
      && (-mouse_initial_y+y_zoom) > (-345 -100*0.5) && (-mouse_initial_y+y_zoom) < (-345 +100*0.5)) highlight_red = 1;
               	if((mouse_initial_x*ratio_zoom - x_zoom) > (greenpos - 100*0.5) && (ratio_zoom*mouse_initial_x - x_zoom) < (greenpos + 100*0.5)
      && (-mouse_initial_y+y_zoom) > (-345 -100 *0.5) && (-mouse_initial_y+y_zoom) < (-345 +100 *0.5)) highlight_green = 1;
               mouse_click();
            }
            if (action == GLFW_RELEASE) {
                mouse_release(window,button);
                highlight_red = 0;
                highlight_green = 0;
            }
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS) {
                right_mouse_clicked=1;
            }
            if (action == GLFW_RELEASE) {
                right_mouse_clicked=0;
            }
            break;
        default:
            break;
    }
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

  GLfloat fov = 90.0f;

  // sets the viewport of openGL renderer
  glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

  // set the projection matrix as perspective
  /* glMatrixMode (GL_PROJECTION);
     glLoadIdentity ();
     gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
  // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    Matrices.projection = glm::ortho(-400.0f/zoom_camera, 400.0f/zoom_camera, -400.0f/zoom_camera, 400.0f/zoom_camera, 0.1f, 500.0f);

//    Matrices.projection = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 500.0f);
}

VAO *triangle, *trapezium;

// Creates the triangle object used in this sample code
void createTriangle ()
{
  /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

  /* Define vertex array as used in glBegin (GL_TRIANGLES) */
  static const GLfloat vertex_buffer_data [] = {
    0, 1,0, // vertex 0
    -1,-1,0, // vertex 1
    1,-1,0, // vertex 2
  };

  static const GLfloat color_buffer_data [] = {
    1,0,0, // color 0
    0,1,0, // color 1
    0,0,1, // color 2
  };

  // create3DObject creates and returns a handle to a VAO that can be used later
  triangle = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_LINE);
}

void createTrapezium()
{
    static const GLfloat vertex_buffer_data [] = {
    -1.2,-1,0, // vertex 1
    1.2,-1,0, // vertex 2
    0.6, 1,0, // vertex 3

    0.6, 1,0, // vertex 3
    -0.6, 1,0, // vertex 4
    -1.2,-1,0  // vertex 1
  };
  static const GLfloat color_buffer_data [] = {
    1,0,0, // color 1
    0,0,1, // color 2
    0,1,0, // color 3

    0,1,0, // color 3
    0.3,0.3,0.3, // color 4
    1,0,0  // color 1
  };

  // create3DObject creates and returns a handle to a VAO that can be used later
  trapezium = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);

}

// Creates the rectangle object used in this sample code
VAO * createRectangle (string name, float weight, COLOR colorA, COLOR colorB, COLOR colorC, COLOR colorD, float x, float y, float height, float width, string component)
{
    // GL3 accepts only Triangles. Quads are not supported
    float w=width/2,h=height/2;
    GLfloat vertex_buffer_data [] = {
        -w,-h,0, // vertex 1
        -w,h,0, // vertex 2
        w,h,0, // vertex 3

        w,h,0, // vertex 3
        w,-h,0, // vertex 4
        -w,-h,0  // vertex 1
    };

    GLfloat color_buffer_data [] = {
        colorA.r,colorA.g,colorA.b, // color 1
        colorB.r,colorB.g,colorB.b, // color 2
        colorC.r,colorC.g,colorC.b, // color 3

        colorC.r,colorC.g,colorC.b, // color 4
        colorD.r,colorD.g,colorD.b, // color 5
        colorA.r,colorA.g,colorA.b // color 6
    };

  // cout<<"create3dobject called\n";
  VAO *rectangle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
  dimension[component] = MP(height,width);
  dimension[name] = MP(height,width);
  return rectangle;
}
VAO * createCircle (string name, COLOR color, float r, int NoOfParts, string component, int fill)
{
	// cout<<name<<" yes in circle\n";
    int parts = NoOfParts;
    float radius = r;
    GLfloat vertex_buffer_data[parts*9];
    GLfloat color_buffer_data[parts*9];
    int i,j;
    float angle=(2*M_PI/parts);
    float current_angle = 0;
    for(i=0;i<parts;i++){
        for(j=0;j<3;j++){
            color_buffer_data[i*9+j*3]=color.r;
            color_buffer_data[i*9+j*3+1]=color.g;
            color_buffer_data[i*9+j*3+2]=color.b;
        }
        vertex_buffer_data[i*9]=0;
        vertex_buffer_data[i*9+1]=0;
        vertex_buffer_data[i*9+2]=0;
        vertex_buffer_data[i*9+3]=radius*cos(current_angle);
        vertex_buffer_data[i*9+4]=radius*sin(current_angle);
        vertex_buffer_data[i*9+5]=0;
        vertex_buffer_data[i*9+6]=radius*cos(current_angle+angle);
        vertex_buffer_data[i*9+7]=radius*sin(current_angle+angle);
        vertex_buffer_data[i*9+8]=0;
        current_angle+=angle;
    }
    VAO* circle;
    if(fill==1)
        circle = create3DObject(GL_TRIANGLES, (parts*9)/3, vertex_buffer_data, color_buffer_data, GL_FILL);
    else
        circle = create3DObject(GL_TRIANGLES, (parts*9)/3, vertex_buffer_data, color_buffer_data, GL_LINE);
    // cout<<"circle created\n";
    dimension[name].F = r;
    dimension[name].S = r;
    return circle;
}
float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
//pair<float,float> pos[100];

int detectCollision(int NoOfBullets, pair<pair<float,float> , COLOR > bri)
{

  if(bri.S.r==gold.r && bri.S.g==gold.g && bri.S.b==gold.b) return 0;
  float h1 = dimension["brick"].F;
  float w1 = dimension["brick"].S;
  float h2 = dimension["bullet"].F;
  float w2 = dimension["bullet"].S;
  for (int i = 0; i < NoOfBullets; ++i)
  {
    float thieta = bulpos[i].S.F*(M_PI)/180.0f;
    if(((w1+w2*cos(thieta)) >= 2.0f*abs(bri.F.F - bulpos[i].F.F)) && ((h1+w2*sin(thieta)) >= 2.0f*abs(bri.F.S - bulpos[i].F.S)))
    {
	  if(bri.S.r==red.r && bri.S.g==red.g && bri.S.b==red.b) ERRORHIT--,score--;
	  if(bri.S.r==green.r && bri.S.g==green.g && bri.S.b==green.b) ERRORHIT--,score--;
	  if(bri.S.r==black.r && bri.S.g==black.g && bri.S.b==black.b) score += 3;
	  // cout<<"yesL errorhit: "<<ERRORHIT<<endl;
	  if(ERRORHIT<=0) gameover = 1;
    	// if(bri)
      if(bulpos[i].S.S.r==white.r && bulpos[i].S.S.g==white.g && bulpos[i].S.S.b==white.b) bulpos.erase(bulpos.begin() + i),countBul--;
      return 1;
    }

  }
  return 0;

}
int detectMirror(int x_bul,int y_bul,float ang_bul)
{
  float tipx = x_bul + 15.0*cos(ang_bul*M_PI/180.0f);
  float tipy = y_bul + 15.0*sin(ang_bul*M_PI/180.0f);
  // cout<<"tipx"<<tipx<<endl;
  for (int i = 0; i < 4; ++i)
  {
    float x = mirror[i].F.F;
    float y = mirror[i].F.S;
    float thieta = 1.0*mirror[i].S*M_PI/180.0f;
    // cout<<cos(thieta)<<" "<<sin(thieta)<<endl;
    float param1 = (tipx - x)/cos(thieta);
    float param2 = (tipy - y)/sin(thieta);
    // cout<<param1<<" "<<param2<<endl;
    if(param1 > -35.0 && param1 < 35.0 && param2 > -35.0 && param2 < 35.0 && abs(param1-param2) <=(15.0f))
    {
      return i;
    }
  }
  return -1;  
}

void createReqObj(VAO * obj, float x,float y, float angle, glm::mat4 VP)
{
	glm::mat4 MVP;  // MVP = Projection * View * Model
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translateRectangle = glm::translate (glm::vec3(x,y,0));        // glTranslatef
	glm::mat4 rotateRectangle = glm::rotate((float)(angle*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
	Matrices.model *= (translateRectangle * rotateRectangle);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	// VAO *rect = createRectangle ("redbuck", 100, redbuck,redbuck,redbuck,redbuck, 100, 100, 40, 40, "bucket");
  	draw3DObject(obj);
	return;
}
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw (GLFWwindow* window)
{

  // clear the color and depth in the frame buffer
    glfwGetCursorPos(window, &new_mouse_pos_x, &new_mouse_pos_y);
    if(right_mouse_clicked==1){
        x_change+=new_mouse_pos_x-mouse_pos_x;
        y_change-=new_mouse_pos_y-mouse_pos_y;
        check_pan();
    }
  Matrices.projection = glm::ortho((float)(-400.0f/zoom_camera+x_change), (float)(400.0f/zoom_camera+x_change), (float)(-400.0f/zoom_camera+y_change), (float)(400.0f/zoom_camera+y_change), 0.1f, 500.0f);
  glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // use the loaded shader program
  // Don't change unless you know what you are doing
  glUseProgram (programID);

  // Eye - Location of camera. Don't change unless you are sure!!
  glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (0, 0, 0);
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);

  // Compute Camera matrix (view)
  // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
  //  Don't change unless you are sure!!
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  //first vector is where our eye is, second is where you wanna look, third is the up direction of our perspective to look
  // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
  //  Don't change unless you are sure!!
  glm::mat4 VP = Matrices.projection * Matrices.view;

  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  //  Don't change unless you are sure!!
  glm::mat4 MVP;  // MVP = Projection * View * Model

  // Load identity to model matrix
  Matrices.model = glm::mat4(1.0f);

  /* Render your scene */

  glm::mat4 translateTriangle = glm::translate (glm::vec3(-2.0f, 0.0f, 0.0f)); // glTranslatef
  glm::mat4 rotateTriangle = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
  glm::mat4 rotateRectangle = glm::rotate((float)(triangle_rotation*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
  glm::mat4 triangleTransform = translateTriangle * rotateTriangle;
  Matrices.model *= triangleTransform; 
  MVP = VP * Matrices.model; // MVP = p * V * M

  //  Don't change unless you are sure!!
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  float xx,yy;
  // draw3DObject draws the VAO given to it using current MVP matrix
  Matrices.model = glm::mat4(1.0f);
  glm::mat4 translateRectangle;
  if(!gameover)
  {



  if((greenpos + 10*flgreen) >= -345 && (greenpos + 10*flgreen) <= 345) {xx = greenpos+10*flgreen, yy=-345;greenpos = greenpos + 10*flgreen;}
  else {xx=greenpos;yy=-345;}

  if(highlight_green) createReqObj(greenbuck2O,xx,yy,0,VP);
  else createReqObj(greenbuckO,xx,yy,0,VP);

  Matrices.model = glm::mat4(1.0f);
  
  if((redpos + 10*flred) >= -345 && (redpos + 10*flred) <= 345) {xx=redpos+10*flred;yy=-345; redpos = redpos + 10*flred;}
  else {xx=redpos;yy=-345;}
  // VAO *rect = createRectangle ("redbuck", 100, redbuck,redbuck,redbuck,redbuck, 100, 100, 40, 40, "bucket");
  if(highlight_red) createReqObj(redbuck2O,xx,yy,0,VP);
  else createReqObj(redbuckO,xx,yy,0,VP);
  //WHITE LINE
  createReqObj(whitelineO,0,-289,0,VP);

  mirror.PB(MP(MP(150.0,200.0),-60.0));
  createReqObj(mirrorO1,150,200,-60,VP);
  
  // VAO *rect = createRectangle ("redbuck", 100, redbuck,redbuck,redbuck,redbuck, 100, 100, 40, 40, "bucket");
  mirror.PB(MP(MP(150.0,-100.0),60.0));
  createReqObj(mirrorO2, 150,-100,60,VP);

  // VAO *rect = createRectangle ("redbuck", 100, redbuck,redbuck,redbuck,redbuck, 100, 100, 40, 40, "bucket");
  mirror.PB(MP(MP(-150.0,-100.0),30.0));
  createReqObj(mirrorO3, -150, -100, 30, VP);

  mirror.PB(MP(MP(-150.0,200.0),-30.0));
  createReqObj(mirrorO4, -150,200,-30,VP);



  Matrices.model = glm::mat4(1.0f);
  if((float)(lazpos2 + 10*fllaz+1.0*15*sin(1.0*angle*M_PI/180.0f)) >= -270 && (float)(lazpos2 + 10*fllaz+15*sin(1.0*angle*M_PI/180.0f) < 385)
   && (float)(lazpos + 10*fllaz >= -270) && (float)(lazpos+10*fllaz < 385)) 
  {
    xx=-335 + 1.0*15*(cos(1.0*angle*M_PI/180.0f) - 1);
    yy=lazpos2 + 10*fllaz+1.0*15*sin(1.0*angle*M_PI/180.0f);
    createReqObj(lazerO2,xx,yy,angle,VP);

    xx = -375;
    yy = lazpos + 10*fllaz;
    createReqObj(lazerO1,xx,yy,0,VP);

    lazpos += 10*fllaz;
    lazpos2 += 10*fllaz;
  }
  else 
  {
    xx = -335 + 1.0*15*(cos(1.0*angle*M_PI/180.0f) - 1); 
    yy = lazpos2 +1.0*15*sin(1.0*angle*M_PI/180.0f);
    createReqObj(lazerO2,xx,yy,angle,VP);

    xx=-375;
    yy=lazpos;
    createReqObj(lazerO1,xx,yy,0,VP);
  }

  for (int i = 0; i < brickpos.size(); ++i)
  {

    Matrices.model = glm::mat4(1.0f);
    if(detectCollision(countBul,brickpos[i])) {brickpos.erase(brickpos.begin() + i); countBricks--;}

    xx=brickpos[i].F.F;
    yy=brickpos[i].F.S;
    if(brickpos[i].S.r==red.r && brickpos[i].S.g==red.g && brickpos[i].S.b==red.b) createReqObj(redbrick,xx,yy,0,VP);
    if(brickpos[i].S.r==green.r && brickpos[i].S.g==green.g && brickpos[i].S.b==green.b) createReqObj(greenbrick,xx,yy,0,VP);
    if(brickpos[i].S.r==black.r && brickpos[i].S.g==black.g && brickpos[i].S.b==black.b) createReqObj(blackbrick,xx,yy,0,VP);
    if(brickpos[i].S.r==gold.r && brickpos[i].S.g==gold.g && brickpos[i].S.b==gold.b) createReqObj(gold_brick,xx,yy,0,VP);
    //if(brickpos[i].S.r==darkpink.r && brickpos[i].S.g==darkpink.g && brickpos[i].S.b==darkpink.b) draw3DObject(spc_blackbrick);
  }
  int reflect = -1;
  for (int i = 0; i < countBul; ++i)
  {
    float angle_from_mirr,ang_mir;
    float myangle;
    reflect = detectMirror(bulpos[i].F.F+1.0*15*(cos(myangle*1.0*M_PI/180.0f)-1),bulpos[i].F.S+ 25*sin(myangle*1.0*M_PI/180.0f),bulpos[i].S.F);
    if(reflect==-1) myangle = bulpos[i].S.F;
    else { 
      ang_mir = mirror[i].S;  
      myangle = 1.0*(2*ang_mir - bulpos[i].S.F);
      if(reflect==1 || reflect==2) myangle *= -1.0; 
      bulpos[i].S.F = myangle; 
      if(bulpos[i].S.S.r==black.r && bulpos[i].S.S.g==black.g && bulpos[i].S.S.b==black.b) {bulpos.erase(bulpos.begin() + i); countBul--;i--;continue;}
     // if(reflect==1) bulpos[i].S.F *= -1.0;
    }
    // cout<<"myangle"<<myangle
    Matrices.model = glm::mat4(1.0f);
    if(bulpos[i].S.S.r==black.r && bulpos[i].S.S.g==black.g && bulpos[i].S.S.b==black.b) 
    {
    	translateRectangle = glm::translate (glm::vec3((float)(bulpos[i].F.F+1.0*bulletvel_black*cos(myangle*1.0*M_PI/180.0f)+1.0*15*(cos(myangle*1.0*M_PI/180.0f)-1)),
    	(float)(bulpos[i].F.S+ 1.0*bulletvel_black*sin(myangle*1.0*M_PI/180.0f)+25*sin(myangle*1.0*M_PI/180.0f)),0));
    	bulpos[i].F.F += 1.0*bulletvel_black*cos(1.0*myangle*M_PI/180.0f);
    	bulpos[i].F.S += 1.0*bulletvel_black*sin(1.0*myangle*M_PI/180.0f);
    }
    else if(bulpos[i].S.S.r==red.r && bulpos[i].S.S.g==red.g && bulpos[i].S.S.b==red.b)
    {
    	translateRectangle = glm::translate (glm::vec3((float)(bulpos[i].F.F+1.0*bulletvel_red*cos(myangle*1.0*M_PI/180.0f)+1.0*15*(cos(myangle*1.0*M_PI/180.0f)-1)),
    	(float)(bulpos[i].F.S+ 1.0*bulletvel_red*sin(myangle*1.0*M_PI/180.0f)+25*sin(myangle*1.0*M_PI/180.0f)),0));
    	bulpos[i].F.F += 1.0*bulletvel_red*cos(1.0*myangle*M_PI/180.0f);
    	bulpos[i].F.S += 1.0*bulletvel_red*sin(1.0*myangle*M_PI/180.0f);

    }
    else
    {
    	translateRectangle = glm::translate (glm::vec3((float)(bulpos[i].F.F+1.0*bulletvel*cos(myangle*1.0*M_PI/180.0f)+1.0*15*(cos(myangle*1.0*M_PI/180.0f)-1)),
    	(float)(bulpos[i].F.S+ 1.0*bulletvel*sin(myangle*1.0*M_PI/180.0f)+25*sin(myangle*1.0*M_PI/180.0f)),0));
    	bulpos[i].F.F += 1.0*bulletvel*cos(1.0*myangle*M_PI/180.0f);
    	bulpos[i].F.S += 1.0*bulletvel*sin(1.0*myangle*M_PI/180.0f);

    }
    rotateRectangle = glm::rotate((float)(myangle*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1)
    Matrices.model *= (translateRectangle * rotateRectangle);
    MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(bulpos[i].S.S.r==red.r && bulpos[i].S.S.g==red.g && bulpos[i].S.S.b==red.b) draw3DObject(bullet1);
    else if(bulpos[i].S.S.r==black.r && bulpos[i].S.S.g==black.g && bulpos[i].S.S.b==black.b) draw3DObject(bullet2);
    else draw3DObject(bulletO);

  }
 }
 else
 {
 	  brickpos.clear();
 	  bulpos.clear();
 	  // xxx++;
 	  if(!said) cout<<"Game is over, press 'P' to play again: "<<endl,said = 1;
  	  char gameover[] = {'Y','O','U','L','O','S','E'};
  	  int start_points = 340;
	  int points_offset = 340;
	  for (int i = 6; i >= 0; --i)
	  {
		setStrokes(gameover[6-i]);
		if(sevenseg["top"])
	    {
	      createReqObj(topO_big,start_points - 40 -100*i,385-300,0,VP);
	    }
	    if(sevenseg["left1"])
	    {
	      createReqObj(left1O_big,start_points - 80 -100*i,365-300-20,0,VP);
	    }
	    if(sevenseg["right1"])
	    {

	      createReqObj(right1O_big,start_points-100*i,365-300-20,0,VP);
	    }  
	    if(sevenseg["middle"])
	    {

	      createReqObj(middleO_big,start_points- 40 -100*i,345-300-40,0,VP);
	    }
	    if(sevenseg["middle1"])
	    {

	      createReqObj(middle1O_big,start_points- 40 -100*i,365-300-20,0,VP);
	    }
	    if(sevenseg["middle2"])
	    {

	      createReqObj(middle2O_big,start_points- 40 -100*i,325-300-60,0,VP);
	    }
	    if(sevenseg["left2"])
	    {

	      createReqObj(left2O_big,start_points - 80 -100*i,325-300-60,0,VP);
	    }
	    if(sevenseg["right2"])
	    {

	      createReqObj(right2O_big,start_points-100*i,325-300-60,0,VP);
	    }
	    if(sevenseg["bottom"])
	    {

	      createReqObj(bottomO_big,start_points-100*i-40,305-300-80,0,VP);
	    }
	}

 }

  if(score<0) score = 0;
  int copy = score;
  int place = 0;
  int start_points = 350;
  while(1)
  {
    setStrokes(copy%10  + '0');
    if(sevenseg["top"])
    {
      createReqObj(topO,370-50*place,385,0,VP);
    }
    
    if(sevenseg["left1"]){
      createReqObj(left1O,350-50*place,365,0,VP);
    }
    if(sevenseg["right1"])
    {
      createReqObj(right1O,390-50*place,365,0,VP);
    }  
    if(sevenseg["middle"])
    {
      createReqObj(middleO,370-50*place,345,0,VP);
    }
    if(sevenseg["left2"])
    {
      createReqObj(left2O,350-50*place,325,0,VP);
    }
    if(sevenseg["right2"])
    {
      createReqObj(right2O,390-50*place,325,0,VP);
    }
    if(sevenseg["bottom"])
    {
      createReqObj(bottomO,370-50*place,305,0,VP);
    }
    copy /= 10;
    place++;
    start_points = 350 - 50*place;
    if(copy<=0) break;
  }
 
  char points[] = {'P','O','I','N','T','S'};
  int points_offset = start_points;
  for (int i = 5; i >= 0; --i)
  {
	setStrokes(points[5-i]);
	if(sevenseg["top"])
    {
      createReqObj(topO,start_points - 20 -50*i,385,0,VP);
    }
    if(sevenseg["left1"])
    {
      createReqObj(left1O,start_points - 40 -50*i,365,0,VP);
    }
    if(sevenseg["right1"])
    {

      createReqObj(right1O,start_points-50*i,365,0,VP);
    }  
    if(sevenseg["middle"])
    {

      createReqObj(middleO,start_points- 20 -50*i,345,0,VP);
    }
    if(sevenseg["middle1"])
    {

      createReqObj(middle1O,start_points- 20 -50*i,365,0,VP);
    }
    if(sevenseg["middle2"])
    {

      createReqObj(middle2O,start_points- 20 -50*i,325,0,VP);
    }
    if(sevenseg["left2"])
    {

      createReqObj(left2O,start_points - 40 -50*i,325,0,VP);
    }
    if(sevenseg["right2"])
    {

      createReqObj(right2O,start_points-50*i,325,0,VP);
    }
    if(sevenseg["bottom"])
    {

      createReqObj(bottomO,start_points-50*i-20,305,0,VP);
    }

  }

	{
  	  
	  int copy = score;
	  int place = 0;
	  start_points = 350;
	  while(1)
	  {
	    setStrokes(copy%10  + '0');
	    if(sevenseg["top"])
	    {
	      createReqObj(topO,370-50*place,385,0,VP);
	    }
	    if(sevenseg["left1"])
	    {
	      createReqObj(left1O,350-50*place,365,0,VP);
	    }
	    if(sevenseg["right1"])
	    {

	      createReqObj(right1O,390-50*place,365,0,VP);
	    }  
	    if(sevenseg["middle"])
	    {

	      createReqObj(middleO,370-50*place,345,0,VP);
	    }
	    if(sevenseg["left2"])
	    {

	      createReqObj(left2O,350-50*place,325,0,VP);
	    }
	    if(sevenseg["right2"])
	    {

	      createReqObj(right2O,390-50*place,325,0,VP);
	    }
	    if(sevenseg["bottom"])
	    {

	      createReqObj(bottomO,370-50*place,305,0,VP);
	    }
	    copy /= 10;
	    place++;
	    start_points = 350 - 50*place;
	    if(copy<=0) break;
	  }
  }
  
}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
//        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Brick Breaker", NULL, NULL);

    if (!window) {
        glfwTerminate();
//        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
    glfwSetScrollCallback(window, mousescroll); // mouse scroll


    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
  // Create the models
  createTriangle (); // Generate the VAO, VBOs, vertices data & copy into the array buffer
  redbuckO = createRectangle("redbuck", 100, red,red,red,red, 100, 100, 100, 100, "bucket");
  redbuck2O = createRectangle("redbuck", 100, lightred,lightred,lightred,lightred, 100, 100, 100, 100, "bucket");
  greenbuckO = createRectangle("greenbuck", 100, green,green,green,green, 100, 100, 100, 100, "bucket");
  greenbuck2O = createRectangle("greenbuck", 100, lightgreen,lightgreen,lightgreen,lightgreen, 100, 100, 100, 100, "bucket");
  blacklineO = createRectangle("black", 100, black,black,black,black, 100, 100, 2, width, "division");
  whitelineO = createRectangle("white", 100, white,white,white,white, 100, 100, 2, width, "division");
  lazerO1 = createRectangle("base", 100, blue,blue,blue,blue, 100, 100, 30,50, "lazer");
  lazerO2 = createRectangle("dir", 100, bluegreen,bluegreen,bluegreen,bluegreen, 100, 100, 12,30, "lazer");
  redbrick = createRectangle("redbrick", 100, red,red,red,red, 100, 100, 50, 20, "brick");
  greenbrick = createRectangle("greenbrick", 100, green,green,green,green, 100, 100, 50, 20, "brick");
  blackbrick = createRectangle("blackbrick", 100, black,black,black,black, 100, 100, 50, 20, "brick");
  // pink_brick = createRectangle("blackbrick", 100, darkpink,darkpink,darkpink,darkpink, 100, 100, 50, 20, "brick");
  gold_brick = createCircle("goldbrick", gold ,20 ,360, "brick",1);
  draw3DObject(gold_brick);
  bulletO = createRectangle("bullet", 100, white,white,white,white, 100, 100, 5, 30, "bullet");
  bullet1 = createRectangle("bullet", 100, red,blue,blue,red, 100, 100, 5, 30, "bullet");
  bullet2 = createRectangle("bullet", 100, black,black,black,black, 100, 100, 5, 30, "bullet");
  mirrorO1 = createRectangle("mirror1", 100, lightpink,lightpink,lightpink,lightpink, 100, 100, 2, 70, "mirror");
  mirrorO4 = createRectangle("mirror4", 100, lightpink,lightpink,lightpink,lightpink, 100, 100, 2, 70, "mirror");
  mirrorO3 = createRectangle("mirror3", 100, lightpink,lightpink,lightpink,lightpink, 100, 100, 2, 70, "mirror");
  mirrorO2 = createRectangle("mirror2", 100, lightpink,lightpink,lightpink,lightpink, 100, 100, 2, 70, "mirror");
  // void createCircle (string name, COLOR color, float r, int NoOfParts, string component, int fill)

  COLOR color = black;
  int offset = 100;
  float seg_h = 2;
  float seg_w = 30;
  string layer = "sevenseg";
  topO = createRectangle("top",100000,color,color,color,color,0,offset,seg_h,seg_w,layer);
  bottomO = createRectangle("bottom",100000,color,color,color,color,0,-offset,seg_h,seg_w,layer);
  middleO = createRectangle("middle",100000,color,color,color,color,0,0,seg_h,seg_w,layer);
  left1O = createRectangle("left1",100000,color,color,color,color,-offset/2,offset/2,seg_w,seg_h,layer);
  left2O = createRectangle("left2",100000,color,color,color,color,-offset/2,-offset/2,seg_w,seg_h,layer);
  right1O = createRectangle("right1",100000,color,color,color,color,offset/2,offset/2,seg_w,seg_h,layer);
  right2O = createRectangle("right2",100000,color,color,color,color,offset/2,-offset/2,seg_w,seg_h,layer);
  middle1O = createRectangle("middle1",100000,color,color,color,color,0,offset/2,seg_w,seg_h,layer);
  middle2O = createRectangle("middle2",100000,color,color,color,color,0,-offset/2,seg_w,seg_h,layer);

  color = red;
  seg_h = 10;
  seg_w = 60;
  layer = "youlose";
  topO_big = createRectangle("top",100000,color,color,color,color,0,offset,seg_h,seg_w,layer);
  bottomO_big = createRectangle("bottom",100000,color,color,color,color,0,-offset,seg_h,seg_w,layer);
  middleO_big = createRectangle("middle",100000,color,color,color,color,0,0,seg_h,seg_w,layer);
  left1O_big = createRectangle("left1",100000,color,color,color,color,-offset/2,offset/2,seg_w,seg_h,layer);
  left2O_big = createRectangle("left2",100000,color,color,color,color,-offset/2,-offset/2,seg_w,seg_h,layer);
  right1O_big = createRectangle("right1",100000,color,color,color,color,offset/2,offset/2,seg_w,seg_h,layer);
  right2O_big = createRectangle("right2",100000,color,color,color,color,offset/2,-offset/2,seg_w,seg_h,layer);
  middle1O_big = createRectangle("middle1",100000,color,color,color,color,0,offset/2,seg_w,seg_h,layer);
  middle2O_big = createRectangle("middle2",100000,color,color,color,color,0,-offset/2,seg_w,seg_h,layer);


	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

	
	reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
  int width = 800;
  int height = 800;

  GLFWwindow* window = initGLFW(width, height);

  initGL (window, width, height);

    double last_update_time = glfwGetTime(), current_time;
    
    audio_init();
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {

        // OpenGL Draw commands
        draw(window);
        if(!gameover) audio_play();
        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();
        //glfwGetCursorPos(window,&mouse_x,&mouse_y);

        int rx = rand()%740;
        while((rx-310+50-20 < 150+35 && rx-310+50+20 > 150-35) || (rx-310+50-20 < -150+35 && rx-310+50+20 > -150-35)) { rx = rand()%740;}
        int rcol = rand()%3;
        int spc_brick = rand()%10;
        int bul_color = rand()%5;
        glfwGetCursorPos(window,&mouse_x,&mouse_y);
       //  cout<<mouse_x<<" "<<mouse_y<<endl;
        mouse_x = mouse_x - 50;
        mouse_y = mouse_y + lazpos2 - 400;
        angle = -1.0*(float)(atan(mouse_y/mouse_x));
        angle = 1.0*angle*180/M_PI;
      //  cout<<angle<<"<-angle\n";
        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 0.05) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds .
            if(gameover) ;
            else
            {

            canShoot += 1;
            c++;
            if(c >= BRICKWAITTIME)
            {
              for (int i = 0; i < 4; ++i)
              {
                float r_tip,l_tip;
                r_tip = mirror[i].F.F + 35.0;
                l_tip = mirror[i].F.F - 35.0;
                if((-310+rx+50) < r_tip && (-310+rx+50) > l_tip) {legal = 0;break;}
                /* code */
              }
              if(legal)
              {
                countBricks++;
                if(spc_brick!=0)
                {
	                brickpos.PB(MP(MP(-310+rx+50,380),red));
	                if(rcol==1) brickpos[countBricks-1].S = green;
	                if(rcol==2) brickpos[countBricks-1].S = black;
	                c=0;
                }
                else
                {
//	                if(rcol==1) brickpos[countBricks-1].S = darkpink;
	                brickpos.PB(MP(MP(-310+rx+50,380),gold));
                }
              }
            }
            for (int i = 0; i < max(0,countBricks-1); ++i)
            {
              brickpos[i].F.S-=BRICKSPEED;
              string name;
                if(brickpos[i].S.r==red.r && brickpos[i].S.g==red.g && brickpos[i].S.b==red.b) name="redbrick";
                if(brickpos[i].S.r==green.r && brickpos[i].S.g==green.g && brickpos[i].S.b==green.b) name="greenbrick";
                if(brickpos[i].S.r==black.r && brickpos[i].S.g==black.g && brickpos[i].S.b==black.b) name="blackbrick";
                if(brickpos[i].S.r==gold.r && brickpos[i].S.g==gold.g && brickpos[i].S.b==gold.b) name="goldbrick";
              if(brickpos[i].F.S - dimension[name].F <(-330)){
             //   cout<<"red "<<redpos<<" "<<"green "<<greenpos<<endl;
               // cout<<"brick x: "<<brickpos[i].F.F<<endl;
                if(brickpos[i].S.r==red.r && brickpos[i].S.g==red.g && brickpos[i].S.b==red.b) 
                  {
                    if(((brickpos[i].F.F - 10) >= (redpos - 50) && (brickpos[i].F.F + 10) <= (redpos+50))
                      || (((brickpos[i].F.F+10 >= (redpos-50))) && (brickpos[i].F.F-10 <= (redpos-50)))
                      || (((brickpos[i].F.F+10 >= (redpos+50))) && (brickpos[i].F.F-10 <= (redpos+50)))) score+=1/*,cout<<"redpoints: "<<score<<endl*/; 
                  }
                if(brickpos[i].S.r==green.r && brickpos[i].S.g==green.g && brickpos[i].S.b==green.b)  
                  {
                    if(((brickpos[i].F.F - 10) >= (greenpos - 50) && (brickpos[i].F.F + 10) <= (greenpos+50))
                      || (((brickpos[i].F.F+10 >= (greenpos-50))) && (brickpos[i].F.F-10 <= (greenpos-50)))
                      || (((brickpos[i].F.F+10 >= (greenpos+50))) && (brickpos[i].F.F-10 <= (greenpos+50)))) score+=1/*,cout<<"greenpoints: "<<score<<endl*/;
                  }
                if(brickpos[i].S.r==black.r && brickpos[i].S.g==black.g && brickpos[i].S.b==black.b) 
                {
      
                    if(((brickpos[i].F.F - 10) >= (redpos - 50) && (brickpos[i].F.F + 10) <= (redpos+50))
                      || (((brickpos[i].F.F+10 >= (redpos-50))) && (brickpos[i].F.F-10 <= (redpos-50)))
                      || (((brickpos[i].F.F+10 >= (redpos+50))) && (brickpos[i].F.F-10 <= (redpos+50)))
                      || ((brickpos[i].F.F - 10) >= (greenpos - 50) && (brickpos[i].F.F + 10) <= (greenpos+50))
                      || (((brickpos[i].F.F+10 >= (greenpos-50))) && (brickpos[i].F.F-10 <= (greenpos-50)))
                      || (((brickpos[i].F.F+10 >= (greenpos+50))) && (brickpos[i].F.F-10 <= (greenpos+50)))){
                      	gameover = 1;
                  }
               }
               if(brickpos[i].S.r==gold.r && brickpos[i].S.g==gold.g && brickpos[i].S.b==gold.b) 
                {
      
                    if(((brickpos[i].F.F - 10) >= (redpos - 50) && (brickpos[i].F.F + 10) <= (redpos+50))
                      || (((brickpos[i].F.F+10 >= (redpos-50))) && (brickpos[i].F.F-10 <= (redpos-50)))
                      || (((brickpos[i].F.F+10 >= (redpos+50))) && (brickpos[i].F.F-10 <= (redpos+50)))
                      || ((brickpos[i].F.F - 10) >= (greenpos - 50) && (brickpos[i].F.F + 10) <= (greenpos+50))
                      || (((brickpos[i].F.F+10 >= (greenpos-50))) && (brickpos[i].F.F-10 <= (greenpos-50)))
                      || (((brickpos[i].F.F+10 >= (greenpos+50))) && (brickpos[i].F.F-10 <= (greenpos+50)))) score+=5;/*cout<<"Game over:score "<<score<<endl;*/
               }
               if(brickpos[i].S.r==green.r && brickpos[i].S.g==green.g && brickpos[i].S.b==green.b) 
                  {
                    if(((brickpos[i].F.F - 10) >= (redpos - 50) && (brickpos[i].F.F + 10) <= (redpos+50))
                      || (((brickpos[i].F.F+10 >= (redpos-50))) && (brickpos[i].F.F-10 <= (redpos-50)))
                      || (((brickpos[i].F.F+10 >= (redpos+50))) && (brickpos[i].F.F-10 <= (redpos+50)))) {
                    ERRORCATCH--;
                    if(ERRORCATCH==0) gameover = 1;
                    }
                  }
                if(brickpos[i].S.r==red.r && brickpos[i].S.g==red.g && brickpos[i].S.b==red.b)  
                  {
                    if(((brickpos[i].F.F - 10) >= (greenpos - 50) && (brickpos[i].F.F + 10) <= (greenpos+50))
                      || (((brickpos[i].F.F+10 >= (greenpos-50))) && (brickpos[i].F.F-10 <= (greenpos-50)))
                      || (((brickpos[i].F.F+10 >= (greenpos+50))) && (brickpos[i].F.F-10 <= (greenpos+50)))) {
                      	ERRORCATCH--;
                    	if(ERRORCATCH==0) gameover = 1;
                    }
                  }
                brickpos.erase(brickpos.begin() + i),countBricks--;
              }
            }
            }
            last_update_time = current_time;
        }
        for (int i = 0; i < max(0,countBul); ++i)
        {
          // cout<<bulpos[i].F.F<<"x coordi\n";
          if(bulpos[i].F.F >= 420 || bulpos[i].F.S< -440 || bulpos[i].F.S > 440) bulpos.erase(bulpos.begin() + i),countBul--;
        }
        if(canShoot>=10 && flbul)
        {
	    	if(!gameover)
	    	{

	          countBul++;
	          // cout<<"rand = "<<bul_color<<endl;
	          if(bul_color!=0 && bul_color!=1) bulpos.PB(MP(MP(xbul+15,lazpos2+10*fllaz),MP(angle,white)));
	          else if(bul_color==1) bulpos.PB(MP(MP(xbul+15,lazpos2+10*fllaz),MP(angle,red)));
	          else if(bul_color==0) bulpos.PB(MP(MP(xbul+15,lazpos2+10*fllaz),MP(angle,black)));
	          for (int i = 0; i < max(0,countBul-1); ++i)
	          {
	            if(bulpos[i].F.F >= 420 || bulpos[i].F.S< -440 || bulpos[i].F.S > 440) bulpos.erase(bulpos.begin() + i),countBul--;
	          }
	          canShoot = 0;
	          flbul = 0;
	        }
	        else
	        {
	          flbul = 0;
	        }
    	}
    }
    // audio_close();
    glfwTerminate();
//    exit(EXIT_SUCCESS);
}
