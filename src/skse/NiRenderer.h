#pragma once

#include <d3d11_4.h>
#include <dxgi1_5.h>

// Unknown class name, No RTTI
class BSRenderManager
{
public:
    static BSRenderManager* GetSingleton();

    uint64_t                   unk00[0x48 >> 3]; // 00
    ID3D11Device*              forwarder;        // 48 - Actually CID3D11Forwarder
    ID3D11DeviceContext*       context;          // 50 - ID3D11DeviceContext4
    uint64_t                   unk58;            // 58
    uint64_t                   unk60;            // 60
    uint64_t                   unk68;            // 68
    IDXGISwapChain4*           swapChain;        // 70
    uint64_t                   unk78;            // 78
    uint64_t                   unk80;            // 80
    ID3D11RenderTargetView1*   renderView;       // 88
    ID3D11ShaderResourceView1* resourceView;     // 90
    uint64_t                   unk2788[(0x2788 - 0x90) >> 3];
    CRITICAL_SECTION           lock; // 2790
};