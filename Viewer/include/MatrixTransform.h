#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MatrixTransform
{
public:
    MatrixTransform();
    const float get_scale_factor_local();
    const glm::vec3& get_move_vector_local();
    const glm::vec3& get_rotate_factor_local();
    const glm::vec3& get_move_vector_world();
    const glm::vec3& get_rotate_factor_world();
    const float get_scale_factor_world();

    glm::mat4 create_rotate_mat(char coordinates, float degree);
    void create_trans_local();
    void create_trans_world(const char c);
    glm::mat4 get_mat_transformation();
    glm::mat4 get_local_transformation();
    glm::mat4 get_model_axes(const char c);
    const glm::mat4& get_rotate_local_mat();
    const glm::mat4& get_rotate_world_mat();


    void set_scale_factor_local(const float& scale);
    void set_scale_factor_world(const float& scale);
    void set_local_move_factor(const char c, const float d);
    void set_world_move_factor(const char c, const float d);
    void set_rotate_world_factor(const char c, const float d);
    void set_rotate_local_factor(const char c, const float d);
    virtual void Reset();

private:

    //local
    glm::mat4 local_matrix;
    glm::vec3 move_factor_local;
    glm::vec3 rotate_factor_local;
    float scale_factor_local;
    glm::mat4 rotate_mat_local;

    //world
    glm::mat4 world_matrix;
    glm::mat4 model_axes_matrix;
    glm::vec3 rotate_factor_world;
    glm::vec3 move_factor_world;
    float scale_factor_world;
    glm::mat4 rotate_mat_world;


};

