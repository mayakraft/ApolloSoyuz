//
//  ApolloSoyuz.cpp
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#include "ApolloSoyuz.h"

void ApolloSoyuz::setup(int r){
	ofSetVerticalSync(true);  // limits update() calls to 60/sec (?)
    
    programState = ProgramStateDisconnected;
    animationState = AnimationStateNotStarted;
    
    role = r;
    
    if(role == 0){  // server is built into role. TODO- separate these
        server.setup(PORT);
        isServer = true;
        //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/TCP]
        server.setMessageDelimiter("\n");
    }
    else{  // client
        msgTx	= "";
        msgRx	= "";
        if( client.setup("169.254.254.27", PORT) )
            isClient = true;
        //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
        client.setMessageDelimiter("\n");
        client.setVerbose(true);
    }
}

void ApolloSoyuz::update(){
    updateTCP();
    
    // TODO scene increment can iterate over AnimationStates programmatically, instead of long form
    if(animationState == AnimationStateEnteringCapsule){
        // transition by sendMessage "goForLaunch"
    }
    else if(animationState == AnimationStateReadyForLaunch){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
            animationState = AnimationStateLaunching;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if(animationState == AnimationStateLaunching){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
            animationState = AnimationStateDockingAirlock;
            sceneBeginTime = ofGetElapsedTimeMillis();
        }
    }
    else if(animationState == AnimationStateDockingAirlock){
        // transition by sendMessage "airlockDockingSuccessful"
    }
    else if(animationState == AnimationStateOrbiting){
        if(ofGetElapsedTimeMillis() > sceneBeginTime + 4000){
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

void ApolloSoyuz::draw(){
    
    if(isServer){
        ofClear(0, 0, 255);
    }
    if(isClient){
        ofClear(0, 255, 0);
    }
    
    if(role == 2){
        if(animationState == AnimationStateEnteringCapsule){
            ofDrawBitmapString("Astronauts, are you GO for launch?", ofGetWidth()*.5, ofGetHeight()*.25);
            ofDrawBitmapString("touch to confirm", ofGetWidth()*.5, ofGetHeight()*.33);
        }
    }

    if(animationState == AnimationStateEnteringCapsule){
        ofDrawBitmapString("On the launch pad", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    if(animationState == AnimationStateReadyForLaunch){
        ofDrawBitmapString("ready for launch", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if(animationState == AnimationStateLaunching){
        ofDrawBitmapString("LAUNCH!", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if(animationState == AnimationStateDockingAirlock){
        ofDrawBitmapString("turn around and dock with the airlock", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if(animationState == AnimationStateOrbiting){
        ofDrawBitmapString("waiting for the Russians", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if(animationState == AnimationStateApproach){
        ofDrawBitmapString("docking with the Russians", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if(animationState == AnimationStateVisit){
        ofDrawBitmapString("visit with the Russians", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if (animationState == AnimationStateUndocking){
        ofDrawBitmapString("undocking", ofGetWidth()*.5, ofGetHeight()*.5);
    }
    else if (animationState == AnimationStateReEntry){
        ofDrawBitmapString("Re-entry", ofGetWidth()*.5, ofGetHeight()*.5);
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
        if(animationState == AnimationStateEnteringCapsule){
            sendMessage("goForLaunch");
        }
        if(animationState == AnimationStateDockingAirlock){
            sendMessage("airlockDockingSuccessful");
        }
        if(animationState == AnimationStateApproach){
            sendMessage("soyuzDockingSuccessful");
        }
        if(animationState == AnimationStateVisit){
            sendMessage("undockAndReEntry");
        }
    }
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


