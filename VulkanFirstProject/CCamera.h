#pragma once
class CCamera
{
public:
    CCamera() = default;
    ~CCamera() = default;

    // Projection methods
    void SetPerspectiveProjection(float FOV, float aspectRatio, float zNear, float zFar);

    // View / Position
    void Update();
    void MoveCamSpherical(float psi, float fi);
    void ChangeViewSphereRadius(float r);

    void AddToView(float x, float y, float z);
    void AddToEye(float x, float y, float z);


    // Setters & Getters
    void SetEye(const glm::vec3& eye) { m_Eye = eye; }
    void SetView(const glm::vec3& view) { m_View = view; }

    const glm::mat4& ViewMatrix() const { return m_ViewMtx; }
    const glm::mat4& ProjectionMatrix() const { return m_ProjectionMtx; }
    const glm::mat4& InvProjectionMatrix() const { return m_InvProjectionMtx; }
    const glm::vec3& CameraPosition() const { return m_Eye; }
    const glm::vec3& CameraView() const { return m_View; }
    const glm::vec3& CameraUp() const { return m_Up; }

private:
    // Move
    void MoveCamSpherical();

    glm::mat4 m_ViewMtx;
    glm::mat4 m_ProjectionMtx;
    glm::mat4 m_InvProjectionMtx;

    glm::vec3 m_Eye = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_View = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Cam move attributes
    float m_SphereCamPSI = -0.5f;
    float m_SphereCamFI = 0.0f;
    float m_SphereCamRadius = -10.0f;;
};