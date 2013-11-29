#ifndef _CE_GUI_H
#define _CE_GUI_H

#include <d3dx9.h>
#include "CEGUI.h"
#include "d3d9renderer.h"
#include "CEGUIDefaultResourceProvider.h"

class CeGUI
{
private:
	CEGUI::DirectX9Renderer * m_pCEGUIRender;	///< CEGUI Render
public:
	//static bool DoOrigin(const CEGUI::EventArgs& e);
	void InitGUI(LPDIRECT3DDEVICE9 m_d3dDevice);	
	//渲染GUI
	void RenderGUI();
	//得到GUI的操作 即注入鼠标操作
	void HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};


#endif