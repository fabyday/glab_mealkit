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




#include <iostream>
#include "encoder.h"
#include <vector>
#include <string>
#include "viewer.h"

using U8encoder = Encoder<>; //default

int main(int argc, char** argv) {
	/*
	prog root_dir fname_prefix_pattern1 fname_prefix_pattern2 ... fname_prefix_pattern_n
	*/
	std::string root = argv[1];
	std::vector<std::string> fname_prefix;
	fname_prefix.reserve(argc - 1);
	for (int i = 2; i < argc; i++) {
		fname_prefix.push_back(argv[i]);
	}



	
	






	std::cout << "init ..." << std::endl;
	U8encoder e("test");
	Viewer s(1920, 1080,  false);
	s.run(e, fname_prefix);


	
	std::cout << "end ..." << std::endl;






	return 0;
}