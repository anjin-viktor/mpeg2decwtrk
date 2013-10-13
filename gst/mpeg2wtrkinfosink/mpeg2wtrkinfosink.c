#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "mpeg2wtrkinfosink.h"

#include <string.h>
#include <stdio.h>



static FILE *
gst_fopen (const gchar * filename, const gchar * mode)
{
#ifdef G_OS_WIN32
  wchar_t *wfilename = g_utf8_to_utf16 (filename, -1, NULL, NULL, NULL);
  wchar_t *wmode;
  FILE *retval;
  int save_errno;

  if (wfilename == NULL) {
    errno = EINVAL;
    return NULL;
  }

  wmode = g_utf8_to_utf16 (mode, -1, NULL, NULL, NULL);

  if (wmode == NULL) {
    g_free (wfilename);
    errno = EINVAL;
    return NULL;
  }

  retval = _wfopen (wfilename, wmode);
  save_errno = errno;

  g_free (wfilename);
  g_free (wmode);

  errno = save_errno;
  return retval;
#else
  return fopen (filename, mode);
#endif
}

static GstStaticPadTemplate sink_template =
GST_STATIC_PAD_TEMPLATE ("sink", GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/mpeg, "
        "mpegversion = (int)2, " 
        "parsed = (boolean) true, "
        "systemstream = (boolean) false")
    );

GST_DEBUG_CATEGORY_STATIC (gst_mpeg2_wtrk_info_sink_debug);
#define GST_CAT_DEFAULT gst_mpeg2_wtrk_info_sink_debug

#define gst_mpeg2_wtrk_info_sink_parent_class parent_class


enum
{
  PROP_0,
  PROP_LOCATION
};

static void gst_mpeg2_wtrk_info_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_mpeg2_wtrk_info_sink_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);
static void gst_mpeg2_wtrk_info_sink_dispose (GObject * object);

static gboolean gst_mpeg2_wtrk_info_sink_start (GstBaseSink * sink);
static gboolean gst_mpeg2_wtrk_info_sink_stop (GstBaseSink * sink);
static GstFlowReturn gst_mpeg2_wtrk_info_sink_render (GstBaseSink * bsink,
    GstBuffer * buffer);


G_DEFINE_TYPE_WITH_CODE (GstMpeg2WtrkInfoSink, gst_mpeg2_wtrk_info_sink, GST_TYPE_BASE_SINK,
  GST_DEBUG_CATEGORY_INIT (gst_mpeg2_wtrk_info_sink_debug, "mpeg2_wtrk_info_insink", 0,
  "debug category for mpeg2_wtrk_info_sink element"));


static void
gst_mpeg2_wtrk_info_sink_class_init (GstMpeg2WtrkInfoSinkClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstBaseSinkClass *gstbasesink_class = GST_BASE_SINK_CLASS (klass);


  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "mpeg2wtrkinfosink", "Analyzer/Mpeg2", "Mpeg2 watermark bit position finder",
      "Viktor Anjin <viktor dot anjin at gmail dot com>");

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&sink_template));

  gobject_class->set_property = GST_DEBUG_FUNCPTR(gst_mpeg2_wtrk_info_sink_set_property);
  gobject_class->get_property = GST_DEBUG_FUNCPTR(gst_mpeg2_wtrk_info_sink_get_property);

  g_object_class_install_property (gobject_class, PROP_LOCATION,
      g_param_spec_string ("location", "File Location",
          "Location of the file for write info about watermark", NULL,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  gobject_class->dispose = GST_DEBUG_FUNCPTR(gst_mpeg2_wtrk_info_sink_dispose);


  gstbasesink_class->start = GST_DEBUG_FUNCPTR (gst_mpeg2_wtrk_info_sink_start);
  gstbasesink_class->stop = GST_DEBUG_FUNCPTR (gst_mpeg2_wtrk_info_sink_stop);
  gstbasesink_class->render = GST_DEBUG_FUNCPTR (gst_mpeg2_wtrk_info_sink_render);
}

static void
gst_mpeg2_wtrk_info_sink_init (GstMpeg2WtrkInfoSink * mpeg2_wtrk_info_sink)
{
  mpeg2_wtrk_info_sink->filename = NULL;
  mpeg2_wtrk_info_sink->file = NULL;

  gst_base_sink_set_sync (GST_BASE_SINK (mpeg2_wtrk_info_sink), FALSE);
}

static void
gst_mpeg2_wtrk_info_sink_dispose (GObject * object)
{
  GstMpeg2WtrkInfoSink *sink = GST_MPEG2_WTRK_INFO_SINK (object);

  g_free (sink->filename);
  sink->filename = NULL;

  G_OBJECT_CLASS (parent_class)->dispose (object);
}


static void
gst_mpeg2_wtrk_info_sink_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMpeg2WtrkInfoSink *sink;

  sink = GST_MPEG2_WTRK_INFO_SINK (object);

  switch (prop_id) 
  {
    case PROP_LOCATION:
    {
      const gchar *filename = g_value_get_string (value);
      if (!sink->file)
      {
        g_free (sink->filename);

        if (filename != NULL) 
        {
          sink->filename = g_strdup (filename);
          GST_INFO ("filename : %s", sink->filename);
        } 
        else 
          sink->filename = NULL;
      }
      else
      {
        g_warning ("Changing the `location' property on _mpeg2wtrkinfosink when a file is "
          "open is not supported.");
        g_set_error (NULL, GST_URI_ERROR, GST_URI_ERROR_BAD_STATE,
          "Changing the 'location' property on _mpeg2wtrkinfosink when a file is "
          "open is not supported");
      }
      break;
    }
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_mpeg2_wtrk_info_sink_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstMpeg2WtrkInfoSink *sink;
  sink = GST_MPEG2_WTRK_INFO_SINK (object);

  switch (prop_id) 
  {
    case PROP_LOCATION:
      g_value_set_string (value, sink->filename);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}


static GstFlowReturn
gst_mpeg2_wtrk_info_sink_render (GstBaseSink * bsink, GstBuffer * buf)
{
  GstMpeg2WtrkInfoSink *sink;
  GstMapInfo info;
  GstMpegVideoPacket packet;
  guint offset = 0;

  sink = GST_MPEG2_WTRK_INFO_SINK (bsink);
  gst_buffer_map (buf, &info, GST_MAP_READ);

  sink->current_pos += info.size;

  while (gst_mpeg_video_parse(&packet, info.data, info.size, offset))
  {
    if (packet.size == (guint)-1)
      packet.size = info.size - packet.offset;

    offset = packet.offset + packet.size;

    if (packet.type == GST_MPEG_VIDEO_PACKET_SEQUENCE)
    {
      gboolean res = gst_mpeg_video_packet_parse_sequence_header(&packet, &(sink -> sequence_hdr));
      if (res)
        sink -> seq_hdr_is_actual = TRUE;
    }

    if (packet.type >= GST_MPEG_VIDEO_PACKET_SLICE_MIN &&
        packet.type <= GST_MPEG_VIDEO_PACKET_SLICE_MAX) 
    {
      if (sink -> seq_hdr_is_actual)
      {
        GstMpegVideoSliceHdr slice_hdr;
        gboolean res = gst_mpeg_video_packet_parse_slice_header(&packet, &slice_hdr, 
                   &(sink -> sequence_hdr), NULL);
      }
      else
        GST_WARNING ("slice hdr before sequence hdr");
    }
  }

  gst_buffer_unmap (buf, &info);

  return GST_FLOW_OK;
}

static gboolean
gst_mpeg2_wtrk_info_sink_start (GstBaseSink * basesink)
{
  GstMpeg2WtrkInfoSink *sink;
  sink = GST_MPEG2_WTRK_INFO_SINK (basesink);

  if (sink->filename == NULL || sink->filename[0] == '\0')
    goto no_filename;

  sink->file = gst_fopen (sink->filename, "wb");

  if (sink->file == NULL)
    goto open_failed;

  sink->current_pos = 0;

  GST_DEBUG_OBJECT (sink, "opened file %s",
      sink->filename);

  sink -> seq_hdr_is_actual = FALSE;

  return TRUE;

no_filename:
  {
    GST_ELEMENT_ERROR (sink, RESOURCE, NOT_FOUND,
        ("No file name specified for writing."), (NULL));
    return FALSE;
  }
open_failed:
  {
    GST_ELEMENT_ERROR (sink, RESOURCE, OPEN_WRITE,
        ("Could not open file \"%s\" for writing.", sink->filename),
        GST_ERROR_SYSTEM);
    return FALSE;
  }
}

static gboolean
gst_mpeg2_wtrk_info_sink_stop (GstBaseSink * basesink)
{
  GstMpeg2WtrkInfoSink *sink;
  sink = GST_MPEG2_WTRK_INFO_SINK (basesink);

  if (sink->file) {
    if (fclose (sink->file) != 0)
      goto close_failed;

    GST_DEBUG_OBJECT (sink, "closed file");
    sink->file = NULL;
  }

  return TRUE;

close_failed:
  GST_ELEMENT_ERROR (sink, RESOURCE, CLOSE,
        ("Error closing file \"%s\".", sink->filename), GST_ERROR_SYSTEM);
  return TRUE;
}



static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "mpeg2wtrkinfosink", GST_RANK_NONE,
      GST_TYPE_MPEG2_WTRK_INFO_SINK);
}


#ifndef VERSION
#define VERSION 
#endif
#ifndef PACKAGE
#define PACKAGE "mpeg2decwtrk"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "Mpeg2 watermark info getter"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "https://github.com/anjin-viktor/mpeg2decwtrk"
#endif



GST_PLUGIN_DEFINE (/*major*/1,
    /*minor*/0,
    mpeg2wtrkinfosink,
    "Mpeg2 watermark info getter",
    plugin_init, "1.0", "MIT/X11", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
