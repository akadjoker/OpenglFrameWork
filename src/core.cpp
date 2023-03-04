/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:20:19 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/03 19:35:46 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "core.hpp"




//**************************************************************************************************************
//
//                                                 APPLICATION
//
//*************************************************************************************************************


App::App()
{
        if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        {
                Log(2,"SDL could not initialize! Error: %s", SDL_GetError());
            return ;
        }
        m_shouldclose=false;

    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES)!=0)
    {
        SDL_Log( "ERROR loading context profile mask");
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)!=0)
    {
        SDL_Log("ERROR setting context major version");
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0)!=0)
    {
        SDL_Log("ERROR setting context minor version");
    }
    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0){
        SDL_Log( " ERROR \n setting double buffer");
    } // I have tried without the dubble buffer and nothing changes
    if(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE   , 24) != 0){
        SDL_Log( " ERROR \n setting depth buffer");
    }
    if(SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32)!=0){
        SDL_Log( "ERROR setting buffer size");
    }
    if(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8)!=0){
        SDL_Log( " ERROR loading red");
    }
    if(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8)!=0){
        SDL_Log( " ERROR loading red");
    }if(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8)!=0){
        SDL_Log( " ERROR loading red");
    }if(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8)!=0){
        SDL_Log(" Error setting alpha");
    }

   if(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8) != 0){
        SDL_Log( " Error  setting stencil buffer");
    }



        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        // SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    

}
App::~App()
{
    Close();
    SDL_Quit();
    Log(0,"Unload and free");
}

bool App::CreateWindow(int width, int height, const std::string &tile, bool vsync )
{
window = SDL_CreateWindow(tile.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
if (!window) 
{
    std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
    
    return false;
}

    context = SDL_GL_CreateContext(window);
    if (!context) 
    {
        Log(2, "OpenGL context could not be created! %s ", SDL_GetError());
        SDL_DestroyWindow(window);
        return false; 
    }

    gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    Log(0,"Vendor  :  %s",glGetString(GL_VENDOR));
    Log(0,"Renderer:  %s",glGetString(GL_RENDERER));
    Log(0,"Version :  %s",glGetString(GL_VERSION));

    srand((unsigned int)SDL_GetTicks());              // Initialize random seed
    m_previous = GetTime();       // Get time as double
    Random_Seed(0);

    (vsync==true)?SDL_GL_SetSwapInterval(1):SDL_GL_SetSwapInterval(0);

    return true;
}



bool App::ShouldClose()
{
        m_current = GetTime();            // Number of elapsed seconds since InitTimer()
        m_update = m_current - m_previous;
        m_previous = m_current;


    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        
        switch (event.type)
        {
            case SDL_QUIT:
            {
                m_shouldclose = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                if (event.key.keysym.sym==SDLK_ESCAPE)
                {
                    m_shouldclose = true;
                    break;
                }
        
                break;
            }
            
            case SDL_KEYUP:
            {
            
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
        

            }break;
            case SDL_MOUSEBUTTONUP:
            {
                
                break;
            }
            case SDL_MOUSEMOTION:
            {

            break;   
            }
            
            case SDL_MOUSEWHEEL:
            {
            
            break;
            }
        }
    } 
    
    return m_shouldclose;
}

void App::Close()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}

void App::Swap()
{
    SDL_GL_SwapWindow(window);
        // Frame time control system
    m_current = GetTime();
    m_draw = m_current - m_previous;
    m_previous = m_current;

    m_frame = m_update + m_draw;

    // Wait for some milliseconds...
    if (m_frame < m_target)
    {
        Wait((float)(m_target -m_frame)*1000.0f);

        m_current = GetTime();
        double waitTime = m_current - m_previous;
        m_previous = m_current;

        m_frame += waitTime;      // Total frame time: update + draw + wait
    }
     glBindVertexArray(0);
     glBindBuffer(GL_ARRAY_BUFFER, 0); 
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  0); 
        
}
void App::Wait(float ms)
{
SDL_Delay((int)ms);
}



void App::SetTargetFPS(int fps)
{
    if (fps < 1) m_target = 0.0;
    else m_target = 1.0/(double)fps;

    Log(0,"TIMER: Target time per frame: %02.03f milliseconds", (float)m_target*1000);
}

int App::GetFPS(void)
{
    #define FPS_CAPTURE_FRAMES_COUNT    30      // 30 captures
    #define FPS_AVERAGE_TIME_SECONDS   0.5f     // 500 millisecondes
    #define FPS_STEP (FPS_AVERAGE_TIME_SECONDS/FPS_CAPTURE_FRAMES_COUNT)

    static int index = 0;
    static float history[FPS_CAPTURE_FRAMES_COUNT] = { 0 };
    static float average = 0, last = 0;
    float fpsFrame = GetFrameTime();

    if (fpsFrame == 0) return 0;

    if ((GetTime() - last) > FPS_STEP)
    {
        last = (float)GetTime();
        index = (index + 1)%FPS_CAPTURE_FRAMES_COUNT;
        average -= history[index];
        history[index] = fpsFrame/FPS_CAPTURE_FRAMES_COUNT;
        average += history[index];
    }

    return (int)roundf(1.0f/average);
}


//**************************************************************************************************************
//
//                                                 FILE STREAM
//
//*************************************************************************************************************




FileStream::FileStream(const std::string &filename)
{
    if (FileExists(filename.c_str()))
    {
        m_file = SDL_RWFromFile(filename.c_str(), "rb");
        Log(0, "FILEIO: [%s] File loaded successfully", filename.c_str());
                
    } else
        Log(1, "FILEIO: [%s] Failed to open file", filename.c_str());
}


std::string FileStream::readString()
{
        int size = readInt();
        char buffer[256];
        SDL_RWread(m_file, &buffer, size * sizeof(char), 1);
        buffer[size]='\0';
        std::string str(buffer);
        return str;
}


FileStream::~FileStream()
{
        Log(0, "FILEIO: Close stream .");
    if (m_file)
        SDL_RWclose(m_file);
}

//**************************************************************************************************************
//
//                                                 MEMORY STREAM
//
//*************************************************************************************************************


