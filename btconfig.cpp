
#include <sys/types.h>
#include "btconfig.h"

#define NULL_CHAR ((char *)0)

size_t cfg_req_slice_size = DEFAULT_SLICE_SIZE;
size_t cfg_req_queue_length = 74;
size_t cfg_cache_size = 64;
size_t cfg_max_peers = 100;
size_t cfg_min_peers = 1;

unsigned long cfg_listen_ip = 0ul;
int cfg_listen_port = 0;
int cfg_min_listen_port = 6881;
int cfg_max_listen_port = 7881;

char *cfg_public_ip = NULL_CHAR;

int cfg_max_bandwidth_down = 0;
int cfg_max_bandwidth_up = 0;

time_t cfg_seed_hours = 72;
double cfg_seed_ratio = 0.0F;

// arguments global value
char *arg_metainfo_file = NULL_CHAR;
char *arg_bitfield_file = NULL_CHAR;
char *arg_save_as = NULL_CHAR;
char *arg_user_agent = NULL_CHAR;

unsigned char arg_flg_force_seed_mode = 0u;
unsigned char arg_flg_check_only = 0u;
unsigned char arg_flg_exam_only = 0u;
unsigned char arg_flg_make_torrent = 0;
unsigned char arg_flg_private = 0u;
unsigned char arg_flg_convert_filenames = 0u;

char *arg_file_to_download = NULL_CHAR;

unsigned char arg_verbose = 0u;
unsigned char arg_allocate = 0u;
unsigned char arg_daemon = 0u;

size_t arg_piece_length = 0x40000u;	// 262144;

char *arg_announce = NULL_CHAR;
char *arg_comment = NULL_CHAR;
char *arg_ctcs = NULL_CHAR;
char *arg_completion_exit = NULL_CHAR;
char *cfg_user_agent = NULL_CHAR;
