#include "VideoSampler.h"

VideoSampler::VideoSampler(){
        bufferSize=512;
        playStart=0;
        playEnd=1.0;
        bPauseBuffer=false;
        fps=30;
}

VideoSampler::~VideoSampler(){

}


void VideoSampler::setup(){
        bufferSize=512;
        playStart=0;
        playEnd=1.0;
        bPauseBuffer=false;
        fps=30;

        //setup grabber
        vGrabber->initGrabber(640,480);
        vGrabber->setVerbose(true);

        for (int i=0;i<NumBuffer; i++){

            buffers.push_back(new ofxVideoBuffers());
            bPlayBuffer[i]=false;

        }
}
//setup internal grabber
void VideoSampler::setup(int _grabberID, int _grabberHeight, int _grabberWidth, ofPixelFormat _grabberPixelFormat){
        bufferSize=512;
        playStart=0;
        playEnd=1.0;
        bPauseBuffer=false;
        fps=30;
        GrabberDeviceID= _grabberID;

        //setup grabber
        vGrabber->setPixelFormat(_grabberPixelFormat);
        vGrabber->setDeviceID(GrabberDeviceID);
        vGrabber->initGrabber(_grabberHeight, _grabberWidth);
        vGrabber->setVerbose(true);

        for (int i=0;i<NumBuffer; i++){

            buffers.push_back(new ofxVideoBuffers());
            bPlayBuffer[i]=false;

        }


}

//setup external grabber
void VideoSampler::setup(ofVideoGrabber & _VideoGrabber, ofPixelFormat _grabberPixelFormat){

        bufferSize=512;
        playStart=0;
        playEnd=1.0;
        bPauseBuffer=false;
        fps=30;

        //setup Buffer
        vGrabber= &_VideoGrabber;
        for (int i=0;i<NumBuffer; i++){

            buffers.push_back(new ofxVideoBuffers());
            bPlayBuffer[i]=false;

        }


}

void VideoSampler::setup(ofVideoGrabber & _VideoGrabber, ofImageType _samplerPixType){

        bufferSize=512;
        playStart=0;
        playEnd=1.0;
        bPauseBuffer=false;
        fps=30;

        //setup Buffer
        vGrabber= &_VideoGrabber;
        pix_type= _samplerPixType;

        for (int i=0;i<NumBuffer; i++){

            buffers.push_back(new ofxVideoBuffers());
            bPlayBuffer[i]=false;

        }

}


//draw function with both grabbers and buffers

void VideoSampler::draw(){

        ofSetColor(255,255,255);

}

void VideoSampler::drawCurrentBuffer(int _x, int _y, int _height, int _width){

    if (buffers.size() != 0){
        buffers[currentBufferNum]->draw(_x , _y, _height, _width);
    }
}

void VideoSampler::drawBuffer(int _x, int _y, int _height, int _width, int _BufferNum){

    if (buffers.size() != 0){
        buffers[_BufferNum]->draw(_x , _y, _height, _width);
    }
    else cout<<"vs drawbuffer null"<<endl;

}

void VideoSampler::update(){

    vGrabber->update();
    if (bRecLiveInput){

            //increment recordPosition
        if (recordPosition<NUM_FRAMES){
                buffers[currentBufferNum]->getNewImage(vGrabber->getPixelsRef(),pix_type);
        }else {

                bRecLiveInput=false;
                bPlayAnyBuffer=true;
                bPlayBuffer[currentBufferNum]=true;
                recordPosition=0;

        }

    }
    else{

        //buffers[currentBufferNum]->stop();
    }
    if (bPlayAnyBuffer ){

        for (int i = 0; i < buffers.size(); i++)
        {
            if (buffers[i]->isFinished())
            {
                buffers[i]->reset();
            }
            if (bPlayBuffer[i])
            {
                buffers[i]->start();
                // we grab frames at 30fps, app is running at 60,
                //so update buffers only once every two frames
                if ((ofGetFrameNum() % 2 == 0)&& !bPauseBuffer)
                {
                    buffers[i]->update();
                }
            }else{
                buffers[i]->stop();
            }
        }
    }else{
        for (int i = 0; i < buffers.size(); i++)
        {
            buffers[i]->stop();
        }
    }

}

float VideoSampler::getRecordPostion(){
    return recordPosition;
}

int VideoSampler::getGrabberDeviceID (){
    return GrabberDeviceID;
}

void VideoSampler::clearBuffer(){
    buffers[currentBufferNum]->clear();
}

void VideoSampler::clearBuffer(int bufferNum){
    buffers[bufferNum]->clear();
}
