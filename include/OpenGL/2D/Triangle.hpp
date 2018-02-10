#ifndef __TRIANGLE__HPP
#define __TRIANGLE__HPP

#include "simpleGL_macro.hpp"  // Needed for macro definition
#include "Shape2D.hpp"
#include "Utility/Tools.hpp"

#include "OpenGL/Texture.hpp"


namespace simpleGL
{
    // Can be used to construct and draw a triangle
    class SIMPLEGL_API Triangle: public Shape2D
    {
    private:
        // Vertex array buffer (VAO)
        // Vertex buffer object (VBO)
        unsigned int m_VAO, m_VBO;

        static const int m_sizeData = 24;
        static const int m_sizeVertices = 3;
        static const int m_sizePos = 3;
        static const int m_sizeColor = 3;
        static const int m_sizeUV = 2;

        float m_vertices[m_sizeData];

        void SendData();

    public:
        Triangle();
        virtual ~Triangle();

        void Create(GL_POS3 _pos[m_sizeVertices], GL_COLOR3 _colors[m_sizeVertices]);
        void Create(GL_POS3 _pos[m_sizeVertices]);

        virtual void Draw();
        virtual void Init() {};
        virtual void Update(){};
        virtual void Quit(){};
        virtual GameObject* Clone() {return new Triangle();}
    };
}
#endif