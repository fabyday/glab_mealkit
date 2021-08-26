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
#include <vector>
#include <string>
#include "viewer.h"
#include "encoder.h"
#include "filemanager.h"

using U8encoder = Encoder<>; //default

int main(int argc, char** argv) {
	/*
	prog output_file_path file extension input_root_dir fname_prefix_pattern1 fname_prefix_pattern2 ... fname_prefix_pattern_n
	*/
	std::string output_name = argv[1]; //output full path ex ) ./test
	std::string ext = argv[2];
	std::string root = argv[3]; // input root
	//int num = std::stoi(argv[4]);
	std::vector<std::string> fname_prefix;
	fname_prefix.reserve(argc - 4);
	for (int i = 4; i < argc; i++) {
		fname_prefix.push_back(argv[i]);
	}



	
	//file inflate
	FileManager fmg(root, fname_prefix, ".obj", false);
	fmg.compile();
	std::vector<FileManager::vec> fname_list = fmg.get_all();

	std::cout << "init ..." << std::endl;
	U8encoder e(output_name, ext, 1920, 1080, 1);
	Viewer view(800, 400,  false);
	view.run(e, fname_list);

	std::cout << "end ..." << std::endl;

	return 0;
}