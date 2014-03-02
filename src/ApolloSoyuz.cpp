//
//  ApolloSoyuz.cpp
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#include "ApolloSoyuz.h"

void ApolloSoyuz::setup(int role){
//	ofSetVerticalSync(true);
    
    programState = ProgramStateDisconnected;
    animationState = AnimationStateNotStarted;
    
    if(role == 0){
        server.setup(PORT);
        isServer = true;
        //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same, default being [/TCP]
        server.setMessageDelimiter("\n");
    }
    else{
        msgTx	= "";
        msgRx	= "";
        if( client.setup("169.254.169.194", PORT) )
            isClient = true;
        //optionally set the delimiter to something else.  The delimter in the client and the server have to be the same
        client.setMessageDelimiter("\n");
        client.setVerbose(true);
    }
}

void ApolloSoyuz::update(){
    updateTCP();
    
    if(animationState == AnimationStateReadyForLaunch){
        if(ofGetElapsedTimeMillis() > animationStartTime + 2000)
            animationState = AnimationStateLaunching;
    }
    else if(animationState == AnimationStateLaunching){
        if(ofGetElapsedTimeMillis() > animationStartTime + 4000)
            animationState = AnimationStateDockingAirlock;
    }
    else if(animationState == AnimationStateDockingAirlock){
        if(ofGetElapsedTimeMillis() > animationStartTime + 6000)
            animationState = AnimationStateOrbiting;
    }
    else if(animationState == AnimationStateOrbiting){
        if(ofGetElapsedTimeMillis() > animationStartTime + 8000)
            animationState = AnimationStateApproach;
    }
    else if(animationState == AnimationStateApproach){
        if(ofGetElapsedTimeMillis() > animationStartTime + 10000)
            animationState = AnimationStateVisit;
    }
    else if(animationState == AnimationStateVisit){
        if(ofGetElapsedTimeMillis() > animationStartTime + 12000)
            animationState = AnimationStateUndocking;
    }
    else if (animationState == AnimationStateUndocking){
        if(ofGetElapsedTimeMillis() > animationStartTime + 14000)
            animationState = AnimationStateReEntry;
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
    
	ofSetHexColor(0xDDDDDD);
	ofDrawBitmapString("TCP SERVER Example \n\nconnect on port: "+ofToString(server.getPort()), 10, 20);
    
	ofSetHexColor(0x000000);
	ofRect(10, 60, ofGetWidth()-24, ofGetHeight() - 65 - 15);
    
	ofSetHexColor(0xDDDDDD);
    
	//for each connected client lets get the data being sent and lets print it to the screen
	for(unsigned int i = 0; i < (unsigned int)server.getLastID(); i++){
        
		if( !server.isClientConnected(i) )continue;
        
		//give each client its own color
		ofSetColor(255 - i*30, 255 - i * 20, 100 + i*40);
        
		//calculate where to draw the text
		int xPos = 15;
		int yPos = 80 + (12 * i * 4);
        
		//get the ip and port of the client
		string port = ofToString( server.getClientPort(i) );
		string ip   = server.getClientIP(i);
		string info = "client "+ofToString(i)+" -connected from "+ip+" on port: "+port;
        
		//we only want to update the text we have recieved there is data
		string str = server.receive(i);
        
		//draw the info text and the received text bellow it
		ofDrawBitmapString(info, xPos, yPos);
		ofDrawBitmapString(str, 25, yPos + 20);
        
	}
    
    if(animationState == AnimationStateReadyForLaunch){
        ofDrawBitmapString("On the launch pad", ofGetWidth()*.5, ofGetHeight()*.5);
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
    printf("%d",key);
    if(key == 103){
        if(isServer && animationState == AnimationStateNotStarted){
            beginAnimation();
        }
    }
}

void ApolloSoyuz::beginAnimation(){
    sendMessage("animationBegin");
    animationStartTime = ofGetElapsedTimeMillis();
    animationState = AnimationStateReadyForLaunch;
}


void ApolloSoyuz::updateTCP() {

//    if(isServer){
//        for(int i = 0; i < server.getLastID(); i++){
//            if( !server.isClientConnected(i) )continue;
//            
//            server.send(i, "hello client - you are connected on port - "+ofToString(server.getClientPort(i)) );
//        }
//    }

	if (isServer){
	    for(int i = 0; i < server.getLastID(); i++) { // getLastID is UID of all clients
            if( server.isClientConnected(i) ) { // check and see if it's still around

                // maybe the client is sending something
                string str = server.receive(i);
                //server.send(i, "You sent: "+str);
                
                if (str.length()){
                	strcpy( cMessage, str.c_str() );
                    ofLogNotice("TCP") << "Server Received (" + ofToString(i) + "):" + str;
                    if (strcmp(cMessage, "command1") == 0){
                    }
                    else if (strcmp(cMessage, "command2") == 0){
                    }
                    else if (strcmp(cMessage, "command3") == 0) {
                    }
                    else if (strcmp(cMessage, "command4") == 0) {
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
	    	ofLogNotice("TCP") << "Received From Server: " + str;
            strcpy( cMessage, str.c_str() );
            if (strcmp(cMessage, "animationBegin") == 0) {
                animationStartTime = ofGetElapsedTimeMillis();
                animationState = AnimationStateReadyForLaunch;
            }
            else if (strcmp(cMessage, "command2") == 0) {
            }
            else if (strcmp(cMessage, "command3") == 0) {
            }
            else if (strcmp(cMessage, "command4") == 0) {
            }
            else {
                //connectedAgents = ofToInt(str);
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
