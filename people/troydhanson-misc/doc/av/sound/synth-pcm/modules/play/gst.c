#include <stdint.h>
#include <assert.h>
#include <gst/gst.h>
#include <gio/gio.h>

/*
 * The PCM synth example has a "frontend" and a "backend".
 * This is the "backend" which takes an already-built PCM
 * buffer and uses gstreamer to play it.
 *
 * entry point: play_pcm()
 * 
 * Troy D. Hanson, July 2014. 
 */
static gboolean 
gst_bus_call(GstBus *bus,
         GstMessage *msg,
         gpointer data)
{
  GMainLoop *loop = (GMainLoop*)data;
  switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_EOS:
      //g_print("end of stream\n");
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

int play_pcm (int argc, char ** argv, int16_t *pcm, size_t pcmlen, 
              int sample_rate, int verbose) 
{
  GMemoryInputStream *mistream;
  GstElement *source, *sink, *pipeline;
  GMainLoop *loop;
  GstBus *bus;
  guint bus_watch_id;

  gst_init (&argc, &argv);

  mistream = G_MEMORY_INPUT_STREAM(g_memory_input_stream_new_from_data(pcm,
                       pcmlen, (GDestroyNotify) g_free));

  loop = g_main_loop_new (NULL, FALSE);

  pipeline = gst_pipeline_new ("gst-player");
  bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  bus_watch_id = gst_bus_add_watch(bus,gst_bus_call,loop);
  gst_object_unref(bus);

  source = gst_element_factory_make ("giostreamsrc", NULL);
  g_object_set (G_OBJECT (source), "stream", G_INPUT_STREAM (mistream), NULL);
  sink = gst_element_factory_make ("autoaudiosink", NULL);
  gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
  
  GstCaps *caps = gst_caps_new_simple ("audio/x-raw",
                 "format",   G_TYPE_STRING,   "S16LE",
                 "rate",     G_TYPE_INT,      sample_rate,
                 "channels", G_TYPE_INT,      1,
                 NULL);
  if (gst_element_link_filtered (source, sink, caps) == FALSE) g_printerr("link failed\n");
  gst_caps_unref(caps);
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  g_main_loop_run (loop);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (pipeline));

  g_source_remove(bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}


