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
#include <Eigen/Dense>
#include <vector>
#include <thread>
#include <chrono>


// ffmpeg C library headers
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}

// see https://stackoverflow.com/questions/46444474/c-ffmpeg-create-mp4-file
// see https://github.com/shi-yan/videosamples/blob/master/libavmp4encoding/main.cpp
#if  !defined(__ENCODER_H__)
#define __ENCODER_H__

#define GLAB_DEFAULT_FPS 30
#define GLAB_DEFAULT_BIT_RATE 2000
#define GLAB_DEFAULT_WIDTH 1920
#define GLAB_DEFAULT_HEIGHT 1080
#define GLAB_DEFAULT_EXT "mp4"

template<typename data_size=uint8_t>
class Encoder {

	/*
		encoder


		RAII Pattern...

	*/

private:
	std::string __name; // file ext
	std::string __m_ext; // file ext
	int __m_bitrate;
	int __m_fps;
	int __m_width;
	int __m_height;


	AVFrame* videoFrame = nullptr;
	AVCodecContext* cctx = nullptr;
	SwsContext* swsCtx = nullptr;
	int frameCounter = 0;
	AVFormatContext* ofctx = nullptr;
	AVOutputFormat* oformat = nullptr;
	AVStream* stream = nullptr;


public:
	typename typedef data_size TYPE ;

	using Dense = Eigen::Matrix<data_size, Eigen::Dynamic, Eigen::Dynamic>;
	using vec = std::vector<Dense>;

	//typename typedef datasize data_size;


	int get_height() { return __m_height } const;
	int get_width() { return __m_width } const;


	Encoder(
			std::string name,
			std::string ext = GLAB_DEFAULT_EXT, 
			int width = GLAB_DEFAULT_WIDTH, 
			int height = GLAB_DEFAULT_HEIGHT, 
			int __m_fps = GLAB_DEFAULT_FPS, 
			int bit_rate = GLAB_DEFAULT_BIT_RATE
			)
			:__m_name(name),__m_ext(ext), __m_width(width), __m_height(height)
	{
		
	};

	
	int encode( std::string fname,
				const vec& R,
				const vec& G,
				const vec& B,
				const vec& A);
	//int add_frame(const vec& R,
	//			const vec& G,
	//			const vec& B,
	//			const vec& A);

	~Encoder() {
		finish();
		free();
	}

private:
	void pushFrame(data_size* data);
	//void init();



	void free();
	void finish();






};

#include "encoder.hpp"
#endif


