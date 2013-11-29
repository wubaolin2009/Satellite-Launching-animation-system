#include "OgreMath.h"
#include "OgreCamera.h"
#include "OgreVector3.h"
#include "MyGUI.h"
#include "OgreUTFString.h"
#include "CEGUIWindowManager.h"
#include "elements\CEGUIListbox.h"
#include "elements\CEGUIListboxItem.h"
#include "elements\CEGUICombobox.h"
#include "elements\CEGUICheckbox.h"
#include "CEGUIEventArgs.h"
#include "satellite.h"

MyGUI* MyGUI::ms_singleton = NULL;

extern Ogre::Camera* g_camera;

//将中文转换成CEGUI::Strign
CEGUI::String GetUTF(wchar_t* str)
{
	CEGUI::String wstr = (CEGUI::utf8*)Ogre::UTFString(str).asUTF8_c_str();
	return wstr;
}

//下边是事件的处理函数,这里也不做什么面向对象设计了能用就可以,因为工作还有很多很多
//选择视角
bool EventSelectView(const CEGUI::EventArgs& e)
{
	//g_camera;
	CEGUI::Combobox* win = (CEGUI::Combobox*)(((const CEGUI::WindowEventArgs&)e).window);
	if(win->isItemSelected(2) ){
		//MessageBox(NULL,NULL,NULL,NULL);
		g_camera->setPosition(0,1200,0);
		g_camera->lookAt(-10,1200,0);
		//Ogre::Quaternion qa;
		//qa.FromAngleAxis(Ogre::Radian(Ogre::Degree(90)),Ogre::Vector3::UNIT_X);
		//g_camera->rotate(qa);
		//g_camera->setPosition(0,1200,0);
		//g_camera->lookAt(0,0,0);
	}

	return true;
}
bool EventSelectNP(const CEGUI::EventArgs& e)
{

	return true;
}

//是否显示轨迹
bool EventShowTrack(const CEGUI::EventArgs& e)
{ 
	if( ((CEGUI::Checkbox*)((const CEGUI::WindowEventArgs&)e).window)->isSelected() ){	
		SatelliteStatus::show_track = true;
	}
	else{
		SatelliteStatus::show_track = false;
	}
	return true;
}
//发射卫星
bool EventLaunchSatellite(const CEGUI::EventArgs& e)
{
	if(SatelliteStatus::current_status != SatelliteStatus::UNLAUNCHED){
		//TODO: Anouce BUG
		return true;
	}
	SatelliteStatus::current_status = SatelliteStatus::RISING;
	return true;
}
//近地点加速
bool EventSpeedUp(const CEGUI::EventArgs& e)
{
	/*if(SatelliteStatus::current_status != SatelliteStatus::NEAR_TRACK){
		//TODO: Anouce BUG
		return true;
	}
	SatelliteStatus::current_status = SatelliteStatus::ECLLIPSE;*/
	if(SatelliteStatus::current_status != SatelliteStatus::UNLAUNCHED){
		return false;
	}
	//Read the Data and set it
	double near_ = 468.55;
	double far_ = 800.00;
	CEGUI::WindowManager* winMgr = 	CEGUI::WindowManager::getSingletonPtr(); 
	CEGUI::String& strnear = const_cast<CEGUI::String&>(winMgr->getWindow("edit_near")->getText() );
	CEGUI::String& strfar  = const_cast<CEGUI::String&>(winMgr->getWindow("edit_far")->getText() );
	near_ = atof(strnear.c_str() );
	far_ = atof(strfar.c_str() );
	SetParameter(near_,far_);
	return true;
}



bool MyGUI::init()
{
	bool ret_value = MyGUISystem::getSingletonPtr()->init();
	//加载窗口
	if(ret_value){
		MyGUISystem::getSingletonPtr()->loadLayout("GameGUI");
		//设置按钮等文本为中文
		// 得到窗口管理单件
		CEGUI::WindowManager* winMgr = 	CEGUI::WindowManager::getSingletonPtr(); 
		//bool is_presented = winMgr->isWindowPresent("WindowControl");
		//CEGUI::Window* win = winMgr->getWindow("GameGUI");
	
		//int mmm = 0;
		//winMgr->getWindow("WindowControl")->setText(GetUTF(L"控制选项:") );
		
		winMgr->getWindow("WindowControl_text_near")->setText(GetUTF(L"近地轨道半径(10km):") );
		winMgr->getWindow("WindowControl_text_far")->setText(GetUTF(L"远地轨道半径(10km):") );
		winMgr->getWindow("button_launch")->setText(GetUTF(L"发射卫星") );
		winMgr->getWindow("button_speed_up")->setText(GetUTF(L"更新参数"));
		winMgr->getWindow("edit_near")->setText(GetUTF(L"468.55") );
		winMgr->getWindow("edit_far")->setText(GetUTF(L"800.00"));

		//winMgr->getWindow("WindowRender")->setText(GetUTF(L"渲染选项:") );
		winMgr->getWindow("select_viewport")->setText(GetUTF(L"视角:") );
		winMgr->getWindow("select_show_track")->setText(GetUTF(L"显示卫星轨迹") );
		
		//winMgr->getWindow("WindowShow")->setText(GetUTF(L"参数显示:") );
		winMgr->getWindow("text_param")->setText(GetUTF(L"卫星参数:") );
		winMgr->getWindow("text_st")->setText(GetUTF(L"卫星状态") );
		winMgr->getWindow("text_status")->setText(GetUTF(L"未发射") );
		winMgr->getWindow("text_coords")->setText(GetUTF(L"卫星坐标(10km):") );
		winMgr->getWindow("text_coords_x")->setText(GetUTF(L"X坐标:") );
		winMgr->getWindow("text_coords_y")->setText(GetUTF(L"Y坐标:") );
		winMgr->getWindow("text_coords_z")->setText(GetUTF(L"Z坐标:") );
		winMgr->getWindow("text_x")->setText(GetUTF(L"0") );
		winMgr->getWindow("text_y")->setText(GetUTF(L"0") );
		winMgr->getWindow("text_z")->setText(GetUTF(L"0") );
		winMgr->getWindow("text_axis")->setText(GetUTF(L"卫星自转角:") );
		winMgr->getWindow("text_axisaxis")->setText(GetUTF(L"0") );
		winMgr->getWindow("text_lv")->setText(GetUTF(L"卫星线速度:") );
		winMgr->getWindow("textwv")->setText(GetUTF(L"卫星角速度:") );
		winMgr->getWindow("text_a")->setText(GetUTF(L"卫星加速度:") );
		winMgr->getWindow("text_lvlv")->setText(GetUTF(L"0 km/h") );
		winMgr->getWindow("text_wvwv")->setText(GetUTF(L"0 /h") );
		winMgr->getWindow("text_aa")->setText(GetUTF(L"0 km/h2") );

		//设置“选择视角”的ListBox
		CEGUI::ListboxTextItem* item1 = new CEGUI::ListboxTextItem((GetUTF(L"大气层内")),0 );
		CEGUI::ListboxTextItem* item2 = new CEGUI::ListboxTextItem((GetUTF(L"外太空")),1 );
		CEGUI::ListboxTextItem* item3 = new CEGUI::ListboxTextItem((GetUTF(L"南极上方")),2 );
		CEGUI::ListboxTextItem* item4 = new CEGUI::ListboxTextItem((GetUTF(L"自由视角")),3 );
		CEGUI::Combobox* list_box_select_viewport = (CEGUI::Combobox*)(winMgr->getWindow("select_viewport") );
		list_box_select_viewport->addItem(item1);
		list_box_select_viewport->addItem(item2);
		list_box_select_viewport->addItem(item3);
		list_box_select_viewport->addItem(item4);

		//设置“显示轨迹”的Checkbox
		CEGUI::Checkbox* check_box = (CEGUI::Checkbox*)(winMgr->getWindow("select_show_track") );
		check_box->setSelected(false);
		
		//设置
		//这次注册一些事件处理函数等
		//“选择视角”的ListBox
		//item3->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(EventSelectNP));
		list_box_select_viewport->subscribeEvent(CEGUI::Combobox::EventTextSelectionChanged,CEGUI::Event::Subscriber(EventSelectView));

		//设置“显示轨迹的”CheckBox
		check_box->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged,CEGUI::Event::Subscriber(EventShowTrack));

		//发射卫星的Button
		winMgr->getWindow("button_launch")->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(EventLaunchSatellite));
		//近地轨道加速的Button
		winMgr->getWindow("button_speed_up")->subscribeEvent(CEGUI::Window::EventMouseButtonDown,CEGUI::Event::Subscriber(EventSpeedUp));
	}
	return ret_value;
}

void MyGUI::update(float timeSinceLastFrame) 
{
	//在這裡更新衛星的狀態
	CEGUI::WindowManager* winMgr = 	CEGUI::WindowManager::getSingletonPtr(); 
	//衛星的狀態
	CEGUI::String str_status;
	if(SatelliteStatus::current_status == SatelliteStatus::UNLAUNCHED){
		str_status = GetUTF(L"未发射");
	}
	else if(SatelliteStatus::current_status == SatelliteStatus::RISING){
		str_status = GetUTF(L"上升中");
	}
	else if(SatelliteStatus::current_status == SatelliteStatus::NEAR_TRACK){
		str_status = GetUTF(L"近地轨道");
	}
	else if(SatelliteStatus::current_status == SatelliteStatus::ECLLIPSE){
		str_status = GetUTF(L"椭圆轨道");
	}
	else if(SatelliteStatus::current_status == SatelliteStatus::FAR_TRACK){
		str_status = GetUTF(L"远地轨道");
	}
	else if(SatelliteStatus::current_status == SatelliteStatus::OUT_CONTROL){
		str_status = GetUTF(L"失控");
	}
	winMgr->getWindow("text_status")->setText(str_status);

	//卫星的坐标
	char buffer0[1024];
	char buffer1[1024];
	char buffer2[1024];
	sprintf(buffer0,"%f",SatelliteStatus::satellite_x);
	sprintf(buffer1,"%f",SatelliteStatus::satellite_y);
	sprintf(buffer2,"%f",SatelliteStatus::satellite_z);
	CEGUI::String strx(buffer0);
	CEGUI::String stry(buffer1);
	CEGUI::String strz(buffer2);
	winMgr->getWindow("text_x")->setText(strx);
	winMgr->getWindow("text_y")->setText(stry);
	winMgr->getWindow("text_z")->setText(strz);
	//卫星自转角
	sprintf(buffer0,"%f",SatelliteStatus::satellite_angle);
	CEGUI::String str_rotate(buffer0);
	winMgr->getWindow("text_axisaxis")->setText(strx);
	//线速度
	sprintf(buffer0,"%f km/h",SatelliteStatus::satellite_lv);
	CEGUI::String str_lv(buffer0);
	winMgr->getWindow("text_lvlv")->setText(str_lv);
	//角速度
	sprintf(buffer0,"%f /h",SatelliteStatus::satellite_wv);
	CEGUI::String str_wv(buffer0);
	winMgr->getWindow("text_wvwv")->setText(str_wv);
	//线加速度
	sprintf(buffer0,"%f /h^2",SatelliteStatus::satellite_a);
	CEGUI::String str_a(buffer0);
	winMgr->getWindow("text_aa")->setText(str_a);

	MyGUISystem::getSingletonPtr()->update(timeSinceLastFrame);
}