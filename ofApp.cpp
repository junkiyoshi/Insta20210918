#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	vector<ofColor> color_list;
	ofColor color;
	vector<int> hex_list = { 0xff0000, 0xffed00, 0xff0092, 0xc2ff00, 0x00c7f2, 0xc1f1fc, 0xebffac, 0xffc2e5 };
	for (auto hex : hex_list) {

		color.setHex(hex);
		color_list.push_back(color);
	}

	float R = 300;
	float r = 100;
	int v_span = 10;
	int u_span = 10;

	this->walker_log_list.clear();
	this->walker_color_list.clear();
	for (int i = 0; i < 15; i++) {

		vector<glm::vec3> walker_location_list;
		auto noise_param = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		auto step_u = ofRandom(1, 1.5);
		auto step_v = ofRandom(1, 1.5);
		for (int k = 0; k < 40; k++) {

			auto tmp_r = ofMap(ofNoise(noise_param.x, (ofGetFrameNum() * 0.01 + k) * 0.005), 0, 1, 80, 96);
			auto tmp_u = noise_param.y + (ofGetFrameNum() + k) * step_u;
			auto tmp_v = noise_param.z + (ofGetFrameNum() + k) * step_v;

			walker_location_list.push_back(this->make_point(R, tmp_r, tmp_u, tmp_v));
		}

		this->walker_log_list.push_back(walker_location_list);
		this->walker_color_list.push_back(color_list[(int)ofRandom(color_list.size())]);

	}

	for (int v = 0; v < 360; v += v_span) {

		for (auto u = 0; u < 360; u += u_span) {

			auto location = this->make_point(R, r, u, v);
			auto flag = false;
			for (auto& walker_log : this->walker_log_list) {

				for (auto& walker_location : walker_log) {

					if (glm::distance(location, walker_location) < 35) {

						flag = true;
						break;
					}
				}
			}

			if (flag) {

				vector<glm::vec3> vertices;
				vertices.push_back(this->make_point(R, r, u, v - v_span * 0.5));
				vertices.push_back(this->make_point(R, r, u + u_span, v - v_span * 0.5));
				vertices.push_back(this->make_point(R, r, u + u_span, v + v_span * 0.5));
				vertices.push_back(this->make_point(R, r, u, v + v_span * 0.5));

				int index = face.getNumVertices();
				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); face.addIndex(index + 1); face.addIndex(index + 2);
				this->face.addIndex(index + 0); face.addIndex(index + 2); face.addIndex(index + 3);

				this->frame.addVertices(vertices);

				this->frame.addIndex(index + 0); this->frame.addIndex(index + 1);
				this->frame.addIndex(index + 1); this->frame.addIndex(index + 2);
				this->frame.addIndex(index + 2); this->frame.addIndex(index + 3);
				this->frame.addIndex(index + 3); this->frame.addIndex(index + 0);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(0);
	this->face.draw();

	ofSetColor(255);
	this->frame.drawWireframe();

	for (int i = 0; i < this->walker_log_list.size(); i++) {

		auto walker_log = this->walker_log_list[i];
		ofSetColor(this->walker_color_list[i]);

		ofFill();
		ofDrawSphere(walker_log.back(), 5);

		ofNoFill();
		ofBeginShape();
		for (auto& walker_location : walker_log) {

			ofVertex(walker_location);
		}
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}