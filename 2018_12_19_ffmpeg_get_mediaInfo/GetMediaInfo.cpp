#include <iostream>
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avutil.lib")

void getMediaThreadInfo(char **argv, int index, char *logName) {

	std::cout << std::endl << "-----------------Enter thread " << index << "-------------------" << std::endl;

	std::ofstream log;

	int ret = -1;
	char errobuf[1024] = { 0 };
	int errorBufSize = 1024;


	log.open(logName);

	// init 
	av_register_all();
	avformat_network_init();


	// alloc context
	log << "Run: " << argv[0] << ".exe " << argv[index] << std::endl << std::endl;
	AVFormatContext *fmtCtx = avformat_alloc_context();
	if (NULL == fmtCtx) {
		av_strerror(ret, errobuf, errorBufSize);
		log << "avformat_alloc_context failed: " << errobuf << std::endl;
		log.close();
		return;
	}


	// open input file
	ret = avformat_open_input(&fmtCtx, argv[index], NULL, NULL);
	if (ret != 0) {
		av_strerror(ret, errobuf, errorBufSize);
		log << "avformat_open_input error: " << errobuf << std::endl;
		log.close();
		return;
	}

	// find stream info
	ret = avformat_find_stream_info(fmtCtx, NULL);
	if (ret < 0) {
		av_strerror(ret, errobuf, errorBufSize);
		log << "avformat_find_stream_info error: " << errobuf << std::endl;
		return;
	}


	// get video and auido index
	int nbStreams = fmtCtx->nb_streams;
	int videoIndex = -1;
	int audioIndex = -1;
	for (int i = 0; i < nbStreams; i++) {
		if (AVMEDIA_TYPE_VIDEO == fmtCtx->streams[i]->codec->codec_type) {
			videoIndex = i;
		}
		if (AVMEDIA_TYPE_AUDIO == fmtCtx->streams[i]->codec->codec_type) {
			audioIndex = i;
		}
	}

	if (audioIndex != -1) {
		log << "[AudioIndex]: " << audioIndex << std::endl;
	}

	if (videoIndex != -1) {
		log << "[VideoIndex]: " << videoIndex << std::endl;
	}

	// get media info
	int width = fmtCtx->streams[videoIndex]->codec->width;
	int height = fmtCtx->streams[videoIndex]->codec->height;
	double duration = fmtCtx->duration;   // us
	int fps = fmtCtx->streams[videoIndex]->codec->framerate.num / fmtCtx->streams[videoIndex]->codec->framerate.den;

	int fileBitrate = fmtCtx->bit_rate;
	int videoCodec = fmtCtx->streams[videoIndex]->codec->codec_id;
	const AVCodecDescriptor *videoDesc = avcodec_descriptor_get(fmtCtx->streams[videoIndex]->codec->codec_id);
	const AVCodecDescriptor *audioDesc = avcodec_descriptor_get(fmtCtx->streams[audioIndex]->codec->codec_id);

	int audioBitrate = fmtCtx->streams[audioIndex]->codec->bit_rate;
	int audioSampleRate = fmtCtx->streams[audioIndex]->codec->sample_rate;
	int audioChannels = fmtCtx->streams[audioIndex]->codec->channels;

	int videoBitrate = fileBitrate - audioBitrate;

	log << "-----------------------VideoInfo---------------------" << std::endl;
	log << "[videoType]: " << std::endl;
	log << "[videoContainerType]: " << fmtCtx->iformat->name << ", [detail]: " << fmtCtx->iformat->long_name << std::endl;;
	log << "[VideoCodec]: " << videoDesc->name << ", [detail]" << videoDesc->long_name << std::endl;
	log << "[Duration]: " << duration / 1000.0 << "ms" << std::endl;
	log << "[width]:" << width << std::endl;
	log << "[height]:" << height << std::endl;
	log << "[VideoBitrate]:" << videoBitrate / 1024.0 << "kb/s" << std::endl;
	log << "[VideoFps]:" << fps << std::endl;
	log << "[VideoDarw]:" << "ÊÓÆµ¿í¸ß±È" << std::endl;
	log << "[VideoDrah]:" << "" << std::endl;
	log << "[PixelSarw]:" << "" << std::endl;
	log << "[PixelSarh]:" << "" << std::endl;




	log << "---------------------AudioInfo---------------------" << std::endl;
	log << "[VideoCodec]:" << audioDesc->name << ", [detail]:" << audioDesc->long_name << std::endl;
	log << "[AudioBitrate]:" << audioBitrate / 1024.0 << "kb/s" << std::endl;
	log << "[AudioSamplerate]:" << audioSampleRate << std::endl;
	log << "[AudioChannels]:" << audioChannels << std::endl;
	//double fileSize = (videoBitrate + audioBitrate) * (duration / 1000.0 / 1000.0) / 8 / 1024.0 / 1024.0; //MB
	double fileSize = (fileBitrate) * (duration / 1000.0 / 1000.0) / 8 / 1024.0 / 1024.0; //MB
	log << "[FileSize]:" << fileSize << "MB" << std::endl;


	av_dump_format(fmtCtx, 0, argv[index], 0);

	if (fmtCtx != NULL) {
		avformat_free_context(fmtCtx);
		fmtCtx = NULL;
	}

	std::cout << "-----------------Quit thread " << index << "-------------------" << std::endl << std::endl;

	log.close();
}