//
//  ApolloSoyuz.h
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#ifndef __ApolloSoyuz__ApolloSoyuz__
#define __ApolloSoyuz__ApolloSoyuz__

#define PORT 11999

typedef enum
{
    AnimationStateNotStarted,
    AnimationStateEnteringCapsule,
    AnimationStateReadyForLaunch,
    AnimationStateLaunching,
    AnimationStateDockingAirlock,
    AnimationStateOrbiting,
    AnimationStateApproach,
    AnimationStateVisit,
    AnimationStateUndocking,
    AnimationStateReEntry
}
AnimationState;

typedef enum
{
    ProgramStateDisconnected,
    ProgramStateWaiting,
    ProgramStateRunning
}
ProgramState;

#include <iostream>
#include "ofMain.h"
#include "ofxNetwork.h"

class ApolloSoyuz {
    
public:
    // roles:
    // 0: server, left window  (desktop)
    // 1: client, right window  (desktop)
    // 2: client, front panel (iPad)
    // 3: client, side panel 1 (iPod/iPhone)
    // 4: client, side panel 2 (iPod/iPhone)

    void setup(int role);
    void update();
    void draw();

// desktop
    void keyPressed(int key);
//    void keyReleased(int key);
//    void windowResized(int w, int h);
// desktop
    
// mobile
    void touchDown(ofTouchEventArgs & touch);
//    void touchMoved(ofTouchEventArgs & touch);
//    void touchUp(ofTouchEventArgs & touch);
//    void touchDoubleTap(ofTouchEventArgs & touch);
//    void touchCancelled(ofTouchEventArgs & touch);
// mobile
private:
    ofxTCPServer server;
	ofxTCPClient client;
    
    bool isClient = false;
    bool isServer = false;

    void updateTCP();
    void updateSlowTCP();
    int oneSecond; // track updateSlowTCP()

    void sendMessage(string message);   // if client, send to server.  if server, send to all clients
    string msgTx;
    string msgRx;  // last message received
    char cMessage[128];
    
    // script
    
    ProgramState programState;
    AnimationState animationState;
    
    long animationStartTime;
    long sceneBeginTime;
    int role;
    
    void beginAnimation();
    
    // desktop only
    bool fullScreen = false;

};

#endif /* defined(__ApolloSoyuz__ApolloSoyuz__) */
