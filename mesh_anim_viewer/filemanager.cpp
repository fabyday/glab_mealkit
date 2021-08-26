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



#include "filemanager.h"
#include "alphanum.h"

namespace fs = std::filesystem;

static bool compare_alphanum(const std::string& lhs, const std::string& rhs) {
	return alphanum_less<std::string>()(lhs, rhs);
}


bool FileManager::compile() {
	
	

	std::vector<std::regex> regex_list;
	std::string regex_postfix;
	regex_postfix.clear();
	if (*(f_ext.begin()) != '.')
		regex_postfix += ".";
	regex_postfix += f_ext; // make regex_postfix = ".ext"

	//prepare regex.
	regex_list.reserve(this->prefix.size());
	for (int i = 0; i < this->prefix.size(); i++) {
		regex_list.emplace_back("(" + this->prefix[i] +")\\w*(" + regex_postfix+")"); //(prefix)\w + regex_postfix
		result[this->prefix[i]] = std::move(FileManager::vec());
	}
	


	//searching
 	fs::directory_iterator dir_iter(this->root_path);
	for (fs::path p : dir_iter) {
		for (int i = 0; i < regex_list.size(); i++){
			if (std::regex_match(p.filename().string(),regex_list[i])){
				result[prefix[i]].emplace_back(p.string());
				break;
			}
		}
	}

	// sorting
	for (auto& r : result)
		std::stable_sort(r.second.begin(), r.second.end(), compare_alphanum);

	return true;
}


FileManager::vec& FileManager::operator[](const std::string& name){
	/*
	dictionary-like searching
	*/
	return this->result[name];
}



std::vector<FileManager::vec> FileManager::get_all() {
	std::vector<vec> re;
	std::transform(result.begin(), result.end(), std::back_inserter(re), [](auto& map_list) { return map_list.second; });
	return re;
}
