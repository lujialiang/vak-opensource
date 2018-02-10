/*
 * View the contents of floppy image in SCP format.
 *
 * Copyright (C) 2018 Serge Vakulenko
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. The name of the author may not be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "scp.h"

#define VERSION         "0.0"
#define COPYRIGHT       "Copyright: (C) 2018 Serge Vakulenko"

int debug_level;
char *progname;

int main(int argc, char **argv)
{
    static const struct option long_options[] = {
        { "help",        0, 0, 'h' },
        { "version",     0, 0, 'V' },
        { NULL,          0, 0, 0 },
    };
    int ch;

    setvbuf(stdout, (char *)NULL, _IOLBF, 0);
    setvbuf(stderr, (char *)NULL, _IOLBF, 0);
    printf("SCP Image Viewer, Version %s\n", VERSION);
    progname = argv[0];

    while ((ch = getopt_long(argc, argv, "DhV", long_options, 0)) != -1) {
        switch (ch) {
        case 'D':
            ++debug_level;
            continue;
        case 'h':
            break;
        case 'V':
            /* Version already printed above. */
            return 0;
        }
usage:
        printf("%s\n\n", COPYRIGHT);
        printf("Usage:\n");
        printf("       %s [-v] file.scp\n", progname);
        printf("\nArgs:\n");
        printf("       file.scp            Floppy image in SCP format\n");
        printf("       -D                  Debug mode\n");
        printf("       -h, --help          Print this help message\n");
        printf("       -V, --version       Print version\n");
        printf("\n");
        return 0;
    }
    argc -= optind;
    argv += optind;

    if (argc != 1)
        goto usage;

    /* Open the image file. */
    scp_file_t sf;
    if (scp_open(&sf, argv[0]) < 0) {
        perror(argv[0]);
        exit(1);
    }
    scp_print_disk_header(&sf);

    int tn;
    for (tn = sf.header.start_track; tn <= sf.header.end_track; tn++) {

        if (scp_select_track(&sf, tn) < 0) {
            fprintf(stderr, "%s: Cannot select track %d\n", argv[0], tn);
            exit(1);
        }
        scp_print_track(&sf);
    }

    scp_close(&sf);
    return 0;
}

//TODO: generate VCD output
#if 0
$date
        Sat, 10 Feb 2018 03:52:44 -0000
$end
$version
        1.0
$end
$timescale
        1ns
$end
$scope module input $end
$var wire 1 ! CLK $end
$var wire 1 " CS $end
$var wire 1 # DOUT $end
$var wire 1 $ DIN $end
$upscope $end
$enddefinitions $end
$dumpvars
#0
0!
1"
0#
0$
$end
#endif