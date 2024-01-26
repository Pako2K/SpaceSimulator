#ifndef G_PICTURE_H
#define G_PICTURE_H

#include <memory>

#include <glboost/program.h>
#include <glboost/graphic_types.h>


namespace glboost
{
  // A Texture basic representation 
  class gPicture
  {
  public:
    // Constructors: THE BIG-6
    // Show the picture in the specified position and adjust to the given size
    gPicture(const Position2D& pos, const Size2D& size, const std::string& img_file, bool keep_aspect = false);
    // Show the picture in the specified position with the picture size
    gPicture(const Position2D& pos, const std::string& img_file, bool keep_aspect = false);
    gPicture(const gPicture&) = delete;
    gPicture(gPicture&&) = delete;
    gPicture& operator=(const gPicture&) = delete;
    gPicture& operator=(gPicture&&) = delete;
    ~gPicture();

    // SETTERS
    void   position(const Position2D& new_pos);
    void   size(const Size2D& new_size);
    Size2D image(const std::string& img_file);
    // Transparency applied to the image
    void   opaqueness(float alfa);

    // GETTERS
    const Position2D& position() const { return _position; }
    const Size2D& size() const { return _size; }
    float transparency() const { return _alfa; }

    // Draws the image in a rectangle
    void draw();


  protected:
    static void createRenderer();

    void uploadVertices();


  private:
    inline static std::unique_ptr<glboost::Program> _renderer_ptr{ nullptr };

    /**
      *  \brief  GL objects: VAO, VBO
      */
    GLuint  _vertex_array;
    GLuint  _vertex_buffer;

    // Uniforms
    GLint   _uAlfa;

    // Texture
    GLuint  _texture_id{ 0 };
    
    Position2D _position;
    Size2D     _size;
    bool       _keep_aspect;
    ColorAttrNormalized _alfa{ 1 };
  };

}


#endif // G_PICTURE_H