#ifndef W_MAIN_WINDOW_FS_H
#define W_MAIN_WINDOW_FS_H

#ifndef GLEW_STATIC
  #define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <glboost/graphic_types.h>
#include <glwnd/core/w_container.h>
#include <glwnd/core/keyboard_interface.h>


/**
  * THESE MACROS CAN BE USED FOR DEBUGGING PURPOSES
  *
  *  #define W_DEBUG_BUILD
  *  #define W_DEBUG_OPENGL
  */

namespace glwnd
{
  /**
    * \brief  Full Screen Main Window abstract singleton class (drawMainWindow() and eventLoopIteration() should be re-implemented)
              Application main windows must be derived from this class.
              It's a container and it's able to handle keyboard events.
              Derived classes must choose the member methods to be exposed from WContainer and the MouseInterface, since both are inherited as protected.
              It initializes the window context using GLFW and initializes OpenGL with GLEW,as well as handling the mouse and keyboard callbacks
              Contains the event loop and all the window related setup (GLFW, OPenGL), event callbacks, etc.
     
              The sequence of methods is:
                0. (OPTIONAL) -> Static methods to change the default window and context features. They can also be called after initialize(), but fbefore the constructor:
                                         contextVersion()
                                         contextProfile() -> NA (TBD)
                                         samples()
                                         maximized()-> NA
                                         border()-> NA
                                         visible()-> NA
                1. Static initialize() -> Initialization of GLFW and setup of Window and OpenGL Context parameters:
                                               OPEN GL version and profile, multisampling, some window features (like full maximized, no border, no visible)
                                          Stores the screen resolution in pixels.
                2. Singleton constructor WMainWindowFS() -> Checks that no MainWindow was already created. If so it generates an exception
                                                            It initializes the Main Window Container/Object with the size of the screen
                                                            No parameters are provided. Parameters like window title, size, background color, swap interval, etc. can be changed before the window is shown.
                                                            Creates the GLFW window and links it to the current context.
                                                            Set keyboard and mouse callbacks.
                                                            Initialize GLEW and if W_DEBUG_OPENGL is defined it enables OpenGL debugging callbacks.
                                                              Only used for Debug and Debug-Lib builds !!!!
                                                            After this constructor is finished, the derived class constructor is executed. It should have
                                                              the logic to create and setup the objects in the Main Window
                    2-b (OPTIONAL) -> Methods to change the default window instance parameters (position, size, background color, title, swap interval, etc):
                                        swapInterval()
                                        background()
                                        position() -> NA
                                        size() -> NA
                                        title() -> NA
                3. Execute run() -> Sets the background color, sets the swap interval and makes the window visible.
                                    Starts the event loop, based on event polling, which runs until the WindowShouldClose event is received.
                                    In the event loop:
                                        a- Buffers are cleared
                                        b- virtual method eventLoopIteration() is called. It can be implemented by derived classes. For instance,
                                             to execute all the non graphical logic
                                        c- Container objDraw() method is called. This calls:
                                              i - The main window virtual draw(). It can be implemented by derived classes
                                              ii - The container postDrawing(). It calls the virtual draw() of all the children objects in the container,
                                                     which were possibly added inside the initializeApp()
                4. Singleton destructor ~WMainWindowFS() -> Destroys the GLWF window, and resets the _main_window_ptr to null. The destructor of the derived application
                                                              must take care of the clean up of the objects created in its constructor
                5. Static terminate() -> Terminates GLFW

              Other GET methods:
                  keyState()
                  mouseButtonState()
                  mousePosition()
                  close()

              Callbacks: mouse and keyboard events are captured by the MainWindow and translated into the events defined in MouseInterface,
                which must be implemented by the derived classes
    */
  class WMainWindowFS : protected WContainer, protected KeyboardInterface
  {
  public:
    /**
      *  \brief  Initialize GLFW, GLFW error callback, get screen size
      *  @throw  std::string   Description of the issue. Typical issues are:
      *                        \par
      *                        - GLFW cannot be initialized
      */
    static void  initialize();

    /**
      *  \brief Set the OpenGL version to be supported by the context
      *         Must be set before creating the Main Window. It will always use the CORE_PROFILE
      */
    static void  contextVersion(uint8_t major, uint8_t minor);

    /**
      *  \brief Set number of samples to be used for antialiasing
      *         Must be set before running the Main Window
      */
    static void  samples(uint8_t samples);

    /**
      *  \brief Disable mouse move/drag events
      *         Must be set before running the Main Window
      */
    static void  disableMouseMove();

    /**
      *  \brief Disable mouse scroll events
      *         Must be set before running the Main Window
      */
    static void  disableMouseScroll();

    /**
      *  \brief Disable mouse click events
      *         Must be set before running the Main Window
      */
    static void  disableMouseClick();

    /**
      *  \brief Disable keyboard events
      *         Must be set before running the Main Window
      */
    static void  disableKeyboard();

    /**
      *  \brief  Terminate GLFW
      */
    static void  terminate();

    /**
      *  \brief  Get screen size
      */
    static glboost::Size2D  screenSize() { return _screen_size; }

    /**
      *  \brief Destructor of the main window.
      *         Destroys the GLFW window
      */
    virtual ~WMainWindowFS();

    // Get the position (making the WObject visible at the WMainWindowFS level)
    const glboost::Position2D&  position() const override { return WObject::position(); }

    // Get the size (making the WObject visible at the WMainWindowFS level)
    const glboost::Size2D&      size() const override { return WObject::size(); }
    
    /**
      *  \brief Set number of screen refreshes before the actual buffer swap is executed
      *         Must be set before running the Main Window
      */
    void  swapInterval(uint8_t interval) { _swap_interval = interval; }

    /**
      *  \brief Set default window background color
      *  @param [in] color  Color4i: RGBA color with values [0,255]
      */
    void  background(glboost::Color4i color) { background(color.normalized()); }

    /**
      *  \brief Set default window background color
      *  @param [in] color  Color4f: Normalized RGBA color
      */
    void  background(glboost::Color4f color) { _bkg_color = color; }

    /**
      *  \brief   Starts the Event Loop. See class description
      */
    void  run();


  protected:
    /**
      *  \brief  Constructor of the main window singleton. See class description
      *
      *  @throw  std::logic_error  Main Window already created
      *  @throw  std::runtime_error  Window cannot be created or used with the current OpenGL context
      */
    WMainWindowFS();

    /**
      *  \brief DELETED Copy Constructors
      */
    WMainWindowFS(const WMainWindowFS &other) = delete;
    WMainWindowFS(WMainWindowFS &&other) = delete;

    /**
      *  \brief DELETED Assignment Operators
      */
    WMainWindowFS& operator=(const WMainWindowFS& other) = delete;
    WMainWindowFS& operator=(WMainWindowFS&& other) = delete;

    // Implement pure virtual method. Should be re-implemented by derived classes
    virtual void        drawContainer() final { drawMainWindow(); }

    // Implement pure virtual method. Should be re-implemented by derived classes
    virtual void        drawMainWindow() {}

    /**
      *  \brief  Application specific logic for each event loop iteration
      *          It should be overridden by the derived Main Window Application.
      */
    virtual void        eventLoopIteration() {};
       
    /**
      *  \brief Key state
      */
    int                 keyState(int key) const { return glfwGetKey(_window, key); }

    /**
     *  \brief Get Mouse state
     */
    int                 mouseButtonState(int button) const { return glfwGetMouseButton(_window, button); }

    /**
      *  \brief Get Mouse position
      */
    glboost::Position2D mousePosition() const {
      double posX, posY;
      glfwGetCursorPos(_window, &posX, &posY);
      return glboost::Position2D(glboost::PositionPxl(posX), glboost::PositionPxl(posY));
    };
    
    /**
      *  \brief Closes the Main Window
      *  @return void
      */
    void                close() { glfwSetWindowShouldClose(_window, GLFW_TRUE); }


  private:
    /* ******************************************** Data Members ************************************************************ */
    // OpenGL major version (not initialized)
    static int             _Open_GL_major;

    // OpenGL major version (not initialized)
    static int             _Open_GL_minor;

    // OpenGL profile (not initialized)
    static int             _Open_GL_profile;

    // Multisampling: number of samples for antialiasing
    static int             _samples;

    // Flags for enabling mouse and keyboard interfaces
    static bool            _enable_mouse_move;
    static bool            _enable_mouse_scroll;
    static bool            _enable_mouse_click;
    static bool            _enable_keyboard;

    // Flag to detecet whether the singleton already exists
    static WMainWindowFS*  _main_window_fs;

    /**
      *  \brief  Full Screen size
      */
    static glboost::Size2D _screen_size;

    /**
      *  \brief  Buffer Swap interval
      */
    uint8_t             _swap_interval;


    /**
      *  \brief  Background color (DEFAULT: white, opaque)
      */
    glboost::Color4f    _bkg_color;


    /**
      *  \brief  Pointer to GLFW window
      */
    GLFWwindow*         _window{ nullptr };
    /* ******************************************** Data Members (END) ****************************************************** */
    
    /* ******************************************** Operations (CALLBACKS) ************************************************** */

    /**
      *  \brief GLFW Error events callback. It shows the error
      */
    static void  glfw_error_callback(int error, const char* description);

    /**
      *  \brief GLFW Keyboard events callback
      */
    static void  key_callback(GLFWwindow * window, int key, int scancode, int action, int modifiers);

    /**
      *  \brief GLFW Mouse events callback
      */
    static void  mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    /**
      *  \brief GLFW Mouse motion callback
      */
    static void  mouse_position_callback(GLFWwindow* window, double xpos, double ypos);

    /**
      *  \brief GLFW Mouse scroll callback
      */
    static void  mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


    /**
      *  \brief OPEN GL Debug Callback
      */
#ifdef W_DEBUG_OPENGL
    static void GLAPIENTRY glErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif
    /* *********************************************** Operations (END) ***************************************************** */


    // Check whether the MouseRepeatClicked event mus be called
    void checkButtonRepeatClicked();
  };

}


#endif // W_MAIN_WINDOW_FS_H
