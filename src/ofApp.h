#ifndef _OF_APP
#define _OF_APP

#include "config.h"

#define OF_ADDON_USING_OFXXMLSETTINGS

//#define WINDOW_W 800
//#define WINDOW_H 600

// OSC stuff - listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

#include "ofMain.h"

#include "quad.h"
#ifdef WITH_KINECT
#include "kinectManager.h"
#endif
#include "ofxXmlSettings.h"
#include "ofxMostPixelsEver.h"
#include "ofxOsc.h"
#ifdef WITH_TIMELINE
#include "ofxTimeline.h"
#endif
#include "ofxSimpleGuiToo.h"
#ifdef WITH_SYPHON
#include "ofxSyphon.h"
#endif
#ifdef WITH_MIDI
#include "ofxMidi.h"
//#include "ofxFenster.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
#endif

//#include <GL/glut.h>
#ifdef WITH_MIDI
class ofApp : public ofBaseApp, public ofxMidiListener
#else
class ofApp : public ofBaseApp
#endif
{

public:

    int WINDOW_W;
    int WINDOW_H;

    void setup();
    void update();
    void draw();
    void prepare();
    void dostuff();
    void exit();

    void mpeSetup();
    void resync();
    void startProjection();
    void stopProjection();
    void copyQuadSettings(int sourceQuad);


    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();
    void windowResized(int w, int h);
    void drawGrid(float x, float y);


    int whichCorner;

    ofTrueTypeFont ttf;

    quad quads[72];
    int layers[72];

    int activeQuad;
    int nOfQuads;
    int borderColor;
    int copyQuadNum;
    bool loaded;
    bool rendering;
    int currentRenderFrame;
    void renderCurrentFrame();

    string renderFolder;

    bool bdrawGrid;
    bool configOk;
    bool autoStart;

    bool isSetup;
    bool bFullscreen;
    bool bGameMode;
    bool bGui;
    bool bTimeline;
    bool snapshotOn;
    bool bStarted;
    bool bSplash;
    bool bSplash2;
    bool maskSetup;
    bool gridSetup;
    bool bCameraOk;
    bool bSnapOn;

    ofImage splashImg;
    ofImage splashImg2;
    float splashTime;

/*    //3dmodel
    ofxAssimpModelLoader model;
    bool bAnimate;
    bool bAnimateMouse;
    float animationPosition;


    ofMesh mesh;
    ofLight	light;
*/
    vector<ofxAssimpModelLoader> models;
    vector<string> modelFiles;
    // use of MostPixelsEver
    bool bMpe;
    mpeClientTCP client;
    void mpeFrameEvent(ofxMPEEventArgs& event);
    void mpeMessageEvent(ofxMPEEventArgs& event);
    void mpeResetEvent(ofxMPEEventArgs& event);
    float lastFrameTime;

    // OSC stuff
    ofxOscReceiver	receiver;
    int				current_msg_string;
    string		msg_strings[NUM_MSG_STRINGS];
    float			timers[NUM_MSG_STRINGS];
    void parseOsc();
    float oscControlMin;
    float oscControlMax;
    vector<ofxOscMessage> oscHotkeyMessages;
    vector<int> oscHotkeyKeys;

    // MIDI stuff
    #ifdef WITH_MIDI
    void newMidiMessage(ofxMidiMessage& eventArgs);
    ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
	vector<ofxMidiMessage> midiHotkeyMessages;
	vector<int> midiHotkeyKeys;
	#endif

    // Shaders
    ofShader edgeBlendShader;
    ofShader quadMaskShader;
    ofShader chromaShader;
    //ofShader noiseShader;
    //ofShader brickShader;
    //ofShader EnvBMShader;

    //ofxAssimpModelLoader obj;

    // gui elements
    bool showGui;
    bool bImageLoad;
    bool bVideoLoad;
    bool bAnimaLoad;
    bool bSharedVideoLoad0;
    bool bSharedVideoLoad1;
    bool bSharedVideoLoad2;
    bool bSharedVideoLoad3;
    bool bSharedVideoLoad4;
    bool bSharedVideoLoad5;
    bool bSharedVideoLoad6;
    bool bSharedVideoLoad7;
    bool bSharedVideoLoad8;
    bool bQuadReset;
    bool bQuadBezierSpherize;
    bool bQuadBezierSpherizeStrong;
    bool bQuadBezierReset;
    bool bMidiHotkeyCoupling;
    bool bMidiHotkeyLearning;
    int midiHotkeyPressed;

    void openImageFile();
    void openVideoFile();
    void openAnimaFile();
    void openSharedVideoFile(int i);
    void openSharedVideoFile(string path, int i);
    void quadDimensionsReset(int q);
    void quadPlacementReset(int q);
    void quadBezierSpherize(int q);
    void quadBezierSpherizeStrong(int q);
    void quadBezierReset(int q);

    float totRotationAngle;
    ofPolyline rotationSector;


    // camera grabber
    //ofVideoGrabber camGrabber;
    ofTexture camTexture;
    ofTexture snapshotTexture;
    int numOfCams;
	vector<ofVideoGrabber> cameras;
	vector<string> cameraIDs;

    vector<ofVideoPlayer> sharedVideos;
    vector<string> sharedVideosFiles;

    int camID;
    int reqCamWidth;
    int reqCamHeight;
    int camWidth;
    int camHeight;

    vector<string> imgFiles;
    vector<string> videoFiles;
    vector<string> slideshowFolders;
    ofxXmlSettings XML;
    void setXml();
    void getXml(string xmlFile);
    string originalFileExtension;

    // double-click stuff
    unsigned long doubleclickTime;
    unsigned long lastTap;

    void activateQuad(int x, int y);

    ofPoint startDrag;


    // snapshot loading
    ofImage loadImageFromFile();

    string loadSlideshow();
    bool bSlideshowLoad;

    #ifdef WITH_KINECT
    kinectManager kinect;
    bool bKinectOk;
    bool bCloseKinect;
    bool bOpenKinect;
    #endif

    // timeline
    #ifdef WITH_TIMELINE
    ofxTimeline timeline;
    //brightness,contrast,saturation shader found here: http://mouaif.wordpress.com/2009/01/05/photoshop-math-with-glsl-shaders/
    ofShader colorControl;
    ofFbo frameBuffer;
    //ofxAssimpModelLoader models;

    ofRectangle contentRectangle; //matched to video size
    ofRectangle outputRectangle;
    ofRectangle loadVideoButton;
    ofRectangle renderButton;

    float timelineDurationSeconds;
    float timelinePreviousDuration;
    void timelineSetup(float duration);
    void timelineUpdate();
    void timelineAddQuadPage(int i);
    void timelineTriggerReceived(ofxTLBangEventArgs& trigger);
    bool useTimeline;\


    #endif

    // syphon
    #ifdef WITH_SYPHON
	ofxSyphonClient syphClient;
    #endif


};

#endif

