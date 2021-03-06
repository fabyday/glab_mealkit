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