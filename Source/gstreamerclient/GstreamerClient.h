#pragma once

extern "C" {

typedef struct _GstPad GstPad;
typedef struct _GstElement GstElement;

typedef enum SinkType_t {
    THUNDER_GSTREAMER_CLIENT_AUDIO,
    THUNDER_GSTREAMER_CLIENT_VIDEO,
    THUNDER_GSTREAMER_CLIENT_TEXT
} SinkType;

/**/
int gstreamer_client_link_sink (SinkType type, GstElement *pipeline, GstPad *srcPad);

/**/
int gstreamer_client_unlink_sink (SinkType type, GstElement *pipeline);

};
