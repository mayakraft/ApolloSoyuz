//
//  ApolloSoyuz.cpp
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#include "ApolloSoyuz.h"

void ApolloSoyuz::setup(int r){

    role = r;
    
    networkState = NetworkStateDisconnected;
    animationState = AnimationStateNotStarted;
    
    //	ofSetVerticalSync(true);  // limits update() calls to 60/sec (?)
    
    // NETWORK
    
    if(role == 0){  // server is built into role. TODO- separate these
        server.setup(PORT);
        isServer = true;
        networkState = NetworkStateConnected;
        server.setMessageDelimiter("\n");
    }
    else{  // client
        msgTx	= "";
        msgRx	= "";
        if( client.setup("169.254.153.53", PORT) ){
            isClient = true;
            networkState = NetworkStateConnected;
            client.setMessageDelimiter("\n");
        }
    }
    
    // GRAPHICS
    
    camera.setPosition(0.0f, 0.0f, 0.0f);
    // correlate each window's position in the simulator to a direction in the 3D world
//    if(role == 0)
//        camera.lookAt(ofVec3f(0.0f, 0.0f, 10.0f));
//    else if (role == 1)
//        camera.lookAt(ofVec3f(0.0f, 0.0f, -10.0f));
    camera.lookAt(ofVec3f(0.0f, 0.0f, 10.0f));

    
    if(role == 0 || role == 1 || role == 3){
        ofLoadImage(cloud1, "cloud1.png");
        cloud1.setAnchorPercent(0.5, 0.5);
        ofLoadImage(cloud2, "cloud2.png");
        cloud2.setAnchorPercent(0.5, 0.5);
        ofLoadImage(cloud3, "cloud3.png");
        cloud3.setAnchorPercent(0.5, 0.5);
        ofLoadImage(earthTexture, "earth.png");
        earthTexture.setAnchorPercent(0.5f, 0.5f);
        ofLoadImage(s4bTexture, "s4b.png");
        s4bTexture.setAnchorPercent(0.5f, 0.5f);
        ofLoadImage(soyuzTexture, "soyuz.png");
        soyuzTexture.setAnchorPercent(0.5f, 0.5f);
    }
    
    // sounds
    if(role == 0){
        for(int i = 0; i < NUM_SOUNDS; i++)
            soundsHavePlayed[i] = false;
        gonogo.loadSound("gonogo.mp3");
        tenSeconds.loadSound("10seconds.mp3");
        launch.loadSound("321launch.mp3");
        rollPitch.loadSound("rollpitch.mp3");
        splashDown.loadSound("splashdown.mp3");
        soyuzOrbit.loadSound("soyuzorbit.mp3");
        lookingFine.loadSound("lookingfine.mp3");
        alarmSound.loadSound("alarm.mp3");
        alarmSound.setLoop(true);
        alarmSound.setVolume(.5);
        launchComplete.loadSound("launchcomplete.mp3");
        goForDocking.loadSound("gofordocking.mp3");
        contactSound.loadSound("contact.mp3");
        goForUndock.loadSound("goforundock.mp3");
        deorbitBurn.loadSound("deorbitburn.mp3");
        applause.loadSound("applause.mp3");
        reEntrySound.loadSound("reentry.mp3");
        reEntrySound.setLoop(true);
        docking1Sound.loadSound("docking1.mp3");
    }
    
    if(role == 2){
        cylinder.set(50, 200, 48, 48);
        cylinder.rotate(90, 1, 0, 0);
        icoSphere.setRadius(66);
        cone.set(50, 50, 40, 40);
        cone.rotate(90, 1, 0, 0);
    }
    
    // scene transition times
    sceneTransition[AnimationStateNotStarted] = 0;      // transition by sendMessage "animationBegin"
    sceneTransition[AnimationStateEnteringCapsule] = 36000;
    sceneTransition[AnimationStateReadyForLaunch] = 10000;
    sceneTransition[AnimationStateLaunching] = 16000;
    sceneTransition[AnimationStateSpinAfterLaunch] = 40000;
    sceneTransition[AnimationStateDockingAirlock] = 0;  // transition by sendMessage "airlockDockingSuccessful"
    sceneTransition[AnimationStateOrbiting] = 40000;
    sceneTransition[AnimationStateApproach] = 0;        // transition by sendMessage "soyuzDockingSuccessful"
    sceneTransition[AnimationStateVisit] = 0;           // transition by sendMessage "undockAndReEntry"
    sceneTransition[AnimationStateUndocking] = 14000;
    sceneTransition[AnimationStateReEntry] = 8000;
    sceneTransition[AnimationStateAirFailure] = 0;      // transition by sendMessage "airFailureRepaired"
    sceneTransition[AnimationStateSafeReEntry] = 8000;
    sceneTransition[AnimationStateSplashDown] = 0;      // end of play, reset triggered by sendMessage "animationBegin"
}

void ApolloSoyuz::update(){
    updateTCP();
    
    // if sceneTransition time is 0, transition must be triggered by an event, not time
    if(sceneTransition[animationState] && ofGetElapsedTimeMillis() > sceneBeginTime + sceneTransition[animationState]){
        animationState++;
        sceneBeginTime = ofGetElapsedTimeMillis();
    }
    
    if(role == 0)
        updateSounds();
}

void ApolloSoyuz::draw(){
    
    if(role == 0 || role == 1 || role == 3){
        
        // clear screen
        // blue sky
        if(animationState == AnimationStateNotStarted ||
           animationState == AnimationStateEnteringCapsule ||
           animationState == AnimationStateReadyForLaunch ||
           animationState == AnimationStateSplashDown){
            ofClear(124, 160, 192);
        }
        // re-entry flames
        else if(animationState == AnimationStateReEntry){
            long time = ofGetElapsedTimeMillis() - sceneBeginTime;
            float easeIn = time/8000.0f;
            if(time > 8000)
                easeIn = 1.0;
            int rand = ofRandom(0, 255);
            ofClear(255*easeIn, rand*easeIn, 0);
            if(role == 0)
                reEntrySound.setVolume(easeIn);
        }
        else if (animationState == AnimationStateAirFailure){
            int rand = ofRandom(0, 255);
            ofClear(255, rand, 0);
        }
        else if (animationState == AnimationStateSafeReEntry){
            long time = ofGetElapsedTimeMillis() - sceneBeginTime;
            float easeIn = time/8000.0f;
            if(time > 8000)
                easeIn = 1.0;
            int rand = ofRandom(0, 255);
            int r = 124*easeIn + 255 * (1-easeIn);
            int g = 160*easeIn + rand * (1-easeIn);
            int b = 192*easeIn;
            ofClear(r, g, b);
            if(role == 0)
                reEntrySound.setVolume(1-easeIn);
        }
        // launching fade to black
        else if (animationState == AnimationStateLaunching){
            static int beginDarkening = 7000;
            static int endDarkening = 13000;
            long time = ofGetElapsedTimeMillis() - sceneBeginTime;
            if(time < beginDarkening)
                ofClear(124, 160, 192);
            else if (time > endDarkening)
                ofClear(0, 0, 0);
            else{
                float fraction = (time-beginDarkening)/(float)(endDarkening-beginDarkening);
                ofClear(124*(1.0-fraction), 160*(1.0-fraction), 192*(1.0-fraction));
            }
        }
        // space black
        else{
            ofClear(0, 0, 0);
        }
        
        ofSetColor(255, 255, 255, 255);
        
        camera.begin();
        
        if(animationState == AnimationStateLaunching){
            long time = ofGetElapsedTimeMillis() - sceneBeginTime;
            if(time > 2000 && time < 4000){
                int offset = (3000-time)/4.0;
                if(role == 0){
                    cloud1.draw(10,  offset+100, 60,  50, 30);
                    cloud2.draw(-10, offset-200, 100, 50, 30);
                    cloud3.draw(0,   offset    , 30,  50, 30);
                    cloud1.draw(20,  offset+66,  40,  50, 30);
                    cloud2.draw(-15, offset-66,  90,  50, 30);
                    cloud3.draw(5,  offset-100,  70,  50, 30);
                }
                if(role == 1){
                    cloud1.draw(10,  offset+200, 60,  50, 30);
                    cloud2.draw(-10, offset-100, 30,  50, 30);
                    cloud3.draw(0,   offset    , 100, 50, 30);
                    cloud1.draw(20,  offset+66,  70,  50, 30);
                    cloud2.draw(-15, offset+100, 90,  50, 30);
                    cloud3.draw(5,   offset-33,  70,  50, 30);
                }
            }
        }
        
        if(animationState == AnimationStateSpinAfterLaunch){
            if(role==0){
                long cue = ofGetElapsedTimeMillis() - (sceneBeginTime + 2000);
                if(cue > 0){
                    float offset = cue/150.0f;  // earth & s4b float in from the right
                    ofPushMatrix();
                    ofRotate(-90, 0, 0, 1);
                    earthTexture.draw(0, offset-70, 30, 100, 100);
                    ofPushMatrix();
                    ofScale(-1, 1);
                    s4bTexture.draw(0, offset-70, 25, 25, 25);
                    ofPopMatrix();
                    ofPopMatrix();
                }
            }
            if(role == 1){
                long cue = ofGetElapsedTimeMillis() - (sceneBeginTime + 20000);
                if(cue > 0){
                    float offset = cue/150.0f;  // earth & s4b float in from the right
                    ofPushMatrix();
                    ofRotate(-90, 0, 0, 1);
                    earthTexture.draw(0, offset-70, 30, 100, 100);
                    ofPushMatrix();
                    ofScale(-1, 1);
                    s4bTexture.draw(0, offset-70, 25, 25, 25);
                    ofPopMatrix();
                    ofPopMatrix();
                }
            }
        }
        
        if(animationState == AnimationStateOrbiting){
            if(role==0){
                long cue1 = ofGetElapsedTimeMillis() - (sceneBeginTime + 2000);
                long cue2 = ofGetElapsedTimeMillis() - (sceneBeginTime + 10000);
                long cue3 = ofGetElapsedTimeMillis() - (sceneBeginTime + 25000);
                float cue3Offset = 0;
                if(cue3 > 0)
                    cue3Offset = cue3/200.0f;
                if(cue1 > 0){
                    float offset;
                    if(cue1 < 5000)
                        offset = cue1/200.0f;
                    else
                        offset = 5000/200.0f;
                    ofPushMatrix();
                    ofRotate(-90, 0, 0, 1);
                    earthTexture.draw(0, offset-70+cue3Offset, 30, 100, 100);
                    ofPopMatrix();
                }
                if(cue2 > 0){
                    float size = cue2/1000.0f;
                    ofPushMatrix();
                    ofScale(-1, 1);
                    ofPushMatrix();
                    soyuzTexture.draw(-cue3Offset*.25, 0, 30-size, .1+size/10.0, .1+size/10.0);
                    ofPopMatrix();
                    ofPopMatrix();
                }
            }
        }
        camera.end();
    }
    
    
    if(role == 2){
        ofClear(0, 0, 0);
        ofSetColor(255, 255, 255, 255);
        ofSetLineWidth(1);
        ofPushMatrix();
        ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5);
        ofRotate(90, 0, 0, 1);
       
        if(animationState == AnimationStateDockingAirlock){
            
            drawControls();
            
            long cue1 = ofGetElapsedTimeMillis() - (sceneBeginTime + 4000);
            long cue2 = ofGetElapsedTimeMillis() - (sceneBeginTime + 2000);
            const float STEP = .01;
            if(cue1 < 0){
                ofDrawBitmapString("PREPARE FOR S4B DOCKING",-20, 0);
                if(cue2 > 0)
                    ofDrawBitmapString("   TOUCH THE ARROWS    ", -20, 40);
                position[0] = position[1] = position[2] = 0.0f;
                acceleration[0] = acceleration[1] = acceleration[2] = 0.0f;
                velocity[0] = -0.02f;
                velocity[1] = 0.03f;
                velocity[2] = 0.1f;
                miniGameDockingComplete = false;
            }
            else if(cue1 > 0 && !miniGameDockingComplete){
                printf("%f\n",position[2]);
                if(controlLeft) acceleration[0]=STEP;
                else if(controlRight) acceleration[0]=-STEP;
                else acceleration[0] = 0;
                
                if(controlUp) acceleration[1]=STEP;
                else if(controlDown) acceleration[1]=-STEP;
                else acceleration[1] = 0;
                
                if(controlForward) acceleration[2]=STEP;
                else if(controlReverse) acceleration[2]=-STEP;
                else acceleration[2] = 0;
                velocity[0] += acceleration[0];
                velocity[1] += acceleration[1];
                velocity[2] += acceleration[2];
                if(!miniGameDockingComplete){
                    position[0] += velocity[0];
                    position[1] += velocity[1];
                    position[2] += velocity[2];
                }
                cylinder.setPosition(position[0], position[1], position[2]);
                cylinder.drawWireframe();
                cone.setPosition(position[0], position[1], position[2]);
                cone.drawWireframe();
                ofSetColor(0, 0, 180);
                ofLine(0, -500, 0, 500);
                ofLine(-500, 0, 500, 0);
                ofSetColor(0, 255, 0);
                ofLine(position[0], -50, position[0], 50);
                ofLine(-50, position[1], 50, position[1]);
                ofSetColor(255, 255, 255);
                ofDrawBitmapString("DISTANCE", -150, -250);
                if(position[2] > 0)
                    drawTimer(-250, -250, 1.0-(700-position[2])/700.0);
                if(position[2] > 700 && !miniGameDockingComplete){
                    miniGameDocking1Score = 100 - (fabs(position[0]) + fabs(position[1]));
                    if(miniGameDocking1Score < 0) miniGameDocking1Score = 0;
                    miniGameCompletionTime = ofGetElapsedTimeMillis();
                    miniGameDockingComplete = true;
                }
            }
            
            if(miniGameDockingComplete){
                string scoreString = ofToString(miniGameDocking1Score);
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime ){
                    ofDrawBitmapString("SUCCESS", -10, -100);
                    ofDrawBitmapString(scoreString + " %", -2, 0);
                }
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime + 3000){
                    ofDrawBitmapString("RETRACTING AIRLOCK FROM S4B BOOSTER", -50, 100);
                }
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime + 7000){
                    sendMessage("airlockDockingSuccessful");
                    animationState = AnimationStateOrbiting;
                }
            }
        }
        
        if(animationState == AnimationStateApproach){
            
            drawControls();
            
            long cue1 = ofGetElapsedTimeMillis() - (sceneBeginTime + 4000);
            const float STEP = .01;
            if(cue1 < 0){
                ofDrawBitmapString("PREPARE FOR DOCKING",-20, 0);
                ofDrawBitmapString(" TOUCH THE ARROWS  ", -20, 40);
                position[0] = position[1] = position[2] = 0.0f;
                acceleration[0] = acceleration[1] = acceleration[2] = 0.0f;
                velocity[0] = -0.02f;
                velocity[1] = 0.03f;
                velocity[2] = 0.1f;
                miniGameDockingComplete = false;
            }
            else if(cue1 > 0 && !miniGameDockingComplete){
                printf("%f\n",position[2]);
                if(controlLeft) acceleration[0]=STEP;
                else if(controlRight) acceleration[0]=-STEP;
                else acceleration[0] = 0;
                
                if(controlUp) acceleration[1]=STEP;
                else if(controlDown) acceleration[1]=-STEP;
                else acceleration[1] = 0;
                
                if(controlForward) acceleration[2]=STEP;
                else if(controlReverse) acceleration[2]=-STEP;
                else acceleration[2] = 0;
                velocity[0] += acceleration[0];
                velocity[1] += acceleration[1];
                velocity[2] += acceleration[2];
                if(!miniGameDockingComplete){
                    position[0] += velocity[0];
                    position[1] += velocity[1];
                    position[2] += velocity[2];
                }
                cylinder.setPosition(position[0], position[1], position[2]);
                cylinder.drawWireframe();
                icoSphere.setPosition(position[0], position[1], position[2]);
                icoSphere.drawWireframe();
                ofSetColor(0, 0, 180);
                ofLine(0, -500, 0, 500);
                ofLine(-500, 0, 500, 0);
                ofSetColor(0, 255, 0);
                ofLine(position[0], -50, position[0], 50);
                ofLine(-50, position[1], 50, position[1]);
                ofSetColor(255, 255, 255);
                ofDrawBitmapString("DISTANCE", -10, -250);
                if(position[2] > 0)
                    drawTimer(0, -150, 1.0-(700-position[2])/700.0);
                if(position[2] > 700 && !miniGameDockingComplete){
                    miniGameDocking1Score = 100 - (fabs(position[0]) + fabs(position[1]));
                    if(miniGameDocking1Score < 0) miniGameDocking1Score = 0;
                    miniGameCompletionTime = ofGetElapsedTimeMillis();
                    miniGameDockingComplete = true;
                }
            }
            
            if(miniGameDockingComplete){
                string scoreString = ofToString(miniGameDocking1Score);
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime ){
                    ofDrawBitmapString("SUCCESS", -10, -100);
                    ofDrawBitmapString(scoreString + " %", -2, 0);
                }
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime + 3000){
                    ofDrawBitmapString(" ", -50, 100);
//                    ofDrawBitmapString("", ofGetWidth()*.5, ofGetHeight()*.6);
                }
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime + 7000){
                    sendMessage("soyuzDockingSuccessful");
                    animationState = AnimationStateVisit;
                }
            }
        }
        
        if(animationState == AnimationStateAirFailure){
            
            static long completionTime;
            if(failStage == 0){
                int flash = ofGetElapsedTimeMillis()/500.0;
                if(flash % 2 == 0)
                    ofClear(255, 0, 0);
                else
                    ofClear(0, 0, 0);
                ofDrawBitmapString("TOUCH", -5, 0);
            }
            else{
                ofDrawBitmapString("HURRY- O2 LEVELS FALLING", -15, -250);
                float time = ( ofGetElapsedTimeMillis()-(sceneBeginTime+2000) / 15000);
                if(time < 0) time = 0;
                if(time > 1) time = 1;
                drawControls();
            }
            
            if(failStage == 0){}
            else if(failStage == 1){
                ofDrawBitmapString("DISENGAGE CAPSULE ATMOSPHERE", -20, -40);
                ofDrawBitmapString("(LEFT)", -5, 0);
                if(controlLeft) failStage++;
            }
            else if(failStage == 2){
                ofDrawBitmapString("DISCONNECT THE FUEL TANK", -20, -40);
                ofDrawBitmapString("(RIGHT)", -5, 0);
                if(controlRight) failStage++;
            }
            else if(failStage == 3){
                ofDrawBitmapString("RE-ROUTE AUX OXYGEN TO FRONT", -20, -40);
                ofDrawBitmapString("(DOWN)", -5, 0);
                if(controlDown) failStage++;
            }
            else if(failStage == 4){
                ofDrawBitmapString("JIGGLE MAIN ENGINE PUMP", -20, -40);
                ofDrawBitmapString("(UP)", -5, 0);
                if(controlUp) failStage++;
            }
            else if(failStage == 5){
                ofDrawBitmapString("POWER ON BACKUP AIR PUMP", -20, -40);
                ofDrawBitmapString("(DOWN)", -5, 0);
                if(controlDown) failStage++;
            }
            else if(failStage == 6){
                ofDrawBitmapString("POUND FIST ON CONTROL PANEL", -20, -40);
                ofDrawBitmapString("(RIGHT)", -5, 0);
                if(controlRight) failStage++;
            }
            else if(failStage == 7){
                ofDrawBitmapString("STIR OXYGEN TANKS", -20, -40);
                ofDrawBitmapString("(UP)", -5, 0);
                if(controlUp){
                    failStage++;
                    completionTime = ofGetElapsedTimeMillis();
                }
            }
            else {
                ofDrawBitmapString("SUCCESS ", -10, 0);
                // make sure to tell server to stop alarm sound
                if(ofGetElapsedTimeMillis() > completionTime + 1000)
                    sendMessage("airFailureRepaired");
            }
        }
        
        ofPopMatrix();
    }
    
    // scene location for debugging
    
    if(animationState == AnimationStateEnteringCapsule)
        ofDrawBitmapString("On the launch pad", 20, 20);
//    if(animationState == AnimationStateReadyForLaunch)
//        ofDrawBitmapString("ready for launch", 20,20);
//    else if(animationState == AnimationStateLaunching)
//        ofDrawBitmapString("LAUNCH!", 20,20);
//    else if(animationState == AnimationStateSpinAfterLaunch)
//        ofDrawBitmapString("turn around and dock with the airlock", 20,20);
//    else if(animationState == AnimationStateDockingAirlock)
//        ofDrawBitmapString("docking with the airlock, good luck!", 20,20);
//    else if(animationState == AnimationStateOrbiting)
//        ofDrawBitmapString("waiting for the Russians", 20,20);
//    else if(animationState == AnimationStateApproach)
//        ofDrawBitmapString("docking with the Russians", 20,20);
//    else if(animationState == AnimationStateVisit)
//        ofDrawBitmapString("visit with the Russians", 20,20);
//    else if (animationState == AnimationStateUndocking)
//        ofDrawBitmapString("undocking", 20,20);
//    else if (animationState == AnimationStateReEntry)
//        ofDrawBitmapString("Re-entry", 20,20);
//    else if (animationState == AnimationStateAirFailure)
//        ofDrawBitmapString("PROBLEM", 20,20);
//    else if (animationState == AnimationStateSafeReEntry)
//        ofDrawBitmapString("safe again", 20,20);
//    else if (animationState == AnimationStateSplashDown)
//        ofDrawBitmapString("Splash Down", 20,20);
}

void ApolloSoyuz::drawControls(){
    ofFill();
    if(controlUp) ofSetColor(255, 60, 60);
    else ofSetColor(255, 255, 255);
    ofBeginShape();
    ofVertex(0, -ofGetWidth()*.5+40);  ofVertex(-40,-ofGetWidth()*.5);  ofVertex(40,-ofGetWidth()*.5);
    ofEndShape();
    if(controlDown) ofSetColor(255, 60, 60);
    else ofSetColor(255, 255, 255);
    ofBeginShape();
    ofVertex(0, ofGetWidth()*.5 - 40);  ofVertex(-40, ofGetWidth()*.5);  ofVertex(40, ofGetWidth()*.5);
    ofEndShape();
    if(controlRight) ofSetColor(255, 60, 60);
    else ofSetColor(255, 255, 255);
    ofBeginShape();
    ofVertex(ofGetHeight()*.5 - 40, 0);  ofVertex(ofGetHeight()*.5, -40);  ofVertex(ofGetHeight()*.5, 40);
    ofEndShape();
    if(controlLeft) ofSetColor(255, 60, 60);
    else ofSetColor(255, 255, 255);
    ofBeginShape();
    ofVertex(-ofGetHeight()*.5 + 40, 0);  ofVertex(-ofGetHeight()*.5, -40);  ofVertex(-ofGetHeight()*.5, 40);
    ofEndShape();
    if(controlForward) ofSetColor(255, 60, 60);
    else ofSetColor(255, 255, 255);
    ofDrawBitmapString("FORWARD", ofGetHeight()*.5-80, ofGetWidth()*.5-40);
    if(controlReverse) ofSetColor(255, 60, 60);
    else ofSetColor(255, 255, 255);
    ofDrawBitmapString("REVERSE", -ofGetHeight()*.5+40, ofGetWidth()*.5-40);
    ofNoFill();
}

void ApolloSoyuz::drawTimer(int centerX, int centerY, float roundProgress){
    ofFill();
    ofBeginShape();
    ofVertex(centerX,centerY);
    static float outerRadius = 50;
    static float resolution = 256;
    static float deltaAngle = TWO_PI / resolution;
    float angle = 0;
    for(int i = 0; i <= resolution; i++){
        if((float)i/resolution <= roundProgress){
            float x = centerX + outerRadius * sin(angle);
            float y = centerY + outerRadius * -cos(angle);
            ofVertex(x,y);
            angle += deltaAngle;
        }
    }
    ofVertex(centerX,centerY);
    ofEndShape();
    ofNoFill();
}

void ApolloSoyuz::resetForNewRound(){
    if(role == 0){
        for(int i = 0; i < NUM_SOUNDS; i++)
            soundsHavePlayed[i] = false;
    }
    failStage = 0;
}

void ApolloSoyuz::beginAnimation(){
    sendMessage("animationBegin");
    resetForNewRound();
    animationStartTime = ofGetElapsedTimeMillis();
    animationState = AnimationStateEnteringCapsule;
    sceneBeginTime = ofGetElapsedTimeMillis();
}

void ApolloSoyuz::updateTCP() {
    
	if (isServer){
	    for(int i = 0; i < server.getLastID(); i++) { // getLastID is UID of all clients
            if( server.isClientConnected(i) ) { // check and see if it's still around
                
                // maybe the client is sending something
                string str = server.receive(i);
                //server.send(i, "You sent: "+str);
                
                if (str.length()){
                    msgRx = str;
                	strcpy( cMessage, str.c_str() );
                    ofLogNotice("TCP") << "Server Received (" + ofToString(i) + "):" + str;
                    if (strcmp(cMessage, "goForLaunch") == 0){
                        animationState = AnimationStateReadyForLaunch;
                        sceneBeginTime = ofGetElapsedTimeMillis();
                        sendMessage("goForLaunch");
                    }
                    else if (strcmp(cMessage, "airlockDockingSuccessful") == 0){
                        animationState = AnimationStateOrbiting;
                        sceneBeginTime = ofGetElapsedTimeMillis();
                        sendMessage("airlockDockingSuccessful");
                    }
                    else if (strcmp(cMessage, "soyuzDockingSuccessful") == 0) {
                        animationState = AnimationStateVisit;
                        sceneBeginTime = ofGetElapsedTimeMillis();
                        sendMessage("soyuzDockingSuccessful");
                    }
                    else if (strcmp(cMessage, "undockAndReEntry") == 0) {
                        animationState = AnimationStateUndocking;
                        sceneBeginTime = ofGetElapsedTimeMillis();
                        sendMessage("undockAndReEntry");
                    }
                    else if (strcmp(cMessage, "airFailureRepaired") == 0) {
                        alarmSound.stop();
                        animationState = AnimationStateSafeReEntry;
                        sceneBeginTime = ofGetElapsedTimeMillis();
                        sendMessage("airFailureRepaired");
                    }
                    else{
                    
                    }
                }
            }
	    }
	}
    
    else if (isClient){
        
    	if (!client.isConnected()){
    		client.close();
    		isClient = false;
    		networkState = NetworkStateDisconnected;
    		return;
    	}
        
        string str = client.receive();
        
        if (str.length()){
            msgRx = str;
	    	ofLogNotice("TCP") << "Received From Server: " + str;
            strcpy( cMessage, str.c_str() );
            if (strcmp(cMessage, "animationBegin") == 0) {
                resetForNewRound();
                animationStartTime = ofGetElapsedTimeMillis();
                animationState = AnimationStateEnteringCapsule;
                sceneBeginTime = ofGetElapsedTimeMillis();
            }
            else if (strcmp(cMessage, "goForLaunch") == 0) {
                animationState = AnimationStateReadyForLaunch;
                sceneBeginTime = ofGetElapsedTimeMillis();
            }
            else if (strcmp(cMessage, "airlockDockingSuccessful") == 0) {
                animationState = AnimationStateOrbiting;
                sceneBeginTime = ofGetElapsedTimeMillis();
            }
            else if (strcmp(cMessage, "soyuzDockingSuccessful") == 0) {
                animationState = AnimationStateVisit;
                sceneBeginTime = ofGetElapsedTimeMillis();
            }
            else if (strcmp(cMessage, "undockAndReEntry") == 0) {
                animationState = AnimationStateUndocking;
                sceneBeginTime = ofGetElapsedTimeMillis();
            }
            else if (strcmp(cMessage, "airFailureRepaired") == 0){
                animationState = AnimationStateSafeReEntry;
                sceneBeginTime = ofGetElapsedTimeMillis();
            }
            else {

            }
        }
    }
}

void ApolloSoyuz::updateSounds(){
    if(animationState == AnimationStateEnteringCapsule && !soundsHavePlayed[0])
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 1000){
            gonogo.play();
            soundsHavePlayed[0] = true;
        }
    if(animationState == AnimationStateReadyForLaunch && !soundsHavePlayed[1])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            tenSeconds.play();
            soundsHavePlayed[1] = true;
        }
    if(animationState == AnimationStateReadyForLaunch && !soundsHavePlayed[2])
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 6000){
            launch.play();
            soundsHavePlayed[2] = true;
        }
    if(animationState == AnimationStateSpinAfterLaunch && !soundsHavePlayed[3])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            launchComplete.play();
            soundsHavePlayed[3] = true;
        }
    if(animationState == AnimationStateLaunching && !soundsHavePlayed[4])
        if(ofGetElapsedTimeMillis() > sceneBeginTime+6000){
            lookingFine.play();
            soundsHavePlayed[4] = true;
        }
    if(animationState == AnimationStateOrbiting && !soundsHavePlayed[5])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            soyuzOrbit.play();
            soundsHavePlayed[5] = true;
        }
    if(animationState == AnimationStateOrbiting && !soundsHavePlayed[6])
        if(ofGetElapsedTimeMillis() > sceneBeginTime+26000){
            goForDocking.play();
            soundsHavePlayed[6] = true;
        }
    if(animationState == AnimationStateAirFailure && !soundsHavePlayed[7])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            alarmSound.play();
            soundsHavePlayed[7] = true;
        }
    if(animationState == AnimationStateSplashDown && !soundsHavePlayed[8])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            reEntrySound.stop();
            splashDown.play();
            soundsHavePlayed[8] = true;
        }
    if(animationState == AnimationStateVisit && !soundsHavePlayed[9])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            contactSound.play();
            soundsHavePlayed[9] = true;
        }
    if(animationState == AnimationStateUndocking && !soundsHavePlayed[10])
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 1000){
            goForUndock.play();
            soundsHavePlayed[10] = true;
        }
    if(animationState == AnimationStateUndocking && !soundsHavePlayed[11])
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 8000){
            deorbitBurn.play();
            soundsHavePlayed[11] = true;
        }
    if(animationState == AnimationStateSplashDown && !soundsHavePlayed[12])
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
            applause.play();
            soundsHavePlayed[12] = true;
        }
    if(animationState == AnimationStateReEntry && !soundsHavePlayed[13])
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
            reEntrySound.setVolume(0);
            reEntrySound.play();
            soundsHavePlayed[13] = true;
        }
    if(animationState == AnimationStateDockingAirlock && !soundsHavePlayed[14])
        if(ofGetElapsedTimeMillis() > sceneBeginTime){
            docking1Sound.play();
            soundsHavePlayed[14] = true;
        }
}

void ApolloSoyuz::sendMessage(string message){
	if (isServer){
	    for(int i = 0; i < server.getLastID(); i++){ // getLastID is UID of all clients
            if( server.isClientConnected(i) ) { // check and see if it's still around
                server.send(i,message);
            }
	    }
	}
	else if (isClient){
		client.send(message);
	}
}

void ApolloSoyuz::keyPressed(int key){
    printf("%d\n",key);
    if(key == 32){  // space bar starts animation
        if(isServer && ( animationState == AnimationStateNotStarted || animationState == AnimationStateSplashDown ) ){
            beginAnimation();
        }
    }
    if (key == 102 || key == 70){
        fullScreen = !fullScreen;
        ofSetFullscreen(fullScreen);
    }
}

void ApolloSoyuz::touchDown(ofTouchEventArgs &touch){
    if(role == 2){
        // hardcoded for iPad screen resolution
        const float maxX = 760.0f;
        const float maxY = 1000.0f;
        if(touch.x < 100 && touch.y > 100 && touch.y < maxY-100)
            controlDown = true;
        if(touch.x > maxX-100 && touch.y > 100 && touch.y < maxY-100)
            controlUp = true;
        if(touch.y < 100 && touch.x > 100 && touch.x < maxX-100)
            controlLeft = true;
        if(touch.y > maxY-100 && touch.x > 100 && touch.x < maxX-100)
            controlRight = true;
        if(touch.y > maxY-100 && touch.x < 100)
            controlForward = true;
        if(touch.y < 100 && touch.x < 100)
            controlReverse = true;
        
        printf("up %d  down %d  left %d  right %d  forward %d  reverse %d\n",controlUp, controlDown, controlLeft, controlRight, controlForward, controlReverse);
        
        if(animationState == AnimationStateEnteringCapsule){
//            sendMessage("goForLaunch");
        }
        if(animationState == AnimationStateDockingAirlock){
//            sendMessage("airlockDockingSuccessful");
        }
        if(animationState == AnimationStateApproach){
//            sendMessage("soyuzDockingSuccessful");
        }
        if(animationState == AnimationStateVisit){
            if(controlRight && controlLeft)
                sendMessage("undockAndReEntry");
        }
        if(animationState == AnimationStateAirFailure){
            if(failStage == 0)
                failStage++;
//            sendMessage("airFailureRepaired");
        }
    }
}

void ApolloSoyuz::touchUp(ofTouchEventArgs &touch){
    controlDown = false;
    controlUp = false;
    controlLeft = false;
    controlRight = false;
    controlForward = false;
    controlReverse = false;
}