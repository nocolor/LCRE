#include <iostream>
#define  MAX_SHADER_LENGTH 8192

#include <GL/glew.h>
#include <GL/glfw3.h>

#include "qyt/GLSLProgram.hpp"
#include "qyt/batch.hpp"
#include "qyt/FileUtils.hpp"

GLFWwindow* window;

GLuint loadBMP_custom(const char * imagepath)
{

    printf("Reading image %s\n", imagepath);

    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    unsigned char * data;

    FILE * file = fopen(imagepath,"rb");
    if (!file)
    {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
        getchar();
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
    if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    if (imageSize==0)    imageSize=width*height*3;
    if (dataPos==0)      dataPos=54;

    data = new unsigned char [imageSize];

    fread(data,1,imageSize,file);
    fclose (file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    delete [] data;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

qyt::Batch triangle1;
qyt::Batch grid;


qyt::Batch textRect;
qyt::GLSLProgram* _program;
qyt::GLSLProgram* _posProgram;
GLuint texture;
GLuint textureID;

qyt::mat4 MVP;
qyt::mat4 trans;

qyt::mat4 _M;

typedef struct
{
    float mouse_x, mouse_y;
    float delta_x, delta_y;
    bool is_left_button_pressd;
}SampleControl;

typedef struct
{
    qyt::vec3 eye;
    qyt::vec3 center;
    qyt::vec3 up;
}SampleCamera;

SampleControl control;
SampleCamera  camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        default:
            break;
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) switch(button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                std::cout << "Mosue left button clicked!" << std::endl;
                control.is_left_button_pressd = true;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                std::cout << "Mosue middle button clicked!" << std::endl;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                std::cout << "Mosue right button clicked!" << std::endl;
                break;
            default:
                return;
        }
    else if (action == GLFW_RELEASE) switch(button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
                std::cout << "Mosue left button released!" << std::endl;
                control.is_left_button_pressd = false;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                std::cout << "Mosue middle button released!" << std::endl;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                std::cout << "Mosue right button released!" << std::endl;
                break;
            default:
                return;

        }
    return;
}
void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    control.delta_x = x - control.mouse_x;
    control.delta_y = y - control.mouse_y;

    control.mouse_x = x;
    control.mouse_y = y;
    return;
}
void scroll_callback(GLFWwindow* window, double x, double y)
{
    return;
}

void update_mvp()
{
    qyt::vec4 _eye(camera.eye, 1.f);
    float radx = -control.delta_x/(2*M_PI);
//    float rady = -control.delta_y/(2*M_PI);
    _eye = qyt::rotate(qyt::mat4(1.f), (float)radx/50.f, qyt::vec3(0, 1, 0))*_eye;
    camera.eye = _eye.xyz;
//    camera.eye = (qyt::rotate(qyt::mat4(1.f), (float)rady/50.f, qyt::vec3(1, 0, 0))*_eye).xyz;

    //指定视见矩阵
    qyt::mat4 V = qyt::lookAt(camera.eye,
                              camera.center,
                              camera.up);

    //指定模型变换矩阵
    qyt::mat4 M = qyt::translate(qyt::mat4(1.f),
                                 qyt::vec3(0.f,0.f,0.f));

    //指定投影矩阵
    qyt::mat4 P = qyt::perspective(45.0f, 1.f, 5.f, 100.f);

    //乘起来，获得最后的模视矩阵
    MVP = P*V*M;

    _M = qyt::translate(qyt::mat4(MVP),
                        qyt::vec3(3.f,1.f,1.f));

    control.delta_x = control.delta_y = 0.f;
}

void init(void)
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat colBeginX = -4.5f;
    GLfloat colBeginZ = 5.f;

    GLfloat rowBeginZ = -4.5f, rowBeginX = -5.f;
    grid.begin(GL_LINES, 80);
    for (int i = 0; i < 20-1; i++)
    {
        if(i%2 == 0)
            grid.color4f(1.f, 1.f, 1.f, 1.f);
        else
            grid.color4f(0.2f, 0.2f, 0.2f, 1.f);
        grid.vertex3f(colBeginX, 0.f, colBeginZ);

        if(i%2 == 0)
            grid.color4f(1.f, 1.f, 1.f, 1.f);
        else
            grid.color4f(0.2f, 0.2f, 0.2f, 1.f);
        grid.vertex3f(colBeginX, 0.f, -colBeginZ);
        colBeginX += 0.5f;

        //-----------------------------------------
        if(i%2 == 0)
            grid.color4f(1.f, 1.f, 1.f, 1.f);
        else
            grid.color4f(0.2f, 0.2f, 0.2f, 1.f);
        grid.vertex3f(rowBeginX, 0.f, rowBeginZ);

        if(i%2 == 0)
            grid.color4f(1.f, 1.f, 1.f, 1.f);
        else
            grid.color4f(0.2f, 0.2f, 0.2f, 1.f);
        grid.vertex3f(-rowBeginX, 0.f, rowBeginZ);
        rowBeginZ += 0.5f;

    }
    grid.end();



    triangle1.begin(GL_TRIANGLES, 3);

    triangle1.color4f(1.0f, 0.f, 0.f, 1.f);
    triangle1.vertex3f(-0.9f, -0.9f, 0.f);

    triangle1.color4f(0.0f, 1.f, 0.f, 1.f);
    triangle1.vertex3f(0.9f, -0.9f, 0.f);

    triangle1.color4f(0.0f, 0.f, 1.f, 1.f);
    triangle1.vertex3f(-0.9f, 0.85f, 0.f);

    triangle1.end();

    //----------------------------------------
    textRect.begin(GL_TRIANGLES, 6, 1);

    textRect.texCoord2f(0, 0.f, 0.f);
    textRect.vertex3f(-5.f, -5.f, 0.f);

    textRect.texCoord2f(0, 1.f, 0.f);
    textRect.vertex3f(5.f, -5.f, 0.f);

    textRect.texCoord2f(0, 0.f, 1.f);
    textRect.vertex3f(-5.f, 5.f, 0.f);



    textRect.texCoord2f(0, 1.f, 0.f);
    textRect.vertex3f(5.f, -5.f, 0.f);

    textRect.texCoord2f(0, 1.f, 1.f);
    textRect.vertex3f(5.f, 5.f, 0.f);

    textRect.texCoord2f(0, 0.f, 1.f);
    textRect.vertex3f(-5.f, 5.f, 0.f);

    textRect.end();
    //----------------------------------------

    camera.eye = qyt::vec4(10.f, 10.f, 10.f, 1.f).xyz;
    camera.up = qyt::vec4(0.f, 1.f, 0.f, 0.f).xyz;
    camera.center = qyt::vec4(0.f, 0.f, 0.f, 1.f).xyz;

    //指定视见矩阵
    qyt::mat4 V = qyt::lookAt(camera.eye,
                              camera.center,
                              camera.up);

    //指定模型变换矩阵
    qyt::mat4 M = qyt::translate(qyt::mat4(1.f),
                                 qyt::vec3(0.f,0.f,0.f));

    //指定投影矩阵
    qyt::mat4 P = qyt::perspective(45.0f, 1.f, 5.f, 100.f);

    //乘起来，获得最后的模视矩阵
    MVP = P*V*M;

    _M = qyt::translate(qyt::mat4(MVP), qyt::vec3(3.f,1.f,1.f));

    _program = qyt::GLSLProgram::createWithFile("textureShader.vert",
                                                "textureShader.frag");

    auto fileutil = qyt::FileUtils::getInstance();
    texture = loadBMP_custom(fileutil->fullPathForFilename("texture.bmp").c_str());
    textureID  = _program->getUniformLocation("__qyt_sampler0");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureID, 0);

    _posProgram = qyt::GLSLProgram::createWithFile("positionShader.vert",
                                                   "positionShader.frag");
}

void display(void)
{
    // 1. 调用glClear()清空窗口
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    _posProgram->use();
    _posProgram->uniformMatrix4fv(_posProgram->getUniformLocation("__qyt_mvpMatrix"), &MVP[0][0], 1);
    grid.draw();

    _posProgram->uniformMatrix4fv(_posProgram->getUniformLocation("__qyt_mvpMatrix"), &_M[0][0], 1);
    triangle1.draw();

    _program->use();
    _program->uniformMatrix4fv(_program->getUniformLocation("__qyt_mvpMatrix"), &MVP[0][0], 1);
    textRect.draw();

    // 3. 请求将图像绘制到窗口
    glfwSwapBuffers(window);
}


int main(int argc, char* argv[])
{
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( 1024, 768, "test", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwPollEvents();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    init();

    do
    {
        if (control.is_left_button_pressd == true)
            update_mvp();

        display();
        glfwPollEvents();
    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

    return 0;
}