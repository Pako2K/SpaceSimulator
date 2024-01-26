//  MIT License
//
//  Copyright (c) 2018 Francisco de Lanuza
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.


#include <string>
#include <iostream>

// Using GLEW-2.1.0: requires glew.h and the static libraries glew32s.lib  (compiled as a dependency in
//        project glew) and libopengl32.a (BE SURE THIS THE ORDER!!!)

#include <GL/glew.h>
// Using GLFW-3.2.1: requires the header file glfw3.h and the static libraries libglfw3.a and libgdi32.a
#include <GLFW/glfw3.h>

#include <glboost/graphic_types.h>
#include <glboost/text.h>

#include <glboost/font.h>

#include <glboost/g_rectangle.h>
#include <glboost/g_triangle.h>
#include <glboost/g_picture.h>


using namespace glboost;



// Number of frame buffer samples for antialiasing
constexpr int FRAME_BUFFER_SAMPLES {4};

// Number of screen refreshes to complete before next framebuffer swap
constexpr int SWAP_INTERVAL {0};

void GLAPIENTRY glErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void createWnd();
void show();

GLFWwindow * _window;


int main(int argc, char** args) {

  Position2D ppp{ 80,90 };
  Size2D sss( 8, 8 );
  Position2D ppp2{ 280,90 };
  Size2D sss2{ 28, 8 };

  ppp += sss;
  Size2D sss3{ sss + sss2 };
  sss3 += sss2;

  auto cc = sss.w();
  sss.w() = 999;

  try{
    createWnd();
  }
  catch(std::string &exc){
    std::cout << "Error creating window: " + exc << std::endl;
    return 1;
  }

  // Window Event Loop
  try{
    show();;
  }
  catch(std::string &exc){
    std::cout << "Error showing window: " + exc << std::endl;
    return 1;
  }

  return 0;
}


/// ************************************************************************************** ///


void createWnd() {
  //Start GLFW for the whole application (all windows reuse the same GLFW)
	if (!glfwInit()) {
		// Initialization failed
		throw std::string("GLFW CANNOT BE INITIALIZED.");
	}

  // Using OpenGL 4.0, core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, FRAME_BUFFER_SAMPLES); // antialiasing

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // Get primary monitor and its current resolution (video mode)
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	if (!monitor) {
    glfwTerminate();
		throw std::string("Could not get monitor.");
	}
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	if (!mode) {
    glfwTerminate();
		throw std::string("Could not get Video mode.");
	}

  // Resolution of the screen is in pixels
	_window = glfwCreateWindow(mode->width, mode->height, "GL WINDOW",  nullptr, nullptr);
	//_window = glfwCreateWindow(mode->width, mode->height, "GL WINDOW",  glfwGetPrimaryMonitor(), nullptr);
	if (!_window){
    glfwTerminate();
    throw std::string("Could not create window.");
	}

	// Make this window OpenGL context the current one
	glfwMakeContextCurrent(_window);
	if (!glfwGetCurrentContext()) {
		glfwDestroyWindow(_window);
		glfwTerminate();
		throw std::string("Couldn't make OpenGL context the current one.");
	}

  // Refresh for the current OpenGL context
	glfwSwapInterval(SWAP_INTERVAL);


	/// INITIALIZE OPEN GL
	glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    glfwDestroyWindow(_window);
    glfwTerminate();
    throw std::string("Not possible to initialize GL: " + std::string((char *)glewGetErrorString(err)));
  }
  
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(glErrorMessageCallback, 0);
}


void show()
{
  Font &fnt {Font::get("Candarab", 12)};

  fnt.release();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  Text txt_fps {"", 0, 0};
  Font::addFontsDir("fonts/");
  txt_fps.font("Orbitron Medium", 15);
  //txt_fps.font(24);
  txt_fps.color(Color3i(205, 205, 0));

  Text txt {"aÄÄSSida apioas KJJSääaop sfpdfu asdu aisdua aius isu sui  uss su su. asddd sdd, isido pfa opsfp dfuisidopf aopsfpdfu isidopf", 50, 50};
  txt.color(Color3i(100, 0, 0));
  Text txt1 {"1aÄÄSSida apioas KJJSääaop sfpdfuisidop faopsfp df uisi dopf aop sfpdfu isidopf", 50, 50};
  txt1.color(Color3i(255, 0, 0));
  Text txt2 {"2aÄÄSSida apioas KJJSääa opsfpdfui sid opfaop sfpdfu isid opf aopsfp dfu isidopf", 50, 50};
  txt2.color(Color3i(255, 0, 0));

  gRectangle rec_bor{ Position2D(1300,300), Size2D(800,100) };
  rec_bor.borderColor(Color4f(1, 1, 0, 1));
  rec_bor.fillColor(Color4f(1, 0, 0, 1));
  rec_bor.fill(true);
  rec_bor.lineWidth(26);

  gRectangle rec_bor2{ Position2D(1300,500), rec_bor.size() };
  rec_bor2.borderColor(Color4f(1, 1, 0, 1));
  rec_bor2.fillColor(Color4f(1, 0, 0, 1));
  rec_bor2.fill(true);
  rec_bor2.lineWidth(1);
  
  gRectangle rec_bor3{ Position2D(1300,800), rec_bor.size() };
  rec_bor3.borderColor(Color4f(0.5, 1, 0, 1));
  rec_bor3.lineWidth(10);

  gRectangle rec_bor4{ Position2D(1300,1100), rec_bor.size() };
  rec_bor4.borderColor(Color4f(0.5, 1, 1, 1));
  rec_bor4.fillColor(Color4f(1, 1, 0, 1));
  rec_bor4.fill(true);
  rec_bor4.border(false);

  gRectangle rec_bor5{ Position2D(1300,1300), rec_bor.size() };
  rec_bor5.borderColor(Color4f(0.5, 1, 1, 1));
  rec_bor5.lineWidth(5);
  rec_bor5.fillColor(Color4f(1, 1, 0, 1));
  rec_bor5.fill(true);

  gTriangle tr_bor{ Position2D(2400,1400), Position2D(2600,1400), Position2D(2500,1600) };
  tr_bor.borderColor(Color4f(1, 1, 1, 1));
  tr_bor.lineWidth(8);

  gTriangle tr_bor2{ Position2D(2400,400), Position2D(2600,400), Position2D(2500,600) };
  tr_bor2.borderColor(Color4f(1, 0, 0, 1));
  tr_bor2.lineWidth(3);
  tr_bor2.fillColor(Color4f(0.2f, 1, 0, 1));
  tr_bor2.fill(true);

  gTriangle tr_bor3{ Position2D(2400,800), Position2D(2600,800), Position2D(2500,900) };
  tr_bor3.borderColor(Color4f(1, 1, 1, 1));
  tr_bor3.fillColor(Color4f(1, 0, 0, 1));
  tr_bor3.fill(true);
  tr_bor3.border(false);


  gPicture pict{ Position2D(1300,1500), Size2D(20, 20), "img/play_blue.png", true };
  pict.position(Position2D(1400, 1350));
  pict.size(Size2D(24, 124));
    
  gPicture pict2{ Position2D(1800,1370), Size2D(200, 200), "img/inc_released.png" };
  pict2.opaqueness(0.2f);

  gPicture pict3{ Position2D(1600,1350), Size2D(100, 100), "img/play_blue.png" };
  pict3.image("img/inc_released.png");

  glfwShowWindow(_window);

  

  // Event loop
  GLint64 timer;
  GLint64 new_timer;
  GLint64 diff_timer;
  std::string elapsed_time {""}, fps{" fps"};
  glGetInteger64v(GL_TIMESTAMP, &timer);
  new_timer = timer;
  uint32_t num_frames {0};
	do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    rec_bor.draw();
    rec_bor2.draw();
    rec_bor3.draw();
    rec_bor4.draw();
    rec_bor5.draw();

    tr_bor.draw();
    tr_bor2.draw();
    tr_bor3.vertex(2, Position2D(2700+PositionPxl(100*cos(timer/100000)), 999));
    tr_bor3.draw();

    pict.draw();
    pict2.draw();
    pict3.draw();


    txt_fps.text(fps);
    txt_fps.show();

    txt.font(10);
    txt.show();
    txt1.font(14);
    txt1.position(50,70);
    txt1.show();
    txt2.font(19);
    txt2.position(50,80);
    txt2.show();
    txt.font(24);
    txt.position(50,120);
    txt.show();
        txt.font(9);
    txt.position(50,160);
    txt.show();
    txt.position(50,180);
    txt.show();
    txt.position(50,200);
    txt.show();
    txt.position(50,220);
    txt.show();
    txt.position(50,240);
    txt.show();
    txt.position(50,260);
    txt.show();
    txt.position(50,280);
    txt.show();
    txt.position(50,300);
    txt.show();
    txt.position(50,320);
    txt.show();
    txt.position(50,340);
    txt.show();
    txt.position(450,360);
    txt.show();
    txt.position(450,380);
    txt.show();
    txt.position(450,400);
    txt.show();
    txt.position(450,450);
    txt.show();
    txt.position(450,550);
    txt.show();
    txt.position(450,650);
    txt.show();
    txt.position(450,750);
    txt.show();
    txt.position(450,850);
    txt.show();
    txt.position(450,950);
    txt.show();

    diff_timer = new_timer - timer;
    if (diff_timer > 500000000) {
      fps = std::to_string(int(num_frames * 1e9 / (diff_timer))) + " fps kja";
      std::cout << fps << std::endl;
      timer = new_timer;
      num_frames = 0;
    }


    //std::cout << fps << std::endl;

    glGetInteger64v(GL_TIMESTAMP, &new_timer);
    num_frames++;

//    elapsed_time = std::to_string(1e6 * new_timer - timer);




    glfwSwapBuffers(_window);

		glfwPollEvents();
	} while (!glfwWindowShouldClose(_window));

}



/* glErrorMessageCallback */
void GLAPIENTRY glErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
  static std::map<int, std::string> source_map{ {GL_DEBUG_SOURCE_API, std::string("API - Calls to the OpenGL API")},
                                                {GL_DEBUG_SOURCE_WINDOW_SYSTEM, std::string("WINDOW_SYSTEM - Calls to a window - system API")},
                                                {GL_DEBUG_SOURCE_SHADER_COMPILER, std::string("SHADER_COMPILER - A compiler for a shading language")},
                                                {GL_DEBUG_SOURCE_THIRD_PARTY, std::string("THIRD_PARTY - An application associated with OpenGL")},
                                                {GL_DEBUG_SOURCE_APPLICATION, std::string("APPLICATION - Generated by the user of this application")},
                                                {GL_DEBUG_SOURCE_OTHER, std::string("OTHER - Some other source ")} };

  static std::map<int, std::string> type_map{ {GL_DEBUG_TYPE_ERROR, std::string("ERROR - An error, typically from the API")},
                                              {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, std::string("DEPRECATED_BEHAVIOR - Some behavior marked deprecated has been used")},
                                              {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, std::string("UNDEFINED_BEHAVIOR - Something has invoked undefined behavior")},
                                              {GL_DEBUG_TYPE_PORTABILITY, std::string("PORTABILITY - Some functionality the user relies upon is not portable")},
                                              {GL_DEBUG_TYPE_PERFORMANCE, std::string("PERFORMANCE - Code has triggered possible performance issues")},
                                              {GL_DEBUG_TYPE_MARKER, std::string("MARKER - Command stream annotation")},
                                              {GL_DEBUG_TYPE_PUSH_GROUP, std::string("PUSH_GROUP - Group pushing")},
                                              {GL_DEBUG_TYPE_POP_GROUP, std::string("POP_GROUP")},
                                              {GL_DEBUG_TYPE_OTHER, std::string("OTHER: Some other type")} };

  static std::map<int, std::string> severity_map{ {GL_DEBUG_SEVERITY_HIGH, std::string("HIGH -  Error or highly dangerous undefined behavior")},
                                                  {GL_DEBUG_SEVERITY_MEDIUM, std::string("MEDIUM - Major performance warnings, shader compilation / linking warnings, or use of deprecated functionality")},
                                                  {GL_DEBUG_SEVERITY_LOW, std::string("LOW - Redundant state change, minor performance warning, or unimportant undefined behavior")},
                                                  {GL_DEBUG_SEVERITY_NOTIFICATION, std::string("NOTIFICATION")} };

  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    std::cout  << "GL DEBUG CALLBACK. \nSource = " << source_map[source] << "\nType = " << type_map[type] << "\nId = " << id << "\nSeverity = " << severity_map[severity] << "\nMessage = " << message << "\n";

}
