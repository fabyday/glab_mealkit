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
#include <utility>

// ffmpeg C library headers
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}

//https://en.wikipedia.org/wiki/YUV
#define WR 0.299
#define WB 0.114
#define WG 0.587 //1.0-WR-WB
#define U_MAX 0.436
#define V_MAX 0.615

#define Y(R,G,B) WR*(R) + WG*(G) + WB*(B)
#define U(R,G,B) 0.492*((B)-(Y((R),(G),(B)))) //approximation
#define V(R,G,B) 0.887*((R)-(Y((R),(G),(B)))) //approximation


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

		this program doesn't suuport mp4 format(H264, H265 ...)
		
		#######ONLY SUPPORT {webm} FORMAT!!!!
		
		
		if you want to use mp4 video format. just donwload x264 library yourself.


	*/

private:
	std::string __m_name; // file ext
	std::string __m_ext; // file ext
	int __m_bitrate;
	int __m_fps;
	int __m_width;
	int __m_height;
	bool __m_is_record;
	data_size* __m_buffer;

	AVFrame* videoFrame = nullptr;
	AVCodecContext* cctx = nullptr;
	SwsContext* swsCtx = nullptr;
	int frameCounter = 0;
	AVFormatContext* ofctx = nullptr;
	AVOutputFormat* oformat = nullptr;
	AVStream* stream = nullptr;


public:
	typedef data_size TYPE;

	using Dense = Eigen::Matrix<data_size, Eigen::Dynamic, Eigen::Dynamic>;
	using vec = std::vector<Dense>;

	
	//getter setter
	inline int get_height() { return __m_height; };
	inline int get_width() { return __m_width; };
	inline std::string get_name() { return __m_name; };
	inline std::string get_ext() { return __m_ext; };
	inline int get_fps() { return fps; };

	inline void set_name(std::string name) { __m_name = name; };
	inline void set_ext(std::string ext) { __m_ext = ext; };
	inline void set_width(int w) { __m_width = w; };
	inline void set_height(int h) { __m_height = h; };
	inline void set_fps(int fps) { __m_fps = fps; };
	
	/*
	name : output filename without extention.
	ext : extention
	width : width of video
	height : height of video
	fps : frame / sec, just simply write in second
	bit_rate : video frame bitrate.
	*/
	Encoder(
			std::string name,
			std::string ext = GLAB_DEFAULT_EXT, 
			int width = GLAB_DEFAULT_WIDTH, 
			int height = GLAB_DEFAULT_HEIGHT, 
			int fps = GLAB_DEFAULT_FPS, // unit sec
			int bit_rate = GLAB_DEFAULT_BIT_RATE
			)
			:__m_name(name),__m_ext(ext), __m_width(width), __m_height(height), __m_fps(fps), __m_is_record(false)
	{
		
	};

	//** all in one **/
	//encode := init + add_frame1 ... add_frame_n + end()
	int encode( std::string fname,
				const vec& R,
				const vec& G,
				const vec& B,
				const vec& A);


	int init();
	int add_frame( Dense& R,
				 Dense& G,
				 Dense& B,
				 Dense& A);
	bool end();
	
	~Encoder() {
		end();
	}

private:
	void pushFrame(data_size* data);
	//void init();



	void free();
	void finish();






};

#include "encoder.hpp"
#endif


