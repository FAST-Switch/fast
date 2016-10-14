## Generated SDC file "nmp_cb08.out.sdc"

## Copyright (C) 1991-2014 Altera Corporation. All rights reserved.
## Your use of Altera Corporation's design tools, logic functions 
## and other software and tools, and its AMPP partner logic 
## functions, and any output files from any of the foregoing 
## (including device programming or simulation files), and any 
## associated documentation or information are expressly subject 
## to the terms and conditions of the Altera Program License 
## Subscription Agreement, the Altera Quartus II License Agreement,
## the Altera MegaCore Function License Agreement, or other 
## applicable license agreement, including, without limitation, 
## that your use is for the sole purpose of programming logic 
## devices manufactured by Altera and sold by Altera or its 
## authorized distributors.  Please refer to the applicable 
## agreement for further details.


## VENDOR  "Altera"
## PROGRAM "Quartus II"
## VERSION "Version 14.0.0 Build 200 06/17/2014 SJ Full Version"

## DATE    "Wed Jun 03 11:28:06 2015"

##
## DEVICE  "5SGXMA3K2F40C3"
##


#**************************************************************
# Time Information
#**************************************************************

set_time_format -unit ns -decimal_places 3



#**************************************************************
# Create Clock
#**************************************************************
create_clock -name {pcie_l_refclk} -period 10.000 -waveform { 0.000 5.000 } [get_ports {pcie_l_refclk}]
create_clock -name {FPGA_SYS_CLK} -period 8.000 -waveform { 0.000 4.000 } [get_ports {FPGA_SYS_CLK}]
create_clock -name {R_SGMII_REFCLK} -period 8.000 -waveform { 0.000 4.000 } [get_ports {R_SGMII_REFCLK}]

#**************************************************************
# Create Generated Clock
#**************************************************************

#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************
derive_pll_clocks 
derive_clock_uncertainty
#**************************************************************
# Set Input Delay
#**************************************************************



#**************************************************************
# Set Output Delay
#**************************************************************



#**************************************************************
# Set Clock Groups
#**************************************************************
set_clock_groups -asynchronous -group [get_clocks {NPE_PCIE|PCIEX8|pciex8_inst|altpcie_hip_256_pipen1b|stratixv_hssi_gen3_pcie_hip|coreclkout}] \
-group [get_clocks {CLK_MANAGE|card1_pll|altpll_component|auto_generated|generic_pll1~PLL_OUTPUT_COUNTER|divclk}] \
-group [get_clocks {CLK_MANAGE|card1_pll|altpll_component|auto_generated|generic_pll4~PLL_OUTPUT_COUNTER|divclk}]

#**************************************************************
# Set False Path
#**************************************************************

#**************************************************************
# Set Multicycle Path
#**************************************************************


#**************************************************************
# Set Maximum Delay
#**************************************************************

#**************************************************************
# Set Minimum Delay
#**************************************************************

#**************************************************************
# Set Input Transition
#**************************************************************

