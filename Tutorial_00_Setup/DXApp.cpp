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
    m_AppTitle = "TUTORIAL 02 - DIRECTX TOOLKIT";
    m_WndStyle = WS_OVERLAPPEDWINDOW;
    g_pApp = this;

    m_pDevice = nullptr;
    m_pImmediateContext = nullptr;
    m_pSwapChain = nullptr;
    m_pRenderTargetView = nullptr;

}

DXApp::~DXApp()
{
    // CLEANUP DIRECT3D
    if (m_pImmediateContext) m_pImmediateContext->ClearState();
    Memory::SafeRelease(m_pRenderTargetView);
    Memory::SafeRelease(m_pSwapChain);
    Memory::SafeRelease(m_pImmediateContext);
    Memory::SafeRelease(m_pDevice);

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

    if (!InitDirect3D())
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

bool DXApp::InitDirect3D()
{
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC swapDesc;
    ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapDesc.BufferCount = 1;   // Double buffered
    swapDesc.BufferDesc.Width = m_ClientWidth;
    swapDesc.BufferDesc.Height = m_ClientHeight;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = m_hAppWnd;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Windowed = true;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // alt-enter fullscreen

    HRESULT result;
    for (UINT i = 0; i < numDriverTypes; ++i)
    {
        result = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[i], 0, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &swapDesc, &m_pSwapChain, &m_pDevice, &m_FeatureLevel,
            &m_pImmediateContext);

        if (SUCCEEDED(result))
        {
            m_DriverType = driverTypes[i];
            break;
        }

        if (FAILED(result))
        {
            OutputDebugString("FAILED TO CREATE DEVICE AND SWAP CHAIN");
            return false;
        }
    }

    // CREATE RENDER TARGET VIEW
    ID3D11Texture2D* m_pBackBufferTex = 0;
    HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBufferTex)));
    HR(m_pDevice->CreateRenderTargetView(m_pBackBufferTex, nullptr, &m_pRenderTargetView));
    Memory::SafeRelease(m_pBackBufferTex);

    // BIND RENDER TARGET VIEW
    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // VIEWPORT CREATION
    m_Viewport.Width = static_cast<float>(m_ClientWidth);
    m_Viewport.Height = static_cast<float>(m_ClientHeight);
    m_Viewport.TopLeftX = 0;
    m_Viewport.TopLeftY = 0;
    m_Viewport.MinDepth = 0.0f;
    m_Viewport.MaxDepth = 1.0f;
    
    // BIND VIEWPORT
    m_pImmediateContext->RSSetViewports(1, &m_Viewport);

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