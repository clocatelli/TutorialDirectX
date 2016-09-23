#include "DXApp.h"


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