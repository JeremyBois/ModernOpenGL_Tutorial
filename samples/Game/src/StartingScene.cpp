#include "StartingScene.hpp"

#include "math.h"
#include "glm.hpp"

typedef simpleGL::GameManager Game;



StartingScene::StartingScene()
{
    m_pFirstNode =  new simpleGL::Node();

    // Compile shader
    m_colorShader =  simpleGL::Shader("shaders/basic.vert",
                                      "shaders/colorFromProgram.frag");
    float color[] = {0.0f, 1.0f, 0.0f, 1.0f};
    m_colorShader.Use();
    m_colorShader.SetFloat4("ourColor", color);

    m_uvShader =  simpleGL::Shader("shaders/basic.vert",
                                   "shaders/UVscale.frag");
    m_uvShader.Use();
    m_uvShader.SetFloat("uvScale", 1.0f);

    m_colorVertexShader = simpleGL::Shader("shaders/positionColor.vert",
                                           "shaders/colorFromVertex.frag");

    // Get a texture
    m_textureWall.Load("data/images/wall.jpg");
    m_textureContainer.Load("data/images/container.jpg", false, true);
    m_textureSmile.Load("data/images/awesomeface.png", true, true);

    // Create a triangle
    m_pTriangles[0] = new simpleGL::Triangle();
    m_pTriangles[1] = new simpleGL::Triangle();
    m_pQuad = new simpleGL::Quad();

    // Create a cube
    m_pCuboid = new simpleGL::Cuboid();

}

StartingScene::~StartingScene()
{

}


void StartingScene::ChangeGreenOverTime(simpleGL::Shader& _shader)
{
    float color[] = {0.0f, 1.0f, 0.0f, 1.0f};

    float timeValue = glfwGetTime();
    color[1] =  (std::sin(timeValue) / 2.0f) + 0.5f;
    _shader.Use();
    _shader.SetFloat4("ourColor", color);
}


void StartingScene::ZoomUVOverTime(simpleGL::Shader& _shader)
{
    float timeValue = glfwGetTime();
    float scale = 5.0f * (std::sin(timeValue * 0.25f) / 2.0f + 0.5f);
    _shader.Use();
    _shader.SetFloat("uvScale", scale);
}


void StartingScene::ZoomUV(simpleGL::Shader& _shader, bool _zoom)
{
    m_zoomScale += _zoom ? 0.1f : -0.1f;
    if (m_zoomScale < 0)
    {
        m_zoomScale = 0.0f;
    }
    _shader.Use();
    _shader.SetFloat("uvScale", m_zoomScale);
}




bool StartingScene::OnInit()
{
    simpleGL::GameManager::AttachNodeMgr(&m_container);

    glm::vec3 pos1[]
    {
        {0.4f,  0.8f, 0.0f},
        {0.4f, 0.4f, 0.0f},
        {0.8f, 0.4f, 0.0f}
    };

    glm::vec3 pos2[]
    {
        {-0.8f,  -0.4f, 0.0f},
        {-0.8f, -0.8f, 0.0f},
        {-0.4f, -0.8f, 0.0f}
    };

    glm::vec4 color1[]
    {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    };

    m_pTriangles[0]->Create(pos1, color1);
    m_pTriangles[0]->LinkTexture(&m_textureWall);

    m_pTriangles[1]->Create(pos2);
    m_pTriangles[1]->LinkShader(&m_colorShader);


    // Add quad
    m_pQuad->Create(0.8f, 1.2f);
    m_pQuad->LinkShader(&m_uvShader);
    m_pQuad->LinkTexture(&m_textureContainer, GL_TEXTURE0);
    m_pQuad->LinkTexture(&m_textureSmile, GL_TEXTURE1);
    m_pQuad->SetYawPitchRollAngles(glm::vec3(-55.0f, 0.0f, 0.0f));
    m_uvShader.Use();
    m_uvShader.SetInt("tex0", 0);
    m_uvShader.SetInt("tex1", 1);

    // Add Cuboid
    m_pCuboid->Create(0.5f, 0.5f, 0.5f);
    m_pCuboid->LinkShader(&m_colorVertexShader);
    m_pCuboid->SetPosition(glm::vec3(-2.0f, 0.0f, -2.0f));
    m_pCuboid->SetScale(glm::vec3(1.0f, 1.0f, 3.0f));


    // Populate container
    m_pFirstNode->AddGameObject(m_pTriangles[0]);
    m_pFirstNode->AddGameObject(m_pTriangles[1]);
    m_pFirstNode->AddGameObject(m_pQuad);
    m_pFirstNode->AddGameObject(m_pCuboid);
    m_container.AddNode(m_pFirstNode);


    // Add callback for key events
    Game::GetWindow().AttachKeyEventCallback(MyKeyEventHandler);
}

bool StartingScene::OnUpdate()
{
    ChangeGreenOverTime(m_colorShader);
    if (Game::GetWindow().GetKey(GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        ZoomUV(m_uvShader, true);
    }
    else if (Game::GetWindow().GetKey(GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        ZoomUV(m_uvShader, false);
    }


    // Rotate
    float rotation = 360 * (std::cos(glfwGetTime() * 0.5f) * 0.5f + 0.5f);
    // m_pTriangles[0]->SetYawPitchRollAngles(glm::vec3(0.0f, 0.0f, rotation));
    m_pTriangles[0]->SetRotationZ(rotation);
}

bool StartingScene::OnQuit()
{
    delete m_pFirstNode;

    // Detach obsolete services
    simpleGL::GameManager::DetachNodeMgr();

    // Remove event callback
    Game::GetWindow().DetachKeyEventCallback();

    return true;
}

void StartingScene::OnRender()
{

}

// @TODO Replace with set
void StartingScene::MyKeyEventHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
    // Change wrap mode
    if (_key == GLFW_KEY_W && _action == GLFW_PRESS)
    {
        // Set texture to affect
        // Get ref to active Scene
        StartingScene* myScene = (StartingScene*)Game::GetSceneMgr().GetCurrent();
        simpleGL::Texture myTexture = myScene->m_textureContainer;

        // Get current wrap mode
        GLint result[1];
        myTexture.Get(GL_TEXTURE_WRAP_S, result);

        switch (result[0])
        {
            case GL_REPEAT:
                myTexture.Set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                myTexture.Set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                cout << "Wrap mode is now: " << "CLAMP TO EDGE" << endl;
                break;
            case GL_CLAMP_TO_EDGE:
                myTexture.Set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                myTexture.Set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                cout << "Wrap mode is now: " << "CLAMP TO BORDER" << endl;
                break;
            case GL_CLAMP_TO_BORDER:
                myTexture.Set(GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                myTexture.Set(GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                cout << "Wrap mode is now: " << "MIRRORED REPEAT" << endl;
                break;
            case GL_MIRRORED_REPEAT:
                myTexture.Set(GL_TEXTURE_WRAP_S, GL_REPEAT);
                myTexture.Set(GL_TEXTURE_WRAP_T, GL_REPEAT);
                cout << "Wrap mode is now: " << "REPEAT" << endl;
                break;
        }
    }

    // Change filtering mode
    else if (_key == GLFW_KEY_F && _action == GLFW_PRESS)
    {
        // Set texture to affect
        // Get ref to active Scene
        StartingScene* myScene = (StartingScene*)Game::GetSceneMgr().GetCurrent();
        simpleGL::Texture myTexture = myScene->m_textureContainer;

        // Get current wrap mode
        GLint result[1];
        myTexture.Get(GL_TEXTURE_MIN_FILTER, result);

        switch (result[0])
        {
            case GL_NEAREST:
                myTexture.Set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                myTexture.Set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                cout << "Filtering mode is now: " << "LINEAR" << endl;
                break;
            case GL_LINEAR:
                myTexture.Set(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                myTexture.Set(GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                cout << "Filtering mode is now: " << "NEAREST + MIPMAP NEAREST" << endl;
                break;
            case GL_NEAREST_MIPMAP_NEAREST:
                myTexture.Set(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                myTexture.Set(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                cout << "Filtering mode is now: " << "LINEAR + MIPMAP NEAREST" << endl;
                break;
            case GL_LINEAR_MIPMAP_NEAREST:
                myTexture.Set(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                myTexture.Set(GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                cout << "Filtering mode is now: " << "NEAREST + MIPMAP LINEAR" << endl;
                break;
            case GL_NEAREST_MIPMAP_LINEAR:
                myTexture.Set(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                myTexture.Set(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                cout << "Filtering mode is now: " << "LINEAR + MIPMAP LINEAR" << endl;
                break;
            case GL_LINEAR_MIPMAP_LINEAR:
                myTexture.Set(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                myTexture.Set(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                cout << "Filtering mode is now: " << "NEAREST" << endl;
                break;
        }
    }
}

