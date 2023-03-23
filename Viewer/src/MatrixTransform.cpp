#include "MatrixTransform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"


MatrixTransform::MatrixTransform(){
    //all local matrix fields
    this->local_matrix = glm::mat4(1.0f);
    this->rotate_factor_local = glm::vec3(0.f, 0.f, 0.f);
    this->scale_factor_local = 1.f;
    this->move_factor_local = glm::vec3(0.f, 0.f, 0.f);
    this->model_axes_matrix = glm::mat4(1.0f);
    this->rotate_mat_local = glm::mat4(1.0f);

    //all world matrix fields
    this->world_matrix = glm::mat4(1.0f);
    this->rotate_factor_world = glm::vec3(0.f, 0.f, 0.f);
    this->move_factor_world = glm::vec3(0.f, 0.f, 0.f);
    this->scale_factor_world = 1.0f;
    this->rotate_mat_world = glm::mat4(1.0f);

}

const float MatrixTransform::get_scale_factor_local()
{
    return scale_factor_local;
}

const float MatrixTransform::get_scale_factor_world()
{
    return scale_factor_world;
}

void MatrixTransform::set_scale_factor_local(const float& scale){
    scale_factor_local = scale;
}
void MatrixTransform::set_scale_factor_world(const float& scale){
    this->scale_factor_world = scale;
}

const glm::vec3& MatrixTransform::get_move_vector_local()
{
    return move_factor_local;
}




const glm::vec3& MatrixTransform::get_move_vector_world()
{
    return move_factor_world;
}


const glm::vec3& MatrixTransform::get_rotate_factor_local()
{
    return rotate_factor_local;
}

const glm::vec3& MatrixTransform::get_rotate_factor_world()
{
    return rotate_factor_world;
}





void MatrixTransform::set_local_move_factor(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->move_factor_local[0] = d;
        break;
    case 'y':
            this->move_factor_local[1] = d;
        break;
    case 'z':
            this->move_factor_local[2] = d;
        break;
    }
}

void MatrixTransform::set_world_move_factor(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->move_factor_world[0] = d;
        break;
    case 'y':
            this->move_factor_world[1] = d;
        break;
    case 'z':
            this->move_factor_world[2] = d;
        break;
    }
}

void MatrixTransform::set_rotate_local_factor(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->rotate_factor_local[0] = d;
        break;
    case 'y':
            this->rotate_factor_local[1] = d;
        break;
    case 'z':
            this->rotate_factor_local[2] = d;
        break;
    }
}

void MatrixTransform::set_rotate_world_factor(const char c, const float d)
{
    switch (c) {
    case 'x':
            this->rotate_factor_world[0] = d;
        break;
    case 'y':
            this->rotate_factor_world[1] = d;
        break;
    case 'z':
            this->rotate_factor_world[2] = d;
        break;
    }
}



glm::mat4 MatrixTransform::create_rotate_mat(char c,float d)
{
    switch (c) {
    case 'x':
            return glm::rotate(glm::mat4(1.0f),glm::radians(d),glm::vec3(1.0f, 0.0f, 0.0f));
        break;
    case 'y':
            return glm::rotate(glm::mat4(1.0f),glm::radians(d),glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case 'z':
            return glm::rotate(glm::mat4(1.0f),glm::radians(d),glm::vec3(0.0f, 0.0f, 1.0f));
        break;
    
    }
}


void MatrixTransform::create_trans_local(){
    //scale
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor_local, scale_factor_local, scale_factor_local));
    
    //translate
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), get_move_vector_local());
    
    //rotateion
    glm::mat4 rotation_x = create_rotate_mat('x', rotate_factor_local[0]);
    glm::mat4 rotation_y = create_rotate_mat('y', rotate_factor_local[1]);
    glm::mat4 rotation_z = create_rotate_mat('z', rotate_factor_local[2]);
    rotate_mat_local = rotation_z * rotation_y * rotation_x;
    
    //update local matrix
    local_matrix = transMat * scaleMat * rotate_mat_local;
}


void MatrixTransform::create_trans_world(const char c){
    //scale
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor_world, scale_factor_world, scale_factor_world));
    
    //translate
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), get_move_vector_world());
    
    //rotateion
    glm::mat4 rotation_x = create_rotate_mat('x', rotate_factor_world[0]);
    glm::mat4 rotation_y = create_rotate_mat('y', rotate_factor_world[1]);
    glm::mat4 rotation_z = create_rotate_mat('z', rotate_factor_world[2]);
    rotate_mat_world = rotation_z * rotation_y * rotation_x;
    switch (c) {
    case 'a':
            //create transormed matrix
            world_matrix = transMat * scaleMat * rotate_mat_world;
    case 'x':
            //rotate y and z axes
            model_axes_matrix = transMat * rotate_mat_world/rotation_x;
        break;
    case 'y':
            //rotate x and z axis
            model_axes_matrix = transMat *  rotate_mat_world/rotation_y;
        break;
    case 'z':
            //rotate x and y axis
            model_axes_matrix = transMat *  rotate_mat_world/rotation_z;
        break;
    case 'o':
            //move 0.0
            model_axes_matrix = transMat ;
        break;
    }
}

glm::mat4 MatrixTransform::get_mat_transformation(){
    create_trans_world('a');
    create_trans_local();
    return world_matrix * local_matrix;
}

glm::mat4 MatrixTransform::get_local_transformation() {
    create_trans_local();
    return local_matrix;
}

glm::mat4 MatrixTransform::get_model_axes(const char c) 
{
    create_trans_world(c);
    return model_axes_matrix;
}

void MatrixTransform::Reset(){
    //all local matrix fields
    this->local_matrix = glm::mat4(1.0f);
    this->rotate_factor_local = glm::vec3(0.f, 0.f, 0.f);
    this->scale_factor_local = 1.f;
    this->move_factor_local = glm::vec3(0.f, 0.f, 0.f);


    //all world matrix fields
    this->world_matrix = glm::mat4(1.0f);
    this->rotate_factor_world = glm::vec3(0.f, 0.f, 0.f);
    this->move_factor_world = glm::vec3(0.f, 0.f, 0.f);
    this->scale_factor_world = 1.0f;
    this->rotate_mat_local = glm::mat4(1.0f);
    this->rotate_mat_world = glm::mat4(1.0f);

}



const glm::mat4& MatrixTransform::get_rotate_local_mat(){
    return this->rotate_mat_local;
}
const glm::mat4& MatrixTransform::get_rotate_world_mat(){
    
    return this->rotate_mat_world;
}
