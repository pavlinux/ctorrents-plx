
#include <sys/types.h>
#include "btconfig.h"

size_t cfg_req_slice_size = DEFAULT_SLICE_SIZE;
size_t cfg_req_queue_length = 74;
size_t cfg_cache_size = 64;
size_t cfg_max_peers = 100;
size_t cfg_min_peers = 1;

unsigned long cfg_listen_ip = 0ul;
int cfg_listen_port = 0;
int cfg_max_listen_port = 2706;
int cfg_min_listen_port = 2106;
char *cfg_public_ip = NULL;

int cfg_max_bandwidth_down = 0;
int cfg_max_bandwidth_up = 0;

//
size_t arg_hash_fails = 0u;
size_t quit_after_download = 0u;

time_t cfg_seed_hours = 7777;
double cfg_seed_ratio = 0;

// arguments global value
char *arg_metainfo_file = NULL;
char *arg_bitfield_file = NULL;
char *arg_save_as = NULL;
char *arg_user_agent = NULL;

unsigned char arg_flg_force_seed_mode = 0;
unsigned char arg_flg_check_only = 0;
unsigned char arg_flg_exam_only = 0;
unsigned char arg_flg_make_torrent = 0;
unsigned char arg_flg_private = 0;
unsigned char arg_flg_convert_filenames = 0;

char *arg_file_to_download = NULL;

unsigned char arg_verbose = 0;
unsigned char arg_allocate = 0;
unsigned char arg_daemon = 0;

size_t arg_piece_length = 0x40000;	// 262144;

char *arg_announce = NULL;
char *arg_comment = NULL;

char *arg_ctcs = NULL;
char *arg_completion_exit = NULL;

char *cfg_user_agent = NULL;

