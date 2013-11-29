#include <cassert>
#include "MyGUISystem.h"

//-----------------------------------------------------------------------------  
MyGUISystem* MyGUISystem::ms_singleton = NULL;  
//-----------------------------------------------------------------------------  
MyGUISystem::~MyGUISystem()  
{  
    destroy();  
}  
//-----------------------------------------------------------------------------  
bool MyGUISystem::destroy()  
{  
    CEGUI::OgreRenderer::destroySystem();  
    delete ms_singleton;  
    ms_singleton = 0;  
    return true;  
}  
//-----------------------------------------------------------------------------  
CEGUI::MouseButton MyGUISystem::convertButton(OIS::MouseButtonID buttonId)  
{  
    switch(buttonId)  
    {  
    case OIS::MB_Left:  
        return CEGUI::LeftButton;  
    case OIS::MB_Right:  
        return CEGUI::RightButton;  
    case OIS::MB_Middle:  
        return CEGUI::MiddleButton;  
    default:  
        return CEGUI::LeftButton;  
    }  
}  

//-----------------------------------------------------------------------------  
bool MyGUISystem::init()  
{  
    if(!mGUIRender)  
    { 	
	    mGUIRender = &CEGUI::OgreRenderer::bootstrapSystem();  
		mGUISystem = CEGUI::System::getSingletonPtr();  
        //加载GUI主题， 设置默认参数  
        CEGUI::Imageset::setDefaultResourceGroup("Imagesets");  
        CEGUI::Font::setDefaultResourceGroup("Fonts");  
        CEGUI::Scheme::setDefaultResourceGroup("Schemes");  
        CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");  
        CEGUI::WindowManager::setDefaultResourceGroup("Layouts");  
        CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");  
		CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme"); 
		CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
        CEGUI::FontManager::getSingleton().create("SimHei-14.font");  
        mGUISystem->setDefaultFont("SimHei-14");  
        /* 
         *  此处设置图片资源(Imageset) 
         */  
        //设置默认鼠标样式  
        setDefaultMouseCursor("TaharezLook", "MouseArrow");  
        //设置GUI鼠标是否可见  
        showGUICursor(true);  
        return true;  
    }  
    return false;  
}  
//-----------------------------------------------------------------------------  
void MyGUISystem::update(float timeSinceLastFrame)  
{  
    mGUISystem->injectTimePulse(timeSinceLastFrame);  
}  
//-----------------------------------------------------------------------------  
void MyGUISystem::createImageset(const std::string& name, const std::string& filename)  
{  
    CEGUI::ImagesetManager::getSingleton().createFromImageFile(name, filename);  
}  
//-----------------------------------------------------------------------------  
bool MyGUISystem::loadLayout(const std::string& layoutName)  
{  
    CEGUI::Window *guiSheet;  
    // 检测给定layout的文件是否加载,没有加载则加载  
    if(!getGUIWindowManager()->isWindowPresent(layoutName))  
    {  
        // 从 .layout脚本文件读取一个UI布局设计，并将其放置到GUI资源组中。  
        guiSheet = getGUIWindowManager()->loadWindowLayout(layoutName + ".layout");  
    }  
    else  
    {     
        // 如果已经加载则直接显示  
        guiSheet = getGUIWindowManager()->getWindow(layoutName);  
    }  
    // 接下来我们告诉CEGUI显示哪份UI布局。当然我们可以随时更换显示的UI布局。  
    mGUISystem->setGUISheet(guiSheet);  
    //显示UI布局  
    guiSheet->show();  
    return true;  
}  
//-----------------------------------------------------------------------------  
bool MyGUISystem::destroyWindow(const std::string& windowName)  
{  
    getGUIWindowManager()->destroyWindow(windowName);  
    return true;  
}  
//-----------------------------------------------------------------------------  
void MyGUISystem::subscribeEvent(const CEGUI::String& windowName,  
    const CEGUI::String &eventName, const CEGUI::Event::Subscriber &subscriber)  
{  
    CEGUI::WindowManager::getSingleton().getWindow(CEGUI::String(windowName))  
        ->subscribeEvent(eventName,subscriber);  
}  
//-----------------------------------------------------------------------------  
void MyGUISystem::setProperty(const std::string& windowName,   
    const std::string& key, const std::string& value)  
{  
    MyGUISystem::getSingletonPtr()->getWindow(windowName)->setProperty(key, value);  
}  
//-----------------------------------------------------------------------------  
