#pragma once
#include "ITechnique.h"

// struct IVertex
// {
// };

class IGObject
{
public:
    IGObject();
    ~IGObject();

    typedef unsigned int uint; // #TYPEDEF_UINT czemu nei bierze z stdafx??

    virtual size_t   GetVertexSize() const = 0;
    virtual size_t   GetVerticesSize() const = 0;
    virtual uint     GetVerticesCount() const = 0;

    virtual void* GetVerticesPtr() = 0;

    // #TECH chyba chodzilo o pointer m_Tech ale one moga zniknac, lepiej trzymac Id
    //virtual void ResolvePointers() const = 0;

    uint GetTechniqueId() const { return m_TechId; }
    //ITechnique* GetTechnique() const { return m_Tech; }

protected:
    uint m_TechId = UINT_MAX; //#TECH zobaczymy czy w ogole potrzebne
    //ITechnique* m_Tech = nullptr;
};

