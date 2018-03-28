/* this code is derived from an example by Mike Gran on in-memory PCM waveform generation
 * and playback using GStreamer. His example uses Gstreamer-0.10, see 
 * at https://github.com/spk121/burro-engine/blob/master/experiments/gstreamer-beep.c
 * This version updated by Troy D. Hanson for compatibility with gstreamer-1.0.
 */
#include <stdint.h>
#include <assert.h>
#include <gst/gst.h>
#include <gio/gio.h>

#define SAMPLE_RATE 22000
#define FREQ 440
#define DURATION 2
#define LEN (SAMPLE_RATE * DURATION)

static gboolean 
bus_call(GstBus *bus,
         GstMessage *msg,
         gpointer data)
{
  GMainLoop *loop = (GMainLoop*)data;
  switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
      g_print("end of stream\n");
      g_main_loop_quit(loop);
      break;
    case GST_MESSAGE_ERROR: {
      gchar *debug;
      GError *error;
      gst_message_parse_error(msg,&error,&debug);
      g_free(debug);
      g_printerr("Error: %s\n", error->message);
      g_error_free(error);
      g_main_loop_quit(loop);
      break;
    }
    default:
      break;
  }
  return TRUE;
}

int timer_callback (const void *data)
{
  g_main_loop_quit ((GMainLoop *) data);
  return FALSE;
}

int main (int argc, char ** argv)
{
  int i;
  uint8_t *wave;
  GMemoryInputStream *mistream;
  GstElement *source, *sink, *pipeline;
  GMainLoop *loop;
  GstBus *bus;
  guint bus_watch_id;

  gst_init (&argc, &argv);

  wave = g_new (uint8_t, LEN);
  for (i = 0; i < LEN; i ++)
  {
    if (i % 50 < 25)
      wave[i] = 255;
    else
      wave[i] = 0;
  }
  mistream = G_MEMORY_INPUT_STREAM(g_memory_input_stream_new_from_data(wave,
                       LEN, (GDestroyNotify) g_free));

  loop = g_main_loop_new (NULL, FALSE);

  pipeline = gst_pipeline_new ("beep-player");
  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  bus_watch_id = gst_bus_add_watch(bus,bus_call,loop);
  gst_object_unref(bus);

  source = gst_element_factory_make ("giostreamsrc", NULL);
  g_object_set (G_OBJECT (source), "stream", G_INPUT_STREAM (mistream), NULL);
  sink = gst_element_factory_make ("autoaudiosink", NULL);
  gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
  
  GstCaps *caps = gst_caps_new_simple ("audio/x-raw",
                 "format",   G_TYPE_STRING,   "U8",
                 "rate",     G_TYPE_INT,      22000,
                 "channels", G_TYPE_INT,      1,
                 //"layout",   G_TYPE_STRING,  "non-interleaved",
                 NULL);
  if (gst_element_link_filtered (source, sink, caps) == FALSE) g_printerr("link failed\n");
  gst_caps_unref(caps);
  
  //if (! gst_element_link_many (source, sink, NULL)) g_warning("Failed to link elements\n");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  g_timeout_add (2500, (GSourceFunc) timer_callback, loop);
  g_printerr("running main loop...\n");
  g_main_loop_run (loop);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));

  g_source_remove(bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}
