#include "Camera.h"
#include <iostream>



Camera::Camera()
{
    view_transformation = glm::mat4(1.0f);
    projection_transformation = glm::mat4(1.0f);
    inv_view = glm::mat4(1.0f);
    
    eye = glm::vec3(0.0f, 0.0f, 1000.0f);
    at = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    projection_type = 'o';
    view_volume = {-640.0f, 640.0f, -360.0f, 360.0f ,1.0f, 2000.0f};
    view_volume_p = {45.0f, 1280.0f/720.0f, 1.0f, 2000.0f};
    cameraPos = {0.0f, 0.0f, 0.0f};
    }

Camera::~Camera()
{
   
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}
const glm::mat4x4& Camera::GetInvViewTransformation() const
{
    return inv_view;
}

const glm::mat4x4 Camera::GetCameraTransformation()
{
    CreateProjection();
    LookAt(eye, at, up);
    glm::mat4 m = glm::inverse(get_mat_transformation());
    UpdatePoposition(m);
    return inv_view * m;
}


void Camera::LookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
    inv_view = glm::lookAt(eye, at, up);
    view_transformation = glm::inverse(inv_view);
}

void Camera::SetProjectionType(const char type)
{
    this->projection_type = type;
}

const char Camera::GetProjectionType()
{
    return this->projection_type;
}



void Camera::Set_view_vol(float width, float height){
    view_volume[0] = -width/2.f;
    view_volume[1] = width/2.f;
    view_volume[2] = -height/2.f;
    view_volume[3] = height/2.f;
    
    //update aspect ratio
    view_volume_p[1] = width/height;
}

void Camera::Set_view_vol(int index, float value){
    switch (index) {
        case 0:
            view_volume[0] = value;
            break;
        case 1:
            view_volume[1] = value;
            break;
        case 2:
            view_volume[2] = value;
            break;
        case 3:
            view_volume[3] = value;
            break;
        case 4:
            view_volume[4] = value;
            break;
        case 5:
            view_volume[5] = value;
            break;
    }
}


void Camera::Set_view_vol_p(int index, float value){
    switch (index) {
        case 0:
            view_volume_p[0] = value;
            break;
        case 1:
            view_volume_p[1] = value;
            break;
        case 2:
            view_volume_p[2] = value;
            break;
        case 3:
            view_volume_p[3] = value;
            break;

    }
}


void Camera::Set_eye(int index, float value){
    switch (index) {
        case 0:
            eye[0] = value;
            break;
        case 1:
            eye[1] = value;
            break;
        case 2:
            eye[2] = value;
            break;
    
    }
}

void Camera::Set_at(int index, float value){
    switch (index) {
        case 0:
            at[0] = value;
            break;
        case 1:
            at[1] = value;
            break;
        case 2:
            at[2] = value;
            break;
    }
}

void Camera::Set_up(int index, float value){
    switch (index) {
        case 0:
            up[0] = value;
            break;
        case 1:
            up[1] = value;
            break;
        case 2:
            up[2] = value;
            break;
    }
}



void Camera::CreateProjection(){
    switch (this->projection_type)
    {
    case 'o':
            projection_transformation = glm::ortho(view_volume[0], view_volume[1], view_volume[2], view_volume[3], view_volume[4], view_volume[5]);
        break;
    case 'p':
            projection_transformation = glm::perspective(glm::radians(view_volume_p[0]), view_volume_p[1], view_volume_p[2],view_volume_p[3]);
        break;
    }
}


void Camera::Reset(){
    view_transformation = glm::mat4(1.0f);
    projection_transformation = glm::mat4(1.0f);
    inv_view = glm::mat4(1.0f);
    eye = glm::vec3(0.0f, 0.0f, 1.0f);
    at = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    view_volume = {-640.0f, 640.0f, -360.0f, 360.0f ,1.0f, 2000.0f};
    view_volume_p = {45.0f, 1280.0f/720.0f, 1.0f, 2000.0f};
    dollyZoom = 0;
    MatrixTransform::Reset();
}


void Camera::SetDollyZoom(const float i)
{
    this->dollyZoom = i;
    Set_view_vol_p(0, 45.0f + (0.5*i));
    Set_eye(2, 1000 - (i*10.0f));
}

glm::vec3 Camera::GetCameraPosition()
{
    return cameraPos;
}

void Camera::UpdatePoposition(glm::mat4 m)
{
    cameraPos = m * glm::vec4(1.0f);
}



