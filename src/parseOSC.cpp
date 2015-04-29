#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::parseOsc()
{
    // get the next message
    ofxOscMessage m;
    receiver.getNextMessage( &m );
    //cout << endl << "OSC message '" << m.getAddress() << " args " << m.getArgAsFloat(0) << "  " << m.getArgAsFloat(1) << " numargs " << m.getNumArgs() << endl;


    // check for quads corner x movements
    if ( m.getAddress() == "/corners/x" )
    {
        // arguments are iif
        int osc_quad = m.getArgAsInt32( 0 );
        int osc_corner = m.getArgAsInt32( 1 );
        float osc_coord = m.getArgAsFloat( 2 );
        quads[osc_quad].corners[osc_corner].x = osc_coord;
    }
    // check for quads corner y movements
    else if ( m.getAddress() == "/corners/y" )
    {
        // arguments are iif
        int osc_quad = m.getArgAsInt32( 0 );
        int osc_corner = m.getArgAsInt32( 1 );
        float osc_coord = m.getArgAsFloat( 2 );
        quads[osc_quad].corners[osc_corner].y = osc_coord;
    }

    // check for active quad corner x movements
    if ( m.getAddress() == "/active/corners/0" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
        float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[0].x = osc_coord_x;
        quads[activeQuad].corners[0].y = osc_coord_y;
    }

    if ( m.getAddress() == "/active/corners/1" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
        float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[1].x = osc_coord_x;
        quads[activeQuad].corners[1].y = osc_coord_y;
    }

    if ( m.getAddress() == "/active/corners/2" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
        float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[2].x = osc_coord_x;
        quads[activeQuad].corners[2].y = osc_coord_y;
    }

    if ( m.getAddress() == "/active/corners/3" )
    {
        // arguments are ff
        float osc_coord_x = m.getArgAsFloat( 0 );
        float osc_coord_y = m.getArgAsFloat( 1 );
        quads[activeQuad].corners[3].x = osc_coord_x;
        quads[activeQuad].corners[3].y = osc_coord_y;
    }


    // resync
    else if ( m.getAddress() == "/projection/resync" )
    {
        resync();
    }

    // stop
    else if ( m.getAddress() == "/projection/stop" )

    {
        if (isSetup)
        {
            isSetup = False;
            gui.hide();
            bGui = False;
            bdrawGrid = False;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isSetup = False;
                }
            }
        }
        else
        {
            isSetup = True;
            gui.hide();
            bGui = False;
            bdrawGrid = False;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isSetup = True;
                }
            }
        }
    }

    /*
    {
        stopProjection();
    }

    // start
    else if ( m.getAddress() == "/projection/start" )
    {
        startProjection();
    }*/

    // save
    else if ( m.getAddress() == "/projection/save" )
    {
        setXml();
        XML.saveFile("_lpmt_settings.xml");
        cout<<"saved settings to data/_lpmt_settings.xml"<<endl;
    }
    else if ( m.getAddress() == "/projection/savefile" )
    {
        ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower(originalFileExtension), "Save as - XML Project");
            if(saveFileResult.bSuccess)
            {
                //ofImage img;
                setXml();
                string fileName = saveFileResult.getName();
                string filePath = saveFileResult.getPath();
                XML.saveFile(filePath);
                gui.setPage((activeQuad*4)+2);
            }
    }


    // load
    else if ( m.getAddress() == "/projection/load" )
    {
        getXml("_lpmt_settings.xml");
        gui.setPage((activeQuad*4)+2);
    }

    else if ( m.getAddress() == "/projection/loadfile" )
    {
        ofFileDialogResult dialog_result = ofSystemLoadDialog("Load XML Project", false);
            if(dialog_result.bSuccess)
            {
                ofImage img;
                string fileName = dialog_result.getName();
                string filePath = dialog_result.getPath();
                getXml(filePath);
                gui.setPage((activeQuad*4)+2);
            }
    }

    // toggle fullscreen
    else if ( m.getAddress() == "/projection/fullscreen/toggle" )
    {
        bFullscreen = !bFullscreen;

        if(!bFullscreen)
        {
            ofSetWindowShape(WINDOW_W, WINDOW_H);
            ofSetFullscreen(false);
            // figure out how to put the window in the center:
            int screenW = ofGetScreenWidth();
            int screenH = ofGetScreenHeight();
            ofSetWindowPosition(screenW/2-WINDOW_W/2, screenH/2-WINDOW_H/2);
        }
        else if(bFullscreen == 1)
        {
            ofSetFullscreen(true);
        }
    }

    else if ( m.getAddress() == "/projection/fullscreen/on" )
    {
        bFullscreen = true;
        ofSetFullscreen(true);
    }

    else if ( m.getAddress() == "/projection/fullscreen/off" )
    {
        bFullscreen = false;
        ofSetWindowShape(WINDOW_W, WINDOW_H);
        ofSetFullscreen(false);
        // figure out how to put the window in the center:
        int screenW = ofGetScreenWidth();
        int screenH = ofGetScreenHeight();
        ofSetWindowPosition(screenW/2-WINDOW_W/2, screenH/2-WINDOW_H/2);
    }

    // toggle gui
    else if ( m.getAddress() == "/projection/gui/toggle" )
    {
        gui.toggleDraw();
        bGui = !bGui;
    }

    else if ( m.getAddress() == "/projection/mode/masksetup/toggle" )
    {
        if (!bGui)
        {
            maskSetup = !maskSetup;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isMaskSetup = !quads[i].isMaskSetup;
                }
            }
        }
    }

    else if ( m.getAddress() == "/projection/mode/masksetup/on" )
    {
        if (!bGui)
        {
            maskSetup = true;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isMaskSetup = true;
                }
            }
        }
    }

    else if ( m.getAddress() == "/projection/mode/masksetup/off" )
    {
        if (!bGui)
        {
            maskSetup = false;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isMaskSetup = false;
                }
            }
        }
    }

    // toggle setup
    else if ( m.getAddress() == "/projection/mode/setup/toggle" )
    {
        if (isSetup)
        {
            isSetup = False;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isSetup = False;
                }
            }
        }
        else
        {
            isSetup = True;
            for(int i = 0; i < 72; i++)
            {
                if (quads[i].initialized)
                {
                    quads[i].isSetup = True;
                }
            }
        }
    }

    else if ( m.getAddress() == "/projection/mode/setup/on" )
    {
        isSetup = True;
        for(int i = 0; i < 72; i++)
        {
            if (quads[i].initialized)
            {
                quads[i].isSetup = True;
            }
        }
    }

    else if ( m.getAddress() == "/projection/mode/setup/off" )
    {
        isSetup = False;
        for(int i = 0; i < 72; i++)
        {
            if (quads[i].initialized)
            {
                quads[i].isSetup = False;
            }
        }
    }

    // connects to mpe server
    else if ( m.getAddress() == "/projection/mpe/connect" )
    {
        mpeSetup();
    }


    // timeline stuff
#ifdef WITH_TIMELINE
    // use toggle
    else if ( m.getAddress() == "/projection/timeline/toggle" )
    {
        // no argument
        useTimeline = !useTimeline;
    }

    // use
    else if ( m.getAddress() == "/projection/timeline/use" )
    {
        // argument is int32
        int osc_timeline = m.getArgAsInt32( 0 );
        if(osc_timeline == 0)
        {
            useTimeline = false;
        }
        else if(osc_timeline == 1)
        {
            useTimeline = true;
        }
    }

    // timeline duration in seconds
    else if ( m.getAddress() == "/projection/timeline/duration" )
    {
        // argument is float
        float osc_timelineDurationSeconds = m.getArgAsFloat( 0 );
        if(osc_timelineDurationSeconds >= 10.0)
        {
            timelineDurationSeconds =osc_timelineDurationSeconds;
        }
    }

    else if ( m.getAddress() == "/projection/timeline/start")
    {
        timeline.togglePlay();
    }

    else if ( m.getAddress() == "/projection/timeline/show")
    {
        bTimeline = !bTimeline;
        timeline.toggleShow();
        if(bTimeline)
        {
            timeline.enable();
            gui.hide();
            bGui = false;
        }
        else
        {
            timeline.disable();
        }
    }

#endif

//------------------------------------------------------
// active quad stuff
//------------------------------------------------------

    // change active quad
    else if ( m.getAddress() == "/active/set" )
    {
        // argument is int32
        int osc_activequad = m.getArgAsInt32( 0 );
        if (osc_activequad <= nOfQuads-1)
        {
            quads[activeQuad].isActive = False;
            activeQuad = osc_activequad;
            quads[activeQuad].isActive = True;
            gui.setPage((activeQuad*4)+2);
        }
    }

    // on/off
    else if ( m.getAddress() == "/active/show" )
    {
        // argument is int32
        int osc_quad_isOn = m.getArgAsInt32( 0 );
        if(osc_quad_isOn == 0)
        {
            quads[activeQuad].isOn = false;
        }
        else if(osc_quad_isOn == 1)
        {
            quads[activeQuad].isOn = true;
        }
    }

    // use timeline color
    else if ( m.getAddress() == "/active/timeline/color" )
    {
        // argument is int32
        int osc_quad_bTimelineColor = m.getArgAsInt32( 0 );
        if(osc_quad_bTimelineColor == 0)
        {
            quads[activeQuad].bTimelineColor = false;
        }
        else if(osc_quad_bTimelineColor == 1)
        {
            quads[activeQuad].bTimelineColor = true;
        }
    }

    // use timeline color
    else if ( m.getAddress() == "/active/timeline/alpha" )
    {
        // argument is int32
        int osc_quad_bTimelineAlpha = m.getArgAsInt32( 0 );
        if(osc_quad_bTimelineAlpha == 0)
        {
            quads[activeQuad].bTimelineAlpha = false;
        }
        else if(osc_quad_bTimelineAlpha == 1)
        {
            quads[activeQuad].bTimelineAlpha = true;
        }
    }

    // use timeline for slides
    else if ( m.getAddress() == "/active/timeline/slides" )
    {
        // argument is int32
        int osc_quad_bTimelineSlideChange = m.getArgAsInt32( 0 );
        if(osc_quad_bTimelineSlideChange == 0)
        {
            quads[activeQuad].bTimelineSlideChange = false;
        }
        else if(osc_quad_bTimelineSlideChange == 1)
        {
            quads[activeQuad].bTimelineSlideChange = true;
        }
    }

    else if ( m.getAddress() == "/active/timeline/tint" )
    {
        // argument is int32
        int osc_quad_bTimelineTint = m.getArgAsInt32( 0 );
        if(osc_quad_bTimelineTint == 0)
        {
            quads[activeQuad].bTimelineTint = false;
        }
        else if(osc_quad_bTimelineTint == 1)
        {
            quads[activeQuad].bTimelineTint = true;
        }
    }

    // img stuff on active quad
    else if ( m.getAddress() == "/active/img" )
    {
        quads[activeQuad].imgBg = !quads[activeQuad].imgBg;
    }

    // img on/off
    else if ( m.getAddress() == "/active/img/show" )
    {
        // argument is int32
        int osc_quad_imgBg = m.getArgAsInt32( 0 );
        if(osc_quad_imgBg == 0)
        {
            quads[activeQuad].imgBg = false;
        }
        else if(osc_quad_imgBg == 1)
        {
            quads[activeQuad].imgBg = true;
        }
    }

    // img load
    else if ( m.getAddress() == "/active/img/load" )
    {
        // no argument
        openImageFile();
    }

    // img HFlip
    else if ( m.getAddress() == "/active/img/hmirror" )
    {
        // argument is int32
        int osc_quad_imgHFlip = m.getArgAsInt32( 0 );
        if(osc_quad_imgHFlip == 0)
        {
            quads[activeQuad].imgHFlip = false;
        }
        else if(osc_quad_imgHFlip == 1)
        {
            quads[activeQuad].imgHFlip = true;
        }
    }

    // img VFlip
    else if ( m.getAddress() == "/active/img/vmirror" )
    {
        // argument is int32
        int osc_quad_imgVFlip = m.getArgAsInt32( 0 );
        if(osc_quad_imgVFlip == 0)
        {
            quads[activeQuad].imgVFlip = false;
        }
        else if(osc_quad_imgVFlip == 1)
        {
            quads[activeQuad].imgVFlip = true;
        }
    }
    else if ( m.getAddress() == "/active/img/greenscreen" )
    {
        // argument is int32
        int osc_quad_imgGreenscreen = m.getArgAsInt32( 0 );
        if(osc_quad_imgGreenscreen == 0)
        {
            quads[activeQuad].imgBgGreenscreen = false;
        }
        else if(osc_quad_imgGreenscreen == 1)
        {
            quads[activeQuad].imgBgGreenscreen = true;
        }
    }

    else if ( m.getAddress() == "/active/img/color" )
    {
        // arguments are ffff
        float img_color_r = m.getArgAsFloat( 0 );
        float img_color_g = m.getArgAsFloat( 1 );
        float img_color_b = m.getArgAsFloat( 2 );
        float img_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].imgColorize.r = img_color_r;
        quads[activeQuad].imgColorize.g = img_color_g;
        quads[activeQuad].imgColorize.b = img_color_b;
        quads[activeQuad].imgColorize.a = img_color_a;
    }

    else if ( m.getAddress() == "/active/img/color/1" )
    {
        // arguments are f
        float img_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.r = img_color_r;
    }

    else if ( m.getAddress() == "/active/img/color/2" )
    {
        // arguments are f
        float img_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.g = img_color_g;
    }

    else if ( m.getAddress() == "/active/img/color/3" )
    {
        // arguments are f
        float img_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.b = img_color_b;
    }

    else if ( m.getAddress() == "/active/img/color/4" )
    {
        // arguments are f
        float img_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].imgColorize.a = img_color_a;
    }

    else if ( m.getAddress() == "/active/img/mult/x" )
    {
        // arguments are f
        float img_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].imgMultX = img_mult_x;
    }

    else if ( m.getAddress() == "/active/img/mult/y" )
    {
        // arguments are f
        float img_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].imgMultY = img_mult_y;
    }


    // blendModes on/off
    else if ( m.getAddress() == "/active/blendmodes/show" )
    {
        // argument is int32
        int osc_quad_bBlendModes = m.getArgAsInt32( 0 );
        if(osc_quad_bBlendModes == 0)
        {
            quads[activeQuad].bBlendModes = false;
        }
        else if(osc_quad_bBlendModes == 1)
        {
            quads[activeQuad].bBlendModes = true;
        }
    }

    // blendModes mode
    else if ( m.getAddress() == "/active/blendmodes/mode" )
    {
        // argument is int32
        int osc_quad_blendMode = m.getArgAsInt32( 0 );
        if(osc_quad_blendMode < 4)
        {
            quads[activeQuad].blendMode = osc_quad_blendMode;
        }
    }

    // solid color stuff
    else if ( m.getAddress() == "/active/solid" )
    {
        quads[activeQuad].colorBg = !quads[activeQuad].colorBg;
    }

    // solid on/off
    else if ( m.getAddress() == "/active/solid/show" )
    {
        // argument is int32
        int osc_quad_colorBg = m.getArgAsInt32( 0 );
        if(osc_quad_colorBg == 0)
        {
            quads[activeQuad].colorBg = false;
        }
        else if(osc_quad_colorBg == 1)
        {
            quads[activeQuad].colorBg = true;
        }
    }

    else if ( m.getAddress() == "/active/solid/color" )
    {
        // arguments are ffff
        float solid_color_r = m.getArgAsFloat( 0 );
        float solid_color_g = m.getArgAsFloat( 1 );
        float solid_color_b = m.getArgAsFloat( 2 );
        float solid_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].bgColor.r = solid_color_r;
        quads[activeQuad].bgColor.g = solid_color_g;
        quads[activeQuad].bgColor.b = solid_color_b;
        quads[activeQuad].bgColor.a = solid_color_a;
    }

    else if ( m.getAddress() == "/active/solid/color/1" )
    {
        // arguments are f
        float solid_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.r = solid_color_r;
    }

    else if ( m.getAddress() == "/active/solid/color/2" )
    {
        // arguments are f
        float solid_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.g = solid_color_g;
    }

    else if ( m.getAddress() == "/active/solid/color/3" )
    {
        // arguments are f
        float solid_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.b = solid_color_b;
    }

    else if ( m.getAddress() == "/active/solid/color/4" )
    {
        // arguments are f
        float solid_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].bgColor.a = solid_color_a;
    }

    else if ( m.getAddress() == "/active/solid/trans" )
    {
        quads[activeQuad].transBg = !quads[activeQuad].transBg;
    }

    // trans on/off
    else if ( m.getAddress() == "/active/solid/trans/show" )
    {
        // argument is int32
        int osc_quad_transBg = m.getArgAsInt32( 0 );
        if(osc_quad_transBg == 0)
        {
            quads[activeQuad].transBg = false;
        }
        else if(osc_quad_transBg == 1)
        {
            quads[activeQuad].transBg = true;
        }
    }

    else if ( m.getAddress() == "/active/solid/trans/color" )
    {
        // arguments are ffff
        float trans_color_r = m.getArgAsFloat( 0 );
        float trans_color_g = m.getArgAsFloat( 1 );
        float trans_color_b = m.getArgAsFloat( 2 );
        float trans_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].secondColor.r = trans_color_r;
        quads[activeQuad].secondColor.g = trans_color_g;
        quads[activeQuad].secondColor.b = trans_color_b;
        quads[activeQuad].secondColor.a = trans_color_a;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/1" )
    {
        // arguments are f
        float trans_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.r = trans_color_r;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/2" )
    {
        // arguments are f
        float trans_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.g = trans_color_g;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/3" )
    {
        // arguments are f
        float trans_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.b = trans_color_b;
    }

    else if ( m.getAddress() == "/active/solid/trans/color/4" )
    {
        // arguments are f
        float trans_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].secondColor.a = trans_color_a;
    }

    else if ( m.getAddress() == "/active/solid/trans/duration" )
    {
        // arguments are f
        float trans_duration = m.getArgAsFloat( 0 );
        quads[activeQuad].transDuration = trans_duration;
    }


    // mask stuff on active quad
    else if ( m.getAddress() == "/active/mask" )
    {
        quads[activeQuad].bMask = !quads[activeQuad].bMask;
    }

    // mask on/off
    else if ( m.getAddress() == "/active/mask/show" )
    {
        // argument is int32
        int osc_quad_bMask = m.getArgAsInt32( 0 );
        if(osc_quad_bMask == 0)
        {
            quads[activeQuad].bMask = false;
        }
        else if(osc_quad_bMask == 1)
        {
            quads[activeQuad].bMask = true;
        }
    }

    // mask invert
    else if ( m.getAddress() == "/active/mask/invert" )
    {
        // argument is int32
        int osc_quad_maskInvert = m.getArgAsInt32( 0 );
        if(osc_quad_maskInvert == 0)
        {
            quads[activeQuad].maskInvert = false;
        }
        else if(osc_quad_maskInvert == 1)
        {
            quads[activeQuad].maskInvert = true;
        }
    }

    // deform stuff

    // deform on/off
    else if ( m.getAddress() == "/active/deform/show" )
    {
        // argument is int32
        int osc_quad_bDeform = m.getArgAsInt32( 0 );
        if(osc_quad_bDeform == 0)
        {
            quads[activeQuad].bDeform = false;
        }
        else if(osc_quad_bDeform == 1)
        {
            quads[activeQuad].bDeform = true;
        }
    }

    //deform edit
    else if ( m.getAddress() == "/active/deform/edit" )
    {
        // argument is int32
        if (!bGui)
            {
                gridSetup = !gridSetup;
                for(int i = 0; i < 72; i++)
                {
                    if (quads[i].initialized)
                    {
                        quads[i].isBezierSetup = !quads[i].isBezierSetup;
                    }
                }
            }
    }

    // deform bezier
    else if ( m.getAddress() == "/active/deform/bezier" )
    {
        // argument is int32
        int osc_quad_bBezier = m.getArgAsInt32( 0 );
        if(osc_quad_bBezier == 0)
        {
            quads[activeQuad].bBezier = false;
        }
        else if(osc_quad_bBezier == 1)
        {
            quads[activeQuad].bBezier = true;
        }
    }

    else if ( m.getAddress() == "/active/deform/bezier/spherize/light" )
    {
        // no argument
        quadBezierSpherize(activeQuad);
    }

    else if ( m.getAddress() == "/active/deform/bezier/spherize/strong" )
    {
        // no argument
        quadBezierSpherizeStrong(activeQuad);
    }

    else if ( m.getAddress() == "/active/deform/bezier/reset" )
    {
        // no argument
        quadBezierReset(activeQuad);
    }

    // deform grid
    else if ( m.getAddress() == "/active/deform/grid" )
    {
        // argument is int32
        int osc_quad_bGrid = m.getArgAsInt32( 0 );
        if(osc_quad_bGrid == 0)
        {
            quads[activeQuad].bGrid = false;
        }
        else if(osc_quad_bGrid == 1)
        {
            quads[activeQuad].bGrid = true;
        }
    }

    // deform grid rows
    else if ( m.getAddress() == "/active/deform/grid/rows" )
    {
        // argument is int32
        int osc_quad_gridRows = m.getArgAsInt32( 0 );
        if(osc_quad_gridRows >= 2 && osc_quad_gridRows <= 15)
        {
            quads[activeQuad].gridRows = osc_quad_gridRows;
        }
    }

    // deform grid columns
    else if ( m.getAddress() == "/active/deform/grid/columns" )
    {
        // argument is int32
        int osc_quad_gridColumns = m.getArgAsInt32( 0 );
        if(osc_quad_gridColumns >= 2 && osc_quad_gridColumns <= 20)
        {
            quads[activeQuad].gridColumns = osc_quad_gridColumns;
        }
    }

    // edge-blend stuff
    // edge-blend on/off
    else if ( m.getAddress() == "/active/edgeblend/show" )
    {
        // argument is int32
        int osc_quad_edgeBlendBool = m.getArgAsInt32( 0 );
        if(osc_quad_edgeBlendBool == 0)
        {
            quads[activeQuad].edgeBlendBool = false;
        }
        else if(osc_quad_edgeBlendBool == 1)
        {
            quads[activeQuad].edgeBlendBool = true;
        }
    }

    else if ( m.getAddress() == "/active/edgeblend/power" )
    {
        // argument is float
        float osc_quad_edgeBlendExponent = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendExponent = osc_quad_edgeBlendExponent;
    }

    else if ( m.getAddress() == "/active/edgeblend/gamma" )
    {
        // argument is float
        float osc_quad_edgeBlendGamma = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendGamma = osc_quad_edgeBlendGamma;
    }

    else if ( m.getAddress() == "/active/edgeblend/luminance" )
    {
        // argument is float
        float osc_quad_edgeBlendLuminance = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendLuminance = osc_quad_edgeBlendLuminance;
    }

    else if ( m.getAddress() == "/active/edgeblend/amount/left" )
    {
        // argument is float
        float osc_quad_edgeBlendAmountSin = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendAmountSin = osc_quad_edgeBlendAmountSin;
    }

    else if ( m.getAddress() == "/active/edgeblend/amount/right" )
    {
        // argument is float
        float osc_quad_edgeBlendAmountDx = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendAmountDx = osc_quad_edgeBlendAmountDx;
    }

    else if ( m.getAddress() == "/active/edgeblend/amount/top" )
    {
        // argument is float
        float osc_quad_edgeBlendAmountTop = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendAmountTop = osc_quad_edgeBlendAmountTop;
    }

    else if ( m.getAddress() == "/active/edgeblend/amount/bottom" )
    {
        // argument is float
        float osc_quad_edgeBlendAmountBottom = m.getArgAsFloat( 0 );
        quads[activeQuad].edgeBlendAmountBottom = osc_quad_edgeBlendAmountBottom;
    }

    else if ( m.getAddress() == "/active/edgeblend/amount" )
    {
        // argument is ffff
        float osc_quad_edgeBlendAmountTop = m.getArgAsFloat( 0 );
        float osc_quad_edgeBlendAmountDx = m.getArgAsFloat( 1 );
        float osc_quad_edgeBlendAmountBottom = m.getArgAsFloat( 2 );
        float osc_quad_edgeBlendAmountSin = m.getArgAsFloat( 3 );
        quads[activeQuad].edgeBlendAmountTop = osc_quad_edgeBlendAmountTop;
        quads[activeQuad].edgeBlendAmountDx = osc_quad_edgeBlendAmountDx;
        quads[activeQuad].edgeBlendAmountBottom = osc_quad_edgeBlendAmountBottom;
        quads[activeQuad].edgeBlendAmountSin = osc_quad_edgeBlendAmountSin;
    }

    // content displacement
    // displacement X
    else if ( m.getAddress() == "/active/placement/x" )
    {
        // argument is int32
        int osc_quad_quadDispX = m.getArgAsInt32( 0 );
        quads[activeQuad].quadDispX = osc_quad_quadDispX;
    }

    // displacement Y
    else if ( m.getAddress() == "/active/placement/y" )
    {
        // argument is int32
        int osc_quad_quadDispY = m.getArgAsInt32( 0 );
        quads[activeQuad].quadDispY = osc_quad_quadDispY;
    }

    // displacement XY
    else if ( m.getAddress() == "/active/placement" )
    {
        // argument is int32 int32
        int osc_quad_quadDispX = m.getArgAsInt32( 0 );
        int osc_quad_quadDispY = m.getArgAsInt32( 1 );
        quads[activeQuad].quadDispX = osc_quad_quadDispX;
        quads[activeQuad].quadDispY = osc_quad_quadDispY;
    }

    // displacement W
    else if ( m.getAddress() == "/active/placement/w" )
    {
        // argument is int32
        int osc_quad_quadW = m.getArgAsInt32( 0 );
        quads[activeQuad].quadW = osc_quad_quadW;
    }

    // displacement H
    else if ( m.getAddress() == "/active/placement/h" )
    {
        // argument is int32
        int osc_quad_quadH = m.getArgAsInt32( 0 );
        quads[activeQuad].quadH = osc_quad_quadH;
    }

    // displacement WH
    else if ( m.getAddress() == "/active/placement/dimensions" )
    {
        // argument is int32 int32
        int osc_quad_quadW = m.getArgAsInt32( 0 );
        int osc_quad_quadH = m.getArgAsInt32( 1 );
        quads[activeQuad].quadW = osc_quad_quadW;
        quads[activeQuad].quadH = osc_quad_quadH;
    }

    // displacement reset
    else if ( m.getAddress() == "/active/placement/reset" )
    {
        // no argument
        quadDimensionsReset(activeQuad);
        quadPlacementReset(activeQuad);
    }

    // video stuff on active quad
    else if ( m.getAddress() == "/active/video" )
    {
        quads[activeQuad].videoBg = !quads[activeQuad].videoBg;
    }

    // video on/off
    else if ( m.getAddress() == "/active/video/show" )
    {
        // argument is int32
        int osc_quad_videoBg = m.getArgAsInt32( 0 );
        if(osc_quad_videoBg == 0)
        {
            quads[activeQuad].videoBg = false;
        }
        else if(osc_quad_videoBg == 1)
        {
            quads[activeQuad].videoBg = true;
        }
    }

    // video load
    else if ( m.getAddress() == "/active/video/load" )
    {
        // no argument
        openVideoFile();
    }

    // video HFlip
    else if ( m.getAddress() == "/active/video/hmirror" )
    {
        // argument is int32
        int osc_quad_videoHFlip = m.getArgAsInt32( 0 );
        if(osc_quad_videoHFlip == 0)
        {
            quads[activeQuad].videoHFlip = false;
        }
        else if(osc_quad_videoHFlip == 1)
        {
            quads[activeQuad].videoHFlip = true;
        }
    }

    // video VFlip
    else if ( m.getAddress() == "/active/video/vmirror" )
    {
        // argument is int32
        int osc_quad_videoVFlip = m.getArgAsInt32( 0 );
        if(osc_quad_videoVFlip == 0)
        {
            quads[activeQuad].videoVFlip = false;
        }
        else if(osc_quad_videoVFlip == 1)
        {
            quads[activeQuad].videoVFlip = true;
        }
    }


    else if ( m.getAddress() == "/active/video/color" )
    {
        // arguments are ffff
        float video_color_r = m.getArgAsFloat( 0 );
        float video_color_g = m.getArgAsFloat( 1 );
        float video_color_b = m.getArgAsFloat( 2 );
        float video_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].videoColorize.r = video_color_r;
        quads[activeQuad].videoColorize.g = video_color_g;
        quads[activeQuad].videoColorize.b = video_color_b;
        quads[activeQuad].videoColorize.a = video_color_a;
    }

    else if ( m.getAddress() == "/active/video/color/1" )
    {
        // arguments are f
        float video_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.r = video_color_r;
    }

    else if ( m.getAddress() == "/active/video/color/2" )
    {
        // arguments are f
        float video_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.g = video_color_g;
    }

    else if ( m.getAddress() == "/active/video/color/3" )
    {
        // arguments are f
        float video_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.b = video_color_b;
    }

    else if ( m.getAddress() == "/active/video/color/4" )
    {
        // arguments are f
        float video_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].videoColorize.a = video_color_a;
    }

    else if ( m.getAddress() == "/active/video/mult/x" )
    {
        // arguments are f
        float video_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].videoMultX = video_mult_x;
    }

    else if ( m.getAddress() == "/active/video/mult/y" )
    {
        // arguments are f
        float video_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].videoMultY = video_mult_y;
    }

    else if ( m.getAddress() == "/active/video/speed" )
    {
        // arguments are f
        float video_speed = m.getArgAsFloat( 0 );
        quads[activeQuad].videoSpeed = video_speed;
    }

    else if ( m.getAddress() == "/active/video/volume" )
    {
        // arguments are i
        int video_volume = m.getArgAsInt32( 0 );
        quads[activeQuad].videoVolume = video_volume;
    }

    // video loop
    else if ( m.getAddress() == "/active/video/loop" )
    {
        // argument is int32
        int osc_quad_videoLoop = m.getArgAsInt32( 0 );
        if(osc_quad_videoLoop == 0)
        {
            quads[activeQuad].videoLoop = false;
        }
        else if(osc_quad_videoLoop == 1)
        {
            quads[activeQuad].videoLoop = true;
        }
    }

    // video fit
    else if ( m.getAddress() == "/active/video/fit" )
    {
        // argument is int32
        int osc_quad_videoFit = m.getArgAsInt32( 0 );
        if(osc_quad_videoFit == 0)
        {
            quads[activeQuad].videoFit = false;
        }
        else if(osc_quad_videoFit == 1)
        {
            quads[activeQuad].videoFit = true;
        }
    }

    // video keep aspect ration
    else if ( m.getAddress() == "/active/video/keepaspect" )
    {
        // argument is int32
        int osc_quad_videoKeepAspectRatio = m.getArgAsInt32( 0 );
        if(osc_quad_videoKeepAspectRatio == 0)
        {
            quads[activeQuad].videoKeepAspect = false;
        }
        else if(osc_quad_videoKeepAspectRatio == 1)
        {
            quads[activeQuad].videoKeepAspect = true;
        }
    }

    // video reset
    else if ( m.getAddress() == "/active/video/reset" )
    {
        //no argument
        quads[activeQuad].video.setPosition(0.0);
    }

    //video stop
    else if (m.getAddress() == "/active/video/stop")
    {
        quads[activeQuad].video.stop();
    }
    //video play
    //video stop
    else if (m.getAddress() == "/active/video/play")
    {
        quads[activeQuad].video.play();
    }


    // video greenscreen
    else if ( m.getAddress() == "/active/video/greenscreen" )
    {
        // argument is int32
        int osc_quad_videoGreenscreen = m.getArgAsInt32( 0 );
        if(osc_quad_videoGreenscreen == 0)
        {
            quads[activeQuad].videoGreenscreen = false;
        }
        else if(osc_quad_videoGreenscreen == 1)
        {
            quads[activeQuad].videoGreenscreen = true;
        }
    }

    // sharedvideo
    else if ( m.getAddress() == "/active/video/sharedvideo" )
    {
        // argument is int32
        int osc_quad_sharedVideoBg = m.getArgAsInt32( 0 );
        if(osc_quad_sharedVideoBg == 0)
        {
            quads[activeQuad].sharedVideoBg = false;
        }
        else if(osc_quad_sharedVideoBg == 1)
        {
            quads[activeQuad].sharedVideoBg = true;
        }
    }
    //sharedvideo fit
    else if ( m.getAddress() == "/active/video/svfit" )
    {
        // argument is int32
        int osc_quad_sharedVideofit = m.getArgAsInt32( 0 );
        if(osc_quad_sharedVideofit == 0)
        {
            quads[activeQuad].sVideoFit = false;
        }
        else if(osc_quad_sharedVideofit== 1)
        {
            quads[activeQuad].sVideoFit = true;
        }
    }
    //sharedvideo keep aspect
    else if ( m.getAddress() == "/active/video/svaspect" )
    {
        // argument is int32
        int osc_quad_sharedVideoaspect = m.getArgAsInt32( 0 );
        if(osc_quad_sharedVideoaspect == 0)
        {
            quads[activeQuad].sVideoKeepAspect = false;
        }
        else if(osc_quad_sharedVideoaspect== 1)
        {
            quads[activeQuad].sVideoKeepAspect = true;
        }
    }
    else if ( m.getAddress() == "/active/video/svvflip" )
    {
        // argument is int32
        int osc_quad_sharedVideovflip = m.getArgAsInt32( 0 );
        if(osc_quad_sharedVideovflip == 0)
        {
            quads[activeQuad].sVideoVFlip = false;
        }
        else if(osc_quad_sharedVideovflip== 1)
        {
            quads[activeQuad].sVideoVFlip = true;
        }
    }
    else if ( m.getAddress() == "/active/video/svhflip" )
    {
        // argument is int32
        int osc_quad_sharedVideohflip = m.getArgAsInt32( 0 );
        if(osc_quad_sharedVideohflip == 0)
        {
            quads[activeQuad].sVideoHFlip = false;
        }
        else if(osc_quad_sharedVideohflip== 1)
        {
            quads[activeQuad].sVideoHFlip = true;
        }
    }
    else if ( m.getAddress() == "/active/video/svgreenscreen" )
    {
        // argument is int32
        int osc_quad_sharedVideogreenscreen = m.getArgAsInt32( 0 );
        if(osc_quad_sharedVideogreenscreen == 0)
        {
            quads[activeQuad].sVideoGreenscreen = false;
        }
        else if(osc_quad_sharedVideogreenscreen== 1)
        {
            quads[activeQuad].sVideoGreenscreen = true;
        }
    }



    // sharedvideonum
    else if ( m.getAddress() == "/active/video/sharedvideonum" )
    {
        // argument is int32
        int osc_quad_sharedVideoNum = m.getArgAsInt32( 0 );
        if((osc_quad_sharedVideoNum > 0)&&(osc_quad_sharedVideoNum < 9))
        {
            quads[activeQuad].sharedVideoNum = osc_quad_sharedVideoNum;
        }
    }

    // camera stuff

    else if ( m.getAddress() == "/active/cam" )
    {
        quads[activeQuad].camBg = !quads[activeQuad].camBg;
    }

    // video on/off
    else if ( m.getAddress() == "/active/cam/show" )
    {
        // argument is int32
        int osc_quad_camBg = m.getArgAsInt32( 0 );
        if(osc_quad_camBg == 0)
        {
            quads[activeQuad].camBg = false;
        }
        else if(osc_quad_camBg == 1)
        {
            quads[activeQuad].camBg = true;
        }
    }

    // video HFlip
    else if ( m.getAddress() == "/active/cam/hmirror" )
    {
        // argument is int32
        int osc_quad_camHFlip = m.getArgAsInt32( 0 );
        if(osc_quad_camHFlip == 0)
        {
            quads[activeQuad].camHFlip = false;
        }
        else if(osc_quad_camHFlip == 1)
        {
            quads[activeQuad].camHFlip = true;
        }
    }

    // camera VFlip
    else if ( m.getAddress() == "/active/cam/vmirror" )
    {
        // argument is int32
        int osc_quad_camVFlip = m.getArgAsInt32( 0 );
        if(osc_quad_camVFlip == 0)
        {
            quads[activeQuad].camVFlip = false;
        }
        else if(osc_quad_camVFlip == 1)
        {
            quads[activeQuad].camVFlip = true;
        }
    }

    else if ( m.getAddress() == "/active/cam/color" )
    {
        // arguments are ffff
        float cam_color_r = m.getArgAsFloat( 0 );
        float cam_color_g = m.getArgAsFloat( 1 );
        float cam_color_b = m.getArgAsFloat( 2 );
        float cam_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].camColorize.r = cam_color_r;
        quads[activeQuad].camColorize.g = cam_color_g;
        quads[activeQuad].camColorize.b = cam_color_b;
        quads[activeQuad].camColorize.a = cam_color_a;
    }

    else if ( m.getAddress() == "/active/cam/color/1" )
    {
        // arguments are f
        float cam_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].camColorize.r = cam_color_r;
    }

    else if ( m.getAddress() == "/active/cam/color/2" )
    {
        // arguments are f
        float cam_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].camColorize.g = cam_color_g;
    }

    else if ( m.getAddress() == "/active/cam/color/3" )
    {
        // arguments are f
        float cam_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].camColorize.b = cam_color_b;
    }

    else if ( m.getAddress() == "/active/cam/color/4" )
    {
        // arguments are f
        float cam_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].camColorize.a = cam_color_a;
    }

    else if ( m.getAddress() == "/active/cam/mult/x" )
    {
        // arguments are f
        float cam_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].camMultX = cam_mult_x;
    }

    else if ( m.getAddress() == "/active/cam/mult/y" )
    {
        // arguments are f
        float cam_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].camMultY = cam_mult_y;
    }


    // cam greenscreen
    else if ( m.getAddress() == "/active/cam/greenscreen" )
    {
        // argument is int32
        int osc_quad_camGreenscreen = m.getArgAsInt32( 0 );
        if(osc_quad_camGreenscreen == 0)
        {
            quads[activeQuad].camGreenscreen = false;
        }
        else if(osc_quad_camGreenscreen == 1)
        {
            quads[activeQuad].camGreenscreen = true;
        }
    }

    // cam fit
    else if ( m.getAddress() == "/active/cam/fit" )
    {
        // argument is int32
        int osc_quad_camFit = m.getArgAsInt32( 0 );
        if(osc_quad_camFit == 0)
        {
            quads[activeQuad].camFit = false;
        }
        else if(osc_quad_camFit == 1)
        {
            quads[activeQuad].camFit = true;
        }
    }

    // cam keep aspect ration
    else if ( m.getAddress() == "/active/cam/keepaspect" )
    {
        // argument is int32
        int osc_quad_camKeepAspectRatio = m.getArgAsInt32( 0 );
        if(osc_quad_camKeepAspectRatio == 0)
        {
            quads[activeQuad].camKeepAspect = false;
        }
        else if(osc_quad_camKeepAspectRatio == 1)
        {
            quads[activeQuad].camKeepAspect = true;
        }
    }

    else if ( m.getAddress() == "/active/cam/num" )
    {
        // arguments are int32
        int cam_num = m.getArgAsFloat( 0 );
        quads[activeQuad].camNumber = cam_num;
    }

    // greenscreen stuff

    // greenscreen threshold
    else if ( m.getAddress() == "/active/greenscreen/threshold" )
    {
        // argument is int32
        int osc_quad_thresholdGreenscreen = m.getArgAsInt32( 0 );
        quads[activeQuad].thresholdGreenscreen = osc_quad_thresholdGreenscreen;
    }

    else if ( m.getAddress() == "/active/greenscreen/color" )
    {
        // arguments are ffff
        float greenscreen_color_r = m.getArgAsFloat( 0 );
        float greenscreen_color_g = m.getArgAsFloat( 1 );
        float greenscreen_color_b = m.getArgAsFloat( 2 );
        float greenscreen_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].colorGreenscreen.r = greenscreen_color_r;
        quads[activeQuad].colorGreenscreen.g = greenscreen_color_g;
        quads[activeQuad].colorGreenscreen.b = greenscreen_color_b;
        quads[activeQuad].colorGreenscreen.a = greenscreen_color_a;
    }

    else if ( m.getAddress() == "/active/greenscreen/color/1" )
    {
        // arguments are f
        float greenscreen_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].colorGreenscreen.r = greenscreen_color_r;
    }

    else if ( m.getAddress() == "/active/greenscreen/color/2" )
    {
        // arguments are f
        float greenscreen_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].colorGreenscreen.g = greenscreen_color_g;
    }

    else if ( m.getAddress() == "/active/greenscreen/color/3" )
    {
        // arguments are f
        float greenscreen_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].colorGreenscreen.b = greenscreen_color_b;
    }

    else if ( m.getAddress() == "/active/greenscreen/color/4" )
    {
        // arguments are f
        float greenscreen_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].colorGreenscreen.a = greenscreen_color_a;
    }

    // slideshow stuff

    else if ( m.getAddress() == "/active/slideshow" )
    {
        quads[activeQuad].slideshowBg = !quads[activeQuad].slideshowBg;
    }

    // slideshow on/off
    else if ( m.getAddress() == "/active/slideshow/show" )
    {
        // argument is int32
        int osc_quad_slideshowBg = m.getArgAsInt32( 0 );
        if(osc_quad_slideshowBg == 0)
        {
            quads[activeQuad].slideshowBg = false;
        }
        else if(osc_quad_slideshowBg == 1)
        {
            quads[activeQuad].slideshowBg = true;
        }
    }

    else if ( m.getAddress() == "/active/slideshow/folder" )
    {
        // argument is int32
        loadSlideshow();
    }

    else if ( m.getAddress() == "/active/slideshow/fit" )
    {
        // argument is int32
        int osc_quad_slideFit = m.getArgAsInt32( 0 );
        if(osc_quad_slideFit == 0)
        {
            quads[activeQuad].slideFit = false;
        }
        else if(osc_quad_slideFit == 1)
        {
            quads[activeQuad].slideFit = true;
        }
    }

    else if ( m.getAddress() == "/active/slideshow/keep_aspect" )
    {
        // argument is int32
        int osc_quad_slideKeepAspect = m.getArgAsInt32( 0 );
        if(osc_quad_slideKeepAspect == 0)
        {
            quads[activeQuad].slideKeepAspect = false;
        }
        else if(osc_quad_slideKeepAspect == 1)
        {
            quads[activeQuad].slideKeepAspect = true;
        }
    }
    else if ( m.getAddress() == "/active/slideshow/greenscreen" )
    {
        // argument is int32
        int osc_quad_slidegreenscreen = m.getArgAsInt32( 0 );
        if(osc_quad_slidegreenscreen == 0)
        {
            quads[activeQuad].imgGreenscreen = false;
        }
        else if(osc_quad_slidegreenscreen == 1)
        {
            quads[activeQuad].imgGreenscreen = true;
        }
    }

    else if ( m.getAddress() == "/active/slideshow/duration" )
    {
        // arguments are f
        float osc_quad_slideshowSpeed = m.getArgAsFloat( 0 );
        quads[activeQuad].slideshowSpeed = osc_quad_slideshowSpeed;
    }


    // kinect stuff
    else if ( m.getAddress() == "/active/kinect" )
    {
        quads[activeQuad].kinectBg = !quads[activeQuad].kinectBg;
    }

    // slideshow on/off
    else if ( m.getAddress() == "/active/kinect/show" )
    {
        // argument is int32
        int osc_quad_kinectBg = m.getArgAsInt32( 0 );
        if(osc_quad_kinectBg == 0)
        {
            quads[activeQuad].kinectBg = false;
        }
        else if(osc_quad_kinectBg == 1)
        {
            quads[activeQuad].kinectBg = true;
        }
    }

    //kinect stuff
#ifdef WITH_KINECT
    else if ( m.getAddress() == "/active/kinect/open" )
    {
        // no argument
        kinect.kinect.open();
    }

    else if ( m.getAddress() == "/active/kinect/close" )
    {
        // no argument
        kinect.kinect.setCameraTiltAngle(0);
        kinect.kinect.close();
    }

    // kinect image
    else if ( m.getAddress() == "/active/kinect/show/image" )
    {
        // argument is int32
        int osc_quad_kinectImg = m.getArgAsInt32( 0 );
        if(osc_quad_kinectImg == 0)
        {
            quads[activeQuad].kinectImg = false;
        }
        else if(osc_quad_kinectImg == 1)
        {
            quads[activeQuad].kinectImg = true;
        }
    }

    // kinect grayscale image
    else if ( m.getAddress() == "/active/kinect/show/grayscale" )
    {
        // argument is int32
        int osc_quad_getKinectGrayImage = m.getArgAsInt32( 0 );
        if(osc_quad_getKinectGrayImage == 0)
        {
            quads[activeQuad].getKinectGrayImage = false;
        }
        else if(osc_quad_getKinectGrayImage == 1)
        {
            quads[activeQuad].getKinectGrayImage = true;
        }
    }

    else if ( m.getAddress() == "/active/kinect/mask" )
    {
        // argument is int32
        int osc_quad_kinectMask = m.getArgAsInt32( 0 );
        if(osc_quad_kinectMask == 0)
        {
            quads[activeQuad].kinectMask = false;
        }
        else if(osc_quad_kinectMask == 1)
        {
            quads[activeQuad].kinectMask = true;
        }
    }


    else if ( m.getAddress() == "/active/kinect/mult/x" )
    {
        // arguments are f
        float kinect_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectMultX = kinect_mult_x;
    }

    else if ( m.getAddress() == "/active/kinect/mult/y" )
    {
        // arguments are f
        float kinect_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectMultY = kinect_mult_y;
    }

    else if ( m.getAddress() == "/active/kinect/scale/x" )
    {
        // arguments are f
        float kinect_mult_x = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectMultX = kinect_mult_x;
    }

    else if ( m.getAddress() == "/active/kinect/scale/y" )
    {
        // arguments are f
        float kinect_mult_y = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectMultY = kinect_mult_y;
    }


    else if ( m.getAddress() == "/active/kinect/threshold/near" )
    {
        // arguments are int32
        int osc_quad_nearDepthTh = m.getArgAsInt32( 0 );
        quads[activeQuad].nearDepthTh = osc_quad_nearDepthTh;
    }

    else if ( m.getAddress() == "/active/kinect/threshold/far" )
    {
        // arguments are int32
        int osc_quad_farDepthTh = m.getArgAsInt32( 0 );
        quads[activeQuad].farDepthTh = osc_quad_farDepthTh;
    }

    else if ( m.getAddress() == "/active/kinect/angle" )
    {
        // argument is int32
        int osc_quad_kinectAngle = m.getArgAsInt32( 0 );
        if(osc_quad_kinectAngle >= -30 && osc_quad_kinectAngle <= 30)
        {
            kinect.kinectAngle = osc_quad_kinectAngle;
        }
    }

    else if ( m.getAddress() == "/active/kinect/blur" )
    {
        // argument is int32
        int osc_quad_kinectBlur = m.getArgAsInt32( 0 );
        if(osc_quad_kinectBlur >= 0 && osc_quad_kinectBlur <= 10)
        {
            quads[activeQuad].kinectBlur = osc_quad_kinectBlur;
        }
    }

    else if ( m.getAddress() == "/active/kinect/contour" )
    {
        // argument is int32
        int osc_quad_getKinectContours = m.getArgAsInt32( 0 );
        if(osc_quad_getKinectContours == 0)
        {
            quads[activeQuad].getKinectContours = false;
        }
        else if(osc_quad_getKinectContours == 1)
        {
            quads[activeQuad].getKinectContours = true;
        }
    }

    else if ( m.getAddress() == "/active/kinect/contour/curves" )
    {
        // argument is int32
        int osc_quad_kinectContourCurved = m.getArgAsInt32( 0 );
        if(osc_quad_kinectContourCurved == 0)
        {
            quads[activeQuad].kinectContourCurved = false;
        }
        else if(osc_quad_kinectContourCurved == 1)
        {
            quads[activeQuad].kinectContourCurved = true;
        }
    }

    else if ( m.getAddress() == "/active/kinect/contour/smooth" )
    {
        // argument is int32
        int osc_quad_kinectContourSmooth = m.getArgAsInt32( 0 );
        if(osc_quad_kinectContourSmooth >= 0 && osc_quad_kinectContourSmooth <= 20)
        {
            quads[activeQuad].kinectContourSmooth = osc_quad_kinectContourSmooth;
        }
    }

    else if ( m.getAddress() == "/active/kinect/contour/simplify" )
    {
        // argument is f
        float osc_quad_kinectContourSimplify = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectContourSimplify = osc_quad_kinectContourSimplify;
    }

    else if ( m.getAddress() == "/active/kinect/contour/area/min" )
    {
        // argument is f
        float osc_quad_kinectContourMin = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectContourMin = osc_quad_kinectContourMin;
    }

    else if ( m.getAddress() == "/active/kinect/contour/area/max" )
    {
        // argument is f
        float osc_quad_kinectContourMax = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectContourMax = osc_quad_kinectContourMax;
    }

    else if ( m.getAddress() == "/active/kinect/contour/area" )
    {
        // argument is f f
        float osc_quad_kinectContourMin = m.getArgAsFloat( 0 );
        float osc_quad_kinectContourMax = m.getArgAsFloat( 1 );
        quads[activeQuad].kinectContourMin = osc_quad_kinectContourMin;
        quads[activeQuad].kinectContourMax = osc_quad_kinectContourMax;
    }

    else if ( m.getAddress() == "/active/kinect/color" )
    {
        // arguments are ffff
        float kinect_color_r = m.getArgAsFloat( 0 );
        float kinect_color_g = m.getArgAsFloat( 1 );
        float kinect_color_b = m.getArgAsFloat( 2 );
        float kinect_color_a = m.getArgAsFloat( 3 );
        quads[activeQuad].kinectColorize.r = kinect_color_r;
        quads[activeQuad].kinectColorize.g = kinect_color_g;
        quads[activeQuad].kinectColorize.b = kinect_color_b;
        quads[activeQuad].kinectColorize.a = kinect_color_a;
    }

    else if ( m.getAddress() == "/active/kinect/color/1" )
    {
        // arguments are f
        float kinect_color_r = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectColorize.r = kinect_color_r;
    }

    else if ( m.getAddress() == "/active/kinect/color/2" )
    {
        // arguments are f
        float kinect_color_g = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectColorize.g = kinect_color_g;
    }

    else if ( m.getAddress() == "/active/kinect/color/3" )
    {
        // arguments are f
        float kinect_color_b = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectColorize.b = kinect_color_b;
    }

    else if ( m.getAddress() == "/active/kinect/color/4" )
    {
        // arguments are f
        float kinect_color_a = m.getArgAsFloat( 0 );
        quads[activeQuad].kinectColorize.a = kinect_color_a;
    }
#endif

    else if ( m.getAddress() == "/active/crop/rectangular/top" )
    {
        // arguments are f
        float crop_top = m.getArgAsFloat(0);
        quads[activeQuad].crop[0] = crop_top;
    }
    else if ( m.getAddress() == "/active/crop/rectangular/right" )
    {
        // arguments are f
        float crop_right = m.getArgAsFloat(0);
        quads[activeQuad].crop[1] = crop_right;
    }
    else if ( m.getAddress() == "/active/crop/rectangular/bottom" )
    {
        // arguments are f
        float crop_bottom = m.getArgAsFloat(0);
        quads[activeQuad].crop[2] = crop_bottom;
    }
    else if ( m.getAddress() == "/active/crop/rectangular/left" )
    {
        // arguments are f
        float crop_left = m.getArgAsFloat(0);
        quads[activeQuad].crop[3] = crop_left;
    }
    else if ( m.getAddress() == "/active/crop/circular/x" )
    {
        // arguments are f
        float crop_center_x = m.getArgAsFloat(0);
        quads[activeQuad].circularCrop[0] = crop_center_x;
    }
    else if ( m.getAddress() == "/active/crop/circular/y" )
    {
        // arguments are f
        float crop_center_y = m.getArgAsFloat(0);
        quads[activeQuad].circularCrop[1] = crop_center_y;
    }
    else if ( m.getAddress() == "/active/crop/circular/radius" )
    {
        // arguments are f
        float crop_radius = m.getArgAsFloat(0);
        quads[activeQuad].circularCrop[2] = crop_radius;
    }

    //3D Model
    else if ( m.getAddress() == "/active/3d/load" )
    {
        // no argument
        openAnimaFile();
    }
    else if ( m.getAddress() == "/active/3d/scale/x" )
    {
        // arguments are f
        float scale_x_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaScalex = scale_x_3d;
    }
    else if ( m.getAddress() == "/active/3d/scale/y" )
    {
        // arguments are f
        float scale_y_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaScaley = scale_y_3d;
    }
    else if ( m.getAddress() == "/active/3d/scale/z" )
    {
        // arguments are f
        float scale_z_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaScalez = scale_z_3d;
    }
    else if ( m.getAddress() == "/active/3d/rotate/x" )
    {
        // arguments are f
        float rotate_x_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaRotateX = rotate_x_3d;
    }
    else if ( m.getAddress() == "/active/3d/rotate/y" )
    {
        // arguments are f
        float rotate_y_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaRotateY = rotate_y_3d;
    }
    else if ( m.getAddress() == "/active/3d/rotate/z" )
    {
        // arguments are f
        float rotate_z_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaRotateZ = rotate_z_3d;
    }
    else if ( m.getAddress() == "/active/3d/move/x" )
    {
        // arguments are f
        float move_x_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaMovex = move_x_3d;
    }
    else if ( m.getAddress() == "/active/3d/move/y" )
    {
        // arguments are f
        float move_y_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaMovey = move_y_3d;
    }
    else if ( m.getAddress() == "/active/3d/move/z" )
    {
        // arguments are f
        float move_z_3d = m.getArgAsFloat(0);
        quads[activeQuad].animaMovez = move_z_3d;
    }
        // texture modes
    else if ( m.getAddress() == "/active/3d/texture/mode" )
    {
        // argument is int32
        int osc_quad_texmod = m.getArgAsInt32( 0 );
        if(osc_quad_texmod < 4)
        {
            quads[activeQuad].textureModes = osc_quad_texmod;
        }
    }

    else if ( m.getAddress() == "/active/3d/animation" )
    {
        // argument is int32
        int osc_quad_animate = m.getArgAsInt32( 0 );
        if(osc_quad_animate == 0)
        {
            quads[activeQuad].bAnimate = false;
        }
        else if(osc_quad_animate == 1)
        {
            quads[activeQuad].bAnimate = true;
        }
    }


//***//***//messages to address surface directly//***//***//

    else if (ofIsStringInString (m.getAddress(), "/surface/")){
            //split address " ","surface","index","show|"
        vector<string> splittedAdress = ofSplitString(m.getAddress(),"/");
        // erase first element (empty string), we have after split address "surface","index","show|img|cam|video|greenscreen|kinect|slideshow|crop|placement"
        splittedAdress.erase(splittedAdress.begin());
        int surfaceIndex=ofToInt(splittedAdress[1]);

        // /surface/0/show
        if (splittedAdress[2]=="show"){
            // arguments is int (on/off)
            int osc_quad_isOn = m.getArgAsInt32(0);
            if(osc_quad_isOn == 0)
            {
                quads[surfaceIndex].isOn = false;
            }
            else if(osc_quad_isOn == 1)
            {
                quads[surfaceIndex].isOn = true;
            }
        }
        //surface/0/img
        else if (splittedAdress[2]=="img"){
                //end
            if (splittedAdress.size()<4){

                    quads[surfaceIndex].imgBg = !quads[surfaceIndex].imgBg;

            }else if (splittedAdress[3]=="show"){
            // argument is int32
                int osc_quad_imgBg = m.getArgAsInt32( 0 );
                if(osc_quad_imgBg == 0)
                {
                    quads[surfaceIndex].imgBg = false;
                }
                else if(osc_quad_imgBg == 1)
                {
                    quads[surfaceIndex].imgBg = true;
                }

            // surface/0/img/load
            }else if (splittedAdress[3]=="load"){
            // no args

                openImageFile();
            }

            //surface/0/img/hmirror
            else if (splittedAdress[3]=="hmirror"){
                // argument is int32
                int osc_quad_imgHFlip = m.getArgAsInt32( 0 );
                if(osc_quad_imgHFlip == 0)
                {
                    quads[surfaceIndex].imgHFlip = false;
                }
                else if(osc_quad_imgHFlip == 1)
                {
                    quads[surfaceIndex].imgHFlip = true;
                }
            }

            //surface/0/img/vmirror
            else if (splittedAdress[3]=="vmirror"){
                // argument is int32
                int osc_quad_imgVFlip = m.getArgAsInt32( 0 );
                if(osc_quad_imgVFlip == 0)
                {
                   quads[surfaceIndex].imgVFlip = false;
                }
                else if(osc_quad_imgVFlip == 1)
                {
                    quads[surfaceIndex].imgVFlip = true;
                }
            }

            //surface/0/img/color
            else if (splittedAdress[3]=="color"){
                // /surface/0/img/color
                if (splittedAdress.size()<5)
                {
                     // arguments are ffff
                    float img_color_r = m.getArgAsFloat( 0 );
                    float img_color_g = m.getArgAsFloat( 1 );
                    float img_color_b = m.getArgAsFloat( 2 );
                    float img_color_a = m.getArgAsFloat( 3 );
                    quads[surfaceIndex].imgColorize.r = img_color_r;
                    quads[surfaceIndex].imgColorize.g = img_color_g;
                    quads[surfaceIndex].imgColorize.b = img_color_b;
                    quads[surfaceIndex].imgColorize.a = img_color_a;
                }
                //surface/0/img/color/1
                else if (splittedAdress[4]=="1")
                {
                         // arguments are f
                    float img_color_r = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].imgColorize.r = img_color_r;

                }
                //surface/0/img/color/2
                else if (splittedAdress[4]=="2")
                {
                         // arguments are f
                    float img_color_g = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].imgColorize.g = img_color_g;

                }
                //surface/0/img/color/3
                else if (splittedAdress[4]=="3")
                {

                         // arguments are f
                    float img_color_b = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].imgColorize.b = img_color_b;

                }
                //surface/0/img/color/4
                else if (splittedAdress[4]=="4")
                {
                         // arguments are f
                    float img_color_a = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].imgColorize.a = img_color_a;

                }
            //end color
            }
            // /surface/0/img/mult
            else if (splittedAdress[3]=="mult")
            {
                if (splittedAdress.size()>=4)
                {

                        // /surface/0/img/mult/x
                    if (splittedAdress[4]=="x")
                    {
                         // arguments are f
                        float img_mult_x = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].imgMultX = img_mult_x;
                    }
                        // /surface/0/img/mult/y
                    else if (splittedAdress[4]=="y")
                    {
                         // arguments are f
                        float img_mult_y = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].imgMultY = img_mult_y;
                    }
                }
            }

        //end img
        }

        //surface/0/cam
        else if (splittedAdress[2]=="cam"){
                //end
            if (splittedAdress.size()<4){

                    quads[surfaceIndex].camBg = !quads[surfaceIndex].camBg;

            }
            // /surface/0/cam/show
            else if (splittedAdress[3]=="show"){
            // argument is int32
                int osc_quad_camBg = m.getArgAsInt32( 0 );
                if(osc_quad_camBg == 0)
                {
                    quads[surfaceIndex].camBg = false;
                }
                else if(osc_quad_camBg == 1)
                {
                    quads[surfaceIndex].camBg = true;
                }

            }

            //surface/0/cam/hmirror
            else if (splittedAdress[3]=="hmirror"){
                // argument is int32
                int osc_quad_camHFlip = m.getArgAsInt32( 0 );
                if(osc_quad_camHFlip == 0)
                {
                    quads[surfaceIndex].camHFlip = false;
                }
                else if(osc_quad_camHFlip == 1)
                {
                    quads[surfaceIndex].camHFlip = true;
                }
            }

            //surface/0/cam/vmirror
            else if (splittedAdress[3]=="vmirror"){
                // argument is int32
                int osc_quad_camVFlip = m.getArgAsInt32( 0 );
                if(osc_quad_camVFlip == 0)
                {
                   quads[surfaceIndex].camVFlip = false;
                }
                else if(osc_quad_camVFlip == 1)
                {
                    quads[surfaceIndex].camVFlip = true;
                }
            }

            //surface/0/cam/color
            else if (splittedAdress[3]=="color"){
                // /surface/0/cam/color
                if (splittedAdress.size()<5)
                {
                     // arguments are ffff
                    float cam_color_r = m.getArgAsFloat( 0 );
                    float cam_color_g = m.getArgAsFloat( 1 );
                    float cam_color_b = m.getArgAsFloat( 2 );
                    float cam_color_a = m.getArgAsFloat( 3 );
                    quads[surfaceIndex].camColorize.r = cam_color_r;
                    quads[surfaceIndex].camColorize.g = cam_color_g;
                    quads[surfaceIndex].camColorize.b = cam_color_b;
                    quads[surfaceIndex].camColorize.a = cam_color_a;
                }
                //surface/0/cam/color/1
                else if (splittedAdress[4]=="1")
                {
                         // arguments are f
                    float cam_color_r = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].camColorize.r = cam_color_r;

                }
                //surface/0/cam/color/2
                else if (splittedAdress[4]=="2")
                {
                         // arguments are f
                    float cam_color_g = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].camColorize.g = cam_color_g;

                }
                //surface/0/cam/color/3
                else if (splittedAdress[4]=="3")
                {

                         // arguments are f
                    float cam_color_b = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].camColorize.b = cam_color_b;

                }
                //surface/0/cam/color/1
                else if (splittedAdress[4]=="4")
                {
                         // arguments are f
                    float cam_color_a = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].camColorize.a = cam_color_a;

                }
            //end color
            }
            // /surface/0/cam/mult
            else if (splittedAdress[3]=="mult")
            {
                if (splittedAdress.size()>=4)
                {

                        // /surface/0/cam/mult/x
                    if (splittedAdress[4]=="x")
                    {
                         // arguments are f
                        float cam_mult_x = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].camMultX = cam_mult_x;
                    }
                        // /surface/0/cam/mult/y
                    else if (splittedAdress[4]=="y")
                    {
                         // arguments are f
                        float cam_mult_y = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].camMultY = cam_mult_y;
                    }
                }
            }
            // /surface/0/cam/greenscreen
            else if (splittedAdress[3]=="greenscreen"){
             // argument is int32
                int osc_quad_camGreenscreen = m.getArgAsInt32( 0 );
                if(osc_quad_camGreenscreen == 0)
                {
                    quads[surfaceIndex].camGreenscreen = false;
                }
                else if(osc_quad_camGreenscreen == 1)
                {
                    quads[surfaceIndex].camGreenscreen = true;
                }

            }
            // /surface/0/cam/fit
            else if (splittedAdress[3]=="fit"){
                // argument is int32
                int osc_quad_camFit = m.getArgAsInt32( 0 );
                if(osc_quad_camFit == 0)
                {
                    quads[surfaceIndex].camFit = false;
                }
                else if(osc_quad_camFit == 1)
                {
                    quads[surfaceIndex].camFit = true;
                }
            }

            // /surface/0/cam/keepaspect
            else if (splittedAdress[3]=="keepaspect"){
                // argument is int32
                int osc_quad_camKeepAspect = m.getArgAsInt32( 0 );
                if(osc_quad_camKeepAspect == 0)
                {
                    quads[surfaceIndex].camKeepAspect = false;
                }
                else if(osc_quad_camKeepAspect == 1)
                {
                    quads[surfaceIndex].camKeepAspect = true;
                }
            }

            // /surface/0/cam/num
            else if (splittedAdress[3]=="num"){
                // arguments are int32
                int cam_num = m.getArgAsFloat( 0 );
                quads[surfaceIndex].camNumber = cam_num;
            }


        //end cam
        }

        //surface/0/video
        else if (splittedAdress[2]=="video"){
                //end
            if (splittedAdress.size()<4){

                    quads[surfaceIndex].videoBg = !quads[surfaceIndex].videoBg;

            }
            // /surface/0/video/show
            else if (splittedAdress[3]=="show"){
            // argument is int32
                int osc_quad_videoBg = m.getArgAsInt32( 0 );
                if(osc_quad_videoBg == 0)
                {
                    quads[surfaceIndex].videoBg = false;
                }
                else if(osc_quad_videoBg == 1)
                {
                    quads[surfaceIndex].videoBg = true;
                }

            }

            //surface/0/video/hmirror
            else if (splittedAdress[3]=="hmirror"){
                // argument is int32
                int osc_quad_videoHFlip = m.getArgAsInt32( 0 );
                if(osc_quad_videoHFlip == 0)
                {
                    quads[surfaceIndex].videoHFlip = false;
                }
                else if(osc_quad_videoHFlip == 1)
                {
                    quads[surfaceIndex].videoHFlip = true;
                }
            }

            //surface/0/video/vmirror
            else if (splittedAdress[3]=="vmirror"){
                // argument is int32
                int osc_quad_videoVFlip = m.getArgAsInt32( 0 );
                if(osc_quad_videoVFlip == 0)
                {
                   quads[surfaceIndex].videoVFlip = false;
                }
                else if(osc_quad_videoVFlip == 1)
                {
                    quads[surfaceIndex].videoVFlip = true;
                }
            }

            //surface/0/video/color
            else if (splittedAdress[3]=="color"){
                // /surface/0/video/color
                if (splittedAdress.size()<5)
                {
                     // arguments are ffff
                    float video_color_r = m.getArgAsFloat( 0 );
                    float video_color_g = m.getArgAsFloat( 1 );
                    float video_color_b = m.getArgAsFloat( 2 );
                    float video_color_a = m.getArgAsFloat( 3 );
                    quads[surfaceIndex].videoColorize.r = video_color_r;
                    quads[surfaceIndex].videoColorize.g = video_color_g;
                    quads[surfaceIndex].videoColorize.b = video_color_b;
                    quads[surfaceIndex].videoColorize.a = video_color_a;
                }
                //surface/0/video/color/1
                else if (splittedAdress[4]=="1")
                {
                         // arguments are f
                    float video_color_r = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].videoColorize.r = video_color_r;

                }
                //surface/0/video/color/2
                else if (splittedAdress[4]=="2")
                {
                         // arguments are f
                    float video_color_g = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].videoColorize.g = video_color_g;

                }
                //surface/0/video/color/3
                else if (splittedAdress[4]=="3")
                {

                         // arguments are f
                    float video_color_b = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].videoColorize.b = video_color_b;

                }
                //surface/0/video/color/1
                else if (splittedAdress[4]=="4")
                {
                         // arguments are f
                    float video_color_a = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].videoColorize.a = video_color_a;

                }
            //end color
            }
            // /surface/0/video/mult
            else if (splittedAdress[3]=="mult")
            {
                if (splittedAdress.size()>=4)
                {

                        // /surface/0/video/mult/x
                    if (splittedAdress[4]=="x")
                    {
                         // arguments are f
                        float video_mult_x = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].videoMultX = video_mult_x;
                    }
                        // /surface/0/video/mult/y
                    else if (splittedAdress[4]=="y")
                    {
                         // arguments are f
                        float video_mult_y = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].videoMultY = video_mult_y;
                    }
                }
            }
            // /surface/0/video/greenscreen
            else if (splittedAdress[3]=="greenscreen"){
             // argument is int32
                int osc_quad_videoGreenscreen = m.getArgAsInt32( 0 );
                if(osc_quad_videoGreenscreen == 0)
                {
                    quads[surfaceIndex].videoGreenscreen = false;
                }
                else if(osc_quad_videoGreenscreen == 1)
                {
                    quads[surfaceIndex].videoGreenscreen = true;
                }

            }

            // /surface/0/video/speed
            else if (splittedAdress[3]=="speed"){
              // arguments are f
                float video_speed = m.getArgAsFloat( 0 );
                quads[surfaceIndex].videoSpeed = video_speed;

            }

            // /surface/0/video/volume
            else if (splittedAdress[3]=="volume"){
             // arguments are i
                int video_volume = m.getArgAsInt32( 0 );
                quads[surfaceIndex].videoVolume = video_volume;
            }

            // /surface/0/video/loop
            else if (splittedAdress[3]=="loop"){
             // argument is int32
                int osc_quad_videoLoop = m.getArgAsInt32( 0 );
                if(osc_quad_videoLoop == 0)
                {
                    quads[surfaceIndex].videoLoop = false;
                }
                else if(osc_quad_videoLoop == 1)
                {
                    quads[surfaceIndex].videoLoop = true;
                }
            }
            // quads[i].sharedVideoBg

            // /surface/0/video/sharedvideo
            else if (splittedAdress[3]=="sharedvideo"){
             // argument is int32
                int osc_quad_sharedVideoBg = m.getArgAsInt32( 0 );
                if(osc_quad_sharedVideoBg == 0)
                {
                    quads[surfaceIndex].sharedVideoBg = false;
                }
                else if(osc_quad_sharedVideoBg == 1)
                {
                    quads[surfaceIndex].sharedVideoBg = true;
                }
            }
            else if (splittedAdress[3]=="svfit"){
             // argument is int32
                int osc_quad_sharedVideofit = m.getArgAsInt32( 0 );
                if(osc_quad_sharedVideofit == 0)
                {
                    quads[surfaceIndex].sVideoFit = false;
                }
                else if(osc_quad_sharedVideofit == 1)
                {
                    quads[surfaceIndex].sVideoFit = true;
                }
            }
            else if (splittedAdress[3]=="svaspect"){
             // argument is int32
                int osc_quad_sharedVideoaspect = m.getArgAsInt32( 0 );
                if(osc_quad_sharedVideoaspect == 0)
                {
                    quads[surfaceIndex].sVideoKeepAspect = false;
                }
                else if(osc_quad_sharedVideoaspect == 1)
                {
                    quads[surfaceIndex].sVideoKeepAspect = true;
                }
            }
            else if (splittedAdress[3]=="svvflip"){
             // argument is int32
                int osc_quad_sharedVideovflip = m.getArgAsInt32( 0 );
                if(osc_quad_sharedVideovflip == 0)
                {
                    quads[surfaceIndex].sVideoVFlip = false;
                }
                else if(osc_quad_sharedVideovflip == 1)
                {
                    quads[surfaceIndex].sVideoVFlip = true;
                }
            }
            else if (splittedAdress[3]=="svhflip"){
             // argument is int32
                int osc_quad_sharedVideohflip = m.getArgAsInt32( 0 );
                if(osc_quad_sharedVideohflip == 0)
                {
                    quads[surfaceIndex].sVideoHFlip = false;
                }
                else if(osc_quad_sharedVideohflip == 1)
                {
                    quads[surfaceIndex].sVideoHFlip = true;
                }
            }
            else if (splittedAdress[3]=="svgreenscreen"){
             // argument is int32
                int osc_quad_sharedVideogreenscreen = m.getArgAsInt32( 0 );
                if(osc_quad_sharedVideogreenscreen == 0)
                {
                    quads[surfaceIndex].sVideoGreenscreen = false;
                }
                else if(osc_quad_sharedVideogreenscreen == 1)
                {
                    quads[surfaceIndex].sVideoGreenscreen = true;
                }
            }

            // /surface/0/video/sharedvideonum
            else if (splittedAdress[3]=="sharedvideonum"){
             // argument is int32
                int osc_quad_sharedVideoNum = m.getArgAsInt32( 0 );
                if((osc_quad_sharedVideoNum > 0)&&(osc_quad_sharedVideoNum < 9))
                {
                    quads[surfaceIndex].sharedVideoNum = osc_quad_sharedVideoNum;
                }
            }
            //surface/0/video/reset
            else if (splittedAdress[3] == "reset"){
            //no argument
            quads[surfaceIndex].video.setPosition(0.0);
            }
            //surface/0/video/stop
            else if (splittedAdress[3] == "stop"){
            //no argument
            quads[surfaceIndex].video.stop();
            }
            //surface/0/video/play
            else if (splittedAdress[3] == "play"){
            //no argument
            quads[surfaceIndex].video.play();
            }

            // /surface/0/video/fit
            else if (splittedAdress[3]=="fit"){
                // argument is int32
                int osc_quad_videoFit = m.getArgAsInt32( 0 );
                if(osc_quad_videoFit == 0)
                {
                    quads[surfaceIndex].videoFit = false;
                }
                else if(osc_quad_videoFit == 1)
                {
                    quads[surfaceIndex].videoFit = true;
                }
            }

            // /surface/0/video/keepaspect
            else if (splittedAdress[3]=="keepaspect"){
                // argument is int32
                int osc_quad_videoKeepAspect = m.getArgAsInt32( 0 );
                if(osc_quad_videoKeepAspect == 0)
                {
                    quads[surfaceIndex].videoKeepAspect = false;
                }
                else if(osc_quad_videoKeepAspect == 1)
                {
                    quads[surfaceIndex].videoKeepAspect = true;
                }
            }
        //end video
        }

		//surface/0/solid
        else if (splittedAdress[2]=="solid"){
                //end
            if (splittedAdress.size()<4){

                    quads[surfaceIndex].colorBg = !quads[surfaceIndex].colorBg;

            }else if (splittedAdress[3]=="show"){
            // argument is int32
                int osc_quad_solidBg = m.getArgAsInt32( 0 );
                if(osc_quad_solidBg == 0)
                {
                    quads[surfaceIndex].colorBg = false;
                }
                else if(osc_quad_solidBg == 1)
                {
                    quads[surfaceIndex].colorBg = true;
                }

			}

            //surface/0/solid/color
            else if (splittedAdress[3]=="color"){
                // /surface/0/solid/color
                if (splittedAdress.size()<5)
                {
                     // arguments are ffff
                    float solid_color_r = m.getArgAsFloat( 0 );
                    float solid_color_g = m.getArgAsFloat( 1 );
                    float solid_color_b = m.getArgAsFloat( 2 );
                    float solid_color_a = m.getArgAsFloat( 3 );
                    quads[surfaceIndex].bgColor.r = solid_color_r;
                    quads[surfaceIndex].bgColor.g = solid_color_g;
                    quads[surfaceIndex].bgColor.b = solid_color_b;
                    quads[surfaceIndex].bgColor.a = solid_color_a;
                }
                //surface/0/solid/color/1
                else if (splittedAdress[4]=="1")
                {
                         // arguments are f
                    float solid_color_r = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].bgColor.r = solid_color_r;

                }
                //surface/0/solid/color/2
                else if (splittedAdress[4]=="2")
                {
                         // arguments are f
                    float solid_color_g = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].bgColor.g = solid_color_g;

                }
                //surface/0/solid/color/3
                else if (splittedAdress[4]=="3")
                {

                         // arguments are f
                    float solid_color_b = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].bgColor.b = solid_color_b;

                }
                //surface/0/solid/color/4
                else if (splittedAdress[4]=="4")
                {
                         // arguments are f
                    float solid_color_a = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].bgColor.a = solid_color_a;

                }
            //end color
            }
            // /surface/0/solid/trans
            else if (splittedAdress[3]=="trans")
            {
                if (splittedAdress.size()==4)
				{
					quads[surfaceIndex].transBg = !quads[activeQuad].transBg;
				}

				else if (splittedAdress.size()>4)
                {

                        // /surface/0/solid/trans/show
                    if (splittedAdress[4]=="show")
                    {
                        // argument is int32
						int osc_quad_transBg = m.getArgAsInt32( 0 );
						if(osc_quad_transBg == 0)
						{
							quads[surfaceIndex].transBg = false;
						}
						else if(osc_quad_transBg == 1)
						{
							quads[surfaceIndex].transBg = true;
						}

                    }
                        // /surface/0/solid/trans/color
                    else if (splittedAdress[4]=="color")
                    {
						if (splittedAdress.size()==5)
						{
						// arguments are ffff
						float trans_color_r = m.getArgAsFloat( 0 );
						float trans_color_g = m.getArgAsFloat( 1 );
						float trans_color_b = m.getArgAsFloat( 2 );
						float trans_color_a = m.getArgAsFloat( 3 );
						quads[surfaceIndex].secondColor.r = trans_color_r;
						quads[surfaceIndex].secondColor.g = trans_color_g;
						quads[surfaceIndex].secondColor.b = trans_color_b;
						quads[surfaceIndex].secondColor.a = trans_color_a;
						}

						else if (splittedAdress.size()>5)
						{
							//surface/0/solid/trans/color/1
							if (splittedAdress[5]=="1")
							{
							// arguments are f
							float trans_color_r = m.getArgAsFloat( 0 );
							quads[surfaceIndex].secondColor.r = trans_color_r;

							}

							//surface/0/solid/trans/color/2
							else if (splittedAdress[5]=="2")
							{
							// arguments are f
							float trans_color_g = m.getArgAsFloat( 0 );
							quads[surfaceIndex].secondColor.g = trans_color_g;

							}
							//surface/0/solid/trans/color/3
							else if (splittedAdress[5]=="3")
							{

								// arguments are f
								float trans_color_b = m.getArgAsFloat( 0 );
								quads[surfaceIndex].secondColor.b = trans_color_b;

							}
							//surface/0/solid/color/4
							else if (splittedAdress[4]=="4")
							{
							// arguments are f
							float trans_color_a = m.getArgAsFloat( 0 );
							quads[surfaceIndex].secondColor.a = trans_color_a;

							}
						}
                    // end /surface/0/solid/trans/color
                    }

					// /surface/0/solid/trans/duration
					else if (splittedAdress[4]=="duration")
                    {
						// arguments are f
						float trans_duration = m.getArgAsFloat( 0 );
						quads[surfaceIndex].transDuration = trans_duration;
					}

                }
            }

        //end solid
        }

		//surface/0/greenscreen
		else if (splittedAdress[2]=="greenscreen"){

		     //surface/0/greenscreen/threshold
            if (splittedAdress[3]=="threshold"){
				// argument is int32
				int osc_quad_thresholdGreenscreen = m.getArgAsInt32( 0 );
				quads[surfaceIndex].thresholdGreenscreen = osc_quad_thresholdGreenscreen;

			}

            //surface/0/greenscreen/color
            else if (splittedAdress[3]=="color"){
                // /surface/0/greenscreen/color
                if (splittedAdress.size()<5)
                {
                    // arguments are ffff
					float greenscreen_color_r = m.getArgAsFloat( 0 );
					float greenscreen_color_g = m.getArgAsFloat( 1 );
					float greenscreen_color_b = m.getArgAsFloat( 2 );
					float greenscreen_color_a = m.getArgAsFloat( 3 );
					quads[surfaceIndex].colorGreenscreen.r = greenscreen_color_r;
					quads[surfaceIndex].colorGreenscreen.g = greenscreen_color_g;
					quads[surfaceIndex].colorGreenscreen.b = greenscreen_color_b;
					quads[surfaceIndex].colorGreenscreen.a = greenscreen_color_a;
                }
                //surface/0/greenscreen/color/1
                else if (splittedAdress[4]=="1")
                {
                         // arguments are f
                    float greenscreen_color_r = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].colorGreenscreen.r = greenscreen_color_r;

                }
                //surface/0/greenscreen/color/2
                else if (splittedAdress[4]=="2")
                {
                         // arguments are f
                    float greenscreen_color_g = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].colorGreenscreen.g = greenscreen_color_g;

                }
                //surface/0/greenscreen/color/3
                else if (splittedAdress[4]=="3")
                {

                         // arguments are f
                    float greenscreen_color_b = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].colorGreenscreen.b = greenscreen_color_b;

                }
                //surface/0/greenscreen/color/4
                else if (splittedAdress[4]=="4")
                {
                         // arguments are f
                    float greenscreen_color_a = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].colorGreenscreen.a = greenscreen_color_a;

                }
            //end color
            }
		// end greenscreen
		}

        //surface/0/mask
        else if (splittedAdress[2]=="mask"){
                //end
            if (splittedAdress.size()<4){

                    quads[surfaceIndex].bMask = !quads[surfaceIndex].bMask;

            }else if (splittedAdress[3]=="show"){
            // argument is int32
                int osc_quad_bMask = m.getArgAsInt32( 0 );
                if(osc_quad_bMask == 0)
                {
                    quads[surfaceIndex].bMask = false;
                }
                else if(osc_quad_bMask == 1)
                {
                    quads[surfaceIndex].bMask = true;
                }

            // surface/0/mask/invert
            }else if (splittedAdress[3]=="invert"){
            // argument is int32
                int osc_quad_maskInvert = m.getArgAsInt32( 0 );
                if(osc_quad_maskInvert == 0)
                {
                    quads[surfaceIndex].maskInvert = false;
                }
                else if(osc_quad_maskInvert == 1)
                {
                    quads[surfaceIndex].maskInvert = true;
                }
            }

        // end mask
        }

        //surface/0/timeline
        else if (splittedAdress[2]=="timeline"){

            if (splittedAdress[3]=="color"){
            // argument is int32
                int osc_quad_bTimelineColor = m.getArgAsInt32( 0 );
                if(osc_quad_bTimelineColor == 0)
                {
                    quads[surfaceIndex].bTimelineColor = false;
                }
                else if(osc_quad_bTimelineColor == 1)
                {
                    quads[surfaceIndex].bTimelineColor = true;
                }

            }

            if (splittedAdress[3]=="alpha"){
            // argument is int32
                int osc_quad_bTimelineAlpha = m.getArgAsInt32( 0 );
                if(osc_quad_bTimelineAlpha == 0)
                {
                    quads[surfaceIndex].bTimelineAlpha = false;
                }
                else if(osc_quad_bTimelineAlpha == 1)
                {
                    quads[surfaceIndex].bTimelineAlpha = true;
                }

            }

           if (splittedAdress[3]=="slides"){
            // argument is int32
                int osc_quad_bTimelineSlideChange = m.getArgAsInt32( 0 );
                if(osc_quad_bTimelineSlideChange == 0)
                {
                    quads[activeQuad].bTimelineSlideChange = false;
                }
                else if(osc_quad_bTimelineSlideChange == 1)
                {
                    quads[activeQuad].bTimelineSlideChange = true;
                }
           }

           if (splittedAdress[3]=="tint"){
            // argument is int32
                int osc_quad_bTimelineTint = m.getArgAsInt32( 0 );
                if(osc_quad_bTimelineTint == 0)
                {
                    quads[surfaceIndex].bTimelineTint = false;
                }
                else if(osc_quad_bTimelineTint == 1)
                {
                    quads[surfaceIndex].bTimelineTint = true;
                }

            }
        // end timeline
        }

        // /surface/0/edgeblend
        else if (splittedAdress[2]=="edgeblend"){

                // /surface/0/edgeblend/show
                if (splittedAdress[3]=="show")
                {
                    // argument is int32
                    int osc_quad_edgeBlendBool = m.getArgAsInt32( 0 );
                    if(osc_quad_edgeBlendBool == 0)
                    {
                        quads[surfaceIndex].edgeBlendBool = false;
                    }
                    else if(osc_quad_edgeBlendBool == 1)
                    {
                        quads[surfaceIndex].edgeBlendBool = true;
                    }

                }

                else if (splittedAdress[3]=="power")
                {
                    // argument is float
                    float osc_quad_edgeBlendExponent = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].edgeBlendExponent = osc_quad_edgeBlendExponent;
                }

                else if (splittedAdress[3]=="gamma")
                {
                    // argument is float
                    float osc_quad_edgeBlendGamma = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].edgeBlendGamma = osc_quad_edgeBlendGamma;
                }

                else if (splittedAdress[3]=="luminance")
                {
                    // argument is float
                    float osc_quad_edgeBlendLuminance = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].edgeBlendLuminance = osc_quad_edgeBlendLuminance;
                }

                else if (splittedAdress[3]=="amount")
                {

                    if (splittedAdress.size()<5)
                    {

                         // argument is ffff
                        float osc_quad_edgeBlendAmountTop = m.getArgAsFloat( 0 );
                        float osc_quad_edgeBlendAmountDx = m.getArgAsFloat( 1 );
                        float osc_quad_edgeBlendAmountBottom = m.getArgAsFloat( 2 );
                        float osc_quad_edgeBlendAmountSin = m.getArgAsFloat( 3 );
                        quads[surfaceIndex].edgeBlendAmountTop = osc_quad_edgeBlendAmountTop;
                        quads[surfaceIndex].edgeBlendAmountDx = osc_quad_edgeBlendAmountDx;
                        quads[surfaceIndex].edgeBlendAmountBottom = osc_quad_edgeBlendAmountBottom;
                        quads[surfaceIndex].edgeBlendAmountSin = osc_quad_edgeBlendAmountSin;
                    }
                    else if (splittedAdress[4]=="top")
                    {
                        // argument is float
                        float osc_quad_edgeBlendAmountTop = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].edgeBlendAmountTop = osc_quad_edgeBlendAmountTop;
                    }

                    else if (splittedAdress[4]=="bottom")
                    {
                        // argument is float
                        float osc_quad_edgeBlendAmountBottom = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].edgeBlendAmountBottom = osc_quad_edgeBlendAmountBottom;
                    }

                    else if (splittedAdress[4]=="right")
                    {
                        // argument is float
                        float osc_quad_edgeBlendAmountDx = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].edgeBlendAmountDx = osc_quad_edgeBlendAmountDx;
                    }

                    else if (splittedAdress[4]=="left")
                    {
                        // argument is float
                        float osc_quad_edgeBlendAmountSin = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].edgeBlendAmountSin = osc_quad_edgeBlendAmountSin;
                    }

                // end amount
                }
        //end edgeblend
        }

		// /surface/0/slideshow
		else if (splittedAdress[2]=="slideshow"){

			if (splittedAdress.size()<4)
			{
				quads[surfaceIndex].slideshowBg = !quads[surfaceIndex].slideshowBg;
			}
			// surface/0/slideshow/show
			else if (splittedAdress[3]=="show")
			{
				// argument is int32
				int osc_quad_slideshowBg = m.getArgAsInt32( 0 );
				if(osc_quad_slideshowBg == 0)
				{
					quads[surfaceIndex].slideshowBg = false;
				}
				else if(osc_quad_slideshowBg == 1)
				{
					quads[surfaceIndex].slideshowBg = true;
				}

			}
			// surface/0/slideshow/folder
			else if (splittedAdress[3]=="folder")
			{
				// argument is int32
                loadSlideshow();
			}
			// surface/0/slideshow/fit
			else if (splittedAdress[3]=="fit")
			{
				// argument is int32
				int osc_quad_slideFit = m.getArgAsInt32( 0 );
				if(osc_quad_slideFit == 0)
				{
					quads[surfaceIndex].slideFit = false;
				}
				else if(osc_quad_slideFit == 1)
				{
					quads[surfaceIndex].slideFit = true;
				}

			}
			// surface/0/slideshow/keep_aspect
			else if (splittedAdress[3]=="keep_aspect")
			{
				// argument is int32
				int osc_quad_slideKeepAspect = m.getArgAsInt32( 0 );
				if(osc_quad_slideKeepAspect == 0)
				{
					quads[surfaceIndex].slideKeepAspect = false;
				}
				else if(osc_quad_slideKeepAspect == 1)
				{
					quads[surfaceIndex].slideKeepAspect = true;
				}
			}
                // surface/0/slideshow/keep_aspect
			else if (splittedAdress[3]=="greenscreen")
			{
				// argument is int32
				int osc_quad_slidegreenscreen = m.getArgAsInt32( 0 );
				if(osc_quad_slidegreenscreen == 0)
				{
					quads[surfaceIndex].imgGreenscreen = false;
				}
				else if(osc_quad_slidegreenscreen == 1)
				{
					quads[surfaceIndex].imgGreenscreen = true;
				}
			}
			// surface/0/slideshow/duration
			else if (splittedAdress[3]=="duration")
			{
				// arguments are f
				float osc_quad_slideshowSpeed = m.getArgAsFloat( 0 );
				quads[surfaceIndex].slideshowSpeed = osc_quad_slideshowSpeed;

			}



		//end slideshow
		}

		// /surface/0/blendmodes
		else if (splittedAdress[2]=="blendmodes"){

			// /surface/0/blendmodes/show
			if (splittedAdress[3]=="show")
			{
				// argument is int32
				int osc_quad_bBlendModes = m.getArgAsInt32( 0 );
				if(osc_quad_bBlendModes == 0)
				{
					quads[surfaceIndex].bBlendModes = false;
				}
				else if(osc_quad_bBlendModes == 1)
				{
					quads[surfaceIndex].bBlendModes = true;
				}
			}
			else if (splittedAdress[3]=="mode")
			{
				// argument is int32
				int osc_quad_blendMode = m.getArgAsInt32( 0 );
				if(osc_quad_blendMode < 4)
				{
					quads[surfaceIndex].blendMode = osc_quad_blendMode;
				}
			}

		// end blendmodes
		}

		// /surface/0/deform
		else if (splittedAdress[2]=="deform"){

			// /surface/0/deform/show
			if (splittedAdress[3]=="show")
			{
				// argument is int32
				int osc_quad_bDeform = m.getArgAsInt32( 0 );
				if(osc_quad_bDeform == 0)
				{
					quads[surfaceIndex].bDeform = false;
				}
				else if(osc_quad_bDeform == 1)
				{
					quads[surfaceIndex].bDeform = true;
				}
			}

			// /surface/0/deform/bezier
			else if (splittedAdress[3]=="bezier")
			{
				if (splittedAdress.size()<5)
				{
					// argument is int32
					int osc_quad_bBezier = m.getArgAsInt32( 0 );
					if(osc_quad_bBezier == 0)
					{
						quads[surfaceIndex].bBezier = false;
					}
					else if(osc_quad_bBezier == 1)
					{
						quads[surfaceIndex].bBezier = true;
					}
				}
				// surface/0/deform/bezier/spherize/
				else if (splittedAdress[4]=="spherize")
				{
					if (splittedAdress[5]=="light")
					{
						// no argument
						quadBezierSpherize(surfaceIndex);
					}
					else if (splittedAdress[5]=="strong")
					{
						// no argument
						quadBezierSpherizeStrong(surfaceIndex);
					}
				}
				// surface/0/deform/bezier/reset
				else if (splittedAdress[4]=="reset")
				{
					// no argument
					quadBezierReset(surfaceIndex);
				}


			}

			// /surface/0/deform/grid
			else if (splittedAdress[3]=="grid")
			{
				if (splittedAdress.size()<5)
				{
					// argument is int32
					int osc_quad_bGrid = m.getArgAsInt32( 0 );
					if(osc_quad_bGrid == 0)
					{
						quads[surfaceIndex].bGrid = false;
					}
					else if(osc_quad_bGrid == 1)
					{
						quads[surfaceIndex].bGrid = true;
					}
				}
				// surface/0/deform/bezier/grid/rows
				else if (splittedAdress[4]=="rows")
				{
						 // argument is int32
					int osc_quad_gridRows = m.getArgAsInt32( 0 );
					if(osc_quad_gridRows >= 2 && osc_quad_gridRows <= 15)
					{
						quads[surfaceIndex].gridRows = osc_quad_gridRows;
					}
				}

				// surface/0/deform/bezier/grid/columns
				else if (splittedAdress[4]=="columns")
				{
					// argument is int32
					int osc_quad_gridColumns = m.getArgAsInt32( 0 );
					if(osc_quad_gridColumns >= 2 && osc_quad_gridColumns <= 20)
					{
						quads[surfaceIndex].gridColumns = osc_quad_gridColumns;
					}
				}


			}

		// end deform
		}

		// /surface/0/placement
		else if (splittedAdress[2]=="placement"){

			if (splittedAdress.size()<4)
			{
				// argument is int32 int32
				int osc_quad_quadDispX = m.getArgAsInt32( 0 );
				int osc_quad_quadDispY = m.getArgAsInt32( 1 );
				quads[surfaceIndex].quadDispX = osc_quad_quadDispX;
				quads[surfaceIndex].quadDispY = osc_quad_quadDispY;
			}

			// /surface/0/placement/x
			else if (splittedAdress[3]=="x")
			{
				// argument is int32
				int osc_quad_quadDispX = m.getArgAsInt32( 0 );
				quads[surfaceIndex].quadDispX = osc_quad_quadDispX;
			}

			// /surface/0/placement/y
			else if (splittedAdress[3]=="y")
			{
				// argument is int32
				int osc_quad_quadDispY = m.getArgAsInt32( 0 );
				quads[surfaceIndex].quadDispY = osc_quad_quadDispY;
			}

			// /surface/0/placement/w
			else if (splittedAdress[3]=="w")
			{
				// argument is int32
				int osc_quad_quadW = m.getArgAsInt32( 0 );
				quads[surfaceIndex].quadW = osc_quad_quadW;
			}

			// /surface/0/placement/h
			else if (splittedAdress[3]=="h")
			{
				// argument is int32
				int osc_quad_quadH = m.getArgAsInt32( 0 );
				quads[surfaceIndex].quadH = osc_quad_quadH;
			}

			// /surface/0/placement/dimensions
			else if (splittedAdress[3]=="dimensions")
			{
				// argument is int32 int32
				int osc_quad_quadW = m.getArgAsInt32( 0 );
				int osc_quad_quadH = m.getArgAsInt32( 1 );
				quads[surfaceIndex].quadW = osc_quad_quadW;
				quads[surfaceIndex].quadH = osc_quad_quadH;
			}

			// /surface/0/placement/reset
			else if (splittedAdress[3]=="reset")
			{
				// no argument
				quadDimensionsReset(surfaceIndex);
				quadPlacementReset(surfaceIndex);
			}

		// end placement
		}

		// /surface/0/crop
		else if (splittedAdress[2]=="crop"){

			//surface/0/crop/rectangular
			if (splittedAdress[3]=="rectangular")
			{
				//surface/0/crop/rectangular/top
				if (splittedAdress[4]=="top")
				{
					// arguments are f
					float crop_top = m.getArgAsFloat(0);
					quads[surfaceIndex].crop[0] = crop_top;
				}

				//surface/0/crop/rectangular/bottom
				else if (splittedAdress[4]=="bottom")
				{
					// arguments are f
					float crop_bottom = m.getArgAsFloat(0);
					quads[surfaceIndex].crop[2] = crop_bottom;
				}

				//surface/0/crop/rectangular/right
				else if (splittedAdress[4]=="right")
				{
					// arguments are f
					float crop_right = m.getArgAsFloat(0);
					quads[surfaceIndex].crop[1] = crop_right;
				}

				//surface/0/crop/rectangular/left
				else if (splittedAdress[4]=="left")
				{
					// arguments are f
					float crop_left = m.getArgAsFloat(0);
					quads[surfaceIndex].crop[3] = crop_left;
				}

			}

			//surface/0/crop/circular
			else if (splittedAdress[3]=="circular")
			{
				//surface/0/crop/circular/x
				if (splittedAdress[4]=="x")
				{
					// arguments are f
					float crop_center_x = m.getArgAsFloat(0);
					quads[surfaceIndex].circularCrop[0] = crop_center_x;
				}

				//surface/0/crop/circular/y
				else if (splittedAdress[4]=="y")
				{
					// arguments are f
					float crop_center_y = m.getArgAsFloat(0);
					quads[surfaceIndex].circularCrop[1] = crop_center_y;
				}

				//surface/0/crop/circular/radius
				else if (splittedAdress[4]=="radius")
				{
					// arguments are f
					float crop_radius = m.getArgAsFloat(0);
					quads[surfaceIndex].circularCrop[2] = crop_radius;
				}

			}

		// end crop
		}
		//3D Model
		else if (splittedAdress[2]=="3d"){

            if (splittedAdress[3]=="load")
            {
             openAnimaFile();
            }
            if (splittedAdress[3]=="scale"){

            if (splittedAdress[4]=="x")
                    {
                     float scale_x_3d = m.getArgAsFloat(0);
                     quads[surfaceIndex].animaScalex = scale_x_3d;
                    }
            else if (splittedAdress[4]=="y")
                    {
                     float scale_y_3d = m.getArgAsFloat(0);
                     quads[surfaceIndex].animaScaley = scale_y_3d;
                     }
            else if (splittedAdress[4]=="z")
                     {
                     float scale_z_3d = m.getArgAsFloat(0);
                     quads[surfaceIndex].animaScalez = scale_z_3d;
                     }
             }

             if (splittedAdress[3]=="rotate")
                    {
             if (splittedAdress[4]=="x")
                        {
                        float rotate_x_3d = m.getArgAsFloat(0);
                        quads[surfaceIndex].animaRotateX = rotate_x_3d;
                        }
             else if (splittedAdress[4]=="y")
                        {
                        float rotate_y_3d = m.getArgAsFloat(0);
                        quads[surfaceIndex].animaRotateY = rotate_y_3d;
                        }
              else if (splittedAdress[4]=="z")
                        {
                        float rotate_z_3d = m.getArgAsFloat(0);
                        quads[surfaceIndex].animaRotateZ = rotate_z_3d;
                        }
                    }
               if (splittedAdress[3]=="move")
                   {
                        if(splittedAdress[4]=="x")
                        {
                        float mov_x_3d = m.getArgAsFloat(0);
                        quads[surfaceIndex].animaMovex = mov_x_3d;
                        }
                        else if(splittedAdress[4]=="y")
                        {
                        float mov_y_3d = m.getArgAsFloat(0);
                        quads[surfaceIndex].animaMovey = mov_y_3d;
                        }
                        else if(splittedAdress[4]=="y")
                        {
                        float mov_z_3d = m.getArgAsFloat(0);
                        quads[surfaceIndex].animaMovez = mov_z_3d;
                        }
            }
              else if  (splittedAdress[3]=="texture")
                {
                    if (splittedAdress[4]=="mode"){

                                int osc_quad_texmod = m.getArgAsInt32( 0 );
                                if(osc_quad_texmod < 4)
                                {
                                quads[surfaceIndex].textureModes = osc_quad_texmod;
                                }
                    }
                }
                else if (splittedAdress[3]=="animation")
                    {
                    int osc_quad_animate = m.getArgAsInt32( 0 );
                    if(osc_quad_animate == 0)
                    {
                    quads[surfaceIndex].bAnimate = false;
                    }
                    else if(osc_quad_animate == 1)
                    {
                    quads[surfaceIndex].bAnimate = true;
                    }
                }

		}


		// /surface/0/kinect
		else if (splittedAdress[2]=="kinect"){

			// /surface/0/kinect
			if (splittedAdress.size()<4)
			{
                quads[surfaceIndex].kinectBg= !quads[surfaceIndex].kinectBg;
			}

			// /surface/0/kinect/show
			else if (splittedAdress[3]=="show")
			{
			    if (splittedAdress.size()<5)
                {
                    // argument is int32
                    int osc_quad_kinectBg = m.getArgAsInt32( 0 );
                    if(osc_quad_kinectBg == 0)
                    {
                        quads[surfaceIndex].kinectBg = false;
                    }
                    else if(osc_quad_kinectBg == 1)
                    {
                        quads[surfaceIndex].kinectBg = true;
                    }
                }
                // /surface/0/kinect/show/image
                else if (splittedAdress[4]=="image")
                {
                    // argument is int32
                    int osc_quad_kinectImg = m.getArgAsInt32( 0 );
                    if(osc_quad_kinectImg == 0)
                    {
                        quads[surfaceIndex].kinectImg = false;
                    }
                    else if(osc_quad_kinectImg == 1)
                    {
                        quads[surfaceIndex].kinectImg = true;
                    }
                }
                // /surface/0/kinect/show/grayscale
                else if (splittedAdress[4]=="grayscale")
                {
                      // argument is int32
                    int osc_quad_getKinectGrayImage = m.getArgAsInt32( 0 );
                    if(osc_quad_getKinectGrayImage == 0)
                    {
                        quads[surfaceIndex].getKinectGrayImage = false;
                    }
                    else if(osc_quad_getKinectGrayImage == 1)
                    {
                        quads[surfaceIndex].getKinectGrayImage = true;
                    }
                }

			}

			// /surface/0/kinect/open
			else if (splittedAdress[3]=="open")
			{
                // no argument
                kinect.kinect.open();
			}

			// /surface/0/kinect/close
			else if (splittedAdress[3]=="close")
			{
                // no argument
                kinect.kinect.setCameraTiltAngle(0);
                kinect.kinect.close();
			}

			// /surface/0/kinect/mask
			else if (splittedAdress[3]=="mask")
			{
                // argument is int32
                int osc_quad_kinectMask = m.getArgAsInt32( 0 );
                if(osc_quad_kinectMask == 0)
                {
                    quads[surfaceIndex].kinectMask = false;
                }
                else if(osc_quad_kinectMask == 1)
                {
                    quads[surfaceIndex].kinectMask = true;
                }
			}

			// /surface/0/kinect/mult|scale
			else if ((splittedAdress[3]=="mult")||(splittedAdress[3]=="scale"))
			{
			    if (splittedAdress[4]=="x")
                {
                    // arguments are f
                    float kinect_mult_x = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].kinectMultX = kinect_mult_x;
                }
                else if (splittedAdress[4]=="y")
                {
                    // arguments are f
                    float kinect_mult_y = m.getArgAsFloat( 0 );
                    quads[activeQuad].kinectMultY = kinect_mult_y;
                }

			}

			// /surface/0/kinect/threshold
			else if (splittedAdress[3]=="threshold")
			{
                if (splittedAdress[4]=="near")
                {
                    // arguments are int32
                    int osc_quad_nearDepthTh = m.getArgAsInt32( 0 );
                    quads[surfaceIndex].nearDepthTh = osc_quad_nearDepthTh;
                }
                else if (splittedAdress[4]=="far")
                {
                    // arguments are int32
                    int osc_quad_farDepthTh = m.getArgAsInt32( 0 );
                    quads[surfaceIndex].farDepthTh = osc_quad_farDepthTh;
                }
			}

			// /surface/0/kinect/angle
			else if (splittedAdress[3]=="angle")
			{
                // argument is int32
                int osc_quad_kinectAngle = m.getArgAsInt32( 0 );
                if(osc_quad_kinectAngle >= -30 && osc_quad_kinectAngle <= 30)
                {
                    kinect.kinectAngle = osc_quad_kinectAngle;
                }
			}

			// /surface/0/kinect/blur
			else if (splittedAdress[3]=="blur")
			{
                // argument is int32
                int osc_quad_kinectBlur = m.getArgAsInt32( 0 );
                if(osc_quad_kinectBlur >= 0 && osc_quad_kinectBlur <= 10)
                {
                    quads[surfaceIndex].kinectBlur = osc_quad_kinectBlur;
                }
			}

			// /surface/0/kinect/contour
			else if (splittedAdress[3]=="contour")
			{
                if (splittedAdress.size()<5)
                {
                     // argument is int32
                    int osc_quad_getKinectContours = m.getArgAsInt32( 0 );
                    if(osc_quad_getKinectContours == 0)
                    {
                        quads[surfaceIndex].getKinectContours = false;
                    }
                    else if(osc_quad_getKinectContours == 1)
                    {
                        quads[surfaceIndex].getKinectContours = true;
                    }
                }
                // /surface/0/kinect/contour/curves
                else if (splittedAdress[4]=="curves")
                {
                    // argument is int32
                    int osc_quad_kinectContourCurved = m.getArgAsInt32( 0 );
                    if(osc_quad_kinectContourCurved == 0)
                    {
                        quads[surfaceIndex].kinectContourCurved = false;
                    }
                    else if(osc_quad_kinectContourCurved == 1)
                    {
                        quads[surfaceIndex].kinectContourCurved = true;
                    }
                }
                // /surface/0/kinect/contour/smooth
                else if (splittedAdress[4]=="smooth")
                {
                     // argument is int32
                    int osc_quad_kinectContourSmooth = m.getArgAsInt32( 0 );
                    if(osc_quad_kinectContourSmooth >= 0 && osc_quad_kinectContourSmooth <= 20)
                    {
                        quads[surfaceIndex].kinectContourSmooth = osc_quad_kinectContourSmooth;
                    }
                }
                // /surface/0/kinect/contour/simplify
                else if (splittedAdress[4]=="simplify")
                {
                    // argument is f
                    float osc_quad_kinectContourSimplify = m.getArgAsFloat( 0 );
                    quads[surfaceIndex].kinectContourSimplify = osc_quad_kinectContourSimplify;
                }
                // /surface/0/kinect/contour/area
                else if (splittedAdress[4]=="area")
                {
                    if (splittedAdress.size()<6)
                    {
                          // argument is f f
                        float osc_quad_kinectContourMin = m.getArgAsFloat( 0 );
                        float osc_quad_kinectContourMax = m.getArgAsFloat( 1 );
                        quads[surfaceIndex].kinectContourMin = osc_quad_kinectContourMin;
                        quads[surfaceIndex].kinectContourMax = osc_quad_kinectContourMax;
                    }
                    // /surface/0/kinect/contour/area/min
                    else if (splittedAdress[5]=="min")
                    {
                        // argument is f
                        float osc_quad_kinectContourMin = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].kinectContourMin = osc_quad_kinectContourMin;
                    }
                    // /surface/0/kinect/contour/area/max
                    else if (splittedAdress[5]=="max")
                    {
                        // argument is f
                        float osc_quad_kinectContourMin = m.getArgAsFloat( 0 );
                        quads[surfaceIndex].kinectContourMin = osc_quad_kinectContourMin;
                    }
                }
			}

			// /surface/0/kinect/color
			else if (splittedAdress[3]=="color")
			{
                if (splittedAdress.size()<5)
                {
                    // arguments are ffff
                    float kinect_color_r = m.getArgAsFloat( 0 );
                    float kinect_color_g = m.getArgAsFloat( 1 );
                    float kinect_color_b = m.getArgAsFloat( 2 );
                    float kinect_color_a = m.getArgAsFloat( 3 );
                    quads[surfaceIndex].kinectColorize.r = kinect_color_r;
                    quads[surfaceIndex].kinectColorize.g = kinect_color_g;
                    quads[surfaceIndex].kinectColorize.b = kinect_color_b;
                    quads[surfaceIndex].kinectColorize.a = kinect_color_a;
                }
                // /surface/0/kinect/color/1
                else if (splittedAdress[4]=="1")
                {
                    // arguments are f
                    float kinect_color_r = m.getArgAsFloat( 0 );
                    quads[activeQuad].kinectColorize.r = kinect_color_r;
                }
                // /surface/0/kinect/color/2
                else if (splittedAdress[4]=="2")
                {
                    // arguments are f
                    float kinect_color_g = m.getArgAsFloat( 0 );
                    quads[activeQuad].kinectColorize.g = kinect_color_g;
                }
                // /surface/0/kinect/color/3
                else if (splittedAdress[4]=="3")
                {
                    // arguments are f
                    float kinect_color_b = m.getArgAsFloat( 0 );
                    quads[activeQuad].kinectColorize.b = kinect_color_b;
                }
                // /surface/0/kinect/color/1
                else if (splittedAdress[4]=="4")
                {
                    // arguments are f
                    float kinect_color_a = m.getArgAsFloat( 0 );
                    quads[activeQuad].kinectColorize.a = kinect_color_a;
                }
			}

		// end kinect
		}

//end surface
    }


    /*
    else
    {
        // unrecognized message: display on the bottom of the screen
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        for ( int i=0; i<m.getNumArgs(); i++ )
        {
            // get the argument type
            msg_string += m.getArgTypeName( i );
            msg_string += ":";
            // display the argument - make sure we get the right type
            if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
                msg_string += ofToString( m.getArgAsInt32( i ) );
            else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
                msg_string += ofToString( m.getArgAsFloat( i ) );
            else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
                msg_string += m.getArgAsString( i );
            else
                msg_string += "unknown";
        }
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
        cout << endl << msg_string << endl;
    }
    */
    ///*

    // if we get an OSC message not recognized, we can use it for auto-learning gui control as with midi
    else
    {
        // hotkey stuff - learning
        if(bMidiHotkeyCoupling && midiHotkeyPressed >= 0)
        {
            if(oscHotkeyMessages.size()>0 && oscHotkeyMessages.size() == oscHotkeyKeys.size())
            {
                for(int i=0; i < oscHotkeyMessages.size(); i++)
                {
                    // check if we already have a message for selected hotkey and eventually removes it
                    if(oscHotkeyKeys[i] == midiHotkeyPressed)
                    {
                        oscHotkeyKeys.erase(oscHotkeyKeys.begin()+i);
                        oscHotkeyMessages.erase(oscHotkeyMessages.begin()+i);
                    }
                }
            }
            oscHotkeyMessages.push_back(m);
            oscHotkeyKeys.push_back(midiHotkeyPressed);
            cout << endl << "OSC message '" << m.getAddress() << " " << m.getArgAsString(0) << "' coupled to hotkey '" << (char) midiHotkeyPressed << "'" << endl;
            midiHotkeyPressed = -1;
            bMidiHotkeyCoupling = false;
            bMidiHotkeyLearning = false;
            return;
        }

        // hotkey stuff - checking
        if(oscHotkeyMessages.size()>0 && oscHotkeyMessages.size() == oscHotkeyKeys.size())
        {
            bool keyFound = false;
            for(int i=0; i < oscHotkeyMessages.size(); i++)
            {
                ofxOscMessage oscControl = oscHotkeyMessages[i];
                if(m.getAddress() == oscControl.getAddress())
                {
                    if(m.getNumArgs()>0 && m.getArgType(0) == OFXOSC_TYPE_INT32)
                    {
                        if(m.getArgAsInt32(0) == oscControl.getArgAsInt32(0))
                        {
                            keyPressed(oscHotkeyKeys[i]);
                            keyFound = true;
                            }
                        }
                    else if(m.getNumArgs()>0 && m.getArgType(0) == OFXOSC_TYPE_FLOAT)
                    {
                        if(m.getArgAsFloat(0) == oscControl.getArgAsFloat(0))
                        {
                            keyPressed(oscHotkeyKeys[i]);
                            keyFound = true;
                            }
                        }
                    else if(m.getNumArgs()>0 && m.getArgType(0) == OFXOSC_TYPE_STRING)
                    {
                        if(m.getArgAsString(0) == oscControl.getArgAsString(0))
                        {
                            keyPressed(oscHotkeyKeys[i]);
                            keyFound = true;
                            }
                        }
                    else if(m.getNumArgs()==0)
                    {
                        keyPressed(oscHotkeyKeys[i]);
                        keyFound = true;
                        }
                    }
                }
                if(keyFound) return;
            }

        // gui coupling stuff
        for(int i=0; i < gui.getPages().size(); i++)
        {
            for(int j=0; j < gui.getPages()[i]->getControls().size(); j++)
            {
                // toggle case
                if(gui.getPages()[i]->getControls()[j]->controlType == "Toggle")
                {
                    // learning
                    if(gui.getPages()[i]->getControls()[j]->bLearning)
                    {
                        gui.getPages()[i]->getControls()[j]->bLearning = false;
                        gui.getPages()[i]->getControls()[j]->bLearnt = true;
                        gui.getPages()[i]->getControls()[j]->oscControl = m;
                    }
                    // checking
                    else if(gui.getPages()[i]->getControls()[j]->bLearnt)
                    {
                        ofxOscMessage oscControl = gui.getPages()[i]->getControls()[j]->oscControl;
                        if(m.getAddress() == oscControl.getAddress())
                        {
                            if(m.getNumArgs()>0 && m.getArgType(0) == OFXOSC_TYPE_INT32)
                            {
                                if(m.getArgAsInt32(0) == oscControl.getArgAsInt32(0))
                                {
                                    ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle *) gui.getPages()[i]->getControls()[j];
                                    t->toggle();
                                }
                            }
                            else if(m.getNumArgs()>0 && m.getArgType(0) == OFXOSC_TYPE_FLOAT)
                            {
                                if(m.getArgAsFloat(0) == oscControl.getArgAsFloat(0))
                                {
                                    ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle *) gui.getPages()[i]->getControls()[j];
                                    t->toggle();
                                }
                            }
                            else if(m.getNumArgs()>0 && m.getArgType(0) == OFXOSC_TYPE_STRING)
                            {
                                if(m.getArgAsString(0) == oscControl.getArgAsString(0))
                                {
                                    ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle *) gui.getPages()[i]->getControls()[j];
                                    t->toggle();
                                }
                            }
                            else if(m.getNumArgs()==0)
                            {
                                ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle *) gui.getPages()[i]->getControls()[j];
                                t->toggle();
                            }
                        }

                    }
                }
                // slider case
                else if(gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat" || gui.getPages()[i]->getControls()[j]->controlType == "SliderInt")
                {
                    // learning
                    if(gui.getPages()[i]->getControls()[j]->bLearning)
                    {
                        if(m.getNumArgs()>0)
                        {
                            gui.getPages()[i]->getControls()[j]->bLearning = false;
                            gui.getPages()[i]->getControls()[j]->bLearnt = true;
                            gui.getPages()[i]->getControls()[j]->oscControl = m;
                        }
                    }
                    // checking
                    else if(gui.getPages()[i]->getControls()[j]->bLearnt)
                    {
                        ofxOscMessage oscControl = gui.getPages()[i]->getControls()[j]->oscControl;
                        if(m.getNumArgs()>0 && (m.getArgType(0) == OFXOSC_TYPE_INT32 || m.getArgType(0) == OFXOSC_TYPE_FLOAT))
                        {
                            if(m.getAddress() == oscControl.getAddress())
                            {
                                if(gui.getPages()[i]->getControls()[j]->controlType == "SliderFloat")
                                {
                                    ofxSimpleGuiSliderFloat *s = (ofxSimpleGuiSliderFloat *) gui.getPages()[i]->getControls()[j];
                                    float remappedValue;
                                    if(m.getArgType(0) == OFXOSC_TYPE_INT32)
                                    {
                                        float value = (float)m.getArgAsInt32(0);
                                        remappedValue = ofMap(value, (float) oscControlMin, (float) oscControlMax, (float) s->min, (float) s->max);
                                    }
                                    else
                                    {
                                        float value = m.getArgAsFloat(0);
                                        remappedValue = ofMap(value, (float) oscControlMin, (float) oscControlMax, (float) s->min, (float) s->max);
                                    }
                                    s->setValue(remappedValue);
                                }
                                else
                                {
                                    ofxSimpleGuiSliderInt *s = (ofxSimpleGuiSliderInt *) gui.getPages()[i]->getControls()[j];
                                    float remappedValue;
                                    if(m.getArgType(0) == OFXOSC_TYPE_INT32)
                                    {
                                        float value = (float)m.getArgAsInt32(0);
                                        remappedValue = ofMap(value, (float) oscControlMin, (float) oscControlMax, (float) s->min, (float) s->max);
                                    }
                                    else
                                    {
                                        float value = m.getArgAsFloat(0);
                                        remappedValue = ofMap(value, (float) oscControlMin, (float) oscControlMax, (float) s->min, (float) s->max);
                                    }
                                    s->setValue((int)remappedValue);
                                }
                            }
                        }
                    }
                }
            }
        }

    }//*/

}
