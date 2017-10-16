#ifndef __V4L2_UTIL_H__
#define __V4L2_UTIL_H__
#include <stdio.h>
#include <linux/videodev2.h>

typedef struct VideoBuffer {                          
	void *start;
	size_t length;
} VideoBuffer;


int cam_query_cap(int fd,struct v4l2_capability *cap);
int cam_enum_fmt(int fd,struct v4l2_fmtdesc * fmtdesc);
int cam_set_fmt(int fd,int w,int h);
int cam_get_fmt(int fd,struct v4l2_format * fmt);
int cam_req_buf(int fd,int count);
int cam_query_buf(int fd,int index,VideoBuffer * vbuffer);
int cam_q_buf(int fd,int index);
int cam_dq_buf(int fp);
int cam_stream_on(int fd);
int cam_stream_off(int fd);
int cam_poll(int fd,int ms);
int cam_free_buffer(VideoBuffer * vbuffers,int count);
#endif
