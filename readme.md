# Apollo-Soyuz Test Project
## Simulator Ride

![ApolloSoyuz](http://robbykraft.com/ASTPscreenshot1.jpg)

### simulating the [Apollo-Soyuz Space Test](http://www.nasa.gov/mission_pages/apollo-soyuz/) mission

### parts list

* 2 desktops/laptops
* 1 iPad
* (optional iPhones/iPod Touches for more capsule windows)
* speaker / amp

![ApolloSoyuz](http://robbykraft.com/ASTPscreenshot2.jpg)

### simulation

* load astronauts into the capsule
* spacebar on the server to start the program
* cosmonauts wait outside for the cue after second docking “contact, we also have capture”, enter the capsule and visit the astronauts. when ready to leave, press two fingers on the iPad, on the left and right side of the screen within 100px of the border to continue the program.

### setup

![ApolloSoyuz](http://robbykraft.com/ApolloSoyuzScreens.png)

##### simulator

* build a blanket fort
* setup the 2 main screens as left and right windows of the Apollo capsule, 180 degree angles from each other
* the iPad will be the control panel somewhere in front of the astronauts
* the other optional screens are directionless, they can go anywhere
* connect the speaker to the server computer

##### computer

* on host computer, under WIFI menu, “Create Network...”
* connect all other devices to this network
* run program on server
* run program on all other devices

##### compiling

* in testApp.m, passing a different integer for setup(int n) is all you need to do to run the different program for the different screens

### built with open frameworks www.openframeworks.cc