/*
 *  Copyright 2019-2022 Diligent Graphics LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

#include <vector>
#include <memory>

#include "ArchiverFactory.h"

#include "SerializationEngineImplTraits.hpp"
#include "ObjectBase.hpp"
#include "DXCompiler.hpp"
#include "RenderDeviceBase.hpp"

namespace Diligent
{

class SerializationDeviceImpl final : public RenderDeviceBase<SerializationEngineImplTraits>
{
public:
    using TBase = RenderDeviceBase<SerializationEngineImplTraits>;

    SerializationDeviceImpl(IReferenceCounters* pRefCounters, const SerializationDeviceCreateInfo& CreateInfo);
    ~SerializationDeviceImpl();

    virtual void DILIGENT_CALL_TYPE QueryInterface(const INTERFACE_ID& IID, IObject** ppInterface) override final;

#define UNSUPPORTED_METHOD(MethodName, ...)                                   \
    virtual void DILIGENT_CALL_TYPE MethodName(__VA_ARGS__) override final    \
    {                                                                         \
        UNSUPPORTED(#MethodName " is not supported by serialization device"); \
    }

    // clang-format off
    UNSUPPORTED_METHOD(CreateGraphicsPipelineState,   const GraphicsPipelineStateCreateInfo&   PSOCreateInfo, IPipelineState** ppPipelineState)
    UNSUPPORTED_METHOD(CreateComputePipelineState,    const ComputePipelineStateCreateInfo&    PSOCreateInfo, IPipelineState** ppPipelineState)
    UNSUPPORTED_METHOD(CreateRayTracingPipelineState, const RayTracingPipelineStateCreateInfo& PSOCreateInfo, IPipelineState** ppPipelineState)
    UNSUPPORTED_METHOD(CreateTilePipelineState,       const TilePipelineStateCreateInfo&       PSOCreateInfo, IPipelineState** ppPipelineState)

    UNSUPPORTED_METHOD(CreateShader,      const ShaderCreateInfo&  CreateInfo, IShader** ppShader)

    UNSUPPORTED_METHOD(CreateBuffer,      const BufferDesc&  Desc, const BufferData*  pData, IBuffer**  ppBuffer)
    UNSUPPORTED_METHOD(CreateTexture,     const TextureDesc& Desc, const TextureData* pData, ITexture** ppTexture)

    UNSUPPORTED_METHOD(CreateSampler,     const SamplerDesc&            Desc, ISampler**            ppSampler)
    UNSUPPORTED_METHOD(CreateFence,       const FenceDesc&              Desc, IFence**              ppFence)
    UNSUPPORTED_METHOD(CreateQuery,       const QueryDesc&              Desc, IQuery**              ppQuery)
    UNSUPPORTED_METHOD(CreateFramebuffer, const FramebufferDesc&        Desc, IFramebuffer**        ppFramebuffer)
    UNSUPPORTED_METHOD(CreateBLAS,        const BottomLevelASDesc&      Desc, IBottomLevelAS**      ppBLAS)
    UNSUPPORTED_METHOD(CreateTLAS,        const TopLevelASDesc&         Desc, ITopLevelAS**         ppTLAS)
    UNSUPPORTED_METHOD(CreateSBT,         const ShaderBindingTableDesc& Desc, IShaderBindingTable** ppSBT)
    UNSUPPORTED_METHOD(CreatePipelineResourceSignature, const PipelineResourceSignatureDesc& Desc, IPipelineResourceSignature** ppSignature)
    UNSUPPORTED_METHOD(CreateDeviceMemory,       const DeviceMemoryCreateInfo&       CreateInfo, IDeviceMemory**       ppMemory)
    UNSUPPORTED_METHOD(CreatePipelineStateCache, const PipelineStateCacheCreateInfo& CreateInfo, IPipelineStateCache** ppPSOCache)
    UNSUPPORTED_METHOD(IdleGPU)
    UNSUPPORTED_METHOD(ReleaseStaleResources, bool ForceRelease)
    // clang-format on
#undef UNSUPPORTED_METHOD

    /// Implementation of IRenderDevice::CreateRenderPass().
    virtual void DILIGENT_CALL_TYPE CreateRenderPass(const RenderPassDesc& Desc,
                                                     IRenderPass**         ppRenderPass) override final;

    /// Implementation of IRenderDevice::GetSparseTextureFormatInfo().
    virtual SparseTextureFormatInfo DILIGENT_CALL_TYPE GetSparseTextureFormatInfo(TEXTURE_FORMAT     TexFormat,
                                                                                  RESOURCE_DIMENSION Dimension,
                                                                                  Uint32             SampleCount) const override final
    {
        UNSUPPORTED("GetSparseTextureFormatInfo is not supported by serialization device");
        return SparseTextureFormatInfo{};
    }

    /// Implementation of ISerializationDevice::CreateShader().
    virtual void DILIGENT_CALL_TYPE CreateShader(const ShaderCreateInfo&  ShaderCI,
                                                 const ShaderArchiveInfo& ArchiveInfo,
                                                 IShader**                ppShader) override final;

    /// Implementation of ISerializationDevice::CreatePipelineResourceSignature().
    virtual void DILIGENT_CALL_TYPE CreatePipelineResourceSignature(const PipelineResourceSignatureDesc& Desc,
                                                                    const ResourceSignatureArchiveInfo&  ArchiveInfo,
                                                                    IPipelineResourceSignature**         ppSignature) override final;

    /// Implementation of ISerializationDevice::CreateGraphicsPipelineState().
    virtual void DILIGENT_CALL_TYPE CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& PSOCreateInfo,
                                                                const PipelineStateArchiveInfo&        ArchiveInfo,
                                                                IPipelineState**                       ppPipelineState) override final;

    /// Implementation of ISerializationDevice::CreateComputePipelineState().
    virtual void DILIGENT_CALL_TYPE CreateComputePipelineState(const ComputePipelineStateCreateInfo& PSOCreateInfo,
                                                               const PipelineStateArchiveInfo&       ArchiveInfo,
                                                               IPipelineState**                      ppPipelineState) override final;

    /// Implementation of ISerializationDevice::CreateRayTracingPipelineState().
    virtual void DILIGENT_CALL_TYPE CreateRayTracingPipelineState(const RayTracingPipelineStateCreateInfo& PSOCreateInfo,
                                                                  const PipelineStateArchiveInfo&          ArchiveInfo,
                                                                  IPipelineState**                         ppPipelineState) override final;

    /// Implementation of ISerializationDevice::CreateTilePipelineState().
    virtual void DILIGENT_CALL_TYPE CreateTilePipelineState(const TilePipelineStateCreateInfo& PSOCreateInfo,
                                                            const PipelineStateArchiveInfo&    ArchiveInfo,
                                                            IPipelineState**                   ppPipelineState) override final;

    void CreateSerializedResourceSignature(const PipelineResourceSignatureDesc& Desc,
                                           const ResourceSignatureArchiveInfo&  ArchiveInfo,
                                           SHADER_TYPE                          ShaderStages,
                                           SerializedResourceSignatureImpl**    ppSignature);

    void CreateSerializedResourceSignature(SerializedResourceSignatureImpl** ppSignature, const char* Name);

    virtual void DILIGENT_CALL_TYPE GetPipelineResourceBindings(const PipelineResourceBindingAttribs& Attribs,
                                                                Uint32&                               NumBindings,
                                                                const PipelineResourceBinding*&       pBindings) override final;

    struct D3D11Properties
    {
        Uint32 FeatureLevel = 0;
    };

    struct D3D12Properties
    {
        IDXCompiler* pDxCompiler = nullptr;
        Version      ShaderVersion;
    };

    struct VkProperties
    {
        IDXCompiler* pDxCompiler     = nullptr;
        Uint32       VkVersion       = 0;
        bool         SupportsSpirv14 = false;
    };

    struct MtlProperties
    {
        const char* CompileOptionsMacOS = nullptr;
        const char* CompileOptionsIOS   = nullptr;
        const char* MslPreprocessorCmd  = nullptr;

        const Uint32 MaxBufferFunctionArgumets = 31;
    };

    const D3D11Properties& GetD3D11Properties() { return m_D3D11Props; }
    const D3D12Properties& GetD3D12Properties() { return m_D3D12Props; }
    const VkProperties&    GetVkProperties() { return m_VkProps; }
    const MtlProperties&   GetMtlProperties() { return m_MtlProps; }

    ARCHIVE_DEVICE_DATA_FLAGS GetValidDeviceFlags() const
    {
        return m_ValidDeviceFlags;
    }

protected:
    static PipelineResourceBinding ResDescToPipelineResBinding(const PipelineResourceDesc& ResDesc, SHADER_TYPE Stages, Uint32 Register, Uint32 Space);

private:
    static void GetPipelineResourceBindingsD3D11(const PipelineResourceBindingAttribs& Attribs,
                                                 std::vector<PipelineResourceBinding>& ResourceBindings);
    static void GetPipelineResourceBindingsD3D12(const PipelineResourceBindingAttribs& Attribs,
                                                 std::vector<PipelineResourceBinding>& ResourceBindings);
    static void GetPipelineResourceBindingsGL(const PipelineResourceBindingAttribs& Attribs,
                                              std::vector<PipelineResourceBinding>& ResourceBindings);
    static void GetPipelineResourceBindingsVk(const PipelineResourceBindingAttribs& Attribs,
                                              std::vector<PipelineResourceBinding>& ResourceBindings);
    static void GetPipelineResourceBindingsMtl(const PipelineResourceBindingAttribs& Attribs,
                                               std::vector<PipelineResourceBinding>& ResourceBindings,
                                               const Uint32                          MaxBufferArgs);

    virtual void TestTextureFormat(TEXTURE_FORMAT TexFormat) override final
    {
        UNSUPPORTED("TestTextureFormat is not supported by serialization device");
    }

    ARCHIVE_DEVICE_DATA_FLAGS m_ValidDeviceFlags = ARCHIVE_DEVICE_DATA_FLAG_NONE;

    std::unique_ptr<IDXCompiler> m_pDxCompiler;
    std::unique_ptr<IDXCompiler> m_pVkDxCompiler;

    D3D11Properties m_D3D11Props;
    D3D12Properties m_D3D12Props;
    VkProperties    m_VkProps;
    MtlProperties   m_MtlProps;

    // Metal
    String m_MtlCompileOptionsMacOS;
    String m_MtlCompileOptionsiOS;
    String m_MslPreprocessorCmd;

    std::vector<PipelineResourceBinding> m_ResourceBindings;
};

} // namespace Diligent
