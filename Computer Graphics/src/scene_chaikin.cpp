#include "scene_chaikin.h"


void scene_chaikin::initVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	{

		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO); 
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		} 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	glBindVertexArray(0);
}

void scene_chaikin::defineVAO() {
	glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * v.size(), v.data(), GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	glBindVertexArray(0);
}

void scene_chaikin::chaikinize(bool in_a_loop = false) {
	float threefourths = 3.0f / 4.0f;
	float onefourth = 1.0f / 4.0f;
	std::vector<cgmath::vec2> chaikinized;

	// for each geometry
	for (int range = 0; range < firsts.size(); range++) {
		int count = counts[range];
		int first = firsts[range];

		int n_first = chaikinized.size();
		int n_count = 0;

		if (!in_a_loop) {
			chaikinized.push_back(v[first]);
			n_count++;
		}

		// for each vertex in the geometry minus last one
		for (int j = first; j < first + count - 1; j++) {
			cgmath::vec2 p1 = v[j];
			cgmath::vec2 p0 = v[j + 1];

			chaikinized.push_back(onefourth * p0 + threefourths * p1);
			chaikinized.push_back(threefourths * p0 + onefourth * p1);

			n_count += 2;
		}
		if (!in_a_loop) {
			chaikinized.push_back(v[first + count - 1]);
			n_count++;
		}

		counts[range] = n_count;
		firsts[range] = n_first;
	}

	v = chaikinized;
}

void scene_chaikin::initDots() {
	glGenVertexArrays(1, &dotsVAO);
	glBindVertexArray(dotsVAO);
	{
		glGenBuffers(1, &dotsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, dotsVBO);
		{
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBufferData(GL_ARRAY_BUFFER, sizeof cgmath::vec2 * orig.size(), orig.data(), GL_DYNAMIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void scene_chaikin::densifyDots(int factor) {
	float slice = 1.0f / (float)factor;
	std::vector<cgmath::vec2> dense;

	for (int region = 0; region < o_firsts.size(); region++) {

		int first = o_firsts[region];
		int count = o_counts[region];

		int n_first = dense.size();
		int n_count = 0;

		for (int i = first; i < first + count - 1; i++) {
			auto p0 = orig[i];
			auto p1 = orig[i + 1];

			dense.push_back(p0);
			n_count++;

			for (int j = 1; j < factor; j++) {
				float slices_l = j * slice;
				float slices_r = 1.0f - slices_l;
				dense.push_back(slices_r * p0 + slices_l * p1);
				n_count++;
			}
		}
		dense.push_back(orig[orig.size() - 1]);
		n_count++;
		
		o_firsts[region] = n_first;
		o_counts[region] = n_count;
	}

	orig = dense;
}

void scene_chaikin::add_segment(std::vector<cgmath::vec2>& segment) {
	float scale = (1.0f / 6.5f);
	auto disp = cgmath::vec2(0.0f, 1.3f);

	int start_at = orig.size();
	for (auto point : segment) orig.push_back((point + disp) * scale);

	firsts.push_back(start_at); o_firsts.push_back(start_at);
	counts.push_back( segment.size() ); o_counts.push_back(segment.size());
}

void scene_chaikin::init() {
	primitiveType = GL_POINTS;
	dotted = false;

	std::vector<cgmath::vec2> pickle;
	pickle.push_back(cgmath::vec2(2.62f, 2.54f) );
	pickle.push_back( cgmath::vec2(1.95f, -0.34f) );
	pickle.push_back( cgmath::vec2(1.12, -2.81) );
	pickle.push_back( cgmath::vec2(0.27f, -4.32f) );
	pickle.push_back( cgmath::vec2(-1.09f, -6.22f) );
	pickle.push_back( cgmath::vec2(-4.29f, -7.4f) );
	pickle.push_back( cgmath::vec2(-4.53f, -4.71f) );
	pickle.push_back( cgmath::vec2(-3.87f, -3.41f) );
	pickle.push_back( cgmath::vec2(-2.63f, -1.44f) );
	pickle.push_back( cgmath::vec2(-1.02f, 3.2f) );
	pickle.push_back( cgmath::vec2(-0.36f, 4.43f) );
	pickle.push_back( cgmath::vec2(0.66f, 5.04f) );
	pickle.push_back( cgmath::vec2(1.94f, 4.45f) );
	pickle.push_back( cgmath::vec2(2.44f, 3.75f) );
	pickle.push_back( cgmath::vec2(2.62f, 2.54f) );
	this->add_segment(pickle);

	std::vector<cgmath::vec2> eyebrow;
	eyebrow.push_back(cgmath::vec2(0.0f, 4.01f));
	eyebrow.push_back(cgmath::vec2(0.22f, 4.17f));
	eyebrow.push_back(cgmath::vec2(0.91f, 4.191f));
	eyebrow.push_back(cgmath::vec2(1.45f, 3.65f));
	eyebrow.push_back(cgmath::vec2(1.51f, 3.22f));
	eyebrow.push_back(cgmath::vec2(0.96f, 3.91f));
	eyebrow.push_back(cgmath::vec2(0.42f, 3.97f));
	eyebrow.push_back(cgmath::vec2(0.01f, 3.88f));
	eyebrow.push_back(cgmath::vec2(0.0f, 4.01f));
	this->add_segment(eyebrow);

	std::vector<cgmath::vec2> left_eye;
	left_eye.push_back( cgmath::vec2(-0.57f, 3.17f) );
	left_eye.push_back(cgmath::vec2(-0.24f, 3.68f));
	left_eye.push_back(cgmath::vec2(0.58f, 3.28f));
	left_eye.push_back(cgmath::vec2(0.27f, 2.53f));
	left_eye.push_back(cgmath::vec2(-0.52f, 2.51f));
	left_eye.push_back(cgmath::vec2(-0.57f, 3.17f));
	this->add_segment(left_eye);


	std::vector<cgmath::vec2> left_pupil;
	left_pupil.push_back(cgmath::vec2(-0.15f, 3.16f));
	left_pupil.push_back(cgmath::vec2(-0.1f, 3.08f));
	left_pupil.push_back(cgmath::vec2(-0.18f, 3.07f));
	left_pupil.push_back(cgmath::vec2(-0.23f, 3.14f));
	left_pupil.push_back(cgmath::vec2(-0.15f, 3.16f));
	this->add_segment(left_pupil);


	std::vector<cgmath::vec2> right_eye;
	right_eye.push_back(cgmath::vec2(0.55f, 3.22f));
	right_eye.push_back(cgmath::vec2(1.18f, 3.23f));
	right_eye.push_back(cgmath::vec2(1.53f, 2.86f));
	right_eye.push_back(cgmath::vec2(1.34f, 2.35f));
	right_eye.push_back(cgmath::vec2(0.46f, 2.34f));
	right_eye.push_back(cgmath::vec2(0.38f, 2.72f));
	right_eye.push_back(cgmath::vec2(0.55f, 3.22f));
	this->add_segment(right_eye);

	std::vector<cgmath::vec2> right_pupil;
	right_pupil.push_back(cgmath::vec2(0.98f, 2.83f));
	right_pupil.push_back(cgmath::vec2(1.01f, 2.73f));
	right_pupil.push_back(cgmath::vec2(0.96f, 2.65f));
	right_pupil.push_back(cgmath::vec2(0.88f, 2.72f));
	right_pupil.push_back(cgmath::vec2(0.98f, 2.83f));
	this->add_segment(right_pupil);

	std::vector<cgmath::vec2> nose;
	nose.push_back(cgmath::vec2(0.13f, 2.53f));
	nose.push_back(cgmath::vec2(-0.03f, 2.31f));
	//nose.push_back(cgmath::vec2(-0.08, 2.24f));
	nose.push_back(cgmath::vec2(-0.07f, 2.16f));
	nose.push_back(cgmath::vec2(-0.02f, 1.91f));
	nose.push_back(cgmath::vec2(0.33f, 2.28f));
	this->add_segment(nose);

	std::vector<cgmath::vec2> left_eyebag;
	left_eyebag.push_back(cgmath::vec2(-0.55, 2.6f));
	left_eyebag.push_back(cgmath::vec2(-0.43f, 2.43f));
	left_eyebag.push_back(cgmath::vec2(-0.15f, 2.4f));
	this->add_segment(left_eyebag);



	v = orig;

	this->densifyDots(10);
	this->initDots();
	
	this->initVAO();
	this->defineVAO();
}

void scene_chaikin::awake() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(5.0f);
}

void scene_chaikin::mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao); 
	{
		glMultiDrawArrays(primitiveType, firsts.data(), counts.data(), counts.size());
	}
	glBindVertexArray(0);

	if (dotted) {
		glBindVertexArray(dotsVAO);
		{
			glMultiDrawArrays(GL_LINES, o_firsts.data(), o_counts.data(), o_counts.size());
		}
		glBindVertexArray(0);
	}
}

void scene_chaikin::normalKeysDown(unsigned char key) {
	if (key == '1') primitiveType = GL_POINTS;
	if (key == '2') primitiveType = GL_LINES;
	if (key == '3') primitiveType = GL_LINE_STRIP;
	if (key == '4') primitiveType = GL_LINE_LOOP;
	if (key == '5') primitiveType = GL_TRIANGLES;
	if (key == '6') primitiveType = GL_TRIANGLE_STRIP;
	if (key == '7') primitiveType = GL_TRIANGLE_FAN;

	if (key == 's') {
		this->chaikinize();
		this->defineVAO();
	}
	if (key == 'd') {
		dotted = !dotted;
	}
}