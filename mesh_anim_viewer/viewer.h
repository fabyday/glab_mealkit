#include <igl/opengl/glfw/Viewer.h>
#include "encoder.h"
#include <igl/jet.h>
#include <igl/read_triangle_mesh.h>

using u8Encoder = Encoder<>;

class Viewer {
public:
	using iglViewer = igl::opengl::glfw::Viewer;
	iglViewer __m_viewer;
	int __m_w;
	int __m_h;
	bool __m_visible;
	Viewer(int w=1920, int h=1080, bool visible=false)
		:__m_w(w), __m_h(h), __m_visible(visible), __m_viewer()
	{

	};



	void run(u8Encoder& enc, const std::vector<std::vector<std::string>>& f_names );

	



};