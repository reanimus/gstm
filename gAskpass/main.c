/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gAskpass/main.c
 * 
 * Copyright (c) 2005-2019	Mark Smulders <msmulders@elsar.nl>
 * Copyright (C) 2019		Dallen Wilson <dwjwilson@lavabit.com>
 *
 * gSTM is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gSTM is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "main.h"
#include "gaskpass.h"

gAskpass *app = NULL;

char *gstmpixmaps = NULL;
char *gstmui = NULL;

int main (int argc, char **argv)
{
	int status;

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	init_paths ();

	app = gaskpass_new();

	status = g_application_run (G_APPLICATION (app), argc, argv);

	if (builder)
		g_object_unref (builder);
	
	g_object_unref (app);

	return status;
}

//	Find location of pixmaps and glade ui file
void init_paths ()
{
	//	Check if local data exists
	char *tempdir = NULL;
	tempdir  = malloc (strlen (PACKAGE_SRC_DIR) + strlen ("/../pixmaps/") + 1);
	strcpy (tempdir, PACKAGE_SRC_DIR);
	strcat (tempdir, "/../pixmaps/");

	DIR* dir = opendir (tempdir);
	
	if (dir)
	{
		closedir (dir);
		gstmpixmaps = malloc (strlen (tempdir));
		strcpy (gstmpixmaps, tempdir);

		gstmui = malloc (strlen (PACKAGE_SRC_DIR) + strlen ("/gaskpass.ui") + 1);
		strcpy (gstmui, PACKAGE_SRC_DIR);
		strcat (gstmui, "/gaskpass.ui");
	}

	//	If not, check if system data exists
	if (!gstmpixmaps)
	{
		dir = opendir(PACKAGE_DATA_DIR);

		if (dir)
		{
			closedir (dir);
			gstmpixmaps = malloc (strlen (PACKAGE_DATA_DIR) + strlen ("/pixmaps/") + 1);
			strcpy (gstmpixmaps, PACKAGE_DATA_DIR);
			strcat (gstmpixmaps, "/pixmaps/");

			gstmui = malloc (strlen (PACKAGE_DATA_DIR) + strlen ("/ui/gaskpass.ui") + 1);
			strcpy (gstmui, PACKAGE_DATA_DIR);
			strcat (gstmui, "/ui/gaskpass.ui");
		}
	}

	if (!gstmpixmaps)
	{
		exit (EXIT_FAILURE);
	}
}

/*	This is an internally used function to create pixmaps.	*/
GdkPixbuf* create_pixbuf (const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;

	char *iconfile = malloc (strlen (gstmpixmaps) + strlen (filename) + 1);
	strcpy (iconfile, gstmpixmaps);
	strcat (iconfile, filename);

	if (!iconfile || !iconfile[0])
		return NULL;

	if (access (iconfile, R_OK))
	{
		g_warning (_("create_pixbuf: Couldn't find pixmap file: %s"), iconfile);
		return NULL;
	}

	pixbuf = gdk_pixbuf_new_from_file (iconfile, &error);
	if (!pixbuf)
	{
		fprintf (stderr, "Failed to load pixbuf file: %s: %s\n",
		iconfile, error->message);
		g_error_free (error);
	}

	return pixbuf;
}
