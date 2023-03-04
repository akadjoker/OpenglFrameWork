/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 13:23:53 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/03 20:36:36 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.hpp"
#include "utils.hpp"
#include "core.hpp"



 int FVFDecodeLength(unsigned int fvf)
{
	int nSize = 0;
	if (fvf & FVF_XYZ)		nSize+=3 * sizeof(FLOAT);
	if (fvf & FVF_NORMAL)	nSize+=3 * sizeof(FLOAT);

	if (fvf & FVF_COLOR)	nSize+=4 * sizeof(BYTE);

    if (fvf & FVF_FCOLOR)	nSize+=4 * sizeof(FLOAT);

	if (fvf & FVF_BINORMAL)	nSize+=3 * sizeof(FLOAT);
    if (fvf & FVF_TANGENT)	nSize+=3 * sizeof(FLOAT);


	if (fvf & FVF_TEX1)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX2)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX3)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX4)		nSize+=2 * sizeof(FLOAT);
	if (fvf & FVF_TEX5)		nSize+=2 * sizeof(FLOAT);

	return nSize;
}



UINT getTypeFormat(VertexAttributeType etype)
{
    switch(etype)
    {
    case VET_FLOAT1:
        return GL_FLOAT;
    case VET_FLOAT2:
        return GL_FLOAT;
    case VET_FLOAT3:
        return GL_FLOAT;
    case VET_FLOAT4:
        return GL_FLOAT;
    case VET_SHORT1:
        return GL_SHORT;
    case VET_SHORT2:
        return GL_SHORT;
    case VET_SHORT3:
        return GL_SHORT;
    case VET_SHORT4:
        return GL_SHORT;
    case VET_BYTE3:
        return GL_UNSIGNED_BYTE;
    case VET_BYTE4:
        return GL_UNSIGNED_BYTE;
    }
    return 0;
}
UINT getTypeSize(VertexAttributeType etype)
{
    switch(etype)
    {
    case VET_FLOAT1:
        return sizeof(float);
    case VET_FLOAT2:
        return sizeof(float)*2;
    case VET_FLOAT3:
        return sizeof(float)*3;
    case VET_FLOAT4:
        return sizeof(float)*4;
    case VET_SHORT1:
        return sizeof(short);
    case VET_SHORT2:
        return sizeof(short)*2;
    case VET_SHORT3:
        return sizeof(short)*3;
    case VET_SHORT4:
        return sizeof(short)*4;
    case VET_BYTE3:
        return sizeof(unsigned char)*3+1;
    case VET_BYTE4:
        return sizeof(unsigned char)*4+1;
    }
    return 0;
}


UINT getTypeCount(VertexAttributeType etype)
{
    switch (etype)
    {
    case VET_FLOAT1:
        return 1;
    case VET_FLOAT2:
        return 2;
    case VET_FLOAT3:
        return 3;
    case VET_FLOAT4:
        return 4;
    case VET_SHORT1:
        return 1;
    case VET_SHORT2:
        return 2;
    case VET_SHORT3:
        return 3;
    case VET_SHORT4:
        return 4;
    case VET_BYTE3:
        return 3;
    case VET_BYTE4:
        return 4;
    }
    return 0;
}


//**************************************************************************************************************
//
//                                                 TEXTURE2D
//
//*************************************************************************************************************

Texture2D::Texture2D(int w, int h)
{
    width =w;
    height=h;
    components=4;

    unsigned char *data =( unsigned char *)malloc(width * height * components);
    if (data == nullptr) 
    {
        Log(2,"Failed to cretae image ");
        return ;
    }
    memset(data, 0xFF, width * height * components);
    
        GLenum  format = GL_RGBA;

    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //glBindTexture(GL_TEXTURE_2D, 0);
    Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D", id);

    free(data);
    
}
bool Texture2D::Load(const std::string &file_name,bool flip_y)
{
    unsigned int bytesRead;
    unsigned char *fileData =  LoadFileData(file_name.c_str(),&bytesRead);
    if(!fileData)
        return false;

if (flip_y)
{
    stbi_set_flip_vertically_on_load(1);
}
unsigned char *data =  stbi_load_from_memory(fileData,bytesRead,&width, &height, &components, STBI_default);

if (data == nullptr) 
{
    Log(2,"Failed to load image: %s",file_name.c_str());
    return false;
}

GLenum format;
switch (components) 
{
    case STBI_grey:
    format = GL_RED;
    break;
    case STBI_grey_alpha:
    format = GL_RG;
    break;
    case STBI_rgb:
    format = GL_RGB;
    break;
    case STBI_rgb_alpha:
    format = GL_RGBA;
    break;
    default:
    free(data);
    return false;
}

glGenTextures(1, &id);
glBindTexture(GL_TEXTURE_2D, id);
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);

//glBindTexture(GL_TEXTURE_2D, 0);
Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D", id);
free(fileData);
free(data);
return true;
}
void Texture2D::Bind(UINT unit) 
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture2D::UnBind(UINT unit) 
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}


TextureCube::TextureCube(std::vector<std::string> faces)
{

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(0);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
             Log(2,"Failed to load image: %s",faces[i].c_str());
             stbi_image_free(data);
        }
    }
 

}
void TextureCube::Bind()
{
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_CUBE_MAP, id);

}
void TextureCube::UnBind()
{
 glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

} 

//**************************************************************************************************************
//
//                                                 SHADER
//
//*************************************************************************************************************

Shader Shader::createSkyBox()
{
    const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
           
    out vec3 TexCoords;

    uniform mat4 projection;
    uniform mat4 view;
    


    void main()
    {
       TexCoords = aPos;
       
      // mat4  fview =mat4(mat3(view)); // remove translation from the view matrix
       
      vec4 pos = projection * view * vec4(aPos, 1.0);
      gl_Position = pos.xyww;
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;

    in vec3 TexCoords;

    uniform samplerCube skybox;

    void main()
    {    
        FragColor = texture(skybox, TexCoords);
    }
        
        
    
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();
    shader.setInt("skybox", 0);
  
    return shader;
}
Shader Shader::createColor()
{
    const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
           
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
   


    void main()
    {
       gl_Position = projection * view * model* vec4(aPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;

    uniform vec4 color;


    void main()
    {
    
    FragColor = color;
      
        
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();
    shader.setFloat4("color", 0.4, 0.4, 0.4, 1.0);
  
    return shader;
}
Shader Shader::createColorAmbientDiffuse()
{
        const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 2) in vec3 aNormal;
            
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
   
    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    void main()
    {
            
        TexCoord = aTexCoord;
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = aNormal;  

        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;
    

    in vec2 TexCoord;
    in vec3 Normal;
    in vec3 FragPos;    

    uniform sampler2D tex;
    uniform vec3 lightPos; 
    uniform vec3 lightColor;
    uniform vec3 objectColor;


    void main()
    {
    
    vec4 texColor = texture(tex, TexCoord);
        
   // ambient
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0) + texColor;
      
        
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();
    shader.setInt("tex",0);
    shader.setFloat3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setFloat3("lightPos", 1.2f, 1.0f, 2.0f);
    return shader;
}

Shader Shader::createColorAmbientDiffuseSpecular()
{
        const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 2) in vec3 aNormal;
            
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
   
    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    void main()
    {
            
        TexCoord = aTexCoord;
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = aNormal;  

        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;
    

    in vec2 TexCoord;
    in vec3 Normal;
    in vec3 FragPos;    

    uniform sampler2D tex;
    uniform float force;
    uniform vec3 viewPos;
    uniform vec3 lightPos; 
    uniform vec3 lightColor;
    uniform vec3 objectColor;
    uniform vec3 diffuseColor;
    uniform vec3 specularColor;


    void main()
    {
    
    vec4 texColor = texture(tex, TexCoord);
        
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), force);
    vec3 specular = specularStrength * spec * specularColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0) + texColor;
      
        
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();
    shader.setInt("tex",0);
    shader.setFloat3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setFloat3("lightPos", 1.2f, 1.0f, 2.0f);
    shader.setFloat3("diffuseColor",  1.0f, 1.0f, 1.0f);
    shader.setFloat3("specularColor", 1.0f, 1.0f, 1.0f);
          
    shader.setFloat("force",32.0f);
    return shader;
}


Shader Shader::createSolidAmbientDiffuseSpecular()
{
        const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
    layout (location = 2) in vec3 aNormal;
            
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 Normal;
    out vec3 FragPos;

    void main()
    {
     
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = aNormal;  

        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;
    

    in vec3 Normal;
    in vec3 FragPos;    

    uniform float force;
    uniform vec3 viewPos;
    uniform vec3 lightPos; 

    uniform vec3 ambientColor;
    uniform vec3 diffuseColor;
    uniform vec3 specularColor;


    void main()
    {

        
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * ambientColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), force);
    vec3 specular = specularStrength * spec * specularColor;  
        
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0) ;
      
        
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();

    shader.setFloat3("lightPos", 1.2f, 1.0f, 2.0f);
    shader.setFloat3("diffuseColor" ,  1.0f, 1.0f, 1.0f);
    shader.setFloat3("specularColor", 1.0f, 1.0f, 1.0f);
    shader.setFloat3("ambientColor" , 1.0f, 1.0f, 1.0f);
          
    shader.setFloat("force",32.0f);
    return shader;
}

Shader Shader::createAmbientDiffuseSpecular()
{
        const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    layout (location = 2) in vec3 aNormal;
            
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
   
    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    void main()
    {
            
        TexCoord = aTexCoord;
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = aNormal;  

        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;
    

    in vec2 TexCoord;
    in vec3 Normal;
    in vec3 FragPos;    

    uniform sampler2D tex;
    uniform float force;
    uniform vec3 viewPos;
    uniform vec3 lightPos; 
    uniform vec3 lightColor;

    uniform vec3 ambient;
    uniform vec3 diffuse;
    uniform vec3 specular;
    uniform float shininess;


    void main()
    {
    
    vec4 texColor = texture(tex, TexCoord);
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), force);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) ;
    FragColor = vec4(result, 1.0) + texColor;
      
        
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();
    shader.setInt("tex",0);
    shader.setFloat3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.setFloat3("lightPos", 1.2f, 1.0f, 2.0f);
    shader.setFloat("force",32.0f);
    return shader;
}
Shader Shader::createColorMaterial()
{
        const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
            
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
   

    out vec3 Normal;
    out vec3 FragPos;

    void main()
    {
            

        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = aNormal;  

        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

    void main()
    {
        // ambient
        vec3 ambient = light.ambient * material.ambient;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse);
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular);  
            
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();

    shader.setFloat3("light.ambient", 1.0f, 1.0f, 1.0f);
    shader.setFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    shader.setFloat3("material.ambient", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("material.diffuse", 1.0f, 0.5f, 0.31f);
    shader.setFloat3("material.specular", 0.5f, 0.5f, 0.5f); 
    shader.setFloat("material.shininess", 32.0f);
    
    return shader;
}

Shader Shader::createTangentNormalMap()
{
        const char *vertexShaderSource = R"(
    #version 320 es
    precision mediump float;
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;
    layout (location = 2) in vec3 aNormal;
    layout (location = 3) in vec3 aTangent;
 

            
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
   
  out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform vec3 lightPos;
uniform vec3 viewPos;

    void main()
    {
            
        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
        vs_out.TexCoords = aTexCoords;

        mat3 normalMatrix = transpose(inverse(mat3(model)));
        vec3 T = normalize(normalMatrix * aTangent);
        vec3 N = normalize(normalMatrix * aNormal);
        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(N, T);

        mat3 TBN = transpose(mat3(T, B, N));    
        vs_out.TangentLightPos = TBN * lightPos;
        vs_out.TangentViewPos  = TBN * viewPos;
        vs_out.TangentFragPos  = TBN * vs_out.FragPos;
        
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    )";

    const char *fragmentShaderSource = R"(
    #version 320 es
    precision mediump float;
    out vec4 FragColor;
    

    in VS_OUT {
        vec3 FragPos;
        vec2 TexCoords;
        vec3 TangentLightPos;
        vec3 TangentViewPos;
        vec3 TangentFragPos;
    } fs_in;

    uniform sampler2D diffuseMap;
    uniform sampler2D normalMap;

    uniform vec3 lightPos;
    uniform vec3 viewPos;


    void main()
    {
    
      // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
      
        
    }
    )";
    Shader shader;
    shader.create(vertexShaderSource,fragmentShaderSource);
    shader.LoadDefaults();
    shader.setInt("diffuseMap",0);
    shader.setInt("normalMap",1);
    shader.setFloat3("lightPos", 0.5f, 1.0f, 0.3f);
    return shader;
}
bool Shader::create(const char* vShaderCode, const char* fShaderCode)
{
    // 2. compile shaders
    unsigned int vertex, fragment;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // shader Program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    checkCompileErrors(m_program, "PROGRAM");

    if (m_program>0)
        Log(0, "SHADER: [ID %i] Create shader program.", m_program);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    Bind();
    return true;
}
bool Shader::findUniform(const std::string &name)const
{
    std::map<std::string, int>::const_iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        if (strcmp(it->first.c_str(),name.c_str())==0)
        {
        return true;
        }
        it++;
    }
    return false;
}
int  Shader::getUniform(const std::string &name)const
{
    std::map<std::string, int>::const_iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        if (strcmp(it->first.c_str(),name.c_str())==0)
        {
        return it->second;
        }

        it++;
    }
        return -1;
}
int Shader::getUniformLocation(const std::string &uniformName) const
{
    int location = -1;
    location =getUniform(uniformName);//uniforms[uniformName];
    if (location == -1)
        Log(2, "SHADER: [ID %i] Failed to find shader uniform: %s", m_program, uniformName.c_str());

//  else SDL_Log( "SHADER: [ID %i] IShader uniform (%s) set at location: %i", id, uniformName, location);
    return location;
}
int Shader::getAttribLocation( const std::string &attribName) const
{
    int location = -1;
    location = glGetAttribLocation(m_program, attribName.c_str());
    if (location == -1)
        Log(2, "SHADER: [ID %i] Failed to find shader attribute: %s", m_program, attribName.c_str());
    //App::getInstance()->ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SHADER Error",TextFormat("SHADER Failed to find shader attribute: %s)",attribName));
//  else SDL_Log( "SHADER: [ID %i] IShader attribute (%s) set at location: %i", id, attribName, location);
    return location;
}
bool Shader::addUniform(const std::string &name)
{
    int location = -1;
    location = glGetUniformLocation(m_program, name.c_str());
    if (location == -1)
    {
        Log(2, "SHADER: [ID %i] Failed to find shader uniform: %s", m_program, name.c_str());
        //ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SHADER Error",TextFormat("SHADER Failed to find shader uniform: %s)",name));
        return false;
    }

    // Log(0, "SHADER: [ID %i] shader uniform (%s) set at location: %i", m_program, name.c_str(), location);
    m_uniforms.insert(std::make_pair(name, location));

return true;
}
bool Shader::addAttribute(const std::string &name)
{
int location = -1;
    location = glGetAttribLocation(m_program, name.c_str());

    if (location == -1)
    {
        Log(2, "SHADER: [ID %i] Failed to find shader attribute: %s", m_program, name.c_str());
    //ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"SHADER Error",TextFormat("SHADER Failed to find shader attribute: %s)",name));
    return false;
    }

    //  Log(2, "SHADER: [ID %i] shader attribute (%s) set at location: %i", m_program, name.c_str(), location);
    m_attributes.insert(std::make_pair(name, location));
    return true;
}
void Shader::printData()
{

Log(0,"[SHADER]  Id(%d) Num Attributes(%d)  Num Uniforms (%d)",m_program, m_numAttributes,m_numUniforms);
{
        std::map<std::string, int>::iterator it = m_attributes.begin();
        while(it != m_attributes.end())
        {
            Log(0,"[SHADER]  Id(%d)  attribute  index(%d) name(%s)",m_program,it->second, it->first.c_str());
            it++;
        }
}
{
    std::map<std::string, int>::iterator it = m_uniforms.begin();
        while(it != m_uniforms.end())
        {
            Log(0,"[SHADER]  Id(%d) uniform index(%d) name(%s)",m_program,it->second, it->first.c_str());

            it++;
        }
    }
}
void Shader::LoadDefaults()
{
    GLint numActiveAttribs = 0;
    GLint maxAttribNameLength = 0;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
    m_numAttributes=numActiveAttribs;
    for(GLint attrib = 0; attrib < numActiveAttribs; ++attrib)
    {
        int namelen = -1;
        int num = -1;
        char name[256]; // Assume no variable names longer than 256
        GLenum type = GL_ZERO;
        glGetActiveAttrib(m_program, attrib,  sizeof(name) - 1, &namelen, &num, &type, name);
        name[namelen] = 0;
        addAttribute(std::string((char*)&name[0]));
        glBindAttribLocation(m_program, attrib, (char*)&name[0]);
        Log(0,"SHADER: [ID %i] Active attribute (%s) set at location: %i", m_program, name,attrib);
    }
    // Get available shader uniforms
    int uniformCount = -1;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);
    m_numUniforms=uniformCount;

    for (int i = 0; i < uniformCount; i++)
    {
        int namelen = -1;
        int num = -1;
        char name[256]; // Assume no variable names longer than 256
        GLenum type = GL_ZERO;
        glGetActiveUniform(m_program, i, sizeof(name) - 1, &namelen, &num, &type, name);
        name[namelen] = 0;
        addUniform(std::string((char*)&name[0]));
        Log(0,"SHADER: [ID %i] Active uniform (%s) set at location: %i", m_program, name, glGetUniformLocation(m_program, name));
    }
}
void Shader::checkCompileErrors(unsigned int shader,const std::string &type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            Log(2,"SHADER: Error Compile %s Msg: %s", type.c_str(), infoLog );
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            Log(2,"SHADER: Error Link %s Msg: %s", type.c_str(), infoLog );
        }
    }
}


//**************************************************************************************************************
//
//                                                 BUFFERS
//
//*************************************************************************************************************

IndexBuffer *VertexData::createIndexBuffer( E_INDEX_TYPE type, UINT indexCount, bool dynamic)
{ 
    if (m_index_buffer)
    {
        delete m_index_buffer;
    }
    m_index_buffer = new IndexBuffer(type,indexCount,dynamic);
    m_isDirty=true;
    return m_index_buffer;
}

VertexBuffer *VertexData::createVertexBuffer(UINT vertexSize, UINT vertexCount, bool dynamic)
{
if(m_vertex_buffer)
{
    delete m_vertex_buffer;
}
m_vertex_buffer = new VertexBuffer(vertexSize,vertexCount,dynamic);
m_isDirty=true;
return m_vertex_buffer;
}


VertexDeclaration *VertexData::getVertexDeclation()
{
    m_vertex_array->Bind();
    return m_vertex_declaration;
} 

void VertexData::Render(UINT mode,UINT count)
{
    m_vertex_array->Bind();
    if (m_isDirty)
    {
        Update();
    }
    if (m_index_buffer)
    {
        m_index_buffer->Bind();
        
        E_INDEX_TYPE type=m_index_buffer->getType();
        if (type==E_INDEX_TYPE::I16BIT)
        {
            glDrawElements(mode, count, GL_UNSIGNED_SHORT, 0);  
        } else
            if (type==E_INDEX_TYPE::I32BIT)
        {
                glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
        
        }
                
    } else
    {
        if(m_vertex_buffer)
        {
            glDrawArrays(mode, 0, count);
        }
    }
}

void VertexData::Update()
{
    if (!m_vertex_buffer || m_vertex_declaration->getSize()<=0)
        return ;
        m_vertex_array->Bind();
        m_vertex_buffer->Bind();
        const std::vector<VertexDeclaration::Element> elements=  m_vertex_declaration->getElements();

        UINT m_iVertexOffSetSize = m_vertex_declaration->getSize();
        UINT m_offSet=0;

    for (int i=0; i < (int)elements.size(); i++)
    {
        void* pointer = reinterpret_cast<void*>(m_offSet);

        VertexDeclaration::Element elemnt = elements[i];
       
      glVertexAttribPointer(elemnt.index, 
        
        getTypeCount(elemnt.type), 
        getTypeFormat(elemnt.type),   GL_FALSE,  
        m_iVertexOffSetSize, 
        pointer);
    
        glEnableVertexAttribArray(elemnt.index); 

        m_offSet += getTypeSize(elemnt.type);
        
    }


//  	glVertexPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const Vertex*>(vertices))[0].Pos);


    m_vertex_array->unBind();
    m_isDirty=false;
}


//**************************************************************************************************************
//
//                                                 RENDER
//
//*************************************************************************************************************


//**************************************************************************************************************
//
//                                                 MESH
//
//*************************************************************************************************************

void Surface::addElement(UINT index, VertexAttribute attribute, VertexAttributeType  type)
{
VertexDeclaration *decl = vertexData->getVertexDeclation();
decl->addElement(index, attribute  , type);
}
void Surface::setElements()
{
addElement(0, VF_POSITION  , VET_FLOAT3);
addElement(1, VF_TEXCOORD1 , VET_FLOAT2);
addElement(2, VF_NORMAL    , VET_FLOAT3);
addElement(3, VF_TANGENT   , VET_FLOAT3);


}
        
    
void Surface::render()
{
    vertexData->Render(GL_TRIANGLES,  indices.size());
}
void Surface::build()
{
VertexDeclaration *decl = vertexData->getVertexDeclation();
VertexBuffer * vertex  =vertexData->createVertexBuffer(  decl->getSize(), vertices.size());
IndexBuffer * indexes = vertexData->createIndexBuffer(I32BIT, indices.size() );
vertex->createBuffer(vertices.data());
indexes->createBuffer(indices.data()); 

} 

int Surface::addFace(int a,int b,int c)
{
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);

    return (int)indices.size()-1;
}
int Surface::AddTriangle(int a,int b,int c)
{
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
    return (int)indices.size()-1;
}

void Surface::ComputeTangents()
{
     for (int j =0 ; j < (int)vertices.size()/3 ;j++)
        {
            
            Vertex v0 = vertices[(j *3 + 0)];
            Vertex v1 = vertices[(j *3 + 1)];
            Vertex v2 = vertices[(j *3 + 2)];

                Vec3 tangent;
                Vec3 binormal;
                
               calculateTangents(tangent,binormal,
            	v0.pos,
                v1.pos,
                v2.pos,
                v0.uv,
                v1.uv,
                v2.uv);

  
                
                vertices[(j *3 + 0)].tangent.set(tangent);
                vertices[(j *3 + 1)].tangent.set(tangent);
                vertices[(j *3 + 2)].tangent.set(tangent);
                
                
        }
        
}
   
   
void Surface::UpdateNormals() 
{
// Limpar quaisquer normais existentes
for (int i = 0; i < (int)vertices.size(); i++) 
{
    vertices[i].normal.x = 0.0f;
    vertices[i].normal.y = 0.0f;
    vertices[i].normal.z = 0.0f;
}

// Calcular normais para cada face do objeto
for (int i = 0; i <(int) indices.size(); i += 3) 
{
    // Obter os índices dos três vértices que formam esta face
    int i1 = indices[i];
    int i2 = indices[i+1];
    int i3 = indices[i+2];
    
    // Obter as coordenadas dos vértices
    float x1 = vertices[i1].pos.x;
    float y1 = vertices[i1].pos.y;
    float z1 = vertices[i1].pos.z;
    float x2 = vertices[i2].pos.x;
    float y2 = vertices[i2].pos.y;
    float z2 = vertices[i2].pos.z;
    float x3 = vertices[i3].pos.x;
    float y3 = vertices[i3].pos.y;
    float z3 = vertices[i3].pos.z;
    
    // Calcular dois vetores que apontam para fora do plano da face
    float v1x = x2 - x1;
    float v1y = y2 - y1;
    float v1z = z2 - z1;
    float v2x = x3 - x1;
    float v2y = y3 - y1;
    float v2z = z3 - z1;
    float nx = v1y * v2z - v1z * v2y;
    float ny = v1z * v2x - v1x * v2z;
    float nz = v1x * v2y - v1y * v2x;
    
    // Adicionar a normal calculada para os vértices que formam esta face
    vertices[i1].normal.x += nx;
    vertices[i1].normal.y += ny;
    vertices[i1].normal.z += nz;
    vertices[i2].normal.x += nx;
    vertices[i2].normal.y += ny;
    vertices[i2].normal.z += nz;
    vertices[i3].normal.x += nx;
    vertices[i3].normal.y += ny;
    vertices[i3].normal.z += nz;
}

// Normalizar os vetores normais dos vértices
for (int i = 0; i < (int)vertices.size(); i++) 
{
    float length = sqrt(
    vertices[i].normal.x * vertices[i].normal.x + 
    vertices[i].normal.y * vertices[i].normal.y +
    vertices[i].normal.z * vertices[i].normal.z);
    vertices[i].normal.x /= length;
    vertices[i].normal.y /= length;
    vertices[i].normal.z /= length;
    
}
}





    Surface *Mesh::CreateSurface(const std::string &name,int material)
    {
      Surface *surf = new Surface(name,material);
      surfaces.push_back(surf);
      return surf;
    } 
    void Mesh::Clear()
    {
           for (int i = 0; i <(int) surfaces.size(); i++) 
            {
                delete surfaces[i];
            }
            surfaces.clear();
            for (int i = 0; i <(int) materials.size(); i++) 
            {
                delete materials[i];
            }
            materials.clear();
    }
      void Mesh::Build()
    {
          for (int i = 0; i <(int) surfaces.size(); i++) 
        {
            Surface *surf = surfaces[i];
            surf->build();
        }
    }
     void Mesh::UpdateNormals()
    {
          for (int i = 0; i <(int) surfaces.size(); i++) 
        {
            Surface *surf = surfaces[i];
            surf->UpdateNormals();
        }
    }
         void Mesh::ComputeTangents()
    {
          for (int i = 0; i <(int) surfaces.size(); i++) 
        {
            Surface *surf = surfaces[i];
            surf->ComputeTangents();
        }
    }
    void Mesh::Render(const Shader &shader)
      {
          for (int i = 0; i <(int) surfaces.size(); i++) 
          {
             
              Surface *surf = surfaces[i];
              if (surf->getMaterialIndex()>0 && surf->getMaterialIndex()<=(int)materials.size())
              {
              //  std::cout << surf->getMaterialIndex() << "  " <<(int)materials.size()  << std::endl;
                Material * mat = materials[surf->getMaterialIndex()];
                shader.setVector3("diffuseColor",  mat->difusse);
                shader.setVector3("specularColor", mat->specular);
                shader.setVector3("ambientColor", mat->ambient);
              }
            // 
              
              surf->render();
          }
      }
      

    int   Mesh::getMaterialIndex(int id)
    {
        return surfaces[id]->getMaterialIndex();
    }
    Material   *Mesh::getMaterial(int id)
    {
        return materials[id];
    }
    std::string   Mesh::getName(int id) const
    {
        return surfaces[id]->getName();
    }
    void Mesh::setMaterialIndex(int id,int mat)
    {
        surfaces[id]->setMaterialIndex(mat);
    }
    void Mesh::setName(int id,const std::string &name)
    {
        surfaces[id]->setName(name);
    }

Mesh *Mesh::CreatePlane(float w, float d) 
    {

      Mesh* mesh=new Mesh();

      Surface* surf=mesh->CreateSurface();
 
      surf->setElements();

      surf->AddVertex(-w/2.0f,0.0,-d/2.0f);
      surf->AddVertex(-w/2.0f,0.0, d/2.0f);
      surf->AddVertex( w/2.0f,0.0, d/2.0f);
      surf->AddVertex( w/2.0f,0.0,-d/2.0f);


      surf->VertexNormal(0,0.0,1.0,0.0);
      surf->VertexNormal(1,0.0,1.0,0.0);
      surf->VertexNormal(2,0.0,1.0,0.0);
      surf->VertexNormal(3,0.0,1.0,0.0);

      surf->VertexTexCoords(0,0.0,1.0);
      surf->VertexTexCoords(1,0.0,0.0);
      surf->VertexTexCoords(2,1.0,0.0);
      surf->VertexTexCoords(3,1.0,1.0);
      surf->AddTriangle(0,1,2); // front
      surf->AddTriangle(0,2,3);
      
      mesh->Build();

      return mesh;

    }

Mesh* Mesh::CreateCone(	float height,int segments,bool solid)
{

	int top=0,br=0,bl=0; // side of cone
	int bs0=0,bs1=0,newbs=0; // bottom side vertices

	if(segments<3 || segments>100) return NULL;

  Mesh* mesh=new Mesh();
  Surface* thissurf=mesh->CreateSurface();
  thissurf->setElements();

  
	Surface* thissidesurf=NULL;
	if(solid)
  {
		thissidesurf=mesh->CreateSurface();
        thissidesurf->setElements();
	}
	float div=float(360.0f/segments);


	float upos=1.0f;
	float udiv=float(1.0f/(segments));
	float RotAngle=90.0f;

	// first side
	float XPos=-cosdeg(RotAngle);
	float ZPos=sindeg(RotAngle);

	top=thissurf->AddVertex(0.0f,height,0.0f,upos-(udiv/2.0f),0.0f);
	br=thissurf->AddVertex(XPos,-height,ZPos,upos,1.f);

	// 2nd tex coord set
	thissurf->VertexTexCoords(top,upos-(udiv/2.0f),0.0f);
	thissurf->VertexTexCoords(br,upos,1.0f);

	if(solid==true) bs0=thissidesurf->AddVertex(XPos,-height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
	if(solid==true) thissidesurf->VertexTexCoords(bs0,XPos/2.0f+0.5f,ZPos/2.0f+0.5f); // 2nd tex coord set

	RotAngle=RotAngle+div;

	XPos=-cosdeg(RotAngle);
	ZPos=sindeg(RotAngle);

	bl=thissurf->AddVertex(XPos,-height,ZPos,upos-udiv,1.0f);
	thissurf->VertexTexCoords(bl,upos-udiv,1.0f); // 2nd tex coord set

	if(solid==true) bs1=thissidesurf->AddVertex(XPos,-height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
	if(solid==true) thissidesurf->VertexTexCoords(bs1,XPos/2.0f+0.5f,ZPos/2.0f+0.5f); // 2nd tex coord set

	thissurf->AddTriangle(bl,top,br);

	// rest of sides
	for(int i=1;i<=(segments-1);i++)
  {
		br=bl;
		upos=upos-udiv;
		top=thissurf->AddVertex(0.0,height,0.0f,upos-(udiv/2.0f),0.0f);
		thissurf->VertexTexCoords(top,upos-(udiv/2.0f),0.0f); // 2nd tex coord set

		RotAngle=RotAngle+div;

		XPos=-cosdeg(RotAngle);
		ZPos=sindeg(RotAngle);

		bl=thissurf->AddVertex(XPos,-height,ZPos,upos-udiv,1.0f);
		thissurf->VertexTexCoords(bl,upos-udiv,1.0f); // 2nd tex coord set

		if(solid==true) newbs=thissidesurf->AddVertex(XPos,-height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
		if(solid==true) thissidesurf->VertexTexCoords(newbs,XPos/2.0f+0.5f,ZPos/2.0f+0.5f); // 2nd tex coord set

		thissurf->AddTriangle(bl,top,br);

		if(solid)
    {
			thissidesurf->AddTriangle(newbs,bs1,bs0);
      thissidesurf->UpdateNormals();
      thissidesurf->build();
      

			if(i<(segments-1))
      {
				bs1=newbs;
			}
		}
	}

  mesh->UpdateNormals();
	mesh->Build();
	return mesh;

}

Mesh* Mesh::CreateCylinder(float height, int verticalsegments,bool solid)
{

	int ringsegments=0; // default?

	int tr=0,tl=0,br=0,bl=0;// 		side of cylinder
	int ts0=0,ts1=0,newts=0;// 	top side vertexs
	int bs0=0,bs1=0,newbs=0;// 	bottom side vertexs
	if(verticalsegments<3 || verticalsegments>100) return NULL;
	if(ringsegments<0 || ringsegments>100) return NULL;

      Mesh* mesh=new Mesh();
      Surface* thissurf=mesh->CreateSurface();
      thissurf->setElements();
	    Surface* thissidesurf=NULL;
	if(solid==true)
  {
		thissidesurf=mesh->CreateSurface();
         thissidesurf->setElements();
	}
	float div=float(360.0f/(verticalsegments));


	float ringSegmentHeight=(height*2.0)/(ringsegments+1);
	float upos=1.0f;
	float udiv=float(1.0f/(verticalsegments));
	float vdiv=float(1.0f/(ringsegments+1));

	float SideRotAngle=90.0f;

	// re-diminsion arrays to hold needed memory.
	// this is used just for helping to build the ring segments...

	int* tRing=new int[verticalsegments+1];
	int* bRing=new int[verticalsegments+1];

	// render end caps if solid
	if(solid==true)
  {

		float XPos=-cosdeg(SideRotAngle);
		float ZPos=sindeg(SideRotAngle);

		ts0=thissidesurf->AddVertex(XPos,height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
		bs0=thissidesurf->AddVertex(XPos,-height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);

		// 2nd tex coord set
		thissidesurf->VertexTexCoords(ts0,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
		thissidesurf->VertexTexCoords(bs0,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);

		SideRotAngle=SideRotAngle+div;

		XPos=-cosdeg(SideRotAngle);
		ZPos=sindeg(SideRotAngle);

		ts1=thissidesurf->AddVertex(XPos,height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
		bs1=thissidesurf->AddVertex(XPos,-height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);

		// 2nd tex coord set
		thissidesurf->VertexTexCoords(ts1,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
		thissidesurf->VertexTexCoords(bs1,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);

		for(int i=1;i<=verticalsegments-2;i++)
    {
			SideRotAngle=SideRotAngle+div;

			XPos=-cosdeg(SideRotAngle);
			ZPos=sindeg(SideRotAngle);

			newts=thissidesurf->AddVertex(XPos,height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
			newbs=thissidesurf->AddVertex(XPos,-height,ZPos,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);

			// 2nd tex coord set
			thissidesurf->VertexTexCoords(newts,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);
			thissidesurf->VertexTexCoords(newbs,XPos/2.0f+0.5f,ZPos/2.0f+0.5f);

			thissidesurf->AddTriangle(ts0,ts1,newts);
			thissidesurf->AddTriangle(newbs,bs1,bs0);

			if(i<(verticalsegments-2))
      {
				ts1=newts;
				bs1=newbs;
			}

		}
	}

	// -----------------------
	// middle part of cylinder
	float thisHeight=height;

	// top ring first
	SideRotAngle=90.0f;
	float XPos=-cosdeg(SideRotAngle);
	float ZPos=sindeg(SideRotAngle);
	float thisUPos=upos;
	float thisVPos=0.0f;
	tRing[0]=thissurf->AddVertex(XPos,thisHeight,ZPos,thisUPos,thisVPos);
	thissurf->VertexTexCoords(tRing[0],thisUPos,thisVPos); // 2nd tex coord set
	for(int i=0;i<=verticalsegments-1;i++)
  {
		SideRotAngle=SideRotAngle+div;
		XPos=-cosdeg(SideRotAngle);
		ZPos=sindeg(SideRotAngle);
		thisUPos=thisUPos-udiv;
		tRing[i+1]=thissurf->AddVertex(XPos,thisHeight,ZPos,thisUPos,thisVPos);
		thissurf->VertexTexCoords(tRing[i+1],thisUPos,thisVPos); // 2nd tex coord set
	}

	for(int ring=0;ring<=ringsegments;ring++)
  {

		// decrement vertical segment
		thisHeight=thisHeight-ringSegmentHeight;

		// now bottom ring
		SideRotAngle=90;
		XPos=-cosdeg(SideRotAngle);
		ZPos=sindeg(SideRotAngle);
		thisUPos=upos;
		thisVPos=thisVPos+vdiv;
		bRing[0]=thissurf->AddVertex(XPos,thisHeight,ZPos,thisUPos,thisVPos);
		thissurf->VertexTexCoords(bRing[0],thisUPos,thisVPos); // 2nd tex coord set
		for(int i=0;i<=verticalsegments-1;i++)
    {
			SideRotAngle=SideRotAngle+div;
			XPos=-cosdeg(SideRotAngle);
			ZPos=sindeg(SideRotAngle);
			thisUPos=thisUPos-udiv;
			bRing[i+1]=thissurf->AddVertex(XPos,thisHeight,ZPos,thisUPos,thisVPos);
			thissurf->VertexTexCoords(bRing[i+1],thisUPos,thisVPos); // 2nd tex coord set
		}

		// Fill in ring segment sides with triangles
		for(int v=1;v<=verticalsegments;v++)
    {
			tl=tRing[v];
			tr=tRing[v-1];
			bl=bRing[v];
			br=bRing[v-1];

			thissurf->AddTriangle(tl,tr,br);
			thissurf->AddTriangle(bl,tl,br);
		}

		// make bottom ring segment the top ring segment for the next loop.
		for(int v=0;v<=verticalsegments;v++)
    {
			tRing[v]=bRing[v];
		}
	}

	delete [] tRing;
	delete [] bRing;

	
  mesh->UpdateNormals();
	mesh->Build();
	
  
	return mesh;

}
Mesh* Mesh::CreateCube(float w, float h, float d)
  {

   Mesh* mesh=new Mesh();

    Surface* surf=mesh->CreateSurface();
    surf->setElements();

  
float x = (w / 2.0f) +0.5f;
float y = (h / 2.0f) +0.5f;
float z = (d / 2.0f) +0.5f;
surf->AddVertex(-x,-y,-z);
surf->AddVertex(-x, y,-z);
surf->AddVertex( x, y,-z);
surf->AddVertex( x,-y,-z);

surf->AddVertex(-x,-y, z);
surf->AddVertex(-x, y, z);
surf->AddVertex( x, y, z);
surf->AddVertex( x,-y, z);

surf->AddVertex(-x,-y, z);
surf->AddVertex(-x, y, z);
surf->AddVertex( x, y, z);
surf->AddVertex( x,-y, z);

surf->AddVertex(-x,-y,-z);
surf->AddVertex(-x, y,-z);
surf->AddVertex( x, y,-z);
surf->AddVertex( x,-y,-z);

surf->AddVertex(-x,-y, z);
surf->AddVertex(-x, y, z);
surf->AddVertex( x, y, z);
surf->AddVertex( x,-y, z);

surf->AddVertex(-x,-y,-z);
surf->AddVertex(-x, y,-z);
surf->AddVertex( x, y,-z);
surf->AddVertex( x,-y,-z);




	surf->VertexNormal(0,0.0,0.0,-1.0);
	surf->VertexNormal(1,0.0,0.0,-1.0);
	surf->VertexNormal(2,0.0,0.0,-1.0);
	surf->VertexNormal(3,0.0,0.0,-1.0);

	surf->VertexNormal(4,0.0,0.0,1.0);
	surf->VertexNormal(5,0.0,0.0,1.0);
	surf->VertexNormal(6,0.0,0.0,1.0);
	surf->VertexNormal(7,0.0,0.0,1.0);

	surf->VertexNormal(8,0.0,-1.0,0.0);
	surf->VertexNormal(9,0.0,1.0,0.0);
	surf->VertexNormal(10,0.0,1.0,0.0);
	surf->VertexNormal(11,0.0,-1.0,0.0);

	surf->VertexNormal(12,0.0,-1.0,0.0);
	surf->VertexNormal(13,0.0,1.0,0.0);
	surf->VertexNormal(14,0.0,1.0,0.0);
	surf->VertexNormal(15,0.0,-1.0,0.0);

	surf->VertexNormal(16,-1.0,0.0,0.0);
	surf->VertexNormal(17,-1.0,0.0,0.0);
	surf->VertexNormal(18,1.0,0.0,0.0);
	surf->VertexNormal(19,1.0,0.0,0.0);

	surf->VertexNormal(20,-1.0,0.0,0.0);
	surf->VertexNormal(21,-1.0,0.0,0.0);
	surf->VertexNormal(22,1.0,0.0,0.0);
	surf->VertexNormal(23,1.0,0.0,0.0);

	surf->VertexTexCoords(0,0.0,1.0);
	surf->VertexTexCoords(1,0.0,0.0);
	surf->VertexTexCoords(2,1.0,0.0);
	surf->VertexTexCoords(3,1.0,1.0);

	surf->VertexTexCoords(4,1.0,1.0);
	surf->VertexTexCoords(5,1.0,0.0);
	surf->VertexTexCoords(6,0.0,0.0);
	surf->VertexTexCoords(7,0.0,1.0);

	surf->VertexTexCoords(8,0.0,1.0);
	surf->VertexTexCoords(9,0.0,0.0);
	surf->VertexTexCoords(10,1.0,0.0);
	surf->VertexTexCoords(11,1.0,1.0);

	surf->VertexTexCoords(12,0.0,0.0);
	surf->VertexTexCoords(13,0.0,1.0);
	surf->VertexTexCoords(14,1.0,1.0);
	surf->VertexTexCoords(15,1.0,0.0);

	surf->VertexTexCoords(16,0.0,1.0);
	surf->VertexTexCoords(17,0.0,0.0);
	surf->VertexTexCoords(18,1.0,0.0);
	surf->VertexTexCoords(19,1.0,1.0);

	surf->VertexTexCoords(20,1.0,1.0);
	surf->VertexTexCoords(21,1.0,0.0);
	surf->VertexTexCoords(22,0.0,0.0);
	surf->VertexTexCoords(23,0.0,1.0);

	surf->VertexTexCoords(0,0.0,1.0);
	surf->VertexTexCoords(1,0.0,0.0);
	surf->VertexTexCoords(2,1.0,0.0);
	surf->VertexTexCoords(3,1.0,1.0);

	surf->VertexTexCoords(4,1.0,1.0);
	surf->VertexTexCoords(5,1.0,0.0);
	surf->VertexTexCoords(6,0.0,0.0);
	surf->VertexTexCoords(7,0.0,1.0);

	surf->VertexTexCoords(8,0.0,1.0);
	surf->VertexTexCoords(9,0.0,0.0);
	surf->VertexTexCoords(10,1.0,0.0);
	surf->VertexTexCoords(11,1.0,1.0);

	surf->VertexTexCoords(12,0.0,0.0);
	surf->VertexTexCoords(13,0.0,1.0);
	surf->VertexTexCoords(14,1.0,1.0);
	surf->VertexTexCoords(15,1.0,0.0);

	surf->VertexTexCoords(16,0.0,1.0);
	surf->VertexTexCoords(17,0.0,0.0);
	surf->VertexTexCoords(18,1.0,0.0);
	surf->VertexTexCoords(19,1.0,1.0);

	surf->VertexTexCoords(20,1.0,1.0);
	surf->VertexTexCoords(21,1.0,0.0);
	surf->VertexTexCoords(22,0.0,0.0);
	surf->VertexTexCoords(23,0.0,1.0);



	surf->AddTriangle(0,1,2); // front
	surf->AddTriangle(0,2,3);
	surf->AddTriangle(6,5,4); // back
	surf->AddTriangle(7,6,4);
	surf->AddTriangle(6+8,5+8,1+8); // top
	surf->AddTriangle(2+8,6+8,1+8);
	surf->AddTriangle(0+8,4+8,7+8); // bottom
	surf->AddTriangle(0+8,7+8,3+8);
	surf->AddTriangle(6+16,2+16,3+16); // right
	surf->AddTriangle(7+16,6+16,3+16);
	surf->AddTriangle(0+16,1+16,5+16); // left
	surf->AddTriangle(0+16,5+16,4+16);

    surf->build();

	return mesh;

}


Mesh *Mesh::CreateSphere(int segments)
{

	if(segments<3 || segments>100) return NULL;

    Mesh* mesh=new Mesh();
    
    Surface* thissurf=mesh->CreateSurface();
 
    
    thissurf->setElements();


	float div=360.0/(segments*2);
	float height=1.0;
	float upos=1.0;
	float udiv=1.0/(segments*2);
	float vdiv=1.0/segments;
	float RotAngle=90;

	if(segments<3)
  { // diamond shape - no center strips

		for(int i=1;i<=segments*2;i++)
    {
			int np=thissurf->AddVertex(0.0f,height,0.0f,upos-(udiv/2.0f), 0.0f);//northpole
			int sp=thissurf->AddVertex(0.0f,-height,0.0f,upos-(udiv/2.0f),1.0f);//southpole
			float XPos=-cosdeg(RotAngle);
			float ZPos=sindeg(RotAngle);
			int v0=thissurf->AddVertex(XPos,0.0f,ZPos,upos,0.5f);
			RotAngle=RotAngle+div;
			if(RotAngle>=360.0f) RotAngle=RotAngle-360.0f;
			XPos=-cosdeg(RotAngle);
			ZPos=sindeg(RotAngle);
			upos=upos-udiv;
			int v1=thissurf->AddVertex(XPos,0.0f,ZPos,upos,0.5f);
			thissurf->AddTriangle(np,v0,v1);
			thissurf->AddTriangle(v1,v0,sp);
		}

	}

	if(segments>2){

		// poles first
		for(int i=1;i<=segments*2;i++)
    {

			int np=thissurf->AddVertex(0.0f,height,0.0f,upos-(udiv/2.0f),0.0f);//northpole
			int sp=thissurf->AddVertex(0.0f,-height,0.0f,upos-(udiv/2.0f),1.0f);//southpole

			float YPos=cosdeg(div);

			float XPos=-cosdeg(RotAngle)*(sindeg(div));
			float ZPos=sindeg(RotAngle)*(sindeg(div));

			int v0t=thissurf->AddVertex(XPos,YPos,ZPos,upos,vdiv);
			int v0b=thissurf->AddVertex(XPos,-YPos,ZPos,upos,1.0f-vdiv);

			RotAngle=RotAngle+div;

			XPos=-cosdeg(RotAngle)*(sindeg(div));
			ZPos=sindeg(RotAngle)*(sindeg(div));

			upos=upos-udiv;

			int v1t=thissurf->AddVertex(XPos,YPos,ZPos,upos,vdiv);
			int v1b=thissurf->AddVertex(XPos,-YPos,ZPos,upos,1.0f-vdiv);

			thissurf->AddTriangle(np,v0t,v1t);
			thissurf->AddTriangle(v1b,v0b,sp);

		}

		// then center strips

		upos=1.0f;
		RotAngle=90;
		for(int i=1;i<=segments*2;i++)
    {

			float mult=1;
			float YPos=cosdeg(div*(mult));
			float YPos2=cosdeg(div*(mult+1.0f));
			float Thisvdiv=vdiv;

			for(int j=1;j<=segments-2;j++){

				float XPos=-cosdeg(RotAngle)*(sindeg(div*(mult)));
				float ZPos=sindeg(RotAngle)*(sindeg(div*(mult)));

				float XPos2=-cosdeg(RotAngle)*(sindeg(div*(mult+1.0f)));
				float ZPos2=sindeg(RotAngle)*(sindeg(div*(mult+1.0f)));

				int v0t=thissurf->AddVertex(XPos,YPos,ZPos,upos,Thisvdiv);
				int v0b=thissurf->AddVertex(XPos2,YPos2,ZPos2,upos,Thisvdiv+vdiv);

				// 2nd tex coord set
				thissurf->VertexTexCoords(v0t,upos,Thisvdiv);
				thissurf->VertexTexCoords(v0b,upos,Thisvdiv+vdiv);

				float tempRotAngle=RotAngle+div;

				XPos=-cosdeg(tempRotAngle)*(sindeg(div*(mult)));
				ZPos=sindeg(tempRotAngle)*(sindeg(div*(mult)));

				XPos2=-cosdeg(tempRotAngle)*(sindeg(div*(mult+1.0f)));
				ZPos2=sindeg(tempRotAngle)*(sindeg(div*(mult+1.0f)));

				float temp_upos=upos-udiv;

				int v1t=thissurf->AddVertex(XPos,YPos,ZPos,temp_upos,Thisvdiv);
				int v1b=thissurf->AddVertex(XPos2,YPos2,ZPos2,temp_upos,Thisvdiv+vdiv);

				// 2nd tex coord set
				thissurf->VertexTexCoords(v1t,temp_upos,Thisvdiv);
				thissurf->VertexTexCoords(v1b,temp_upos,Thisvdiv+vdiv);

				thissurf->AddTriangle(v1t,v0t,v0b);
				thissurf->AddTriangle(v1b,v1t,v0b);

				Thisvdiv=Thisvdiv+vdiv;
				mult=mult+1;

				YPos=cosdeg(div*(mult));
				YPos2=cosdeg(div*(mult+1.0f));

			}

			upos=upos-udiv;
			RotAngle=RotAngle+div;

		}

	}
	thissurf->UpdateNormals();
     thissurf->build();
	return mesh;

}



 Mesh *Mesh::CreateTorus(float min_rad,float max_rad)
{
      Mesh* mesh=new Mesh();
      Surface* surf=mesh->CreateSurface();
   
  
      surf->setElements();

      float major_radius = max_rad;
      float minor_radius = min_rad;
      int major_segments = 32;
      int minor_segments = 16;


// Cria os vértices do torus
for (int i = 0; i <= major_segments; i++) 
{
  float theta = i * (2.0f * PI / major_segments);
  float sin_theta = sinf(theta);
  float cos_theta = cosf(theta);

  for (int j = 0; j <= minor_segments; j++) 
  {
    float phi = j * (2.0f * PI / minor_segments);
    float sin_phi = sinf(phi);
    float cos_phi = cosf(phi);

    // Posição do vértice
    Vec3 position(
      (major_radius + minor_radius * cos_phi) * cos_theta,
      (major_radius + minor_radius * cos_phi) * sin_theta,
      minor_radius * sin_phi
    );

    // Normal do vértice
    Vec3 normal(
      cos_theta * cos_phi,
      sin_theta * cos_phi,
      sin_phi
    );

    // Coordenada de textura do vértice
    Vec2 texture_coord(
      i / (float) major_segments,
      j / (float) minor_segments
    );

    // Adiciona o vértice ao Mesh
    surf->AddVertex(position, texture_coord, normal);
  }
}

// Cria as faces do torus
for (int i = 0; i < major_segments; i++)
 {
  for (int j = 0; j < minor_segments; j++)
   {
    // Indices dos vértices da face
    int a = (minor_segments + 1) * i + j;
    int b = (minor_segments + 1) * i + j + 1;
    int c = (minor_segments + 1) * (i + 1) + j + 1;
    int d = (minor_segments + 1) * (i + 1) + j;

    // Adiciona as faces ao Mesh
    surf->addFace(a, b, c);
    surf->addFace(a, c, d);
  }
 }


      surf->build();
      return mesh;
}


 Mesh *Mesh::CreateHills(const Vec2 &tileSize, int NumXTiles,int NumYTiles, float hillHeight,const Vec2 &textureRepeatCount, const Vec2 &countHills)
{
    Mesh* mesh=new Mesh();
    Surface* surf=mesh->CreateSurface();
 
  
    surf->setElements();

    	// center
	const Vec2 center((tileSize.x * NumXTiles) * 0.5f, (tileSize.y * NumYTiles) * 0.5f);

	// texture coord step
	const Vec2 tx(
			textureRepeatCount.x  / NumXTiles,
			textureRepeatCount.y  / NumYTiles);

      // add one more point in each direction for proper tile count
	++NumXTiles;
	++NumYTiles;


  // create vertices from left-front to right-back
	int x;

 	float sx=0.f, tsx=0.f;
	for (x=0; x<NumXTiles; ++x)
	{
		float sy=0.f, tsy=0.f;
		for (int y=0; y<NumYTiles; ++y)
		{
			float p_x =   sx - center.x;
      float p_z  =  sy - center.y;
			float uv_x=tsx;
      float uv_y=1.0f - tsy;
      float p_y=0.0f;
 

      if (isnotzero(hillHeight))
      {
        p_y =  (sinf(p_x * countHills.x * PI / center.x) * 	cosf(p_z * countHills.y * PI / center.y)) * 	hillHeight;
      }
	
		

			surf->AddVertex(p_x, p_y, p_z, uv_x,uv_y);
      
			sy += tileSize.y;
			tsy += tx.y;
		}
		sx += tileSize.x;
		tsx += tx.x;
	}

  for (x=0; x<NumXTiles-1; ++x)
	{
		for (int y=0; y<NumYTiles-1; ++y)
		{
			const int current = x * NumYTiles + y;

      surf->addFace(current,
                   current + 1, 
                   current + NumYTiles);

      surf->addFace(current + 1,
                   current + 1 + NumYTiles, 
                   current + NumYTiles);




		}
	}

  surf->UpdateNormals();
  surf->build();
      
    return mesh;
}
  
 Mesh* Mesh::LoadMesh(const std::string &filename,bool invertZ)
{

    if (!FileExists(filename.c_str()))
        return nullptr;

  FileStream file(filename.c_str());
  

	std::string header = file.readString();
	int id             = file.readInt();
  int numMaterials   = file.readInt();
  Log(0," Load mesh %s id:%d   materials:%d \n",header.c_str() , id,numMaterials);
  Mesh * mesh = new Mesh();

  for (int i=0; i<numMaterials;i++)
  {
    	int flags                = file.readInt();
        Material *mat = new Material();

        float r,g,b;
      //ambient
        r = file.readFloat();
        g = file.readFloat();
        b = file.readFloat();
        mat->ambient.set(r,g,b);
      //Difusse
        r = file.readFloat();
        g = file.readFloat();
        b = file.readFloat();
        mat->difusse.set(r,g,b);
      //Specular
        r = file.readFloat();
        g = file.readFloat();
        b = file.readFloat();
        mat->specular.set(r,g,b);

        //Emissive
        r = file.readFloat();
        g = file.readFloat();
        b = file.readFloat();
        mat->emisive.set(r,g,b);
      
         int use_diffuse          = file.readInt();
         int use_lightmap            = file.readInt();

      std::string diffuse;
      std::string lightmap;

      


      if (use_diffuse>=1)
      {
         diffuse  = file.readString();
      }
      
      if (use_lightmap>=1)
      {
         lightmap  = file.readString();
      }
       Log(0," Material %d  %d %d \n",i, use_diffuse,use_lightmap);
       mesh->AddMaterial(mat);
  }
  
 int num_surfaces=file.readInt();//numero of meshs
 Log(0," Surfaces %d \n",num_surfaces);
 for (int i=0;i < num_surfaces;i++)
 {
    std::string mesh_name    = file.readString();
    int flags                = file.readInt();
    int materialIndex        = file.readInt();
    int numVertices          = file.readInt();
    int numFaces             = file.readInt();
    int numUVChannels        = file.readInt();

    Surface * surf=mesh->CreateSurface(mesh_name,materialIndex);
    surf->setElements();
    

 
    Log(0," Name (%s) MateriaIndex: %d numVertex:%d numFaces:%d numUvMaps:%d \n",mesh_name.c_str(),materialIndex,numVertices,numFaces,numUVChannels);

     for (int x=0; x< numVertices ;x++)
		 {
            Vec3 pos;
            Vec3 normal;
            Vec2 uv;
          if (invertZ)
          {
            pos.x=file.readFloat();//pos
            pos.z=file.readFloat();
            pos.y=file.readFloat();

            normal.x=-file.readFloat();//normal
            normal.y=-file.readFloat();
            normal.z=-file.readFloat();
          }  else
          {
           pos.x=file.readFloat();//pos
           pos.y=file.readFloat();
           pos.z=file.readFloat();
           
           normal.x=file.readFloat();//normal
           normal.y=file.readFloat();
           normal.z=file.readFloat();     
          }


           uv.x=file.readFloat();//texture 0
           uv.y=file.readFloat();
            if(numUVChannels==2)
            {
                file.readFloat();//texture 1
                file.readFloat();
            }
        surf->AddVertex(pos,uv,normal);
		 }
		  for (int x=0;x < numFaces;x++)
		 {
            int a=file.readInt();
            int b=file.readInt();
            int c=file.readInt();
            if (invertZ)
                surf->addFace(a,b,c);
            else
                surf->addFace(c,b,a);

		 }
      
    
  }
  
  int useBones = file.readInt();
  
      
    
  // mesh->UpdateNormals(); 
   mesh->Build();  
   return mesh;
}
void            Mesh::AddMaterial(Material *material)
{
    materials.push_back(material);
}
Mesh* Mesh::ImportOBJ(const std::string &filename)
{
         struct Face
    {
        int a;
        int b;
        int c;
        Vec3 na;
        Vec3 nb;
        Vec3 nc;
        Vec2 a_uv;
        Vec2 b_uv;
        Vec2 c_uv;
    } ;

            if (!FileExists(filename.c_str()))
            {
                Log(2," File %s don't exists",filename.c_str());
                return Mesh::CreateCube(1,1,1);
            }
                std::vector<Vec3> vertices;   
                std::vector<Vec3> normals;   
                std::vector<Vec2> texcoords;      
                std::vector<Face>    faces;    

          
            FILE* file;
            file = fopen(filename.c_str(), "r");
            char line[1024];
            bool hasNormals  =false;
            bool hasTextures =false;
             while (fgets(line, 1024, file)) 
            {
                // Vertex information
                if (strncmp(line, "v ", 2) == 0) 
                {
                    Vec3 vertex ;
                    sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                    vertices.push_back(vertex);
                    
                }
                if (strncmp(line, "vn ", 3) == 0) 
                {
                    Vec3 vertex ;
                    sscanf(line, "vn %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                    normals.push_back(vertex);
                    hasNormals=true;
                } else
                {
                    
                }
                // Texture coordinate information
                if (strncmp(line, "vt ", 3) == 0) 
                {
                    Vec2 texcoord ;
                    sscanf(line, "vt %f %f", &texcoord.x, &texcoord.y);
                    texcoords.push_back(texcoord);
                    hasTextures=true;
                 }
                // Face information
                if (strncmp(line, "f ", 2) == 0)
                {
                    int vertex_indices[3];
                    int texture_indices[3];
                    int normal_indices[3];
                    sscanf(
                        line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                        &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                        &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                        &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                    );
             
                    Face face;
                    face.a = vertex_indices[0];
                    face.b = vertex_indices[1];
                    face.c = vertex_indices[2];
                    face.a_uv = texcoords[texture_indices[0] - 1];
                    face.b_uv = texcoords[texture_indices[1] - 1];
                    face.c_uv = texcoords[texture_indices[2] - 1];
                    face.na = normals[normal_indices[0] - 1];
                    face.nb = normals[normal_indices[1] - 1];
                    face.nc = normals[normal_indices[2] - 1];
                    faces.push_back(face);
                }
            }
            Mesh *mesh = new Mesh();
            Surface *surf = mesh->CreateSurface("obj",0);
            surf->setElements();
            for (int i=0; i < (int)faces.size();i++)
            {
                
                Vec3 v0 = vertices[faces[i].a - 1];
                Vec3 v1 = vertices[faces[i].b - 1];
                Vec3 v2 = vertices[faces[i].c - 1];


                Vec2 uv0 = faces[i].a_uv;
                Vec2 uv1 = faces[i].b_uv;
                Vec2 uv2 = faces[i].c_uv;

                Vec3 n0 = faces[i].na;
                Vec3 n1 = faces[i].nb;
                Vec3 n2 = faces[i].nc;
                
                int f0 = surf->AddVertex(v0,uv0,n0);
                int f1 = surf->AddVertex(v1,uv1,n1);
                int f2 = surf->AddVertex(v2,uv2,n2);

                surf->AddTriangle(f0,f1,f2);
            }
            mesh->ComputeTangents();
            mesh->Build();
            fclose(file);
        
           return mesh;
}
Surface *Mesh::getSurface(int index) 
{
    if (index>=0 && index <=getTotalSurfaces())
        return surfaces[index];
    return nullptr;
}
int Mesh::getTotalSurfaces()
{
    return (int)surfaces.size();
}

//****************************************************************************************
 float skyboxVertices[] = 
 {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
SkyBox::SkyBox()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}
SkyBox::~SkyBox()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}
void SkyBox::Render(TextureCube *texture)
{
        glDisable(GL_CULL_FACE);  
        glDepthFunc(GL_LEQUAL);  
        glBindVertexArray(mVAO);
        texture->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS); 
        glEnable(GL_CULL_FACE);  
        glCullFace(GL_FRONT);

}



    
void ShadowMesh::renderVolume()
{
    const int count= (int)ShadowVolumes.size();
    if (!count)
        return;


    

for (int i=0;i<count;i++)
{
     const int points = ShadowVolumes[i].size();
     glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
     glBufferData(GL_ARRAY_BUFFER, points * sizeof(Vec3), ShadowVolumes[i].data(), GL_STATIC_DRAW);
     
     glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
     glDrawArrays(GL_TRIANGLES, 0, points);
     

}


  

}

void ShadowMesh::createShadowVolume()
{

  SShadowVolume* svp = 0;
	

	// builds the shadow volume and adds it to the shadow volume list.

	if (ShadowVolumes.size() > ShadowVolumesUsed)
	{
		// get the next unused buffer

		svp = &ShadowVolumes[ShadowVolumesUsed];
		svp->clear();
	}
	else
	{
		ShadowVolumes.push_back(SShadowVolume());
		svp = &ShadowVolumes[ShadowVolumes.size()-1];

	}
	svp->reserve(IndexCount*5);
	++ShadowVolumesUsed;

	// We use triangle lists
	Edges.resize(IndexCount*2);
	int numEdges = 0;

	numEdges=createEdgesAndCaps(svp);

	// for all edges add the near->far quads
	Vec3 lightDir1(m_light*Infinity);
	Vec3 lightDir2(m_light*Infinity);
	for (int i=0; i<numEdges; ++i)
	{
		const Vec3 &v1 = Vertices[Edges[2*i+0]];
		const Vec3 &v2 = Vertices[Edges[2*i+1]];
		if ( !isDirectional )
		{
			lightDir1 = (v1 - m_light).normalized()*Infinity;
			lightDir2 = (v2 - m_light).normalized()*Infinity;
		}
		const Vec3 v3(v1+lightDir1);
		const Vec3 v4(v2+lightDir2);

		// Add a quad (two triangles) to the vertex list


		svp->push_back(v1);
		svp->push_back(v2);
		svp->push_back(v3);

		svp->push_back(v2);
		svp->push_back(v4);
		svp->push_back(v3);
	}   

}
int  ShadowMesh::createEdgesAndCaps( SShadowVolume* svp)
{
     int numEdges=0;
	const int faceCount = IndexCount / 3;

	

	// Check every face if it is front or back facing the light.
	Vec3 lightDir0(m_light);
	Vec3 lightDir1(m_light);
	Vec3 lightDir2(m_light);
	for (int i=0; i<faceCount; ++i)
	{
		const Vec3 v0 = Vertices[Indices[3*i+0]];
		const Vec3 v1 = Vertices[Indices[3*i+1]];
		const Vec3 v2 = Vertices[Indices[3*i+2]];

		if ( !isDirectional )
		{
            Vec3 ld= v0 - m_light;
			lightDir0 = ld.normalized();
		}

		FaceData[i]=Triangle3d(v2,v1,v0).isFrontFacing(lightDir0);	// actually the back-facing polygons
    //     FaceData[i]=Triangle3d(v0,v1,v2).isFrontFacing(lightDir0);
        



		if (UseZFailMethod && FaceData[i])
		{

			//if (svp->size() <= 5)
			//	Log(1,"Allocation too small.");

			// add front cap from light-facing faces
			svp->push_back(v2);
			svp->push_back(v1);
			svp->push_back(v0);

			// add back cap
			if ( !isDirectional )
			{
				lightDir1 = (v1-m_light).normalized();
				lightDir2 = (v2-m_light).normalized();
			}
			const Vec3 i0 = v0+lightDir0*Infinity;
			const Vec3 i1 = v1+lightDir1*Infinity;
			const Vec3 i2 = v2+lightDir2*Infinity;

			svp->push_back(i0);
			svp->push_back(i1);
			svp->push_back(i2);

			
		}
	}

	// Create edges
	for (int i=0; i<faceCount; ++i)
	{
		// check all front facing faces
		if (FaceData[i] == true)
		{
			const unsigned short wFace0 = Indices[3*i+0];
			const unsigned short wFace1 = Indices[3*i+1];
			const unsigned short wFace2 = Indices[3*i+2];

				// add edge v0-v1
				 Edges[2*numEdges+0] = wFace0;
				 Edges[2*numEdges+1] = wFace1;
				 ++numEdges;

				// // add edge v1-v2
				 Edges[2*numEdges+0] = wFace1;
				 Edges[2*numEdges+1] = wFace2;
				 ++numEdges;

				 // add edge v2-v0
				 Edges[2*numEdges+0] = wFace2;
				 Edges[2*numEdges+1] = wFace0;
				 ++numEdges;
			// }
			// else
			// {
				const unsigned short adj0 = Adjacency[3*i+0];
				const unsigned short adj1 = Adjacency[3*i+1];
				const unsigned short adj2 = Adjacency[3*i+2];

				// // add edges if face is adjacent to back-facing face
				// // or if no adjacent face was found
				// if (adj0 == i || FaceData[adj0] == false)
				// {
				// 	// add edge v0-v1
				// 	Edges[2*numEdges+0] = wFace0;
				// 	Edges[2*numEdges+1] = wFace1;
				// 	++numEdges;
				// }

				// if (adj1 == i || FaceData[adj1] == false)
				// {
				// 	// add edge v1-v2
				// 	Edges[2*numEdges+0] = wFace1;
				// 	Edges[2*numEdges+1] = wFace2;
				// 	++numEdges;
				// }

				// if (adj2 == i || FaceData[adj2] == false)
				// {
				// 	// add edge v2-v0
				// 	Edges[2*numEdges+0] = wFace2;
				// 	Edges[2*numEdges+1] = wFace0;
				// 	++numEdges;
				// }
			
		}
	}
	return numEdges;
}
void ShadowMesh::calculateAdjacency()
{
	    AdjacencyDirtyFlag = false;

	 	Adjacency.clear();

		Adjacency.reserve(IndexCount);

		// go through all faces and fetch their three neighbours
		for (int f=0; f<IndexCount; f+=3)
		{
			for (int edge = 0; edge<3; ++edge)
			{
				Vec3& v1 = Vertices[Indices[f+edge]];
				Vec3& v2 = Vertices[Indices[f+((edge+1)%3)]];

				// now we search an_O_ther _F_ace with these two
				// vertices, which is not the current face.
				int of;

				for (of=0; of<IndexCount; of+=3)
				{
					// only other faces
					if (of != f)
					{
						bool cnt1 = false;
						bool cnt2 = false;

						for (int e=0; e<3; ++e)
						{
							if (v1.equals(Vertices[Indices[of+e]]))
								cnt1=true;

							if (v2.equals(Vertices[Indices[of+e]]))
								cnt2=true;
						}
						// one match for each vertex, i.e. edge is the same
						if (cnt1 && cnt2)
							break;
					}
				}

				// no adjacent edges -> store face number, else store adjacent face
				if (of >= IndexCount)
					Adjacency[f + edge] = f/3;
				else
					Adjacency[f + edge] = of/3;
			}
		}
}
void ShadowMesh::updateShadowVolumes()
{
    const int oldIndexCount = IndexCount;
	const int oldVertexCount = VertexCount;

	VertexCount = 0;
	IndexCount = 0;
	ShadowVolumesUsed = 0;

	Mesh*  mesh = m_mesh;
	if (!mesh)
		return;

	// create as much shadow volumes as there are lights but
	// do not ignore the max light settings.
	const int lightCount =1;
	if (!lightCount)
		return;

	// calculate total amount of vertices and indices

	int i;
	int totalVertices = 0;
	int totalIndices = 0;
	const int bufcnt = mesh->getTotalSurfaces();

	for (i=0; i<bufcnt; ++i)
	{
		Surface *buf = mesh->getSurface(i);
		totalIndices += buf->getTotalIndices();
		totalVertices += buf->getTotalVertices();
	
	}
	
	// allocate memory if necessary

	Vertices.reserve(totalVertices);
	Indices.reserve(totalIndices);
	FaceData.reserve(totalIndices / 3);

	// copy mesh
	// (could speed this up for static meshes by adding some user flag to prevents copying)
	for (i=0; i<bufcnt; ++i)
	{
		 Surface* buf = mesh->getSurface(i);

		const unsigned int* idxp = buf->getIndices();
		const unsigned int* idxpend = idxp + buf->getTotalVertices();
		for (; idxp!=idxpend; ++idxp)
			Indices[IndexCount++] = *idxp + VertexCount;

		const int vtxcnt = buf->getTotalVertices();
		for (int j=0; j<vtxcnt; ++j)
			Vertices[VertexCount++] = buf->getPosition(j);
	}

	// recalculate adjacency if necessary
	if (oldVertexCount != VertexCount || oldIndexCount != IndexCount || AdjacencyDirtyFlag)
		calculateAdjacency();

	
  
    


    
	

	createShadowVolume();

	
}
void ShadowMesh::render(const Mat4 &m, const Vec3 &light)
{
    m_light.set(light);
    isDirectional=false;

	Vec3 ldir;
    ldir.set(m_light);
    
    if (isDirectional)
    {
       Mat4 transpose = m.transposed();
       transpose.transformVect(ldir);
       
    } else
    {
        Mat4 inverse =  m.inverted();
        inverse.transformVect(ldir);
    }
    
     updateShadowVolumes();

    //  glEnableVertexAttribArray(0);
    //  renderVolume();

//     bool drawShadow = false;
    
GLint cullFaceMode = 0;
glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);
GLint depthFunc = 0;
glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
GLboolean depthMask = 0;
glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

//glEnable(GL_STENCIL_TEST);
glEnableVertexAttribArray(0);
//glEnable(GL_CULL_FACE);  

UseZFailMethod=true;
     renderVolume();       
//     glEnableVertexAttribArray(0);
    
    // if (UseZFailMethod)
	// 	{
	// 		glCullFace(GL_FRONT);
	// 		glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	// 		renderVolume();

	// 		glCullFace(GL_BACK);
	// 		glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
    //         renderVolume();
	// 	}
	// 	else // zpass
	// 	{
	// 		glCullFace(GL_BACK);
	// 		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    //         renderVolume();

	// 		glCullFace(GL_FRONT);
	// 		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
    //         renderVolume();
	// 	}

glDisableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glDisable(GL_STENCIL_TEST);

glEnable(GL_CULL_FACE);  
glCullFace(GL_FRONT);
glDisable(GL_CULL_FACE);  

//glCullFace(cullFaceMode);
glDepthFunc(depthFunc);
glDepthMask(depthMask);
  
}