#ifndef W_LIST_H
#define W_LIST_H

#include <map>
#include <set>

#include <glwnd/core/w_control.h>
#include <glboost/g_rectangle.h>
#include <glwnd/w_label.h>


namespace glwnd
{
  // A list is a collection of labels which can be seleted, one at a time or unlimited (multi-selection)
  // Each label has an id and a text
  // In case the list contains more than twice the number of visible elements, scroll buttons will be shown
  class WList : protected WControl
  {
  public:
    static void borderColor(glboost::Color4f color) { __def_border_color = color; }
    static void backgroundColor(glboost::Color4f color) { __def_background_color = color; }
    static void elementBorderColor(glboost::Color4f color) { __def_label_border_color = color; }
    static void selectedBorderColor(glboost::Color4f color) { __def_selected_border_color = color; }
    static void elementBackgroundColor(glboost::Color4f color) { __def_label_bkg_color = color; }
    static void fontName(std::string font_name) { __def_font_name = font_name; }
    static void fontColor(glboost::Color4f color) { __def_font_color = color; }
    static void fontPreSelColor(glboost::Color4f color) { __def_font_presel_color = color; }
    static void fontSize(glboost::SizePxl size) { __def_font_size = size; };
    static void elementSpacing(glboost::SizePxl spacing) { __def_label_spacing = spacing; };

    WList(const glboost::Position2D& pos, const glboost::Size2D& size, bool multi_selection = false);
    WList(glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height, bool multi_selection = false);
    WList(WContainer& parent, const glboost::Position2D& pos, const glboost::Size2D& size, bool multi_selection = false);
    WList(WContainer& parent, glboost::PositionPxl pos_x, glboost::PositionPxl pos_y, glboost::SizePxl width, glboost::SizePxl height, bool multi_selection = false);
    
    WList(const WList&) = delete;
    WList(WList&&) = delete;
    WList& operator=(const WList&) = delete;
    WList& operator=(WList&&) = delete;
    
    ~WList();

    // Add element 
    void add(const std::string& text);

    // Update exising element (starting position = 1), or inserts at the end of the list if the position is bigger that the current size
    void update(size_t pos, const std::string& text);

    // Remove last element 
    void pop();

    // Remove all elements
    void clear();

    // Deselect all elements
    void unselect();

    // Returns the text of the selected elements
    std::vector<std::string> selected() const;

    // Returns the number of elements
    size_t listSize() const;


  protected:
    virtual void buttonClicked(MouseButton button, int modifiers, const glboost::Position2D& pos) override;
    virtual void buttonReleased(MouseButton button, int modifiers, const glboost::Position2D& pos) override;
    virtual void scrolled(glboost::PositionPxl offset, const glboost::Position2D& pos) override;
    virtual void buttonRepeatClicked(const glboost::Position2D& pos) override {};
    virtual void buttonDoubleClicked(const glboost::Position2D& pos)  override {};
    virtual void dragged(MouseButton button, const glboost::Position2D& pos)  override {};
    virtual void mouseMoved(const glboost::Position2D& pos)  override {};
    virtual void draw() override;

  private:
    class LabelSEButton;
    class LabelPagingButton;

    using LabelId = uint64_t;

    inline static glboost::Color4f __def_border_color{ 0, 0, 0, 1 };
    inline static glboost::Color4f __def_background_color{ 0, 0, 0, 1 };
    inline static glboost::Color4f __def_label_bkg_color{ 0, 0, 0, 1 };;
    inline static glboost::Color4f __def_label_border_color{ 0, 0, 0, 1 };
    inline static glboost::Color4f __def_selected_border_color{ 0, 0, 0, 1 };
    inline static glboost::SizePxl __def_label_spacing{ 0 };
    inline static glboost::Color4f __def_font_color{ 1, 1, 1, 1 };
    inline static glboost::Color4f __def_font_presel_color{ 0.5f, 0.5f, 0.5f, 1 };
    inline static glboost::SizePxl __def_font_size{ 20 };
    inline static std::string      __def_font_name{ "Arial" };

    const glboost::Color4f _BORDER_COLOR{ __def_border_color };
    const glboost::Color4f _BACKGROUND_COLOR{ __def_background_color };
    const glboost::Color4f _LABEL_BKG_COLOR{ __def_label_bkg_color };
    const glboost::Color4f _LABEL_BORDER_COLOR{ __def_label_border_color };
    const glboost::SizePxl _LABEL_SPACING{ __def_label_spacing };
    const glboost::Color4f _SELECTED_BORDER_COLOR{ __def_selected_border_color };
    const glboost::Color4f _FONT_COLOR{ __def_font_color };
    const glboost::Color4f _FONT_PRESEL_COLOR{ __def_font_presel_color };
    const glboost::SizePxl _FONT_SIZE{ __def_font_size };
    const std::string      _FONT_NAME{ __def_font_name };
#define ELEMENT_HEIGHT (glboost::SizePxl(2 * __def_font_size - 1))
#define ELEMENT_DISTANCE (glboost::SizePxl(ELEMENT_HEIGHT + __def_label_spacing)) 
    const glboost::SizePxl _ELEMENT_HEIGHT{ ELEMENT_HEIGHT };
    const glboost::SizePxl _ELEMENT_DISTANCE{ ELEMENT_DISTANCE };

    bool _multi_selection;

    // Rectangle with border which encloses the list and buttons
    glboost::gRectangle  _rectangle;

    std::map<LabelId, std::unique_ptr<WLabel>> _labels;

    std::set<LabelId> _selected;

    LabelId _candidate{ 0 };
    size_t _max_visible_elems{ 0 };
    LabelId _lastId{ 0 };
    LabelId _top_element{ 1 };

    size_t _scroll_step{ 1 };
    size_t _scroll_pages{ 1 };
    size_t _fast_scroll_pages{ 1 };
        
    std::unique_ptr<LabelSEButton> _btn_scroll_start;
    std::unique_ptr<LabelSEButton> _btn_scroll_end;
    std::unique_ptr<LabelPagingButton> _btn_scroll_up;
    std::unique_ptr<LabelPagingButton> _btn_scroll_down;
    
    // Initialization
    void initialize();

    // Show elements from the specified position
    void showElementsFrom(LabelId top_pos);

    // Calculate scroll factors
    void setScrollFactors();
  };

}


#endif // W_LIST_H