/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 17:25:56 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/03/03 19:44:40 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "glad.h"
#include "utils.hpp"
#include "math.hpp"
#include "stb_image.h" 


#define FVF_XYZ              1<<0
#define FVF_NORMAL           1<<1
#define FVF_COLOR            1<<2
#define FVF_FCOLOR           1<<3
#define FVF_BINORMAL         1<<4
#define FVF_TANGENT          1<<5

#define FVF_TEX1             1<<6
#define FVF_TEX2             1<<7
#define FVF_TEX3             1<<8
#define FVF_TEX4             1<<9
#define FVF_TEX5             1<<10



enum E_INDEX_TYPE
{
    I16BIT = 0,
    I32BIT = 1
};

enum E_PRIMITIVE_TYPE
{
    //! All vertices are non-connected points.
    EPT_POINTS=0,

    //! All vertices form a single connected line.
    EPT_LINE_STRIP,

    //! Just as LINE_STRIP, but the last and the first vertex is also connected.
    EPT_LINE_LOOP,

    //! Every two vertices are connected creating n/2 lines.
    EPT_LINES,

    //! After the first two vertices each vertex defines a new triangle.
    //! Always the two last and the new one form a new triangle.
    EPT_TRIANGLE_STRIP,

    //! After the first two vertices each vertex defines a new triangle.
    //! All around the common first vertex.
    EPT_TRIANGLE_FAN,

    //! Explicitly set all vertices for each triangle.
    EPT_TRIANGLES
};

enum VertexAttributeType
{
    VET_FLOAT1,
    VET_FLOAT2,
    VET_FLOAT3,
    VET_FLOAT4,
    VET_SHORT1,
    VET_SHORT2,
    VET_SHORT3,
    VET_SHORT4,
    VET_BYTE3,
    VET_BYTE4,
};

enum VertexAttribute
{
    VF_POSITION      = 1<<0, // vertex position 3 floats x,y,z
    VF_COLOR         = 1<<1, // vertex color 4 bytes r,g,b,a
    VF_NORMAL        = 1<<2, // normal 3 floats x,y,z
    VF_TANGENT       = 1<<3, // normal 3 floats x,y,z
    VF_BINORMAL      = 1<<4, // normal 3 floats x,y,z
    VF_TEXCOORD1     = 1<<5, // texcoord 1 2 floats u,v
    VF_TEXCOORD2     = 1<<6, // texcoord 1 2 floats u,v
    VF_TEXCOORD3     = 1<<7, // texcoord 1 2 floats u,v
    VF_TEXCOORD4     = 1<<8, // texcoord 1 2 floats u,v
    VF_TEXCOORD5     = 1<<9, // texcoord 1 2 floats u,v
    VF_TEXCOORD6     = 1<<10, //texcoord 1 2 floats u,v

};





int FVFDecodeLength(unsigned int fvf);
UINT getTypeFormat(VertexAttributeType etype);
UINT getTypeSize(VertexAttributeType etype);
UINT getTypeCount(VertexAttributeType etype);



struct Color
{
    float r,g,b,a;

	Color() : r( 1.0f ), g( 1.0f ), b( 1.0f ), a( 1.0f )
	{
	}
	
	Color( const float r, const float g, const float b, const float a=1.0f) :
		r( r ), g( g ), b( b ), a( a )
	{
	}

	Color(const Color &v ) : r( v.r ), g( v.g ), b( v.b ), a( v.a )
	{
	}
    void set(const Color &c)
    {
        this->r=c.r;
        this->g=c.g;
        this->b=c.b;
        this->a=c.a;
    }
    void set(const float r, const float g, const float b, const float a=1.0f)
    {
        this->r=r;
        this->g=g;
        this->b=b;
        this->a=a;
    }
    
    bool operator==( const Color &v ) const
	{
		return (r == v.r  && 
				g == v.g  && 
                b == v.b  && 
                a == v.a  );
	}
};


class Texture
{
    public:
        Texture(){};
        virtual ~Texture(){};
        UINT getId() {return id;}
        
    protected:
        UINT id;   
     
};

class Texture2D  : public Texture
{
    public:
    Texture2D()
    {

    }
    virtual ~Texture2D()
    {
         if (id != 0) 
         {
           glDeleteTextures(1, &id);
           Log(0, "TEXTURE2D: [ID %i] Unload Opengl Texture2D", id);
        }
    }
    Texture2D(int w, int h);
    bool Load(const std::string &file_name,bool flip_y=false);
    void Bind(UINT unit);
    void UnBind(UINT unit); 
    
  

    private:
      
        int width;          
        int height;         
        int components;         
};


class TextureCube : public Texture
{
    
    public:
    ~TextureCube()
    {
        if (id>0)
            Log(0, "TEXTURE2D: [ID %i] Unload Opengl TextureCubemap", id);
    }
    
    TextureCube(std::vector<std::string> faces);
    void Bind();
    void UnBind(); 

};

class Material
{
    public:
        Vec3 ambient;
        Vec3 difusse;
        Vec3 specular;
        Vec3 emisive;
        Texture2D textures[4];
    
};

class Shader
{
    public:
    Shader()
    {
        m_program = 0;
    }
    ~Shader()
    {
        glDeleteProgram(m_program);
        Log(0, "SHADER: [ID %i] Unloaded shader program", m_program);
    }
    
    bool operator ==(const Shader&      other) const { return m_program == other.m_program; }
    bool operator !=(const Shader&      other) const { return m_program != other.m_program; }

    
    void Bind() 
    { 
        glUseProgram(m_program); 
    }
    void unBind() 
    { 
        glUseProgram(0); 
    }
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(m_program, name.c_str()), value); 
    }
    void setFloat4(const std::string &name, float x,float y, float z,float w) const
    { 
        glUniform4f(glGetUniformLocation(m_program, name.c_str()), x,y,z,w); 
    }
      void setColor(const std::string &name, const Color &c) const
    { 
        glUniform4f(glGetUniformLocation(m_program, name.c_str()), c.r,c.g,c.b,c.a); 
    }
    void setFloat3(const std::string &name, float x,float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(m_program, name.c_str()), x,y,z); 
    }
    void setVector3(const std::string &name, const Vec3 &v) const
    { 
        glUniform3f(glGetUniformLocation(m_program, name.c_str()), v.x,v.y,v.z); 
    }
    void setFloat2(const std::string &name, float x,float y) const
    { 
        glUniform2f(glGetUniformLocation(m_program, name.c_str()), x,y); 
    }
    
    void setMatrix(const std::string &name, const GLfloat *value, GLboolean transpose = GL_FALSE) const
    { 
        glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1 , transpose,value); 
    }
    void setMatrix4(const std::string &name, Mat4 mat, GLboolean transpose = GL_FALSE) const
    { 
       setMatrix(name, mat.x, transpose); 
    }
    
    static Shader createColor();
    static Shader createAmbientDiffuseSpecular();
    static Shader createColorAmbientDiffuse();
    static Shader createColorAmbientDiffuseSpecular();
    static Shader createColorMaterial();
    static Shader createSkyBox();
    static Shader createSolidAmbientDiffuseSpecular();
    static Shader createTangentNormalMap();

    bool create(const char* vShaderCode, const char* fShaderCode);
    void printData();
    bool findUniform(const std::string &name)const;
    int  getUniform(const std::string &name)const;
    int  getUniformLocation(const std::string &uniformName) const;
    int getAttribLocation( const std::string &attribName) const;
    bool addUniform(const std::string &name);
    bool addAttribute(const std::string &name);
    void LoadDefaults();
  
    
    private:
        UINT m_program;
        std::map<std::string, int> m_uniforms;
        std::map<std::string, int> m_attributes;
        int m_numAttributes;
        int m_numUniforms;

    private:
         void checkCompileErrors(unsigned int shader, const std::string &type);
   
};

class VertexDeclaration
{
public:
     struct Element
    {
        UINT index;
        UINT size;
        VertexAttribute   attribute;
        VertexAttributeType type;
     };

    VertexDeclaration():m_iVertexOffSetSize(0)
    {}

    void addElement(UINT index, VertexAttribute attribute, VertexAttributeType  type)
    {
        Element element;
        element.index=index;
        element.attribute=attribute;
        element.type=type;
        element.size=getTypeSize(type);
        m_iVertexOffSetSize += getTypeSize(type);
        elements.push_back(element);
    }

    const std::vector<Element>& getElements() const
    {
        return elements;
    }
    const UINT getSize(){ return (UINT)m_iVertexOffSetSize;};

private:
    std::vector<Element> elements;
    UINT m_iVertexOffSetSize;

};



class VertexArray
{
    public:
        VertexArray()
        {
            glGenVertexArrays(1, &m_id);
            glBindVertexArray(0);
          //  Log(0, "VAO: [ID %i] Create vertex array", m_id);
        }

        ~VertexArray()
        {
            glDeleteVertexArrays(1, &m_id);
          //  Log(0, "VAO: [ID %i] Delete vertex array", m_id);
        }

        void Bind()
        {
            glBindVertexArray(m_id);
        }
        void unBind()
        {
            glBindVertexArray(0);
        }
    private:
            UINT m_id{0};
            
             
};

class VertexBuffer

{
    public:
    VertexBuffer(UINT vertexSize, UINT vertexCount, bool dynamic=false)
        : m_dynamic(dynamic),m_vertexSize(vertexSize), m_vertexCount(vertexCount)
        {
            m_dataSize = vertexSize * vertexCount;
            glGenBuffers(1, &m_id);
         //   Log(0, "VBO: [ID %i] Create vertex buffer", m_id);
        }

        ~VertexBuffer()
        {
            glDeleteBuffers(1, &m_id);
           // Log(0, "VBO: [ID %i] Delete vertex buffer ", m_id);
        }

        UINT getVertexSize()   const { return m_vertexSize; }
        UINT  getVertexCount() const { return m_vertexCount; }


        void Bind()
        {
             glBindBuffer(GL_ARRAY_BUFFER, m_id); 
        }
        void UnBind()
        {
             glBindBuffer(GL_ARRAY_BUFFER, 0); 
        }
      
        // unsigned char* Lock(UINT offset)
        // {
        //     m_offset = offset;
        //     unsigned char* m_data;
        //     Bind();
        //     m_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
             
        //     return m_data;
        // }
        // void Unlock()
        // {
            
        //    // glBindBuffer(GL_ARRAY_BUFFER, m_id);
        //    // glBufferSubData(GL_ARRAY_BUFFER, m_offset, m_dataSize, m_data);
        //    glUnmapBuffer(GL_ARRAY_BUFFER);
        //    UnBind();
            
        // }
        void createBuffer(void *data)
        {
              glBindBuffer(GL_ARRAY_BUFFER, m_id);
              glBufferData(GL_ARRAY_BUFFER, m_dataSize, data, m_dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
              glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        void updateBuffer(void *data, UINT offset)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id);
            glBufferSubData(GL_ARRAY_BUFFER, offset, m_dataSize, data);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
     
        }


 
    private:
            UINT m_id{0};
            bool m_dynamic{false};
            UINT m_offset{0};
            UINT m_vertexSize;
            UINT m_vertexCount;
            UINT m_dataSize;
           // unsigned char* m_data;
             
};


class IndexBuffer

{
    public:
        IndexBuffer(E_INDEX_TYPE type, UINT indexCount, bool dynamic=false)
          : m_dynamic(dynamic),m_type(type), m_indexCount(indexCount)
      
        {

            if (type==I16BIT)
                 m_dataSize = indexCount * sizeof(unsigned short);
            else
                 m_dataSize = indexCount * sizeof(unsigned int);
            
            glGenBuffers(1, &m_id);
           // Log(0, "EBO: [ID %i] Create index buffer", m_id);
        }

        ~IndexBuffer()
        {
            glDeleteBuffers(1, &m_id);
          //  Log(0, "EBO: [ID %i] Delete index buffer ", m_id);
        
        }

        void Bind()
        {
              glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  m_id); 
        }
        void UnBind()
        {
              glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  0); 
        }
     
        void createBuffer(void *data)
        {
               glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
               glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_dataSize, data, m_dynamic? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
               glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  0); 
        }
  
        E_INDEX_TYPE getType(){return m_type;}
        
    private:
            UINT m_id{0};
            bool m_dynamic{false};
            UINT m_indexCount;
            E_INDEX_TYPE m_type;
             UINT m_dataSize;
};


class VertexData
{
public:

    VertexData()
    {
      m_vertex_declaration = new VertexDeclaration();  
      m_vertex_array       = new VertexArray();
    }
    ~VertexData()
    {
        delete m_vertex_array;
        delete m_vertex_declaration;
        if (m_vertex_buffer)
            delete m_vertex_buffer;
        if(m_index_buffer)
            delete m_index_buffer;
    }
    
   IndexBuffer *createIndexBuffer( E_INDEX_TYPE type, UINT indexCount, bool dynamic=false);
   VertexBuffer *createVertexBuffer(UINT vertexSize, UINT vertexCount, bool dynamic=false);
   VertexDeclaration *getVertexDeclation();
   void Render(UINT mode,UINT count);
   void Update();
   
  
   VertexBuffer *getVertexBuffer(){ return m_vertex_buffer;};
   IndexBuffer  *getIndexBuffer(){return m_index_buffer;};
   VertexArray  *getVertexArray(){return m_vertex_array;};

     
private:
    VertexArray *m_vertex_array;
    IndexBuffer *m_index_buffer{nullptr};
    VertexBuffer *m_vertex_buffer{nullptr};
    VertexDeclaration *m_vertex_declaration;
    bool m_isDirty{true};

};




struct Vertex
{
  //ordem IMPORTA = SHADER = Elements
      Vec3  pos;
      Vec2  uv;
      Vec3  normal;
      Vec3  tangent;
    
    
      Vertex()
      {

      };
     Vertex(float x, float y, float z)
		 {
          pos.set(x,y,z);
     }
     Vertex(const Vec3 &v)
		 {
          pos.set(v.x, v.y, v.z);
     }

     Vertex(float x, float y, float z, float tu, float tv)
		 {
        pos.set(x,y,z);
        uv.set(tu,tv);
      
     }

     Vertex(float x, float y, float z, float tu, float tv,float nx, float ny,float nz)
		 {
        pos.set(x,y,z);
        uv.set(tu,tv);
        normal.set(nx,ny,nz);
     }
 
};

class Mesh;



class Surface
{
  private:
    
      friend class Mesh;
      

  std::vector<Vertex>         vertices;
  std::vector<unsigned int>    indices;
  VertexData *vertexData;
  
  std::string                   m_name;
  int                           m_material;
  
  public:
  Surface(const std::string &name="Surface",int MateriaIndex=0):m_material(MateriaIndex),m_name(name)
  {
    vertexData = new VertexData();
  }
  ~Surface()
  {
    delete vertexData;
  }

  int AddVertex( const Vertex &v)
  {
      vertices.push_back(v);
      return (int)vertices.size()-1;
  }
  int AddVertex( float x, float y, float z)
  {
    return AddVertex(Vertex(x,y,z));
  }

  int AddVertex( float x, float y, float z, float tu,float tv)
  {
    return AddVertex(Vertex(x,y,z,tu,tv));
  }

    int AddVertex( float x, float y, float z, float tu,float tv,float nx, float ny, float nz)
  {
    return AddVertex(Vertex(x,y,z,tu,tv,nx,ny,nz));
  }

  int AddVertex( const Vec3 &pos, const Vec2 &uv)
  {
    return AddVertex(Vertex(pos.x,pos.y,pos.z,uv.x,uv.y));
  }
    int AddVertex( const Vec3 &pos, const Vec2 &uv, const Vec3 &n)
  {
    return AddVertex(Vertex(pos.x,pos.y,pos.z,uv.x,uv.y,n.x,n.y,n.z));
  }
  int AddVertex( const Vec3 &pos, const Vec2 &uv, const Vec3 &n,const Vec3 &tan)
 {
    Vertex v;
    v.pos.set(pos);
    v.normal.set(n);
    v.uv.set(uv);
    v.tangent.set(tan);
    vertices.push_back(v);
    return (int)vertices.size()-1;
 }
  
    int AddVertex( const Vertex &vtx,const Vec3 &tan)
 {
    Vertex v;
    v.pos.set(vtx.pos);
    v.normal.set(vtx.normal);
    v.uv.set(vtx.uv);
    v.tangent.set(tan);
    vertices.push_back(v);
    return (int)vertices.size()-1;
 }
  
    void VertexNormal(int index, float x, float y ,float z)
    {
        if (index < 0 || index > (int)vertices.size())
            return;
        vertices[index].normal.set(x,y,z);
    }
  
   void VertexTexCoords(int index , float x, float y)
   {
        if (index < 0 || index > (int)vertices.size())
            return;
        vertices[index].uv.set(x,y);;
    }


    
    void addElement(UINT index, VertexAttribute attribute, VertexAttributeType  type);
    void setElements();
    void render();
    void build();
    int AddTriangle(int a,int b,int c);
    int addFace(int a,int b,int c);
    void UpdateNormals() ;
    void ComputeTangents();

    int   getTotalVertices()   {return (int) vertices.size();}
    int   getTotalIndices()   {return (int) indices.size();}
    int   getTotalFaces()  {return (int) indices.size()/3;}

    Vec3 &getPosition(int index)  
    {
        return vertices[index].pos;   
    }
    Vertex &getVertex(int index)  
    {
        return vertices[index];   
    }
      Vertex *getVertices()  
    {
        return vertices.data();   
    }
    unsigned int getIndice(int index)  
    {
        return indices[index]   ;
    }
    unsigned int *getIndices()  
    {
        return indices.data();
    }
    
    unsigned int getFace(int index,int layer) const 
    {
        return indices[index * 3 + layer]   ;
    }
    
    int   getMaterialIndex() const  {return m_material;};
    std::string   getName() const    {return m_name;}

   void setMaterialIndex(int id){m_material=id;};
   void setName(const std::string &name)
   {m_name=name;}
   

 
};

class Mesh
{
  public:
    Mesh()
    {
        
    }
    ~Mesh()
    { 
        Clear();
    }
    
Surface *CreateSurface(const std::string &name="sruface",int material=0);
void Clear();
void Build();
void UpdateNormals();
void ComputeTangents();
void Render(const Shader &shader);
int             getMaterialIndex(int id);
std::string     getName(int id) const;
void            setMaterialIndex(int id,int mat);
void            setName(int id,const std::string &name);
void            AddMaterial(Material *material);
Material        *getMaterial(int id);

std::vector<Surface*>  getSurfaces()  { return surfaces;}
Surface *getSurface(int index)  ;
int getTotalSurfaces() ;

static Mesh *CreatePlane(float w, float d);
static Mesh* CreateCone(float height=1.0f,int segments=8,bool solid=true);
static Mesh* CreateCylinder(float height=1.0f, int verticalsegments=12,bool solid=true);
static Mesh* CreateCube(float w, float h, float d);
static Mesh *CreateSphere(int segments=12);
static Mesh *CreateTorus(float min_rad,float max_rad);
static Mesh *CreateHills(const Vec2 &tileSize, int NumXTiles,int NumYTiles, float hillHeight,const Vec2 &textureRepeatCount, const Vec2 &countHills);
static Mesh* LoadMesh(const std::string &filename,bool invertZ=false);
static Mesh* ImportOBJ(const std::string &filename);




  private:

      friend class Surface;
   
    std::vector<Surface*>  surfaces;
    std::vector<Material*> materials;
  
};
class ShadowMesh 
{
    public:
        ShadowMesh(Mesh *m, bool zfailmethod=true, float infinity=100000.0f):
        m_mesh(m),AdjacencyDirtyFlag(true),	 IndexCount(0), VertexCount(0), ShadowVolumesUsed(0),	Infinity(infinity), UseZFailMethod(zfailmethod)
        {
            glGenBuffers(1, &VertexBufferID);


            int StencilBits;
            glGetIntegerv(GL_STENCIL_BITS, &StencilBits);
            midStencilVal = (StencilBits - 1)^2;
            glClearStencil(midStencilVal);

            glGenBuffers(1, &stencil_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, stencil_vbo);
            GLfloat q3[] = {1,1,-1,1,-1,-1,1,-1};
            glBufferData(GL_ARRAY_BUFFER,8*sizeof(float),q3,GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        }
        ~ShadowMesh()
        {
            glDeleteBuffers(1, &VertexBufferID);
            glDeleteBuffers(1, &stencil_vbo);
        }

        void render(const Mat4 &m, const Vec3 &light);
        
    private:
           typedef std::vector<Vec3> SShadowVolume;
            std::vector<SShadowVolume> ShadowVolumes;

    	void createShadowVolume();
		int  createEdgesAndCaps(SShadowVolume* svp);
		void renderVolume();
        void calculateAdjacency();
        void updateShadowVolumes();

		
	
    
    private:
        Mesh *m_mesh;
        std::vector<Vec3> Vertices;
		std::vector<unsigned short> Indices;
		std::vector<unsigned short> Adjacency;
		std::vector<unsigned short> Edges;
		std::vector<bool> FaceData;
        Vec3 m_light;
        bool isDirectional{false};
		bool AdjacencyDirtyFlag;
       


		
		int IndexCount;
		int VertexCount;
		int ShadowVolumesUsed;

        unsigned int VertexBufferID;
        GLuint stencil_vbo;
        
		float Infinity;
		bool UseZFailMethod;
		int midStencilVal;
		
        
    
};

class SkyBox
{
  public:
    SkyBox();
    ~SkyBox();
    void Render(TextureCube *texture);
private:
   unsigned int mVAO, mVBO;
};
