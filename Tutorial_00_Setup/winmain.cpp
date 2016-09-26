#include <memory>

#include "DXApp.h"
#include "SpriteBatch.h"        // DEL TOOLKIT DE DIRECTX

class TestApp : public DXApp
{
public:
    TestApp(HINSTANCE hInstance);
    ~TestApp();

    bool Init() override;
    void Update(float dt) override;
    void Render(float dt) override;

private:

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

};

TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{

}

TestApp::~TestApp()
{
}

bool TestApp::Init()
{
    if (!DXApp::Init())
        return false;

    spriteBatch.reset(new DirectX::SpriteBatch(m_pImmediateContext));

    return true;
}

void TestApp::Update(float dt)
{

}

void TestApp::Render(float dt)
{
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Purple);

    spriteBatch->Begin();

    // DRAW SPRITE, FONTS, ETC.
    

    spriteBatch->End();


    HR(m_pSwapChain->Present(0, 0));
}

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
    TestApp tApp(hInstance);

    if (!tApp.Init()) return 1;

    return tApp.Run();
}