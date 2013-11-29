//-----------------------------------------------------------------------------
// File: Matrices.cpp
//
// Desc: Now that we know how to create a device and render some 2D vertices,
//       this tutorial goes the next step and renders 3D geometry. To deal with
//       3D geometry we need to introduce the use of 4x4 Matrices to transform
//       the geometry with translations, rotations, scaling, and setting up our
//       camera.
//       Geometry is defined in model space. We can move it (translation),
//       rotate it (rotation), or stretch it (scaling) using a world transform.
//       The geometry is then said to be in world space. Next, we need to
//       position the camera, or eye point, somewhere to look at the geometry.
//       Another transform, via the view matrix, is used, to position and
//       rotate our view. With the geometry then in view space, our last
//       transform is the projection transform, which "projects" the 3D scene
//       into our 2D viewport.
//
//       Note that in this tutorial, we are introducing the use of D3DX, which
//       is a set of helper utilities for D3D. In this case, we are using some
//       of D3DX's useful matrix initialization functions. To use D3DX, simply
//       include <d3dx9.h> and link with d3dx9.lib.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <assert.h>
#include "my_CEGUI.h"
#include "camera.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices

LPD3DXMESH		g_pMesh = NULL;
D3DMATERIAL9* 		g_pMeshMaterials = NULL;
LPDIRECT3DTEXTURE9* 	g_pMeshTextures = NULL;
DWORD			g_dwNumMaterials = 0L;

CeGUI g_gui;
CCamera g_camera;
// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	DWORD color;        // The vertex color
};
// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

HRESULT InitD3D( HWND hWnd )
{
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}
	//g_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_gui.InitGUI(g_pd3dDevice);
	//这里打开一个控制台
	AllocConsole();

	freopen("conin$","r+t",stdin);

	freopen("conout$","w+t",stdout);

	return S_OK;

}
IDirect3DIndexBuffer9*    g_index_buffer;

HRESULT InitGeometry()
{
	static  CUSTOMVERTEX g_Vertices[3] = {
		10.0,0.0,0.0,0xffff0000,
		0.0,10.0,0.0,0xff00ff00,
		0.0,0.0,10.0,0xff0000ff
	};

	if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3 * sizeof( CUSTOMVERTEX ),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
	{
		return E_FAIL;
	}
	VOID* pVertices;
	if( FAILED( g_pVB->Lock( 0, sizeof( g_Vertices ), ( void** )&pVertices, 0 ) ) )
		return E_FAIL;
	memcpy( pVertices, g_Vertices, sizeof( g_Vertices ) );
	g_pVB->Unlock();
	WORD indices[64 * 6];
	//InitIndexBuffer(indices);
	//g_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_index_buffer,NULL);
	//拷贝Index
	//void* index_ptr;
	//g_index_buffer->Lock(0, sizeof(indices), (void**)&index_ptr, 0);
	//memcpy(index_ptr, indices, sizeof(indices));
	//g_index_buffer->Unlock();
	LPD3DXBUFFER pD3DXMtrlBuffer;
	if(FAILED(D3DXLoadMeshFromX(L"c:\\earth.X",D3DXMESH_SYSTEMMEM,
		g_pd3dDevice,NULL,&pD3DXMtrlBuffer,NULL,&g_dwNumMaterials,&g_pMesh) ) )
	{
		MessageBox(NULL,L"Could not find earth.x",L"Error!",MB_OK);
		return E_FAIL;
	};
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
	if(g_pMeshMaterials == NULL)
		return E_OUTOFMEMORY;
	g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
    if( g_pMeshTextures == NULL )
        return E_OUTOFMEMORY;

	for(DWORD i = 0;i < g_dwNumMaterials; i++)
	{
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
		g_pMeshTextures[i] = NULL;
		if(d3dxMaterials[i].pTextureFilename != NULL && 
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if(FAILED(D3DXCreateTextureFromFileA(g_pd3dDevice,d3dxMaterials[i].pTextureFilename,&g_pMeshTextures[i])))
			{
				const CHAR* strPrefix = "c:\\";
				CHAR strTexture[1024];
				strcpy_s(strTexture,1024,strPrefix);
				strcat_s(strTexture,104,d3dxMaterials[i].pTextureFilename);
				if(FAILED(D3DXCreateTextureFromFileA(g_pd3dDevice,strTexture,&g_pMeshTextures[i])))
				{
					MessageBox(NULL,L"Couldn't find texture map",L"Error!",MB_OK);
				}
			}
		}
	}
		pD3DXMtrlBuffer->Release();
		return S_OK;
}
VOID Cleanup()
{
	if(g_pMeshMaterials != NULL)
		delete[] g_pMeshMaterials;
	if(g_pMeshTextures)
	{
		for(DWORD i = 0;i < g_dwNumMaterials;i++)
		{
			if(g_pMeshTextures[i])
				g_pMeshTextures[i]->Release();
		}
		delete[] g_pMeshTextures;
	}
	if(g_pMesh != NULL)
		g_pMesh->Release();


	if( g_pVB != NULL )
		g_pVB->Release();

	if( g_pd3dDevice != NULL )
		g_pd3dDevice->Release();

	if( g_pD3D != NULL )
		g_pD3D->Release();
}

VOID Render()
{
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{

		// SetupMatrices();
		//g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ); 


		//g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
		//g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
		//g_pd3dDevice->SetIndices(g_index_buffer);

		D3DXMATRIX mWorldMatrix;

		D3DXMatrixIdentity( &mWorldMatrix );
		D3DXMatrixTranslation(&mWorldMatrix,0.0f,0.0f,0.0f);
		//转一下
		static FLOAT arc = 0.0;
		D3DXMatrixRotationY(&mWorldMatrix,arc);
		arc -= 0.02;
		if(arc <= -360)
			arc = 0;

		g_pd3dDevice->SetTransform(D3DTS_WORLD,&mWorldMatrix);

		g_camera.ApplyDevice(g_pd3dDevice);

		HRESULT hr;
		//		D3DXMatrixTranslation( &matWorld, x*interval_x_b,y*interval_y_b,0);
		//g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		//g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);
		//hr = g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,81,0,128);

		//Draw The earth 
		//g_dwNumMaterials
		for(DWORD i = 0;i < 1;i++)
		{
			g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
			g_pd3dDevice->SetTexture(0,g_pMeshTextures[i]);
			g_pMesh->DrawSubset(i);
		}
		g_gui.RenderGUI();


		g_pd3dDevice->EndScene();
	}
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	}
	g_gui.HandleMessage(hWnd,msg,wParam,lParam);
	g_camera.HandleMessage(hWnd,msg,wParam,lParam);
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	UNREFERENCED_PARAMETER( hInst );
	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL
	};
	RegisterClassEx( &wc );

	// Create the application's window
	HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 03: Matrices",
		WS_OVERLAPPEDWINDOW, 100, 100, 800, 600,
		NULL, NULL, wc.hInstance, NULL );
	// Initialize Direct3D
	if( SUCCEEDED( InitD3D( hWnd ) ) )
	{
		// Create the scene geometry
		if( SUCCEEDED( InitGeometry() ) )
		{
			// Show the window
			ShowWindow( hWnd, SW_SHOWDEFAULT );
			UpdateWindow( hWnd );

			// Enter the message loop
			MSG msg;
			ZeroMemory( &msg, sizeof( msg ) );
			while( msg.message != WM_QUIT )
			{
				if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
				else
					Render();
			}
		}
	}
	UnregisterClass( L"D3D Tutorial", wc.hInstance );
	return 0;
}



