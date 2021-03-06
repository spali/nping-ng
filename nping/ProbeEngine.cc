
/***************************************************************************
 * ProbeEngine.cc -- Probe Mode is Nping's default working mode. Basically,*
 * it involves sending the packets that the user requested at regular      *
 * intervals and capturing responses from the wire.                        *
 *                                                                         *
 ***********************IMPORTANT NMAP LICENSE TERMS************************
 *                                                                         *
 * The Nmap Security Scanner is (C) 1996-2011 Insecure.Com LLC. Nmap is    *
 * also a registered trademark of Insecure.Com LLC.  This program is free  *
 * software; you may redistribute and/or modify it under the terms of the  *
 * GNU General Public License as published by the Free Software            *
 * Foundation; Version 2 with the clarifications and exceptions described  *
 * below.  This guarantees your right to use, modify, and redistribute     *
 * this software under certain conditions.  If you wish to embed Nmap      *
 * technology into proprietary software, we sell alternative licenses      *
 * (contact sales@insecure.com).  Dozens of software vendors already       *
 * license Nmap technology such as host discovery, port scanning, OS       *
 * detection, and version detection.                                       *
 *                                                                         *
 * Note that the GPL places important restrictions on "derived works", yet *
 * it does not provide a detailed definition of that term.  To avoid       *
 * misunderstandings, we consider an application to constitute a           *
 * "derivative work" for the purpose of this license if it does any of the *
 * following:                                                              *
 * o Integrates source code from Nmap                                      *
 * o Reads or includes Nmap copyrighted data files, such as                *
 *   nmap-os-db or nmap-service-probes.                                    *
 * o Executes Nmap and parses the results (as opposed to typical shell or  *
 *   execution-menu apps, which simply display raw Nmap output and so are  *
 *   not derivative works.)                                                *
 * o Integrates/includes/aggregates Nmap into a proprietary executable     *
 *   installer, such as those produced by InstallShield.                   *
 * o Links to a library or executes a program that does any of the above   *
 *                                                                         *
 * The term "Nmap" should be taken to also include any portions or derived *
 * works of Nmap.  This list is not exclusive, but is meant to clarify our *
 * interpretation of derived works with some common examples.  Our         *
 * interpretation applies only to Nmap--we don't speak for other people's  *
 * GPL works.                                                              *
 *                                                                         *
 * If you have any questions about the GPL licensing restrictions on using *
 * Nmap in non-GPL works, we would be happy to help.  As mentioned above,  *
 * we also offer alternative license to integrate Nmap into proprietary    *
 * applications and appliances.  These contracts have been sold to dozens  *
 * of software vendors, and generally include a perpetual license as well  *
 * as providing for priority support and updates as well as helping to     *
 * fund the continued development of Nmap technology.  Please email        *
 * sales@insecure.com for further information.                             *
 *                                                                         *
 * As a special exception to the GPL terms, Insecure.Com LLC grants        *
 * permission to link the code of this program with any version of the     *
 * OpenSSL library which is distributed under a license identical to that  *
 * listed in the included docs/licenses/OpenSSL.txt file, and distribute   *
 * linked combinations including the two. You must obey the GNU GPL in all *
 * respects for all of the code used other than OpenSSL.  If you modify    *
 * this file, you may extend this exception to your version of the file,   *
 * but you are not obligated to do so.                                     *
 *                                                                         *
 * If you received these files with a written license agreement or         *
 * contract stating terms other than the terms above, then that            *
 * alternative license agreement takes precedence over these comments.     *
 *                                                                         *
 * Source is provided to this software because we believe users have a     *
 * right to know exactly what a program is going to do before they run it. *
 * This also allows you to audit the software for security holes (none     *
 * have been found so far).                                                *
 *                                                                         *
 * Source code also allows you to port Nmap to new platforms, fix bugs,    *
 * and add new features.  You are highly encouraged to send your changes   *
 * to nmap-dev@insecure.org for possible incorporation into the main       *
 * distribution.  By sending these changes to Fyodor or one of the         *
 * Insecure.Org development mailing lists, it is assumed that you are      *
 * offering the Nmap Project (Insecure.Com LLC) the unlimited,             *
 * non-exclusive right to reuse, modify, and relicense the code.  Nmap     *
 * will always be available Open Source, but this is important because the *
 * inability to relicense code has caused devastating problems for other   *
 * Free Software projects (such as KDE and NASM).  We also occasionally    *
 * relicense the code to third parties as discussed above.  If you wish to *
 * specify special license conditions of your contributions, just say so   *
 * when you send them.                                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *
 * General Public License v2.0 for more details at                         *
 * http://www.gnu.org/licenses/gpl-2.0.html , or in the COPYING file       *
 * included with Nmap.                                                     *
 *                                                                         *
 ***************************************************************************/

#include "nping.h"
#include "ProbeEngine.h"
#include <vector>
#include "nsock.h"
#include "output.h"
#include "NpingOps.h"

extern NpingOps o;
extern ProbeEngine prob;


/* The ProbeEngine class is the one that handles Nping's basic operation mode:
 * send packets and receive responses. There is a global instance of the class
 * because we need to be able to call methods of ProbeEngine from Nsock event
 * handlers. This happens because in C++ one cannot pass method pointers as
 * callback functions.
 *
 * The ProbeEngine class is also used by the EchoClient, which basically
 * hacks it to add TCP READ events into the same Nsock handler. The ProbeEngine
 * sends packets and captures packets from the wire and the EchoClient handles
 * any TCP READ event through its own handler. Such READ event normally
 * corresponds to the reception of an echoed packet (CAPT).
 *
 * The ProbeEngine is run calling the start() method, passing a list of
 * TargetHost and the list of network interfaces that the engine needs to
 * use to reach those targets. However, note that ProbeEngine also extracts
 * information from Nping's global configuration object, "NpingOps o" */
ProbeEngine::ProbeEngine() {
  this->reset();
} /* End of ProbeEngine constructor */


ProbeEngine::~ProbeEngine() {
} /* End of ProbeEngine destructor */


/* Sets every attribute to its default value. */
void ProbeEngine::reset() {
  nping_print(DBG_4,"%s()", __func__);
  this->nsock_init=false;
  memset(&start_time, 0, sizeof(struct timeval));
  this->rawsd4=-1;
  this->rawsd6=-1;
  this->fds=NULL;
  this->max_iods=0;
  this->packetno=0;
} /* End of reset() */


/* Sets up the internal nsock pool and the nsock trace level. Note that
 * this function is meant to be called just once. Subsequent calls will
 * have no effect unless ProbeEngine::reset() has been called previously. */
int ProbeEngine::init_nsock(){
  nping_print(DBG_4,"%s()", __func__);
  struct timeval now;
  if(nsock_init==false){
    /* Create a new nsock pool */
    if ((nsp = nsp_new(NULL)) == NULL)
      nping_fatal(QT_3, "Failed to create new pool.  QUITTING.\n");

    /* Allow broadcast addresses */
    nsp_setbroadcast(nsp, 1);

    /* Set nsock trace level */
    gettimeofday(&now, NULL);
    if( o.getDebugging() == DBG_5)
      nsock_set_loglevel(nsp, NSOCK_LOG_INFO);
    else if( o.getDebugging() > DBG_5 )
      nsock_set_loglevel(nsp, NSOCK_LOG_DBG_ALL);

    /* If users passed an explicit network interface, set it in Nsock so
     * all IODs get bound to the interface. */
    if(o.issetDevice()){
      nsp_setdevice(this->nsp, o.getDevice());
    }

    /* Flag it as already initialized so we don't do it again */
    nsock_init=true;
  }
  return OP_SUCCESS;
} /* End of init() */


/* Cleans up the internal nsock pool and any other internal data that
 * needs to be taken care of before destroying the object. */
int ProbeEngine::cleanup(){
  nping_print(DBG_4,"%s()", __func__);
  nsp_delete(this->nsp);
  return OP_SUCCESS;
} /* End of cleanup() */


/* Returns the internal Nsock pool handler. This method is provided because
 * the EchoClient needs it in order to "hack" the ProbeEngine and inject its
 * own TCP READ events (which correspond to operation of the NEP protocol,
 * performed over the TCP side channel established with the Echo server.
 *
 * @warning the caller must ensure that init_nsock() has been called before
 * calling this method; otherwise, it will fatal() */
nsock_pool ProbeEngine::getNsockPool(){
  nping_print(DBG_4,"%s()", __func__);
  if( this->nsock_init==false)
    nping_fatal(QT_3, "%s() called before init_nsock(). Please report a bug.", __func__);
  return this->nsp;
} /* End of getNsockPool() */


/* This method gets the probe engine ready for packet capture. Basically it
 * obtains a pcap descriptor from Nsock and sets an appropriate BPF filter for
 * EACH supplied interface.
 * @warning Note that the list of interfaces and the list of BPF filters MUST
 *          have the same number of elements. Otherwise, there will be an
 *          assertion failure.
 * @return OP_SUCCESS on success, fatals on error. */
int ProbeEngine::setup_sniffer(vector<NetworkInterface *> &ifacelist, vector<const char *>bpf_filters){
  nping_print(DBG_4,"%s()", __func__);
  int errcode = 0;
  char pcapdev[128];
  nsock_iod my_pcap_iod;
  assert(ifacelist.size()==bpf_filters.size());

  for(u32 i=0; i<ifacelist.size(); i++){

    /* Get a new descriptor from Nsock and associate it with the interface name
     * it belongs to. */
    my_pcap_iod=nsi_new(this->nsp, (void *)ifacelist[i]->getName());

    /* Do some magic to make pcap names work in Windows. Nping may use device
     * names obtained through dnet, but WinPcap has its own naming system, so
     * the conversion is done here*/
    #ifdef WIN32
      if (!DnetName2PcapName(ifacelist[i]->getName(), pcapdev, sizeof(pcapdev))) {
        /* Couldn't find the corresponding dev. We'll try with the one we have */
        Strncpy(pcapdev, ifacelist[i]->getName(), sizeof(pcapdev));
      }
    #else
      Strncpy(pcapdev, ifacelist[i]->getName(), sizeof(pcapdev));
    #endif

    /* Obtain the pcap descriptor */
    if ((errcode=nsock_pcap_open(this->nsp, my_pcap_iod, pcapdev, 8192, o.getSpoofAddress() ? 1 : 0, bpf_filters[i]))!=0)
      nping_fatal(QT_3, "Error opening capture device %s --> Error %d", pcapdev, errcode);

    /* Add the IOD for the current interface to the list of pcap IODs */
    this->pcap_iods.push_back(my_pcap_iod);
  }
  return OP_SUCCESS;
} /* End of setup_sniffer() */


/* This function handles regular ping mode. Basically it handles both
 * unprivileged modes (TCP_CONNECT and UDP_UNPRIV) and raw packet modes
 * (TCP, UDP, ICMP, ARP). This function is where the loops that iterate
 * over target hosts and target ports are located. It uses the nsock lib
 * to keep timing and to handle captured packets or unprivileged READ events.
 *
 * Note that although a complete list of targets and network interfaces is
 * passed, the ProbeEngine still needs to access some additional configuration
 * parameters through the global NpingOps object.  */
int ProbeEngine::start(vector<TargetHost *> &Targets, vector<NetworkInterface *> &Interfaces){
  nping_print(DBG_4,"%s()", __func__);
  const char *filter = NULL;
  vector<const char *>bpf_filters;
  vector<PacketElement *> Packets;
  struct timeval now, now2, next_time;
  int wait_time=0, time_deviation=0;
  u16 total_ports=0;
  u16 *pts=NULL;
  u16 spts_len=0;
  u16 spts_idx=0;
  u16 *spts=o.getSourcePorts(&spts_len);
  u16 curr_spt=0; /* Current source port for unpriv mode. Must be init to zero. */
  u32 count=1;
  int max_rtt=0;


  nping_print(DBG_1, "Starting Nping Probe Engine...");

  /* Initialize Nsock */
  this->init_nsock();
  pts=o.getTargetPorts(&total_ports);
  total_ports = (total_ports==0) ? 1 : total_ports;

  if(o.mode(MODE_IS_PRIVILEGED)){
    /* Build a BPF filter for each interface */
    for(u32 i=0; i<Interfaces.size(); i++){
      filter = this->bpf_filter(Targets, Interfaces[i]);
      assert(filter!=NULL);
      bpf_filters.push_back(strdup(filter));
      nping_print(DBG_2, "[ProbeEngine] Interface=%s BPF:%s", Interfaces[i]->getName(), filter);
    }

    /* Set up the sniffer(s) */
    this->setup_sniffer(Interfaces, bpf_filters);

    /* Schedule the first pcap read event (one for each interface we use) */
    if(!o.disablePacketCapture()){
      for(size_t i=0; i<this->pcap_iods.size(); i++){
        nsock_pcap_read_packet(this->nsp, this->pcap_iods[i], packet_capture_handler_wrapper, -1, NULL);
      }
    }
  }

  /* Init the time counters */
  gettimeofday(&this->start_time, NULL);
  o.stats.start_clocks();

  /* Do the Probe Mode rounds! */
  for(unsigned int r=0; r<o.getRounds(); r++){

    for(unsigned int p=0; p<total_ports; p++){

      /* Use a custom source port if appropriate */
      if(spts!=NULL){
        curr_spt=spts[spts_idx];
        if(spts_idx==(spts_len-1))
          spts_idx=0;
        else
          spts_idx++;
      }

      for(unsigned int t = 0; t < Targets.size(); t++){

        /* Get current timestamp so we can output time elapsed  */
        if(r==0 && p==0 && t==0){
          now=this->start_time;
        }else{
          gettimeofday(&now, NULL);
        }

        /* There are two possibilities.
         *   1: we are in some unprivileged mode in which we have to issue TCP
         *      connects or send UDP datagrams using regular system calls
         *
         *   2: We need to produce and send raw packets.
         */
        if(o.mode(DO_TCP_CONNECT) || o.mode(DO_UDP_UNPRIV)){
          if(o.mode(DO_TCP_CONNECT)){ // TODO: @todo set the target port right!!
            if(pts!=NULL){
              do_tcp_connect(Targets[t], pts[p], curr_spt, &now);
            }else{
              do_tcp_connect(Targets[t], DEFAULT_TCP_TARGET_PORT, curr_spt, &now);
            }
          }
          if(o.mode(DO_UDP_UNPRIV)){
            if(pts!=NULL){
              do_udp_unpriv(Targets[t], pts[p], curr_spt, &now);
            }else{
              do_udp_unpriv(Targets[t], DEFAULT_UDP_TARGET_PORT, curr_spt, &now);
            }
          }
        }
        if(o.mode(DO_TCP) || o.mode(DO_UDP) || o.mode(DO_ICMP) || o.mode(DO_ARP)){
          /* Obtain a list of packets to send (each TargetHost adds whatever
           * packets it wants to send to the supplied vector) */
          Targets[t]->getNextPacketBatch(Packets);

          /* Here, schedule the immediate transmission of all the packets
           * provided by the TargetHosts. */
          nping_print(DBG_2, "Starting transmission of %d packets", (int)Packets.size());
          while(Packets.size()>0){
              this->send_packet(Targets[t], Packets[0], &now);
             /* Delete the packet we've just sent from the list so we don't send
              * it again the next time */
             Packets.erase(Packets.begin(), Packets.begin()+1);
          }
        }

        /* Check if we've just sent the last packet. In that case, stop the
         * Tx clock.*/
        if(r==(o.getRounds()-1) && p==(total_ports-1) && t==(Targets.size()-1))
          o.stats.stop_tx_clock();

        /* Determine how long do we have to wait until we send the next pkt.
         * If we still have more packets to send, we wait until a whole
         * inter-packet delay has elapsed since the last packet we sent. */
        if(!(r==(o.getRounds()-1) && p==(total_ports-1) && t==(Targets.size()-1))){
          TIMEVAL_MSEC_ADD(next_time, start_time, count*o.getDelay() );
          gettimeofday(&now, NULL);
          if((wait_time=TIMEVAL_MSEC_SUBTRACT(next_time, now)-time_deviation) < 0){
            nping_print(DBG_1, "Wait time < 0 ! (wait_time=%d)", wait_time);
            wait_time=0;
          }
        /* If we have sent the last packet already, it doesn't make sense to wait
         * for the same amount of time as before (the inter-packet delay). Imagine
         * we have an interpacket delay of 10s and a max RTT of 0.2s. Why wait 10s
         * when we can reasonably stop capturing packets after 0.2s? So here what we
         * do is to determine the higher RTT that we've observed in the past, and
         * wait for t=4*Max_RTT (We multiply the max RTT observed by 4 so if
         * we have multiple targets, we don't miss a first response sent by
         * a slow target). If we don't have any RTT we wait for a fixed 1 second. */
        }else{
          for(u32 z=0; z<Targets.size(); z++){
            if(Targets[z]->stats.get_max_rtt()>max_rtt)
              max_rtt=Targets[z]->stats.get_max_rtt();
          }
          if(max_rtt==0){
            wait_time=DEFAULT_TIME_WAIT_AFTER_LAST_PACKET;
          }else{
            wait_time=(4*max_rtt)/1000;
          }
          nping_print(DBG_2, "Final wait time for responses: %d msecs.", wait_time);
        }

        /* Now schedule a dummy wait event so we don't send more packets
         * until the inter-packet delay has passed */
        nping_print(DBG_2, "Waiting for %d msecs.", wait_time);
        nsock_timer_create(nsp, interpacket_delay_wait_handler, wait_time, NULL);

        /* Now wait until all events have been dispatched */
        nsock_loop(this->nsp, -1);

        /* Let's see what time it is now so we can determine if we got the
         * wait_time right. If we didn't, we compute the time deviation and
         * apply it in the next iteration. */
        gettimeofday(&now2, NULL);
        if((time_deviation=TIMEVAL_MSEC_SUBTRACT(now2, now) - wait_time)<0){
          time_deviation=0;
        }
        count++;
      }
    }
  }

  /* Cleanup and return */
  o.stats.stop_rx_clock();
  nping_print(DBG_1, "Nping Probe Engine Finished.");
  return OP_SUCCESS;
} /* End of start() */


/* This function creates a BPF filter specification, suitable to be passed to
 * pcap_compile() or nsock_pcap_open(). Note that @param target_interface
 * determines which subset of @param Targets will be considered for the
 * BPF filter. In other words, if we have some targets that use eth0 and a
 * target that uses "lo", then if "target_interface" is "lo", only the right
 * target host will be included in the filter. Same thing for "eth0", etc.
 * If less than 20 targets are associated with the supplied interfacer,
 * the filter contains an explicit list of target addresses. It looks similar
 * to this:
 *
 * dst host fe80::250:56ff:fec0:1 and (src host fe80::20c:29ff:feb0:2316 or src host fe80::20c:29ff:fe9f:5bc2)
 *
 * When more than 20 targets are passed, a generic filter based on the source
 * address is used. The returned filter looks something like:
 *
 * dst host fe80::250:56ff:fec0:1
 *
 * @warning Returned pointer is a statically allocated buffer that subsequent
 *  calls will overwrite. */
char *ProbeEngine::bpf_filter(vector<TargetHost *> &Targets, NetworkInterface *target_interface){
  nping_print(DBG_4,"%s()", __func__);
  static char pcap_filter[2048];
  /* 20 IPv6 addresses is max (46 byte addy + 14 (" or src host ")) * 20 == 1200 */
  char dst_hosts[1220];
  int filterlen=0;
  int len=0;
  bool ipv4_targets=false;
  bool ipv6_targets=false;
  unsigned int targetno;
  memset(pcap_filter, 0, sizeof(pcap_filter));
  IPAddress *src_addr=NULL;
  bool first=true;

  /* If the user specified a custom BPF filter, use it. */
  if(o.issetBPFFilterSpec())
    return o.getBPFFilterSpec();

  /* First of all, check if our targets are all IPv6 or all IPv4. If there is
   * a mixture of both versions, then we need to avoid setting a "dst host ADDR"
   * in the BPF filter, because IPv6 hosts will use different target addresses
   * than IPv4 hosts. */
   for(targetno = 0; targetno < Targets.size(); targetno++){
     /* Only process hosts whose network interface matches target_interface */
     if( strcmp(Targets[targetno]->getInterface()->getName(), target_interface->getName()))
       continue;
     assert(Targets[targetno]->getTargetAddress()!=NULL);
     if(Targets[targetno]->getTargetAddress()->getVersion()==AF_INET)
       ipv4_targets=true;
     else  if(Targets[targetno]->getTargetAddress()->getVersion()==AF_INET6)
       ipv6_targets=true;
     else
       nping_fatal(QT_3, "Wrong address family found in a TargetHost");
     if(ipv4_targets && ipv6_targets)
       break;
   }

  /* If we have 20 or less targets, build a list of addresses so we can set
   * an explicit BPF filter */
  if (target_interface->associatedHosts() <= 20) {
    /* Iterate over all targets so we can build the list of addresses we
     * expect packets from */
    for(targetno = 0; targetno < Targets.size(); targetno++) {
      /* Only process hosts whose network interface matches target_interface */
      if( strcmp(Targets[targetno]->getInterface()->getName(), target_interface->getName()) ){
        continue;
      }else if(first){
        src_addr=Targets[targetno]->getSourceAddress();
      }
      len = Snprintf(dst_hosts + filterlen, sizeof(dst_hosts) - filterlen,
                     "%ssrc host %s", (first)? "" : " or ",
                     Targets[targetno]->getTargetAddress()->toString());
      first=false;
      if (len < 0 || len + filterlen >= (int) sizeof(dst_hosts))
        nping_fatal(QT_3, "ran out of space in dst_hosts");
      filterlen += len;
    }
    /* Now build the actual BPF filter, where we specify the address we expect
     * the packets to be directed to (our address) and the address we expect
     * the packets to come from */
    if (len < 0 || len + filterlen >= (int) sizeof(dst_hosts))
      nping_fatal(QT_3, "ran out of space in dst_hosts");
    /* Omit the destination address part when dealing with IPv4 and IPv6 targets
     * simultaneously. */
    if(ipv4_targets && ipv6_targets){
      if(o.doMulticast())
        len = Snprintf(pcap_filter, sizeof(pcap_filter), "");
      else
        len = Snprintf(pcap_filter, sizeof(pcap_filter), "%s", dst_hosts);
    }else{
      if(o.doMulticast())
        len = Snprintf(pcap_filter, sizeof(pcap_filter), "dst host %s", src_addr->toString());
      else
        len = Snprintf(pcap_filter, sizeof(pcap_filter), "dst host %s and (%s)",
                       src_addr->toString(), dst_hosts);
    }
  /* If we have too many targets to list every single IP address that we
   * plan to send packets too, just set the filter with our own address, so
   * we only capture packets destined to the source address we chose for the
   * packets we sent. */
  }else{
    /* If we are dealing with IPv4 and IPv6 hosts at the same time, just use
     * a NULL filter, so we get all the packets. Nping's matching engine will
     * discard all the rubbish. */
    if(ipv4_targets && ipv6_targets){
      len = Snprintf(pcap_filter, sizeof(pcap_filter), "");
    }else{
     /* Find the first target that uses our interface so we can extract the source
       * IP address */
      for(targetno = 0; targetno < Targets.size(); targetno++) {
        /* Only process hosts whose network interface matches target_interface */
        if( strcmp(Targets[targetno]->getInterface()->getName(), target_interface->getName()) ){
          continue;
        }else{
          src_addr=Targets[targetno]->getSourceAddress();
          break;
        }
      }
      len = Snprintf(pcap_filter, sizeof(pcap_filter), "dst host %s", src_addr->toString());
    }
  }
  /* Make sure we haven't screwed up */
  if (len < 0 || len >= (int) sizeof(pcap_filter))
    nping_fatal(QT_3, "ran out of space in pcap filter");
  return pcap_filter;
} /* End of bpf_filter() */


/* This method sends a packet to the supplied target host. The packet is not
 * supplied in a raw "binary form" but as a chain of PacketElements. If the
 * first element of the chain is an Ethernet header, then the packet will be
 * sent at the raw Ethernet level through the interface associated with the
 * TargetHost. Note that if the packet does NOT start with an Ethernet header,
 * then the network layer MUST be IP (either v4 or v6).
 * The "now" parameter should hold the time that the caller wants to display
 * in Nping's "SENT (X.XXXX)..." output line */
int ProbeEngine::send_packet(TargetHost *tgt, PacketElement *pkt, struct timeval *now){
  nping_print(DBG_4,"%s()", __func__);
  eth_t *ethsd=NULL;       /* DNET Ethernet handler                 */
  struct sockaddr_in s4;   /* Target IPv4 address                   */
  struct sockaddr_in6 s6;  /* Target IPv6 address                   */
  u8 pktbuff[65535];       /* Binary buffer for the outgoing packet */
  PacketElement *tlayer=NULL;
  assert(tgt!=NULL && pkt!=NULL && now!=NULL);
  pkt->dumpToBinaryBuffer(pktbuff, 65535);

  /* Now decide whether the packet should be transmitted at the raw Ethernet
   * level or at the IP level. TargetHosts are already aware of their needs
   * so if the PacketElement that we got starts with an Ethernet frame,
   * that means we have to inject and Ethernet frame. Otherwise we do raw IP. */
  if(pkt->protocol_id()==HEADER_TYPE_ETHERNET){
    /* Determine which interface we should use for the packet */
    NetworkInterface *dev=tgt->getInterface();
    assert(dev!=NULL);

    /* Obtain an Ethernet handler from DNET */
    if((ethsd=eth_open_cached(dev->getName()))==NULL)
      nping_fatal(QT_3, "%s: Failed to open ethernet device (%s)", __func__, dev->getName());

    /* Inject the packet into the wire */
    if(eth_send(ethsd, pktbuff, pkt->getLen()) < pkt->getLen()){
      nping_warning(QT_2, "Failed to send Ethernet frame through %s", dev->getName());
      return OP_FAILURE;
    }
  }else if(pkt->protocol_id()==HEADER_TYPE_IPv4){
    tgt->getTargetAddress()->getIPv4Address(&s4);
    /* First time this is called, we obtain a raw socket for IPv4 */
    if(this->rawsd4<0){
      if((this->rawsd4=socket(AF_INET, SOCK_RAW, IPPROTO_RAW))<0){
        nping_fatal(QT_3, "%s(): Unable to obtain raw socket.", __func__);
      }
    }
    send_ip_packet_sd(this->rawsd4, &s4, pktbuff, pkt->getLen() );
  }else if(pkt->protocol_id()==HEADER_TYPE_IPv6){
    tgt->getTargetAddress()->getIPv6Address(&s6);
    send_ipv6_packet_eth_or_sd(-1, NULL, &s6, pktbuff, pkt->getLen());
  }else{
    nping_fatal(QT_3, "%s(): Unknown protocol", __func__);
  }

  /* Update statistics */
  this->ts_last_sent=*now;
  if((tlayer=PacketParser::find_transport_layer(pkt))!=NULL){
    o.stats.update_sent(tgt->getTargetAddress()->getVersion(), tlayer->protocol_id(), pkt->getLen());
    tgt->stats.update_sent(tgt->getTargetAddress()->getVersion(), tlayer->protocol_id(), pkt->getLen());
  }else{
    nping_warning(QT_2, "%s(): No transport layer found. Please report this bug.", __func__);
  }

  /* Finally, print the packet we've just sent */
  if(o.showSentPackets()){
    PacketElement *pkt2print=pkt;
    nping_print(VB_0|NO_NEWLINE,"SENT (%.4fs) ", ((double)TIMEVAL_MSEC_SUBTRACT(*now, this->start_time)) / 1000);

    /* Skip the Ethernet layer if necessary */
    if(o.showEth()==false && pkt->protocol_id()==HEADER_TYPE_ETHERNET){
      pkt2print=pkt->getNextElement();
    }
    pkt2print->print(stdout, o.getDetailLevel());
    if(o.getVerbosity()>=VB_3){
      int mylen=0;
      u8 *mybuff = pkt2print->getBinaryBuffer(&mylen);
      if(mybuff!=NULL){
        if(mylen>0){
          nping_print(VB_3|NO_NEWLINE,"\n");
          print_hexdump(VB_3 | NO_NEWLINE, mybuff,mylen);
        }
        free(mybuff);
      }
    }else{
      nping_print(VB_0|NO_NEWLINE,"\n");
    }
  }
  return OP_SUCCESS;
} /* End of send_packet() */


/* This function handles TCP and UDP connection attempts. Obviously UDP is
 * a non-connection-oriented protocol but the interface provided by Nsock
 * treats it just like TCP. So basically what we do here is to schedule an
 * immediate TCP/UDP CONNECT event. All other operations (data writes, data
 * reads, etc) are handled by an specialized event handler. */
int ProbeEngine::do_unprivileged(int proto, TargetHost *tgt, u16 tport, u16 sport, struct timeval *now){
  nping_print(DBG_4,"%s()", __func__);
  struct sockaddr_storage ss;      /* Source address */
  struct sockaddr_storage to;      /* Destination address                     */
  size_t sslen=0;                  /* Destination address length              */
  IPAddress *srcaddr=NULL;         /* To store target's source address        */

  /* Initializations */
  memset(&to, 0, sizeof(struct sockaddr_storage));
  memset(&ss, 0, sizeof(struct sockaddr_storage));

  assert(tgt!=NULL);
  tgt->getTargetAddress()->getAddress(&to);

  /* Try to determine the max number of opened descriptors. If the limit is
   * less than than we need, try to increase it. */
  if(fds==NULL){
    max_iods=get_max_open_descriptors()-RESERVED_DESCRIPTORS;
    if( o.getTotalProbes() > max_iods ){
      max_iods=set_max_open_descriptors( o.getTotalProbes() )-RESERVED_DESCRIPTORS;
    }
    /* If we couldn't determine the limit, just use a predefined value */
    if(max_iods<=0)
      max_iods=DEFAULT_MAX_DESCRIPTORS-RESERVED_DESCRIPTORS;
    /* Allocate space for nsock_iods */
    if( (fds=(nsock_iod *)calloc(max_iods, sizeof(nsock_iod)))==NULL ){
      /* If we can't allocate for that many descriptors, reduce our requirements */
      max_iods=DEFAULT_MAX_DESCRIPTORS-RESERVED_DESCRIPTORS;
      if( (fds=(nsock_iod *)calloc(max_iods, sizeof(nsock_iod)))==NULL ){
        nping_fatal(QT_3, "ProbeMode::probe_tcpconnect_event_handler(): Not enough memory");
      }
    }
    nping_print(DBG_7, "%d descriptors needed, %d available", o.getTotalProbes(), max_iods);
  }

  /* Determine the size of the sockaddr */
  if( tgt->getTargetAddress()->getVersion()==AF_INET6 ){
    sslen=sizeof(struct sockaddr_in6);
  }else{
    sslen=sizeof(struct sockaddr_in);
  }
  /* We need to keep many IODs open in parallel but we don't allocate
   * millions, just as many as the OS let us (max number of open files).
   * If we run out of them, we just start overwriting the oldest one.
   * If we don't have a response by that time we probably aren't gonna
   * get any, so it shouldn't be a big problem. */
  if( packetno>(u32)max_iods ){
    nsi_delete(fds[packetno%max_iods], NSOCK_PENDING_SILENT);
  }
  /* Create new IOD for connects */
  if ((fds[packetno%max_iods] = nsi_new(nsp, NULL)) == NULL)
    nping_fatal(QT_3, "%s(): Failed to create new nsock_iod.\n", __func__);

  /* If we have a source port or a source address, bind it to the socket. */
  if(((srcaddr=tgt->getSourceAddress())!=NULL) || sport!=0){
    if(srcaddr!=NULL){
      srcaddr->getAddress(&ss);
    }else{
      setsockaddrfamily(&ss, tgt->getTargetAddress()->getVersion());
      setsockaddrany(&ss);
    }
    if(sport!=0){
      setsockaddrport(&ss, sport);
    }
    nsi_set_localaddr(fds[packetno%max_iods], &ss, sslen);
  }
  if(proto==DO_TCP_CONNECT){
    nsock_connect_tcp(nsp, fds[packetno%max_iods], tcpconnect_handler_wrapper, 100000, tgt, (struct sockaddr *)&to, sslen, tport);
    if( o.showSentPackets() ){
      nping_print(VB_0,"SENT (%.4fs) Starting TCP Handshake > %s:%d", ((double)TIMEVAL_MSEC_SUBTRACT(*now, this->start_time)) / 1000, tgt->getTargetAddress()->toString() , tport);
    }
    /* Update statistics. */
    tgt->stats.update_connects(tgt->getTargetAddress()->getVersion(), HEADER_TYPE_TCP);
    o.stats.update_connects(tgt->getTargetAddress()->getVersion(), HEADER_TYPE_TCP);
  }else if(proto==DO_UDP_UNPRIV){
    nsock_connect_udp(nsp, fds[packetno%max_iods], udpunpriv_handler_wrapper, tgt, (struct sockaddr *)&to, sslen, tport);
  }
  packetno++;
  return OP_SUCCESS;
} /* End of do_unprivileged() */


/* Schedules an immediate TCP CONNECT event. For more information, check
 * do_unprivileged() above. */
int ProbeEngine::do_tcp_connect(TargetHost *tgt, u16 tport, u16 sport, struct timeval *now){
  nping_print(DBG_4,"%s()", __func__);
  return do_unprivileged(DO_TCP_CONNECT, tgt, tport, sport, now);
} /* End of do_tcp_connect() */


/* Schedules an immediate UDP CONNECT event. For more information, check
 * do_unprivileged() above. */
int ProbeEngine::do_udp_unpriv(TargetHost *tgt, u16 tport, u16 sport, struct timeval *now){
  nping_print(DBG_4,"%s()", __func__);
  return do_unprivileged(DO_UDP_UNPRIV, tgt, tport, sport, now);
} /* End of do_udp_unpriv() */


/* This method is the handler for PCAP_READ events. In other words, every time
 * nsock captures a packet from the wire, this method is called. In it, we
 * parse the captured packet and decide if it corresponds to a reply to one of
 * the probes we've already sent. If it does, the contents are printed out and
 * the statistics are updated. */
int ProbeEngine::packet_capture_handler(nsock_pool nsp, nsock_event nse, void *arg){
  nping_print(DBG_4,"%s()", __func__);
  nsock_iod nsi = nse_iod(nse);
  enum nse_status status = nse_status(nse);
  enum nse_type type = nse_type(nse);
  const u8 *rcvd_pkt = NULL;                /* Points to the captured packet */
  size_t rcvd_pkt_len = 0;                  /* Length of the captured packet */
  struct timeval pcaptime;                  /* Time the packet was captured  */
  struct timeval now;
  gettimeofday(&now, NULL);
  PacketElement *pkt=NULL, *tlayer=NULL;

  if (status == NSE_STATUS_SUCCESS) {
    switch(type) {

      case NSE_TYPE_PCAP_READ:

        /* Schedule a new pcap read operation */
        nsock_pcap_read_packet(nsp, nsi, packet_capture_handler_wrapper, -1, NULL);

        /* Get captured packet */
        nse_readpcap(nse, NULL, NULL, &rcvd_pkt, &rcvd_pkt_len, NULL, &pcaptime);

        /* Here, we convert the raw hex buffer into a nice chain of PacketElement
         * objects. */
        if((pkt=PacketParser::split(rcvd_pkt, rcvd_pkt_len, false))!=NULL){
          /* Now let's lee if the captured packet is a response to a probe
           * we've sent before. What we do is iterate over the list of
           * target hosts and ask each of those hosts to check if that's the
           * case. */
          for(size_t i=0; i<o.target_hosts.size(); i++){
            if(o.target_hosts[i]->is_response(pkt, &now)){

              /* It's a response! Let's update the stats and print its contents. */
              /* First, find which transport layer protocol we have received and
               * update stats accordingly. */
              if((tlayer=PacketParser::find_transport_layer(pkt))!=NULL){
                o.stats.update_rcvd(o.target_hosts[i]->getTargetAddress()->getVersion(), tlayer->protocol_id(), rcvd_pkt_len);
                o.target_hosts[i]->stats.update_rcvd(o.target_hosts[i]->getTargetAddress()->getVersion(), tlayer->protocol_id(), rcvd_pkt_len);
              }else{
                nping_warning(QT_2, "%s(): No transport layer found. Please report this bug.", __func__);
              }
              /* Now print the packet. If we are in Echo Client Mode, we delay the
               * output for a bit, so we can receive the CAPT version and print it
               * right after the SENT line. This allows users to easily compare
               * both packets. Otherwise, there would be a RCVD line in the middle
               * that would make comparisons a bit less straightforward. If we
               * are in normal mode, we just call print_rcvd_pkt() and print it
               * right away. */
              double timestamp=(((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000.0);
              if( o.getRole() == ROLE_CLIENT ){
                int delay=(int)MIN(o.getDelay()*0.33, 333);
                /* Here, we schedule a timing event. When the timer goes off,
                 * the handler prints the packet. However, the packet may
                 * get printed earlier than that, as soon as we get a CAPT
                 * (echoed) packet. We pass the handler so we can cancel the
                 * event in that case, so it doesn't get printed twice. */
                nsock_event_id ev_id=nsock_timer_create(nsp, delayed_output_handler_wrapper, delay, NULL);
                o.setDelayedRcvd(pkt, timestamp, ev_id);
                return OP_SUCCESS; /* Return now, so we don't run PacketParser::freePacketChain() */
              }else{
                ProbeEngine::print_rcvd_pkt(pkt, timestamp);
              }
            }
          }
          /* Free the captured packet */
          PacketParser::freePacketChain(pkt);
        }
      break;

      default:
       nping_fatal(QT_3, "Unexpected Nsock event in %s()",__func__);
      break;

    } /* switch(type) */

  } else if (status == NSE_STATUS_EOF) {
    nping_print(DBG_4,"response_reception_handler(): EOF\n");
  } else if (status == NSE_STATUS_ERROR) {
    nping_print(DBG_4, "%s(): %s failed: %s\n", __func__, nse_type2str(type), strerror(socket_errno()));
  } else if (status == NSE_STATUS_TIMEOUT) {
    nping_print(DBG_4, "%s(): %s timeout: %s\n", __func__, nse_type2str(type), strerror(socket_errno()));
  } else if (status == NSE_STATUS_CANCELLED) {
    nping_print(DBG_4, "%s(): %s canceled: %s\n", __func__, nse_type2str(type), strerror(socket_errno()));
  } else if (status == NSE_STATUS_KILL) {
    nping_print(DBG_4, "%s(): %s killed: %s\n", __func__, nse_type2str(type), strerror(socket_errno()));
  } else {
    nping_print(DBG_4, "%s(): Unknown status code %d\n", __func__, status);
  }
  return OP_SUCCESS;
} /* End of packet_capture_handler() */


/* This is the handler for TCP-Connect unprivileged mode events. Initial
 * CONNECT requests are scheduled directly by start(), using the do_tcp_connect()
 * method. Upon reception of a successful CONNECT event, we schedule a READ
 * event so we can receive any data sent by the server after the connection
 * has been established. This happens with many network services like FTP
 * or even our NEP protocol. Also, if the user passed a payload, we schedule
 * a WRITE event and transmit such data. Any data received afterwards is
 * also read because for each READ event that we are delivered, we schedule a
 * new one. */
int ProbeEngine::tcpconnect_handler(nsock_pool nsp, nsock_event nse, void *mydata) {
  nping_print(DBG_4,"%s()", __func__);
  nsock_iod nsi;                   /* Current nsock IO descriptor.            */
  enum nse_status status;          /* Current nsock event status.             */
  enum nse_type type;              /* Current nsock event type.               */
  struct timeval now;              /* Current time                            */
  struct sockaddr_storage to;      /* Stores destination address for Tx.      */
  struct sockaddr_storage peer;    /* Stores source address for Rx.           */
  struct sockaddr_in *peer4=NULL;  /*   |_ Sockaddr for IPv4.                 */
  struct sockaddr_in6 *peer6=NULL; /*   |_ Sockaddr for IPv6.                 */
  int family=0;                    /* Hill hold Rx address family.            */
  char ipstring[128];              /* To print IP Addresses.                  */
  u16 peerport=0;                  /* To hold peer's port number.             */
  TargetHost *tgt=NULL;            /* TargetHost associated with connection.  */
  u8 *recvbuff=NULL;               /* Data reception buffer.                  */
  int recvbytes=0;                 /* Received bytes                          */

  /* Initializations */
  nsi = nse_iod(nse);
  status = nse_status(nse);
  type = nse_type(nse);
  tgt=(TargetHost *) mydata;
  peer4=(struct sockaddr_in *)&peer;
  peer6=(struct sockaddr_in6 *)&peer;
  memset(&to, 0, sizeof(struct sockaddr_storage));
  memset(&peer, 0, sizeof(struct sockaddr_storage));
  gettimeofday(&now, NULL);
  assert(tgt!=NULL);

  nping_print(DBG_4, "%s(): Received callback of type %s with status %s",  __func__, nse_type2str(type), nse_status2str(status));

  if (status == NSE_STATUS_SUCCESS ) {

    /* First of all, ask Nsock to give us some info about the event we've been
     * delivered. We mainly want to know which port was the one the target
     * responded to. We already know which target is it because Nsock passes
     * a pointer to the TargetHost for which the event was scheduled. However,
     * Nsock also provides that so we use it. */
    nsi_getlastcommunicationinfo(nsi, NULL, &family, NULL, (struct sockaddr*)&peer, sizeof(struct sockaddr_storage) );
    if(family==AF_INET6){
      inet_ntop(AF_INET6, &peer6->sin6_addr, ipstring, sizeof(ipstring));
      peerport=ntohs(peer6->sin6_port);
    }else{
      inet_ntop(AF_INET, &peer4->sin_addr, ipstring, sizeof(ipstring));
      peerport=ntohs(peer4->sin_port);
    }

    switch(type) {
      /* TCP Handshake was completed successfully */
      case NSE_TYPE_CONNECT:

        nping_print(VB_0,"RECV (%.4fs) Handshake with %s:%d completed", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000, ipstring, peerport);

        /* Schedule a read operation so we can determine if the target is
         * sending any data back to us. */
        if(!o.disablePacketCapture())
          nsock_read(nsp, nsi, tcpconnect_handler_wrapper, 10000, tgt);

        /* Update statistics. */
        tgt->stats.update_accepts(family, HEADER_TYPE_TCP);
        o.stats.update_accepts(family, HEADER_TYPE_TCP);

        /* If the user specified a payload, inject it into the connection we've
         * just established. */
        if(o.getPayloadBuffer()!=NULL){
          nsock_write(nsp, nsi, tcpconnect_handler_wrapper, 10000, tgt, (const char *)o.getPayloadBuffer(), o.getPayloadLen());
        }
      break;

      case NSE_TYPE_WRITE:
        if(o.showSentPackets()){
          nping_print(VB_0, "DATA (%.4fs) %d byte%s sent to %s:%d", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000, o.getPayloadLen(), o.getPayloadLen()!=1 ? "s" : "", ipstring, peerport);
          if(o.getVerbosity()>=VB_3 && o.getPayloadBuffer()!=NULL)
            print_hexdump(VB_3 | NO_NEWLINE, o.getPayloadBuffer(),o.getPayloadLen());
        }
        /* Update stats for TCP write() operations. */
        tgt->stats.update_writes(family, HEADER_TYPE_TCP, o.getPayloadLen());
        o.stats.update_writes(family, HEADER_TYPE_TCP, o.getPayloadLen());
      break;

      case NSE_TYPE_READ:

        /* We schedule another read operation, just in case the target sends
         * more data later. */
        nsock_read(nsp, nsi, tcpconnect_handler_wrapper, 10000, tgt);

        /* Now let's see how much data it sent. */
        if((recvbuff=(u8 *)nse_readbuf(nse, &recvbytes))!=NULL){
          nping_print(VB_0, "DATA (%.4fs) %d byte%s received from %s:%d", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000, recvbytes, recvbytes!=1 ? "s" : "", ipstring, peerport);
          if(o.getVerbosity()>=VB_3)
           print_hexdump(VB_3 | NO_NEWLINE, recvbuff, recvbytes);

          /* Update statistics for TCP read() operations  */
          tgt->stats.update_reads(family, HEADER_TYPE_TCP, recvbytes);
          o.stats.update_reads(family, HEADER_TYPE_TCP, recvbytes);
        }
      break;

      default:
        nping_fatal(QT_3, "Unexpected Nsock event in %s()",__func__);
      break;
    } /* switch(type) */
  }else if(status == NSE_STATUS_EOF){
    nping_print(DBG_4, "%s(): Unexpected behavior: Got EOF. Please report this bug.\n", __func__);
  }else if(status == NSE_STATUS_ERROR){
    /* In my tests with Nping and Wireshark, I've seen that we get NSE_STATUS_ERROR
     * whenever we start a TCP handshake but our peer sends a TCP RST packet back
     * denying the connection. So in this case, we inform the user (as opposed
     * to saying nothing, that's what we do when we don't get responses, e.g:
     * when trying to connect to filtered ports). This is not 100% accurate
     * because there may be other reasons why ge get NSE_STATUS_ERROR so that's
     * why we say "Possible TCP RST received". */
    if(type==NSE_TYPE_CONNECT){
      nsi_getlastcommunicationinfo(nsi, NULL, &family, NULL, (struct sockaddr*)&peer, sizeof(struct sockaddr_storage) );
      if(family==AF_INET6){
        inet_ntop(AF_INET6, &peer6->sin6_addr, ipstring, sizeof(ipstring));
        peerport=ntohs(peer6->sin6_port);
      }else{
        inet_ntop(AF_INET, &peer4->sin_addr, ipstring, sizeof(ipstring));
        peerport=ntohs(peer4->sin_port);
      }
      nping_print(VB_0,"RECV (%.4fs) Possible TCP RST received from %s:%d --> %s", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000,ipstring, peerport, strerror(nse_errorcode(nse)) );
     }else{
        nping_warning(QT_2,"ERR: (%.4fs) %s to %s:%d failed: %s", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000, nse_type2str(type), ipstring, peerport, strerror(socket_errno()));
     }
  }else if(status == NSE_STATUS_TIMEOUT){
    nping_print(DBG_4, "%s(): %s timeout: %s\n", __func__, nse_type2str(type), strerror(socket_errno()));
  }else if(status == NSE_STATUS_CANCELLED){
    nping_print(DBG_4, "%s(): %s canceled: %s", __func__, nse_type2str(type), strerror(socket_errno()));
  }else if(status == NSE_STATUS_KILL){
    nping_print(DBG_4, "%s(): %s killed: %s", __func__, nse_type2str(type), strerror(socket_errno()));
  }else{
    nping_warning(QT_2, "%s(): Unknown status code %d. Please report this bug.", __func__, status);
  }
 return OP_SUCCESS;
} /* End of tcpconnect_handler() */


/* This function handles nsock events related to UDP-Unprivileged mode.
 * Initial CONNECT requests are scheduled directly by start(), using the
 * do_udp_unpriv() method. So in this method, the handler receives nsock
 * events and takes the appropriate action based on event type.  This is
 * basically what it does for each event:
 *
 * CONNECTS: These events generated by nsock for consistency with the
 * behavior in TCP connects. They are pretty useless. They merely indicate
 * that nsock successfully obtained a UDP socket ready to allow sending
 * packets to the appropriate target. When we get that event we schedule
 * the actual tranmission of a UDP datagram, in other words, a WRITE evnet.
 *
 * WRITES: When we get event WRITE it means that nsock actually managed to
 * get our data sent to the target. In this case, we inform the user that
 * the packet has been sent, and we schedule a READ operation, to see
 * if our peer actually returns any data.
 *
 * READS: When we get this event it means that the other end actually sent
 * some data back to us. What we do is read that data, tell the user that
 * we received some bytes and update statistics.
 *
 */
int ProbeEngine::udpunpriv_handler(nsock_pool nsp, nsock_event nse, void *mydata) {
  nping_print(DBG_4,"%s()", __func__);
  nsock_iod nsi;                   /* Current nsock IO descriptor.            */
  enum nse_status status;          /* Current nsock event status.             */
  enum nse_type type;              /* Current nsock event type.               */
  struct timeval now;              /* Current time                       .    */
  struct sockaddr_storage to;      /* Stores destination address for Tx.      */
  struct sockaddr_storage peer;    /* Stores source address for Rx.           */
  struct sockaddr_in *peer4=NULL;  /*   |_ Sockaddr for IPv4.                 */
  struct sockaddr_in6 *peer6=NULL; /*   |_ Sockaddr for IPv6.                 */
  int family=0;                    /* Will hold Rx address family.            */
  char ipstring[128];              /* To print IP Addresses.                  */
  u16 peerport=0;                  /* To hold peer's port number.             */
  int readbytes=0;                 /* Bytes read in total.                    */
  char *readbuff=NULL;             /* Hill hold read data.                    */
  TargetHost *tgt=NULL;            /* TargetHost associated with connection.  */
  const char *payload=NULL;
  int payload_len=0;

  /* Initializations */
  nsi = nse_iod(nse);
  status = nse_status(nse);
  type = nse_type(nse);
  tgt=(TargetHost *)mydata;
  gettimeofday(&now, NULL);
  peer4=(struct sockaddr_in *)&peer;
  peer6=(struct sockaddr_in6 *)&peer;
  memset(&to, 0, sizeof(struct sockaddr_storage));
  memset(&peer, 0, sizeof(struct sockaddr_storage));
  payload = o.getPayloadBuffer()!=NULL ? (const char *)o.getPayloadBuffer() : (const char *)"" ;
  payload_len = o.getPayloadBuffer()!=NULL ? o.getPayloadLen() : 0 ;

  nping_print(DBG_4, "%s(): Received callback of type %s with status %s", __func__, nse_type2str(type), nse_status2str(status));

  if (status == NSE_STATUS_SUCCESS ) {
    switch(type) {

      /* This is a bit stupid but, for consistency, Nsock creates an event of
       * type NSE_TYPE_CONNECT after a call to nsock_connect_udp() is made.
       * Basically this just means that nsock successfully obtained a UDP socket
       * ready to allow sending packets to the appropriate target. */
      case NSE_TYPE_CONNECT:
        nping_print(DBG_3,"Nsock UDP \"connection\" completed successfully.");
        nsock_write(nsp, nsi, udpunpriv_handler_wrapper,100000, tgt, payload, payload_len);
      break;

      /* We get this event as a result of the nsock_write() call performed by
       * the code in charge of dealing with the timer event. When we get this
       * even it means that nsock successfully wrote data to the UDP socket so
       * here we basically just print that we did send some data and we schedule
       * a read operation. */
      case NSE_TYPE_WRITE:
        /* Determine which target are we dealing with */
        nsi_getlastcommunicationinfo(nsi, NULL, &family, NULL, (struct sockaddr*)&peer, sizeof(struct sockaddr_storage) );
        if(family==AF_INET6){
          inet_ntop(AF_INET6, &peer6->sin6_addr, ipstring, sizeof(ipstring));
          peerport=ntohs(peer6->sin6_port);
        }else{
          inet_ntop(AF_INET, &peer4->sin_addr, ipstring, sizeof(ipstring));
          peerport=ntohs(peer4->sin_port);
        }
        if(o.showSentPackets()){
          nping_print(VB_0,"SENT (%.4fs) UDP packet with %d bytes to %s:%d", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000, payload_len, ipstring, peerport );
          if(o.getVerbosity()>=VB_3 && o.getPayloadBuffer()!=NULL && o.getPayloadLen()>0)
            print_hexdump(VB_3 | NO_NEWLINE, o.getPayloadBuffer(), o.getPayloadLen());
        }
        /* Update statistics */
        tgt->stats.update_writes(family, HEADER_TYPE_UDP, payload_len);
        o.stats.update_writes(family, HEADER_TYPE_UDP, payload_len);
        /* If user did not disable packet capture, schedule a read operation */
        if(!o.disablePacketCapture())
          nsock_read(nsp, nsi, udpunpriv_handler_wrapper, DEFAULT_UDP_READ_TIMEOUT_MS, tgt);
      break;

      /* We get this event when we've written some data to a UDP socket and
       * the other end has sent some data back. In this case we read the data and
       * inform the user of how many bytes we got. */
      case NSE_TYPE_READ:
        /* Do an actual read() of the recv data */
        readbuff=nse_readbuf(nse, &readbytes);
        /* Determine which target are we dealing with */
        nsi_getlastcommunicationinfo(nsi, NULL, &family, NULL, (struct sockaddr*)&peer, sizeof(struct sockaddr_storage) );
        if(family==AF_INET6){
          inet_ntop(AF_INET6, &peer6->sin6_addr, ipstring, sizeof(ipstring));
          peerport=ntohs(peer6->sin6_port);
        }else{
          inet_ntop(AF_INET, &peer4->sin_addr, ipstring, sizeof(ipstring));
          peerport=ntohs(peer4->sin_port);
        }
        nping_print(VB_0,"RECV (%.4fs) UDP packet with %d bytes from %s:%d", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000,  readbytes, ipstring, peerport );
        if(o.getVerbosity()>=VB_3 && readbuff!=NULL && readbytes>0)
          print_hexdump(VB_3 | NO_NEWLINE, (const u8 *)readbuff, readbytes);
        /* Update statistics */
        tgt->stats.update_reads(family, HEADER_TYPE_UDP, readbytes);
        o.stats.update_reads(family, HEADER_TYPE_UDP, readbytes);
        /* Even when we have received some data, we schedule another read
         * operation so we can get more if the target still transmits stuff */
        if(!o.disablePacketCapture())
          nsock_read(nsp, nsi, udpunpriv_handler_wrapper, DEFAULT_UDP_READ_TIMEOUT_MS, tgt);
      break;

      default:
        nping_fatal(QT_3, "Unexpected Nsock event in %s()",__func__);
      break;

    } /* switch(type) */

 }else if(status == NSE_STATUS_EOF){
    nping_print(DBG_4, "%s(): Unexpected behavior: Got EOF. Please report this bug.\n", __func__);
 }else if (status == NSE_STATUS_ERROR) {
   nsi_getlastcommunicationinfo(nsi, NULL, &family, NULL, (struct sockaddr*)&peer, sizeof(struct sockaddr_storage) );
   if(family==AF_INET6){
     inet_ntop(AF_INET6, &peer6->sin6_addr, ipstring, sizeof(ipstring));
     peerport=ntohs(peer6->sin6_port);
   }else{
     inet_ntop(AF_INET, &peer4->sin_addr, ipstring, sizeof(ipstring));
     peerport=ntohs(peer4->sin_port);
   }
   nping_warning(QT_2,"ERR: (%.4fs) %s to %s:%d failed: %s", ((double)TIMEVAL_MSEC_SUBTRACT(now, this->start_time)) / 1000, nse_type2str(type), ipstring, peerport, strerror(socket_errno()));
 }else if (status == NSE_STATUS_TIMEOUT) {
   nping_print(DBG_4, "%s(): %s timeout: %s\n", __func__, nse_type2str(type), strerror(socket_errno()));
 }else if (status == NSE_STATUS_CANCELLED) {
   nping_print(DBG_4, "%s(): %s canceled: %s", __func__, nse_type2str(type), strerror(socket_errno()));
 }else if (status == NSE_STATUS_KILL) {
   nping_print(DBG_4, "%s(): %s killed: %s", __func__, nse_type2str(type), strerror(socket_errno()));
 }else{
   nping_warning(QT_2, "%s(): Unknown status code %d. Please report this bug.", __func__, status);
 }
 return OP_SUCCESS;
} /* End of udpunpriv_handler() */


/** Prints a delayed RCVD packet when the nsock timer event goes off. This is used
  * by the echo client to delay output of received packets for a bit, so we
  * print the echoed packet (CAPT line) before the RCVD one, so it can be easily
  * compared with the SENT packet. */
int ProbeEngine::delayed_output_handler(nsock_pool nsp, nsock_event nse, void *mydata){
  PacketElement *pkt=NULL;
  double ts=0;
  nsock_event_id ev_id;
  if((pkt=o.getDelayedRcvd(&ts, &ev_id))!=NULL){
    ProbeEngine::print_rcvd_pkt(pkt, ts);
    PacketParser::freePacketChain(pkt);
  }
  return OP_SUCCESS;
} /* End of delayed_output_handler() */


/** Prints RCVD packets. The result is a line like the following:
  * RCVD (2.0000s) IPv4[127.0.0.1 > 127.0.0.1 ver=4 ihl=5 tos=0x00 iplen=28...
  * The supplied packet is not freed(). The caller is responsible for that. */
int ProbeEngine::print_rcvd_pkt(PacketElement *pkt, double timestamp){
  PacketElement *pkt2print=pkt;
  nping_print(VB_0|NO_NEWLINE,"RCVD (%.4lfs) ", timestamp);
  /* Skip the Ethernet layer if necessary */
  if(o.showEth()==false && pkt->protocol_id()==HEADER_TYPE_ETHERNET){
    pkt2print=pkt->getNextElement();
  }
  if(o.getVerbosity()>=VB_0){
    pkt2print->print(stdout, o.getDetailLevel());
  }
  if(o.getVerbosity()>=VB_3){
    int mylen=0;
    u8 *mybuff = pkt2print->getBinaryBuffer(&mylen);
    if(mybuff!=NULL){
      if(mylen>0){
        nping_print(VB_3|NO_NEWLINE,"\n");
        print_hexdump(VB_3 | NO_NEWLINE, mybuff,mylen);
      }
      free(mybuff);
    }
  }else{
    nping_print(VB_0|NO_NEWLINE,"\n");
  }
  return OP_SUCCESS;
} /* End of print_rcvd_pkt() */


/******************************************************************************
 * Nsock handlers and handler wrappers.                                       *
 ******************************************************************************/

/* This handler is a dummy handler used to keep the interpacket delay between
 * packet schedule operations. When this handler is called by nsock, it means
 * it's time for another round of packets. We just call nsock_loop_quit() so
 * packet capture events don't make us miss the next round of probe
 * transmissions */
void interpacket_delay_wait_handler(nsock_pool nsp, nsock_event nse, void *arg){
  nping_print(DBG_4, "%s()", __func__);
  nsock_loop_quit(nsp);
  return;
} /* End of interpacket_delay_wait_handler() */


/* This handler is a wrapper for the ProbeEngine::packet_capture_handler()
 * method. We need this because C++ does not allow to use class methods as
 * callback functions for things like signal() or the Nsock lib. */
void packet_capture_handler_wrapper(nsock_pool nsp, nsock_event nse, void *arg){
  nping_print(DBG_4, "%s()", __func__);
  prob.packet_capture_handler(nsp, nse, arg);
  return;
} /* End of packet_capture_handler_wrapper() */


/* This handler is a wrapper for the ProbeEngine::tcpconnect_handler()
 * method. We need this because C++ does not allow to use class methods as
 * callback functions for things like signal() or the Nsock lib. */
void tcpconnect_handler_wrapper(nsock_pool nsp, nsock_event nse, void *arg){
  nping_print(DBG_4, "%s()", __func__);
  prob.tcpconnect_handler(nsp, nse, arg);
  return;
} /* End of tcpconnect_handler_wrapper() */


/* This handler is a wrapper for the ProbeEngine::udpunpriv_handler()
 * method. We need this because C++ does not allow to use class methods as
 * callback functions for things like signal() or the Nsock lib. */
void udpunpriv_handler_wrapper(nsock_pool nsp, nsock_event nse, void *arg){
  nping_print(DBG_4, "%s()", __func__);
  prob.udpunpriv_handler(nsp, nse, arg);
  return;
} /* End of udpunpriv_handler_wrapper() */


/* This handler is a wrapper for the ProbeEngine::delayed_output_handler()
 * method. We need this because C++ does not allow to use class methods as
 * callback functions for things like signal() or the Nsock lib. */
void delayed_output_handler_wrapper(nsock_pool nsp, nsock_event nse, void *arg){
  nping_print(DBG_4, "%s()", __func__);
  ProbeEngine::delayed_output_handler(nsp, nse, arg);
  return;
} /* End of delayed_output_handler_wrapper() */
