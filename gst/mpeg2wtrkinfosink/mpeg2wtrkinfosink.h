#ifndef GST_MPEG2_WTRK_INFO_SINK__
#define GST_MPEG2_WTRK_INFO_SINK__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

#include "gstmpegvideoparser/gstmpegvideoparser.h"

#include <stdio.h>

G_BEGIN_DECLS

#define GST_TYPE_MPEG2_WTRK_INFO_SINK \
  (gst_mpeg2_wtrk_info_sink_get_type())
#define GST_MPEG2_WTRK_INFO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MPEG2_WTRK_INFO_SINK, GstMpeg2WtrkInfoSink))
#define GST_MPEG2_WTRK_INFO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MPEG2_WTRK_INFO_SINK, GstMpeg2WtrkInfoSinkClass))
#define GST_IS_MPEG2_WTRK_INFO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MPEG2_WTRK_INFO_SINK))
#define GST_IS_MPEG2_WTRK_INFO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MPEG2_WTRK_INFO_SINK))
#define GST_MPEG2_WTRK_INFO_SINK_CAST(obj) ((GstMpeg2WtrkInfoSink *)obj)

typedef struct _GstMpeg2WtrkInfoSink GstMpeg2WtrkInfoSink;
typedef struct _GstMpeg2WtrkInfoSinkClass GstMpeg2WtrkInfoSinkClass;

struct _GstMpeg2WtrkInfoSink {
  GstBaseSink   element;
  gchar         *filename;
  FILE          *file;
  guint64       current_pos;
  
  GstMpegVideoSequenceHdr           sequence_hdr;
  gboolean                          seq_hdr_is_actual;
};

struct _GstMpeg2WtrkInfoSinkClass {
  GstBaseSinkClass parent_class;
};

G_GNUC_INTERNAL GType gst_mpeg2_wtrk_info_sink_get_type (void);

G_END_DECLS

#endif
