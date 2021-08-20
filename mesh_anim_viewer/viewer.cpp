#include "viewer.h"
#include <cstdarg>
#include <utility>
void Viewer::run(u8Encoder& enc, const std::vector<std::vector<std::string>>& f_names) {
	

	for (int i = 0; i < f_names.size(); i++)
		assert(f_names[i].size() != f_names[(i + 1) % f_names.size()].size() && "f_size is diff!");
	
	
	const int h = enc.get_height();
	const int w = enc.get_width();
	
	int view_size = f_names.size();
	int size = f_names[0].size();

	std::vector<int> d_ids;
	for (int i = 0; i < f_names.size(); i++)
		d_ids.push_back(__m_viewer.append_mesh());

	// buffer
	u8Encoder::Dense R(w, h), G(w, h), B(w, h), A(w, h);


	u8Encoder::vec R_list; R_list.reserve(size);
	u8Encoder::vec G_list; G_list.reserve(size);
	u8Encoder::vec B_list; B_list.reserve(size);
	u8Encoder::vec A_list; A_list.reserve(size);

	const int partial_width = w / view_size;
	__m_viewer.core().viewport = Eigen::Vector4f(0, 0, partial_width, h);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < view_size; j++){
			Eigen::MatrixXd V;
			Eigen::MatrixXi F;
			
			igl::read_triangle_mesh(f_names[j][i], V, F);
			__m_viewer.data(d_ids[j]).set_mesh(V, F);
			__m_viewer.core().align_camera_center(V);
			__m_viewer.core().draw_buffer(__m_viewer.data( d_ids[j] ), false, 
											std::forward<u8Encoder::Dense>(R.block(partial_width * j,0, partial_width,h)), 
											std::forward<u8Encoder::Dense>(G.block(partial_width * j, 0, partial_width, h)),
											std::forward<u8Encoder::Dense>(B.block(partial_width * j, 0, partial_width, h)),
											std::forward<u8Encoder::Dense>(A.block(partial_width * j, 0, partial_width, h)));

		}
		R_list.push_back(R);
		G_list.push_back(G);
		B_list.push_back(B);
		A_list.push_back(A);
		
	}

	enc.encode("test.webm", R_list, G_list, B_list, A_list);



	//igl::jet(error_compute(orig, pred), true, C);

	//
	//viewer.data().set_mesh(pred.V, pred.F);
	//viewer.data().set_colors(C);
	//viewer.data().show_lines = false;




}




void caputre() {

}