#include "DXApp.h"

namespace
{
    // Tenemos que encontrar una forma de levantar nuestros mensajes de una función callback global a nuestra función definida
    // GLOBAL POINTER USED TO FORWAR MESSAGES TO USER DEFINED PROCEDURE FUNCTION
    DXApp* g_pApp = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (g_pApp) return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
    else return DefWindowProc(hwnd, msg, wParam, lParam);
}

DXApp::DXApp(HINSTANCE hInstance)
{
    m_hAppInstance = hInstance;
    m_hAppWnd = nullptr;
    m_ClientWidth = 800;
    m_ClientHeight = 600;
    m_AppTitle = "TUTORIAL 00: WIN32 SETUP";
    m_WndStyle = WS_OVERLAPPEDWINDOW;
}


DXApp::~DXApp()
{
}

int DXApp::Run()
{
    // MAIN MESSAGE LOOP
    MSG msg = { 0 };    // En Run vamos a manejar todos los mensajes que son enviados de la aplicación al OS. Ya que Windows se maneja
    //..................// basicamente con mensajes que son enviados y recibidos por windows.
    while (msg.message != WM_QUIT) // WM es por Windows Message. Mientras el tipo de mensaje tipo quit no sea igual al mensaje hacemos
    //.............................// loop, cuando sean iguales, salimos del loop, y termina la app.                            
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // PM_REMOVE va a mirar el mensaje, y lo va a eliminar del stack. Ya que fue
            //...........................................// Analizado, y no lo vamos a necesitar más.
        {
            TranslateMessage(&msg); 
            DispatchMessage(&msg);
        }
        else
        {
            //Update
            Update(0.0f);
            //Render
            Render(0.0f);
        }
    }

    return static_cast<int>(msg.wParam); // Si salimos del loop, vamos a pasar el exit code que va a estar almacenado en wParam.
}

bool DXApp::Init()
{
    if (!InitWindow())
        return false;
    return true;
}

bool DXApp::InitWindow()
{
    // Lo primero que necesitamos cuando se crea una WIN32 desktop app es una clase de ventana que define algunas propiedades de la ventana
    // WNDCLASSEX
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW; // Horizontal and Vertical redraw
    wcex.hInstance = m_hAppInstance;
    wcex.lpfnWndProc = MainWndProc;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "DXAPPWNDCLASS";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        OutputDebugString("\nFAILED TO CREATE WINDOW CLASS\n");
        return false;
    }

    RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&r, m_WndStyle, FALSE);
    UINT width = r.right - r.left;
    UINT height = r.bottom - r.top;

    UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
    
    m_hAppWnd = CreateWindow("DXAPPWNDCLASS", m_AppTitle.c_str(), m_WndStyle, x, y, 
                            width, height, nullptr, nullptr, m_hAppInstance, nullptr);

    if (!m_hAppWnd)
    {
        OutputDebugString("\nFAILED TO CREATE WINDOW\n");
        return false;
    }

    ShowWindow(m_hAppWnd, SW_SHOW);
    
    return true;

}

// Acá es donde realmente implementamos lo que haremos cuando recibimos mensajes específicos
// ESTA SE SUPONE QUE DEBE SER DEFINIDA COMO UNA CALLBACK FUNCTION. PERO NO SE PUEDEN DEFINIR MEMBER FUNCTIONS COMO CALLBACK.
LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:            // Este mensaje se envía cuando presionamos X (close)
        PostQuitMessage(0);     // Esta función postea un quit message en el pipeline, que luego lo recuperamos en run.
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);


    }
}

