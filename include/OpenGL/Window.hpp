#ifndef __WINDOW__HPP
#define __WINDOW__HPP

#include "simpleGL_macro.hpp"  // Needed for macro definition
#include "Interface/IWindow.hpp"
#include "gtc/matrix_transform.hpp"


namespace simpleGL
{
    class SIMPLEGL_LOCAL Window: public IWindow
    {
    private:
        unsigned int m_width, m_height;

        void ProcessInput();
        void SetDefaultFlag();

        // We only one a single instance but no singleton
        static bool instantiated_;

        // Callback to handle keys as events
        static void Key_callback(GLFWwindow* window, int _key, int _scancode, int _action, int _mods);
        static void Framebuffer_size_callback(GLFWwindow* _window, int _width, int _height);

        // Contains handler for debug keys
        static void DebugKeyEventHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);

    protected:
        int const m_versionMajor = 3;
        int const m_versionMinor = 3;
        int const m_profile = GLFW_OPENGL_CORE_PROFILE;
        std::string m_name;
        float       m_clearColor[4];
        GLFWwindow* m_pWindow;

        glm::mat4 m_viewM;
        glm::mat4 m_projM;


        // Callback handler
        GLFWkeyfun m_pUserKeyCallback;

    public:
        Window(unsigned int _width, unsigned int _height, std::string _name);
        ~Window();

        virtual bool Init();
        virtual bool Update();
        virtual bool Quit();
        virtual void Render();

        virtual int GetKey(int _key);

        void AttachKeyEventCallback(GLFWkeyfun _callback);
        void DetachKeyEventCallback() {m_pUserKeyCallback = nullptr;}

        // glfwSetWindowUserPointer and glfwGetWindowUserPointer
        // can be used to set and get specific chunk of data
        // see https://tinyurl.com/ychsjpa8
        // void  RegisterPointer(void* _pointer);
        // void* AccessPointer();

        virtual inline GLFWwindow& GetGLFWwindow() const {return *m_pWindow;}
        virtual inline const glm::mat4 GetViewMatrix() const {return m_viewM;}
        virtual inline const glm::mat4 GetProjectionMatrix() const {return m_projM;}
    };
}
#endif
