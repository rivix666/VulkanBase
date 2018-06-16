#pragma once
#include "IGObject.h"
#include "BaseTechnique.h"

class CGBaseObject : public IGObject
{
public:
    CGBaseObject();
    ~CGBaseObject() = default;

    // Derived from IGObject
    size_t   GetVertexSize() const override;
    size_t   GetVerticesSize() const override;
    uint     GetVerticesCount() const override;
    void*    GetVerticesPtr() override;

    static uint s_TechId; //#TECH keipskie rozwiazanie ale na razie na szybko jest

private:
    std::vector<BaseVertex> m_Vertices;
};