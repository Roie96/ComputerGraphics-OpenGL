#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */


double zoomFactor = 1;
bool zoomChanged = false;


bool show_demo_window = false;
bool show_another_window = false;

bool ortho_flag = true;
bool persp_flag = false;

bool flat_flag = false;
bool phong_flag = true;
bool gouraud_flag = false;

bool cylindrical_flag = false;
bool planar_flag = true;
bool spherical_flag = false;

bool none_flag = true;
bool toon_flag = false;
bool normal_flag = false;
bool environment_flag = false;


glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
static float windowWidth = 1280, windowHeight = 720;


float Aspect;

bool focus;
float localScale, worldScale, localScaleCamera, worldScaleCamera, dolly_zoom;
glm::vec3 local_translate, world_translate, local_rotate, world_rotate, local_translateCamera, world_translateCamera, local_rotateCamera, world_rotateCamera, eye, at, up;
glm::vec4 model_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
std::vector<float> camera_view_vol, camera_view_vol_p;



/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void initCamera(Scene& scene);
void initModel(MeshModel& model);
//void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);


/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    // TODO: Handle mouse scroll here
}

void initCamera(Scene& scene){
    // initialization all loacl parameters for camera
    localScaleCamera = scene.GetActiveCamera().get_scale_factor_local();
    worldScaleCamera = scene.GetActiveCamera().get_scale_factor_world();
    local_translateCamera = scene.GetActiveCamera().get_move_vector_local();
    world_translateCamera = scene.GetActiveCamera().get_move_vector_world();
    local_rotateCamera = scene.GetActiveCamera().get_rotate_factor_local();
    world_rotateCamera = scene.GetActiveCamera().get_rotate_factor_world();
    camera_view_vol = scene.GetActiveCamera().Get_view_volume();
    camera_view_vol_p = scene.GetActiveCamera().Get_view_volume_p();
    eye = scene.GetActiveCamera().Get_eye();
    at = scene.GetActiveCamera().Get_at();
    up = scene.GetActiveCamera().Get_up();
    dolly_zoom = scene.GetActiveCamera().GetDollyZoom();
    
}

void initModel(MeshModel& model){
    // initialization all loacl parameters
    localScale = model.get_scale_factor_local();
    worldScale = model.get_scale_factor_world();
    local_translate = model.get_move_vector_local();
    world_translate = model.get_move_vector_world();
    local_rotate = model.get_rotate_factor_local();
    world_rotate = model.get_rotate_factor_world();
    model_color = glm::vec4 (model.get_model_color(), 1.0f);
}

int main(int argc, char **argv)
{
    GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
    if (!window)return 1;
    
    ImGuiIO& io = SetupDearImgui(window);
//
//    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
//    glEnable(GL_DEPTH_TEST);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    int frameBufferWidth, frameBufferHeight;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);


    Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
    Scene scene = Scene();
    scene.GetActiveCamera().Set_view_vol((float)frameBufferWidth, (float)frameBufferHeight);
    windowWidth = frameBufferWidth;
    windowHeight = frameBufferHeight;
        
    glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        // Poll and process events
        glfwPollEvents();
        
        // Imgui stuff
        StartFrame();
        DrawImguiMenus(io, scene);
        
        // Render scene
        RenderFrame(window, scene, renderer, io);
    }
        Cleanup(window);
        return 0;
    }

    static void GlfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }


GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
  {
      glfwSetErrorCallback(GlfwErrorCallback);
      if (!glfwInit())
          return NULL;
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      
      #if __APPLE__
          glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      #endif
      
      GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
      glfwMakeContextCurrent(window);
      glfwSwapInterval(1); // Enable vsync
                           // very importent!! initialization of glad
                           // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

      gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
      return window;
  }

    ImGuiIO& SetupDearImgui(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        // Setup style
        ImGui::StyleColorsDark();
//        glfwSetScrollCallback(window, glfw_OnMouseScroll);

        return io;
    }
    void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
    {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    zoomFactor = glm::pow(1.1, -yoffset);
    zoomChanged = true;
    }

    void StartFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
    {
        ImGui::Render();
        int frameBufferWidth, frameBufferHeight;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
        if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
               {
                   // TODO: Set new aspect ratio
                   windowWidth = frameBufferWidth;
                   windowHeight = frameBufferHeight;
               
                   //update window size in render
                   renderer.SetViewportWidth(windowWidth);
                   renderer.SetViewportHeight(windowHeight);
//                   renderer.ColorBufferUpdate();
                               
                   //update window size in all cameras
                   for (int i = 0; i < scene.GetCameraCount(); i++){
                       scene.GetCamera(i).Set_view_vol(windowWidth, windowHeight);
                   }

               }
        
        if (!io.WantCaptureKeyboard) {
            // TODO: Handle keyboard events here
            if(scene.GetModelCount()>0){
                //translate
                // using arrows
                if (io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]]){      // Up
                    scene.GetActiveModel().set_local_move_factor('y', scene.GetActiveModel().get_move_vector_local()[1]+0.01);
                }
                if (io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]]){    // Down
                    scene.GetActiveModel().set_local_move_factor('y', scene.GetActiveModel().get_move_vector_local()[1]-0.01);
                }
                if (io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]]){   // Right
                    scene.GetActiveModel().set_local_move_factor('x', scene.GetActiveModel().get_move_vector_local()[0]+0.01);
                }
                if (io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]]){    // Left
                    scene.GetActiveModel().set_local_move_factor('x', scene.GetActiveModel().get_move_vector_local()[0]-0.01);
                }
                
                //rotaion
                // using 'WSDA'
                if (io.KeysDown[87] || io.KeysDown[119]){       // Up
                    scene.GetActiveModel().set_rotate_local_factor('y', scene.GetActiveModel().get_rotate_factor_local()[1]+1);
                }
                if (io.KeysDown[83] || io.KeysDown[115]){       // Down
                    scene.GetActiveModel().set_rotate_local_factor('y', scene.GetActiveModel().get_rotate_factor_local()[1]-1);
                }
                if (io.KeysDown[68] || io.KeysDown[100]){       // Right
                    scene.GetActiveModel().set_rotate_local_factor('x', scene.GetActiveModel().get_rotate_factor_local()[0]+1);
                }
                if (io.KeysDown[65] || io.KeysDown[97]){        // Left
                    scene.GetActiveModel().set_rotate_local_factor('x', scene.GetActiveModel().get_rotate_factor_local()[0]-1);
                }
            }
        }

        if (!io.WantCaptureMouse)
        // TODO: Handle mouse events here
        {
            if(scene.GetModelCount()>0){
                // Left mouse button is down
                // make model bigger
                if (io.MouseDown[0])
                {
                    scene.GetActiveModel().set_scale_factor_local(scene.GetActiveModel().get_scale_factor_local()+0.01);
                }
                // Right mouse button is down
                // make model smaller
                if (io.MouseDown[1])
                {
                    if(scene.GetActiveModel().get_scale_factor_local() != 0)
                        scene.GetActiveModel().set_scale_factor_local(scene.GetActiveModel().get_scale_factor_local()-0.01);
                }
            }
        }
        
        // Clear the screen and depth buffer
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.Render(scene);
        
        // Imgui stuff
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Swap front and back buffers
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    void Cleanup(GLFWwindow* window)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DrawImguiMenus(ImGuiIO& io, Scene& scene)
    {
        /**
         * MeshViewer menu
         */
//        ImGui::Begin("MeshViewer Menu");
        
        // Menu Bar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "CTRL+O"))
                {
                    nfdchar_t* outPath = NULL;
                    nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
                    if (result == NFD_OKAY)
                    {
                        scene.AddModel(Utils::LoadMeshModel(outPath));
                        free(outPath);
                    }
                    else if (result == NFD_CANCEL)
                    {
                    }
                    else
                    {
                    }

                }
         
                //added quick access to objects
                if (ImGui::MenuItem("Banana"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/banana.obj"));
                if (ImGui::MenuItem("Beethoven"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/beethoven.obj"));
                if (ImGui::MenuItem("Bishop"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/bishop.obj"));
                if (ImGui::MenuItem("Blob"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/blob.obj"));
                if (ImGui::MenuItem("Bunny"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/bunny.obj"));
                if (ImGui::MenuItem("Camera"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/camera.obj"));
                if (ImGui::MenuItem("Chain"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/chain.obj"));
                if (ImGui::MenuItem("Cow"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/cow.obj"));
                if (ImGui::MenuItem("Demo"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/demo.obj"));
                if (ImGui::MenuItem("Dolphin"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/dolphin.obj"));
                if (ImGui::MenuItem("Feline"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/feline.obj"));
                if (ImGui::MenuItem("Pawn"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/pawn.obj"));
                if (ImGui::MenuItem("Teapot"))
                        scene.AddModel(Utils::LoadMeshModel("/Users/roieshahar/Desktop/graphics/computer-graphics-2023-roie-yuval/Data/teapot.obj"));

                ImGui::EndMenu();
            }
            
            
            for (int i = 0; i < scene.GetModelCount(); i++)
            {
                MeshModel& model = scene.GetModel(i);
                initModel(model);

                ImGui::Begin((model.GetModelName()).c_str());
                
            
                // local controller
                ImGui::Text("Local Transformations");
                ImGui::SliderFloat("Scale local",&localScale, 1.0f, 10.0f);
                model.set_scale_factor_local(localScale);
                focus = focus || ImGui::IsItemActivated();


                ImGui::Text("Translate");
                ImGui::SliderFloat("X: ", &local_translate[0], -windowWidth/2.0f, windowWidth/2.0f);
                model.set_local_move_factor('x', local_translate[0]);
                focus = focus || ImGui::IsItemActivated();


                ImGui::SliderFloat("Y", &local_translate[1], -windowHeight/2.0f, windowHeight/2.0f);
                model.set_local_move_factor('y', local_translate[1]);
                focus = focus || ImGui::IsItemActivated();


                ImGui::SliderFloat("Z", &local_translate[2], -1000.0f, 1000.0f);
                model.set_local_move_factor('z', local_translate[2]);
                focus = focus || ImGui::IsItemActivated();



                ImGui::Text("Rotate");
                ImGui::SliderFloat("x", &local_rotate[0], 0, 360);
                model.set_rotate_local_factor('x', local_rotate[0]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::SliderFloat("y", &local_rotate[1], 0.0f, 360.0f);
                model.set_rotate_local_factor('y', local_rotate[1]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::SliderFloat("z", &local_rotate[2], 0.0f, 360.0f);
                model.set_rotate_local_factor('z', local_rotate[2]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::Separator();
                //world controller
                ImGui::Text("World Transformations");
                ImGui::SliderFloat("Scale world",&worldScale, 1.0f, 10.0f);
                model.set_scale_factor_world(worldScale);
                focus = focus || ImGui::IsItemActivated();

                ImGui::Text("Translate");
                ImGui::SliderFloat("World X", &world_translate[0], -windowWidth/2.0f, windowWidth/2.0f);

                model.set_world_move_factor('x', world_translate[0]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::SliderFloat("World Y", &world_translate[1], -windowHeight/2.0f, windowHeight/2.0f);
                model.set_world_move_factor('y', world_translate[1]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::SliderFloat("World Z", &world_translate[2], -1000.0f, 1000.0f);
                model.set_world_move_factor('z', world_translate[2]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::Text("Rotate");
                ImGui::SliderFloat("world x", &world_rotate[0], 0, 360);
                model.set_rotate_world_factor('x', world_rotate[0]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::SliderFloat("world y", &world_rotate[1], 0.0f, 360.0f);
                model.set_rotate_world_factor('y', world_rotate[1]);
                focus = focus || ImGui::IsItemActivated();

                ImGui::SliderFloat("world z", &world_rotate[2], 0.0f, 360.0f);
                model.set_rotate_world_factor('z', world_rotate[2]);
                focus = focus || ImGui::IsItemActivated();

                if (ImGui::CollapsingHeader("Materials"))
                {
                    Material& current_material = model.material;
                    ImGui::ColorEdit3("Ambient", (float*)&current_material.ambient);
                    ImGui::ColorEdit3("Diffuse", (float*)&current_material.diffuse);
                    ImGui::ColorEdit3("Specular", (float*)&current_material.specular);
                    ImGui::SliderFloat("Shininess", (float*)&current_material.shininess, 0, 5);
                }

//                ImGui::Separator();
//                ImGui::Text("Shader");
//                ImGui::Checkbox("FLAT", &flat_flag); // Buttons return true when clicked (most widgets return
//                ImGui::SameLine();
//                if (flat_flag) {
//                    gouraud_flag = false;
//                    phong_flag = false;
//                    scene.setActiveShader(0);
//                }
//                ImGui::Checkbox("GOURAUD", &gouraud_flag); // Buttons return true when clicked (most widgets return
//                if (gouraud_flag) {
//                    flat_flag = false;
//                    phong_flag = false;
//                    scene.setActiveShader(1);
//                }
                
                ImGui::Checkbox("PHONG", &phong_flag); // Buttons return true when clicked (most widgets return
                if (phong_flag) {
                    flat_flag = false;
                    gouraud_flag = false;
                    scene.setActiveShader(2);
                }
                
                ImGui::Separator();
                ImGui::Checkbox("Planar", &planar_flag); // Buttons return true when clicked (most widgets return
                if (planar_flag) {
                    cylindrical_flag = false;
                    spherical_flag = false;
                    scene.GetActiveModel().canonicalProjection = 0;
                }
                ImGui::Checkbox("Cylindrical", &cylindrical_flag);
                if (cylindrical_flag) {
                    planar_flag = false;
                    spherical_flag = false;
                    scene.GetActiveModel().canonicalProjection = 1;

                }
                ImGui::Checkbox("Spherical", &spherical_flag); 
                if (spherical_flag) {
                    cylindrical_flag = false;
                    planar_flag = false;
                    scene.GetActiveModel().canonicalProjection = 2;
                }
                
                ImGui::Separator();
                ImGui::Checkbox("None", &none_flag); // Buttons return true when clicked (most widgets return
                if (none_flag) {
                    toon_flag = false;
                    normal_flag = false;
                    environment_flag = false;
                    scene.GetActiveModel().mapTex = 0;
                }

                ImGui::Checkbox("Toon", &toon_flag); // Buttons return true when clicked (most widgets return
                if (toon_flag) {
                    none_flag = false;
                    normal_flag = false;
                    environment_flag = false;
                    scene.GetActiveModel().mapTex = 1;
                }

                ImGui::Checkbox("Normal", &normal_flag); // Buttons return true when clicked (most widgets return
                if (normal_flag) {
                    none_flag = false;
                    toon_flag = false;
                    environment_flag = false;
                    scene.GetActiveModel().mapTex = 2;
                }

                ImGui::Checkbox("Environment", &environment_flag); // Buttons return true when clicked (most widgets return
                if (environment_flag) {
                    none_flag = false;
                    normal_flag = false;
                    toon_flag = false;
                    scene.GetActiveModel().mapTex = 3;
                }


                ImGui::Separator();
                if (ImGui::Button("Reset")){
                    gouraud_flag = false;
                    flat_flag = true;
                    phong_flag = false;
                    model.Reset();
                    model.ResetAxes();
                    initModel(model);
                }
                
                ImGui::Separator();
                if (ImGui::Button("Remove"))
                {
                    gouraud_flag = false;
                    flat_flag = true;
                    model.ResetAxes();
                    scene.RemoveModel(i);
                }
               

                if(focus){
//                    std::cout<<model.GetModelName()<<std::endl;
                    scene.SetActiveModelIndex(i);
                }
                
                ImGui::End();
            
            }

            // TODO: Add more menubar items (if you want to)
            ImGui::EndMainMenuBar();
        }

//        // Controls
//        ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
//        // TODO: Add more controls as needed
//
//        ImGui::End();

        /**
         * Imgui demo - you can remove it once you are familiar with imgui
         */
        
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
         
            static float f = 0.0f;
            static int counter = 0;
            static int indexCamera = scene.GetActiveCameraIndex();
            
            
            initCamera(scene);
            ImGui::Begin("Camera Controller");
            // update render's view port according to screen size
            if (ImGui::Button("Add Camera")){
                scene.AddCamera(std::make_shared<Camera>());
                int i = scene.GetCameraCount()-1;
                scene.GetCamera(i).Set_view_vol(windowWidth, windowHeight);
            }
            
            ImGui::Text("Active Camera Number: %d ", indexCamera);
            ImGui::SameLine();
            if ((ImGui::Button("<-")))
            {
                if (indexCamera >= 1){
                    scene.SetActiveCameraIndex(--indexCamera);
                    initCamera(scene);
                    if(scene.GetActiveCamera().GetProjectionType() == 'o'){
                        ortho_flag = true;
                        persp_flag = false;
                    }
                    else{
                        ortho_flag = false;
                        persp_flag = true;
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("->"))
            {
                if (indexCamera < scene.GetCameraCount()-1){
                    scene.SetActiveCameraIndex(++indexCamera);
                    initCamera(scene);
                    if(scene.GetActiveCamera().GetProjectionType() == 'o'){
                        ortho_flag = true;
                        persp_flag = false;
                        }
                else{
                    ortho_flag = false;
                    persp_flag = true;
                    }
                }
            }
            ImGui::Separator();
            ImGui::Checkbox("Orthographic", &ortho_flag); // Buttons return true when clicked (most widgets return
            ImGui::SameLine();
            if (ortho_flag){
                persp_flag = false;
                scene.GetActiveCamera().SetProjectionType('o');
            }
            else persp_flag = true;
            ImGui::Checkbox("Perspective", &persp_flag); // Buttons return true when clicked (most widgets return
            if (persp_flag){
                ortho_flag = false;
                scene.GetActiveCamera().SetProjectionType('p');
            }
            else ortho_flag = true;
            if (ImGui::CollapsingHeader("Projections")){
                if (!ortho_flag) {
                    float currdolly =dolly_zoom;
                    ImGui::InputFloat("fovy", &camera_view_vol_p[0],1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol_p(0, camera_view_vol_p[0]);
                    ImGui::InputFloat("aspect", &camera_view_vol_p[1],0.01f, 0.01f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol_p(1, camera_view_vol_p[1]);
                    ImGui::InputFloat("near", &camera_view_vol_p[2], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol_p(2, camera_view_vol_p[2]);
                    ImGui::InputFloat("far", &camera_view_vol_p[3],1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol_p(3, camera_view_vol_p[3]);
                    ImGui::InputFloat("Dolly Zoom", &dolly_zoom, 1.0f, 1.0f, "%.2f");
                    if(currdolly != dolly_zoom)
                        scene.GetActiveCamera().SetDollyZoom(dolly_zoom);
                }
                else {
                   
                    ImGui::InputFloat("left", &camera_view_vol[0], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol(0, camera_view_vol[0]);
                    ImGui::InputFloat("right", &camera_view_vol[1], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol(1, camera_view_vol[1]);
                    ImGui::InputFloat("bottom", &camera_view_vol[2], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol(2, camera_view_vol[2]);
                    ImGui::InputFloat("top", &camera_view_vol[3], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol(3, camera_view_vol[3]);
                    ImGui::InputFloat("near", &camera_view_vol[4], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol(4, camera_view_vol[4]);
                    ImGui::InputFloat("far", &camera_view_vol[5], 1.0f, 1.0f, "%.2f");
                    scene.GetActiveCamera().Set_view_vol(5, camera_view_vol[5]);
                
                }
             
            }

            if (ImGui::CollapsingHeader("Look AT")){
                if (ImGui::CollapsingHeader("Eye")){
                    float curreyez = eye[2];
                    ImGui::InputFloat("eye x", &eye[0], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_eye(0, eye[0]);
                    ImGui::InputFloat("eye y", &eye[1], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_eye(1, eye[1]);
                    ImGui::InputFloat("eye z", &eye[2], 10.0f, 10.0f, "%.2f");
                    if(curreyez != eye[2])
                        scene.GetActiveCamera().Set_eye(2, eye[2]);
                }
                if (ImGui::CollapsingHeader("At")){
                    ImGui::InputFloat("at x", &at[0], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_at(0, at[0]);
                    ImGui::InputFloat("at y", &at[1], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_at(1, at[1]);
                    ImGui::InputFloat("at z", &at[2], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_at(2, at[2]);
                }
                if (ImGui::CollapsingHeader("up")){
                    ImGui::InputFloat("up x", &up[0], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_up(0, up[0]);
                    ImGui::InputFloat("up y", &up[1], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_up(1, up[1]);
                    ImGui::InputFloat("up z", &up[2], 10.0f, 10.0f, "%.2f");
                    scene.GetActiveCamera().Set_up(2, up[2]);
                }
            }
               
            if (ImGui::CollapsingHeader("Camera Transformations")){
                if (ImGui::CollapsingHeader("Camera frame")){
                    
                    ImGui::SliderFloat("Scale local",& localScaleCamera, 1.0f, 100.0f);
                    scene.GetActiveCamera().set_scale_factor_local(localScaleCamera);
                    
                    ImGui::Text("Translate");
                    ImGui::SliderFloat("frame X", &local_translateCamera[0], -windowWidth/2.0f, windowWidth/2.0f);
                    scene.GetActiveCamera().set_local_move_factor('x', local_translateCamera[0]);
                    
                    ImGui::SliderFloat("frame Y", &local_translateCamera[1], -windowHeight/2.0f, windowHeight/2.0f);
                    scene.GetActiveCamera().set_local_move_factor('y', local_translateCamera[1]);

                    ImGui::SliderFloat("frame Z", &local_translateCamera[2], -3000.0f, 3000.0f);
                    scene.GetActiveCamera().set_local_move_factor('z', local_translateCamera[2]);

                    ImGui::Text("Rotate");
                    ImGui::SliderFloat("frame x", &local_rotateCamera[0], 0, 360);
                    scene.GetActiveCamera().set_rotate_local_factor('x', local_rotateCamera[0]);

                    ImGui::SliderFloat("frame y", &local_rotateCamera[1], 0.0f, 360.0f);
                    scene.GetActiveCamera().set_rotate_local_factor('y', local_rotateCamera[1]);

                    ImGui::SliderFloat("frame z", &local_rotateCamera[2], 0.0f, 360.0f);
                    scene.GetActiveCamera().set_rotate_local_factor('z', local_rotateCamera[2]);
                }
                
                if (ImGui::CollapsingHeader("World frame")){
             
                    ImGui::SliderFloat("Scale world",&worldScaleCamera, 1.0f, 100.0f);
                    scene.GetActiveCamera().set_scale_factor_world(worldScaleCamera);
                   

                    ImGui::Text("Translate");
                    ImGui::SliderFloat("World X", &world_translateCamera[0], -windowWidth/2.0f, windowWidth/2.0f);
                    scene.GetActiveCamera().set_world_move_factor('x', world_translateCamera[0]);

                    ImGui::SliderFloat("World Y", &world_translateCamera[1], -windowHeight/2.0f, windowHeight/2.0f);
                    scene.GetActiveCamera().set_world_move_factor('y', world_translateCamera[1]);

                    ImGui::SliderFloat("World Z", &world_translateCamera[2], -3000.0f, 3000.0f);
                    scene.GetActiveCamera().set_world_move_factor('z', world_translateCamera[2]);

                    ImGui::Text("Rotate");
                    ImGui::SliderFloat("world x", &world_rotateCamera[0], 0, 360);
                    scene.GetActiveCamera().set_rotate_world_factor('x', world_rotateCamera[0]);

                    ImGui::SliderFloat("world y", &world_rotateCamera[1], 0.0f, 360.0f);
                    scene.GetActiveCamera().set_rotate_world_factor('y', world_rotateCamera[1]);

                    ImGui::SliderFloat("world z", &world_rotateCamera[2], 0.0f, 360.0f);
                    scene.GetActiveCamera().set_rotate_world_factor('z', world_rotateCamera[2]);
                }
                
               
            }
       
                       
            ImGui::Separator();
            if (ImGui::Button("Reset")){
                scene.GetActiveCamera().Reset();
                initCamera(scene);
            }
            
            ImGui::Separator();
            if (ImGui::Button("Remove")){
                if(scene.GetCameraCount() > 1)
                    scene.RemoveCamera(scene.GetActiveCameraIndex());
            }
            
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        for (int i = 0; i < scene.GetModelCount(); i++){
              MeshModel& model = scene.GetModel(i);
              string current_model = "Renderer Features: " + model.GetModelName();
              ImGui::Begin(current_model.c_str());

              ImGui::Separator();
              //model features
              ImGui::Text("Model Features");
              if (ImGui::Button("Draw faces")){
                  model.SetAxes(8);
              }
              if (ImGui::Button("Fill faces with random color")){
                  model.SetAxes(7);
              }
              if (ImGui::Button("Grey scale")){
                  model.SetAxes(9);
              }
              if (ImGui::Button("Display Model Axis")){
                  model.SetAxes(0);
              }
              if (ImGui::Button("Display Model bounding box")){
                  model.SetAxes(2);
              }
              if (ImGui::Button("Display faces bounding")){
                  model.SetAxes(6);
              }
              ImGui::Separator();
              //normal features
              ImGui::Text("Normal Features");
              if (ImGui::Button("Display face normals ")){
                  model.SetAxes(4);
              }
              if (ImGui::Button("Display vertex normals ")){
                  model.SetAxes(5);
              }
              ImGui::Separator();
              //world features
              ImGui::Text("World Features");
              if (ImGui::Button("Display World Axis")){
                  model.SetAxes(1);
              }
              if (ImGui::Button("Display World bounding box")){
                  model.SetAxes(3);
              }
              ImGui::Separator();
              //2b features.
              ImGui::Text("Light Features");
              if (ImGui::Button("Display Light and Reflections vecs")) {
                  model.SetAxes(8);
              }
              ImGui::End();
          }

        ImGui::Begin("Lights");
        if (scene.HasLights()) {
            ImGui::Text("Current Lights:");
            if (ImGui::Button("Remove All"))
                scene.ClearAllLights();
            ImGui::SameLine();
        }
        else {
            ImGui::Text("Add Light");
        }
        if (ImGui::Button("Add Light"))
            scene.AddLight();
        
        size_t lights_count = scene.GetLightCount();
        for (int i = 0; i < lights_count; i++)
        {
            auto& current_light = scene.GetLightAtIndex(i);
            ImGui::PushID(i);
            if (ImGui::CollapsingHeader(scene.GetLightAtIndex(i).GetName().c_str()))
            {
                if (ImGui::Button("Remove")) {
                        scene.RemoveLight(i);
                        ImGui::PopID();
                        break;
                }
                if (ImGui::TreeNode("Reflection"))
                {
                    ImGui::ColorEdit3("Ambient", (float*)&current_light.ambient);
                    ImGui::ColorEdit3("Diffuse", (float*)&current_light.diffuse);
                    ImGui::ColorEdit3("Specular", (float*)&current_light.specular);
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Position"))
                {
                    auto& pos = current_light.GetPosition();
                    ImGui::InputFloat("x", (float*)&pos.x, 1.0f, 1.0f, "%.2f");
                    ImGui::InputFloat("y", (float*)&pos.y, 1.0f, 1.0f, "%.2f");
                    ImGui::InputFloat("z", (float*)&pos.z, 1.0f, 1.0f, "%.2f");
                    ImGui::TreePop();
                }
            }
            ImGui::PopID();
        }
        ImGui::End();
    }






