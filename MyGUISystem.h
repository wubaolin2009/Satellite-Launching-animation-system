#ifndef _MY_GUI_SYSTEM_H
#define _MY_GUI_SYSTEM_H

#include <d3dx9.h>
#include "CEGUI.h"
#include "CEGUIDefaultResourceProvider.h"
#include "OIS.h"
#include "RendererModules\Ogre\CEGuiOgreRenderer.h"

class MyGUISystem  
{  
public:  
	virtual ~MyGUISystem();  
	//把OIS的鼠标ID转换成CEGUI的鼠标ID  
	static CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonId);  
	//销毁资源  
	bool destroy();  
	//初始化CEGUI， 必须在RenderWindow创始后初始, 只在第一次调用有效  
	bool init();  
	//GUI更新  
	virtual void update(float timeSinceLastFrame);  
	/** 
	根据图片构造Imageset 
	params: 
	@name       创建的Imageset名称, 全局唯一 
	@filename   创建Imageset的图片名称 
	*/  
	static void createImageset(const std::string& name, const std::string& filename);  
	//加载layout  
	bool loadLayout(const std::string& layoutName);  
	//得到指定名称的窗口  
	inline CEGUI::Window* getWindow(const std::string& windowName)   
	{  
		return getGUIWindowManager()->getWindow(windowName);   
	}  
	//销毁指定名称的窗口  
	bool destroyWindow(const std::string& windowName);  
	/* 
	*   给指定名称的窗口设定属性 
	params: 
	@windowName 窗口名称 
	@key            XML标签  key值 
	@value          XML标签  value值 
	*/  
	static void setProperty(const std::string& windowName, const std::string& key,   
		const std::string& value);  
	/** 
	注册CEGUI控件事件 
	params: 
	@windowName     要注册事件的控件名称 
	@eventName      注册的控件事件 
	@subscriber     事件触发时的回调函数 
	*/  
	static void subscribeEvent(const CEGUI::String& windowName,   
		const CEGUI::String &eventName,   
		const CEGUI::Event::Subscriber &subscriber);  
	//得到窗口管理器，用来获取管理GUI的窗口对象的管理器  
	inline CEGUI::WindowManager* getGUIWindowManager()  
	{  
		return CEGUI::WindowManager::getSingletonPtr();  
	}  
	// 注入CEGUI鼠标点击事件  
	bool injectMouseButtonDown(CEGUI::MouseButton id)   
	{  
		return mGUISystem->injectMouseButtonDown(id);  
	}  
	bool injectMouseButtonUp(CEGUI::MouseButton id)   
	{  
		return mGUISystem->injectMouseButtonUp(id);   
	}  
	bool injectKeyDown(CEGUI::uint id)
	{
		return mGUISystem->injectKeyDown(id);
	}
	bool injectChar(unsigned int text)
	{
		return mGUISystem->injectChar(text);
	}
	// 注入CEGUI鼠标移动事件  
	bool injectMouseMove(float xRel, float yRel)  
	{  
		return mGUISystem->injectMouseMove(xRel, yRel);  
	}  
	//显示/隐藏GUI鼠标  
	inline void showGUICursor(bool showCursor)  
	{  
		if(showCursor)  
		{  
			CEGUI::MouseCursor::getSingleton().show();  
		}  
		else  
		{  
			CEGUI::MouseCursor::getSingleton().hide();  
		}  
		//隐藏系统鼠标  
		ShowCursor(false);  
	}  
	/* 
	*   设置默认鼠标样式 
	params: 
	@schemeName 样式名称 
	@mouseName      样式中的鼠标名称 
	*/  
	inline void setDefaultMouseCursor(const std::string& schemeName, const std::string& mouseName)   
	{  
		CEGUI::System::getSingleton().setDefaultMouseCursor(schemeName, mouseName);  
		CEGUI::MouseCursor::getSingleton().setImage(mGUISystem->getDefaultMouseCursor());  
	}  
public:  
	/* 
	*   单件实例方法 
	*/  
	static MyGUISystem* getSingletonPtr()  
	{  
		if(ms_singleton == NULL)  
		{  
			ms_singleton = new MyGUISystem;  
		}  
		return ms_singleton;  
	}  
private:  
	//单件实例指针  
	static MyGUISystem* ms_singleton;  
private:  
	CEGUI::OgreRenderer* mGUIRender;  
	CEGUI::System*       mGUISystem;  
private:  
	/* 
	*   私有化实例函数，实现单件 
	*/  
	MyGUISystem()  
		:mGUIRender(0), mGUISystem(0)  
	{}  
	MyGUISystem(const MyGUISystem&);  
	MyGUISystem& operator=(const MyGUISystem&);  
};  

#endif