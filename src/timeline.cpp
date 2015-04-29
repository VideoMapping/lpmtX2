#include "ofApp.h"
#ifdef WITH_TIMELINE
#define BUTTON_HEIGHT 30
//--------------------------------------------------------------
void ofApp::timelineSetup(float duration){

    timeline.setup();
    timeline.setSpacebarTogglePlay(false);
    timeline.setWorkingFolder("timeline/");
    timeline.setDurationInSeconds(duration);
    timeline.setPageName("main"); //changes the first page name
    timeline.addFlags("trigger_main", "timeline/main_trigger.xml");

    for(int i = 0; i < 4; i++)
    {
        timelineAddQuadPage(i);
    }

    loaded = false;
	rendering = false;
    renderFolder = "renders/"; //this is where rendered frames will be saved to

    timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.enableSnapToBPM(120.0);
    timeline.enableSnapToOtherKeyframes(true);
    timeline.setEditableHeaders(true);
    //timeline.collapseAllTracks();
    //initialize the shader
    colorControl.load("shaders/colorcontrol");
    colorControl.begin();
    colorControl.setUniform1i("tex", 0);
    colorControl.end();
    ofAddListener(timeline.events().bangFired, this, &ofApp::timelineTriggerReceived);
}

//--------------------------------------------------------------
void ofApp::timelineUpdate()

{

            for(int j = 0; j < 6; j++)
            {
                if (quads[j].initialized)
                {
                    if(quads[j].bTimelineTint)
                    {
                        quads[j].timelineRed = timeline.getValue("red_"+ofToString(j));
                        quads[j].timelineGreen = timeline.getValue("green_"+ofToString(j));
                        quads[j].timelineBlu = timeline.getValue("blu_"+ofToString(j));
                    }
                    if(quads[j].bTimelineColor)
                    {
                        quads[j].timelineColor = timeline.getColor("color_"+ofToString(j));
                        quads[j].bgColor = quads[j].timelineColor;
                    }
                    if(quads[j].bTimelineAlpha)
                    {
                        quads[j].timelineAlpha = timeline.getValue("alpha_"+ofToString(j));
                    }

                }
            }
}

//--------------------------------------------------------------
void ofApp::timelineTriggerReceived(ofxTLBangEventArgs& trigger){
    //vector<string> triggerParts = ofSplitString(trigger.triggerGroupName, "_", true, true);
    vector<string> triggerParts = ofSplitString(trigger.track->getName(), "_", true, true);

    if(useTimeline)
    {
	//cout << "Trigger from " << trigger.triggerGroupName << " says color " << trigger.triggerName << endl;
	//cout << "Trigger from " << ofToInt(triggerParts[1]) << " says " << trigger.triggerName << endl;

        //string tlMsg = trigger.triggerName;
	string tlMsg = trigger.flag;
        string tlMsgParameter = "";

        if(triggerParts[1] != "main")
        {
        int tlQuad = ofToInt(triggerParts[1]);

        //check if we have a message with a parameter, parameters are given using a colon ':' as separator
        if (ofIsStringInString(tlMsg,":"))
        {
            vector<string> tlMsgParts = ofSplitString(tlMsg, ":", true, true);
            tlMsg = tlMsgParts[0];
            tlMsgParameter = tlMsgParts[1];
        }

	if (tlMsg == "on"){ quads[tlQuad].isOn=true; }
        else if (tlMsg == "off"){ quads[tlQuad].isOn=false; }
	else if(tlMsg == "img_on"){ quads[tlQuad].imgBg=true; }
	else if (tlMsg == "img_off"){ quads[tlQuad].imgBg=false; }
        else if (tlMsg == "col_on"){ quads[tlQuad].colorBg=true; }
        else if (tlMsg == "col_off"){ quads[tlQuad].colorBg=false; }
        else if (tlMsg == "video_on"){ quads[tlQuad].videoBg=true; }
        else if (tlMsg == "video_off"){ quads[tlQuad].videoBg=false; }
        else if (tlMsg == "video_stop"){ quads[tlQuad].video.stop(); }
        else if (tlMsg == "video_play"){ quads[tlQuad].video.play(); }
        else if (tlMsg == "video_reset"){ quads[tlQuad].video.setPosition(0.0); }
        else if (tlMsg == "video_position" && tlMsgParameter != ""){ quads[tlQuad].video.setPosition(ofToFloat(tlMsgParameter));}
        else if (tlMsg == "shared_video_on"){ quads[tlQuad].sharedVideoBg=true; }
        else if (tlMsg == "shared_video_off"){ quads[tlQuad].sharedVideoBg=false; }
        else if (tlMsg == "shared_video_num" && tlMsgParameter != ""){ quads[tlQuad].sharedVideoNum=ofToInt(tlMsgParameter); }
        else if (tlMsg == "slide_on"){ quads[tlQuad].slideshowBg=true; }
        else if (tlMsg == "slide_off"){ quads[tlQuad].slideshowBg=false; }
        else if (tlMsg == "slide_new"){ quads[tlQuad].currentSlide+=1; }
        else if (tlMsg == "slide_num" && tlMsgParameter != ""){ quads[tlQuad].currentSlide=ofToInt(tlMsgParameter); }
        else if (tlMsg == "cam_on"){ quads[tlQuad].camBg=true; }
        else if (tlMsg == "cam_off"){ quads[tlQuad].camBg=false; }
        else if (tlMsg == "cam_num" && tlMsgParameter != ""){ quads[tlQuad].camNumber=ofToInt(tlMsgParameter); cout<< "pass " <<ofToInt(tlMsgParameter)<<endl;}
        else if (tlMsg == "kinect_on"){ quads[tlQuad].kinectBg=true; }
        else if (tlMsg == "kinect_off"){ quads[tlQuad].kinectBg=false; }
        else if (tlMsg == "mask_on"){ quads[tlQuad].bMask=true; }
        else if (tlMsg == "mask_off"){ quads[tlQuad].bMask=false; }
        else if (tlMsg == "mask_invert_on"){ quads[tlQuad].maskInvert=true; }
        else if (tlMsg == "mask_invert_off"){ quads[tlQuad].maskInvert=false; }
        else { cout << "unknown trigger command '" << tlMsg << "' on surface " << tlQuad << endl;}
        }
        else
        {
            // main timeline page
            //check if we have a message with a parameter, parameters are given using a colon ':' as separator
            if (ofIsStringInString(tlMsg,":"))
            {
                vector<string> tlMsgParts = ofSplitString(tlMsg, ":", true, true);
                tlMsg = tlMsgParts[0];
                tlMsgParameter = tlMsgParts[1];
            }

            // check messages
            if (tlMsg == "shared_videos_reset")
            {
                for(int j=0; j<4; j++)
                {
                    if(sharedVideos[j].isLoaded())
                    {
                        sharedVideos[j].setPosition(0.0);
                    }
                }
            }
            else if (tlMsg == "shared_video_reset" && tlMsgParameter != "")
            {
                if(ofToInt(tlMsgParameter) > 0 && ofToInt(tlMsgParameter) <= 4)
                {
                    if(sharedVideos[ofToInt(tlMsgParameter)-1].isLoaded())
                    {
                        sharedVideos[ofToInt(tlMsgParameter)-1].setPosition(0.0);
                    }

                }
            }
            else if (tlMsg == "videos_reset")
            {
                for(int j=0; j<72; j++)
                {
                    if(quads[j].video.isLoaded())
                    {
                        quads[j].video.setPosition(0.0);
                    }
                }
            }
            else if (tlMsg == "videos_stop")
            {
                for(int j=0; j<72; j++)
                {
                    if(quads[j].video.isLoaded())
                    {
                        quads[j].video.stop();
                    }
                }
            }
            else if (tlMsg == "videos_play")
            {
                for(int j=0; j<72; j++)
                {
                    if(quads[j].video.isLoaded())
                    {
                        quads[j].video.play();
                    }
                }
            }


        }
    }
}

//--------------------------------------------------------------
void ofApp::timelineAddQuadPage(int i) {
    timeline.addPage(ofToString(i), true);
    timeline.addCurves("red_"+ofToString(i), ofToString(i)+"timeline/_red.xml", ofRange(0, 1.0));
	timeline.addCurves("green_"+ofToString(i), ofToString(i)+"timeline/_green.xml", ofRange(0, 1.0));
	timeline.addCurves("blu_"+ofToString(i), ofToString(i)+"timeline/_blu.xml", ofRange(0, 1.0));
	timeline.addCurves("alpha_"+ofToString(i), ofToString(i)+"timeline/_alpha.xml", ofRange(0, 1.0));
	timeline.addFlags("trigger_"+ofToString(i), ofToString(i)+"timeline/_trigger.xml");
	timeline.addColors("color_"+ofToString(i), ofToString(i)+"timeline/_color.xml");
	timeline.addCurves("brightness"+ofToString(i), ofToString(i), ofRange(0.0, 2.0), 1.0);
    timeline.addCurves("contrast"+ofToString(i), ofToString(i), ofRange(.5, 2.0), 1.0);
    timeline.addCurves("saturation"+ofToString(i), ofToString(i), ofRange(0.0, 1.5), 1.0);
    timeline.addSwitches("invert"+ofToString(i), ofToString(i));

}

#endif
