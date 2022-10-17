#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#pragma comment(lib, "OpenGL32")

using namespace std;

chrono::system_clock::time_point startRenderTimePoint;
chrono::duration<double> renderDuration;

GLFWwindow* window;
bool isFirstFrame = true;


struct Vertex
{
    glm::vec3 pos;
    float r, g, b, a;
};

struct Transform
{
    glm::mat3 translate;
    glm::mat3 scale;
    glm::mat3 rotation;
};

Vertex star[5];             // static mesh
Vertex transformedStar[5];  //화면에 그릴 오망성


Vertex circle[360];             // static mesh 

Vertex circle_layer_1[360];     // static mesh 
Vertex circle_layer_2[360];     // static mesh 

Vertex transformedCircle[360];  // 화면에 그려질 원

Vertex transformedCircle_layer_1[360];  // 화면에 그려질 원 1번
Vertex transformedCircle_layer_2[360];  // 화면에 그려질 원 1번


Transform transform;  //world 행렬이 될 transform

Transform layerCircle;
Transform layerOne;
Transform layerTwo;
Transform layerThree;

Transform layerStar;


void Init();
void Update();
void Release();

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void);


void Init()
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(720, 720, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSwapInterval(1);

    startRenderTimePoint = chrono::system_clock::now();
    

    //object생성부
    /// Star(오망성) 생성
    int i = 0;
    for (float theta = 0; theta < 360.0f; theta += 72.0f)
    {

        star[i].pos.x = -glm::sin(glm::radians(theta)) * 0.5f;
        star[i].pos.y = glm::cos(glm::radians(theta)) * 0.5f; // * 0.5
        star[i].pos.z = 1.0f;

        star[i].r = 0.3f;
        star[i].g = 0.1f;
        star[i].b = theta / 360.0f;
        star[i].a = 0.7f;


        transformedStar[i] = star[i];
        i++;
    }

    // 원 생성
    for (int theta = 0; theta < 360; theta++)
    {
        circle[theta].pos.x = -glm::sin(glm::radians((float)theta)) * 0.5f;
        circle[theta].pos.y = glm::cos(glm::radians((float)theta)) * 0.5f; // *0.5
        circle[theta].pos.z = 1.0f;

        circle[theta].r = 0.3f;
        circle[theta].g = 0.0f;
        circle[theta].b = (float)theta / 360.0f;
        circle[theta].a = 0.7f;

        transformedCircle[theta] = circle[theta];
    }
    //----------------------------------- 1번 원 ---------------------------------
    for (int theta = 0; theta < 360; theta++)
    {
        circle_layer_1[theta].pos.x = -glm::sin(glm::radians((float)theta)) * 0.5f;
        circle_layer_1[theta].pos.y = glm::cos(glm::radians((float)theta)) * 0.5f; // *0.5
        circle_layer_1[theta].pos.z = 1.0f;

        circle_layer_1[theta].r = 0.1f;
        circle_layer_1[theta].g = 0.1f;
        circle_layer_1[theta].b = 0.5f;
        circle_layer_1[theta].a = 0.7f;

        transformedCircle_layer_1[theta] = circle_layer_1[theta];
    }

    //----------------------------------- 2번 원 ---------------------------------
    for (int theta = 0; theta < 360; theta++)
    {
        circle_layer_2[theta].pos.x = -glm::sin(glm::radians((float)theta)) * 0.5f;
        circle_layer_2[theta].pos.y = glm::cos(glm::radians((float)theta)) * 0.5f; // *0.5
        circle_layer_2[theta].pos.z = 1.0f;

        circle_layer_2[theta].r = 0.1f;
        circle_layer_2[theta].g = 0.1f;
        circle_layer_2[theta].b = 0.0;
        circle_layer_2[theta].a = 0.1f;

        transformedCircle_layer_2[theta] = circle_layer_2[theta];
    }
  

    //트랜스폼 초기화 (기본형 제공)
    transform.translate = glm::mat3(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
    transform.rotation = glm::mat3(
        glm::cos(glm::radians(0.0f)), -glm::sin(glm::radians(0.0f)), 0,
        glm::sin(glm::radians(0.0f)), glm::cos(glm::radians(0.0f)), 0,
        0, 0, 1
    );
    transform.scale = glm::mat3(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
}
int k = 0;
float h = 0.0f;
float dis = 1;
void Release()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
void Update()
{
    int i = 0;
    while (!glfwWindowShouldClose(window))
    {      
        for (float theta = 0; theta < 360; theta+=72)
        {
            star[i].pos.x = -glm::sin(glm::radians(theta+k)) * 0.5f;
            star[i].pos.y = glm::cos(glm::radians(theta+k)) * 0.5f; // * 0.5
            i++;
        }
        if (i > 2) i = 0.0f;

        k += 1;
        if (k > 360) k = 0;
        
        if (h >= 1.0f)
        {
            dis = -1;
            h = 1.0f;
        }
        if (h <= 0.3f)
        {
            dis = 1;
            h = 0.3f;
        }
        h += 0.01f * dis;

        //------------레이어 1-----------------
        layerOne.translate = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );
        layerOne.scale = glm::mat3(
            1, 0, 0,
            0, 0.3, 0,
            0, 0, 1
        );
        layerOne.rotation = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );

        //------------레이어 2-----------------
        layerTwo.translate = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0.3, 1
        );
        layerTwo.scale = glm::mat3(
            h, 0, 0,
            0, 0.3, 0,
            0, 0, 1
        );
        layerTwo.rotation = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );

        //------------별-----------------
        layerStar.translate = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );
        layerStar.scale = glm::mat3(
            1, 0, 0,
            0, 0.3, 0,
            0, 0, 1
        );
        layerStar.rotation = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );

        //------------원-----------------
        layerCircle.translate = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );
        layerCircle.scale = glm::mat3(
            1, 0, 0,
            0, 0.3, 0,
            0, 0, 1
        );
        layerCircle.rotation = glm::mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1
        );


        // ----------------1 번 레이어----------------
        for (int i = 0; i < 360; i++)
        {
            transformedCircle_layer_1[i].pos = layerOne.translate * layerOne.rotation * layerOne.scale * circle_layer_1[i].pos;

        }

        // ----------------2 번 레이어----------------
        for (int i = 0; i < 360; i++)
        {
            transformedCircle_layer_2[i].pos = layerTwo.translate * layerTwo.rotation* layerTwo.scale* circle_layer_2[i].pos;
        }

        // ---------------- 별 레이어----------------
        for (int i = 0; i < 5; i++)
        {
            transformedStar[i].pos = layerStar.translate * layerStar.rotation * layerStar.scale * star[i].pos;
        }
        // ----------------마법진 레이어----------------
        for (int i = 0; i < 360; i++)
        {
            transformedCircle[i].pos = layerCircle.translate * layerCircle.rotation * layerCircle.scale * circle[i].pos;
        }
        

        //색 초기화
        glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        //선두께
        glLineWidth(7.0f);
        //오망성 그리기
        glBegin(GL_LINE_STRIP);

        int a = 0;
        glColor4f(transformedStar[a].r, transformedStar[a].g, transformedStar[a].b, transformedStar[a].a);
        glVertex3f(transformedStar[a].pos.x, transformedStar[a].pos.y, 0.0f);
        a = 3;
        glColor4f(transformedStar[a].r, transformedStar[a].g, transformedStar[a].b, transformedStar[a].a);
        glVertex3f(transformedStar[a].pos.x, transformedStar[a].pos.y, 0.0f);
        a = 1;
        glColor4f(transformedStar[a].r, transformedStar[a].g, transformedStar[a].b, transformedStar[a].a);
        glVertex3f(transformedStar[a].pos.x, transformedStar[a].pos.y, 0.0f);
        a = 4;
        glColor4f(transformedStar[a].r, transformedStar[a].g, transformedStar[a].b, transformedStar[a].a);
        glVertex3f(transformedStar[a].pos.x, transformedStar[a].pos.y, 0.0f);
        a = 2;
        glColor4f(transformedStar[a].r, transformedStar[a].g, transformedStar[a].b, transformedStar[a].a);
        glVertex3f(transformedStar[a].pos.x, transformedStar[a].pos.y, 0.0f);
        a = 0;
        glColor4f(transformedStar[a].r, transformedStar[a].g, transformedStar[a].b, transformedStar[a].a);
        glVertex3f(transformedStar[a].pos.x, transformedStar[a].pos.y, 0.0f);
        glEnd();
            
        //-----------원그리기-----------
        glBegin(GL_LINE_STRIP);
        for (int theta = 0; theta < 360; theta++)
        {
            glColor4f(transformedCircle[theta].r, transformedCircle[theta].g, transformedCircle[theta].b, transformedCircle[theta].a);
            glVertex3f(transformedCircle[theta].pos.x, transformedCircle[theta].pos.y, 0.0f);

        }
        glEnd();

        glBegin(GL_TRIANGLES);
        for (int theta = 0; theta <= 360; theta += 10)
        {
            int j = 5;
            glColor4f(transformedCircle_layer_1[theta].r, transformedCircle_layer_1[theta].g, transformedCircle_layer_1[theta].b, transformedCircle_layer_1[theta].a);
            glVertex3f(transformedCircle_layer_1[theta].pos.x, transformedCircle_layer_1[theta].pos.y, 0.0f);

            glColor4f(transformedCircle_layer_2[theta +1* j].r, transformedCircle_layer_2[theta +1* j].g, transformedCircle_layer_2[theta +1* j].b, transformedCircle_layer_2[theta +1* j].a);
            glVertex3f(transformedCircle_layer_2[theta +1 * j].pos.x, transformedCircle_layer_2[theta +1* j].pos.y, 0.0f);
            if (theta >= 350) {
                glColor4f(transformedCircle_layer_1[0].r, transformedCircle_layer_1[0].g, transformedCircle_layer_1[0].b, transformedCircle_layer_1[0].a);
                glVertex3f(transformedCircle_layer_1[0].pos.x, transformedCircle_layer_1[0].pos.y, 0.0f);
            }
            else {
                glColor4f(transformedCircle_layer_1[theta + 2 * j].r, transformedCircle_layer_1[theta + 2 * j].g, transformedCircle_layer_1[theta + 2 * j].b, transformedCircle_layer_1[theta + 2 * j].a);
                glVertex3f(transformedCircle_layer_1[theta + 2 * j].pos.x, transformedCircle_layer_1[theta + 2 * j].pos.y, 0.0f);
            }
        }
        glEnd();

       glBegin(GL_TRIANGLES);
        for (int theta = 0; theta < 360; theta += 10)
        {
            int j = 5;
            if (theta >= 350) {
                int h = 355;
                glColor4f(transformedCircle_layer_2[h].r, transformedCircle_layer_2[h].g, transformedCircle_layer_2[h].b, transformedCircle_layer_2[h].a);
                glVertex3f(transformedCircle_layer_2[h].pos.x, transformedCircle_layer_2[h].pos.y, 0.0f);
            }
            else {
                glColor4f(transformedCircle_layer_2[theta + 1 * j].r, transformedCircle_layer_2[theta + 1 * j].g, transformedCircle_layer_2[theta + 1 * j].b, transformedCircle_layer_2[theta + 1 * j].a);
                glVertex3f(transformedCircle_layer_2[theta + 1 * j].pos.x, transformedCircle_layer_2[theta + 1 * j].pos.y, 0.0f);
            }
            if (theta >= 350) {
                int h = 0;
                glColor4f(transformedCircle_layer_1[h].r, transformedCircle_layer_1[h].g, transformedCircle_layer_1[h].b, transformedCircle_layer_1[h].a);
                glVertex3f(transformedCircle_layer_1[h].pos.x, transformedCircle_layer_1[h].pos.y, 0.0f);
            }
            else {
                glColor4f(transformedCircle_layer_1[theta + 2 * j].r, transformedCircle_layer_1[theta + 2 * j].g, transformedCircle_layer_1[theta + 2 * j].b, transformedCircle_layer_1[theta + 2 * j].a);
                glVertex3f(transformedCircle_layer_1[theta + 2 * j].pos.x, transformedCircle_layer_1[theta + 2 * j].pos.y, 0.0f);
            }
            if (theta >= 350) {
                int h = 5;
                glColor4f(transformedCircle_layer_2[h].r, transformedCircle_layer_2[h].g, transformedCircle_layer_2[h].b, transformedCircle_layer_2[h].a);
                glVertex3f(transformedCircle_layer_2[h].pos.x, transformedCircle_layer_2[h].pos.y, 0.0f);
            }
            else {
                glColor4f(transformedCircle_layer_2[theta + 3 * j].r, transformedCircle_layer_2[theta + 3 * j].g, transformedCircle_layer_2[theta + 3 * j].b, transformedCircle_layer_2[theta + 3 * j].a);
                glVertex3f(transformedCircle_layer_2[theta + 3 * j].pos.x, transformedCircle_layer_2[theta + 3 * j].pos.y, 0.0f);
            }
            
        }
        glEnd();


        
        //화면 스왑
        glfwSwapBuffers(window);
        glfwPollEvents();

        //렌더시간 측정
        renderDuration = chrono::system_clock::now() - startRenderTimePoint;
        startRenderTimePoint = chrono::system_clock::now();

        float fps = 1.0 / renderDuration.count();
        if (isFirstFrame == true)
        {
            isFirstFrame = false;
            continue;
        }
        if(renderDuration.count() < (1.0f / 60.0f))
            this_thread::sleep_for(chrono::milliseconds( (int)(((1.0f/60.0f) - renderDuration.count())*1000) ));
        string fps_s = "FPS : " + to_string(fps);
        std::cout << fps_s <<endl;
        
    }
}

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    Init(); 
    Update();
    Release();    
    
    exit(EXIT_SUCCESS);
}