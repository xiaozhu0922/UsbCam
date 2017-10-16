#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <linux/videodev2.h>
#include <utils/Log.h>
#include "v4l2_util.h"


int cam_query_cap(int fd,struct v4l2_capability *cap)
{
	int ret = ioctl(fd,VIDIOC_QUERYCAP,cap);
	if(ret<0)
		return ret;

	return 0;
}

int cam_enum_fmt(int fd,struct v4l2_fmtdesc * fmtdesc)
{
	int ret = ioctl(fd, VIDIOC_ENUM_FMT, fmtdesc);
	if(ret<0)
		return ret;

	return 0;
}

int cam_set_fmt(int fd,int w,int h)
{
	int ret = 0;
	struct v4l2_format fmt;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix.height = w;
	fmt.fmt.pix.width = h;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
	
	if(ret<0)
		return ret;

	return 0;
}

int cam_get_fmt(int fd,struct v4l2_format * fmt)
{
	int ret = 0;
	fmt->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_G_FMT, fmt);	
	if(ret<0)
		return ret;

	return 0;
}


int cam_req_buf(int fd,int count)
{
	struct v4l2_requestbuffers req;
	int ret = 0;
	memset(&req, 0, sizeof (req));
	req.count = count;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ret=ioctl(fd,VIDIOC_REQBUFS,&req) ;
	if(ret<0)
		return ret;

	return 0;
}

int cam_query_buf(int fd,int index,VideoBuffer * vbuffer)
{
	struct v4l2_buffer buf;
	int ret = 0;
	memset( &buf, 0, sizeof(buf) );
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = index;
	ret = ioctl(fd, VIDIOC_QUERYBUF, &buf) ;
	if(ret < 0)
		return ret;

	vbuffer->length = buf.length;
	vbuffer->start = mmap(NULL, buf.length,
		          PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		        
	if (vbuffer->start == MAP_FAILED) {
		printf("v4l2 mmap error,index=%d\n",index);
		return -1;
	}
	return 0;
}

int cam_q_buf(int fd,int index)
{
	struct v4l2_buffer buf;
	int ret = 0;
	
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = index;
	
	ret = ioctl(fd, VIDIOC_QBUF, &buf);
	if(ret<0)
		return ret;

	return 0;
}

int cam_dq_buf(int fp)
{
    struct v4l2_buffer buf;
    int ret;

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(fp, VIDIOC_DQBUF, &buf);
    if (ret < 0) {
        return ret;
    }

    return buf.index;
}

int cam_stream_on(int fd)
{
	enum v4l2_buf_type type;
	int ret = 0;
	
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;   
    ret = ioctl (fd, VIDIOC_STREAMON, &type);
    if(ret < 0)
		return ret;
	
	return 0;
}

int cam_stream_off(int fd)
{
	enum v4l2_buf_type type;
	int ret = 0;
	
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;   
    ret = ioctl (fd, VIDIOC_STREAMOFF, &type); 
    if(ret < 0)
		return ret;
	
	return 0;
}

int cam_poll(int fd,int ms)
{
	struct pollfd pollfds; 
	pollfds.fd = fd;
	pollfds.events = POLLIN|POLLPRI;
	return poll(&pollfds,1,ms);
}

int cam_free_buffer(VideoBuffer * vbuffers,int count)
{
	int i = 0;
	for(i=0;i<count;i++){
		if(munmap(vbuffers[i].start,vbuffers[i].length)<0){
			printf("====== munmap videobuffers error i=%d ======\n",i);
		}
		vbuffers[i].start=NULL;
		vbuffers[i].length=0;
	}
	free(vbuffers);
	return 0;
}
