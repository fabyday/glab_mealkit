//MIT License
//
//Copyright(c) 2021 mat
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this softwareand associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright noticeand this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.



#include "viewer.h"
#include <cstdarg>
#include <utility>

#include <igl/png/writePNG.h>
#include <functional>
//#include <igl/embree/EmbreeRenderer.h>
#include "thread_configure.h"
#include <omp.h>

static void set_data_visible_all_core(bool visible, Viewer::iglViewer& v, int data_id) {
	for (const auto& i : v.core_list) {
			v.data(data_id).set_visible(visible, i.id);
	}
}


void Viewer::run(u8Encoder& enc, const std::vector<std::vector<std::string>>& f_names) {

	
	for (int i = 0; i < f_names.size(); i++) {
		std::cout << f_names[i].size() << std::endl;
		std::cout << f_names[(i + 1) % f_names.size()].size() << std::endl;
		assert((f_names[i].size() == f_names[(i + 1) % f_names.size()].size()) && "f_size is diff!");
	}
	__m_viewer.core().is_animating = true;
	int global_idx = 0;


	const int h = enc.get_height();
	const int w = enc.get_width();
	//__m_viewer.resize(h, w);
	int view_size = f_names.size();
	int mesh_elem_size = f_names[0].size();
	const int partial_width = w / view_size;

	//core init
	std::vector<int> core_ids;
	while (__m_viewer.erase_core(0)); //clean data container
	core_ids.push_back(__m_viewer.core().id);
	for (int i = 0; i < f_names.size(); i++){
		core_ids.push_back(__m_viewer.append_core(Eigen::Vector4f(i*partial_width, 0, partial_width, h),true));
	}

	//data init
	std::vector<int> d_ids;
	while (__m_viewer.erase_mesh(0)); //clean mesh container
	d_ids.push_back(__m_viewer.data().id);
	for (int i = 0; i < f_names.size(); i++){
		d_ids.push_back(__m_viewer.append_mesh());
		set_data_visible_all_core(false, __m_viewer, d_ids[i]);
		__m_viewer.data(d_ids[i]).set_visible(true, core_ids[i]);
	}
	



	// buffer
	u8Encoder::Dense R(w, h), G(w, h), B(w, h), A(w, h);
	R.setZero(); G.setZero(); B.setZero(); A.setConstant(255);



	enc.init();
	
	
	std::vector<std::vector<std::pair<Eigen::MatrixXd, Eigen::MatrixXi>>> s;
	int size_ = mesh_elem_size = 100;
	s.resize(view_size);


	for (auto& t : s) {
		t.resize(size_);
	}

	GLAB_OMP_SET_NUM_THREADS(GLAB_THREAD_NUM);
		#ifdef OPENMP
			#pragma omp parallel
			#pragma omp for
		#endif // OPENMP
		for (int i = 0; i < size_; i++) {
			//for (int i = 0; i < size; i++) {
				//int tid = omp_get_thread_num();
				//std::cout << "num : "<< i << "id : " << omp_get_thread_num() << std::endl;
				if(i % 100 == 0 )
					std::cout << i << std::endl;
				for (int j = 0; j < view_size; j++) {
					Eigen::MatrixXd V;
					Eigen::MatrixXi F;
					igl::read_triangle_mesh(f_names[j][i], V, F);
					s[j][i].first = V;
					s[j][i].second = F;

				}
		}

	Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> tmp_R(partial_width, h), tmp_G(partial_width, h), tmp_B(partial_width, h), tmp_A(partial_width, h);
	__m_viewer.callback_post_resize =
		[&](igl::opengl::glfw::Viewer& v, int w, int h) {
		*(const_cast<int*>(&partial_width)) = w / f_names.size();

		for (int i = 0; i < f_names.size(); i++) {
			v.core(core_ids[i]).viewport = Eigen::Vector4f(i * partial_width, 0, partial_width, h);
			
		}
		R.resize(w, h);		G.resize(w, h);		B.resize(w, h);		A.resize(w, h);
		
		tmp_R.resize(partial_width, h);		tmp_G.resize(partial_width, h);
		tmp_B.resize(partial_width, h);		tmp_A.resize(partial_width, h);
		return true;
	};



	bool flag = true;
	__m_viewer.callback_pre_draw = 
									[&, this](Viewer::iglViewer& v)->bool
									//, & enc, & global_idx
									{
										for (int view = 0; view < view_size; view++) {
											v.data(d_ids[view] ).set_mesh(s[view][global_idx].first, s[view][global_idx].second);
											if(flag)
												__m_viewer.core(core_ids[view]).align_camera_center(s[view][global_idx].first);
											
											tmp_R.setZero(); tmp_G.setZero(); tmp_B.setZero(); tmp_A.setZero();
											v.core(core_ids[view]).draw_buffer(v.data(d_ids[view]), false, tmp_R, tmp_G, tmp_B, tmp_A);
											R.block(partial_width * view, 0, partial_width, h) = tmp_R.cast<u8Encoder::TYPE>();
											G.block(partial_width * view, 0, partial_width, h) = tmp_G.cast<u8Encoder::TYPE>();
											B.block(partial_width * view, 0, partial_width, h) = tmp_B.cast<u8Encoder::TYPE>();
											A.block(partial_width * view, 0, partial_width, h) = tmp_A.cast<u8Encoder::TYPE>();
			
											//igl::png::writePNG(tmp_R, tmp_G, tmp_B, tmp_A, "./out"+std::to_string(view)+".png");
										}
										flag = false;
										//igl::png::writePNG(R.cast<unsigned char>(), G.cast<unsigned char>(), B.cast<unsigned char>(), A.cast<unsigned char>(), "./out_global"+std::to_string(global_idx) +".png");
	
										enc.add_frame(R, G, B, A);
										if (global_idx == mesh_elem_size - 1) {
											glfwSetWindowShouldClose(v.window, GL_TRUE);
											return false;
										}
										global_idx++;


										return false; 
									};

	__m_viewer.launch(false,false, "I'm Watching, buddy.", w, h);
	std::cout << "push... end" << std::endl;
	enc.end();
	std::cout << "endcode... end" << std::endl;
	////enc.encode("test.webm", R_list, G_list, B_list, A_list);
	
	
	/*
	viewer.draw() CALL is frame write function.
	My Guess is, if we want to draw off-screen rendering. how about using this draw method?(JUST MY OPINION)
	*/
	//__m_viewer.draw()


	////igl::jet(error_compute(orig, pred), true, C);

	////
	////viewer.data().set_mesh(pred.V, pred.F);
	////viewer.data().set_colors(C);
	////viewer.data().show_lines = false;




}



