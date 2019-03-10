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

void scene_chaikin::chaikinize() {
	float threefourths = 3.0f / 4.0f;
	float onefourth = 1.0f / 4.0f;
	std::vector<cgmath::vec2> chaikinized;

	// for each geometry
	for (int range = 0; range < firsts.size(); range++) {
		bool in_a_loop = inaloop[range];
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
		} else {
			chaikinized.push_back(chaikinized[n_first]);
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

void scene_chaikin::add_segment(std::vector<cgmath::vec2>& segment, bool loopy) {
	float scale = (1.0f / 6.5f);
	auto disp = cgmath::vec2(0.0f, 1.3f);

	int start_at = orig.size();
	for (auto point : segment) orig.push_back((point + disp) * scale);

	firsts.push_back(start_at); o_firsts.push_back(start_at);
	counts.push_back( segment.size() ); o_counts.push_back(segment.size());
	inaloop.push_back(loopy);
}

void scene_chaikin::init() {
	primitiveType = GL_LINE_STRIP;
	dotted = false;
	curved = true;

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
	this->add_segment(pickle, true);

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
	this->add_segment(eyebrow, false);

	std::vector<cgmath::vec2> left_eye;
	left_eye.push_back( cgmath::vec2(-0.57f, 3.17f) );
	left_eye.push_back(cgmath::vec2(-0.24f, 3.68f));
	left_eye.push_back(cgmath::vec2(0.58f, 3.28f));
	left_eye.push_back(cgmath::vec2(0.27f, 2.53f));
	left_eye.push_back(cgmath::vec2(-0.52f, 2.51f));
	left_eye.push_back(cgmath::vec2(-0.57f, 3.17f));
	this->add_segment(left_eye, true);


	std::vector<cgmath::vec2> left_pupil;
	left_pupil.push_back(cgmath::vec2(-0.15f, 3.16f));
	left_pupil.push_back(cgmath::vec2(-0.1f, 3.08f));
	left_pupil.push_back(cgmath::vec2(-0.18f, 3.07f));
	left_pupil.push_back(cgmath::vec2(-0.23f, 3.14f));
	left_pupil.push_back(cgmath::vec2(-0.15f, 3.16f));
	this->add_segment(left_pupil, true);


	std::vector<cgmath::vec2> right_eye;
	right_eye.push_back(cgmath::vec2(0.55f, 3.22f));
	right_eye.push_back(cgmath::vec2(1.18f, 3.23f));
	right_eye.push_back(cgmath::vec2(1.53f, 2.86f));
	right_eye.push_back(cgmath::vec2(1.34f, 2.30f));
	right_eye.push_back(cgmath::vec2(0.46f, 2.30f));
	right_eye.push_back(cgmath::vec2(0.38f, 2.72f));
	right_eye.push_back(cgmath::vec2(0.55f, 3.22f));
	this->add_segment(right_eye, true);

	std::vector<cgmath::vec2> right_pupil;
	right_pupil.push_back(cgmath::vec2(0.98f, 2.83f));
	right_pupil.push_back(cgmath::vec2(1.01f, 2.73f));
	right_pupil.push_back(cgmath::vec2(0.96f, 2.65f));
	right_pupil.push_back(cgmath::vec2(0.88f, 2.72f));
	right_pupil.push_back(cgmath::vec2(0.98f, 2.83f));
	this->add_segment(right_pupil, true);

	std::vector<cgmath::vec2> nose;
	nose.push_back(cgmath::vec2(0.13f, 2.53f));
	nose.push_back(cgmath::vec2(-0.03f, 2.31f));
	nose.push_back(cgmath::vec2(-0.07f, 2.16f));
	nose.push_back(cgmath::vec2(-0.02f, 1.91f));
	nose.push_back(cgmath::vec2(0.33f, 2.28f));
	this->add_segment(nose, false);

	std::vector<cgmath::vec2> left_eyebag;
	left_eyebag.push_back(cgmath::vec2(-0.55, 2.6f));
	left_eyebag.push_back(cgmath::vec2(-0.43f, 2.43f));
	left_eyebag.push_back(cgmath::vec2(-0.15f, 2.4f));
	this->add_segment(left_eyebag, false);

	std::vector<cgmath::vec2> right_eyebag;
	right_eyebag.push_back(cgmath::vec2(0.57, 2.2f));
	right_eyebag.push_back(cgmath::vec2(0.94, 2.05f));
	right_eyebag.push_back(cgmath::vec2(1.2f, 2.15f));
	this->add_segment(right_eyebag, false);

	std::vector<cgmath::vec2> left_mouth_wrinkle;
	left_mouth_wrinkle.push_back(cgmath::vec2(-0.52, 1.22f));
	left_mouth_wrinkle.push_back(cgmath::vec2(-0.76, 1.37f));
	left_mouth_wrinkle.push_back(cgmath::vec2(-1.06, 1.75f));
	left_mouth_wrinkle.push_back(cgmath::vec2(-1.05, 2.33f));
	left_mouth_wrinkle.push_back(cgmath::vec2(-0.68, 2.45f));
	left_mouth_wrinkle.push_back(cgmath::vec2(-0.45, 2.4f));
	left_mouth_wrinkle.push_back(cgmath::vec2(-0.37, 2.32f));
	this->add_segment(left_mouth_wrinkle, false);

	std::vector<cgmath::vec2> right_mouth_wrinkle {
		{1.07, 1.80f},
		{1.22, 1.67f},
		{1.37, 1.27f},
		{1.02, 0.89f},
		{0.66, 0.84f}
	};
	this->add_segment(right_mouth_wrinkle, false);

	std::vector<cgmath::vec2> mouth {
		{-0.77f, 1.41f},
		{0.29f, 0.93f},
		{1.15f, 1.03f},
		{1.19f, 1.62f},
		{0.63f, 1.91f},
		{0.01, 1.84},
		{-0.65f, 2.4f},
		{-0.98, 2.05},
		{-0.77f, 1.41f},
	};
	this->add_segment(mouth, true);

	std::vector<cgmath::vec2> toungue {
		{0.63f, 1.0f},
		{0.52f, 1.31f},
		{0.12f, 1.51f},
		{0.64f, 1.54f},
		{0.78f, 1.2f},
		{0.8f, 1.04f}
	};
	this->add_segment(toungue, false);

	std::vector<cgmath::vec2> l_tooth_0{
		{-0.73, 1.53f},
		{-0.67f, 1.56f},
		{-0.59f, 1.5f},
		{-0.6f, 1.39f},
	};
	this->add_segment(l_tooth_0, false);

	std::vector<cgmath::vec2> l_tooth_1{
		{-0.56, 1.37f},
		{-0.44f, 1.51f},
		{-0.42f, 1.27f},
	};
	this->add_segment(l_tooth_1, false);

	std::vector<cgmath::vec2> l_tooth_2{
		{-0.4, 1.27f},
		{-0.26f, 1.45f},
		{-0.22f, 1.19f},
	};
	this->add_segment(l_tooth_2, false);

	std::vector<cgmath::vec2> l_tooth_3{
		{-0.2, 1.17f},
		{-0.06f, 1.36f},
		{-0.02f, 1.09f},
	};
	this->add_segment(l_tooth_3, false);

	std::vector<cgmath::vec2> l_tooth_4{
		{-0.01, 1.09f},
		{0.15, 1.27f},
		{0.2f, 1.03f},
	};
	this->add_segment(l_tooth_4, false);

	std::vector<cgmath::vec2> l_tooth_5{
		{0.24, 1.01f},
		{0.36, 1.26f},
		{0.45f, 0.99f},
	};
	this->add_segment(l_tooth_5, false);

	std::vector<cgmath::vec2> l_tooth_6{
		{0.47, 0.99f},
		{0.53, 1.15f},
		{0.6f, 0.99f},
	};
	this->add_segment(l_tooth_6, false);

	std::vector<cgmath::vec2> u_tooth_0{
		{-0.51, 2.25f},
		{-0.51, 2.12f},
		{-0.38f, 2.15f},
	};
	this->add_segment(u_tooth_0, false);

	std::vector<cgmath::vec2> u_tooth_1{
		{-0.36, 2.13f},
		{-0.39, 1.94f},
		{-0.19f, 1.99f},
	};
	this->add_segment(u_tooth_1, false);

	std::vector<cgmath::vec2> u_tooth_2{
		{-0.17, 1.97f},
		{-0.23, 1.9f},
		{-0.23f, 1.76f},
		{-0.1f, 1.78f},
		{-0.01f, 1.89f},
	};
	this->add_segment(u_tooth_2, false);

	std::vector<cgmath::vec2> u_tooth_3{
		{-0.01, 1.89f},
		{-0.03, 1.75f},
		{0.02f, 1.68f},
		{0.09f, 1.74f},
		{0.12f, 1.9f},
	};
	this->add_segment(u_tooth_3, false);

	std::vector<cgmath::vec2> u_tooth_4{
		{0.15, 1.85f},
		{0.25, 1.67f},
		{0.36f, 1.88f},
	};
	this->add_segment(u_tooth_4, false);

	std::vector<cgmath::vec2> u_tooth_5{
		{0.37, 1.86f},
		{0.47, 1.69f},
		{0.58f, 1.86f},
	};
	this->add_segment(u_tooth_5, false);

	std::vector<cgmath::vec2> u_tooth_6{
		{0.6f, 1.86f},
		{0.71f, 1.77f},
		{0.78f, 1.81f},
	};
	this->add_segment(u_tooth_6, false);

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
	if (curved) {
		glBindVertexArray(vao);
		{
			glMultiDrawArrays(primitiveType, firsts.data(), counts.data(), counts.size());
		}
		glBindVertexArray(0);
	}
	

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
	if (key == 'c') {
		curved = !curved;
	}
}