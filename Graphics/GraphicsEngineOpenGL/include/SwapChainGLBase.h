/*     Copyright 2019 Diligent Graphics LLC
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#pragma once

#include "SwapChainBase.h"
#include "TextureViewGLImpl.h"

namespace Diligent
{

class IMemoryAllocator;

/// Base implementation of a swap chain for OpenGL.
template <class BaseInterface>
class SwapChainGLBase : public SwapChainBase<BaseInterface>
{
public:
    using TSwapChainBase = SwapChainBase<BaseInterface>;

    SwapChainGLBase(IReferenceCounters*  pRefCounters,
                    IRenderDevice*       pDevice,
                    IDeviceContext*      pDeviceContext,
                    const SwapChainDesc& SCDesc) :
        TSwapChainBase{pRefCounters, pDevice, pDeviceContext, SCDesc}
    {}


    /// Implementation of ISwapChain::GetCurrentBackBufferRTV() in OpenGL backend.
    virtual ITextureView* GetCurrentBackBufferRTV() override final { return m_pRenderTargetView; }

    /// Implementation of ISwapChain::GetDepthBufferDSV() in OpenGL backend.
    virtual ITextureView* GetDepthBufferDSV() override final { return m_pDepthStencilView; }

protected:
    bool Resize(Uint32 NewWidth, Uint32 NewHeight, Int32 /*To be different from virtual function*/)
    {
        if (TSwapChainBase::Resize(NewWidth, NewHeight, 0))
        {
            if (m_pRenderTargetView)
            {
                if (auto pDeviceContext = m_wpDeviceContext.Lock())
                {
                    auto* pImmediateCtxGL = pDeviceContext.RawPtr<DeviceContextGLImpl>();
                    // Unbind the back buffer to be consistent with other backends
                    auto* pCurrentBackBuffer = ValidatedCast<TextureBaseGL>(m_pRenderTargetView->GetTexture());
                    auto  RenderTargetsReset = pImmediateCtxGL->UnbindTextureFromFramebuffer(pCurrentBackBuffer, false);
                    if (RenderTargetsReset)
                    {
                        LOG_INFO_MESSAGE_ONCE("Resizing the swap chain requires back and depth-stencil buffers to be unbound from the device context. "
                                              "An application should use SetRenderTargets() to restore them.");
                    }
                }
            }

            CreateDummyBuffers(m_pRenderDevice.RawPtr<RenderDeviceGLImpl>());
            return true;
        }

        return false;
    }

    void CreateDummyBuffers(RenderDeviceGLImpl* pRenderDeviceGL)
    {
        TextureDesc ColorBuffDesc;
        ColorBuffDesc.Type      = RESOURCE_DIM_TEX_2D;
        ColorBuffDesc.Name      = "Main color buffer stub";
        ColorBuffDesc.Width     = m_SwapChainDesc.Width;
        ColorBuffDesc.Height    = m_SwapChainDesc.Height;
        ColorBuffDesc.Format    = m_SwapChainDesc.ColorBufferFormat;
        ColorBuffDesc.BindFlags = BIND_RENDER_TARGET;
        RefCntAutoPtr<ITexture> pDummyColorBuffer;
        pRenderDeviceGL->CreateDummyTexture(ColorBuffDesc, RESOURCE_STATE_RENDER_TARGET, &pDummyColorBuffer);
        m_pRenderTargetView = ValidatedCast<TextureViewGLImpl>(pDummyColorBuffer->GetDefaultView(TEXTURE_VIEW_RENDER_TARGET));

        TextureDesc DepthBuffDesc = ColorBuffDesc;
        DepthBuffDesc.Name        = "Main depth buffer stub";
        DepthBuffDesc.Format      = m_SwapChainDesc.DepthBufferFormat;
        DepthBuffDesc.BindFlags   = BIND_DEPTH_STENCIL;
        RefCntAutoPtr<ITexture> pDummyDepthBuffer;
        pRenderDeviceGL->CreateDummyTexture(DepthBuffDesc, RESOURCE_STATE_DEPTH_WRITE, &pDummyDepthBuffer);
        m_pDepthStencilView = ValidatedCast<TextureViewGLImpl>(pDummyDepthBuffer->GetDefaultView(TEXTURE_VIEW_DEPTH_STENCIL));
    }

    RefCntAutoPtr<TextureViewGLImpl> m_pRenderTargetView;
    RefCntAutoPtr<TextureViewGLImpl> m_pDepthStencilView;
};

} // namespace Diligent