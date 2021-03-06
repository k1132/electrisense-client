/**
 * @file relay.h
 * The network relay component of the carambola client
 *
 * The relay is a process tasked with moving any data available in either a)
 * the shared buffer, or b) the SD card storage medium. The destination for
 * this data will be a server located on the same local network as the
 * Carambola. This is to ensure that network latency is minimal, so that a
 * minimal amount of processor time will be used sending the data across the
 * network.
 *
 * @authors Larson, Patrick; Pickett, Cameron
 */

#ifndef _RELAY_RELAY_H
#define _RELAY_RELAY_H

/* This is the public header file, all interface related details belong here */
#include <curl/curl.h>
#include "../shared/buffer.h"

/** Server had an issue, not our fault */
#define RELAYE_SERV -2

struct relay_st {
  /* Any operational parameters go here */
  pthread_mutex_t sd_thread_lock;
  Buffer *buffers;
  char *server_url;
  char *dump_dir;
  CURL *curl;
  struct curl_httppost *form0;
  struct curl_httppost *form1;
  struct curl_slist *slist;
  int buf_idx;
  int verbose;
};

/**
 * A handle used to store any operational parameters of the relay.
 */
typedef struct relay_st Relay;

/**
 * Initializes the relay and returns a handle to the configured relay instance.
 *
 * Returns NULL in the event of initialization failure.
 *
 * @return A malloc'd handle to be used for all future calls to to the relay
 * interface. The handle contains all configuration details necessary for the
 * relay to process data. In the event that the relay is stoppped, it is the
 * caller's responsibility to free the Relay handler by calling
 * #relay_cleanup.
 */
Relay* relay_init(Buffer* b, char* server_url, char* backup_source, int verbose);

/**
 * Perform one unit of work.
 *
 * This function is the main function called by the relay driver in order to
 * perform its task of sending data across the network. This function is meant
 * to be called in a loop. For example:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 * Relay handle = relay_init();
 * while(1) {
 *   if (relay_process(handle) < 0)
 *     break;
 * }
 * relay_cleanup(&handle);
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Only performing one "unit" of work allows for the driver to interrupt the
 * process for whatever reason, or cleanup in the event of a failure.
 * 
 * @param r The handle containing all necessary configuration to perform
 * the relay's task. Caller must call #relay_init before this function.
 * @return 0 if successful, -1 if there is an error, or one of RELAYE_*
 * @see #relay_init
 */
int relay_process(Relay *r);

/**
 * Frees the relay handle and performs any additional cleanup required to shut
 * down the relay. The specified handle will be NULL after this function
 * returns.
 *
 * @param r The handle to be freed
 */
void relay_cleanup(Relay **r);

#endif
