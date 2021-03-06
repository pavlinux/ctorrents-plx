#include "def.h"
#include <sys/types.h>

#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "btconfig.h"
#include "btcontent.h"
#include "downloader.h"
#include "peerlist.h"
#include "tracker.h"
#include "ctcs.h"
#include "console.h"
#include "sigint.h"
#include "config.h"
#include "sha1.h"

#ifndef HAVE_RANDOM
#include "compat.h"
#endif

void usage(void);
void Random_init(void);
int param_check(int argc, char **argv);

void Random_init(void)
{
	struct timeval tv;
	unsigned long seed;
	u_int8_t buffer[64];

	gettimeofday(&tv, (__timezone_ptr_t) 0);
	seed = (unsigned long) (tv.tv_usec + (tv.tv_sec * getpid()));
	snprintf((char *) &buffer, 64, "%lu", seed);
	seed = 0ul;
	u_int32_t state[5] = {0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u, 0xc3d2e1f0u};
	SHA1Transform(state, buffer);

	seed = (unsigned long) ((state[0] ^ state[3]) ^ (state[1] ^ state[4]));
	srand48(seed);
}

int main(int argc, char **argv)
{
	try {
		if (argc < 2) {
			throw argc;
		}
	} catch (int) {
		usage();
		exit(1);
	}

	char *s;

	Random_init();

	arg_user_agent = new char[MAX_PF_LEN + 1](); // free'd at end param_check()
	//memmove(arg_user_agent, PEER_PFX, MAX_PF_LEN);
	strcpy(arg_user_agent, PEER_PFX);
	cfg_user_agent = new char[strlen(PACKAGE_NAME) + strlen(PACKAGE_VERSION) + 2]();

	if (!cfg_user_agent)
		return -1;

	sprintf(cfg_user_agent, "%s/%s", PACKAGE_NAME, PACKAGE_VERSION);

	do {
		s = strchr(cfg_user_agent, ' ');
		if (s != NULL)
			*s = '-';
	} while (s);

	if (param_check(argc, argv) < 0) {
		if (arg_user_agent)
			delete[] arg_user_agent;
		return -1;
	}

	if (arg_flg_make_torrent) {
		if (!arg_announce) {
			CONSOLE.Warning(1,
				"Please use -u to specify an announce URL!");
			return EXIT_FAILURE;
		}
		if (!arg_save_as) {
			CONSOLE.Warning(1,
				"Please use -s to specify a metainfo file name!");
			return EXIT_FAILURE;
		}
		if (BTCONTENT.InitialFromFS(arg_metainfo_file, arg_announce,
			arg_piece_length) < 0 ||
			BTCONTENT.CreateMetainfoFile(arg_save_as) < 0) {
			CONSOLE.Warning(1, "create metainfo failed.");
			return EXIT_FAILURE;
		}
		CONSOLE.Print("Create metainfo file %s successful.", arg_save_as);
		return EXIT_SUCCESS;
	}

	if (arg_daemon)
		CONSOLE.Daemonize();

	if (!arg_flg_exam_only
		&& (!arg_flg_check_only || arg_flg_force_seed_mode))
		if (arg_ctcs)
			CTCS.Initial();

	if (BTCONTENT.InitialFromMI(arg_metainfo_file, arg_save_as) < 0) {
		CONSOLE.Warning(1, "error, initial meta info failed.");
		exit(1);
	}

	if (!arg_flg_exam_only
		&& (!arg_flg_check_only || arg_flg_force_seed_mode)) {
		if (WORLD.Initial_ListenPort() < 0) {
			CONSOLE.Warning(2,
				"warn, you can't accept connections.");
		}

		if (Tracker.Initial() < 0) {
			CONSOLE.Warning(1, "error, tracker setup failed.");
			return EXIT_FAILURE;
		}

		sig_setup(); // setup signal handling
		CONSOLE.Interact
			("Press 'h' or '?' for help (display/control client options).");
		Downloader();
		if (cfg_cache_size)
			BTCONTENT.FlushCache();
	}
	if (!arg_flg_exam_only)
		BTCONTENT.SaveBitfield();
	WORLD.CloseAll();

	if (arg_verbose)
		CONSOLE.cpu();

	return EXIT_SUCCESS;
}

int param_check(int argc, char **argv)
{

	int c, l;
	const char *opts;

	if (0 == strncmp(argv[1], "-t", 2))
		opts = "tc:l:ps:u:";
	else
		opts = "aA:b:cC:dD:e:E:F:fi:I:M:m:n:P:p:Qs:S:Tu:U:vxX:z:hH";

	while ((c = getopt(argc, argv, opts)) != -1)
		switch (c) {
		case 'a':
			arg_allocate = 1;
			break;

		case 'b':
			if (arg_bitfield_file)
				delete[] arg_bitfield_file; // Fix CID:28187
			arg_bitfield_file = new char[strlen(optarg) + 1]();
			if (unlikely(!arg_bitfield_file))
				goto err;

			strcpy(arg_bitfield_file, optarg);
			break;

		case 'i': // listen on ip XXXX
			cfg_listen_ip = inet_addr(optarg);
			break;

		case 'I': // set public ip XXXX
			if (cfg_public_ip)
				delete []cfg_public_ip;
			cfg_public_ip = new char[strlen(optarg) + 1]();
			if (unlikely(cfg_public_ip == NULL))
				goto err;
			strcpy(cfg_public_ip, optarg);
			break;

		case 'p': // listen on Port XXXX
			if (arg_flg_make_torrent)
				arg_flg_private = 1;
			else
				cfg_listen_port = atoi(optarg);
			break;

		case 's': // Save as FILE/DIR NAME
			if (arg_save_as)
				goto err; // specified twice
			arg_save_as = new char[strlen(optarg) + 1]();
			if (unlikely(arg_save_as == NULL))
				goto err;

			strcpy(arg_save_as, optarg);
			break;

		case 'e': // Exit while complete
			cfg_seed_hours = (time_t) strtoul(optarg, NULL, 10);
			break;

		case 'E': // target seed ratio
			cfg_seed_ratio = atof(optarg);
			break;

		case 'c': // Check exist only
			if (arg_flg_make_torrent) {
				if (arg_comment)
					delete []arg_comment;
				arg_comment = new char[strlen(optarg) + 1]();
				if (unlikely(arg_comment == NULL)) {
					goto err;
				}
				strcpy(arg_comment, optarg);
			} else
				arg_flg_check_only = 1;
			break;

		case 'C': // Max cache size
			cfg_cache_size = atoi(optarg);
			break;

		case 'F': // arg_hash_fails
			arg_hash_fails = atoi(optarg);
			break;

		case 'M': // Max peers
			cfg_max_peers = atoi(optarg);
			if (cfg_max_peers > 1000 || cfg_max_peers < 20) {
				CONSOLE.Warning(1, "-%c argument must be between 20 and 1000", c);
				goto err;
			}
			break;

		case 'm': // Min peers
			cfg_min_peers = atoi(optarg);
			if (cfg_min_peers > 1000 || cfg_min_peers < 1) {
				CONSOLE.Warning(1, "-%c argument must be between 1 and 1000", c);
				goto err;
			}
			break;

		case 'z': // slice size
			cfg_req_slice_size = atoi(optarg) * 1024;
			if (cfg_req_slice_size < 1024
				|| cfg_req_slice_size > cfg_max_slice_size) {
				CONSOLE.Warning(1,
					"-%c argument must be between 1 and %d",
					c, cfg_max_slice_size / 1024);
				goto err;
			}
			break;

		case 'n': // Which file download
			if (arg_file_to_download)
				goto err; // specified twice
			arg_file_to_download = new char[strlen(optarg) + 1]();
			if (unlikely(arg_file_to_download == NULL))
				goto err;

			strcpy(arg_file_to_download, optarg);
			break;

		case 'f': // force seed mode, skip sha1 check when startup.
			arg_flg_force_seed_mode = 1;
			break;

		case 'D': // download bandwidth limit
			cfg_max_bandwidth_down = (int) (strtod(optarg, NULL) * 1024);
			break;

		case 'U': // upload bandwidth limit
			cfg_max_bandwidth_up = (int) (strtod(optarg, NULL) * 1024);
			break;

		case 'P': // peer ID prefix
			l = strlen(optarg);
			if (l > MAX_PF_LEN) {
				CONSOLE.Warning(1, "-P arg must be %d or less characters",
					MAX_PF_LEN);
				goto err;
			}
			if (l == 1 && *optarg == '-')
				*arg_user_agent = (char) 0;
			else
				strcpy(arg_user_agent, optarg);
			break;

		case 'A': // HTTP user-agent header string
			if (cfg_user_agent)
				delete[]cfg_user_agent;
			cfg_user_agent = new char[strlen(optarg) + 1]();
			if (unlikely(cfg_user_agent == NULL))
				goto err;

			strcpy(cfg_user_agent, optarg);
			break;

		case 'T': // convert foreign filenames to printable text
			arg_flg_convert_filenames = 1;
			break;

			// BELOW OPTIONS USED FOR CREATE TORRENT.
		case 'u': // Announce URL
			if (arg_announce)
				goto err; // specified twice
			arg_announce = new char[strlen(optarg) + 1]();

			if (!arg_announce)
				goto err;

			strcpy(arg_announce, optarg);
			break;

		case 't': // make Torrent
			arg_flg_make_torrent = 1;
			CONSOLE.ChangeChannel(O_INPUT, "off", 0);
			break;

		case 'l': // piece Length (default 262144)
			arg_piece_length = atoi(optarg);
			if (arg_piece_length < 65536
				|| arg_piece_length > 4096 * 1024) {
				CONSOLE.Warning(1,
					"-%c argument must be between 65536 and %d",
					c, 4096 * 1024);
				goto err;
			}
			break;
			// ABOVE OPTIONS USED FOR CREATE TORRENT.

		case 'x': // print torrent information only
			arg_flg_exam_only = 1;
			CONSOLE.ChangeChannel(O_INPUT, "off", 0);
			break;

		case 'S': // CTCS server
			if (arg_ctcs)
				goto err; // specified twice
			arg_ctcs = new char[strlen(optarg) + 1]();

			if (!arg_ctcs)
				goto err;

			if (!strchr(optarg, ':')) {
				CONSOLE.Warning(1,
					"-%c argument requires a port number",
					c);
				goto err;
			}
			strcpy(arg_ctcs, optarg);
			break;

		case 'X': // "user exit" on download completion
			if (arg_completion_exit)
				goto err; // specified twice
			arg_completion_exit = new char[strlen(optarg) + 1]();
			if (!arg_completion_exit)
				goto err;
			;
#ifndef HAVE_SYSTEM
			CONSOLE.Warning(1,
				"-X is not supported on your system");
			goto err;
#endif
#ifndef HAVE_WORKING_FORK
			CONSOLE.Warning(2,
				"No working fork function; be sure the -X command is brief!");
#endif
			strcpy(arg_completion_exit, optarg);
			break;

		case 'Q':
			quit_after_download = 1;
			break;

		case 'v': // verbose output
			arg_verbose = 1;
			break;

		case 'd': // daemon mode (fork to background)
			arg_daemon++;
			break;

		case 'h':
		case 'H': // help
			usage();
			goto err;

		default:
			//unknown option.
			CONSOLE.Warning(1, "Use -h for help/usage.");
			goto err;
		}

	argc -= optind;
	argv += optind;
	if (cfg_min_peers >= cfg_max_peers)
		cfg_min_peers = cfg_max_peers - 1;
	if (argc != 1) {
		if (arg_flg_make_torrent)
			CONSOLE.Warning(1,
			"Must specify torrent contents (one file or directory)");
		else
			CONSOLE.Warning(1, "Must specify one torrent file");
		goto err;
		;
	}
	arg_metainfo_file = new char[strlen(*argv) + 1]();

	if (!arg_metainfo_file)
		goto err;
	;

	strcpy(arg_metainfo_file, *argv);

	if (!arg_bitfield_file) {
		arg_bitfield_file = new char[strlen(arg_metainfo_file) + 4]();

		if (!arg_bitfield_file)
			goto err;

		strcpy(arg_bitfield_file, arg_metainfo_file);
		strcat(arg_bitfield_file, ".bf");
	}

	return 0;
err:
	if (arg_bitfield_file) delete[] arg_bitfield_file;
	if (cfg_public_ip) delete[] cfg_public_ip;
	if (arg_save_as) delete[] arg_save_as;
	if (arg_comment)delete[] arg_comment;
	if (arg_file_to_download) delete[] arg_file_to_download;
	if (cfg_user_agent) delete[]cfg_user_agent;
	if (arg_announce) delete[] arg_announce;
	if (arg_ctcs) delete[] arg_ctcs;
	if (arg_metainfo_file) delete[] arg_metainfo_file;
	if (arg_completion_exit) delete[] arg_completion_exit;

	return -1;
}

void usage(void)
{

	CONSOLE.ChangeChannel(O_INPUT, "off", 0);

	fprintf(stderr, "\nGeneral Options:\n");
	fprintf(stderr, "%-15s %s\n", "-h/-H", "Show this message");
	fprintf(stderr, "%-15s %s\n", "-x",
		"Decode metainfo (torrent) file only, don't download");
	fprintf(stderr, "%-15s %s\n", "-c", "Check pieces only, don't download");
	fprintf(stderr, "%-15s %s\n", "-v", "Verbose output (for debugging)");

	fprintf(stderr, "\nDownload Options:\n");
	fprintf(stderr, "%-15s %s\n", "-e int",
		"Exit while seed <int> hours later (default 72 hours)");
	fprintf(stderr, "%-15s %s\n", "-E num",
		"Exit after seeding to <num> ratio (UL:DL)");
	fprintf(stderr, "%-15s %s\n", "-i ip",
		"Listen for connections on specific IP address (default all/any)");
	fprintf(stderr, "%-15s %s\n", "-p port",
		"Listen port (default 2706 -> 2106)");
	fprintf(stderr, "%-15s %s\n", "-I ip",
		"Specify public/external IP address for peer connections");
	fprintf(stderr, "%-15s %s\n", "-u num or URL",
		"Use an alternate announce (tracker) URL");
	fprintf(stderr, "%-15s %s\n", "-s filename",
		"Download (\"save as\") to a different file or directory");
	fprintf(stderr, "%-15s %s\n", "-C cache_size",
		"Cache size, unit MB (default 16MB)");
	fprintf(stderr, "%-15s %s\n", "-f",
		"Force saved bitfield or seed mode (skip initial hash check)");
	fprintf(stderr, "%-15s %s\n", "-b filename",
		"Specify bitfield save file (default is torrent+\".bf\")");
	fprintf(stderr, "%-15s %s\n", "-M max_peers",
		"Max peers count (default 100)");
	fprintf(stderr, "%-15s %s\n", "-m min_peers", "Min peers count (default 1)");
	fprintf(stderr, "%-15s %s\n", "-z slice_size",
		"Download slice/block size, unit KB (default 16, max 128)");
	fprintf(stderr, "%-15s %s\n", "-n file_list",
		"Specify file number(s) to download");
	fprintf(stderr, "%-15s %s\n", "-Q", "Quit after dowdload");
	fprintf(stderr, "%-15s %s\n", "-D rate", "Max bandwidth down (unit KB/s)");
	fprintf(stderr, "%-15s %s\n", "-U rate", "Max bandwidth up (unit KB/s)");
	fprintf(stderr, "%-15s %s%s\")\n", "-P peer_id",
		"Set Peer ID prefix (default \"", PEER_PFX);
	fprintf(stderr, "%-15s %s%s\")\n", "-A user_agent",
		"Set User-Agent header (default \"", cfg_user_agent);
	fprintf(stderr, "%-15s %s\n", "-S host:port",
		"Use CTCS server at host:port");
	fprintf(stderr, "%-15s %s\n", "-a", "Preallocate files on disk");
	fprintf(stderr, "%-15s %s\n", "-T",
		"Convert foreign filenames to printable text");
	fprintf(stderr, "%-15s %s\n", "-X command",
		"Run command upon download completion (\"user exit\")");
	fprintf(stderr, "%-15s %s\n", "-d", "Daemon mode (fork to background)");
	fprintf(stderr, "%-15s %s\n", "-dd", "Daemon mode with I/O redirection");

	fprintf(stderr, "\nMake metainfo (torrent) file options:\n");
	fprintf(stderr, "%-15s %s\n", "-t", "Create a new torrent file");
	fprintf(stderr, "%-15s %s\n", "-u URL", "Tracker's URL");
	fprintf(stderr, "%-15s %s\n", "-l piece_len",
		"Piece length (default 262144)");
	fprintf(stderr, "%-15s %s\n", "-s filename", "Specify metainfo file name");
	fprintf(stderr, "%-15s %s\n", "-p", "Private (disable peer exchange)");
	fprintf(stderr, "%-15s %s\n", "-c comment", "Include a comment/description");

	fprintf(stderr, "\nExample:\n");
	fprintf(stderr, "ctorrent -s new_filename -e 12 -C 32 -p 6881 example.torrent\n\n");
}

