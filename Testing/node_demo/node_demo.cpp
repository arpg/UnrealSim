#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <node/node.h>
#include "ImageMessage.pb.h"
#include "InputMessage.pb.h"

int main(int argc, char** argv)
{
  ImageMessage imageMsgL;
  ImageMessage imageMsgR;

  node::node node;
  node.init("node_demo");
  node.subscribe("LeftCamera/Image");
  node.subscribe("RightCamera/Image");
  node.advertise("CarInput");

  while (true)
  {
    bool successL = node.receive("LeftCamera/Image", imageMsgL);
    bool successR = node.receive("RightCamera/Image", imageMsgR);

    if (!successL || !successR)
    {
      InputMessage inputMsg;
      inputMsg.set_ahead(false);
      inputMsg.set_back(false);
      inputMsg.set_left(false);
      inputMsg.set_right(false);
      node.publish("CarInput", inputMsg);

      // exit(0);
      continue;
    }

    int w = imageMsgL.width();
    int h = imageMsgL.height();

    std::string dataL = imageMsgL.data();
    std::string dataR = imageMsgR.data();

    void* vdataL = (void*)dataL.data();
    void* vdataR = (void*)dataR.data();

    cv::Mat image(h, w * 2 + 1, CV_8UC3);
    image.setTo(cv::Scalar(0, 0, 0));

    cv::Mat imageL(h, w, CV_8UC3, vdataL);
    cv::Mat tempL(image, cv::Rect(0, 0, w, h));
    imageL.copyTo(tempL);

    cv::Mat imageR(h, w, CV_8UC3, vdataR);
    cv::Mat tempR(image, cv::Rect(w + 1, 0, w, h));
    imageR.copyTo(tempR);

    cv::cvtColor(image, image, CV_RGB2BGR);
    cv::imshow("Stereo Images", image);
    int key = cv::waitKey(30) & 0xFF;

    InputMessage inputMsg;
    inputMsg.set_ahead(false);
    inputMsg.set_back(false);
    inputMsg.set_left(false);
    inputMsg.set_right(false);

    switch(key)
    {
      case 27:
      case 'q':
      case 'Q':
        exit(0);

      case 'w':
      case 'W':
        inputMsg.set_ahead(true);
        break;

      case 's':
      case 'S':
        inputMsg.set_back(true);
        break;

      case 'a':
      case 'A':
        inputMsg.set_left(true);
        break;

      case 'd':
      case 'D':
        inputMsg.set_right(true);
        break;
    }

    node.publish("CarInput", inputMsg);
  }

  return 0;
}
