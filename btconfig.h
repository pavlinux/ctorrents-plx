#ifndef BTCONFIG_H
#define BTCONFIG_H

#include <time.h>

#define barrier() __asm__ __volatile__("nop": : :"memory")
#define prefetch(x) __builtin_prefetch(x)
#define likely(y)   __builtin_expect((y),1)
#define unlikely(z) __builtin_expect((z),0)

extern size_t cfg_req_slice_size;
extern size_t quit_after_download;

#define MAX_METAINFO_FILESIZ    (4*1024*1024)
#define DEFAULT_SLICE_SIZE      (16*1024)

// According to specs the max slice size is 128K.  While most clients now do
// not accept a value that large, we want max compatibility.
#define cfg_max_slice_size (128*1024)
extern size_t cfg_req_queue_length;
#define MAX_PF_LEN 8
#define PEER_ID_LEN 20
#define PEER_PFX "-CD0303-"

extern char *cfg_user_agent; // HTTP header

extern size_t cfg_cache_size;

extern size_t cfg_max_peers;
extern size_t cfg_min_peers;

extern unsigned long cfg_listen_ip;
extern int cfg_listen_port;
extern int cfg_max_listen_port;
extern int cfg_min_listen_port;
extern char *cfg_public_ip;

extern time_t cfg_seed_hours;
extern double cfg_seed_ratio;

extern int cfg_max_bandwidth_down;
extern int cfg_max_bandwidth_up;

extern size_t arg_hash_fails;

// arguments global value
extern char *arg_metainfo_file;
extern char *arg_bitfield_file;
extern char *arg_save_as;
extern char *arg_user_agent; // BT peer ID, not HTTP header

extern unsigned char arg_flg_force_seed_mode;
extern unsigned char arg_flg_check_only;
extern unsigned char arg_flg_exam_only;
extern unsigned char arg_flg_make_torrent;
extern unsigned char arg_flg_private;
extern unsigned char arg_flg_convert_filenames;
extern char *arg_file_to_download;
extern unsigned char arg_verbose;
extern unsigned char arg_allocate;
extern unsigned char arg_daemon;

extern size_t arg_piece_length;
extern char *arg_announce;
extern char *arg_comment;

extern char *arg_ctcs;
extern char *arg_completion_exit;
#endif
