#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <sys/time.h>
#include <time.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach_time.h>
#endif

//#include <opencv2/opencv.hpp>
#include <node/node.h>
#include "ImageMessage.pb.h"
#include "InputMessage.pb.h"

//#include <Eigen/Eigen>

#include <pangolin/pangolin.h>
#include <pangolin/glglut.h>

#include <SceneGraph/SceneGraph.h>



void ResetKeyHook( node::node* n )
{
  if( !n->call_rpc( "VehiclePawn/reset" ) ){
    printf("Error calling server reset\n");
  }
}


double Tic() {
#ifdef __MACH__
  // From Apple Developer Q&A @
  // https://developer.apple.com/library/mac/qa/qa1398/_index.html

  // This doesn't change, so we set it up once
  static mach_timebase_info_data_t timebase;
  if (timebase.denom == 0) {
    mach_timebase_info(&timebase);
  }

  double secs = static_cast<double>(mach_absolute_time()) *
      timebase.numer / timebase.denom * 1e-9;
  return secs;
#elif _POSIX_TIMERS > 0
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec + ts.tv_nsec * 1e-9;
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + 1e-6 * (tv.tv_usec);
#endif
}

double Toc(double dTic) {
    return Tic() - dTic;
}


double TocMS(double dTic) {
  return (Tic() - dTic)*1000.;
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

  // Demonstration of how we can register a keyboard hook to
  // trigger a method
  pangolin::RegisterKeyPressCallback( pangolin::PANGO_CTRL +
      'r', std::bind(ResetKeyHook,&node) );

  // Default hooks for exiting (Esc) and fullscreen (tab).
  double d0 = Tic();
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

//    printf("got image size %d %d\n", w, h );
    std::string dataL = imageMsgL.data();
    std::string dataR = imageMsgR.data();

    void* vdataL = (void*)dataL.data();
    void* vdataR = (void*)dataR.data();

    viewImage.SetImage(vdataL, w,h, GL_RGB,GL_RGB, GL_UNSIGNED_BYTE);

//    printf("running at %.2fhz\n", 1000.0/TocMS(d0) );
    d0 = Tic();

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

    // Swap frames and Process Events
    pangolin::FinishFrame();

    // Pause for 1/60th of a second.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
  }

  return 0;
}
