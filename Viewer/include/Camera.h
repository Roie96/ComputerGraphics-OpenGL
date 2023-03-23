#pragma once
#include <glm/glm.hpp>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "MatrixTransform.h"



class Camera : public MatrixTransform
{
public:
	Camera();
	virtual ~Camera();

	void LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
    void SetProjectionType(const char type);
    void CreateProjection();

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
    const glm::mat4x4 GetCameraTransformation();
    const glm::mat4x4& GetInvViewTransformation() const;
    void Set_view_vol(float width, float height);
    void Set_view_vol(int index, float value);
    void Set_view_vol_p(int index, float value);
    const char GetProjectionType();

    std::vector<float> Get_view_volume_p(){return view_volume_p;};
    std::vector<float> Get_view_volume(){return view_volume;};
    float Get_view_volume(int index){return view_volume[index];};
    glm::vec3 Get_eye(){return eye;};
    glm::vec3 Get_at(){return at;};
    glm::vec3 Get_up(){return up;};
    glm::vec3 GetCameraPosition();
    void Set_eye(int index, float value);
    void Set_at(int index, float value);
    void Set_up(int index, float value);
    void Reset();
    void SetDollyZoom(const float i);
    const float GetDollyZoom(){return this->dollyZoom;};
    
    void UpdatePoposition(glm::mat4 m);
    
    

private:
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
    glm::mat4x4 inv_view;
    glm::vec3 eye;
    glm::vec3 at;
    glm::vec3 up;
    glm::vec3 cameraPos;
    std::vector<float> view_volume;
    std::vector<float> view_volume_p;
    int flag_move;
    float dollyZoom = 0.0f;
    float view_vol_fovy;
    float eyez;

    char projection_type;
    
};
