#ifndef G_RECTANGLE_H
#define G_RECTANGLE_H

#include <memory>



#include <glboost/program.h>

#include <glboost/graphic_types.h>


namespace glboost
{
  // A graphical rectangle defined by a position, size and border color (by default black)
  // It can be filled (by defualt not filled).
  // Border/filling can be added or removed
  class gRectangle
  {
  public:
    // Constructors: THE BIG-6
    gRectangle(const Position2D& pos, const Size2D& size);
    gRectangle(const gRectangle&) = delete;
    gRectangle(gRectangle&&) = delete;
    gRectangle& operator=(const gRectangle&) = delete;
    gRectangle& operator=(gRectangle&&) = delete;
    ~gRectangle();

    // SETTERS
    void position(const Position2D& new_pos);
    void size(const Size2D& new_size);
    void fillColor(const Color4f& new_color) { _fill_color = new_color; }
    void borderColor(const Color4f& new_color) { _border_color = new_color; }
    void fill(bool flag);
    void border(bool flag);
    void lineWidth(SizePxl new_width);

    // GETTERS
    const Position2D& position() const { return _position; }
    const Size2D& size() const { return _size; }
    const Color4f& fillColor() const { return _fill_color; }
    bool fill() const { return _fill; }
    const Color4f& borderColor() const { return _border_color; }
    SizePxl lineWidth() const { return _line_width; }

    // Draws the rectangle depending on the flags. By default only the border,
    void draw() {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      _renderer_ptr->useProgram();
      glBindVertexArray(_vertex_array);
      glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
      (this->*draw_fn_ptr)(); 
      glDisable(GL_BLEND);
    }
         

  protected:
    static void createRenderer();

    void uploadVertices();

    // Draws only the border
    void drawBorder();

    // Draws only the filling
    void drawFilling();

    // Draws border & filling
    void drawAll();

    // Draws nothing
    void drawNothing() {};

  private:
    inline static std::unique_ptr<glboost::Program> _renderer_ptr{ nullptr };

    /**
      *  \brief  GL objects: VAO, VBO
      */
    GLuint  _vertex_array;
    GLuint  _vertex_buffer;
    GLuint  _elements_buffer;

    // Uniforms
    GLint   _uColor;
  
    Position2D _position;
    Size2D     _size;
    bool       _border{ true };
    bool       _fill{ false };
    Color4f    _fill_color{ 0, 0, 0, 0 };
    Color4f    _border_color{ 0, 0, 0, 1 };
    SizePxl    _line_width{ 1 };

    // Function pointer to drawing function
    void (gRectangle::*draw_fn_ptr)(void) { &gRectangle::drawBorder };
  };

}


#endif // G_RECTANGLE_H