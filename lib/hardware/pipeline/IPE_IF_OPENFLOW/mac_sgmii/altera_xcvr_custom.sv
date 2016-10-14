// (C) 2001-2015 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.



`timescale 1 ns / 1 ps

import altera_xcvr_functions::*;  // for get_custom_reconfig_width functions

module altera_xcvr_custom #(
  
  // initially found in sv_xcvr_custom_phy
  parameter device_family = "Stratix V",
  parameter protocol_hint = "basic",  // (basic, gige)
  parameter operation_mode = "Duplex",  //legal value: TX,RX,Duplex
  parameter lanes = 4,  //legal value: 1+
  parameter bonded_group_size = 1,  //legal value: integer from 1 .. lanes
  parameter bonded_mode = "xN", // (xN, fb_compensation)
  parameter pma_bonding_mode = "x1", // ("x1", "xN")
  parameter pcs_pma_width = 8, //legal value: 8, 10, 16, 20
  parameter ser_base_factor = 8,  //legal value: 8,10
  parameter ser_words = 1,  //legal value 1,2,4
  parameter data_rate = "1250 Mbps",  //remove this later
  parameter base_data_rate = "0 Mbps", // (PLL data rate)
  
  // tx bitslip
  parameter tx_bitslip_enable = "false",
  
  //optional coreclks
  parameter tx_use_coreclk = "false",
  parameter rx_use_coreclk = "false",
  parameter en_synce_support = 0, //expose CDR ref-clk in this mode
  
  //Phase compensation FIFO
  parameter std_tx_pcfifo_mode = "low_latency",
  parameter std_rx_pcfifo_mode = "low_latency", 
  
  // 8B10B
  parameter use_8b10b = "false",  //legal value: "false", "true"
  parameter use_8b10b_manual_control = "false",
  
  //Word Aligner
  parameter word_aligner_mode = "manual", //legal value: bitslip, sync_state_machine, manual
  parameter word_aligner_state_machine_datacnt = 0, //legal value: 0-256
  parameter word_aligner_state_machine_errcnt = 0,  //legal value: 0-256
  parameter word_aligner_state_machine_patterncnt = 0,  //legal value: 0-256
  parameter word_aligner_pattern_length = 7,
  parameter word_align_pattern = "1111100",
  parameter run_length_violation_checking = 40, //legal value: 0,1+
  
  //RM FIFO
  parameter use_rate_match_fifo = 0,  //legal value: 0,1
  parameter rate_match_pattern1 = "11010000111010000011",
  parameter rate_match_pattern2 = "00101111000101111100",
  
  //Byte Ordering Block
  parameter byte_order_mode = "none", //legal value: None, sync_state_machine, PLD control
  parameter byte_order_pattern = "000000000",
  parameter byte_order_pad_pattern = "111111011",
  
  //Hidden parameter to enable 0ppm legality bypass
  parameter coreclk_0ppm_enable = "false",
  
  //PLL
  parameter pll_refclk_cnt    = 1,          // Number of reference clocks
  parameter pll_refclk_freq   = "125 MHz",  // Frequency of each reference clock
  parameter pll_refclk_select = "0",        // Selects the initial reference clock for each TX PLL
  parameter cdr_refclk_select = 0,          // Selects the initial reference clock for all RX CDR PLLs
  parameter plls              = 1,          // (1+)
  parameter pll_type          = "AUTO",     // PLL type for each PLL
  parameter pll_select        = 0,          // Selects the initial PLL
  parameter pll_reconfig      = 0,          // (0,1) 0-Disable PLL reconfig, 1-Enable PLL reconfig
  parameter pll_external_enable = 0,         // (0,1) 0-Disable external TX PLL, 1-Enable external TX PLL

  // initially found in siv_xcvr_custom_phy
  //Analog Parameters
  parameter gxb_analog_power = "AUTO",  //legal value: AUTO,2.5V,3.0V,3.3V,3.9V
  parameter pll_lock_speed = "AUTO",
  parameter tx_analog_power = "AUTO", //legal value: AUTO,1.4V,1.5V
  parameter tx_slew_rate = "OFF",
  parameter tx_termination = "OCT_100_OHMS",  //legal value: OCT_85_OHMS,OCT_100_OHMS,OCT_120_OHMS,OCT_150_OHMS
  parameter tx_use_external_termination = "false", //legal value: true, false
  parameter tx_preemp_pretap = 0,
  parameter tx_preemp_pretap_inv = "FALSE",
  parameter tx_preemp_tap_1 = 0,
  parameter tx_preemp_tap_2 = 0,
  parameter tx_preemp_tap_2_inv = "FALSE",
  parameter tx_vod_selection = 4,
  parameter tx_common_mode = "0.65V", //legal value: 0.65V
  parameter rx_pll_lock_speed = "AUTO",
  parameter rx_common_mode = "0.82V", //legal value: "0.65V"
  parameter rx_termination = "OCT_100_OHMS",  //legal value: OCT_85_OHMS,OCT_100_OHMS,OCT_120_OHMS,OCT_150_OHMS
  parameter rx_use_external_termination = "false", //legal value: true, false
  parameter rx_eq_dc_gain = 0,
  parameter rx_eq_ctrl = 3,
    
  //Param siv_xcvr_custom_phy.mgmt_clk_in_mhz has default '50', but module-level default is '150'
  parameter mgmt_clk_in_mhz = 150,  //needed for reset controller timed delays
  parameter embedded_reset = 1,  // (0,1) 1-Enable embedded reset controller
  parameter channel_interface = 0, //legal value: (0,1) 1-Enable channel reconfiguration
  parameter starting_channel_number = 0,  //legal value: 0+em
  parameter rx_ppmselect = 32,
  parameter rx_signal_detect_threshold = 2,
  parameter rx_use_cruclk = "FALSE"
) (
  // initially found in sv_xcvr_custom_phy
  input  wire phy_mgmt_clk,
  input  tri0 phy_mgmt_clk_reset,
  input  tri0 phy_mgmt_read,
  input  tri0 phy_mgmt_write,
  input  wire [8:0] phy_mgmt_address,
  input  wire [31:0] phy_mgmt_writedata,
  output wire [31:0] phy_mgmt_readdata,
  output wire phy_mgmt_waitrequest,
  // Reset inputs
  input  wire [plls -1:0] pll_powerdown, 
  input  wire [lanes-1:0] tx_analogreset,
  input  wire [lanes-1:0] tx_digitalreset,
  input  wire [lanes-1:0] rx_analogreset,
  input  wire [lanes-1:0] rx_digitalreset,
  // Calibration busy signals
  output wire [lanes-1:0] tx_cal_busy,
  output wire [lanes-1:0] rx_cal_busy,
  //clk signal
  input  wire [pll_refclk_cnt-1:0] pll_ref_clk,
  input  wire [0:0]                cdr_ref_clk, // used only in SyncE mode 
  input  wire [lanes-1:0] tx_coreclkin,
  input  wire [lanes-1:0] rx_coreclkin,
  input  wire [(plls*lanes)-1:0] ext_pll_clk,      // clkout from external PLL
  output wire [(lanes/bonded_group_size)-1:0] tx_clkout,
  output wire [lanes-1:0] rx_clkout,
  output wire [lanes-1:0] rx_recovered_clk,
  //data ports - Avalon ST interface
  input  wire [lanes-1:0] rx_serial_data,
  output wire [(channel_interface? 64: ser_base_factor*ser_words)*lanes-1:0] rx_parallel_data,
  input  wire [(channel_interface? 44: ser_base_factor*ser_words)*lanes-1:0] tx_parallel_data,
  output wire [lanes-1:0] tx_serial_data,
  //more optional data
  input  wire [lanes*ser_words-1:0] tx_datak,
  output wire [lanes*ser_words-1:0] rx_datak,
  input  wire [lanes*ser_words-1:0] tx_dispval,
  input  wire [lanes*ser_words-1:0] tx_forcedisp,
  output wire [lanes*ser_words-1:0] rx_disperr,
  output wire [lanes*ser_words-1:0] rx_errdetect,
  output wire [lanes*ser_words-1:0] rx_runningdisp,
  output wire [lanes*ser_words-1:0] rx_patterndetect,
  input  wire [lanes-1:0] tx_forceelecidle,
  input  wire [lanes-1:0] rx_enabyteord,
  input  wire [lanes-1:0] rx_bitslip,
  input  wire [lanes*5-1:0] tx_bitslipboundaryselect,
  output wire [lanes*5-1:0] rx_bitslipboundaryselectout,
  output wire [ser_words*lanes-1:0] rx_rmfifodatainserted,
  output wire [ser_words*lanes-1:0] rx_rmfifodatadeleted,
  output wire [lanes-1:0] rx_rlv,

  //PMA block control and status
  output wire [plls-1:0] pll_locked,  // conduit or ST
  output wire [lanes-1:0] rx_is_lockedtoref,  //conduit or ST
  output wire [lanes-1:0] rx_is_lockedtodata, //conduit or ST
  output wire [lanes-1:0] rx_signaldetect,
  //word alignment
  output wire [lanes*ser_words-1:0] rx_syncstatus,  //conduit or ST
  //byte order 
  output wire  [lanes-1:0] rx_byteordflag,
  //reset controller
  output wire tx_ready, //conduit
  output wire rx_ready, //conduit
  //reconfig
  input   wire  [get_custom_reconfig_to_width  ("Stratix V",operation_mode,lanes,plls,bonded_group_size,"",bonded_mode)-1:0] reconfig_to_xcvr,
  output  wire  [get_custom_reconfig_from_width("Stratix V",operation_mode,lanes,plls,bonded_group_size,"",bonded_mode)-1:0] reconfig_from_xcvr 
);

  localparam is_a5 = has_a5_style_hssi(device_family);
  localparam is_c5 = has_c5_style_hssi(device_family);
  localparam is_s5 = has_s5_style_hssi(device_family);
  localparam is_s4 = has_s4_style_hssi(device_family);
  localparam [MAX_CHARS*8-1:0] cur_dev = current_device_family(device_family);

  //Arria V GZ
  localparam is_a5gz = has_s5_style_hssi(device_family);


  // Conditional generation of sub-instances
  generate
  
  // S5 => sv_xcvr_custom_phy
  if ( is_s5 | is_a5gz) begin
    sv_xcvr_custom_nr #(
      .device_family(device_family),
      .protocol_hint(protocol_hint),
      .lanes(lanes),
      .pcs_pma_width(pcs_pma_width),
      .ser_base_factor(ser_base_factor),
      .ser_words(ser_words),
      .mgmt_clk_in_mhz(mgmt_clk_in_mhz),
      .data_rate(data_rate),
      .base_data_rate(base_data_rate),
      .plls(plls),
      .pll_type(pll_type),
      .pll_select(pll_select),
      .pll_reconfig(pll_reconfig),
      .pll_refclk_cnt(pll_refclk_cnt),
      .pll_refclk_freq(pll_refclk_freq),
      .pll_refclk_select(pll_refclk_select),
	  .pll_external_enable(pll_external_enable),
      .cdr_refclk_select(cdr_refclk_select),
      .operation_mode(operation_mode),
      .starting_channel_number(starting_channel_number),
      .bonded_group_size(bonded_group_size),
      .bonded_mode(bonded_mode),
      .embedded_reset(embedded_reset),
      .channel_interface(channel_interface),
	  .std_tx_pcfifo_mode(std_tx_pcfifo_mode),
	  .std_rx_pcfifo_mode(std_rx_pcfifo_mode),
      .use_8b10b(use_8b10b),
      .use_8b10b_manual_control(use_8b10b_manual_control),
      .tx_use_coreclk(tx_use_coreclk),
      .rx_use_coreclk(rx_use_coreclk),
      .en_synce_support(en_synce_support),
      .tx_bitslip_enable(tx_bitslip_enable),
      .word_aligner_mode(word_aligner_mode),
      .word_aligner_state_machine_datacnt(word_aligner_state_machine_datacnt),
      .word_aligner_state_machine_errcnt(word_aligner_state_machine_errcnt),
      .word_aligner_state_machine_patterncnt(word_aligner_state_machine_patterncnt),
      .run_length_violation_checking(run_length_violation_checking),
      .word_align_pattern(word_align_pattern),
      .word_aligner_pattern_length(word_aligner_pattern_length),
      .use_rate_match_fifo(use_rate_match_fifo),
      .rate_match_pattern1(rate_match_pattern1),
      .rate_match_pattern2(rate_match_pattern2),
      .byte_order_mode(byte_order_mode),
      .byte_order_pattern(byte_order_pattern),
      .byte_order_pad_pattern(byte_order_pad_pattern),
      .coreclk_0ppm_enable(coreclk_0ppm_enable)
    ) S5 (
      .mgmt_clk_reset(phy_mgmt_clk_reset),
      .mgmt_clk(phy_mgmt_clk),
      .mgmt_read(phy_mgmt_read),
      .mgmt_write(phy_mgmt_write),
      .mgmt_address(phy_mgmt_address[7:0]),
      .mgmt_writedata(phy_mgmt_writedata),
      .mgmt_readdata(phy_mgmt_readdata),
      .mgmt_waitrequest(phy_mgmt_waitrequest),
      .pll_powerdown(pll_powerdown),
      .tx_analogreset(tx_analogreset),
      .tx_digitalreset(tx_digitalreset),
      .rx_analogreset(rx_analogreset),
      .rx_digitalreset(rx_digitalreset),
      .tx_cal_busy(tx_cal_busy),
      .rx_cal_busy(rx_cal_busy),
      .pll_ref_clk(pll_ref_clk),
      .cdr_ref_clk(cdr_ref_clk), 
      .tx_coreclkin(tx_coreclkin),
      .rx_coreclkin(rx_coreclkin),
	  .ext_pll_clk(ext_pll_clk),
      .tx_clkout(tx_clkout),
      .rx_clkout(rx_clkout),
      .rx_recovered_clk(rx_recovered_clk),
      .rx_serial_data(rx_serial_data),
      .rx_parallel_data(rx_parallel_data),
      .tx_parallel_data(tx_parallel_data),
      .tx_serial_data(tx_serial_data),
      .tx_datak(tx_datak),
      .rx_datak(rx_datak),
      .tx_dispval(tx_dispval),
      .tx_forcedisp(tx_forcedisp),
      .rx_disperr(rx_disperr),
      .rx_a1a2sizeout(/*unused*/),
      .rx_errdetect(rx_errdetect),
      .rx_runningdisp(rx_runningdisp),
      .rx_patterndetect(rx_patterndetect),
      .tx_forceelecidle(tx_forceelecidle),
      .tx_bitslipboundaryselect(tx_bitslipboundaryselect),
      .rx_bitslipboundaryselectout(rx_bitslipboundaryselectout),
      .rx_rmfifodatainserted(rx_rmfifodatainserted),
      .rx_rmfifodatadeleted(rx_rmfifodatadeleted),
      .rx_rlv(rx_rlv),
      
      .rx_enabyteord(rx_enabyteord),
      .rx_bitslip(rx_bitslip),
      .pll_locked(pll_locked),
      .rx_is_lockedtoref(rx_is_lockedtoref),
      .rx_is_lockedtodata(rx_is_lockedtodata),
      .rx_signaldetect(rx_signaldetect),
      .rx_syncstatus(rx_syncstatus),
      .rx_byteordflag(rx_byteordflag),
      .tx_ready(tx_ready),
      .rx_ready(rx_ready),
      .reconfig_to_xcvr(reconfig_to_xcvr),
      .reconfig_from_xcvr(reconfig_from_xcvr)
    );
  end
  
  //A5 => av_xcvr_custom_phy
  else if ( is_a5 | is_c5 ) begin
    av_xcvr_custom_nr #(
      .device_family(device_family),
      .protocol_hint(protocol_hint),
      .lanes(lanes),
      .pma_bonding_mode(pma_bonding_mode),
      .pcs_pma_width(pcs_pma_width),
      .ser_base_factor(ser_base_factor),
      .ser_words(ser_words),
      .mgmt_clk_in_mhz(mgmt_clk_in_mhz),
      .data_rate(data_rate),
      .base_data_rate(base_data_rate),
      .plls(plls),
      .pll_type(pll_type),
      .pll_select(pll_select),
      .pll_reconfig(pll_reconfig),
      .pll_refclk_cnt(pll_refclk_cnt),
      .pll_refclk_freq(pll_refclk_freq),
      .pll_refclk_select(pll_refclk_select),
	  .pll_external_enable(pll_external_enable),
      .cdr_refclk_select(cdr_refclk_select),
      .operation_mode(operation_mode),
      .starting_channel_number(starting_channel_number),
      .bonded_group_size(bonded_group_size),
      .embedded_reset(embedded_reset),
      .channel_interface(channel_interface),
	  .std_tx_pcfifo_mode(std_tx_pcfifo_mode),
	  .std_rx_pcfifo_mode(std_rx_pcfifo_mode),
      .use_8b10b(use_8b10b),
      .use_8b10b_manual_control(use_8b10b_manual_control),
      .tx_use_coreclk(tx_use_coreclk),
      .rx_use_coreclk(rx_use_coreclk),
      .en_synce_support(en_synce_support),
      .tx_bitslip_enable(tx_bitslip_enable),
      .word_aligner_mode(word_aligner_mode),
      .word_aligner_state_machine_datacnt(word_aligner_state_machine_datacnt),
      .word_aligner_state_machine_errcnt(word_aligner_state_machine_errcnt),
      .word_aligner_state_machine_patterncnt(word_aligner_state_machine_patterncnt),
      .run_length_violation_checking(run_length_violation_checking),
      .word_align_pattern(word_align_pattern),
      .word_aligner_pattern_length(word_aligner_pattern_length),
      .use_rate_match_fifo(use_rate_match_fifo),
      .rate_match_pattern1(rate_match_pattern1),
      .rate_match_pattern2(rate_match_pattern2),
      .byte_order_mode(byte_order_mode),
      .byte_order_pattern(byte_order_pattern),
      .byte_order_pad_pattern(byte_order_pad_pattern),
      .coreclk_0ppm_enable(coreclk_0ppm_enable)
    ) A5 (
      .mgmt_clk_reset(phy_mgmt_clk_reset),
      .mgmt_clk(phy_mgmt_clk),
      .mgmt_read(phy_mgmt_read),
      .mgmt_write(phy_mgmt_write),
      .mgmt_address(phy_mgmt_address[7:0]),
      .mgmt_writedata(phy_mgmt_writedata),
      .mgmt_readdata(phy_mgmt_readdata),
      .mgmt_waitrequest(phy_mgmt_waitrequest),
      .pll_powerdown(pll_powerdown),
      .tx_analogreset(tx_analogreset),
      .tx_digitalreset(tx_digitalreset),
      .rx_analogreset(rx_analogreset),
      .rx_digitalreset(rx_digitalreset),
      .tx_cal_busy(tx_cal_busy),
      .rx_cal_busy(rx_cal_busy),
      .pll_ref_clk(pll_ref_clk),
      .cdr_ref_clk(cdr_ref_clk), 
      .tx_coreclkin(tx_coreclkin),
      .rx_coreclkin(rx_coreclkin),
	  .ext_pll_clk(ext_pll_clk),
      .tx_clkout(tx_clkout),
      .rx_clkout(rx_clkout),
      .rx_recovered_clk(rx_recovered_clk),
      .rx_serial_data(rx_serial_data),
      .rx_parallel_data(rx_parallel_data),
      .tx_parallel_data(tx_parallel_data),
      .tx_serial_data(tx_serial_data),
      .tx_datak(tx_datak),
      .rx_datak(rx_datak),
      .tx_dispval(tx_dispval),
      .tx_forcedisp(tx_forcedisp),
      .rx_disperr(rx_disperr),
      .rx_a1a2sizeout(/*unused*/),
      .rx_errdetect(rx_errdetect),
      .rx_runningdisp(rx_runningdisp),
      .rx_patterndetect(rx_patterndetect),
      .tx_forceelecidle(tx_forceelecidle),
      .tx_bitslipboundaryselect(tx_bitslipboundaryselect),
      .rx_bitslipboundaryselectout(rx_bitslipboundaryselectout),
      .rx_rmfifodatainserted(rx_rmfifodatainserted),
      .rx_rmfifodatadeleted(rx_rmfifodatadeleted),
      .rx_rlv(rx_rlv),
      
      .rx_enabyteord(rx_enabyteord),
      .rx_bitslip(rx_bitslip),
      .pll_locked(pll_locked),
      .rx_is_lockedtoref(rx_is_lockedtoref),
      .rx_is_lockedtodata(rx_is_lockedtodata),
      .rx_signaldetect(rx_signaldetect),
      .rx_syncstatus(rx_syncstatus),
      .rx_byteordflag(rx_byteordflag),
      .tx_ready(tx_ready),
      .rx_ready(rx_ready),
      .reconfig_to_xcvr(reconfig_to_xcvr),
      .reconfig_from_xcvr(reconfig_from_xcvr)
    );
  end
  
    
  // S4 => siv_xcvr_custom_phy
  else if ( is_s4 ) begin
    siv_xcvr_custom_phy #(
      .device_family(device_family),
      .lanes(lanes),
      .ser_base_factor(ser_base_factor),
      .ser_words(ser_words),
      .mgmt_clk_in_mhz(mgmt_clk_in_mhz),
      .data_rate(data_rate),
      .plls(plls),
      .pll_refclk_freq(pll_refclk_freq),
      .operation_mode(operation_mode),
      .starting_channel_number(starting_channel_number),
      .bonded_group_size(bonded_group_size),
      .use_8b10b(use_8b10b),
      .use_8b10b_manual_control(use_8b10b_manual_control),
      .tx_use_coreclk(tx_use_coreclk),
      .rx_use_coreclk(rx_use_coreclk),
      .tx_bitslip_enable(tx_bitslip_enable),
      .word_aligner_mode(word_aligner_mode),
      .word_aligner_state_machine_datacnt(word_aligner_state_machine_datacnt),
      .word_aligner_state_machine_errcnt(word_aligner_state_machine_errcnt),
      .word_aligner_state_machine_patterncnt(word_aligner_state_machine_patterncnt),
      .run_length_violation_checking(run_length_violation_checking),
      .word_align_pattern(word_align_pattern),
      .word_aligner_pattern_length(word_aligner_pattern_length),
      .use_rate_match_fifo(use_rate_match_fifo),
      .rate_match_pattern1(rate_match_pattern1),
      .rate_match_pattern2(rate_match_pattern2),
      .byte_order_mode(byte_order_mode),
      .byte_order_pattern(byte_order_pattern),
      .byte_order_pad_pattern(byte_order_pad_pattern),
      .rx_termination(rx_termination),
      .rx_use_external_termination(rx_use_external_termination),
      .rx_common_mode(rx_common_mode),
      .rx_ppmselect(rx_ppmselect),
      .rx_signal_detect_threshold(rx_signal_detect_threshold),
      .rx_use_cruclk(rx_use_cruclk),
      .tx_termination(tx_termination),
      .tx_use_external_termination(tx_use_external_termination),
      .tx_analog_power(tx_analog_power),
      .tx_common_mode(tx_common_mode),
      .gxb_analog_power(gxb_analog_power),
      .tx_preemp_pretap(tx_preemp_pretap),
      .tx_preemp_pretap_inv(tx_preemp_pretap_inv),
      .tx_preemp_tap_1(tx_preemp_tap_1),
      .tx_preemp_tap_2(tx_preemp_tap_2),
      .tx_preemp_tap_2_inv(tx_preemp_tap_2_inv),
      .tx_vod_selection(tx_vod_selection),
      .rx_eq_dc_gain(rx_eq_dc_gain),
      .rx_eq_ctrl(rx_eq_ctrl),
      .pll_lock_speed(pll_lock_speed),
      .rx_pll_lock_speed(rx_pll_lock_speed),
      .tx_slew_rate(tx_slew_rate)
    ) S4 (
      .phy_mgmt_clk(phy_mgmt_clk),
      .phy_mgmt_clk_reset(phy_mgmt_clk_reset),
      .phy_mgmt_read(phy_mgmt_read),
      .phy_mgmt_write(phy_mgmt_write),
      .phy_mgmt_address(phy_mgmt_address),
      .phy_mgmt_writedata(phy_mgmt_writedata),
      .phy_mgmt_readdata(phy_mgmt_readdata),
      .phy_mgmt_waitrequest(phy_mgmt_waitrequest),
      .pll_ref_clk(pll_ref_clk),
      .tx_coreclkin(tx_coreclkin),
      .rx_coreclkin(rx_coreclkin),
      .tx_clkout(tx_clkout),
      .rx_clkout(rx_clkout),
      .rx_serial_data(rx_serial_data),
      .rx_parallel_data(rx_parallel_data),
      .tx_parallel_data(tx_parallel_data),
      .tx_serial_data(tx_serial_data),
      .tx_datak(tx_datak),
      .rx_datak(rx_datak),
      .tx_dispval(tx_dispval),
      .tx_forcedisp(tx_forcedisp),
      .rx_disperr(rx_disperr),
      .rx_errdetect(rx_errdetect),
      .rx_runningdisp(rx_runningdisp),
      .rx_patterndetect(rx_patterndetect),
      .tx_forceelecidle(tx_forceelecidle),
      .tx_bitslipboundaryselect(tx_bitslipboundaryselect),
      .rx_bitslipboundaryselectout(rx_bitslipboundaryselectout),
      .rx_enabyteord(rx_enabyteord),
      .rx_bitslip(rx_bitslip),
      .pll_locked(pll_locked),
      .rx_is_lockedtoref(rx_is_lockedtoref),
      .rx_is_lockedtodata(rx_is_lockedtodata),
      .rx_signaldetect(rx_signaldetect),
      .rx_syncstatus(rx_syncstatus),
      .tx_ready(tx_ready),
      .rx_ready(rx_ready)
    );
  end

  // default case when family did not match known strings
  else begin
    initial begin
      $display("Critical Warning: device_family value, '%s', is not supported", current_device_family(device_family));
    end
  end

  endgenerate

//initial begin
//  $display("altera_xcvr_custom_phy: cur_dev is '%s'", cur_dev);
//  $display("altera_xcvr_custom_phy: is_s5 is '%d'", is_s5);
//  $display("altera_xcvr_custom_phy: is_c5 is '%d'", is_c5);
//  $display("altera_xcvr_custom_phy: is_s4 is '%d'", is_s4);
//  $display("altera_xcvr_custom_phy: is_a5 is '%d'", is_a5);
//end
endmodule
