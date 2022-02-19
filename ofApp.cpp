#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(1, 5);
	this->base_location_list = ico_sphere.getMesh().getVertices();

	this->number_of_sphere = 800;
	while (this->sphere_list.size() < this->number_of_sphere) {

		int index = ofRandom(this->base_location_list.size());
		auto tmp_location = this->base_location_list[index];
		tmp_location = glm::normalize(tmp_location) * ofRandom(0, 95);

		auto radius = this->sphere_list.size() < 110 ? ofRandom(10, 50) : ofRandom(3, 20);

		bool flag = true;
		for (int i = 0; i < this->sphere_list.size(); i++) {

			if (glm::distance(tmp_location, get<1>(this->sphere_list[i])) < get<2>(this->sphere_list[i]) + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			ofColor color;
			color.setHsb(ofRandom(255), 200, 255);
			auto size = (radius * 2) / sqrt(3);

			this->sphere_list.push_back(make_tuple(color, tmp_location, size));
		}
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	// 配色デザイン ペールオレンジ P052
	this->color_palette.push_back(ofColor(197, 0, 24));
	this->color_palette.push_back(ofColor(184, 12, 65));
	this->color_palette.push_back(ofColor(206, 97, 110));
	this->color_palette.push_back(ofColor(204, 85, 68));
	this->color_palette.push_back(ofColor(190, 145, 176));
	this->color_palette.push_back(ofColor(215, 130, 63));
	this->color_palette.push_back(ofColor(255, 241, 51));
	this->color_palette.push_back(ofColor(107, 182, 187));
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->face.clear();
	this->frame.clear();

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.3333333333333333333);

	for (int i = 0; i < this->sphere_list.size(); i++) {

		auto location = get<1>(this->sphere_list[i]);
		auto size = get<2>(this->sphere_list[i]);

		ofPushMatrix();
		ofTranslate(location);

		ofRotateZ(ofRandom(360));
		ofRotateY(ofRandom(360));
		ofRotateX(ofRandom(360));

		int sphere_color_index = ofRandom(this->color_palette.size());
		int face_color_index = (sphere_color_index + 1) % this->color_palette.size();
		int frame_color_index = (face_color_index + 1) % this->color_palette.size();

		ofSetColor(this->color_palette[sphere_color_index]);
		ofDrawSphere(size * 0.45);

		this->setRingToMesh(this->face, this->frame, location, size * 0.5, size * 0.2, this->color_palette[face_color_index], this->color_palette[frame_color_index]);

		ofPopMatrix();
	}

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height, ofColor face_color, ofColor frame_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 10) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 10) * DEG_TO_RAD), radius * sin((deg + 10) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 10) * DEG_TO_RAD), radius * sin((deg + 10) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value_x = ofNoise(
				location.x, radius * 0.0008 + ofGetFrameNum() * 0.005);
			auto noise_value_y = ofNoise(
				location.y, radius * 0.0008 + ofGetFrameNum() * 0.005);
			auto noise_value_z = ofNoise(
				location.z, radius * 0.0008 + ofGetFrameNum() * 0.005);

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(noise_value_x, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(noise_value_y, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(noise_value_z, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}