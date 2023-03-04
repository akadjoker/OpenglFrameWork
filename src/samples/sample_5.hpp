/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_5.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:14:07 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/03 09:51:59 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "glad.h"

#include "../pch.hpp"
#include "../utils.hpp"
#include "../render.hpp"
#include "../math.hpp"
#include "../camera.hpp"
#include "../core.hpp"
  
  
const int screenWidth = 1280;
const int screenHeight = 720;



   
int run_sample()   
{ 
App app;
app.CreateWindow(screenWidth, screenHeight, "Vertex Texture Colored",false);

Shader shader_solid=Shader::createColor();


Shader shader=Shader::createColorAmbientDiffuse();


Shader normal_shader=Shader::createTangentNormalMap();
  
//Shader shader=Shader::createSolidColorTexture();

Camera camera; 
camera.SetPosition(0, 1, 10);  



Texture2D text_mesh;//(64,64);
text_mesh.Load("assets/african_head_diffuse.tga",true);

Texture2D text_normal_mesh;//(64,64);
text_normal_mesh.Load("assets/african_head_nm.tga",true);


Texture2D texture_floor;//(64,64);
texture_floor.Load("assets/grass_1024.jpg");

//Mesh *mesh = Mesh::LoadMesh("assets/african_head.h3d");
ModernMesh *mesh = ModernMesh::ImportOBJ("assets/african_head.obj");


Mesh *sphere = Mesh::CreateSphere(12);
 

Mesh *plane=Mesh::CreateHills(Vec2(5,5),10,10,0.2,Vec2(5,5),Vec2(8,8));


  
 
   Mat4 projection = Mat4::ProjectionMatrix(45.0f,  static_cast<float>(screenWidth) / screenHeight, 0.1f, 1000.0f);
    

   glClearColor(0.1,0.1,0.3f,1.0);
   glEnable(GL_DEPTH_TEST);  
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  



int frameCount = 0;
Uint32 startTime = SDL_GetTicks();

Vec3 ligth_pos(1.2f, 1.0f, 2.0f);
  

  while (!app.ShouldClose()) 
  {
       
       float deltaTime =  app.GetFrameTime();
       double timer =app.GetTime() /10;

        
 

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

         if(keys[SDL_SCANCODE_W])
            camera.ProcessKeyboard(FORWARD, deltaTime);
        else if(keys[SDL_SCANCODE_S])
           camera.ProcessKeyboard(BACKWARD, deltaTime);
  
        if(keys[SDL_SCANCODE_A])
             camera.ProcessKeyboard(LEFT, deltaTime);
        else if(keys[SDL_SCANCODE_D])
             camera.ProcessKeyboard(RIGHT, deltaTime);
           
         
        int MouseX,MouseY;
        
        Uint32 button = SDL_GetRelativeMouseState(&MouseX, &MouseY);
        if (button & SDL_BUTTON(SDL_BUTTON_LEFT)) 
        {
            camera.ProcessMouseMovement(MouseX, -MouseY);
        } 
 
     ligth_pos.x=0 + sinf(timer/2 *PI) *5.5f;
     ligth_pos.z=0 + cosf(timer/2 *PI) *5.5f;
   
             
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       

        Mat4 view =camera.GetViewMatrix();
  
  

        shader.Bind();
        shader.setMatrix4("view", view);
        shader.setMatrix4("projection" ,projection);
        shader.setVector3("lightPos",ligth_pos);




        Mat4 model= Mat4::Identity();
        normal_shader.setMatrix4("model", model);
        normal_shader.setMatrix4("view", view);
        normal_shader.setMatrix4("projection" ,projection);
        normal_shader.setVector3("lightPos",ligth_pos);

        text_mesh.Bind(0);
        text_normal_mesh.Bind(1);
        mesh->Render(normal_shader);
        
        texture_floor.Bind(0);
        model= Mat4::Translate(0,-1,0);
        shader.setMatrix4("model", model);
        plane->Render(shader);
      
      
        
   

        model= Mat4::Translate(ligth_pos) * Mat4::Scale(0.2f,0.2f,0.2f);

        shader_solid.Bind();
        shader_solid.setMatrix4("view", view);
        shader_solid.setMatrix4("projection" ,projection);

        shader_solid.setFloat4("color",1.0f, 1.0f, 1.0f,1.0f);
        shader_solid.setMatrix4("model", model);
        sphere->Render(shader_solid);
        
        app.Swap();



         frameCount++;
        if (SDL_GetTicks() - startTime >= 1000)
        {
            char title[150];

                snprintf(title, sizeof( title), "  FPS: %d", frameCount);

            SDL_SetWindowTitle(app.getWindow(), title);
            frameCount = 0;
            startTime = SDL_GetTicks();
        }
  } 
 

delete mesh;
delete sphere;
delete plane;

  std::cout<<"Exit"<<std::endl;
  return 0;
}