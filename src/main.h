
#ifndef MAIN_H
#define MAIN_H

#include <sys/time.h>
#include "prozilla.h"

/* Gettext */
#include <libintl.h>
#define gettext_noop(String) (String)
#ifndef HAVE_GNOME
#define N_(String) gettext_noop (String)
#endif
/* Gettext */


/*We will have a runtime structure for this program */

#define PRZCONFDIR ".prozilla"

  typedef enum {
	  RESUME
  } rto;
  
  typedef enum {
    DISP_CURSES,
    DISP_STDOUT
  } DISPLAYMODE;

/* Long options */
#define _OPT_USEPORT        129
#define _OPT_RETRYDELAY     130
#define _OPT_TIMEOUT        131
#define _OPT_NOGETCH        132
#define _OPT_DEBUG          133
#define _OPT_NOSEARCH       135
#define _OPT_PT             136
#define _OPT_PAO            137
#define _OPT_MAXFTPSRV      138
#define _OPT_MAXBPS         139
#define _OPT_NOCURSES       140
#define _OPT_MINSIZE        141
#define _OPT_FTPSID         142

struct runtime {
  int num_connections;
  int max_redirections;
  /* whether to use the netrc file */
  int use_netrc;
  int ftp_use_pasv;
  int max_attempts;
  /* delay in seconds */
  int retry_delay;
  /* The timeout period for the connections */
  struct timeval timeout;
  int itimeout;
  int debug_mode;
  int quiet_mode;
  int libdebug_mode;
  int ftp_search;
  int force_mode;
  /* The maximum number of servers to ping at once */
  int max_simul_pings;
  /* The max number of seconds to wait for a server response to ping */
  int max_ping_wait;
  /* The maximum number of servers/mirrors to request */
  int ftps_mirror_req_n;
  long max_bps_per_dl;
  /* The dir to save the generated file in */
  char *output_dir;
  /*The directory where the Dl'ed portions are stored */
  char *dl_dir;
  char *logfile_dir;
  char *home_dir;
  /*The dir where the config files are stored */
  char *config_dir;
  proxy_info *ftp_proxy;
  proxy_info *http_proxy;
  int use_http_proxy;
  int use_ftp_proxy;
  int http_no_cache;
  int ftpsearch_server_id;
  /* new options */
  int resume_mode;
  /* don't prompt user, display message and die */
  int dont_prompt;
  /* curses, bare terminal, others... */
  int display_mode;
  /* search size in K */
  long min_search_size;
};

extern struct runtime rt;

void shutdown(void);

#endif
