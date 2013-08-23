/******************************************************************************\
* Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

//void gluPerspective( GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar );

#include <iostream>
#include "Leap.h"
using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
};

void SampleListener::onInit(const Controller& controller) {
  // std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  // std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  //Note: not dispatched when running in a debugger.
  // std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  // std::cout << "Exited" << std::endl;
}

Vector average;

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  /* std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", fingers: " << frame.fingers().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << std::endl;*/

  if (!frame.hands().empty()) {
    // Get the first hand
    const Hand hand = frame.hands()[0];

    // Check if the hand has any fingers
    const FingerList fingers = hand.fingers();
    if (!fingers.empty()) {
      // Calculate the hand's average finger tip position
      Vector avgPos;
      for (int i = 0; i < fingers.count(); ++i) {
        avgPos += fingers[i].tipPosition();
      }
      avgPos /= (float)fingers.count();
      // std::cout << "Hand has " << fingers.count()
      // << " fingers, average finger tip position" << avgPos << std::endl;
      average = avgPos;
    }

    // Get the hand's sphere radius and palm position
    // std::cout << "Hand sphere radius: " << hand.sphereRadius()
    // << " mm, palm position: " << hand.palmPosition() << std::endl;

    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Calculate the hand's pitch, roll, and yaw angles
    // std::cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
    // << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
    // << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
  }

  // Get gestures
  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        std::string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        std::cout << "Circle id: " << gesture.id()
                  << ", state: " << gesture.state()
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness << std::endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        std::cout << "Swipe id: " << gesture.id()
          << ", state: " << gesture.state()
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() << std::endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        std::cout << "Key Tap id: " << gesture.id()
          << ", state: " << gesture.state()
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<< std::endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        std::cout << "Screen Tap id: " << gesture.id()
        << ", state: " << gesture.state()
        << ", position: " << screentap.position()
        << ", direction: " << screentap.direction()<< std::endl;
        break;
      }
      default:
        std::cout << "Unknown gesture type." << std::endl;
        break;
    }
  }

  if (!frame.hands().empty() || !gestures.empty()) {
    std::cout << std::endl;
  }
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void Display_InitGL()
{
    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    /* Depth buffer setup */
    glClearDepth( 1.0f );

    /* Enables Depth Testing */
    glEnable( GL_DEPTH_TEST );

    /* The Type Of Depth Test To Do */
    glDepthFunc( GL_LEQUAL );

    /* Really Nice Perspective Calculations */
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

/* function to reset our viewport after a window resize */
int Display_SetViewport( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if ( height == 0 ) {
        height = 1;
    }

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    glOrtho(-300, 300, -300, 300, 0, 20);

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );

    return 1;
}

void Display_Render(SDL_Renderer *displayRenderer)
{
    /* Set the background black */
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    /* Clear The Screen And The Depth Buffer */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* Move Left 1.5 Units And Into The Screen 6.0 */
    glLoadIdentity();
    glTranslatef( -1.5f, 0.0f, -6.0f );

    glBegin( GL_TRIANGLES );            /* Drawing Using Triangles */
      glVertex3f(  0.0f,  15.0f, 0.0f ); /* Top */
      glVertex3f( -15.0f, -15.0f, 0.0f ); /* Bottom Left */
      glVertex3f(  15.0f, -15.0f, 0.0f ); /* Bottom Right */
    glEnd( );                           /* Finished Drawing The Triangle */

    /* Move Right 3 Units */
    glTranslatef( average[0], average[1] - 300, 0.0f );

    glBegin( GL_QUADS );                /* Draw A Quad */
      glVertex3f( -15.0f,  15.0f, 0.0f ); /* Top Left */
      glVertex3f(  15.0f,  15.0f, 0.0f ); /* Top Right */
      glVertex3f(  15.0f, -15.0f, 0.0f ); /* Bottom Right */
      glVertex3f( -15.0f, -15.0f, 0.0f ); /* Bottom Left */
    glEnd( );                           /* Done Drawing The Quad */

    SDL_RenderPresent(displayRenderer);
}

int main() {
  SampleListener listener;
  Controller controller;

  controller.addListener(listener);
 
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* displayWindow;
  SDL_Renderer* displayRenderer;
  SDL_RendererInfo displayRendererInfo;
  SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &displayWindow, &displayRenderer);
  SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
  /*TODO: Check that we have OpenGL */
  if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 || 
      (displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
      /*TODO: Handle this. We have no render surface and not accelerated. */
  }
  
  Display_InitGL();
  
  Display_SetViewport(800, 600);
  
  std::cout << "Press Escpae to quit..." << std::endl;

  bool quit = false;
  SDL_Event event;
  while(!quit) {
    Display_Render(displayRenderer);
    usleep(10000);
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
      }
    }
  }

  controller.removeListener(listener);

  SDL_Quit();
  
  return 0;
}
