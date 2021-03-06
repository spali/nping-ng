
/***************************************************************************
 * NpingOps.cc -- The NpingOps class contains global options, mostly based *
 * on user-provided command-line settings.                                 *
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

#include "nping.h"
#include "nbase.h"
#include "NpingOps.h"
#include "utils.h"
#include "utils_net.h"
#include "ArgParser.h"
#include "output.h"
#include "common.h"

#ifdef WIN32
#include "winfix.h"
#endif

/******************************************************************************
 *  Constructors and destructors                                              *
 ******************************************************************************/

/* Constructor */
NpingOps::NpingOps() {

  /* Probe modes */
  modes=NO_MODE_SET;

  /* Output */
  vb=DEFAULT_VERBOSITY;
  vb_set=false;

  dbg=DEFAULT_DEBUGGING;
  dbg_set=false;

  show_sent_pkts=true;
  show_sent_pkts_set=false;

  show_eth=false;

  show_all_stats=false;

  /* Operation and Performance */
  rounds=DEFAULT_PACKET_ROUNDS;
  rounds_set=false;

  sendpref=PACKET_SEND_NOPREF;

  delay=DEFAULT_DELAY;
  delay_set=false;

  memset(device, 0, MAX_DEV_LEN);
  device_set=false;

  bpf_filter_spec=NULL;
  bpf_filter_spec_set=false;

  current_round=0;

  have_pcap=true;

  disable_packet_capture=false;
  disable_packet_capture_set=false;

  do_multicast=false;

  /* Privileges */
  isr00t=false;
  isr00t_set=false;

  /* Payloads */
  payload_type=PL_NONE;
  payload_type_set=false;

  payload_buff=NULL;
  payload_buff_set=false;

  payload_len=0;
  payload_len_set=false;

  /* Roles */
  role=ROLE_NORMAL;

  /* IP Protocol */
  family=AF_INET;

  mtu=0;
  mtu_set=false;
  custom_ttl_set=false;

  ip_proto=0;
  ip_proto_set=false;

  ip_options=NULL;
  ip_options_set=false;

  spoof_addr=NULL;

  /* TCP / UDP */
  target_ports=NULL;
  tportcount=0;
  target_ports_set=false;

  source_ports=NULL;
  sportcount=0;
  source_ports_set=false;

  /* Ethernet */
  memset(src_mac, 0, 6);
  src_mac_set=false;

  memset(dst_mac, 0, 6);
  dst_mac_set=false;

  eth_type=0;
  eth_type_set=false;

  arp_opcode=0;
  arp_opcode_set=false;

  memset(arp_sha, 0, 6);
  arp_sha_set=false;

  memset(arp_tha, 0, 6);
  arp_tha_set=false;

  arp_spa.s_addr=0;
  arp_spa_set=false;

  arp_tpa.s_addr=0;
  arp_tpa_set=false;

  /* Echo mode */
  echo_port=DEFAULT_ECHO_PORT;
  echo_port_set=false;

  do_crypto=true;

  echo_payload=false;

  echo_server_once=false;
  echo_server_once_set=false;

  memset(echo_passphrase, 0, sizeof(echo_passphrase));
  echo_passphrase_set=false;

  memset(&last_sent_pkt_time, 0, sizeof(struct timeval));

  delayed_rcvd_pkt=NULL;
  delayed_rcvd_pkt_set=false;
  delayed_rcvd_ts=0;

} /* End of NpingOps() */


/* Destructor */
NpingOps::~NpingOps() {
  if(payload_buff!=NULL)
    free(payload_buff);
  if(ip_options!=NULL)
    free(ip_options);
  if(target_ports!=NULL)
    free(target_ports);
  if(source_ports!=NULL)
    free(source_ports);
  /* TODO: Free elements from target_addresses, target_hosts, interfaces, etc. */
 return;
} /* End of ~NpingOps() */


/******************************************************************************
 *  Nping probe modes                                                         *
 ******************************************************************************/

/** Adds a new operation mode.
  * @return OP_SUCCESS on success.
  * @return OP_FAILURE in case of error. */
int NpingOps::addMode(u16 md) {
  this->modes = this->modes | md;
  return OP_SUCCESS;
} /* End of addMode() */


/* Removes an operation mode */
int NpingOps::delMode(u16 md){
  this->modes = this->modes & (~md) ;
  return OP_SUCCESS;
} /* End of delMode() */


/** Returns value of attribute "mode". The value returned is supposed to be
 *  one of : TCP_CONNECT, TCP, UDP, UDP_UNPRIV, ICMP, ARP */
u16 NpingOps::getModes() {
  return this->modes;
} /* End of getMode() */


/* Returns true if option has been set */
bool NpingOps::issetMode(){
  return (this->modes==NO_MODE_SET);
} /* End of isset() */


/* Returns true if the supplied mode has been set. There are two special
 * constants, MODE_IS_PRIVILEGED and MODE_IS_UNPRIVILEGED, that can be passed
 * to determine if the current mode requires root privileges. Here are a few
 * examples of how this can be used:
 * mode(DO_ICMPv4) returns true if we are sending ICMPv4 packets.
 * mode(MODE_IS_UNPRIVILEGED) returns true if only TCP Connect or UDP Unpriv
 * are set.
 * mode(MODE_IS_PRIVILEGED) returns true if any mode that requires raw sockets
 * has been set. */
bool NpingOps::mode(u16 test_value){
  if(test_value==MODE_IS_PRIVILEGED){
    if(this->modes & DO_TCP)
      return true;
    if(this->modes & DO_UDP)
      return true;
    if(this->modes & DO_ICMP)
      return true;
    if(this->modes & DO_ARP)
      return true;
    if(this->modes & DO_TRACEROUTE)
      return true;
    if(this->modes & DO_EXT_HOPOPT)
      return true;
    if(this->modes & DO_EXT_ROUTING)
      return true;
    if(this->modes & DO_EXT_DOPT)
      return true;
    if(this->modes & DO_EXT_FRAGMENT)
      return true;
  }else if(test_value==MODE_IS_UNPRIVILEGED){
      return !this->mode(MODE_IS_PRIVILEGED);
  }else{
    return (this->modes & test_value);
  }
  return false;
} /* End of mode() */


/** Takes a probe mode and returns an ASCII string with the name of the mode.
 *  @warning Returned pointer is a static buffer that subsequent calls
 *            will overwrite.
 *  @return Pointer to the appropriate string on success and pointer to a
 *          string containing "Unknown probe" in case of failure.
 * */
const char *NpingOps::mode2Ascii(u16 md){
  int n=0;
  static char modestr[256];
  memset(modestr,0,256);

  strncat(modestr,"{", 255);

  if(md & DO_TCP_CONNECT){
    strncat(modestr,"TCP-Connect", 255);
    n++;
  }
  if(md & DO_UDP_UNPRIV){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"UDP-Unprivileged", 255);
    n++;
  }
  if(md & DO_TCP){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"TCP", 255);
    n++;
  }
  if(md & DO_UDP){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"UDP", 255);
    n++;
  }
  if(md & DO_ICMP){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"ICMP", 255);
    n++;
  }
  if(md & DO_ARP){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"ARP", 255);
    n++;
  }
  if(md & DO_TRACEROUTE){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"Traceroute", 255);
    n++;
  }
  if(md & DO_EXT_HOPOPT){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"IPv6-HopByHop", 255);
    n++;
  }
  if(md & DO_EXT_ROUTING){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"IPv6-Routing", 255);
    n++;
  }
  if(md & DO_EXT_DOPT){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"IPv6-DestOpts", 255);
    n++;
  }
  if(md & DO_EXT_FRAGMENT){
    if(n>0)
      strncat(modestr," + ", 255);
    strncat(modestr,"IPv6-Fragment", 255);
    n++;
  }
  /* Make sure we received something valid. Otherwise it's a bug */
  assert(n>0);
  strncat(modestr,"}", 255);
  return modestr;
} /* End of mode2Ascii() */

/******************************************************************************
 * Output                                                                     *
 ******************************************************************************/

/** Sets verbosity level. Supplied level must be an integer between -4 and
 *  4. Check man pages for details.
 *
 *  The thing here is that what the argument parser gets from the user is
 *  number in the range [-4, 4]. However, in NpingOps we don't store negative
 *  verbosity values, we just convert the supplied level into our internal
 *  levels (QT_4, QT_3, ... , VB_0, VB_1, ..., VB_4)
 *  So the rest of the code in Nping should check for these defines, rather
 *  than checking for numbers. Check nping.h for more information on how to
 *  handle verbosity levels.
 *
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setVerbosity(int level){
  if(level < -4 || level > 4 ){
    nping_fatal(QT_3,"setVerbosity(): Invalid verbosity level supplied\n");
  }else{
    switch(level){
      case -4:  vb=QT_4;  break;
      case -3:  vb=QT_3;  break;
      case -2:  vb=QT_2;  break;
      case -1:  vb=QT_1;  break;
      case  0:  vb=VB_0;  break;
      case  1:  vb=VB_1;  break;
      case  2:  vb=VB_2;  break;
      case  3:  vb=VB_3;  break;
      case  4:  vb=VB_4;  break;
      default:
        nping_fatal(QT_3,"setVerbosity():2: Invalid verbosity level supplied\n");
      break;
    }
  }
  this->vb_set=true;
  return OP_SUCCESS;
} /* End of setVerbosity() */


/** Returns value of attribute vb (current verbosity level) */
int NpingOps::getVerbosity(){
  return vb;
} /* End of getVerbosity() */



/* Returns true if option has been set */
bool NpingOps::issetVerbosity(){
  return this->vb_set;
} /* End of issetVerbosity() */


/** Increments verbosity level by one. (When it reaches VB_4 it stops
  * getting incremented)
  * @return previous verbosity level */
int NpingOps::increaseVerbosity(){
  this->vb_set=true;
  if (vb < VB_4){
    vb++;
    return vb-1;
  }else{
    return vb;
  }
} /* End of increaseVerbosity() */


/** Decreases verbosity level by one. (When it reaches QT_4 it stops
  * getting incremented)
  * @return previous verbosity level */
int NpingOps::decreaseVerbosity(){
  this->vb_set=true;
  if (vb > QT_4){
    vb--;
    return vb+1;
  }else{
    return vb;
  }
} /* End of decreaseVerbosity() */


/** Sets debugging level. Supplied level must be an integer between DBG_0 and
 *  DBG_9. Check file nping.h for details
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setDebugging(int level){
  if(level < 0 || level > 9){
    nping_fatal(QT_3,"setDebugging(): Invalid debugging level supplied\n");
    return OP_FAILURE;
  }else{
    this->dbg= DBG_0  + level;
  }
  this->dbg_set=true;
  return OP_SUCCESS;
} /* End of setDebugging() */


/** Returns value of attribute dbg (current debugging level) */
int NpingOps::getDebugging(){
  return dbg;
} /* End of getDebugging() */


/** Increments debugging level by one. (When it reaches DBG_9 it stops
  * getting incremented)
  *   * @return previous verbosity level */
int NpingOps::increaseDebugging(){
  this->dbg_set=true;
  if (dbg < DBG_9){
    dbg++;
    return dbg-1;
  }else{
    return dbg;
  }
} /* End of increaseDebugging() */


/* Returns true if option has been set */
bool NpingOps::issetDebugging(){
    return this->dbg_set;
} /* End of issetDebugging() */


/** Sets ShowSentPackets.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setShowSentPackets(bool val){
  this->show_sent_pkts=val;
  this->show_sent_pkts_set=true;
  return OP_SUCCESS;
} /* End of setShowSentPackets() */


/** Returns value of attribute show_sent_pkts */
bool NpingOps::showSentPackets(){
  return this->show_sent_pkts;
} /* End of showSentPackets() */


/* Returns true if option has been set */
bool NpingOps::issetShowSentPackets(){
  return this->show_sent_pkts_set;
} /* End of issetShowSentPackets() */


/** Control whether ethernet frames are displayed or not.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setShowEth(bool val){
  this->show_eth=val;
  return OP_SUCCESS;
} /* End of setShowEth() */


/** Returns true if the ethernet headers need to be printed.  */
bool NpingOps::showEth(){
  return this->show_eth;
} /* End of showEth() */


/** Control whether the stats for unresponsive hosts are printed out or not.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setShowAllStats(bool val){
  this->show_all_stats=val;
  return OP_SUCCESS;
} /* End of setShowAllStats() */


/** Returns true if the ethernet headers need to be printed.  */
bool NpingOps::showAllStats(){
  return this->show_all_stats;
} /* End of showAllStats() */


/* Returns the level of detail to use when printing packets. The returned
 * value is suitable to be passed to the print() function of any PacketElement
 * instance. */
int NpingOps::getDetailLevel(){
  if(this->vb>=VB_2)
    return HIGH_DETAIL;
  else if(this->vb==VB_1)
    return MEDIUM_DETAIL;
  else
    return LOW_DETAIL;
} /* End of getDetailLevel() */


/******************************************************************************
 *  Operation and Performance                                                 *
 ******************************************************************************/

/** Sets packet rounds (number of times that we iterate over each target host)
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setRounds(u64 val){
  /* If zero is supplied, set highest value */
  if(val==0)
    this->rounds=(~0);
  else
    this->rounds=val;
  this->rounds_set=true;
  return OP_SUCCESS;
} /* End of setRounds() */


/** Returns value of attribute pcount (number of packets that should be sent
 *  to each target)  */
u64 NpingOps::getRounds(){
  return this->rounds;
} /* End of getRounds() */


/* Returns true if option has been set */
bool NpingOps::issetRounds(){
  return this->rounds_set;
} /* End of issetRounds() */


/** Sets attribute sendpref which defines user's preference for packet
 *  sending. Supplied parameter must be an integer with one of these values:
 *  PACKET_SEND_NOPREF, PACKET_SEND_ETH or PACKET_SEND_IP.
 *  @return OP_SUCCESS. */
int NpingOps::setSendPreference(int v){
  assert(v==PACKET_SEND_NOPREF || v==PACKET_SEND_ETH || v==PACKET_SEND_IP );
  this->sendpref=v;
  return OP_SUCCESS;
} /* End of setSendPreference() */


/** Returns value of attribute sendpref */
int NpingOps::getSendPreference(){
  return this->sendpref;
} /* End of getSendPreference() */


/** Sets inter-probe delay. Supplied parameter is assumed to be in milliseconds
 *  and must be a long integer greater than zero.
 *  @warning timeout is assumed to be in milliseconds. Use tval2msecs() from
 *           nbase to obtain a proper value.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setDelay(long t){
  if(t < 0 )
    nping_fatal(QT_3,"setDelay(): Invalid time supplied\n");
  this->delay=t;
  this->delay_set=true;
  return OP_SUCCESS;
} /* End of setDelay() */


/** Returns value of attribute delay */
long NpingOps::getDelay(){
  return delay;
} /* End of getDelay() */


/* Returns true if option has been set */
bool NpingOps::issetDelay(){
  return this->delay_set;
} /* End of issetDelay() */


/** Sets network device. Supplied parameter must be a valid network interface
 *  name.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setDevice(char *n){
  if(n==NULL ){
    nping_fatal(QT_3,"setDevice(): Invalid value supplied\n");
  }else{
    Strncpy(this->device, n, MAX_DEV_LEN-1);
  }
  this->device_set=true;
  return OP_SUCCESS;
} /* End of setDevice() */


char *NpingOps::getDevice(){
  return this->device;
} /* End of getDevice() */


/* Returns true if option has been set */
bool NpingOps::issetDevice(){
  return this->device_set;
} /* End of issetDevice() */


/** Sets BPFFilterSpec.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setBPFFilterSpec(char *val){
  this->bpf_filter_spec=val;
  this->bpf_filter_spec_set=true;
  return OP_SUCCESS;
} /* End of setBPFFilterSpec() */


/** Returns value of attribute bpf_filter_spec */
char *NpingOps::getBPFFilterSpec(){
  return this->bpf_filter_spec;
} /* End of getBPFFilterSpec() */


/* Returns true if option has been set */
bool NpingOps::issetBPFFilterSpec(){
  return this->bpf_filter_spec_set;
} /* End of issetBPFFilterSpec() */


/** Sets CurrentRound.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setCurrentRound(int val){
  this->current_round=val;
  return OP_SUCCESS;
} /* End of setCurrentRound() */


/** Returns value of attribute current_round */
int NpingOps::getCurrentRound(){
  return this->current_round;
} /* End of getCurrentRound() */


bool NpingOps::havePcap(){
  return this->have_pcap;
} /* End of havePcap() */


int NpingOps::setHavePcap(bool val){
  this->have_pcap=val;
  return OP_SUCCESS;
} /* End of setHavePcap() */


/** Sets DisablePacketCapture.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setDisablePacketCapture(bool val){
  this->disable_packet_capture=val;
  this->disable_packet_capture_set=true;
  return OP_SUCCESS;
} /* End of setDisablePacketCapture() */


/** Returns value of attribute disable_packet_capture */
bool NpingOps::disablePacketCapture(){
  return this->disable_packet_capture;
} /* End of disablePacketCapture() */


/* Returns true if option has been set */
bool NpingOps::issetDisablePacketCapture(){
  return this->disable_packet_capture_set;
} /* End of issetDisablePacketCapture() */


/* Pass "true" to indicate that Nping is targetting multicast addresses, and
 * therefore, we can receive more than one reply for the same probe. Pass
 * "false" if we are targeting unicast IP addresses. */
bool NpingOps::doMulticast(bool new_val){
  bool old_val=this->do_multicast;
  this->do_multicast=new_val;
  return old_val;
} /* End of doMulticast() */


/* Returns true if we are sending probes to some multicast address. When that
 * happens, Nping should be able to handle multiple responses, coming from
 * different targets, triggered by the same single probe. */
bool NpingOps::doMulticast(){
  return this->do_multicast;
} /* End of doMulticast() */

/** Sets the IP address family that will be used in all packets. Supplied
 * parameter must be either AF_INET or AF_INET6. */
int NpingOps::setAddressFamily(int addrfamily){
  assert(addrfamily==AF_INET || addrfamily==AF_INET6);
  this->family=addrfamily;
  return OP_SUCCESS;
} /* End of setIPVersion() */


/* Returns true if we are using IPv4 */
bool NpingOps::ipv4(){
  return this->family==AF_INET;
} /* End of ipv4() */


/* Returns true if we are using IPv6 */
bool NpingOps::ipv6(){
  return this->family==AF_INET6;
} /* End of ipv6() */


/* Returns AF_INET or AF_INET6, depending on current configuration */
int NpingOps::af(){
  return this->family;
} /* End of af() */


/******************************************************************************
 *  User types and Privileges                                                 *
 ******************************************************************************/

/** Sets value of attribute isr00t.
 *  @returns previous isr00t value */
int NpingOps::setIsRoot(int v) {
  int prev=this->isr00t;
  this->isr00t = (v==0) ? 0 : 1;
  this->isr00t_set=true;
  return prev;
} /* End of setIsRoot() */


/** Sets attribute isr00t to value 1.
 *  @returns previous isr00t value */
int NpingOps::setIsRoot() {
  int prev=this->isr00t;
  this->isr00t=1;
  this->isr00t_set=true;
 return prev;
} /* End of setIsRoot() */


/* Returns the state of attribute isr00t */
bool NpingOps::isRoot() {
  return (this->isr00t);
} /* End of isRoot() */


/* Returns true if option has been set */
bool NpingOps::issetIsRoot(){
  return this->isr00t_set;
} /* End of isset() */


/******************************************************************************
 *  Payloads                                                                  *
 ******************************************************************************/

/** Sets payload type. Supplied parameter must be one of: PL_NONE, PL_HEX,
 * PL_RAND, PL_FILE or PL_STRING;
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setPayloadType(int t){
  assert(t==PL_RAND || t==PL_HEX || t==PL_FILE || t==PL_STRING || t==PL_NONE);
  this->payload_type=t;
  this->payload_type_set=true;
  return OP_SUCCESS;
} /* End of setPayloadType() */


/** Returns value of attribute payload_type */
int NpingOps::getPayloadType(){
  return this->payload_type;
} /* End of getPayloadType() */


/** Sets payload buffer pointer. Supplied pointer must be a free()able
 *  non-NULL pointer; Supplied length must be a positive integer.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setPayloadBuffer(u8 *p, int len){
  if(p==NULL || len < 0 ){
    nping_fatal(QT_3,"setPayloadBuffer(): Invalid value supplied\n");
    return OP_FAILURE;
  }else{
    this->payload_buff=p;
    this->payload_len=len;
  }
  this->payload_buff_set=true;
  this->payload_len_set=true;
  return OP_SUCCESS;
} /* End of setPayloadBuffer() */


/** Returns value of attribute payload_type */
u8 *NpingOps::getPayloadBuffer(){
  return this->payload_buff;
} /* End of getPayloadBuffer() */


/** Returns value of attribute payload_len */
int NpingOps::getPayloadLen(){
  return this->payload_len;
} /* End of getPayloadLen() */


/******************************************************************************
 *  Roles (normal, client, server... )                                        *
 ******************************************************************************/

/** Sets Nping's role. Supplied argument must be one of: ROLE_NORMAL,
 *  ROLE_CLIENT or ROLE_SERVER.
 *  @return OP_SUCCESS on success or OP_FAILURE in case of error.  */
int NpingOps::setRole(int r){
  assert(r==ROLE_NORMAL || r==ROLE_CLIENT || r==ROLE_SERVER);
  this->role=r;
  return OP_SUCCESS;
} /* End of setRole() */


/* Returns Nping's current role. */
int NpingOps::getRole(){
  return this->role;
} /* End of getRole() */


/******************************************************************************
 * Internet Protocol                                                          *
 ******************************************************************************/

/** Sets Maximum Transmission Unit length. Supplied parameter must be a positive
 *  integer and must be a multiple of 8.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setMTU(u32 t){
  if(t==0 || (t%8)!=0){
    nping_fatal(QT_3,"setMTU(): Invalid mtu supplied\n");
  }else{
    this->mtu=t;
    this->mtu_set=true;
  }
  return OP_SUCCESS;
} /* End of setMTU() */


/** Returns value of attribute mtu */
u32 NpingOps::getMTU(){
  return this->mtu;
} /* End of getMTU() */


/* Returns true if option has been set */
bool NpingOps::issetMTU(){
  return this->mtu_set;
} /* End of issetMTU() */


/* Sets the IPv4 "Protocol field" or the IPv6 "Next Header" field. */
int NpingOps::setIPProto(u8 proto){
  this->ip_proto=proto;
  this->ip_proto_set=true;
  return OP_SUCCESS;
} /* End of setIPProto() */

/* Returns the IANA protocol number that will be used for the IPv4
 * "Protocol" field or the IPv6 "Next Header" field. Note that when
 * users supply an explicit value, the IPProto may not match the actual
 * next header in the packet. */
u8 NpingOps::getIPProto(){
  return this->ip_proto;
} /* End of getIPProto() */


bool NpingOps::issetIPProto(){
  return this->ip_proto_set;
} /* End of issetIPProto() */


/** Sets the source address to be used in outgoing packets. */
int NpingOps::setSpoofAddress(IPAddress *addr){
  this->spoof_addr=addr;
  return OP_SUCCESS;
} /* End of setSpoofAddress() */


/** Sets the source address to be used in outgoing packets. */
int NpingOps::setSpoofAddress(IPAddress addr){
  IPAddress *myaddr=new IPAddress();
  *myaddr=addr;
  this->spoof_addr=myaddr;
  return OP_SUCCESS;
} /* End of setSpoofAddress() */


/** Returns the source address to be used in outgoing packets. */
IPAddress *NpingOps::getSpoofAddress(){
  return this->spoof_addr;
} /* End of getSpoofAddress() */


/** @warning  This method makes a copy of the supplied buffer. That copy will
 *  be free()ed by the NpingOps destructor.                                  */
int NpingOps::setIPOptions(char *txt){
  if (txt==NULL)
    nping_fatal(QT_3,"setIPOptions(): NULL pointer supplied\n");
  this->ip_options=strdup(txt) ;
  this->ip_options_set=true;
  return OP_SUCCESS;
} /* End of setIPOptions() */


char *NpingOps::getIPOptions(){
  return this->ip_options;
} /* End of getIPOptions() */


bool NpingOps::issetIPOptions(){
  return this->ip_options_set;
} /* End of issetIPOptions() */


/******************************************************************************
 * Transmission Control Protocol and User Datagram Protocol                   *
 ******************************************************************************/

/** @warning Returned ports are in HOST byte order */
u16 *NpingOps::getTargetPorts(u16 *len){
  if(len!=NULL)
    *len=this->tportcount;
  return this->target_ports;
} /* End of getTargetPorts() */


/** @warning ports in the supplied array must be in HOST byte order */
int NpingOps::setTargetPorts(u16 *ports_array, u16 total_ports){
  this->target_ports=ports_array;
  this->tportcount=total_ports;
  this->target_ports_set=true;
  return OP_SUCCESS;
} /* End of setTargetPorts() */


/* Returns true if option has been set */
bool NpingOps::issetTargetPorts(){
  return this->target_ports_set;
} /* End of issetTargetPorts() */


/* Returns true if the operation mode can use the -p or -g options (target and source ports) */
bool NpingOps::mode_uses_ports(){
  return (this->mode(DO_TCP_CONNECT) || this->mode(DO_TCP) || this->mode(DO_UDP) || this->mode(DO_UDP_UNPRIV));
} /* End of mode_uses_ports */


/** Sets TCP/UPD source ports.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error. */
int NpingOps::setSourcePorts(u16 *ports_array, u16 total_ports){
  this->source_ports=ports_array;
  this->sportcount=total_ports;
  this->source_ports_set=true;
  return OP_SUCCESS;
} /* End of setSourcePorts() */


/** @warning Returned ports are in HOST byte order */
u16 *NpingOps::getSourcePorts(u16 *len){
  if(len!=NULL)
    *len=this->sportcount;
  return this->source_ports;
} /* End of getSourcePorts() */


/* Returns true if option has been set */
bool NpingOps::issetSourcePorts(){
  return this->source_ports_set;
} /* End of issetSourcePorts() */


/******************************************************************************
 *  Ethernet                                                                  *
 ******************************************************************************/
/** Sets SourceMAC.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setSourceMAC(u8 * val){
  memcpy(this->src_mac, val, 6);
  this->src_mac_set=true;
  return OP_SUCCESS;
} /* End of setSourceMAC() */


/** Returns value of attribute src_mac */
u8 * NpingOps::getSourceMAC(){
  return this->src_mac;
} /* End of getSourceMAC() */


/* Returns true if option has been set */
bool NpingOps::issetSourceMAC(){
  return this->src_mac_set;
} /* End of issetSourceMAC() */


/** Sets DestMAC.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setDestMAC(u8 * val){
  memcpy(this->dst_mac, val, 6);
  this->dst_mac_set=true;
  return OP_SUCCESS;
} /* End of setDestMAC() */


/** Returns value of attribute dst_mac */
u8 * NpingOps::getDestMAC(){
  return this->dst_mac;
} /* End of getDestMAC() */


/* Returns true if option has been set */
bool NpingOps::issetDestMAC(){
  return this->dst_mac_set;
} /* End of issetDestMAC() */

/** Sets EtherType.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setEtherType(u16 val){
  this->eth_type=val;
  this->eth_type_set=true;
  return OP_SUCCESS;
} /* End of setEtherType() */


/** Returns value of attribute eth_type */
u16 NpingOps::getEtherType(){
  return this->eth_type;
} /* End of getEtherType() */


/* Returns true if option has been set */
bool NpingOps::issetEtherType(){
  return this->eth_type_set;
} /* End of issetEtherType() */



/******************************************************************************
 *  Address Resolution Protocol / Reverse Address Resolution Protocol         *
 ******************************************************************************/

/** Sets ARPOpCode.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setARPOpCode(u16 val){
  this->arp_opcode=val;
  this->arp_opcode_set=true;
  return OP_SUCCESS;
} /* End of setARPOpCode() */


/** Returns value of attribute arp_opcode */
u16 NpingOps::getARPOpCode(){
  return this->arp_opcode;
} /* End of getARPOpCode() */


/* Returns true if option has been set */
bool NpingOps::issetARPOpCode(){
  return this->arp_opcode_set;
} /* End of issetARPOpCode() */


/** Sets ARPSenderHwAddr.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setARPSenderHwAddr(u8 * val){
  memcpy(this->arp_sha, val, 6); /* MAC Address (6 bytes) */
  this->arp_sha_set=true;
  return OP_SUCCESS;
} /* End of setARPSenderHwAddr() */


/** Returns value of attribute arp_sha */
u8 * NpingOps::getARPSenderHwAddr(){
  return this->arp_sha;
} /* End of getARPSenderHwAddr() */


/* Returns true if option has been set */
bool NpingOps::issetARPSenderHwAddr(){
  return this->arp_sha_set;
} /* End of issetARPSenderHwAddr() */


/** Sets ARPTargetHwAddr.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setARPTargetHwAddr(u8 * val){
  memcpy(this->arp_tha, val, 6); /* MAC Address (6 bytes) */
  this->arp_tha_set=true;
  return OP_SUCCESS;
} /* End of setARPTargetHwAddr() */


/** Returns value of attribute arp_tha */
u8 * NpingOps::getARPTargetHwAddr(){
  return this->arp_tha;
} /* End of getARPTargetHwAddr() */


/* Returns true if option has been set */
bool NpingOps::issetARPTargetHwAddr(){
  return this->arp_tha_set;
} /* End of issetARPTargetHwAddr() */


/** Sets ARPSenderProtoAddr.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setARPSenderProtoAddr(struct in_addr val){
  this->arp_spa=val;
  this->arp_spa_set=true;
  return OP_SUCCESS;
} /* End of setARPSenderProtoAddr() */


/** Returns value of attribute arp_spa */
struct in_addr NpingOps::getARPSenderProtoAddr(){
  return this->arp_spa;
} /* End of getARPSenderProtoAddr() */


/* Returns true if option has been set */
bool NpingOps::issetARPSenderProtoAddr(){
  return this->arp_spa_set;
} /* End of issetARPSenderProtoAddr() */


/** Sets ARPTargetProtoAddr.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setARPTargetProtoAddr(struct in_addr val){
  this->arp_tpa=val;
  this->arp_tpa_set=true;
  return OP_SUCCESS;
} /* End of setARPTargetProtoAddr() */


/** Returns value of attribute arp_tpa */
struct in_addr NpingOps::getARPTargetProtoAddr(){
  return this->arp_tpa;
} /* End of getARPTargetProtoAddr() */


/* Returns true if option has been set */
bool NpingOps::issetARPTargetProtoAddr(){
  return this->arp_tpa_set;
} /* End of issetARPTargetProtoAddr() */



/******************************************************************************
 *  Echo mode                                                                 *
 ******************************************************************************/

/** Sets EchoPort.
 *  @return OP_SUCCESS on success and OP_FAILURE in case of error.           */
int NpingOps::setEchoPort(u16 val){
  this->echo_port=val;
  this->echo_port_set=true;
  return OP_SUCCESS;
} /* End of setEchoPort() */


/** Returns value of attribute echo_port */
u16 NpingOps::getEchoPort(){
  return this->echo_port;
} /* End of getEchoPort() */


/* Returns true if option has been set */
bool NpingOps::issetEchoPort(){
  return this->echo_port_set;
} /* End of issetEchoPort() */


int NpingOps::setEchoPassphrase(const char *str){
  strncpy(this->echo_passphrase, str, sizeof(echo_passphrase)-1);
  this->echo_passphrase_set=true;
  return OP_SUCCESS;
} /* End of setEchoPassphrase() */


char *NpingOps::getEchoPassphrase(){
  return this->echo_passphrase;
} /* End of getEchoPassphrase() */


bool NpingOps::issetEchoPassphrase(){
  return this->echo_passphrase_set;
} /* End of issetEchoPassphrase() */

/** Sets value of this->echo_server_once. When true, the echo server
  * will exit after attending one client. */
int NpingOps::setOnce(bool val){
  this->echo_server_once=val;
  this->echo_server_once_set=true;
  return OP_SUCCESS;
} /* End of once() */


/** Returns value of attribute echo_port */
bool NpingOps::once(){
  return this->echo_server_once;
} /* End of once() */


/******************************************************************************
 *  Option Validation                                                         *
 ******************************************************************************/

void NpingOps::validateOptions() {

  /** DETERMINE ROOT PRIVILEGES ************************************************/
  const char *privreq = "root privileges";
  #ifdef WIN32
    //if (!this->have_pcap)
        privreq = "that WinPcap version 3.1 or higher and iphlpapi.dll be installed. You seem to be missing one or both of these.  Winpcap is available from http://www.winpcap.org.  iphlpapi.dll comes with Win98 and later operating systems and NT 4.0 with SP4 or greater.  For previous Windows versions, you may be able to take iphlpapi.dll from another system and place it in your system32 dir (e.g. c:\\windows\\system32)";
  #endif
  /* If user did not specify --privileged or --unprivileged explicitly, try to
   * determine if has root privileges. */
  if(!this->issetIsRoot() ){
    #if defined WIN32 || defined __amigaos__
    /* TODO: Check this because although nmap does exactly the same, it has a this->have_pcap that may affect to this */
    this->setIsRoot(1);
    #else
      if (getenv("NMAP_PRIVILEGED") || getenv("NPING_PRIVILEGED"))
        this->setIsRoot(1);
      else if (getenv("NMAP_UNPRIVILEGED") || getenv("NPING_UNPRIVILEGED"))
        this->setIsRoot(0);
      else
        this->setIsRoot( !(geteuid()) );
     #endif
  }

  if (this->havePcap()==false){
    #ifdef WIN32
      nping_fatal(QT_3, "Nping requires %s", privreq);
    #else
      nping_fatal(QT_3, "Nping requires libpcap to be installed on your system.");
    #endif
  }

/** TARGET SPECIFICATION *****************************************************/
  /* Check if user entered at least one target spec */
  if(this->target_specs.size()<=0){
    if(this->getRole() == ROLE_NORMAL ){
      nping_fatal(QT_3,"WARNING: No targets were specified, so 0 hosts pinged.");
    }else if(this->getRole() == ROLE_CLIENT ){
      nping_fatal(QT_3,"No echo server was specified.");
    }
  }

/** PROBE MODE SELECTION *****************************************************/
  /* Ensure that one probe mode is selected */
  if(this->getModes()==NO_MODE_SET || this->getModes()==DO_TRACEROUTE){
    if (this->isRoot())
      this->addMode(DO_ICMP);
    else
      this->addMode(DO_TCP_CONNECT);
  }

/** PACKET COUNT / ROUNDS ****************************************************/
  /* If --traceroute is set but the users has not specified any custom packet
   * count, set the packet count to something higher, so we reach hosts over
   * the Internet. */
  if(!this->issetRounds() && this->mode(DO_TRACEROUTE) ){
    this->setRounds(TRACEROUTE_PACKET_ROUNDS);
  }

  /* In traceroute mode, make the TTL field incremental and start from TTL=1 */
  if(this->mode(DO_TRACEROUTE)){
    if(!this->custom_ttl_set)
      this->ip4.ttl.setStartValue(1);
    this->ip4.ttl.setBehavior(FIELD_TYPE_INCREMENTAL);
    if(!this->custom_ttl_set)
      this->ip6.hlim.setStartValue(1);
    this->ip6.hlim.setBehavior(FIELD_TYPE_INCREMENTAL);
  }

/** UDP UNPRIVILEGED MODE? ***************************************************/
  /* If user is NOT root and specified UDP mode, check if he did not specify
   * any option that requires privileges. In that case, we enter
   * UDP-Unprivileged mode, where users can send UDP packets and read responses
   * trough a normal UDP socket.  */
  if(!this->isRoot() && this->mode(DO_UDP) && canRunUDPWithoutPrivileges() ){
    this->addMode(DO_UDP_UNPRIV);
    this->delMode(DO_UDP);
  }


/** CHECK PRIVILEGES FOR CURRENT ROLE ****************************************/
  if(!this->isRoot() && (this->getRole()==ROLE_SERVER || this->getRole()==ROLE_CLIENT) )
    nping_fatal(QT_3,"Echo mode requires %s.", privreq);

/** CHECK PRIVILEGES FOR CURRENT MODE ****************************************/
  if(!this->isRoot()){
    /* If the argparser set UDP mode but we are not root, just go
     * into UDP-Unpriv mode*/
    if(this->mode(DO_UDP)){
        this->delMode(DO_UDP);
        this->addMode(DO_UDP_UNPRIV);
    }
    /* Now do a proper check for privileged modes. */
    if(this->mode(DO_TCP) || this->mode(DO_UDP) || this->mode(DO_ARP) || this->mode(DO_ICMP) ||
        this->mode(DO_TRACEROUTE) || this->mode(DO_EXT_HOPOPT) || this->mode(DO_EXT_ROUTING) ||
        this->mode(DO_EXT_DOPT) || this->mode(DO_EXT_FRAGMENT) )
      nping_fatal(QT_3,"Mode %s requires %s.", this->mode2Ascii( this->getModes() ), privreq);
  }

/** ARP MODE RELATED PARAMETERS *********************************************/
  if(this->mode(DO_ARP) && this->ipv6()) {
    nping_fatal(QT_3, "Sorry, ARP does not support IPv6.");
  }

 if(this->mode(DO_TCP_CONNECT) || this->mode(DO_UDP_UNPRIV) )
    nping_print(DBG_2,"Nping will send packets in unprivileged mode using regular system calls");

/** ECHO MODE ************************************************************/
  if(this->getRole()==ROLE_CLIENT){
    /* Make sure the nping echo client does not generate packets with tcp
     * src port or tcp dst port 9929 (or --echo-port N, if that is set),
     * because 1) the echo server does not capture those packets and 2) to
     * avoid messing with the established side-channel tcp connection. */
    if(this->mode(DO_TCP)){
      if(this->target_ports!=NULL && this->tportcount>0){
        for(int i=0; i<this->tportcount; i++){
          if(this->target_ports[i]==this->getEchoPort())
            nping_fatal(QT_3, "Packets can't be sent to the same port that is used to connect to the echo server (%d)", this->getEchoPort());
        }
      }
      if(this->source_ports!=NULL && this->sportcount>0){
        for(int i=0; i<this->sportcount; i++){
          if(this->source_ports[i]==this->getEchoPort())
            nping_fatal(QT_3, "Packets can't be sent from the same port that is used to connect to the echo server (%d)", this->getEchoPort());
        }
      }
    }

    /* Check the echo client only produces TCP/UDP/ICMP packets */
    if(this->mode(DO_ARP))
      nping_fatal(QT_3, "The echo client can't be run with protocols other than TCP, UDP or ICMP.");

    /* No TCP-Connect or UDP-Unpriv in Echo mode */
    if(this->mode(DO_TCP_CONNECT))
      nping_fatal(QT_3, "TCP-Connect is not supported in Echo mode.");
    if(this->mode(DO_UDP_UNPRIV))
      nping_fatal(QT_3, "Unprivileged UDP is not supported in Echo mode.");

    /* Now let's check if we are running in echo client mode. In this case
     * the protocol fields cannot vary. Otherwise packets would differ from
     * the specification passed to the server during the session
     * establishment (NEP_PACKET_SPEC), and the matching engine wouldn't be
     * able to identify and echo them. For this reason, in Echo Client mode,
     * we need to avoid ProbeFields with the FIELD_TYPE_INCREMENTAL behavior
     * (See HeaderTemplates.cc for details). So here, we go over the list
     * of fields of the headers we plan to send and we change their behavior
     * to make sure they keep constant (FILED_TYPE_CONSTANT).*/
      nping_print(DBG_4, "Preventing variable protocol fields in Echo client mode.");
      this->ip4.id.setBehavior(FIELD_TYPE_CONSTANT);
      this->tcp.sport.setBehavior(FIELD_TYPE_CONSTANT);
      this->tcp.seq.setBehavior(FIELD_TYPE_CONSTANT);
      this->udp.sport.setBehavior(FIELD_TYPE_CONSTANT);
      this->icmp4.seq.setBehavior(FIELD_TYPE_CONSTANT);
      this->icmp6.seq.setBehavior(FIELD_TYPE_CONSTANT);
  }else if(this->getRole()==ROLE_SERVER){
    /* If the user did not pass any interface to listen on, pick one */
    if(!this->issetDevice()){
      char *mydev=select_network_iface();
      this->setDevice(mydev);
    }
  }
  #ifndef HAVE_OPENSSL
  if(this->getRole()==ROLE_CLIENT || this->getRole()==ROLE_SERVER ){
    if(this->doCrypto()==true  ){
        nping_fatal(QT_3, "Nping was compiled without OpenSSL so authentications need to be transmitted as cleartext. If you wish to continue, please specify --no-crypto.");
    }
  }
  #endif

/** FRAGMENTATION ************************************************************/
#if !defined(LINUX) && !defined(OPENBSD) && !defined(FREEBSD) && !defined(NETBSD)
  if (this->issetMTU()) {
    error("Warning: Packet fragmentation selected on a host other than Linux, OpenBSD, FreeBSD, or NetBSD.  This may or may not work.");
  }
#endif

/** MISCELLANEOUS ************************************************************/
  if(this->source_ports!=NULL && this->mode(DO_TCP_CONNECT) && (u16)this->getRounds()>this->sportcount ){
    if(!this->isRoot()){
      nping_warning(QT_1, "Warning: Setting a source port in TCP-Connect mode may not work if you are not root");
    }else{
      nping_warning(QT_1, "Warning: In TCP-Connect mode, setting less source ports than packet rounds may not work. You may want to reduce the number of rounds or increase the number of source ports. (use --count 1).");
    }
  }

  /*If nping is called on something that doesn't take port scanning
   * we should alert the user that their port command is going to be ignored
   * I choose to print out a Fatal error since the scan doesn't make sense. */
  if(!this->mode_uses_ports()){
    if(this->issetTargetPorts())
      nping_fatal(QT_3, "You cannot use -p (explicit port selection) in your current scan mode.\n(Perhaps you meant to use --tcp or --udp)");
    else if(this->issetSourcePorts())
      nping_fatal(QT_3, "You cannot use -g (explicit source port selection) in your current scan mode.\n(Perhaps you meant to use --tcp or --udp)");
  }
} /* End of validateOptions() */


/** Returns true if requested mode is a simple TCP connect probe mode */
bool NpingOps::canRunUDPWithoutPrivileges(){
  if(this->ip4.csum.getBehavior()==FIELD_TYPE_BADSUM ||
    this->ip4.ttl.is_set() ||
    //this->ip6.hlim.is_set() ||
    this->ip4.tos.is_set() ||
    this->ip4.id.is_set() ||
    this->ip4.rf.is_set() ||
    this->ip4.df.is_set() ||
    this->ip4.mf.is_set() ||
    this->getSpoofAddress()!=NULL ||
    this->issetIPOptions() ||
    this->issetMTU() ||
    this->issetSourceMAC() ||
    this->issetDestMAC() ||
    this->issetEtherType() ||
    this->mode(DO_TRACEROUTE) ||
    this->issetBPFFilterSpec()
  )
    return false;
  else
    return true;
} /* End canRunUDPWithoutPrivileges() */


/******************************************************************************
 *  Miscellaneous                                                             *
 ******************************************************************************/

void NpingOps::displayNpingDoneMsg(){
  if(this->getRole()==ROLE_SERVER){
    nping_print(QT_1, "Nping done: %lu %s served in %.2f seconds",
               (unsigned long)this->stats.get_clients_served(),
               (this->stats.get_clients_served()==1)? "client" : "clients",
               this->stats.get_runtime_elapsed(NULL)
              );
  }else{
    nping_print(QT_1, "Nping done: %lu %s pinged in %.2f seconds",
               (long unsigned int)this->target_hosts.size(),
               (this->target_hosts.size() == 1)? "IP address" : "IP addresses",
               this->stats.get_runtime_elapsed(NULL)
              );
  }
} /* End of displayNpingDoneMessage() */


/** @warning This method calls targets.rewind() */
void NpingOps::displayStatistics(){
  char auxbuff[256];
  memset(auxbuff, 0, 256);
  bool print_echoed=(this->getRole()==ROLE_CLIENT);

  nping_print(VB_0|NO_NEWLINE,"\n"); /* Print newline */
 
  /* Per-target RTT statistics */
  if(this->getRole()!=ROLE_SERVER){
    if(this->target_hosts.size() > 1){
      for(u32 i=0; i<this->target_hosts.size(); i++){

        /* First, check if we got any response from this target host. If we
         * didn't, there is no point on printing invidual stats (unless the
         * user passed --show-all-stats, of course. */
        if(this->showAllStats()==false){
          if(this->target_hosts[i]->stats.get_accepts(STATS_TCP)==0 &&
             this->target_hosts[i]->stats.get_reads(STATS_UDP)==0 &&
             this->target_hosts[i]->stats.get_rcvd(STATS_TOTAL)==0)
              /* Host is unresponsive, skip its stats */
              continue;
        }

        nping_print(VB_0|NO_NEWLINE, "Statistics for host %s" , this->target_hosts[i]->getTargetAddress()->toString());
        if(!IPAddress::isIPAddress(this->target_hosts[i]->getHostname()))
          nping_print(VB_0|NO_NEWLINE, " (%s)", this->target_hosts[i]->getHostname());
        nping_print(VB_0|NO_NEWLINE, ":\n");
        if(this->mode(DO_TCP) || this->mode(DO_UDP) || this->mode(DO_ICMP) || this->mode(DO_ARP)){
          this->target_hosts[i]->stats.print_proto_stats(STATS_TOTAL, " |_ ", print_echoed);
        }
        if(this->mode(DO_TCP_CONNECT)){
          this->target_hosts[i]->stats.print_proto_stats(STATS_TCP_CONNECT, " |_ ", print_echoed);
        }
        if(this->mode(DO_UDP_UNPRIV)){
          this->target_hosts[i]->stats.print_proto_stats(STATS_UDP_UNPRIV, " |_ ", print_echoed);
        }
        /* If we sent more than one type of packet, print separate stats for each protocol*/
        if(((int)this->mode(DO_TCP) + (int)this->mode(DO_UDP) + (int)this->mode(DO_ICMP) + (int)this->mode(DO_ARP) + this->mode(DO_TCP_CONNECT) + this->mode(DO_UDP_UNPRIV)) >  1){
          if(this->mode(DO_TCP)){
            this->target_hosts[i]->stats.print_proto_stats(STATS_TCP, " |_ ", print_echoed);
          }
          if(this->mode(DO_UDP)){
            this->target_hosts[i]->stats.print_proto_stats(STATS_UDP, " |_ ", print_echoed);
          }
          if(this->mode(DO_ARP)){
            this->target_hosts[i]->stats.print_proto_stats(STATS_ARP, " |_ ", print_echoed);
          }
          if(this->mode(DO_ICMP)){
            if(this->target_hosts[i]->stats.get_sent(STATS_ICMPv4)>0){
              this->target_hosts[i]->stats.print_proto_stats(STATS_ICMPv4, " |_ ", print_echoed);
            }
            if(this->target_hosts[i]->stats.get_sent(HEADER_TYPE_ICMPv6)>0){
              this->target_hosts[i]->stats.print_proto_stats(STATS_ICMPv6, " |_ ", print_echoed);
            }
          }
        }

        /* Print per-target RTT stats */
        this->target_hosts[i]->stats.print_RTTs(" |_ ");
      }
    }
  }

  /* Sent/Recv/Echoed Packets */
  if(this->getRole()==ROLE_CLIENT){
    this->stats.print_proto_stats(STATS_TOTAL, NULL, print_echoed);
  }else if(this->getRole()==ROLE_SERVER){
    this->stats.print_proto_stats(STATS_ECHO_SERVER, NULL, false);
  }else if(this->getRole()==ROLE_NORMAL){
    if(this->mode(DO_TCP) || this->mode(DO_UDP) || this->mode(DO_ICMP) || this->mode(DO_ARP)){
      this->stats.print_proto_stats(STATS_TOTAL, NULL, print_echoed);

      /* If we sent more than one type of packet, print separate stats for each protocol*/
      if(((int)this->mode(DO_TCP) + (int)this->mode(DO_UDP) + (int)this->mode(DO_ICMP) + (int)this->mode(DO_ARP) + this->mode(DO_TCP_CONNECT) + this->mode(DO_UDP_UNPRIV)) >  1){
        if(this->mode(DO_TCP)){
          this->stats.print_proto_stats(STATS_TCP, NULL, print_echoed);
        }
        if(this->mode(DO_UDP)){
          this->stats.print_proto_stats(STATS_UDP, NULL, print_echoed);
        }
        if(this->mode(DO_ARP)){
          this->stats.print_proto_stats(STATS_ARP, NULL, print_echoed);
        }
        if(this->mode(DO_ICMP)){
          if(this->stats.get_sent(HEADER_TYPE_ICMPv4)>0){
            this->stats.print_proto_stats(STATS_ICMPv4, NULL, print_echoed);
          }
          if(this->stats.get_sent(HEADER_TYPE_ICMPv6)>0){
            this->stats.print_proto_stats(STATS_ICMPv6, NULL, print_echoed);
          }
        }
      }
    }
    if(this->mode(DO_TCP_CONNECT)){
      this->stats.print_proto_stats(STATS_TCP_CONNECT, NULL, false);

    }
    if(this->mode(DO_UDP_UNPRIV)){
      this->stats.print_proto_stats(STATS_UDP_UNPRIV, NULL, false);

    }
    this->stats.print_RTTs(NULL);
  }

  /* Transmission times & rates */
  nping_print(QT_1|NO_NEWLINE,"Tx time: %.5lfs ", this->stats.get_tx_elapsed() );
  nping_print(QT_1|NO_NEWLINE,"| Tx bytes/s: %.2lf ", this->stats.get_tx_byte_rate() );
  nping_print(QT_1,"| Tx pkts/s: %.2lf", this->stats.get_tx_pkt_rate() );
  nping_print(QT_1|NO_NEWLINE,"Rx time: %.5lfs ", this->stats.get_rx_elapsed() );
  nping_print(QT_1|NO_NEWLINE,"| Rx bytes/s: %.2lf ", this->stats.get_rx_byte_rate() );
  nping_print(QT_1,"| Rx pkts/s: %.2lf", this->stats.get_rx_pkt_rate() );
} /* End of displayStatistics() */


/* Close open files, free allocated memory, etc. */
int NpingOps::cleanup(){
  return OP_SUCCESS;
} /* End of cleanup() */


char *NpingOps::select_network_iface(){
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *pcap_ifaces=NULL;

  /* Vars for the current interface in the loop */
  pcap_if_t *curr=NULL;             /* Current pcap pcap_if_t element   */
  bool current_has_address=false;   /* Does it have an addr of any type? */
  bool current_has_ipv6=false;      /* Does it have an IPv6 address?     */
  bool current_has_ipv4=false;      /* Does it have an IPv4 address?     */
  bool current_is_loopback=false;   /* Is it a loopback interface?       */
  bool select_current=false;        /* Is current better than candidate? */
  struct sockaddr_in6 devaddr6;     /* We store iface's IPv6 address     */
  struct sockaddr_in devaddr4;      /* And also its IPv4 address         */

  /* Vars for our candidate interface */
  pcap_if_t *candidate=NULL;
  bool candidate_has_address=false;
  bool candidate_has_ipv6=false;
  bool candidate_has_ipv4=false;
  bool candidate_is_loopback=false;
  //struct sockaddr_in6 candidate_addr6;
  //struct sockaddr_in candidate_addr4;

  /* Ask libpcap for a list of network interfaces */
  if(pcap_findalldevs(&pcap_ifaces, errbuf) != 0)
    nping_fatal(QT_3, "Cannot obtain device for packet capture --> %s. You may want to specify one explicitly using option -e", errbuf);

  /* Iterate over the interface list and select the best one */
  for(curr=pcap_ifaces; curr!=NULL; curr=curr->next){
    current_has_address=false;   candidate_has_ipv6=false;
    candidate_is_loopback=false; candidate_has_ipv4=false;
    select_current=false;

    if(curr->flags==PCAP_IF_LOOPBACK)
      current_is_loopback=true;

    /* Loop through the list of addresses */
    for(pcap_addr_t *curraddr=curr->addresses; curraddr!=NULL; curraddr=curraddr->next){
      current_has_address=true;
      if(curraddr->addr->sa_family==AF_INET){
        current_has_ipv4=true;
        memcpy( &devaddr4, curraddr->addr, sizeof(struct sockaddr_in));
      }else if(curraddr->addr->sa_family==AF_INET6){
        current_has_ipv6=true;
        memcpy( &devaddr6, curraddr->addr, sizeof(struct sockaddr_in6));
      }
    }

    /* If we still have no candidate, take the first one we find */
    if(candidate==NULL){
      select_current=true;
    }
    /* If we already have a candidate, check if the one we are
     * processing right now is better than the one we've already got */
    else{
      /* If our candidate does not have an IPv6 address but this one does,
       * select the new one. */
      if(candidate_has_ipv6==false && current_has_ipv6==true ){
        select_current=true;
      }
      /* If our candidate does not even have an IPv4 address but this
       * one does, select the new one. */
      else if(candidate_has_ipv4==false && candidate_has_ipv6==false && current_has_ipv4){
        select_current=true;
      }
      /* If our candidate is a loopback iface, select the new one */
      else if(candidate_is_loopback && !current_is_loopback){
        /* Select the new one only if it has an IPv6 address
         * and the old one didn't. If our old loopback iface
         * has an IPv6 address and this one does not, we
         * prefer to keep the loopback one, even though the
         * other is not loopback */
        if(current_has_ipv6==true){
          select_current=true;
        }
        /* We also prefer IPv4 capable interfaces than  */
        else if(candidate_has_ipv6==false && current_has_ipv4==true){
          select_current=true;
        }
      }
      /* If both are loopback, select the best one. */
      else if(candidate->flags==PCAP_IF_LOOPBACK && curr->flags==PCAP_IF_LOOPBACK){
        if(candidate_has_ipv6==false && current_has_ipv6)
          select_current=true;
      }
    }

    /* Did we determine that we should discard our old candidate? */
    if(select_current ){
      candidate=curr;
      candidate_has_address=current_has_address;
      candidate_has_ipv4=current_has_ipv4;
      candidate_has_ipv6=current_has_ipv6;
      candidate_is_loopback=current_is_loopback;
    }

    /* Let's see if we have the interface of our dreams... */
    if(candidate_has_address && candidate_has_ipv6 && candidate_has_ipv4 && candidate_is_loopback==false){
      break;
    }
  } /* End of for() loop */

  if(candidate==NULL)
    return NULL;
  else
    return candidate->name;
} /* End of select_network_iface() */


int NpingOps::setLastPacketSentTime(struct timeval t){
  this->last_sent_pkt_time=t;
  return OP_SUCCESS;
} /* End of setLastPacketSentTime() */


struct timeval NpingOps::getLastPacketSentTime(){
  return this->last_sent_pkt_time;
} /* End of getLastPacketSentTime() */


/** Sets the RCVD packet to be delayed. The function stores the packet pointer
  * so it can be freed after its contents are printed out. So be careful, no
  * free()ing on that packet! The "id" parameter is the nsock timer event
  * scheduled for the output of the RCVD string (usually scheduled by ProbeMode)
  * It is provided to allow other objects (like EchoClient) to cancel the event
  * if they take care of printing the RCVD string before the timer goes off.
  * The timestamp parameter is the number to be printed right after the "RCVD"
  * tag (e.g: RCVD (2.0423s) IPv4[127.0.0.1...)*/
int NpingOps::setDelayedRcvd(PacketElement *pkt, double timestamp, nsock_event_id id){
  this->delayed_rcvd_pkt=pkt;
  this->delayed_rcvd_event=id;
  this->delayed_rcvd_ts=timestamp;
  this->delayed_rcvd_pkt_set=true;
  return OP_SUCCESS;
} /* End of setDelayedRcvd() */


/** Returns a pointer to a delayed RCVD output packet. It returns non-NULL
  * PacketElements only once per setDelayedRcvd() call. This is, when a packet
  * has been set through a setDelayRcdv() call, the first time getDelayRcvd()
  * is called, it returns that packets. Subsequent calls will return NULL until
  * another string is set, using setDelayRcdv() again.
  * The "id" parameter will be filled with the timer event that was supposed
  * to print the message. If getDelayedRcvd() is called by the timer handler
  * itself, then NULL can be passed safely since the event id is not needed.
  * If the caller is some other method that wants to print the RCVD string
  * before the timer goes off, it may use the event ID to cancel the scheduled
  * event since it's no longer necessary.
  * @warning returned packet pointer points to the original memory area passed
  * to setDelayedRcvd(). The pointer is not freed, so the caller is responsible
  * for doing that. Note that the proper way to free it is by calling
  * PacketParser::freePacketChain().  */
PacketElement *NpingOps::getDelayedRcvd(double *timestamp, nsock_event_id *id){
  if(delayed_rcvd_pkt_set==false){
    return NULL;
  }else{
    this->delayed_rcvd_pkt_set=false;
    PacketElement *old=this->delayed_rcvd_pkt;
    this->delayed_rcvd_pkt=NULL;
    if(id!=NULL)
      *id=this->delayed_rcvd_event;
    if(timestamp!=NULL)
      *timestamp=this->delayed_rcvd_ts;
    return old;
  }
} /* End of getDelayedRcvd() */


bool NpingOps::doCrypto(){
  return this->do_crypto;
}


int NpingOps::doCrypto(bool value){
  this->do_crypto=value;
  return OP_SUCCESS;
}


/* Returns true if the echo server is allowed to include payloads in NEP_ECHO
 * messages. */
bool NpingOps::echoPayload(){
  return this->echo_payload;
}


/* Enables or disables payload echo for the echo server. Pass true to enable
 * or false to disable. */
int NpingOps::echoPayload(bool value){
  this->echo_payload=value;
  this->echo_payload_set=true;
  return OP_SUCCESS;
}


/** Returns the total number of probes to be sent (this takes into account
  * the number of rounds, ports, and targets. It returns a positive integer
  * on success and n<=0 in case of error. */
int NpingOps::getTotalProbes(){
  u16 total_ports=0;
  this->getTargetPorts(&total_ports);
  return this->getRounds() * total_ports * this->target_hosts.size();
}


/** Adds a target specification to an internal array of specs */
int NpingOps::addTargetSpec(const char *spec){
  if(spec==NULL)
    return OP_FAILURE;
  this->target_specs.push_back(spec);
  return OP_SUCCESS;
} /* End of addTargetSpec() */


/* Processes the internal array of specs and turns it into an array of
 * TargetHosts. */
int NpingOps::setupTargetHosts(){
  nping_print(DBG_4, "%s()", __func__);
  const char *errmsg=NULL;
  TargetHost *newhost=NULL;
  NetworkInterface *newiface=NULL;
  struct sockaddr_storage dst_ss;
  struct sockaddr_storage src_ss;
  struct route_nfo route;
  IPAddress *auxaddr;
  bool iface_found=false;
  bool do_eth=false;
  MACAddress destmac;

  /* Store the spoof address if we have it */
  if(this->spoof_addr!=NULL){
    this->spoof_addr->getAddress(&src_ss);
  }

  /* Turn each target spec into an array of addresses */
  nping_print(DBG_4, "Parsing target specs...");
  for(u32 i=0; i<this->target_specs.size(); i++){
    assert(this->af()==AF_INET || this->af()==AF_INET6 || this->af()==AF_UNSPEC);
    nping_print(DBG_4, "Parsing target spec #%u (%s)", i, this->target_specs[i]);
    /* We always use spec_to_addresses() but we call it in different ways depending
     * on the address family that we want to use. */
    if(starts_with(this->target_specs[i], "ipv4://")){
      nping_print(DBG_4, "Explicit IPv4 target (ipv4://)");
      errmsg=spec_to_addresses( this->target_specs[i]+strlen("ipv4://"), AF_INET, this->target_addresses, MAX_IPv4_NETMASK_ALLOWED);
    }else if(starts_with(this->target_specs[i], "ipv6://")){
      nping_print(DBG_4, "Explicit IPv6 target (ipv6://)");
      errmsg=spec_to_addresses( this->target_specs[i]+strlen("ipv6://"), AF_INET6, this->target_addresses, MAX_IPv6_NETMASK_ALLOWED);
    }else if(IPAddress::isIPv4Address(this->target_specs[i])){
      nping_print(DBG_4, "Explicit IPv4 address");
      errmsg=spec_to_addresses( this->target_specs[i], AF_INET, this->target_addresses, MAX_IPv4_NETMASK_ALLOWED);
    }else if(IPAddress::isIPv6Address(this->target_specs[i])){
      nping_print(DBG_4, "Explicit IPv6 address");
      errmsg=spec_to_addresses( this->target_specs[i], AF_INET6, this->target_addresses, MAX_IPv6_NETMASK_ALLOWED);
    }else if(this->af()==AF_INET){
      nping_print(DBG_4, "AF_INET set");
      errmsg=spec_to_addresses( this->target_specs[i], AF_INET, this->target_addresses, MAX_IPv4_NETMASK_ALLOWED);
    }else if(this->af()==AF_INET6){
      nping_print(DBG_4, "AF_INET6 set");
      errmsg=spec_to_addresses( this->target_specs[i], AF_INET6, this->target_addresses, MAX_IPv6_NETMASK_ALLOWED);
    }else{ // AF_UNSPEC
      nping_print(DBG_4, "No address family set. Using OS default.");
      errmsg=spec_to_addresses(this->target_specs[i], AF_UNSPEC, this->target_addresses, 0);
    }
    if(errmsg!=NULL){
      nping_warning(QT_1, "WARNING: %s (%s)", errmsg, this->target_specs[i]);
    }
    /* Store the targetspec that originated the address in a list. This is
     * done so later we can associate hostnames to TargetHost objects, and
     * we need to do it because otherwise, when IP ranges are expanded, it gets
     * impossible to tell which address came from what spec. */
    while(this->target_hostnames.size() < this->target_addresses.size()){
      this->target_hostnames.push_back(this->target_specs[i]);
    }
  }

  /* Now for each address, let's determine if we have enough info to route
   * packets to it. If we have, turn the address into a full TargetHost object
   * and store it in the list of targets */
  for(u32 i=0; i<this->target_addresses.size(); i++){

    /* Instantiate a new target host */
    nping_print(DBG_4, "Instantiate new TargetHost.");
    newhost = new TargetHost();
    /* Setup target and source addresses (if applicable) */
    newhost->setTargetAddress(this->target_addresses[i]);
    if(this->spoof_addr!=NULL){
      nping_print(DBG_4, "Setting spoofed address.");
      newhost->setSourceAddress(this->spoof_addr);
    }
    /* Setup hostname. Note that only holds a hostname when the user passed one.
     * If he passed an IP address, then the ASCII representation of the address
     * is what gets stored in the object. */
    newhost->setHostname(this->target_hostnames[i]);

    /* Determine if the target address is multicast or unicast. If it's
     * multicast, we set a global flag. This is important for some Nping
     * routines because when we target multicast addresses we can receive
     * more than one response for the same probe. Also, we should expect
     * responses from addresses that don't match the original probe's
     * destination IP. */
    if(newhost->getTargetAddress()->isMulticast()){
      this->doMulticast(true);
    }

    /* If we are running some mode that requires raw sockets, perform
     * route determination so we can fill up the TargetHost object with
     * extra info for later. If we are not, we just store it in the the global
     * NpingOps::target_hosts array right away. */
    if(this->mode(MODE_IS_PRIVILEGED)){
      nping_print(DBG_4, "Determine route for target #%u", i);
      memset(&dst_ss, 0, sizeof(struct sockaddr_storage));
      memset(&route, 0, sizeof(struct route_nfo));
      this->target_addresses[i]->getAddress(&dst_ss);

      /* Let's see if we can route packets to the current target. */
      if(route_dst(&dst_ss, &route, this->device_set ? this->device : NULL, this->spoof_addr!=NULL ? &src_ss : NULL) == 1 ){  // TODO: implement spoofsrc
        /* Yes, we can! Extract the info returned by route_dst() and store it
         * in the target's class */
        nping_print(DBG_4, "Route found!");

        /* Windows does not let us send raw packets to localhost so we skip
         * localhost if we detect it. */
        if(this->win32()){
          if (route.ii.device_type == devt_loopback){
            nping_warning(QT_2, "Skipping %s because Windows does not allow localhost scans (try --unprivileged).", this->target_addresses[i]->toString());
            delete newhost;
            continue;
          }
        }

        /* Set the source IP address. Note that we handle the case where
         * we have a spoof address above */
        if(this->spoof_addr==NULL){
          nping_print(DBG_4, "Setting source address.");
          auxaddr=new IPAddress();
          auxaddr->setAddress(route.srcaddr);
          newhost->setSourceAddress(auxaddr);
        }

        /* Network distance */
        if(route.direct_connect!=0){
          nping_print(DBG_4, "Target host is directly connected.");
          newhost->setNetworkDistance(DISTANCE_DIRECT);
        }else{
          nping_print(DBG_4, "Target host is more than one hop away.");
          auxaddr=new IPAddress();
          auxaddr->setAddress(route.nexthop);
          newhost->setNextHopAddress(auxaddr);
        }

        /* Interface information. Let's see if we previously found a target that
         * requires the same interface. */
        iface_found=false;
        for(u32 k=0; k<this->interfaces.size(); k++){
          if(!strcmp(this->interfaces[k]->getName(), route.ii.devname) ){
            iface_found=true;
            this->interfaces[k]->addAssociatedHost();
            newhost->setInterface(this->interfaces[k]);
            nping_print(DBG_4, "Same interface required. Reusing %s", route.ii.devname);
            break;
          }
        }
        /* If we haven't seen that interface before, instance a new NetworkInterface
         * and store it in the interface vector. */
        if(iface_found==false){
          newiface=new NetworkInterface(route.ii);
          newiface->addAssociatedHost();
          newhost->setInterface(newiface);
          this->interfaces.push_back(newiface);
          nping_print(DBG_4, "New interface required: %s", route.ii.devname);
        }

        /* Now let's see if we need to do address resolution on the target */
        do_eth=false;
	  /* In Windows, we always try sending packets at the Ethernet level, unless
	   * the user passed --send-ip.*/
	  if(this->win32() && this->getSendPreference()==PACKET_SEND_NOPREF){
          if(newhost->getInterface()->getType()!=devt_ethernet){
            nping_fatal(QT_3, "Ethernet device required on Windows platforms. Target host only reachable through a non-Ethernet network interface (%s).", newhost->getInterface()->getName());
          }else{
            do_eth=true;
          }
	  }else if(this->getSendPreference()==PACKET_SEND_NOPREF){
          /* For IPv6 we always try to send at the Ethernet level because many
           * systems impose big limitations on raw IPv6 sockets. We want to be
           * able to produce our own IPv6 headers and injecting packets at
           * the Ethernet level is the best way to do that.
           *
           * If the target is IPv4 then we don't need to make our life more
           * difficult resolving mac addresses, we just send through a raw
           * socket. */
          if(this->target_addresses[i]->getVersion()==AF_INET6){
            /* Of course, the network interface must be Ethernet.*/
            if(newhost->getInterface()->getType()==devt_ethernet)
              do_eth=true;
          }
          /* If we are doing ARP, we need Ethernet... */
          if(this->mode(DO_ARP) && this->target_addresses[i]->getVersion()==AF_INET){
            do_eth=true;
          }
        /* If the user explicitly requested Ethernet, go for it... */
        }else if(this->getSendPreference()==PACKET_SEND_ETH){
          /* ...unless the device is not Ethernet*/
          if(newhost->getInterface()->getType()!=devt_ethernet){
            nping_fatal(QT_3, "Ethernet requested for a host that is only reachable through a non-Ethernet network interface (%s).", newhost->getInterface()->getName());
          }else{
            do_eth=true;
          }
        }

        /* If we have determined that we should send at the Ethernet level and
         * we still don't have a next hop MAC address, we need to resolve it. */
        if(do_eth){
          nping_print(DBG_4, "Target will be reached sending packets at the Ethernet level...");
          /* Do not resolve it if the user passed a specific MAC address */
          if(!this->eth.dst.is_set()){
            /* First of all let's determine which IP address we need to use for
             * the MAC resolution. If we are directly connected to the host
             * then it's the host's address the one we are interested in. Otherwise
             * we'll use the address of the default gateway */
            IPAddress *address2resolve=NULL;;
            if(newhost->getNetworkDistance()==DISTANCE_DIRECT){
              address2resolve=newhost->getTargetAddress();
            }else{
              address2resolve=newhost->getNextHopAddress();
            }
            assert(address2resolve!=NULL);

            /* Now do the actual ARP/ND resolution */
            nping_print(DBG_4, "Attempting ARP/ND resolution...");
            if(mac_resolve(address2resolve, newhost->getSourceAddress(),newhost->getInterface(), &destmac)!=OP_SUCCESS){
              nping_warning(QT_1, "Failed to resolve MAC address for %s. Skipping target host %s", address2resolve->toString(),newhost->getTargetAddress()->toString());
              continue;
            }
            nping_print(DBG_4, "ARP/ND resolution done!");
          }
          /* Now set up the eth info and associate it with the current host */
          EthernetHeaderTemplate myeth;
          /* Source MAC address */
          if(this->eth.src.is_set()){
            myeth.src=this->eth.src;
          }else{
            myeth.src=newhost->getInterface()->getAddress();
          }
          /* Destination MAC address */
          if(this->eth.dst.is_set()){
            myeth.dst=this->eth.dst;
          }else{
            myeth.dst=destmac; /* This was provided by mac_resolve() */
          }
          /* Ether type */
          if(this->eth.type.is_set()){
            myeth.type=this->eth.type;
          }// Don't set it if the user didn't pass an explicit value
          newhost->setEth(myeth);
        }

        /* Now, tell the target host which packets it has to send. Note that when
         * we are doing ARP, the IP header is useless, but we still pass it. This
         * is OK, TargetHosts will ignore the header in this case. */
        nping_print(DBG_4, "Storing packet info in the TargetHost...");
        if(this->target_addresses[i]->getVersion()==AF_INET){
          newhost->setIPv4(this->ip4);
        }else{
          newhost->setIPv6(this->ip6);
        }
        if(this->mode(DO_ICMP)){
          if(this->target_addresses[i]->getVersion()==AF_INET){
            newhost->setICMPv4(this->icmp4);
          }else{
            newhost->setICMPv6(this->icmp6);
          }
        }
        if(this->mode(DO_TCP))
          newhost->setTCP(this->tcp);
        if(this->mode(DO_UDP))
          newhost->setUDP(this->udp);
        if(this->mode(DO_ARP) && this->target_addresses[i]->getVersion()==AF_INET)
          newhost->setARP(this->arp);
        if(this->payload_buff!=NULL)
          newhost->setPayload(this->payload_buff, this->payload_len);
      }else{
        /* We failed to determine a valid route for the target so we can't do anything but to skip it*/
        delete newhost;
        nping_warning(QT_1, "WARNING: Cannot find route for %s. Skipping that target host...", this->target_addresses[i]->toString());
        continue;
      }
    }

    /* Now we have all the info we need, so just add the new host to the globla
     * list of target hosts. */
    this->target_hosts.push_back(newhost);
    nping_print(DBG_2, "Added target host %s.", this->target_addresses[i]->toString() );
  }
  return OP_SUCCESS;
}/* End of setupTargetHosts() */


u32 NpingOps::totalTargetHosts(){
  return this->target_hosts.size();
} /* End of totalTargetHosts() */

/* Returns true if the underlying OS is a Microsoft Windows system. */
bool NpingOps::win32(){
  #ifdef WIN32
    return true;
  #else
    return false;
  #endif
} /* End of win32() */
