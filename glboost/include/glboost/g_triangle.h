#ifndef G_TRIANGLE_H
#define G_TRIANGLE_H

#include <memory>

#include <glboost/program.h>

#include <glboost/graphic_types.h>


namespace glboost
{
  // A graphical triangle defined by the position of 3 points and a border color (by default black)
  // The positions of the triangle vertices can be changed
  // It can be filled (by defualt not filled).
  // Border can be removed
  // Border/filling can be added or removed
  class gTriangle
  {
  public:
    // Constructors: THE BIG-6
    gTriangle(const Position2D& pos1, const Position2D& pos2, const Position2D& pos3);
    gTriangle(const gTriangle&) = delete;
    gTriangle(gTriangle&&) = delete;
    gTriangle& operator=(const gTriangle&) = delete;
    gTriangle& operator=(gTriangle&&) = delete;
    ~gTriangle();

    // SETTERS
    void vertex(size_t pos, const Position2D& new_pos);
    void fillColor(const Color4f& new_color) { _fill_color = new_color; }
    void borderColor(const Color4f& new_color) { _border_color = new_color; }
    void fill(bool flag);
    void border(bool flag);
    void lineWidth(SizePxl new_width);

    // GETTERS
    const Position2D& operator[] (size_t pos) const { return _positions.at(pos); }
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

    // Uniforms
    GLint   _uColor;

    std::array<Position2D, 3> _positions;
    bool       _border{ true };
    bool       _fill{ false };
    Color4f    _fill_color{ 0, 0, 0, 0 };
    Color4f    _border_color{ 0, 0, 0, 1 };
    SizePxl    _line_width{ 1 };

    // Function pointer to drawing function
    void (gTriangle::* draw_fn_ptr)(void) { &gTriangle::drawBorder };
  };

}


#endif // G_TRIANGLE_H