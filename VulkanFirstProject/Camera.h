#pragma once

enum class ECamMoveDir
{
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT,
    UP,
    DOWN,

    _COUNT_
};

class CCamera
{
public:
    CCamera();
    ~CCamera() = default;

    typedef unsigned int uint; // #TYPEDEF_UINT czemu nei bierze z stdafx??

    // Projection methods
    void SetPerspectiveProjection(float FOV, float aspectRatio, float zNear, float zFar);

    // View / Position
    void Update();

    // Setters & Getters
    void SetEye(const glm::vec3& eye) { m_Eye = eye; }
    void SetView(const glm::vec3& view) { m_View = view; }

    bool UseFreeCam() const { return m_UseFreeCam; }
    float MoveSpeed() const { return m_MoveSpeed; }
    void SetUseFreeCam(bool use = true);
    void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }

    // Matrices
    const glm::mat4& ViewMatrix() const { return m_ViewMtx; }
    const glm::mat4& ProjectionMatrix() const { return m_ProjectionMtx; }
    const glm::mat4& InvProjectionMatrix() const { return m_InvProjectionMtx; }

    // Vectors
    const glm::vec3& CameraPosition() const { return m_Eye; }
    const glm::vec3& CameraRight() const { return m_Right; }
    const glm::vec3& CameraView() const { return m_View; }
    const glm::vec3& CameraUp() const { return m_Up; }

    // Input
    void ProcessMouseMoveInput(float xoffset, float yoffset);

    // Free camera move
    void MoveFreeCam(ECamMoveDir dir, bool state);
    void MoveYawPitchFreeCam(float xoffset, float yoffset);

    // Sphere camera move
    void MoveCamSpherical(float psi, float fi);
    void ChangeViewSphereRadius(float r);

private:
    // Move
    void UpdateCameraVectors();
    void UpdateCamSphericalVectors();
    void UpdateFreeCamVectors();
    void UpdateFreeCamPos();
    void UpdateFreeCamView();

    // Matrices
    glm::mat4 m_ViewMtx = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMtx = glm::mat4(1.0f);
    glm::mat4 m_InvProjectionMtx = glm::mat4(1.0f);

    // Camera Attributes
    glm::vec3 m_Eye = glm::vec3(0.0f, 5.0f, 15.0f);
    glm::vec3 m_View = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_Right;

    // Move
    float m_MoveSpeed = 0.1f;

    // Angles
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;;

    // Cam sphere move attributes
    float m_SphereCamPSI = -0.5f;
    float m_SphereCamFI = 0.0f;
    float m_SphereCamRadius = -10.0f;;

    // Misc
    bool m_UseFreeCam = true;
    bool m_MoveDirKeyState[(uint)ECamMoveDir::_COUNT_];
};