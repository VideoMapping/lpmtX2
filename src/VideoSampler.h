#pragma once

#include "ofMain.h"
#include "ofxVideoBuffers.h"

#define NUM_FRAMES 300
#define NUM_BUFFER 4


class VideoSampler{

public:
    VideoSampler();
    virtual ~VideoSampler();
    void setup();
    void setup(int _grabberID, int _grabberHeight, int _grabberWidth, ofPixelFormat _grabberPixelFormat);
    void setup(ofVideoGrabber &_vGrabber, ofPixelFormat _grabberPixelFormat);
    void setup(ofVideoGrabber & _VideoGrabber, ofImageType _samplerPixType);

    void update();
    void draw();


    float getRecordPostion();
    void drawCurrentBuffer(int _x, int _y, int _height, int _width);
    void drawBuffer(int _x, int _y, int _height, int _width, int _BufferNum);
    void clearBuffer();
    void clearBuffer(int bufferNum);


    // Grabber
    ofVideoGrabber * vGrabber;
    int GrabberDeviceID;
    int getGrabberDeviceID ();

    // Buffers
    vector<ofxVideoBuffers *> buffers;
    int NumBuffer=4;
    int currentBufferNum=0;

    // Buffers pixel type
    ofImageType pix_type;

    //Recording
    bool bSetPosition;
    bool bRecLiveInput;
    int recordPosition;

    //Playing
    array <bool, NUM_BUFFER> bPlayBuffer ;// (NUM_BUFFER);
    bool bPlayAnyBuffer;    //increment playhead regerdless of current buffer
    bool bPauseBuffer;
    float playStart;
    float playEnd;
    int bufferSize;
	int	sampleRate;
    int fps;
	float speed;


};
