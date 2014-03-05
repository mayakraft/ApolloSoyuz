//
//  ApolloSoyuz.cpp
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#include "ApolloSoyuz.h"

void ApolloSoyuz::setup(int r){
    //	ofSetVerticalSync(true);  // limits update() calls to 60/sec (?)
    
    programState = ProgramStateDisconnected;
    animationState = AnimationStateNotStarted;
    
    role = r;
    
    // NETWORK
    
    if(role == 0){  // server is built into role. TODO- separate these
        server.setup(PORT);
        isServer = true;
        //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/TCP]
        server.setMessageDelimiter("\n");
    }
    else{  // client
        msgTx	= "";
        msgRx	= "";
        if( client.setup("169.254.153.53", PORT) )
            isClient = true;
        //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
        client.setMessageDelimiter("\n");
    }
    
    // GRAPHICS
    
    if(role == 2){
        cylinder.set(50, 200, 10, 10);
        cylinder.rotate(90, 1, 0, 0);
    }
    
    if(role == 0 || role == 1 || role == 4){
        
        //        ofEnableDepthTest();
        //        ofEnableAlphaBlending();
        
        camera.setPosition(0.0f, 0.0f, 0.0f);
        if(role == 0)
            camera.lookAt(ofVec3f(0.0f, 0.0f, 10.0f));
        else if (role == 1)
            camera.lookAt(ofVec3f(0.0f, 0.0f, -10.0f));
        
        skyTexture.setAnchorPercent(0.5, 0.5);
        
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
        
        //        cloudPlane.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
        //        cloudPlane.rotate(90, 0, 1, 0);
        
        //        glEnable(GL_CULL_FACE);
        //        glCullFace(GL_FRONT);
        
        //        ofDisableAlphaBlending();
        
    }
    
}

void ApolloSoyuz::draw(){
    
    //    if(isServer){
    //        ofClear(0, 0, 255);
    //    }
    //    if(isClient){
    //        ofClear(0, 255, 0);
    //    }
    
    //    ofEnableNormalizedTexCoords();  // is this it?
    
    if(role == 0 || role == 1 || role == 3){
        
        if(animationState == AnimationStateNotStarted ||
           animationState == AnimationStateEnteringCapsule ||
           animationState == AnimationStateReadyForLaunch){
            ofClear(124, 160, 192);
        }
        else if (animationState == AnimationStateLaunching){
            static int beginDarkening = 5000;
            static int endDarkening = 8000;
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
                    cloud2.draw(-15,  offset-66,  90,  50, 30);
                    cloud3.draw(5,  offset-100,  70,  50, 30);
                }
                if(role == 1){
                    cloud1.draw(10,  offset+200, 60,  50, 30);
                    cloud2.draw(-10, offset-100, 30, 50, 30);
                    cloud3.draw(0,   offset    , 100,  50, 30);
                    cloud1.draw(20,  offset+66,  70,  50, 30);
                    cloud2.draw(-15,  offset+100,  90,  50, 30);
                    cloud3.draw(5,  offset-33,  70,  50, 30);
                }
                
            }
        }
        
        if(animationState == AnimationStateSpinAfterLaunch){
            long cue1 = ofGetElapsedTimeMillis() - (sceneBeginTime + 2000);
            long cue2 = ofGetElapsedTimeMillis() - (sceneBeginTime + 20000);
            if(cue1 > 0){
                float offset = cue1/200.0f;
                if(role==0){
                    ofPushMatrix();
                    ofRotate(-90, 0, 0, 1);
                    earthTexture.draw(0, offset-60, 30, 100, 100);
                    ofPushMatrix();
                    ofScale(-1, 1);
                    //ofPushMatrix();
                    //ofRotate(offset/10.0, 0, 0, 1);
                    s4bTexture.draw(0, offset-70, 25, 25, 25);
                    //ofPopMatrix();
                    ofPopMatrix();
                    ofPopMatrix();
                }
            }
            if(cue2 > 0){
                float offset = cue2/200.0f;
                if(role == 1){
                    ofPushMatrix();
                    ofRotate(-90, 0, 0, 1);
                    earthTexture.draw(0, offset-60, 30, 100, 100);
                    ofPushMatrix();
                    ofScale(-1, 1);
                    //ofPushMatrix();
                    //ofRotate(offset/10.0, 0, 0, 1);
                    s4bTexture.draw(0, offset-70, 25, 25, 25);
                    //ofPopMatrix();
                    ofPopMatrix();
                    ofPopMatrix();
                }
            }
        }
        
        if(animationState == AnimationStateOrbiting){
            static int soyuzApproachSpin = 0;
            long cue1 = ofGetElapsedTimeMillis() - (sceneBeginTime + 2000);
//            long cue2 = ofGetElapsedTimeMillis() - (sceneBeginTime + 20000);
            if(cue1 > 0){
                float offset;
                if(cue1 < 5000)
                    offset = cue1/200.0f;
                else
                    offset = 5000/200.0f;
                if(role==0){
                    ofPushMatrix();
                    ofRotate(-90, 0, 0, 1);
                    earthTexture.draw(0, offset-60, 30, 100, 100);
                    ofPushMatrix();
                    ofScale(-1, 1);
                    //ofPushMatrix();
                    //ofRotate(offset/10.0, 0, 0, 1);
//                    s4bTexture.draw(0, offset-70, 25, 25, 25);
                    //ofPopMatrix();
                    ofPopMatrix();
                    ofPopMatrix();
                }
            }
//            if(cue2 > 0){
//                float offset = cue2/200.0f;
//                if(role == 1){
//                    ofPushMatrix();
//                    ofRotate(-90, 0, 0, 1);
//                    earthTexture.draw(0, offset-60, 30, 100, 100);
//                    ofPushMatrix();
//                    ofScale(-1, 1);
//                    //ofPushMatrix();
//                    //ofRotate(offset/10.0, 0, 0, 1);
//                    s4bTexture.draw(0, offset-70, 25, 25, 25);
//                    //ofPopMatrix();
//                    ofPopMatrix();
//                    ofPopMatrix();
//                }
//            }
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
        
        if(animationState == AnimationStateEnteringCapsule){
            ofDrawBitmapString("Astronauts, are you GO for launch?", ofGetWidth()*.5, ofGetHeight()*.25);
            ofDrawBitmapString("touch to confirm", ofGetWidth()*.5, ofGetHeight()*.33);
        }
        
        if(animationState == AnimationStateDockingAirlock){
            
            drawControls();
            
            long cue1 = ofGetElapsedTimeMillis() - (sceneBeginTime + 2000);
            const float STEP = .01;
            if(cue1 < 0){
                ofDrawBitmapString("PREPARE FOR S4B DOCKING",-20, 0);
                ofDrawBitmapString("   TOUCH THE ARROWS    ", -20, 40);
                position[0] = position[1] = position[2] = 0.0f;
                acceleration[0] = acceleration[1] = acceleration[2] = 0.0f;
                velocity[0] = -0.02f;
                velocity[1] = 0.03f;
                velocity[2] = 0.1f;
                miniGameDocking1Complete = false;
            }
            else if(cue1 > 0 && !miniGameDocking1Complete){
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
                if(!miniGameDocking1Complete){
                    position[0] += velocity[0];
                    position[1] += velocity[1];
                    position[2] += velocity[2];
                }
                cylinder.setPosition(position[0], position[1], position[2]);
                cylinder.drawWireframe();
                ofDrawBitmapString("DISTANCE", -10, -200);
                if(position[2] > 0)
                    drawTimer(0, -150, 1.0-(700-position[2])/700.0);
                //                if(cue1 < 15000)
                //                    drawTimer(0, -150, cue1/15000.0f);
                if(position[2] > 700 && !miniGameDocking1Complete){
                    miniGameDocking1Score = 100 - (fabs(position[0]) + fabs(position[1]));
                    if(miniGameDocking1Score < 0) miniGameDocking1Score = 0;
                    miniGameCompletionTime = ofGetElapsedTimeMillis();
                    miniGameDocking1Complete = true;
                }
            }
            
            if(miniGameDocking1Complete){
                string scoreString = ofToString(miniGameDocking1Score);
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime ){
                    ofDrawBitmapString("SUCCESS", -10, -100);
                    ofDrawBitmapString(scoreString + " %", -2, 0);
                }
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime + 3000){
                    ofDrawBitmapString("RETRACTING AIRLOCK FROM S4B BOOSTER", -50, 100);
                    //                    ofDrawBitmapString("", ofGetWidth()*.5, ofGetHeight()*.6);
                }
                if(ofGetElapsedTimeMillis() > miniGameCompletionTime + 7000){
                    sendMessage("airlockDockingSuccessful");
                    animationState = AnimationStateOrbiting;
                }
            }
        }
        
        
        
        ofPopMatrix();
    }
    
    if(animationState == AnimationStateEnteringCapsule){
        ofDrawBitmapString("On the launch pad", 20, 20);
    }
    if(animationState == AnimationStateReadyForLaunch){
        ofDrawBitmapString("ready for launch", 20,20);
    }
    else if(animationState == AnimationStateLaunching){
        ofDrawBitmapString("LAUNCH!", 20,20);
    }
    else if(animationState == AnimationStateSpinAfterLaunch){
        ofDrawBitmapString("turn around and dock with the airlock", 20,20);
    }
    else if(animationState == AnimationStateDockingAirlock){
        ofDrawBitmapString("docking with the airlock, good luck!", 20,20);
    }
    else if(animationState == AnimationStateOrbiting){
        ofDrawBitmapString("waiting for the Russians", 20,20);
    }
    else if(animationState == AnimationStateApproach){
        ofDrawBitmapString("docking with the Russians", 20,20);
    }
    else if(animationState == AnimationStateVisit){
        ofDrawBitmapString("visit with the Russians", 20,20);
    }
    else if (animationState == AnimationStateUndocking){
        ofDrawBitmapString("undocking", 20,20);
    }
    else if (animationState == AnimationStateReEntry){
        ofDrawBitmapString("Re-entry", 20,20);
    }
    
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


void ApolloSoyuz::update(){
    updateTCP();
    //    float angle = ofGetElapsedTimeMillis()/1000.0;
    //    camera.lookAt(ofVec3f(cos(angle), 0.0f, sin(angle)));
    //    skySphere.rotate(angle*2, 0, 0, 1);
    //
    //    cloudPlane.rotate(angle*2, 0, 1, 0);
    
    // TODO scene increment can iterate over AnimationStates programmatically, instead of long form
    if(animationState == AnimationStateEnteringCapsule){
        
        // transition by sendMessage "goForLaunch"
        
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 2000){
            animationState = AnimationStateReadyForLaunch;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if(animationState == AnimationStateReadyForLaunch){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
            animationState = AnimationStateLaunching;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if(animationState == AnimationStateLaunching){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 10000){
            animationState = AnimationStateSpinAfterLaunch;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    
    else if(animationState == AnimationStateSpinAfterLaunch){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){  // 40000
            animationState = AnimationStateDockingAirlock;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if(animationState == AnimationStateDockingAirlock){
        // transition by sendMessage "airlockDockingSuccessful"
    }
    else if(animationState == AnimationStateOrbiting){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 40000){
            animationState = AnimationStateApproach;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if(animationState == AnimationStateApproach){
        // transition by sendMessage "soyuzDockingSuccessful"
    }
    else if(animationState == AnimationStateVisit){
        // transition by sendMessage "undockAndReEntry"
    }
    else if (animationState == AnimationStateUndocking){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
            animationState = AnimationStateReEntry;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if (animationState == AnimationStateReEntry){
    }
}

void ApolloSoyuz::keyPressed(int key){
    printf("%d\n",key);
    if(key == 32){  // space bar starts animation
        if(isServer && animationState == AnimationStateNotStarted){
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
            sendMessage("goForLaunch");
        }
        if(animationState == AnimationStateDockingAirlock){
            //            sendMessage("airlockDockingSuccessful");
        }
        if(animationState == AnimationStateApproach){
            sendMessage("soyuzDockingSuccessful");
        }
        if(animationState == AnimationStateVisit){
            sendMessage("undockAndReEntry");
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

void ApolloSoyuz::beginAnimation(){
    sendMessage("animationBegin");
    animationStartTime = ofGetElapsedTimeMillis();
    animationState = AnimationStateEnteringCapsule;
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
                    else{
                    }
                }
            }
	    }
	}
    
    //    else if(isClient){
    //        if(client.send(msgTx)){
    //
    //            //if data has been sent lets update our text
    //            string str = tcpClient.receive();
    //            if( str.length() > 0 ){
    //                msgRx = str;
    //            }
    //        }else if(!tcpClient.isConnected()){
    //            weConnected = false;
    //        }
    //    }else{
    //        //if we are not connected lets try and reconnect every 5 seconds
    //        deltaTime = ofGetElapsedTimeMillis() - connectTime;
    //
    //        if( deltaTime > 5000 ){
    //            weConnected = tcpClient.setup("127.0.0.1", 11999);
    //            connectTime = ofGetElapsedTimeMillis();
    //        }
    //
    //    }
    
    else if (isClient){
        
    	if (!client.isConnected()){
    		client.close();
    		isClient = false;
    		programState = ProgramStateDisconnected;
    		return;
    	}
        //        else{
        //            //if we are not connected lets try and reconnect every 5 seconds
        //            deltaTime = ofGetElapsedTimeMillis() - connectTime;
        //            if( deltaTime > 5000 ){
        //                weConnected = tcpClient.setup("127.0.0.1", 11999);
        //                connectTime = ofGetElapsedTimeMillis();
        //            }
        //        }
        
        string str = client.receive();
        
        if (str.length()){
            msgRx = str;
	    	ofLogNotice("TCP") << "Received From Server: " + str;
            strcpy( cMessage, str.c_str() );
            if (strcmp(cMessage, "animationBegin") == 0) {
                animationStartTime = ofGetElapsedTimeMillis();
                animationState = AnimationStateEnteringCapsule;
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
            else {
                //connections = ofToInt(str);
            }
        }
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


