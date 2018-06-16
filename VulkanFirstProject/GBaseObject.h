#pragma once
#include "IGObject.h"
#include "BaseTechnique.h"

class CGBaseObject : public IGObject
{
public:
    CGBaseObject();
    ~CGBaseObject() = default;

    // Derived from IGObject
    bool     CreateBuffers() override;

    size_t   GetVertexSize() const override;
    size_t   GetVerticesSize() const override;
    size_t   GetIndexSize() const override;
    size_t   GetIndicesSize() const override;

    uint     GetIndicesCount() const override;
    uint     GetVerticesCount() const override;
    void*    GetVerticesPtr() override;
    void*    GetIndicesPtr() override;

    static uint s_TechId; //#TECH keipskie rozwiazanie ale na razie na szybko jest

protected:
    void CreateVertexBuffer();
    void CreateIndexBuffer();

private:
    std::vector<uint16_t> m_Indices;
    std::vector<BaseVertex> m_Vertices;
};