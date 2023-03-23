#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include <iostream>


Scene::Scene() :
    active_camera_index(0),
    active_model_index(0),
    active_light_index(0),
    next_light_id(1)
{
    model_color = glm::vec3(0.0f, 0.0f, 0.0f);
    cameras.push_back(std::make_shared<Camera>());
    AddLight();

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
    mesh_models.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
    return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
    return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
    
    return *mesh_models[active_model_index];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
    cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
    return cameras.size();
}

Camera& Scene::GetCamera(int index)
{
    return *cameras[index];
}

Camera& Scene::GetActiveCamera() const
{
    return *cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(int index)
{
    active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
    return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
    active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
    return active_model_index;
}

void Scene::RemoveModel(int i){
    if (i > mesh_models.size() || i < 0){
        std::cout<<"index model is not leagel"<<std::endl;
        return;
    }
    mesh_models.erase(mesh_models.begin() + i);
    active_model_index = 0;
}

void Scene::RemoveCamera(int i){
    if (i > cameras.size() || i < 0){
        std::cout<<"index camera is not leagel"<<std::endl;
        return;
    }
    cameras.erase(cameras.begin() + i);
    active_camera_index = 0;
}

void Scene::AddLight()
{
    lights.emplace_back(std::make_shared<Light>(next_light_id++, glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Scene::AddLight(glm::vec3 position)
{
    lights.emplace_back(std::make_shared<Light>(next_light_id++, position));
}

int Scene::GetLightCount() const
{
    return lights.size();
}

Light& Scene::GetLightAtIndex(int index) const
{
    return *lights[index];
}

Light& Scene::GetActiveLight() const
{
    return *lights[active_light_index];
}

void Scene::SetActiveLightIndex(size_t index)
{
    active_light_index = index;
}

size_t Scene::GetActiveLightIndex() const
{
    return active_light_index;
}

void Scene::RemoveLight(size_t index)
{
    if (index < 0 || index > lights.size())
        return;

    lights.erase(lights.begin() + index);
    active_light_index = 0;
}

void Scene::ClearAllLights()
{
    lights.clear();
    active_light_index = 0;
    next_light_id = 1;
}

void Scene::setActiveShader(const int i)
{
    active_shader = shaders[i];
}
