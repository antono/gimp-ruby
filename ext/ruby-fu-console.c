/* GIMP-Ruby -- Allows GIMP plugins to be written in Ruby.
 * Copyright (C) 2006  Scott Lembcke
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,Boston, MA
 * 02110-1301, USA.
 */
 
/* This file isn't actually part of the extension, but a helper
 * program used by the Ruby-Fu console. Hopefully this will be a
 * temporary solution. It simply uses the standard streams to
 * communicate with the console plugin.
 */
 
#include <string.h>
#include <libintl.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#define BUFFSIZE 1024

#define WIDTH 480
#define HEIGHT 400

GtkTextBuffer *textBuffer;
GtkAdjustment *scroll;

static gboolean
scroll_end_idle (gpointer ptr)
{
  gtk_adjustment_set_value(scroll, scroll->upper - scroll->page_size);
  return FALSE;
}

static void
scroll_end_add (void)
{
  g_idle_add(scroll_end_idle, NULL);
}

static void
window_destroy (GtkWidget *widget,
                gpointer   data)
{
  gtk_main_quit();
}

static gboolean
read_func (GIOChannel   *stream,
           GIOCondition  condition,
           gpointer      ptr)
{
  gchar str[BUFFSIZE];
  gsize bytes;
  GError *err = NULL;
  GIOStatus status;

  while ((status = g_io_channel_read_chars(stream, str, BUFFSIZE, &bytes, &err)))
    {    
      GtkTextIter iter;
      gtk_text_buffer_get_end_iter(textBuffer, &iter);
      gtk_text_buffer_insert(textBuffer, &iter, str, bytes);

      if (status == G_IO_STATUS_EOF)
        {
          /* If the plugin closes, we should quit too. */
           
          gtk_main_quit();
          return TRUE;
        }
      else if (status == G_IO_STATUS_AGAIN)
        {
          /* If we reach the end of the stream
           * scroll to the end and wait for more data */
           
          scroll_end_add();
          return TRUE;
        }
    }
    
  return TRUE;
}

static gboolean
key_function (GtkWidget   *widget,
              GdkEventKey *event,
              gpointer     ptr)
{
  /* Catch the return key and process accordingly */
  if (event->keyval == GDK_Return)
    {
      GtkEntry *entry = ptr;
      gchar *str = g_strdup_printf("%s\n", gtk_entry_get_text(entry));
      g_print(str);

      GtkTextIter iter;
      gtk_text_buffer_get_end_iter(textBuffer, &iter);
      gtk_text_buffer_insert_with_tags_by_name(textBuffer, &iter, str, -1,
                                               "bold", NULL);
      scroll_end_add();

      g_free(str);
      gtk_entry_set_text(entry, "");
    }

  return FALSE;
}

int main(int argc, char **argv)
{
  gtk_init(&argc, &argv);

  /* make window */
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), gettext("Ruby-Fu Console"));
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
  g_signal_connect(window, "destroy", G_CALLBACK(window_destroy), NULL);

  /* make vbox */
  GtkWidget *vbox = gtk_vbox_new(FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 12);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  /* make scrolled window */
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_ALWAYS);
  scroll = gtk_scrolled_window_get_vadjustment(
                                 GTK_SCROLLED_WINDOW(scrolled_window));
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);

  /* make text buffer */
  textBuffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_create_tag(textBuffer, "bold",
                             "weight", PANGO_WEIGHT_BOLD,
                             NULL);

  /* make text view */
  GtkWidget *view = gtk_text_view_new_with_buffer(textBuffer);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 6);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 6);
  gtk_container_add(GTK_CONTAINER(scrolled_window), view);

  /* make hbox */
  GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 0);
  
  /* make prompt */
  GtkWidget *label = gtk_label_new(">> ");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);

  /* make text entry */
  GtkWidget *entry = gtk_entry_new();
  g_signal_connect(entry, "key-press-event",
                   G_CALLBACK(key_function), entry);
  gtk_box_pack_start (GTK_BOX (hbox), entry, TRUE, TRUE, 0);

  /* open stdin in non-blocking mode */
  GError *err = NULL;
  GIOChannel *stream = g_io_channel_unix_new(0);
  GIOFlags flags = g_io_channel_get_flags(stream);
  flags |= G_IO_FLAG_NONBLOCK;
  g_io_channel_set_flags(stream, flags, &err);
  g_io_add_watch(stream, G_IO_IN, &read_func, NULL);

  gtk_widget_show_all(window);
  gtk_widget_grab_focus(entry);

  gtk_main();

  return 0;
}
