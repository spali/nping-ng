
/***************************************************************************
 * PacketStats.h -- The PacketStats class handles packet statistics. It is *
 * intended to keep track of the number of packets and bytes sent and      *
 * received, keep track of start and finish times, etc.                    *
 *                                                                         *
 ***********************IMPORTANT NMAP LICENSE TERMS************************
 *                                                                         *
 * The Nmap Security Scanner is (C) 1996-2014 Insecure.Com LLC. Nmap is    *
 * also a registered trademark of Insecure.Com LLC.  This program is free  *
 * software; you may redistribute and/or modify it under the terms of the  *
 * GNU General Public License as published by the Free Software            *
 * Foundation; Version 2 ("GPL"), BUT ONLY WITH ALL OF THE CLARIFICATIONS  *
 * AND EXCEPTIONS DESCRIBED HEREIN.  This guarantees your right to use,    *
 * modify, and redistribute this software under certain conditions.  If    *
 * you wish to embed Nmap technology into proprietary software, we sell    *
 * alternative licenses (contact sales@nmap.com).  Dozens of software      *
 * vendors already license Nmap technology such as host discovery, port    *
 * scanning, OS detection, version detection, and the Nmap Scripting       *
 * Engine.                                                                 *
 *                                                                         *
 * Note that the GPL places important restrictions on "derivative works",  *
 * yet it does not provide a detailed definition of that term.  To avoid   *
 * misunderstandings, we interpret that term as broadly as copyright law   *
 * allows.  For example, we consider an application to constitute a        *
 * derivative work for the purpose of this license if it does any of the   *
 * following with any software or content covered by this license          *
 * ("Covered Software"):                                                   *
 *                                                                         *
 * o Integrates source code from Covered Software.                         *
 *                                                                         *
 * o Reads or includes copyrighted data files, such as Nmap's nmap-os-db   *
 * or nmap-service-probes.                                                 *
 *                                                                         *
 * o Is designed specifically to execute Covered Software and parse the    *
 * results (as opposed to typical shell or execution-menu apps, which will *
 * execute anything you tell them to).                                     *
 *                                                                         *
 * o Includes Covered Software in a proprietary executable installer.  The *
 * installers produced by InstallShield are an example of this.  Including *
 * Nmap with other software in compressed or archival form does not        *
 * trigger this provision, provided appropriate open source decompression  *
 * or de-archiving software is widely available for no charge.  For the    *
 * purposes of this license, an installer is considered to include Covered *
 * Software even if it actually retrieves a copy of Covered Software from  *
 * another source during runtime (such as by downloading it from the       *
 * Internet).                                                              *
 *                                                                         *
 * o Links (statically or dynamically) to a library which does any of the  *
 * above.                                                                  *
 *                                                                         *
 * o Executes a helper program, module, or script to do any of the above.  *
 *                                                                         *
 * This list is not exclusive, but is meant to clarify our interpretation  *
 * of derived works with some common examples.  Other people may interpret *
 * the plain GPL differently, so we consider this a special exception to   *
 * the GPL that we apply to Covered Software.  Works which meet any of     *
 * these conditions must conform to all of the terms of this license,      *
 * particularly including the GPL Section 3 requirements of providing      *
 * source code and allowing free redistribution of the work as a whole.    *
 *                                                                         *
 * As another special exception to the GPL terms, Insecure.Com LLC grants  *
 * permission to link the code of this program with any version of the     *
 * OpenSSL library which is distributed under a license identical to that  *
 * listed in the included docs/licenses/OpenSSL.txt file, and distribute   *
 * linked combinations including the two.                                  *
 *                                                                         *
 * Any redistribution of Covered Software, including any derived works,    *
 * must obey and carry forward all of the terms of this license, including *
 * obeying all GPL rules and restrictions.  For example, source code of    *
 * the whole work must be provided and free redistribution must be         *
 * allowed.  All GPL references to "this License", are to be treated as    *
 * including the terms and conditions of this license text as well.        *
 *                                                                         *
 * Because this license imposes special exceptions to the GPL, Covered     *
 * Work may not be combined (even as part of a larger work) with plain GPL *
 * software.  The terms, conditions, and exceptions of this license must   *
 * be included as well.  This license is incompatible with some other open *
 * source licenses as well.  In some cases we can relicense portions of    *
 * Nmap or grant special permissions to use it in other open source        *
 * software.  Please contact fyodor@nmap.org with any such requests.       *
 * Similarly, we don't incorporate incompatible open source software into  *
 * Covered Software without special permission from the copyright holders. *
 *                                                                         *
 * If you have any questions about the licensing restrictions on using     *
 * Nmap in other works, are happy to help.  As mentioned above, we also    *
 * offer alternative license to integrate Nmap into proprietary            *
 * applications and appliances.  These contracts have been sold to dozens  *
 * of software vendors, and generally include a perpetual license as well  *
 * as providing for priority support and updates.  They also fund the      *
 * continued development of Nmap.  Please email sales@nmap.com for further *
 * information.                                                            *
 *                                                                         *
 * If you have received a written license agreement or contract for        *
 * Covered Software stating terms other than these, you may choose to use  *
 * and redistribute Covered Software under those terms instead of these.   *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes.          *
 *                                                                         *
 * Source code also allows you to port Nmap to new platforms, fix bugs,    *
 * and add new features.  You are highly encouraged to send your changes   *
 * to the dev@nmap.org mailing list for possible incorporation into the    *
 * main distribution.  By sending these changes to Fyodor or one of the    *
 * Insecure.Org development mailing lists, or checking them into the Nmap  *
 * source code repository, it is understood (unless you specify otherwise) *
 * that you are offering the Nmap Project (Insecure.Com LLC) the           *
 * unlimited, non-exclusive right to reuse, modify, and relicense the      *
 * code.  Nmap will always be available Open Source, but this is important *
 * because the inability to relicense code has caused devastating problems *
 * for other Free Software projects (such as KDE and NASM).  We also       *
 * occasionally relicense the code to third parties as discussed above.    *
 * If you wish to specify special license conditions of your               *
 * contributions, just say so when you send them.                          *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the Nmap      *
 * license file for more details (it's in a COPYING file included with     *
 * Nmap, and also available from https://svn.nmap.org/nmap/COPYING)        *
 *                                                                         *
 ***************************************************************************/
#ifndef __STATS_H__
#define __STATS_H__ 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "nping.h"

#ifdef WIN32
#include "mswin32\winclude.h"
#else
#include <sys/types.h>

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include <sys/mman.h>
#include "nping_config.h"
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

/* Make sure we define a 64bit integer type */
#ifndef u64_t
    #if WIN32
      typedef unsigned __int64 u64_t;
    #else
      typedef unsigned long long u64_t;
    #endif
#endif

/* Timeval subtraction in microseconds */
#define TIMEVAL_SUBTRACT(a,b) (((a).tv_sec - (b).tv_sec) * 1000000 + (a).tv_usec - (b).tv_usec)
/* Timeval subtract in milliseconds */
#define TIMEVAL_MSEC_SUBTRACT(a,b) ((((a).tv_sec - (b).tv_sec) * 1000) + ((a).tv_usec - (b).tv_usec) / 1000)
/* Timeval subtract in seconds; truncate towards zero */
#define TIMEVAL_SEC_SUBTRACT(a,b) ((a).tv_sec - (b).tv_sec + (((a).tv_usec < (b).tv_usec) ? - 1 : 0))


class NpingTimer {

  private:
    struct timeval start_tv;
    struct timeval stop_tv;

  public:
    NpingTimer();
    ~NpingTimer();
    void reset();
    int start();
    int stop();
    double elapsed(struct timeval *now);
    bool is_started();
    bool is_stopped();

  private:
    bool timeval_set(const struct timeval *tv);

};

/* Stat identifiers for getters */
#define STATS_TCP                (HEADER_TYPE_TCP)
#define STATS_UDP                (HEADER_TYPE_UDP)
#define STATS_ICMPv4             (HEADER_TYPE_ICMPv4)
#define STATS_ICMPv6             (HEADER_TYPE_ICMPv6)
#define STATS_IPv4               (HEADER_TYPE_IPv4)
#define STATS_IPv6               (HEADER_TYPE_IPv6)
#define STATS_ARP                (HEADER_TYPE_ARP)
#define STATS_TCP_CONNECT        (-1)
#define STATS_UDP_UNPRIV         (-2)
#define STATS_ECHO_SERVER        (-3)
#define STATS_TOTAL              (-4)


class PacketStats {

  private:

    /* Array indexes for raw packet stats */
    #define INDEX_SENT      0
    #define INDEX_RCVD      1
    #define INDEX_ECHOED    2
    #define INDEX_CAPTURED  3
    #define INDEX_READS     4
    #define INDEX_WRITES    5
    #define INDEX_CONNECTS  6
    #define INDEX_ACCEPTS   7

    u64 packets[8];    /* Packets sent/received/echoed/captured/read()s/write()s/connect()s/accept()s */
    u64 bytes[6];      /* Bytes sent/received/echoed/captured/read/written */
    u64 tcp[8];        /* TCP packets sent/received/echoed/captured/read()s/write()s/connect()s/accept()s */
    u64 udp[8];        /* UDP packets sent/received/echoed/captured/read()s/write()s/connect()s/accept()s */
    u64 icmp4[4];      /* ICMPv4 packets sent/received/echoed/captured */
    u64 icmp6[4];      /* ICMPv6 packets sent/received/echoed/captured */
    u64 arp[4];        /* ARP packets sent/received/echoed/captured */
    u64 ip4[8];        /* IPv4 packets sent/received/echoed/captured/read()s/write()s/connect()s/accept()s */
    u64 ip6[8];        /* IPv6 packets sent/received/echoed/captured/read()s/write()s/connect()s/accept()s */

    u64 echo_clients_served;
    int max_rtt;
    int min_rtt;
    int avg_rtt;

    NpingTimer tx_timer;  /* Timer for packet transmission.         */
    NpingTimer rx_timer;  /* Timer for packet reception.            */
    NpingTimer run_timer; /* Timer to measure Nping execution time. */

    u64 *proto2stats(int proto);
    int update_count(int index, int ip_version, int proto, u32 pkt_len);
    u64 get_stat(int proto, int index);
    u64 get_difference(int proto, int index_expected, int index_actual);
    double get_percentage(int proto, int index_expected, int index_actual);

 public:
    PacketStats();
    ~PacketStats();
    void reset();

    /* Raw packets sent and received */
    int update_sent(int ip_version, int proto, u32 pkt_len);
    int update_rcvd(int ip_version, int proto, u32 pkt_len);
    u64 get_sent(int proto);
    u64 get_rcvd(int proto);

    /* Packets that have been echoed. In echo server mode, number of
     * packets that have been echoed so far since the server was started.
     * In echo client mode, number of NEP_ECHO messages received from
     * the server. */
    int update_echoed(int ip_version, int proto, u32 pkt_len);
    u64 get_echoed(int proto);

    /* Number of packets captured from the wire */
    int update_captured(int ip_version, int proto, u32 pkt_len);
    u64 get_captured(int proto);

    /* Number of echo clients served since the echo server was started */
    int update_clients_served();
    u64 get_clients_served();

    /* Number of TCP connect() operations */
    int update_connects(int ip_version, int proto);
    u64 get_connects(int proto);

    /* Number of connect()s that have been accepted */
    int update_accepts(int ip_version, int proto);
    u64 get_accepts(int proto);

    /* Number of TCP or UDP unprivileged read() and write() operations performed */
    int update_reads(int ip_version, int proto, u32 pkt_len);
    int update_writes(int ip_version, int proto, u32 pkt_len);
    u64 get_reads(int proto);
    u64 get_writes(int proto);

    /* RTTs */
    int update_rtt(int rtt);
    int get_max_rtt();
    int get_min_rtt();
    int get_avg_rtt();

    /* Tx and Rx clocks */
    int start_clocks();
    int stop_clocks();
    int start_tx_clock();
    int stop_tx_clock();
    int start_rx_clock();
    int stop_rx_clock();
    int start_runtime();
    int stop_runtime();
    double get_tx_elapsed();
    double get_rx_elapsed();
    double get_runtime_elapsed(struct timeval *now);

    u64 get_bytes_sent();
    u64 get_bytes_rcvd();
    u64 get_bytes_echoed();
    u64 get_bytes_captured();
    u64 get_bytes_read();
    u64 get_bytes_written();

    u64 get_lost(int proto);
    double get_percent_lost(int proto);
    double get_percent_not_echoed(int proto);
    u64 get_connects_failed(int proto);
    double get_percent_failed(int proto);
    u64 get_pkts_lost();
    double get_percent_lost();
    double get_tx_pkt_rate();
    double get_tx_byte_rate();
    double get_rx_pkt_rate();
    double get_rx_byte_rate();
    int print_RTTs(const char *leading_str);
    int print_proto_stats(int proto, const char *leading_str, bool print_echoed);

};


#endif /* __STATS_H__ */
