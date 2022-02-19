#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void mouseEntered(int x, int y) {};
	void mouseExited(int x, int y) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	void setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height, ofColor face_color, ofColor frame_color);

	ofEasyCam cam;
	ofMesh face, frame;

	vector<glm::vec3> base_location_list;
	
	vector<tuple<ofColor, glm::vec3, float>> sphere_list; // BodyColor, Location, size
	int number_of_sphere;

	vector<ofColor> color_palette;
};