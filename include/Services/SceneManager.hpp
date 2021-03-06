#ifndef __SCENEMANAGER__HPP
#define __SCENEMANAGER__HPP

#include "simpleGL_macro.hpp"  // Needed for macro definition

#include "Interface/IScene.hpp"
#include "Interface/ISceneManager.hpp"

namespace simpleGL
{
    /// Basic scene manager as a FSM.
    class SIMPLEGL_API SceneManager: public ISceneManager
    {
    protected:
        bool m_transitionToNext;
        bool m_transitionToPrevious;

        IScene* m_pCurrent;
        IScene* m_pPrevious;
        IScene* m_pNext;

        void ToNext();
        void ToPrevious();

    public:
        SceneManager(IScene& _startScene);
        ~SceneManager();

        virtual bool Init();
        virtual bool Update();
        virtual bool Quit();
        virtual void Render();

        virtual void    Change(IScene* _pScene);
        virtual IScene* GetCurrent();

        virtual void Revert();
    };
}
#endif
