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



#if !defined(__FILEMANAGER_H__)
#define __FILEMANAGER_H__

#include <regex>
#include <iostream>
#include <map>

#include <algorithm>
#include <filesystem>


class FileManager {
	/*
		
	*/
public : 
	using vec = std::vector<std::string>;
	using map = std::map<std::string, std::vector<std::string>>;

private:
	std::string root_path; // root file name
	bool recursive_search;
	vec prefix;
	std::string f_ext;
	map result;
	std::vector<vec> result_vec; // same as map type result.


public :

	FileManager(const std::string& root_path_, const vec& prefix_, const std::string f_ext_, const bool recursive_search_ = false)
		:prefix(prefix_), root_path(root_path_), recursive_search(recursive_search_), f_ext(f_ext_) {

	};

	bool compile();


	std::vector<vec> get_all();

	FileManager::vec& FileManager::operator[](const std::string& name);

	~FileManager() {

	}




};


#endif