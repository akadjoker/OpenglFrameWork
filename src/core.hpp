/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 13:51:30 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/04 10:06:19 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "glad.h"
#include "utils.hpp"
#include "math.hpp"
#include "render.hpp"

class Input
{
    
    
};

class FileStream
{
    public:
        FileStream()
        {
            
        }
        ~FileStream();

       
        int readInt()
        {
                int v;
                SDL_RWread(m_file, &v, sizeof(int), 1);
                return v;
        }
        float readFloat()
        {
                float v;
                SDL_RWread(m_file, &v, sizeof(float), 1);
                return v;
        }
        char readChar()
        {
                char v;
                SDL_RWread(m_file, &v, sizeof(char), 1);
                return v;
        }
        std::string readString();
        FileStream(const std::string &filename);

 
        
   
 
    private:
     SDL_RWops* m_file;
};
  
class App
{
    public:
        App();
        ~App();
        bool CreateWindow(int width, int height, const std::string &tile, bool vsync = true);
        bool ShouldClose();
        void Close();
        void Swap();
        void Wait(float ms);
        void SetTargetFPS(int fps);
        int GetFPS(void);
  
        SDL_Window* getWindow(){return window;};

        float GetFrameTime(void)
        {
            return (float)m_frame;
        }
        double GetTime(void)
        {
        return (double) SDL_GetTicks()/1000.0;
        }
    
    private:
        SDL_Window *window;
        SDL_GLContext context;
        Render *render;
        bool m_shouldclose;
        //time
        double m_current;                     // Current time measure
        double m_previous;                    // Previous time measure
        double m_update;                      // Time measure for frame update
        double m_draw;                        // Time measure for frame draw
        double m_frame;                       // Time measure for one frame
        double m_target;                      // Desired time for one frame, if 0 not applied
};