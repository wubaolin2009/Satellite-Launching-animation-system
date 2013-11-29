//MyGui.h 从MyGUISystem这里不做继承
#ifndef _MY_GUI_H
#define _MY_GUI_H
#include "MyGUISystem.h"
class MyGUI
{
public:
	static MyGUI* getSingletonPtr()  
	{  
		if(ms_singleton == NULL)  
		{  
			ms_singleton = new MyGUI();  
		}  
		return ms_singleton;  
	}  
	bool init();
	static void Destroy()
	{
		if(ms_singleton){
			delete ms_singleton;
		}
		ms_singleton = NULL;
	}
	void update(float timeSinceLastFrame); 
private:
	static MyGUI* ms_singleton;
};


#endif