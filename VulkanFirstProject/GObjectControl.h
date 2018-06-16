#pragma once

class IGObject;
class ITechnique;

#define REGISTER_OBJ(obj) { g_Engine->GetObjectControl()->RegisterObject(obj); }
#define REGISTER_OBJ(tech, obj) { g_Engine->GetObjectControl()->RegisterObject(tech, obj); }
/*#define REGISTER_OBJ_LAY(#v_layout, obj) {  }*/ // #TECH tutaj chcialem zrobic rejestracje obiektu rpzez jego vertex layout ale z jevnego vertex layoutu moze korzystac kilka technik...
#define UNREGISTER_OBJ(tech, obj) { g_Engine->GetObjectControl()->UnregisterObject(tech, obj); }

class CGObjectControl
{
public:
    CGObjectControl(VkDevice device);
    ~CGObjectControl();

    typedef unsigned int uint; // #TYPEDEF_UINT czemu nei bierze z stdafx?? cos z dodaniuem includow do headera engine?

    void Shutdown();

    void RegisterObject(IGObject* obj);
    void RegisterObject(const uint& tech, IGObject* obj);
    void UnregisterObject(const uint& tech, IGObject* obj);

    void RecordCommandBuffer(VkCommandBuffer& cmd_buff);

protected:
    void CleanupBuffers(const uint& tech);
    void RecreateBuffers(const uint& tech);
    bool CreateVertexBuffer(const uint& tech);

private:
    void EnsureTechIdWillFit(const uint& tech_id);

    // Typedefs
    typedef std::vector<size_t> TCacheVec;
    typedef std::vector<IGObject*> TObjectsVec;
    typedef std::vector<TObjectsVec> TTech2ObjVec;

    // Device
    VkDevice m_Device = nullptr;

    // Buffers
    std::vector<VkBuffer> m_VertexBuffers;
    std::vector<VkBuffer> m_IndexBuffers;
    std::vector<VkDeviceMemory> m_VertexBuffersMemory;
    std::vector<VkDeviceMemory> m_IndexBuffersMemory;

    // Objects
    TCacheVec m_SizeCacheVec;
    TTech2ObjVec m_TechToObjVec;
};
