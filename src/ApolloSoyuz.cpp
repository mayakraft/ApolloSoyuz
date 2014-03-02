//
//  ApolloSoyuz.cpp
//  ApolloSoyuz
//
//  Created by Robby Kraft on 3/1/14.
//
//

#include "ApolloSoyuz.h"

//int ApolloSoyuz::clientConnect(){
//    
//    if (localIP.compare("error") == 0 || !localIP.length()) {
//        ofLogNotice("TCP") << "CLIENT FAILED ! NO IP";
//        return -1;
//    }
//    
//    char serverString[16];
//    std::vector<std::string> result;
//    
//    result = ofSplitString(localIP,".");
//    
//    int index = 0;
//    for (int i = 0; i < 3; i++){
//        for (int j = 0; j < result[i].length(); j++){
//            serverString[index] = localIP.c_str()[index];
//            index++;
//        }
//        
//        serverString[index] = '.';
//        index++;
//    }
//    
//    serverString[index] = '\0';
//    
//    serverIP = std::string(serverString);
//    
//    serverIP += ofToString(loginCode);
//    
//    
//    ofLogNotice("+++ Connecting to server:") << serverIP;
//    
//    
//    if (client.setup (serverIP, PORT)){
//        ofLogNotice("TCP") << "connect to server at " + serverIP + " port: " << ofToString(PORT) << "\n";
//        mainMessage = "";  // "Agent"
//        
//        return 1;
//    }
//    
//    return 0;
//    
//    
//    
//}

void ApolloSoyuz::setup(int role){
//	ofSetVerticalSync(true);
    
    if(role == 0){
        //setup the server to listen on 11999
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
    //for each client lets send them a message letting them know what port they are connected on
	for(int i = 0; i < server.getLastID(); i++){
		if( !server.isClientConnected(i) )continue;
        
		server.send(i, "hello client - you are connected on port - "+ofToString(server.getClientPort(i)) );
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
    

}