/******************************************************************************
 libprozilla - a download accelerator library
 Copyright (C) 2001 Kalum Somaratna

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

/* The main file. */

/* $Id: main.c,v 1.43 2005/03/31 20:10:57 sean Exp $ */


#include "common.h"
#include "prozilla.h"
#include "misc.h"
#include "debug.h"

libprozinfo libprozrtinfo;

/******************************************************************************
 Initialize the prozilla library.
******************************************************************************/
void proz_init(int argc, char **argv)
{

  /* Gettext stuff */
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  memset(&libprozrtinfo, 0, sizeof(libprozrtinfo));
  libprozrtinfo.argc = argc;
  libprozrtinfo.argv = argv;
  libprozrtinfo.debug_mode = TRUE;
  libprozrtinfo.ftp_use_pasv = TRUE;
  libprozrtinfo.ftp_default_user = kstrdup(DEFAULT_FTP_USER);
  libprozrtinfo.ftp_default_passwd = kstrdup(DEFAULT_FTP_PASSWD);
  libprozrtinfo.use_netrc = TRUE;
  libprozrtinfo.ftp_proxy = 0;
  libprozrtinfo.http_proxy = 0;
  /* 2 minutes should be enough for the default timeout */
  libprozrtinfo.conn_timeout.tv_sec = 120;
  libprozrtinfo.conn_timeout.tv_usec = 0;
  /* 15 secs for the default retry_delay */
  libprozrtinfo.conn_retry_delay.tv_sec = 15;
  libprozrtinfo.conn_retry_delay.tv_usec = 0;
  /*Default: try infinitely */
  libprozrtinfo.max_attempts = 0;
  /*No no-cache directive is given */
  libprozrtinfo.http_no_cache = FALSE;
  /*default is unlimited (0) bandwith */
  libprozrtinfo.max_bps_per_dl = 0;

  libprozrtinfo.home_dir = home_dir();

  if (libprozrtinfo.home_dir != NULL)
  {
    char *netrc_file = kmalloc(strlen(libprozrtinfo.home_dir)
			       + strlen(".netrc") + 2);
    sprintf(netrc_file, "%s/%s", libprozrtinfo.home_dir, ".netrc");
    libprozrtinfo.netrc_list = parse_netrc(netrc_file);
  }

  /* TODO : Command-line options for dl_dir, output_dir and log_dir */
  libprozrtinfo.dl_dir = kstrdup(".");
  libprozrtinfo.output_dir = kstrdup(".");
  libprozrtinfo.log_dir = kstrdup(".");
  debug_init();
  return;
}

/******************************************************************************
 Shutting down prozilla gracefully
******************************************************************************/
void proz_shutdown(void)
{
  kfree(libprozrtinfo.http_proxy);
  kfree(libprozrtinfo.ftp_proxy);
  kfree(libprozrtinfo.dl_dir);
  kfree(libprozrtinfo.output_dir);
  kfree(libprozrtinfo.log_dir);
}

/******************************************************************************
 Abort with an error message.
******************************************************************************/
void proz_die(const char *format, ...)
{
  va_list args;
  char message[MAX_MSG_SIZE + 1];

  va_start(args, format);
  vsnprintf(message, MAX_MSG_SIZE, format, args);
  va_end(args);

  printf("%s\n", message);
  proz_shutdown();

  exit(EXIT_FAILURE);
}

/******************************************************************************
Set http proxy
******************************************************************************/
void proz_set_http_proxy(proxy_info * proxy)
{
  if (libprozrtinfo.http_proxy)
    kfree(libprozrtinfo.http_proxy);
  libprozrtinfo.http_proxy = kmalloc(sizeof(proxy_info));
  memcpy(libprozrtinfo.http_proxy, proxy, sizeof(proxy_info));
}

/******************************************************************************
Set ftp proxy
******************************************************************************/
void proz_set_ftp_proxy(proxy_info * proxy)
{
  if (libprozrtinfo.ftp_proxy)
    kfree(libprozrtinfo.ftp_proxy);
  libprozrtinfo.ftp_proxy = kmalloc(sizeof(proxy_info));
  memcpy(libprozrtinfo.ftp_proxy, proxy, sizeof(proxy_info));
}

void proz_use_http_proxy(boolean use)
{
  if (libprozrtinfo.http_proxy)
    libprozrtinfo.http_proxy->use_proxy = use;
}

void proz_use_ftp_proxy(boolean use)
{
  if (libprozrtinfo.ftp_proxy)
    libprozrtinfo.ftp_proxy->use_proxy = use;
}

void proz_set_connection_timeout(struct timeval *timeout)
{
  libprozrtinfo.conn_timeout.tv_sec = timeout->tv_sec;
  libprozrtinfo.conn_timeout.tv_usec = timeout->tv_usec;
}


void proz_set_connection_retry_delay(struct timeval *delay)
{
  libprozrtinfo.conn_retry_delay.tv_sec = delay->tv_sec;
  libprozrtinfo.conn_retry_delay.tv_usec = delay->tv_usec;
}


void proz_set_download_dir(char *dir)
{
  assert(dir != NULL);

  if (libprozrtinfo.dl_dir)
    kfree(libprozrtinfo.dl_dir);

  libprozrtinfo.dl_dir = kstrdup(dir);

}

void proz_set_logfile_dir(char *dir)
{
  assert(dir != NULL);
  if (libprozrtinfo.log_dir)
    kfree(libprozrtinfo.log_dir);

  libprozrtinfo.log_dir = kstrdup(dir);
}

void proz_set_output_dir(char *dir)
{
  assert(dir != NULL);

  if (libprozrtinfo.output_dir)
    kfree(libprozrtinfo.output_dir);

  libprozrtinfo.output_dir = kstrdup(dir);
}


char *proz_get_libprozilla_version()
{

  return strdup(VERSION);
}
