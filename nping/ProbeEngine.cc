
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


ProbeEngine::ProbeEngine() {
  this->reset();
} /* End of ProbeEngine constructor */


ProbeEngine::~ProbeEngine() {
} /* End of ProbeEngine destructor */


/** Sets every attribute to its default value- */
void ProbeEngine::reset() {
  this->nsock_init=false;
} /* End of reset() */


/** Sets up the internal nsock pool and the nsock trace level */
int ProbeEngine::init_nsock(){
  struct timeval now;
  if( nsock_init==false ){
      /* Create a new nsock pool */
      if ((nsp = nsp_new(NULL)) == NULL)
        nping_fatal(QT_3, "Failed to create new pool.  QUITTING.\n");

      /* Allow broadcast addresses */
      nsp_setbroadcast(nsp, 1);

      /* Set nsock trace level */
      gettimeofday(&now, NULL);
      if( o.getDebugging() == DBG_5)
        nsp_settrace(nsp, NULL, 1 , &now);
      else if( o.getDebugging() > DBG_5 )
        nsp_settrace(nsp, NULL, 10 , &now);
      /* Flag it as already initialized so we don't do it again */
      nsock_init=true;
  }
  return OP_SUCCESS;
} /* End of init() */


/** Cleans up the internal nsock pool and any other internal data that
  * needs to be taken care of before destroying the object. */
int ProbeEngine::cleanup(){
  nsp_delete(this->nsp);
  return OP_SUCCESS;
} /* End of cleanup() */


/** Returns the internal nsock pool.
  * @warning the caller must ensure that init_nsock() has been called before
  * calling this method; otherwise, it will fatal() */
nsock_pool ProbeEngine::getNsockPool(){
  if( this->nsock_init==false)
    nping_fatal(QT_3, "getNsockPool() called before init_nsock(). Please report a bug.");
  return this->nsp;
} /* End of getNsockPool() */



/* This method gets the probe engine ready for packet capture. Basically it
 * obtains a pcap descriptor from nsock and sets an appropriate BPF filter. */
int ProbeEngine::setup_sniffer(vector<NetworkInterface *> &ifacelist, vector<const char *>bpf_filters){
  char *errmsg = NULL;
  char pcapdev[128];
  nsock_iod my_pcap_iod;

  assert(ifacelist.size()==bpf_filters.size());

  for(u32 i; i<ifacelist.size(); i++){

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
    if ((errmsg = nsock_pcap_open(this->nsp, my_pcap_iod, pcapdev, 8192, o.getSpoofAddress() ? 1 : 0, bpf_filters[i])) != NULL)
      nping_fatal(QT_3, "Error opening capture device %s --> %s\n", pcapdev, errmsg);

    /* Add the IOD for the current interface to the list of pcap IODs */
    this->pcap_iods.push_back(my_pcap_iod);
  }
  return OP_SUCCESS;
} /* End of setup_sniffer() */


/** This function handles regular ping mode. Basically it handles both
  * unprivileged modes (TCP_CONNECT and UDP_UNPRIV) and raw packet modes
  * (TCP, UDP, ICMP, ARP). This function is where the loops that iterate
  * over target hosts and target ports are located. It uses the nsock lib
  * to schedule transmissions. The actual Tx and Rx is done inside the nsock
  * event handlers, here we just schedule them, take care of the timers,
  * set up pcap and the bpf filter, etc. */
int ProbeEngine::start(vector<TargetHost *> &Targets, vector<NetworkInterface *> &Interfaces){

  bool probemode_done = false;
  const char *filter = NULL;
  struct timeval begin_time;
  vector<const char *>bpf_filters;

  nping_print(DBG_1, "Starting Nping Probe Engine...");

  /* Initialize variables, timers, etc. */
  gettimeofday(&begin_time, NULL);
  this->init_nsock();

  /* Build a BPF filter for each interface */
  for(u32 i=0; i<Interfaces.size(); i++){
    filter = this->bpf_filter(Targets, Interfaces[i]);
    assert(filter!=NULL);
    bpf_filters.push_back(strdup(filter));
    nping_print(DBG_2, "[ProbeEngine] Interface=%s BPF:%s", Interfaces[i]->getName(), filter);
  }

  /* Set up the sniffer(s) */
  this->setup_sniffer(Interfaces, bpf_filters);

  /* Do the Probe Mode rounds */
  while (!probemode_done) {
    probemode_done = true; /* It will remain true only when all hosts are .done() */

    /* Go through the list of hosts and ask them to schedule their probes */
    for (unsigned int i = 0; i < Targets.size(); i++) {

      /* If the host is not done yet, call schedule() to let it schedule
       * new probes. */
      if (!Targets[i]->done()) {
        probemode_done = false;
        Targets[i]->schedule();
        nping_print(DBG_2, "[ProbeEngine] Host #%u not done", i);
      }
    }
  }

  /* Cleanup and return */
  nping_print(DBG_1, "Nping Probe Engine Finished.");
  return OP_SUCCESS;

} /* End of start() */

/** This function creates a BPF filter specification, suitable to be passed to
  * pcap_compile() or nsock_pcap_open(). It reads info from "NpingOps o" and
  * creates the right BPF filter for the current operation mode. However, if
  * user has supplied a custom BPF filter through option --bpf-filter, the
  * same string stored in o.getBPFFilterSpec() is returned (so the caller
  * should not even bother to check o.issetBPFFilterSpec() because that check
  * is done here already.
  * @warning Returned pointer is a statically allocated buffer that subsequent
  *  calls will overwrite. */
char *ProbeEngine::bpf_filter(vector<TargetHost *> &Targets, NetworkInterface *target_interface){
  static char filterstring[2048];
  return filterstring;
} /* End of getBPFFilterString() */
