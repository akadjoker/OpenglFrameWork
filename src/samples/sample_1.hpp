/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_1.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:14:07 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/03 09:46:11 by lrosa-do         ###   ########.fr       */
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


int GetRandomValue(int min, int max)
{
    if (min > max)
    {
        int tmp = max;
        max = min;
        min = tmp;
    }  

    return (rand()%(abs(max - min) + 1) + min);
}
   
int run_sample()   
{ 
App app;
app.CreateWindow(screenWidth, screenHeight, "Vertex Texture Colored",false);

//Shader shader=Shader::createSolidColor();
Shader shader=Shader::createAmbientDiffuseSpecular();
  
//Shader shader=Shader::createSolidColorTexture();

Camera camera; 
camera.SetPosition(0, 1, 10);  

Texture2D texture;//(64,64);
texture.Load("assets/cube.png");

Texture2D texture_floor;//(64,64);
texture_floor.Load("assets/grass_1024.jpg");

Texture2D text_mesh;//(64,64);
text_mesh.Load("assets/african_head_diffuse.tga",true);




Mesh *plane=Mesh::CreateHills(Vec2(5,5),10,10,0.2,Vec2(5,5),Vec2(8,8));
//Mesh::CreatePlane(50,50,Color(0.5f,0.5f,0.5f));

Mesh *cube= Mesh::CreateCube(1,1.5,1);

 Mesh *sphere = Mesh::CreateSphere();

 Mesh *cilinder = Mesh::CreateCylinder();

 Mesh *cone = Mesh::CreateCone();

 Mesh *torus = Mesh::CreateTorus(0.2f,1.0f);

 Mesh *mesh =Mesh::LoadMesh("assets/african_head.h3d");

 
//sphere.CreateSphere(1,32,32,Color(1,1,0));


  
 
   Mat4 projection = Mat4::ProjectionMatrix(45.0f,  static_cast<float>(screenWidth) / screenHeight, 0.1f, 1000.0f);
    

   glClearColor(0.1,0.1,0.3f,1.0);
   glEnable(GL_DEPTH_TEST);  
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  



int frameCount = 0;
Uint32 startTime = SDL_GetTicks();
Vec3 ligth_pos(1.2f, 5.5f, 2.0f);

  

  while (!app.ShouldClose()) 
  {
       
       float deltaTime =  app.GetFrameTime();
         double timer =app.GetTime();

        
        ligth_pos.x = 1.0f + sin(timer) * 2.0f;
               ligth_pos.z = sin(timer / 2.0f) * 1.0f;

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
 
 
             
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       

        
  
  
      
       
        Mat4 view =camera.GetViewMatrix();
        Mat4 model= Mat4::Identity();
        shader.Bind();
        shader.setMatrix4("view", view);
        shader.setMatrix4("projection" ,projection);
        shader.setMatrix4("model", model);
        shader.setVector3("viewPos",camera.Position);
        shader.setVector3("lightPos",ligth_pos);
       

         model= Mat4::Identity();
        shader.setMatrix4("model", model);
        plane->Render(shader);

             texture.Bind(0);
		 
        model= Mat4::Translate(-1,1,0);
        shader.setMatrix4("model", model);
        cube->Render(shader);
		 
 

        model= Mat4::Translate(1,1,0);
        shader.setMatrix4("model", model);
        sphere->Render(shader);
		 
        model= Mat4::Translate(3,1,0);
        shader.setMatrix4("model", model);
        cilinder->Render(shader);

        model= Mat4::Translate(-3,1,0);
        shader.setMatrix4("model", model);
        cone->Render(shader);


        model= Mat4::Translate(-5,1,0);
        shader.setMatrix4("model", model);
        torus->Render(shader);
        


        text_mesh.Bind(0);
        model= Mat4::Translate(0,1,5);
        shader.setMatrix4("model", model);
        mesh->Render(shader);
        
        
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
 
 delete plane;
 delete cube;
 delete sphere;
 delete cilinder;
 delete cone;
 delete torus;
delete mesh;

  std::cout<<"Exit"<<std::endl;
  return 0;
}