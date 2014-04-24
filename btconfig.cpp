
#include <sys/types.h>
#include "btconfig.h"

#define NULLCHR ((char *)0)

size_t cfg_req_slice_size = DEFAULT_SLICE_SIZE;
size_t cfg_req_queue_length = 74u;
size_t cfg_cache_size = 64u;
size_t cfg_max_peers = 100u;
size_t cfg_min_peers = 1u;

unsigned long cfg_listen_ip = 0ul;
int cfg_listen_port = 0;
int cfg_max_listen_port = 2706;
int cfg_min_listen_port = 2106;
char *cfg_public_ip = NULLCHR;

int cfg_max_bandwidth_down = 0;
ssize_t cfg_max_bandwidth_up = 0;

time_t cfg_seed_hours = 7777;
size_t arg_hash_fails = 0u;
size_t quit_after_download = 0u;

double cfg_seed_ratio = 0;

// arguments global value
char *arg_metainfo_file = NULLCHR;
char *arg_bitfield_file = NULLCHR;
char *arg_save_as = NULLCHR;
char *arg_user_agent = NULLCHR;

unsigned char arg_flg_force_seed_mode = 0;
unsigned char arg_flg_check_only = 0;
unsigned char arg_flg_exam_only = 0;
unsigned char arg_flg_make_torrent = 0;
unsigned char arg_flg_private = 0;
unsigned char arg_flg_convert_filenames = 0;

char *arg_file_to_download = NULLCHR;

unsigned char arg_verbose = 0;
unsigned char arg_allocate = 0;
unsigned char arg_daemon = 0;

size_t arg_piece_length = 0x40000; // 262144;

char *arg_announce = NULLCHR;
char *arg_comment = NULLCHR;

char *arg_ctcs = NULLCHR;
char *arg_completion_exit = NULLCHR;

char *cfg_user_agent = NULLCHR;
