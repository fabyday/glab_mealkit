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


#define COMMENT(X)
 template <typename data_size>   
 COMMENT("DEPRECATED")
 int Encoder<data_size>::encode(std::string fname,
                                const vec& R,
                                const vec& G,
                                const vec& B,
                                const vec& A) 
{
     init(fname);

    const int color_term = this->__m_width * this->__m_height;
    uint8_t* frameraw = new uint8_t[this->__m_width * this->__m_height * 4];
    memset(frameraw, 222, color_term * 4);
    

    //check max size
    std::vector size_checker{ R.size(), G.size(), B.size(), A.size() };
    std::sort(size_checker.begin(), size_checker.end());
    const int max_length = size_checker.back();

    for (int i = 0; i < max_length; ++i) {
        //cannot use static_cast bacause of below issue
        //  Error	C2440	'static_cast': cannot convert from 'const unsigned char *' to 'void *'
        // it's time to use black magic casting.
        // cast void* after unveil const, 
        if (R.size() > i)
            memcpy(static_cast<void*>(frameraw + (0) * color_term), static_cast<void *>(const_cast<unsigned char*>(R[i].data())), color_term*sizeof(data_size)); // R
        else 
            memset(static_cast<void*>(frameraw + (0) * color_term), 0, color_term * sizeof(data_size)); // R
        if(G.size() > i)
            memcpy(static_cast<void*>(frameraw + (1) * color_term), static_cast<void *>(const_cast<unsigned char*>(G[i].data())), color_term*sizeof(data_size)); // G 
        else
            memset(static_cast<void*>(frameraw + (1) * color_term), 0, color_term * sizeof(data_size)); // G 

        if(B.size() > i)
            memcpy(static_cast<void*>(frameraw + (2) * color_term), static_cast<void *>(const_cast<unsigned char*>(B[i].data())), color_term*sizeof(data_size)); // B
        else
            memset(static_cast<void*>(frameraw + (2) * color_term), 0, color_term * sizeof(data_size)); // B
        if(A.size() > i)
            memcpy(static_cast<void*>(frameraw + (3) * color_term), static_cast<void *>(const_cast<unsigned char*>(A[i].data())), color_term*sizeof(data_size)); // A
        else 
            memset(static_cast<void*>(frameraw + (3) * color_term), 0, color_term * sizeof(data_size)); // A

        pushFrame(frameraw);
    }

    delete[] frameraw;

    return 0;
}


template <typename data_size>
void Encoder<data_size>::pushFrame(data_size* data) {
    int err;
    if (!videoFrame) {
        videoFrame = av_frame_alloc();
        videoFrame->format = AV_PIX_FMT_YUV420P;
        videoFrame->width = cctx->width;
        videoFrame->height = cctx->height;
        if ((err = av_frame_get_buffer(videoFrame, 32)) < 0) {
            std::cout << "Failed to allocate picture" << err << std::endl;
            return;
        }
    }
    if (!swsCtx) {
        swsCtx = sws_getContext(cctx->width, cctx->height, AV_PIX_FMT_RGB24, cctx->width,
            cctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, 0, 0, 0);
    }
    int inLinesize[1] = { 3 * cctx->width };
    // From RGB to YUV
    sws_scale(swsCtx, (const uint8_t* const*)&data, inLinesize, 0, cctx->height,
        videoFrame->data, videoFrame->linesize);
    /*videoFrame->pts = (1.0 / 30.0) * 90000 * (frameCounter++);*/
#define ONE_SEC 1000
    videoFrame->pts = (cctx->time_base.num / cctx->time_base.den) * 1000 * frameCounter++;
    std::cout << videoFrame->pts << " " << cctx->time_base.num << " " <<
        cctx->time_base.den << " " << frameCounter << std::endl;
    if ((err = avcodec_send_frame(cctx, videoFrame)) < 0) {
        std::cout << "Failed to send frame" << err << std::endl;
        return;
    }
    AV_TIME_BASE;
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;
    pkt.flags |= AV_PKT_FLAG_KEY;
    if (avcodec_receive_packet(cctx, &pkt) == 0) {
        static int counter = 0;
        if (counter == 0) {
            FILE* fp = fopen("dump_first_frame1.dat", "wb");
            fwrite(pkt.data, pkt.size, 1, fp);
            fclose(fp);
        }
        /*std::cout << "pkt key: " << (pkt.flags & AV_PKT_FLAG_KEY) << " " <<
            pkt.size << " " << (counter++) << std::endl;*/
        uint8_t* size = ((uint8_t*)pkt.data);
        /*std::cout << "first: " << (int)size[0] << " " << (int)size[1] <<
            " " << (int)size[2] << " " << (int)size[3] << " " << (int)size[4] <<
            " " << (int)size[5] << " " << (int)size[6] << " " << (int)size[7] <<*/
            //std::endl;
        av_interleaved_write_frame(ofctx, &pkt);
        av_packet_unref(&pkt);
    }
}



template<typename data_size> 
void Encoder<data_size>::finish() {
    //DELAYED FRAMES
    AVPacket pkt;
    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    for (;;) {
        avcodec_send_frame(cctx, NULL);
        if (avcodec_receive_packet(cctx, &pkt) == 0) {
            av_interleaved_write_frame(ofctx, &pkt);
            av_packet_unref(&pkt);
        }
        else {
            break;
        }
    }

    av_write_trailer(ofctx);
    if (!(oformat->flags & AVFMT_NOFILE)) {
        int err = avio_close(ofctx->pb);
        if (err < 0) {
            std::cout << "Failed to close file" << err << std::endl;
        }
    }
}

template<typename data_size> 
void Encoder<data_size>::free() {
    if (videoFrame) {
        av_frame_free(&videoFrame);
    }
    if (cctx) {
        avcodec_free_context(&cctx);
    }
    if (ofctx) {
        avformat_free_context(ofctx);
    }
    if (swsCtx) {
        sws_freeContext(swsCtx);
    }
}

template<typename data_size>
bool Encoder<data_size>::end() {
    if (__m_is_record) {
        finish();
        free();
        if (__m_buffer != NULL)
            delete[] __m_buffer;

    }
    
    __m_is_record = false;
    return true;
}

template<typename data_size>
int Encoder<data_size>::init() {
    av_register_all();
    avcodec_register_all();
    std::string filename = __m_name + __m_ext;

    oformat = av_guess_format(nullptr, filename.c_str(), nullptr);
    if (!oformat) {
        std::cout << "can't create output format" << std::endl;
        return -1;
    }
    //oformat->video_codec = AV_CODEC_ID_H265;

    int err = avformat_alloc_output_context2(&ofctx, oformat, nullptr, filename.c_str());

    if (err) {
        std::cout << "can't create output context" << std::endl;
        return -1;
    }

    AVCodec* codec = nullptr;

    codec = avcodec_find_encoder(oformat->video_codec);
    if (!codec) {
        std::cout << "can't create codec" << std::endl;
        return -1;
    }

    stream = avformat_new_stream(ofctx, codec);

    if (!stream) {
        std::cout << "can't find format" << std::endl;
        return -1;
    }

    cctx = avcodec_alloc_context3(codec);

    if (!cctx) {
        std::cout << "can't create codec context" << std::endl;
        return -1;
    }
    //__m_fps = 25;
    stream->codecpar->codec_id = oformat->video_codec;
    stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    stream->codecpar->width = __m_width;
    stream->codecpar->height = __m_height;
    stream->codecpar->format = AV_PIX_FMT_YUV420P;
    stream->codecpar->bit_rate = __m_bitrate * 1000;
    //stream->avg_frame_rate = (AVRational){ __m_fps, 1 };
    stream->avg_frame_rate = AVRational({ __m_fps, 1 });
    avcodec_parameters_to_context(cctx, stream->codecpar);
    //cctx->time_base = (AVRational){ 1, 1 };
    cctx->time_base = AVRational({ 1, __m_fps });
    //cctx->max_b_frames = 2;
    //cctx->gop_size = 12;
    cctx->max_b_frames = 1;
    cctx->gop_size = 10;
    //Error	C4576	a parenthesized type followed by an initializer list is a non - standard explicit type conversion syntax
    //cctx->framerate = (AVRational){ __m_fps, 1 }; 
    cctx->framerate = AVRational({ __m_fps,1 });

    if (stream->codecpar->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(cctx, "preset", "ultrafast", 0);
    }
    else if (stream->codecpar->codec_id == AV_CODEC_ID_H265) {
        av_opt_set(cctx, "preset", "ultrafast", 0);
    }
    else
    {
        av_opt_set_int(cctx, "lossless", 1, 0);
    }

    avcodec_parameters_from_context(stream->codecpar, cctx);

    if ((err = avcodec_open2(cctx, codec, NULL)) < 0) {
        std::cout << "Failed to open codec" << err << std::endl;
        return -1;
    }

    if (!(oformat->flags & AVFMT_NOFILE)) {
        if ((err = avio_open(&ofctx->pb, filename.c_str(), AVIO_FLAG_WRITE)) < 0) {
            std::cout << "Failed to open file" << err << std::endl;
            return -1;
        }
    }

    if ((err = avformat_write_header(ofctx, NULL)) < 0) {
        std::cout << "Failed to write header" << err << std::endl;
        return -1;
    }

    av_dump_format(ofctx, 0, filename.c_str(), 1);
    std::cout << stream->time_base.den << " " << stream->time_base.num << std::endl;
    
    
    __m_is_record = true;
    __m_buffer = new data_size[__m_width * __m_height * 3];
    return 0;
}

template<typename data_size>
int Encoder<data_size>::add_frame(   Dense& R,
                                     Dense& G,
                                     Dense& B,
                                     Dense& A) 
{
    int color_term = __m_width * __m_height;
     //memset(__m_buffer, 222, color_term * 4);
     memset(static_cast<void*>(__m_buffer), 0, color_term * 3);


    //cannot use static_cast bacause of below issue
    //  Error	C2440	'static_cast': cannot convert from 'const unsigned char *' to 'void *'
    // it's time to use black magic casting.
    // cast void* after unveil const, 

#define R_INDEX 0
#define G_INDEX 1
#define B_INDEX 2
     std::cout << __m_height <<","<<__m_width << std::endl;
     for (int h = 0; h < __m_height; h++) {
         for (int w = 0; w < __m_width; w++) {
             *(__m_buffer + h * __m_width * 3 + 3 * w + R_INDEX) = R(w, __m_height - 1 - h);
             *(__m_buffer + h * __m_width * 3 + 3 * w + G_INDEX) = G(w, __m_height - 1 - h);
             *(__m_buffer + h * __m_width * 3 + 3 * w + B_INDEX) = B(w, __m_height - 1 - h);
         }
     }


    //// RGBRGB...RGB
    //for (int h = 0; h< __m_height; h++) {
    //    for(int w = 0; w < __m_width; w++){
    //        *(__m_buffer + h * __m_width + w * 3 + 0) = r_ptr[__m_height * w + h];
    //        *(__m_buffer + h * __m_width + w * 3 + 1) = g_ptr[__m_height * w + h];
    //        *(__m_buffer + h * __m_width + w * 3 + 2) = b_ptr[__m_height * w + h];
    //        //*(__m_buffer + color_term + 0) = a_ptr[__m_width * w + h * __m_height];
    //    }
    //}


    pushFrame(__m_buffer);
    
    return 0;
}