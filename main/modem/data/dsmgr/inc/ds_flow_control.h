#ifndef DS_FLOW_CONTROL_H
#define DS_FLOW_CONTROL_H
/*===========================================================================

                   D A T A   T A S K   I N T E R N A L
                        H E A D E R    F I L E

DESCRIPTION

  Data Task regional header file. All definitions relating to flow
  control masks.

Copyright (c) 2002-2008 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE
$PVCSPath: L:/src/asw/MM_DATA/vcs/ds_flow_control.h_v   1.9   12 Feb 2003 11:26:34   ajithp  $
$Header: //source/qcom/qct/modem/data/common/dsmgr/main/lite/inc/ds_flow_control.h#1 $ $DateTime: 2008/04/11 12:46:53 $ $Author: nsivakum $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/28/08    sn     Featurized WCDMA/GSM code.
05/15/03    vas    Added DS_FLOW_HDRAN_RLP_MASK
05/05/03    usb    Added DS_FLOW_DSM_LEVEL_MASKS which constitutes all 
                   memory level masks.
04/18/03    vsk    added DS_FLOW_UMTS_RMSM_MASK instead of DS_FLOW_GEN_RMSM_MASK
02/12/03    atp    Added flow mask for 707 circuit.
12/29/02    jd     Added DS_FLOW_DSSNET_REG_MASK
12/18/02    TMR    Added DS_FLOW_GCSD_MASK and DS_FLOW_WCDMACS_MASK
11/14/02    usb    Added bogus flow mask for multiple mip sessions (future)
11/12/02    usb    Reversed the flow control enable/disable logic and defined 
                   a new overriede mask.
10/11/02    usb    Added flow mask for generic RMSM.
10/03/02    rc     Added flow masks for UMTS.
08/26/02    mvl    Added flow mask for ps_iface, modified M.IP flow masks.
07/19/02    aku    Added flow mask for dssnet.
05/17/02    ak     Added holddown timer mask.
05/15/02    rsl    Initial version of file.
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_DATA
/*===========================================================================
  TYPEDEFS and DEFINITIONS
===========================================================================*/
/*---------------------------------------------------------------------------
  Masks for the function calls that toggle SIO inbound flow control (CTS/RTS)
---------------------------------------------------------------------------*/
#define DS_FLOW_ALL_MASK         0xFFFFFFFF  /* flow enable/disable overide*/

/*---------------------------------------------------------------------------
  Add each DSM level mask to the generic DSM_LEVEL_MASK below.
---------------------------------------------------------------------------*/
#define DS_FLOW_DSM_LARGE_Q_MASK 0x00000001  /* Out of large items mask    */
#define DS_FLOW_DSM_SMALL_Q_MASK 0x00000002  /* Out of small items mask    */

#define DS_FLOW_PKT_WM_MASK      0x00000004  /* pkt mode wm mask           */
#define DS_FLOW_707_CKT_TCP_MASK 0x00000008  /* 707 circuit TCP Sendq mask */
#define DS_FLOW_NETMODEL_MASK    0x00000010  /* mask for netmodel          */
#define DS_FLOW_707_PKT_MASK     0x00000020  /* IS707 pkt controller mask  */
#define DS_HOLDDOWN_TIMER_MASK   0x00000040  /* 707 pkt holddown timer     */
#define DS_FLOW_707_RLP_MASK     0x00000080  /* RLP high wm flow mask      */
#define DS_FLOW_CTRL_DSSNET_MASK 0x00000100  /* dssnet flow mask           */
#define DS_FLOW_PS_IFACE_MASK    0x00000200  /* psiface self mask          */
#ifdef FEATURE_WCDMA
#error code not present
#endif /* FEATURE_WCDMA */
#define DS_FLOW_707_RMSM_MASK    0x00002000  /* 707 Pkt RMSM mask          */
#define DS_FLOW_SNDCP_MASK       0x00004000  /* SNDCP high wm mask         */
#if (defined(FEATURE_GSM) || defined(FEATURE_WCDMA))
#error code not present
#endif /* FEATURE_GSM || FEATURE_WCDMA */
#define DS_FLOW_DSSNET_REG_MASK  0x00020000  /* DSSNET in MIP registration */
#define DS_FLOW_707_CKT_MASK     0x00040000  /* 707 circuit general mask   */
#define DS_FLOW_HDRAN_RLP_MASK   0x00080000  /* HDRAN RLP high wm flow mask*/ 

/*---------------------------------------------------------------------------
  These start from the top and work their way down because (someday) there
  may be more than one registration state machine.
---------------------------------------------------------------------------*/
#define DS_FLOW_MIP_SOL_MASK     0x80000000  /* M.IP solicitation mask     */
#define DS_FLOW_MIP_REG1_MASK    0x40000000  /* 1st M.IP Reg state mask    */
// #def DS_FLOW_MIP_REGx_MASK    0xxx000000  /* next n bits reserved,      */ 
                                             /* n = MIP_MAX_SESSIONS-1     */
/*---------------------------------------------------------------------------
  This represents all the memory level flow masks
---------------------------------------------------------------------------*/
#define DS_FLOW_DSM_LEVEL_MASKS (DS_FLOW_DSM_LARGE_Q_MASK |  \
                                DS_FLOW_DSM_SMALL_Q_MASK)

/*---------------------------------------------------------------------------
  Flow control variable to be passed into ds3g_siolib_set_inbond_flow().
---------------------------------------------------------------------------*/
typedef enum
{
  DS_FLOW_ENABLE  = 0,    /* Use this to enable FLOW */
  DS_FLOW_DISABLE = 1     /* Use this to disable FLOW */
} ds3g_flow_e_type;

#define DS_FLOW_IS_ENABLED        0x00000000    /* All flows enabled flag  */

/*===========================================================================
MACRO         DS_FLOW_CTRL_SET_MASK

DESCRIPTION   Based on whether flow is to be enabled or not, sets
              the value of static mask.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None.
===========================================================================*/
#define DS_FLOW_CTRL_SET_MASK( flow, static_mask, in_mask)      \
  if (flow == DS_FLOW_ENABLE)                                   \
  {                                                             \
    static_mask &= ~(in_mask);                                  \
  }                                                             \
  else                                                          \
  {                                                             \
    static_mask |= (in_mask);                                   \
  }

/*===========================================================================
MACRO         DS_FLOW_CTRL_IS_BIT_SET

DESCRIPTION   Tells if the bit particular bit in the bitmask is set:
                flow_mask - the overall bit mask
                bit       - the bit to check for.  Must be one of the
                            #defines defined above.

DEPENDENCIES  Assumes that the 'bit' input is not DS_FLOW_ALL, but is one of
              the #defines listed above.

RETURN VALUE  TRUE  - that particular bit is set (i.e., that bit is causing
                      flow to be DISABLED.
              FALSE - that bit is not set.

SIDE EFFECTS  None.
===========================================================================*/
#define DS_FLOW_CTRL_IS_BIT_SET(flow_mask, bit)                 \
  ((((flow_mask) & (bit)) == (bit)) ? TRUE : FALSE)

#endif /* FEATURE_DATA      */
#endif /* DS_FLOW_CONTROL_H */

