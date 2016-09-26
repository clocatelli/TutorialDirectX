#pragma once
#define WIN32_LEAN_AND_MEAN // Desmenuza Windows.h para que s�lo agregue lo que es necesario para desarrollar una app WIN32
//..........................// Por ejemlo no agrega recursos de criptografia, networking, etc
#include <Windows.h>
#include <string>
#include "DXUtil/DXUtil.h"

class DXApp
{
public:
    DXApp(HINSTANCE hInstance );
    virtual ~DXApp();

    //MAIN APPLICATION LOOP
    int Run();

    //FRAMEWORK METHOS
    virtual bool Init();
    virtual void Update(float dt) = 0; // Se les agrega un delta t, dado que ser�n actualizados en determinados momentos
    virtual void Render(float dt) = 0; // Ac� sucede lo mismo. 
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // Este m�todo es muy importante, es el Windows Message
    //..........................................................................// procedure

protected:

    //WIN32 ATTRIBUTES
    HWND            m_hAppWnd;      // Es un handle a la ventana de mi application.
    HINSTANCE       m_hAppInstance; // Es un handle a la instancia real de la aplicaci�n.
    UINT            m_ClientWidth;  // Ser� el ancho de la ventana de mi aplicaci�n. Es un unsigned int
    UINT            m_ClientHeight; // Ser� el alto de la ventana de mi aplicaci�n. Es un unsigned int
    std::string     m_AppTitle;     // El string que ser� mostrado en la barra del t�tulo.
    DWORD           m_WndStyle;     // Ser� el estilo de mi ventana. Es un unsigned long.

    //DIRECTX ATTRIBUTES
    ID3D11Device*           m_pDevice;
    ID3D11DeviceContext*    m_pImmediateContext;
    IDXGISwapChain*         m_pSwapChain;
    ID3D11RenderTargetView* m_pRenderTargetView;
    D3D_DRIVER_TYPE         m_DriverType;
    D3D_FEATURE_LEVEL       m_FeatureLevel;
    D3D11_VIEWPORT          m_Viewport;

protected:

    // INITIALIZE WIN32 WINDOW
    bool InitWindow();

    // INITIALIZE DIRECT3D
    bool InitDirect3D();

};
 