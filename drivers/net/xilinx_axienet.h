/*
 * Header for Xilinx AxiEthernet device driver.
 *
 * Copyright (c) 2009 Secret Lab Technologies, Ltd.
 * Copyright (c) 2010 Xilinx, Inc. All rights reserved.
 */

#ifndef XILINX_AXIENET_H
#define XILINX_AXIENET_H

#include <linux/netdevice.h>
#include <linux/spinlock.h>


/* packet size info */
#define XAE_HDR_SIZE			14 /* size of Ethernet header */
#define XAE_HDR_VLAN_SIZE		18	/* Size of an Ethernet header
						 * with VLAN
						 */
#define XAE_TRL_SIZE			4 /* size of Ethernet trailer (FCS) */
#define XAE_MTU				1500	/* Max MTU size of an Ethernet
						 * frame
						 */
#define XAE_JUMBO_MTU			9000	/* Max MTU size of a jumbo
						 * Ethernet frame
						 */
#define XAE_MAX_FRAME_SIZE	 (XAE_MTU + XAE_HDR_SIZE + XAE_TRL_SIZE)
#define XAE_MAX_VLAN_FRAME_SIZE  (XAE_MTU + XAE_HDR_VLAN_SIZE + XAE_TRL_SIZE)
#define XAE_MAX_JUMBO_FRAME_SIZE (XAE_JUMBO_MTU + XAE_HDR_SIZE + XAE_TRL_SIZE)

/*  Configuration options */

/*  Accept all incoming packets.
 *  This option defaults to disabled (cleared) */
#define XAE_OPTION_PROMISC			(1 << 0)

/*  Jumbo frame support for Tx & Rx.
 *  This option defaults to disabled (cleared) */
#define XAE_OPTION_JUMBO			(1 << 1)

/*  VLAN Rx & Tx frame support.
 *  This option defaults to disabled (cleared) */
#define XAE_OPTION_VLAN				(1 << 2)

/*  Enable recognition of flow control frames on Rx
 *  This option defaults to enabled (set) */
#define XAE_OPTION_FLOW_CONTROL			(1 << 4)

/*  Strip FCS and PAD from incoming frames.
 *  Note: PAD from VLAN frames is not stripped.
 *  This option defaults to disabled (set) */
#define XAE_OPTION_FCS_STRIP			(1 << 5)

/*  Generate FCS field and add PAD automatically for outgoing frames.
 *  This option defaults to enabled (set) */
#define XAE_OPTION_FCS_INSERT			(1 << 6)

/*  Enable Length/Type error checking for incoming frames. When this option is
 *  set, the MAC will filter frames that have a mismatched type/length field
 *  and if XAE_OPTION_REPORT_RXERR is set, the user is notified when these
 *  types of frames are encountered. When this option is cleared, the MAC will
 *  allow these types of frames to be received.
 *   This option defaults to enabled (set) */
#define XAE_OPTION_LENTYPE_ERR			(1 << 7)

/*  Enable the transmitter.
 *  This option defaults to enabled (set) */
#define XAE_OPTION_TXEN				(1 << 11)

/*  Enable the receiver
*   This option defaults to enabled (set) */
#define XAE_OPTION_RXEN				(1 << 12)

/*  Default options set when device is initialized or reset */
#define XAE_OPTION_DEFAULTS				\
	(XAE_OPTION_TXEN |				\
	 XAE_OPTION_FLOW_CONTROL |			\
	 XAE_OPTION_RXEN)

/* Axi Dma Register definitions */

#define XAXIDMA_TX_CR_OFFSET	0x00000000  /* Channel control */
#define XAXIDMA_TX_SR_OFFSET	0x00000004  /* Status */
#define XAXIDMA_TX_CDESC_OFFSET	0x00000008  /* Current descriptor pointer */
#define XAXIDMA_TX_TDESC_OFFSET	0x00000010  /* Tail descriptor pointer */

#define XAXIDMA_RX_CR_OFFSET	0x00000030	/* Channel control */
#define XAXIDMA_RX_SR_OFFSET	0x00000034	/* Status */
#define XAXIDMA_RX_CDESC_OFFSET	0x00000038	/* Current descriptor pointer */
#define XAXIDMA_RX_TDESC_OFFSET	0x00000040	/* Tail descriptor pointer */


#define XAXIDMA_CR_RUNSTOP_MASK	0x00000001	/* Start/stop DMA channel */
#define XAXIDMA_CR_RESET_MASK	0x00000004	/* Reset DMA engine */


#define XAXIDMA_BD_NDESC_OFFSET		0x00  /* Next descriptor pointer */
#define XAXIDMA_BD_BUFA_OFFSET		0x08  /* Buffer address */
#define XAXIDMA_BD_CTRL_LEN_OFFSET	0x18  /* Control/buffer length */
#define XAXIDMA_BD_STS_OFFSET		0x1C  /* Status */
#define XAXIDMA_BD_USR0_OFFSET		0x20  /* User IP specific word0 */
#define XAXIDMA_BD_USR1_OFFSET		0x24  /* User IP specific word1 */
#define XAXIDMA_BD_USR2_OFFSET		0x28  /* User IP specific word2 */
#define XAXIDMA_BD_USR3_OFFSET		0x2C  /* User IP specific word3 */
#define XAXIDMA_BD_USR4_OFFSET		0x30  /* User IP specific word4 */
#define XAXIDMA_BD_ID_OFFSET		0x34  /* Sw ID */
#define XAXIDMA_BD_HAS_STSCNTRL_OFFSET	0x38  /* Whether has stscntrl strm */
#define XAXIDMA_BD_HAS_DRE_OFFSET	0x3C  /* Whether has DRE */

#define XAXIDMA_BD_HAS_DRE_SHIFT	8	/* Whether has DRE shift */
#define XAXIDMA_BD_HAS_DRE_MASK		0xF00	/* Whether has DRE mask */
#define XAXIDMA_BD_WORDLEN_MASK		0xFF	/* Whether has DRE mask */


#define XAXIDMA_BD_CTRL_LENGTH_MASK	0x007FFFFF	/* Requested len */
#define XAXIDMA_BD_CTRL_TXSOF_MASK	0x08000000	/* First tx packet */
#define XAXIDMA_BD_CTRL_TXEOF_MASK	0x04000000	/* Last tx packet */
#define XAXIDMA_BD_CTRL_ALL_MASK	0x0C000000	/* All control bits */

#define XAXIDMA_DELAY_MASK	0xFF000000	/* Delay timeout counter */
#define XAXIDMA_COALESCE_MASK	0x00FF0000	/* Coalesce counter */

#define XAXIDMA_DELAY_SHIFT	24
#define XAXIDMA_COALESCE_SHIFT	16

#define XAXIDMA_IRQ_IOC_MASK	0x00001000 /* Completion intr */
#define XAXIDMA_IRQ_DELAY_MASK	0x00002000 /* Delay interrupt */
#define XAXIDMA_IRQ_ERROR_MASK	0x00004000 /* Error interrupt */
#define XAXIDMA_IRQ_ALL_MASK	0x00007000 /* All interrupts */

/* Default TX/RX Threshold and waitbound values for SGDMA mode */
#define XAXIDMA_DFT_TX_THRESHOLD	24
#define XAXIDMA_DFT_TX_WAITBOUND	254
#define XAXIDMA_DFT_RX_THRESHOLD	01
#define XAXIDMA_DFT_RX_WAITBOUND	10

#define XAXIDMA_BD_CTRL_LENGTH_MASK	0x007FFFFF	/* Requested len */
#define XAXIDMA_BD_CTRL_TXSOF_MASK	0x08000000	/* First tx packet */
#define XAXIDMA_BD_CTRL_TXEOF_MASK	0x04000000	/* Last tx packet */
#define XAXIDMA_BD_CTRL_ALL_MASK	0x0C000000	/* All control bits */

#define XAXIDMA_BD_STS_ACTUAL_LEN_MASK	0x007FFFFF /* Actual len */
#define XAXIDMA_BD_STS_COMPLETE_MASK	0x80000000 /* Completed */
#define XAXIDMA_BD_STS_DEC_ERR_MASK	0x40000000 /* Decode error */
#define XAXIDMA_BD_STS_SLV_ERR_MASK	0x20000000 /* Slave error */
#define XAXIDMA_BD_STS_INT_ERR_MASK	0x10000000 /* Internal err */
#define XAXIDMA_BD_STS_ALL_ERR_MASK	0x70000000 /* All errors */
#define XAXIDMA_BD_STS_RXSOF_MASK	0x08000000 /* First rx pkt */
#define XAXIDMA_BD_STS_RXEOF_MASK	0x04000000 /* Last rx pkt */
#define XAXIDMA_BD_STS_ALL_MASK		0xFC000000 /* All status bits */


#define XAXIDMA_BD_MINIMUM_ALIGNMENT 0x40

/* Axi Ethernet registers definition */

#define XAE_RAF_OFFSET		0x00000000 /* Reset and Address filter */
#define XAE_TPF_OFFSET		0x00000004 /* Tx Pause Frame */
#define XAE_IFGP_OFFSET		0x00000008 /* Tx Inter-frame gap adjustment*/
#define XAE_IS_OFFSET		0x0000000C /* Interrupt status */
#define XAE_IP_OFFSET		0x00000010 /* Interrupt pending */
#define XAE_IE_OFFSET		0x00000014 /* Interrupt enable */
#define XAE_TTAG_OFFSET		0x00000018 /* Tx VLAN TAG */
#define XAE_RTAG_OFFSET		0x0000001C /* Rx VLAN TAG */
#define XAE_UAWL_OFFSET		0x00000020 /* Unicast address word lower */
#define XAE_UAWU_OFFSET		0x00000024 /* Unicast address word upper */
#define XAE_TPID0_OFFSET	0x00000028 /* VLAN TPID0 register */
#define XAE_TPID1_OFFSET	0x0000002C /* VLAN TPID1 register */
#define XAE_RCW0_OFFSET		0x00000400 /* Rx Configuration Word 0 */
#define XAE_RCW1_OFFSET		0x00000404 /* Rx Configuration Word 1 */
#define XAE_TC_OFFSET		0x00000408 /* Tx Configuration */
#define XAE_FCC_OFFSET		0x0000040C /* Flow Control Configuration */
#define XAE_EMMC_OFFSET		0x00000410 /* EMAC mode configuration */
#define XAE_PHYC_OFFSET		0x00000414 /*RGMII/SGMII configuration */
#define XAE_MDIO_MC_OFFSET	0x00000500 /* MII Management Config */
#define XAE_MDIO_MCR_OFFSET	0x00000504 /* MII Management Control */
#define XAE_MDIO_MWD_OFFSET	0x00000508 /* MII Management Write Data */
#define XAE_MDIO_MRD_OFFSET	0x0000050C /* MII Management Read Data */
#define XAE_MDIO_MIS_OFFSET	0x00000600 /* MII Management Interrupt Status */
#define XAE_MDIO_MIP_OFFSET	0x00000620 /* MII Management Interrupt Pending
					    * register offse
					    */
#define XAE_MDIO_MIE_OFFSET	0x00000640 /* MII Management Interrupt Enable
					    * register offset
					    */
#define XAE_MDIO_MIC_OFFSET	0x00000660 /* MII Management Interrupt Clear
					    * register offset.
					    */
#define XAE_UAW0_OFFSET		0x00000700  /* Unicast address word 0 */
#define XAE_UAW1_OFFSET		0x00000704  /* Unicast address word 1 */
#define XAE_FMI_OFFSET		0x00000708  /* Filter Mask Index */
#define XAE_AF0_OFFSET		0x00000710  /* Address Filter 0 */
#define XAE_AF1_OFFSET		0x00000714  /* Address Filter 1 */

#define XAE_TX_VLAN_DATA_OFFSET 0x00004000 /* TX VLAN data table address */
#define XAE_RX_VLAN_DATA_OFFSET 0x00008000 /* RX VLAN data table address */
#define XAE_MCAST_TABLE_OFFSET	0x00020000 /* Multicast table address */

/* Bit Masks for Axi Ethernet RAF register */
#define XAE_RAF_MCSTREJ_MASK		0x00000002 /* Reject receive
						    * multicast destination
						    * address
						    */
#define XAE_RAF_BCSTREJ_MASK		0x00000004 /* Reject receive
						    * broadcast destination
						    * address
						    */
#define XAE_RAF_TXVTAGMODE_MASK		0x00000018 /* Tx VLAN TAG mode */
#define XAE_RAF_RXVTAGMODE_MASK		0x00000060 /* Rx VLAN TAG mode */
#define XAE_RAF_TXVSTRPMODE_MASK	0x00000180 /* Tx VLAN STRIP mode */
#define XAE_RAF_RXVSTRPMODE_MASK	0x00000600 /* Rx VLAN STRIP mode */
#define XAE_RAF_NEWFNCENBL_MASK		0x00000800 /* New function mode */
#define XAE_RAF_EMULTIFLTRENBL_MASK	0x00001000 /* Exteneded Multicast
						    * Filtering mode
						    */
#define XAE_RAF_STATSRST_MASK		0x00002000	/* Statistics Counter
							 * Reset
							 */
#define XAE_RAF_RXBADFRMEN_MASK		0x00004000 /* Receive Bad Frame
						    * Enable
						    */
#define XAE_RAF_TXVTAGMODE_SHIFT	3	/* Tx Tag mode shift bits */
#define XAE_RAF_RXVTAGMODE_SHIFT	5	/* Rx Tag mode shift bits */
#define XAE_RAF_TXVSTRPMODE_SHIFT	7	/* Tx strip mode shift bits*/
#define XAE_RAF_RXVSTRPMODE_SHIFT	9	/* Rx Strip mode shift bits*/

/* Bit Masks for Axi Ethernet TPF and IFGP registers */
#define XAE_TPF_TPFV_MASK		0x0000FFFF /* Tx pause frame value */
#define XAE_IFGP0_IFGP_MASK		0x0000007F /* Transmit inter-frame
						    * gap adjustment value
						    */

/* Bit Masks for Axi Ethernet IS, IE and IP registers, Same masks apply
 * for all 3 registers. */
#define XAE_INT_HARDACSCMPLT_MASK	0x00000001 /* Hard register access
						    * complete
						    */
#define XAE_INT_AUTONEG_MASK		0x00000002 /* Auto negotiation
						    * complete
						    */
#define XAE_INT_RXCMPIT_MASK		0x00000004 /* Rx complete */
#define XAE_INT_RXRJECT_MASK		0x00000008 /* Rx frame rejected */
#define XAE_INT_RXFIFOOVR_MASK		0x00000010 /* Rx fifo overrun */
#define XAE_INT_TXCMPIT_MASK		0x00000020 /* Tx complete */
#define XAE_INT_RXDCMLOCK_MASK		0x00000040 /* Rx Dcm Lock */
#define XAE_INT_MGTRDY_MASK		0x00000080 /* MGT clock Lock */
#define XAE_INT_PHYRSTCMPLT_MASK	0x00000100 /* Phy Reset complete */
#define XAE_INT_ALL_MASK		0x0000003F /* All the ints */

#define XAE_INT_RECV_ERROR_MASK			\
	(XAE_INT_RXRJECT_MASK | XAE_INT_RXFIFOOVR_MASK) /* INT bits that
							 * indicate receive
							 * errors
							 */

/* Bit masks for Axi Ethernet VLAN TPID Word 0 register */
#define XAE_TPID_0_MASK			0x0000FFFF   /* TPID 0 */
#define XAE_TPID_1_MASK			0xFFFF0000   /* TPID 1 */

/* Bit masks for Axi Ethernet VLAN TPID Word 1 register */
#define XAE_TPID_2_MASK			0x0000FFFF   /* TPID 0 */
#define XAE_TPID_3_MASK			0xFFFF0000   /* TPID 1 */

/* Bit masks for Axi Ethernet RCW1 register */
#define XAE_RCW1_RST_MASK	0x80000000 /* Reset */
#define XAE_RCW1_JUM_MASK	0x40000000 /* Jumbo frame enable */
#define XAE_RCW1_FCS_MASK	0x20000000 /* In-Band FCS enable
					    *(FCS not stripped)
					    */
#define XAE_RCW1_RX_MASK	0x10000000 /* Receiver enable */
#define XAE_RCW1_VLAN_MASK	0x08000000 /* VLAN frame enable */
#define XAE_RCW1_LT_DIS_MASK	0x02000000 /* Length/type field valid check
					    *  disable
					    */
#define XAE_RCW1_CL_DIS_MASK	0x01000000 /* Control frame Length check
					    * disable
					    */
#define XAE_RCW1_PAUSEADDR_MASK 0x0000FFFF /* Pause frame source
					    * address bits [47:32].Bits
					    * [31:0] are stored in register
					    * RCW0
					    */

/* Bit masks for Axi Ethernet TC register */
#define XAE_TC_RST_MASK		0x80000000 /* Reset */
#define XAE_TC_JUM_MASK		0x40000000 /* Jumbo frame enable */
#define XAE_TC_FCS_MASK		0x20000000 /* In-Band FCS enable
					    * (FCS not generated)
					    */
#define XAE_TC_TX_MASK		0x10000000 /* Transmitter enable */
#define XAE_TC_VLAN_MASK	0x08000000 /* VLAN frame enable */
#define XAE_TC_IFG_MASK		0x02000000 /* Inter-frame gap adjustment
					    * enable
					    */

/* Bit masks for Axi Ethernet FCC register */
#define XAE_FCC_FCRX_MASK	0x20000000   /* Rx flow control enable */
#define XAE_FCC_FCTX_MASK	0x40000000   /* Tx flow control enable */

/* Bit masks for Axi Ethernet EMMC register */
#define XAE_EMMC_LINKSPEED_MASK		0xC0000000 /* Link speed */
#define XAE_EMMC_RGMII_MASK		0x20000000 /* RGMII mode enable */
#define XAE_EMMC_SGMII_MASK		0x10000000 /* SGMII mode enable */
#define XAE_EMMC_GPCS_MASK		0x08000000 /* 1000BaseX mode enable*/
#define XAE_EMMC_HOST_MASK		0x04000000 /* Host interface enable*/
#define XAE_EMMC_TX16BIT		0x02000000 /* 16 bit Tx client
						    * enable
						    */
#define XAE_EMMC_RX16BIT		0x01000000 /* 16 bit Rx client enable */
#define XAE_EMMC_LINKSPD_10		0x00000000 /* Link Speed mask for
						    * 10 Mbit
						    */
#define XAE_EMMC_LINKSPD_100		0x40000000 /* Link Speed mask for 100
						    * Mbit
						    */
#define XAE_EMMC_LINKSPD_1000		0x80000000 /* Link Speed mask for
						    * 1000 Mbit
						    */

/* Bit masks for Axi Ethernet PHYC register */
#define XAE_PHYC_SGMIILINKSPEED_MASK	0xC0000000 /* SGMII link speed mask*/
#define XAE_PHYC_RGMIILINKSPEED_MASK	0x0000000C /* RGMII link speed */
#define XAE_PHYC_RGMIIHD_MASK		0x00000002 /* RGMII Half-duplex */
#define XAE_PHYC_RGMIILINK_MASK		0x00000001 /* RGMII link status */
#define XAE_PHYC_RGLINKSPD_10		0x00000000 /* RGMII link 10 Mbit */
#define XAE_PHYC_RGLINKSPD_100		0x00000004 /* RGMII link 100 Mbit */
#define XAE_PHYC_RGLINKSPD_1000		0x00000008 /* RGMII link 1000 Mbit */
#define XAE_PHYC_SGLINKSPD_10		0x00000000 /* SGMII link 10 Mbit */
#define XAE_PHYC_SGLINKSPD_100		0x40000000 /* SGMII link 100 Mbit */
#define XAE_PHYC_SGLINKSPD_1000		0x80000000 /* SGMII link 1000 Mbit */

/* Bit masks for Axi Ethernet MDIO interface MC register */
#define XAE_MDIO_MC_MDIOEN_MASK		0x00000040 /* MII management enable */
#define XAE_MDIO_MC_CLOCK_DIVIDE_MAX	0x3F	   /* Maximum MDIO divisor */

/* Bit masks for Axi Ethernet MDIO interface MCR register */
#define XAE_MDIO_MCR_PHYAD_MASK		0x1F000000 /* Phy Address Mask */
#define XAE_MDIO_MCR_PHYAD_SHIFT	24	   /* Phy Address Shift */
#define XAE_MDIO_MCR_REGAD_MASK		0x001F0000 /* Reg Address Mask */
#define XAE_MDIO_MCR_REGAD_SHIFT	16	   /* Reg Address Shift */
#define XAE_MDIO_MCR_OP_MASK		0x0000C000 /* Operation Code Mask */
#define XAE_MDIO_MCR_OP_SHIFT		13	   /* Operation Code Shift */
#define XAE_MDIO_MCR_OP_READ_MASK	0x00008000 /* Op Code Read Mask */
#define XAE_MDIO_MCR_OP_WRITE_MASK	0x00004000 /* Op Code Write Mask */
#define XAE_MDIO_MCR_INITIATE_MASK	0x00000800 /* Ready Mask */
#define XAE_MDIO_MCR_READY_MASK		0x00000080 /* Ready Mask */

/* Bit masks for Axi Ethernet MDIO interface MIS, MIP, MIE, MIC registers */
#define XAE_MDIO_INT_MIIM_RDY_MASK	0x00000001 /* MIIM Interrupt */

/* Bit masks for Axi Ethernet UAW1 register */
#define XAE_UAW1_UNICASTADDR_MASK	0x0000FFFF/* Station address bits
						   * [47:32]
						   * Station address bits [31:0]
						   * are stored in register
						   * UAW0
						   */

/* Bit masks for Axi Ethernet FMI register */
#define XAE_FMI_PM_MASK			0x80000000	/* Promiscuous mode
							 * enable
							 */
#define XAE_FMI_IND_MASK		0x00000003   /* Index Mask */



#define XAE_MDIO_DIV_DFT		29/* Default MDIO clock divisor */

/* Defines for different options for C_PHY_TYPE parameter in Axi Ethernet IP */
#define XAE_PHY_TYPE_MII		0
#define XAE_PHY_TYPE_GMII		1
#define XAE_PHY_TYPE_RGMII_1_3		2
#define XAE_PHY_TYPE_RGMII_2_0		3
#define XAE_PHY_TYPE_SGMII		4
#define XAE_PHY_TYPE_1000BASE_X		5


#define XAE_MULTICAST_CAM_TABLE_NUM	4 /* Total number of entries in the
					   * hardware multicast table.
					   */

/* Axi Ethernet Synthesis features */
#define XAE_FEATURE_PARTIAL_RX_CSUM	(1 << 0)
#define XAE_FEATURE_PARTIAL_TX_CSUM	(1 << 1)

#define DELAY_OF_ONE_MILLISEC		1000

/**
 * struct axidma_bd - AxiDma buffer descriptor layout
 * @next:	MM2S/S2MM Next Descriptor Pointer
 * @reserved1:	Reserved and not used
 * @phys:	MM2S/S2MM Buffer Address
 * @reserved2:	Reserved and not used
 * @reserved3:	Reserved and not used
 * @reserved4:	Reserved and not used
 * @cntrl:	MM2S/S2MM Control value
 * @status:	MM2S/S2MM Status value
 * @app0:	MM2S/S2MM User Application Field 0.
 * @app1:	MM2S/S2MM User Application Field 1.
 * @app2:	MM2S/S2MM User Application Field 2.
 * @app3:	MM2S/S2MM User Application Field 3.
 * @app4:	MM2S/S2MM User Application Field 4.
 * @sw_id_offset: MM2S/S2MM Sw ID
 * @reserved5:	Reserved and not used
 * @reserved6:	Reserved and not used
 */
struct axidma_bd {
	u32 next;	/* Physical address of next buffer descriptor */
	u32 reserved1;
	u32 phys;
	u32 reserved2;
	u32 reserved3;
	u32 reserved4;
	u32 cntrl;
	u32 status;
	u32 app0;
	u32 app1;	/* TX start << 16 | insert */
	u32 app2;	/* TX csum seed */
	u32 app3;
	u32 app4;
	u32 sw_id_offset;
	u32 reserved5;
	u32 reserved6;
};

/**
 * struct axienet_local - Our private per device data
 * @ndev:	Pointer for net_device to which it will be attached.
 * @dev:	Pointer to device structure
 * @phy_dev:	Pointer to PHY device structure attached to the axienet_local
 * @phy_node:	Pointer to device node structure
 * @mii_bus:	Pointer to MII bus structure
 * @mdio_irqs:	IRQs table for MDIO bus required in mii_bus structure
 * @regs:	Base address for the axienet_local device address space
 * @dma_regs:	Base address for the axidma device address space
 * @dma_err_tasklet: Tasklet structure to process Axi DMA errors
 * @tx_irq:	Axidma TX IRQ number
 * @rx_irq:	Axidma RX IRQ number
 * @temac_type:	axienet type to identify between soft and hard temac
 * @phy_type:	Phy type to identify between MII/GMII/RGMII/SGMII/1000 Base-X
 * @options:	AxiEthernet option word
 * @last_link:	Phy link state in which the PHY was negotiated earlier
 * @features:	Stores the extended features supported by the axienet hw
 * @tx_bd_v:	Virtual address of the TX buffer descriptor ring
 * @tx_bd_p:	Physical address(start address) of the TX buffer descriptor
 *		ring
 * @rx_bd_v:	Virtual address of the RX buffer descriptor ring
 * @rx_bd_p:	Physical address(start address) of the RX buffer descriptor
 *		ring
 * @tx_bd_ci:	Stores the index of the Tx buffer descriptor in the ring being
 *		accessed currently. Used while allocating BDs before a TX
 *		starts.
 * @tx_bd_tail:	Stores the index of the Tx buffer descriptor in the ring being
 *		accessed currently. Used while processing BDs after the TX
 *		completed.
 * @rx_bd_ci:	Stores the index of the Rx buffer descriptor in the ring being
 *		accessed currently.
 * @max_frm_size: Stores the maximum size of the frame that can be
 *		  that Txed/Rxed in the existing hardware. If jumbo
 *		  option is supported, the maximum frame size would
 *		  be 9k. Else it is 1522 bytes (assuming support
 *		  for basic VLAN)
 * @jumbo_support: Stores hardware configuration for jumbo support. If hardware
 *		   can handle jumbo packets, this entry will be 1, else 0.
 */
struct axienet_local {
	struct net_device *ndev;
	struct device *dev;

	/* Connection to PHY device */
	struct phy_device *phy_dev;	/* Pointer to PHY device */
	struct device_node *phy_node;

	/* MDIO bus data */
	struct mii_bus *mii_bus;	/* MII bus reference */
	int mdio_irqs[PHY_MAX_ADDR];	/* IRQs table for MDIO bus */

	/* IO registers, dma functions and IRQs */
	void __iomem *regs;
	void __iomem *dma_regs;

	struct tasklet_struct dma_err_tasklet;

	int tx_irq;
	int rx_irq;
	int temac_type;
	int phy_type;

	u32 options;			/* Current options word */
	int last_link;
	unsigned int features;

	/* Buffer descriptors */
	struct axidma_bd *tx_bd_v;
	dma_addr_t tx_bd_p;
	struct axidma_bd *rx_bd_v;
	dma_addr_t rx_bd_p;
	int tx_bd_ci;
	int tx_bd_tail;
	int rx_bd_ci;

	int max_frm_size;
	int jumbo_support;

};

/**
 * struct axiethernet_option - Used to set axi ethernet hardware options
 * @opt:	Option to be set.
 * @reg:	Register offset to be written for setting the option
 * @m_or:	Mask to be ORed for setting the option in the register
 */
struct axienet_option {
	u32 opt;
	u32 reg;
	u32 m_or;
};

/* Function prototypes visible in xilinx_axienet_main.c for other files. */
u32 axienet_ior(struct axienet_local *lp, int offset);
void axienet_iow(struct axienet_local *lp, int offset, u32 value);

/* Function prototypes visible in xilinx_axienet_mdio.c for other files */
int axienet_mdio_setup(struct axienet_local *lp, struct device_node *np);
void axienet_mdio_teardown(struct axienet_local *lp);

#endif /* XILINX_AXI_ENET_H */