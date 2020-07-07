#include "vdpch.h"

#include "Core/Application.h"

#if 0
extern "C" {
#include <ffmpeg/libavcodec/avcodec.h>
#include <ffmpeg/libavformat/avformat.h>
#include <ffmpeg/libavutil/avutil.h>
#include <ffmpeg/libavutil/frame.h>
}
#endif


extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}

int main(int argc, char* argv[]) {
#if _DEBUG || _RELEASE
	Logger::init();
#endif	
	
	auto* app = new Application(1280, 720, "Thermocheck App");
	app->run();
	delete app;
	
	return 0;
}

#if 0

bool load_frame(const std::string& filename, ) {
	AVFormatContext* av_format_ctx = avformat_alloc_context();
	if(!av_format_ctx) {
		return false;
	}

	if(avformat_open_input(&av_format_ctx, filename.c_str(), nullptr, nullptr) != 0) {
		return false;
	}

	int video_stream_index = -1;
	AVCodecParameters* av_codec_params = nullptr;
	AVCodec* av_codec = nullptr;
	for(int i = 0; i < av_format_ctx->nb_streams; ++i) {
		auto stream = av_format_ctx->streams[i];
		av_codec_params = av_format_ctx->streams[i]->codecpar;
		av_codec = avcodec_find_decoder(av_codec_params->codec_id);

		if(!av_codec) {
			continue;
		}

		if(av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
			video_stream_index = i;
			break;
		}
	}

	if(video_stream_index == -1) {
		return false;
	}

	AVCodecContext* av_codec_ctx = avcodec_alloc_context3(av_codec);
	if(!av_codec_ctx) {
		return false;
	}

	if(avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0) {
		return false;
	}

	if(avcodec_open2(av_codec_ctx, av_codec, nullptr) < 0) {
		return false;
	}

	AVFrame* av_frame = av_frame_alloc(); if(!av_frame) { return false; }
	AVPacket* av_packet = av_packet_alloc(); if(!av_packet) { return false; }

	while(av_read_frame(av_format_ctx, av_packet) >= 0) {
		if(av_packet->stream_index != video_stream_index) {
			continue;
		}
		int response = avcodec_send_packet(av_codec_ctx, av_packet);
		if(response < 0) {
			LOG_ERROR("Failed to decode packet: {}", av_err2str(response));
			return false;
		}
		response = avcodec_receive_frame(av_codec_ctx, av_frame);
		if(response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			continue;
		}
		if(response < 0) {
			LOG_ERROR("Failed to decode packet: {}", av_err2str(response));
		}

		av_packet_unref(av_packet);
		break;

	}



	avformat_close_input(&av_format_ctx);
	avformat_free_context(av_format_ctx);
	av_frame_free(&av_frame);
	av_packet_free(&av_packet);
	avcodec_free_context(&av_codec_ctx);

	return false;
}
#endif // 0
