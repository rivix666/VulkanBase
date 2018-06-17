#pragma once
#include "IGObject.h"
#include "BaseTechnique.h"

enum class EBaseObjInitType
{
    PLANE = 0,
    BOX,

    _COUNT_
};

struct SObjUniBuffer
{
    glm::mat4 obj_world;
};

class CGBaseObject : public IGObject
{
public:
    CGBaseObject() = default;
    CGBaseObject(const EBaseObjInitType& type);
    CGBaseObject(const EBaseObjInitType& type, const SObjMtxInitParams& params);
    CGBaseObject(const std::vector<uint16_t>& indices, const std::vector<BaseVertex>& vertices, const SObjMtxInitParams& params);
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
    void InitVectors(const EBaseObjInitType& type);
    void CreateVertexBuffer();
    void CreateIndexBuffer();

private:
    std::vector<uint16_t> m_Indices;
    std::vector<BaseVertex> m_Vertices;
};