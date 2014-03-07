//
//  ApolloSoyuz.h
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#ifndef __ApolloSoyuz__ApolloSoyuz__
#define __ApolloSoyuz__ApolloSoyuz__

#include <iostream>
#include "ofMain.h"
#include "ofxNetwork.h"

typedef enum
{
    AnimationStateNotStarted,
    AnimationStateEnteringCapsule,
    AnimationStateReadyForLaunch,
    AnimationStateLaunching,
    AnimationStateSpinAfterLaunch,
    AnimationStateDockingAirlock,
    AnimationStateOrbiting,
    AnimationStateApproach,
    AnimationStateVisit,
    AnimationStateUndocking,
    AnimationStateReEntry,
    AnimationStateAirFailure,
    AnimationStateSafeReEntry,
    AnimationStateSplashDown
}
AnimationState;

typedef enum
{
    NetworkStateDisconnected,
    NetworkStateConnected
}
NetworkState;


#define PORT 11999


class ApolloSoyuz {

// roles:
// 0: server, left window  (desktop)
// 1: client, right window  (desktop)
// 2: client, front panel (iPad)
// 3: client, aux window (iPod/iPhone)
    
public:

    void setup(int role);
    void update();
    void draw();
    
    // DESKTOP
    void keyPressed(int key);
    
    // MOBILE
    void touchDown(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    
private:
    
    // NETWORK

    NetworkState networkState;
    ofxTCPServer server;
	ofxTCPClient client;
    bool isClient = false;
    bool isServer = false;
    void updateTCP();
    void sendMessage(string message);   // if client, send to server.  if server, send to all clients
    string msgTx;
    string msgRx;  // last message received
    char cMessage[128];
    
    // GRAPHICS
    
    // roles 0, 1, 3
    ofCamera camera;
    ofTexture groundTexture;
    ofTexture earthTexture;
    ofTexture s4bTexture;
    ofTexture soyuzTexture;
    ofTexture cloud1, cloud2, cloud3;
    bool fullScreen = false;
    // role 2
    ofCylinderPrimitive cylinder;
    ofIcoSpherePrimitive icoSphere;
    ofConePrimitive cone;
    
    // SCRIPT
    
    AnimationState animationState;
    int role;
    long sceneTransition[14];
    long animationStartTime;
    long sceneBeginTime;
    void beginAnimation();
    void resetForNewRound();
    
    // MINIGAMES
    
    bool controlLeft, controlRight, controlUp, controlDown, controlForward, controlReverse;
    float position[3];
    float velocity[3];
    float acceleration[3];
    bool miniGameDockingComplete;
    int miniGameDocking1Score;
    long miniGameCompletionTime;
    void drawControls();
    void drawTimer(int centerX, int centerY, float roundProgress);
    int failStage;
    
    // SOUNDS
    
    void updateSounds();
    ofSoundPlayer gonogo;
    ofSoundPlayer tenSeconds;
    ofSoundPlayer launch;
    ofSoundPlayer rollPitch;
    ofSoundPlayer splashDown;
    ofSoundPlayer soyuzOrbit;
    ofSoundPlayer lookingFine;
    ofSoundPlayer alarmSound;
    ofSoundPlayer launchComplete;
    ofSoundPlayer goForDocking;
    ofSoundPlayer contactSound;
    ofSoundPlayer goForUndock;
    ofSoundPlayer deorbitBurn;
    ofSoundPlayer applause;
    ofSoundPlayer reEntrySound;
    ofSoundPlayer docking1Sound;
    const static int NUM_SOUNDS = 20;
    bool soundsHavePlayed[NUM_SOUNDS];
};

#endif /* defined(__ApolloSoyuz__ApolloSoyuz__) */
