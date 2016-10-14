# (C) 2001-2015 Altera Corporation. All rights reserved.
# Your use of Altera Corporation's design tools, logic functions and other 
# software and tools, and its AMPP partner logic functions, and any output 
# files any of the foregoing (including device programming or simulation 
# files), and any associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License Subscription 
# Agreement, Altera MegaCore Function License Agreement, or other applicable 
# license agreement, including, without limitation, that your use is for the 
# sole purpose of programming logic devices manufactured by Altera and sold by 
# Altera or its authorized distributors.  Please refer to the applicable 
# agreement for further details.


# CORE_PARAMETERS
set IS_SGMII 1
set CONNECT_TO_MAC 1
set IS_INT_FIFO 1
set IS_HD_LOGIC 0
set ENABLE_TIMESTAMPING 0

if { [ expr ($IS_SGMII == 1)] } {
   set_false_path -from [get_registers {*|altera_tse_a_fifo_24:U_DSW|altera_tse_gray_cnt:U_RD|g_out[*]}] -to [get_registers {*|altera_tse_a_fifo_24:U_DSW|rd_g_wptr[*]}]
   set_false_path -from [get_registers {*|altera_tse_a_fifo_24:U_DSW|altera_tse_gray_cnt:U_RD|b_out[*]}] -to [get_registers {*|altera_tse_a_fifo_24:U_DSW|rd_g_wptr[*]}]
   set_false_path -from [get_registers {*|altera_tse_a_fifo_24:U_DSW|altera_tse_gray_cnt:U_WRT|g_out[*]}] -to [get_registers {*|altera_tse_a_fifo_24:U_DSW|wr_g_rptr[*]}]

   if {[expr ($CONNECT_TO_MAC == 1)]} {
      if {[expr ($IS_INT_FIFO == 0)]} {
         if {[expr ($IS_HD_LOGIC == 1)]} {
            set_false_path -from [get_registers {*|altera_tse_top_sgmii*:U_SGMII|altera_tse_colision_detect:U_COL|state*}] -to [get_registers {*|altera_tse_fifoless_mac_tx:U_TX|gm_rx_col_reg*}]
         }
      }
   }
}

if {[expr ($ENABLE_TIMESTAMPING == 1)]} {
   # SDC false path for gray pointers in altera_tse_ph_calculator
   set regs [get_registers -nowarn *altera_tse_ph_calculator*sync_wr_ptr[2]*]
   if {[llength [query_collection -report -all $regs]] > 0} {set_false_path -to $regs}

   set regs [get_registers -nowarn *altera_tse_ph_calculator*sync_rd_ptr[2]*]
   if {[llength [query_collection -report -all $regs]] > 0} {set_false_path -to $regs}
}

# Clock Crosser
set_false_path -from [get_registers *altera_tse_clock_crosser:*|in_data_buffer*] -to [get_registers *altera_tse_clock_crosser:*|out_data_buffer*]

# False path marker used in auto negotiation module
set_false_path -to [get_registers *altera_tse_false_path_marker:*|data_out_reg*]

#**************************************************************
# Set False Path for altera_tse_reset_synchronizer
#**************************************************************
set tse_aclr_counter 0
set tse_clrn_counter 0
set tse_aclr_collection [get_pins -compatibility_mode -nocase -nowarn *|altera_tse_reset_synchronizer:*|altera_tse_reset_synchronizer_chain*|aclr]
set tse_clrn_collection [get_pins -compatibility_mode -nocase -nowarn *|altera_tse_reset_synchronizer:*|altera_tse_reset_synchronizer_chain*|clrn]
foreach_in_collection tse_aclr_pin $tse_aclr_collection {
   set tse_aclr_counter [expr $tse_aclr_counter + 1]
}
foreach_in_collection tse_clrn_pin $tse_clrn_collection {
   set tse_clrn_counter [expr $tse_clrn_counter + 1]
}
if {$tse_aclr_counter > 0} {
   set_false_path -to [get_pins -compatibility_mode -nocase *|altera_tse_reset_synchronizer:*|altera_tse_reset_synchronizer_chain*|aclr]
}

if {$tse_clrn_counter > 0} {
   set_false_path -to [get_pins -compatibility_mode -nocase *|altera_tse_reset_synchronizer:*|altera_tse_reset_synchronizer_chain*|clrn]
}
