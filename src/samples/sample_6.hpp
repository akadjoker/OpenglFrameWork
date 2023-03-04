/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_6.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:14:07 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/03 20:22:33 by lrosa-do         ###   ########.fr       */
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

//ModernMesh *mesh = ModernMesh::ImportMesh(Mesh::CreateCube(1,1,1),true);
Mesh *mesh = Mesh::ImportOBJ("assets/african_head.obj");

ShadowMesh *shadow = new ShadowMesh(mesh);

Mesh *sphere = Mesh::CreateSphere(12);
 

Mesh *plane=Mesh::CreateHills(Vec2(5,5),10,10,0.2,Vec2(5,5),Vec2(8,8));

Shader shader_color = Shader::createColor();

  
 
   Mat4 projection = Mat4::ProjectionMatrix(45.0f,  static_cast<float>(screenWidth) / screenHeight, 0.1f, 1000.0f);
    

glClearColor(0.1,0.1,0.3f,1.0);
glEnable(GL_DEPTH_TEST);  
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
// glEnable(GL_STENCIL_TEST);    
// glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
//glFrontFace(GL_CW);
//glEnable(GL_CULL_FACE);  
//  //  glCullFace(GL_FRONT);
// 	glEnable(GL_DEPTH_TEST);
// 	glDepthMask(GL_TRUE);
// 	glClearDepthf(1.0);
// 	glDepthFunc(GL_LEQUAL);
// 	//glDepthFunc(GL_LESS);
// 	glEnable(GL_CULL_FACE);
// 	glEnable(GL_SCISSOR_TEST);
// 	glEnable(GL_BLEND);
//   glFrontFace(GL_CW);
//   glEnable(GL_DEPTH_TEST);
//   glDepthFunc(GL_LESS);
glEnable(GL_STENCIL_TEST);
glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 


    
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
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
       

        Mat4 view =camera.GetViewMatrix();
  
  




        Mat4 model= Mat4::Identity();// * Mat4::Scale(0.8f,0.8f,0.8f);


        //   // --------------------------------------------------------------------
      //   glStencilFunc(GL_ALWAYS, 1, 0xFF);
      //   glStencilMask(0xFF);
      //  shadow->render(model,ligth_pos);
        

      //   //   // -----------------------------------------------------------------------------------------------------------------------------
      //    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      //    glStencilMask(0x00);
      //    glDisable(GL_DEPTH_TEST);
      //    shadow->render(model,ligth_pos);




        model= Mat4::Identity();
        normal_shader.Bind();
        normal_shader.setMatrix4("model", model);
        normal_shader.setMatrix4("view", view);
        normal_shader.setMatrix4("projection" ,projection);
        normal_shader.setVector3("lightPos",ligth_pos);
        text_mesh.Bind(0);
        text_normal_mesh.Bind(1);
        mesh->Render(normal_shader);

   
      
        
//floor
        shader.Bind();
        shader.setMatrix4("view", view);
        shader.setMatrix4("projection" ,projection);
        shader.setVector3("lightPos",ligth_pos);
        texture_floor.Bind(0);
        model= Mat4::Translate(0,-1,0);
        shader.setMatrix4("model", model);
        plane->Render(shader);
      

        
   
//sphe re light
        model= Mat4::Translate(ligth_pos) * Mat4::Scale(0.2f,0.2f,0.2f);
        shader_solid.Bind();
        shader_solid.setMatrix4("view", view);
        shader_solid.setMatrix4("projection" ,projection);
        shader_solid.setFloat4("color",1.0f, 1.0f, 1.0f,1.0f);
        shader_solid.setMatrix4("model", model);
        sphere->Render(shader_solid);


        model= Mat4::Translate(0,0,0);
        shader_color.Bind();
        shader_color.setMatrix4("model", model);
        shader_color.setMatrix4("view", view);
        shader_color.setMatrix4("projection" ,projection);
        shadow->render(model,ligth_pos);

        
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
delete shadow;


    
  std::cout<<"Exit"<<std::endl;
  return 0;
}