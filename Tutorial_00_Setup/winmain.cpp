#include <memory>

#include "DXApp.h"
#include "SpriteBatch.h"        // DEL TOOLKIT DE DIRECTX
#include "SpriteFont.h"         // DEL TOOLKIT DE DIRECTX
#include "DDSTextureLoader.h"
#include "SimpleMath.h"
#include "Sprite.h"

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
    std::unique_ptr<DirectX::SpriteFont> spriteFont;
    
    Sprite* sprite;


};

TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{

}

TestApp::~TestApp()
{
    Memory::SafeDelete(sprite);
}

bool TestApp::Init()
{
    if (!DXApp::Init())
        return false;

    // CREATE SPRITEBATCH OBJECTS
    spriteBatch.reset(new DirectX::SpriteBatch(m_pImmediateContext));

    // CREATE SPRITEFONT OBJECT
    spriteFont.reset(new DirectX::SpriteFont(m_pDevice, L"Arial.spritefont"));

    sprite = new Sprite(DirectX::SimpleMath::Vector2(100, 100));
    sprite->Load(m_pDevice, L"Test.dds");

    return true;
}

void TestApp::Update(float dt)
{
    if (GetAsyncKeyState('D'))
        sprite->SetPosition(DirectX::SimpleMath::Vector2(300, 300));
}

void TestApp::Render(float dt)
{
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Purple);

    spriteBatch->Begin();

    // DRAW SPRITE
    sprite->Draw(spriteBatch.get());

    // DRAW FONT
    spriteFont->DrawString(spriteBatch.get(), L"Hello, World", DirectX::SimpleMath::Vector2(300, 300));

    spriteBatch->End();


    HR(m_pSwapChain->Present(0, 0));
}

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
    TestApp tApp(hInstance);

    if (!tApp.Init()) return 1;

    return tApp.Run();
}