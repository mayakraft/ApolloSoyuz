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
    
private:
    ofxTCPServer server;
	ofxTCPClient client;
    
	std::string localIP;
	std::string serverIP;
    
    bool isClient = false;
    bool isServer = false;
    void updateTCP();
    void updateSlowTCP();
    int oneSecond; // track updateSlowTCP()

    void sendMessage(string message);   // if client, send to server.  if server, send to all clients
    string msgTx, msgRx;

};

#endif /* defined(__ApolloSoyuz__ApolloSoyuz__) */
