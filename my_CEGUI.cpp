#include <d3dx9.h>
#include "CEGUIWindow.h"
#include "CEGUIEventSet.h"
#include "my_CEGUI.h"


extern HWND g_hwnd;
//extern LPDIRECT3DDEVICE9 g_pd3dDevice;

void CeGUI::InitGUI(LPDIRECT3DDEVICE9 m_d3dDevice)
{
	m_pCEGUIRender = new CEGUI::DirectX9Renderer(m_d3dDevice , 0);	
	new CEGUI::System(m_pCEGUIRender);
	/// 初始化GUI资源的缺省路径
	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());
	rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
	rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
	rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
	rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
	//rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
	/// 设置使用的缺省资源
	CEGUI::Imageset::setDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	//CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	/// 设置GUI
	using namespace CEGUI;


	/// 得到GUI样式的图片集
	Imageset* taharezlookImage;
	try{
		taharezlookImage = ImagesetManager::getSingleton().createImageset("TaharezLook.imageset");
	}catch (CEGUI::Exception& exc)
	{
		//const char* p = exc.getMessage().c_str();
		MessageBox(NULL,"Error1",NULL,NULL);
	}
	/// 设置鼠标图标
	System::getSingleton().setDefaultMouseCursor(&taharezlookImage->getImage("MouseArrow"));

	/// 设置字体
	FontManager::getSingleton().createFont("Commonwealth-10.font");

	/// 设置GUI皮肤
	WidgetLookManager::getSingleton().parseLookNFeelSpecification("TaharezLook.looknfeel");

	/// 载入GUI规划
	SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
	/// 得到窗口管理单件
	WindowManager& winMgr = WindowManager::getSingleton();

	/// 从layout文件中载入布局
	Window* gameGUI = winMgr.loadWindowLayout("GameGUI.layout"); 

	/// 设置GUI的Sheet(Sheet是CEGUI中窗口的容器)
	System::getSingleton().setGUISheet(gameGUI);

	/// 设置按钮文本
	winMgr.getWindow("GameGUI/Origin")->setText("Origin");
	winMgr.getWindow("GameGUI/Liner")->setText("Liner");
	winMgr.getWindow("GameGUI/41Liner")->setText("41Liner");
	winMgr.getWindow("GameGUI/Height")->setText("Height");
	winMgr.getWindow("GameGUI/LOD")->setText("LOD");
	winMgr.getWindow("GameGUI/FPSText")->setText("FPS");

	/// 设置鼠标事件自动重复
	winMgr.getWindow("GameGUI/Origin")->setWantsMultiClickEvents(false);
	winMgr.getWindow("GameGUI/Liner")->setWantsMultiClickEvents(false);
	winMgr.getWindow("GameGUI/41Liner")->setWantsMultiClickEvents(false);
	winMgr.getWindow("GameGUI/Height")->setWantsMultiClickEvents(false);
	winMgr.getWindow("GameGUI/LOD")->setWantsMultiClickEvents(false);

	/// 添加按钮的事件处理函数
	/*winMgr.getWindow("GameGUI/Origin")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown, 
	CEGUI::Event::Subscriber(DoOrigin));
	winMgr.getWindow("GameGUI/Liner")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown, 
	CEGUI::Event::Subscriber(DoLiner));
	winMgr.getWindow("GameGUI/41Liner")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown, 
	CEGUI::Event::Subscriber(Do41Liner));
	winMgr.getWindow("GameGUI/Height")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown, 
	CEGUI::Event::Subscriber(DoHeight));
	winMgr.getWindow("GameGUI/LOD")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown, 
	CEGUI::Event::Subscriber(DoLOD));

	winMgr.getWindow("GameGUI/SelectTexture")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(SelectTexture) );
	winMgr.getWindow("GameGUI/CULL")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(Cull) );
	winMgr.getWindow("GameGUI/NOCULL")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(NoCull) );
	winMgr.getWindow("GameGUI/Rain")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(CheckRain) );
	winMgr.getWindow("GameGUI/Snow")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(CheckSnow) );
	winMgr.getWindow("GameGUI/Fog")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(CheckFog) );

	winMgr.getWindow("GameGUI/RenderTexture")->subscribeEvent(
	CEGUI::Window::EventMouseButtonDown,
	CEGUI::Event::Subscriber(CheckRenderTexture) );*/

	//Scroller Bar
	/*CEGUI::Scrollbar* bar = (CEGUI::Scrollbar*)winMgr.getWindow("GameGUI/FogDimense");
	bar->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged,CEGUI::Event::Subscriber(ReadFogDimense) );*/

	return;

}
/*
bool MyCeGUI::DoOrigin(const CEGUI::EventArgs& e)
{
mAlgorithmIndex = 0;
return true;
}*/

void CeGUI::RenderGUI()
{
	CEGUI::System::getSingleton().renderGUI();
}

void CeGUI::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool rbdown = false;
	static int x;
	static int y;
	static int x_new;
	static int y_new;
	switch(msg)
	{
	case WM_RBUTTONDOWN:
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
		break;
	case WM_RBUTTONUP:
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
		break;
	case WM_MOUSEMOVE:
		x_new = LOWORD(lParam);
		y_new = HIWORD(lParam);
		//engine.GetCamera().MoveWithMouse(x_new - x,y_new - y);
		x = x_new;
		y = y_new;
		CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(x), static_cast<float>(y));
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
		//CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
		break;
	case WM_LBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
		break;
	}
}
