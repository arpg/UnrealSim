#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
//#include <opencv2/opencv.hpp>
#include <node/node.h>
#include "ImageMessage.pb.h"
#include "InputMessage.pb.h"

//#include <Eigen/Eigen>

#include <pangolin/pangolin.h>
#include <pangolin/glglut.h>

#include <SceneGraph/SceneGraph.h>

void GlobalKeyHook(std::string str)
{
    std::cout << str << std::endl;
}

// track keyboard buttons
unsigned char keys[256] =  {0};
void Keyboard( unsigned char key, int x, int y)
{
  keys[key] = true;
  pangolin::process::Keyboard( key, x, y );
}

void KeyboardUp(unsigned char key, int x, int y)
{
  keys[key] = false;
  pangolin::process::KeyboardUp( key, x, y );
}


int main(int argc, char** argv)
{
  ImageMessage imageMsgL;
  ImageMessage imageMsgR;

  node::node node;
  node.init("node_demo");
  node.subscribe("LeftCamera/Image");
  node.subscribe("RightCamera/Image");
  node.advertise("CarInput");

  // Create OpenGL window in single line thanks to GLUT
  pangolin::CreateWindowAndBind("Main",640,480);
  SceneGraph::GLSceneGraph::ApplyPreferredGlSettings();
  glClearColor(0, 0, 0, 0);

  // takeover glut callbacks (which we will pass back to pangolin(
  glutKeyboardFunc( &Keyboard );
  glutKeyboardUpFunc( &KeyboardUp );

  // Scenegraph to hold GLObjects and relative transformations
  SceneGraph::GLSceneGraph glGraph;

  // Define Camera Render Object (for view / scene browsing)
  pangolin::OpenGlRenderState stacks3d(
      pangolin::ProjectionMatrix(640,480,420,420,320,240,0.1,1000),
      pangolin::ModelViewLookAt(0,0,0, 0,1,0, pangolin::AxisNegZ)
      );

  // Pangolin abstracts the OpenGL viewport as a View.
  // Here we get a reference to the default 'base' view.
  pangolin::View& container = pangolin::DisplayBase();

  // We define a special type of view which will accept image data
  // to display and set its bounds on screen.
  SceneGraph::ImageView viewImage(true,false);
  viewImage.SetBounds( 0, 1, 0, 1, 640.0/480 );
  viewImage.SetLock(pangolin::LockLeft, pangolin::LockTop);


  // Add our views as children to the base container.
  container.AddDisplay(viewImage);

  // Demonstration of how we can register a keyboard hook to trigger a method
  pangolin::RegisterKeyPressCallback( pangolin::PANGO_CTRL + 'r',
      std::bind(GlobalKeyHook, "You Pushed ctrl-r!" ) );

  // Default hooks for exiting (Esc) and fullscreen (tab).
  while( !pangolin::ShouldQuit() )
  {
    bool successL = node.receive( "LeftCamera/Image", imageMsgL );
    bool successR = node.receive( "RightCamera/Image", imageMsgR );

    if (!successL || !successR){
      InputMessage inputMsg;
      inputMsg.set_ahead(false);
      inputMsg.set_back(false);
      inputMsg.set_left(false);
      inputMsg.set_right(false);
      node.publish("CarInput", inputMsg);
      continue;
    }

    int w = imageMsgL.width();
    int h = imageMsgL.height();

    std::string dataL = imageMsgL.data();
    std::string dataR = imageMsgR.data();

    void* vdataL = (void*)dataL.data();
    void* vdataR = (void*)dataR.data();


    InputMessage inputMsg;
    inputMsg.set_ahead(false);
    inputMsg.set_back(false);
    inputMsg.set_left(false);
    inputMsg.set_right(false);

    if( keys[(unsigned char)'a'] || keys[(unsigned char)'A'] ){
      inputMsg.set_left(true);
    }
    if( keys[(unsigned char)'s'] || keys[(unsigned char)'S'] ){
      inputMsg.set_back(true);
    }
    if( keys[(unsigned char)'d'] || keys[(unsigned char)'D'] ){
      inputMsg.set_right(true);
    }
    if( keys[(unsigned char)'w'] || keys[(unsigned char)'W'] ){
      inputMsg.set_ahead(true);
    }


    node.publish("CarInput", inputMsg);

    // Clear whole screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // These calls can safely be made outside of the OpenGL thread.
    viewImage.SetImage(vdataL, w,h, GL_RGB,GL_RGB, GL_UNSIGNED_BYTE);

    // Swap frames and Process Events
    pangolin::FinishFrame();

    // Pause for 1/60th of a second.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
  }

  return 0;
}
