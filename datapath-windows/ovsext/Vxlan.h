/*
 * Copyright (c) 2014 VMware, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __VXLAN_H_
#define __VXLAN_H_ 1

#include "NetProto.h"
typedef struct _OVS_VXLAN_VPORT {
    UINT32 dstPort;
    UINT64 inPkts;
    UINT64 outPkts;
    UINT64 slowInPkts;
    UINT64 slowOutPkts;
    /*
     * To be filled
     */
} OVS_VXLAN_VPORT, *POVS_VXLAN_VPORT;

/* VXLAN header. */
typedef struct VXLANHdr {
    /* Flags. */
    UINT32   flags1:2;
    /* Packet needs replication to multicast group (used for multicast proxy). */
    UINT32   locallyReplicate:1;
    /* Instance ID flag, must be set to 1. */
    UINT32   instanceID:1;
    /* Flags. */
    UINT32   flags2:4;
    /* Reserved. */
    UINT32  reserved1:24;
    /* VXLAN ID. */
    UINT32  vxlanID:24;
    /* Reserved. */
    UINT32   reserved2:8;
} VXLANHdr;

NTSTATUS OvsInitVxlanTunnel(POVS_VPORT_ENTRY vport,
                            POVS_VPORT_ADD_REQUEST addReq);

VOID OvsCleanupVxlanTunnel(POVS_VPORT_ENTRY vport);

NDIS_STATUS OvsSlowPathDecapVxlan(const PNET_BUFFER_LIST packet,
                                  OvsIPv4TunnelKey *tunnelKey);

NDIS_STATUS OvsEncapVxlan(PNET_BUFFER_LIST curNbl,
                          OvsIPv4TunnelKey *tunKey,
                          POVS_SWITCH_CONTEXT switchContext,
                          VOID *completionList,
                          POVS_PACKET_HDR_INFO layers,
                          PNET_BUFFER_LIST *newNbl);

NDIS_STATUS OvsDoDecapVxlan(POVS_SWITCH_CONTEXT switchContext,
                            PNET_BUFFER_LIST curNbl,
                            OvsIPv4TunnelKey *tunKey,
                            PNET_BUFFER_LIST *newNbl);

static __inline UINT32
OvsGetVxlanTunHdrSize(VOID)
{
    /* XXX: Can L2 include VLAN at all? */
    return sizeof (EthHdr) + sizeof (IPHdr) + sizeof (UDPHdr) +
           sizeof (VXLANHdr);
}

#define VXLAN_UDP_PORT 4789
#define VXLAN_UDP_PORT_NBO 0xB512

#endif /* __VXLAN_H_ */
