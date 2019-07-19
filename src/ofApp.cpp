#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(0,0,0);
    
    findHue = 200;
    w = 640;
    h = 320;
    
    fabricIndex = 1;
    roomIndex = 2;
    
    
    for (int i = 0; i < 6; i++) {
        ofImage img;
        img.load( "room0" + ofToString(i + 1) + ".jpg" );
        rooms.push_back(img);
    }
    for (int i = 0; i < 4; i++) {
        ofImage img;
        img.load( "fabric0" + ofToString(i + 1) + ".jpg" );
        fabrics.push_back(img);
    }
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
    fbo.allocate(w, h, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    fbo.begin();
    ofClear(0);
    ofSetColor(255);
    rooms[ roomIndex ].draw(0,0,w,h);
    float ww = w/2;
    float hh = h/2;
    float t = ofGetElapsedTimef();
    float xx = ofMap( sin( t ), -1, 1, 0, ww);
    float yy = ofMap( sin( t * 2 ), -1, 1, 0, hh);
    fabrics[ fabricIndex ].draw(xx, yy, ww, hh);
    fbo.end();
    
    ofSetColor(255);
    fbo.draw(0,0,ofGetWidth(), ofGetHeight());
    
    fbo.readToPixels(pix);

    rgb.setFromPixels(pix.getData(), w, h);
    
    //mirror horizontal
    rgb.mirror(false, true);

    //duplicate rgb
    hsb = rgb;

    //convert to hsb
    hsb.convertRgbToHsv();

    //store the three channels as grayscale images
    hsb.convertToGrayscalePlanarImages(hue, sat, bri);

    //filter image based on the hue value were looking for
    for (int i=0; i<w*h; i++) {
        filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-5,findHue+5) ? 255 : 0;
    }
    filtered.flagImageChanged();

    //run the contour finder on the filtered image to find blobs with a certain hue
    contours.findContours(filtered, 50, w*h/2, 1, false);
    
    
    //draw all cv images
    rgb.draw(0,0, ofGetWidth(), ofGetHeight());
    contours.draw(0,0, ofGetWidth(), ofGetHeight());
//    hsb.draw(640,0);
//    hue.draw(0,240);
//    sat.draw(320,240);
//    bri.draw(640,240);
//    filtered.draw(0,480);
    
    ofColor c;
    c.setHsb( findHue, 255, 255, 255);
    ofFill();
    
    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        int xx = ofMap( contours.blobs[i].centroid.x , 0, w, 0, ofGetWidth() );
        int yy = ofMap( contours.blobs[i].centroid.y , 0, h, 0, ofGetHeight() );
        ofDrawBitmapStringHighlight( "Point " + ofToString( i), xx, yy );
    }
    
    ofFill();
    for( int i = 0; i < 255; i++) {
        ofColor hsl;
        hsl.setHsb( i, 255, 255, 255);
        ofSetColor( hsl );
        ofRectangle r( 0, ofMap(i, 0.0, 255.0, 0.0, ofGetHeight()), 20.0, ofGetHeight()/255.0);
        ofDrawRectangle( r );
        
    }
    
    ofSetColor(255);
    ofNoFill();
    ofSetLineWidth( 4 );
    ofPoint p1(0, ofMap( findHue, 0, 255, 0, ofGetHeight()) );
    ofPoint p2 = p1;
    p2.x = 20;
    ofDrawLine( p1, p2 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'q') {
        findHue -= 2;
        if (findHue < 0) findHue = 255;
    }
    if (key == 'w') {
        findHue += 2;
        if (findHue > 255) findHue = 0;
    }
    if (key == OF_KEY_RIGHT) {
        fabricIndex += 1;
        if ( fabricIndex >= fabrics.size() ) fabricIndex = 0;
    }
    if (key == OF_KEY_LEFT) {
        fabricIndex -= 1;
        if ( fabricIndex < 0 ) fabricIndex =   fabrics.size() - 1;
    }
    if (key == OF_KEY_UP) {
        roomIndex += 1;
        if ( roomIndex >= rooms.size() ) roomIndex = 0;
    }
    if (key == OF_KEY_DOWN) {
        roomIndex -= 1;
        if ( roomIndex < 0 ) roomIndex =   rooms.size() - 1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    findHue = ofMap( y, 10, ofGetHeight() - 10, 0, 255, true );
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
