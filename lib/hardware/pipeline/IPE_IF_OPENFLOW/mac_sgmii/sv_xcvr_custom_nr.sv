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

import altera_xcvr_functions::*;

module sv_xcvr_custom_nr #(
    parameter device_family           = "Stratix V",
    parameter protocol_hint           = "basic",    // (basic, gige, cpri)
    parameter operation_mode          = "Duplex", //legal value: TX, RX, Duplex
    parameter lanes                   = 1,          //legal value: 1+
    parameter bonded_group_size       = 1, //legal value: integer from 1 .. lanes
    parameter bonded_mode             = "xN",       // (xN, fb_compensation)
    parameter pcs_pma_width           = 8, //legal value: 8,10,16,20
    parameter ser_base_factor         = 8,          //legal value: 8,10
    parameter ser_words               = 1,          // legal values: 1,2,4
    parameter data_rate               = "1250 Mbps",
    parameter base_data_rate          = "0 Mbps",   // (PLL Rate) - must be (data_rate * 1,2,4,or8)

    // tx bitslip
    parameter tx_bitslip_enable       = "false",

    //optional coreclks 
    parameter tx_use_coreclk          = "false",
    parameter rx_use_coreclk          = "false",
    parameter en_synce_support = 0,   //expose CDR ref-clk in this mode


    //Phase compensation FIFO
    parameter std_tx_pcfifo_mode = "low_latency",
    parameter std_rx_pcfifo_mode = "low_latency", 
	
    //8B10B
    parameter use_8b10b               = "false", //legal value: "false", "true"
    parameter use_8b10b_manual_control= "false", //legal value: "false", "true"

    //Word Aligner
    parameter word_aligner_mode                     = "sync_state_machine", //legal value: bitslip, sync state machine, manual
    parameter word_aligner_state_machine_datacnt    = 0, //legal value: 0 - 256
    parameter word_aligner_state_machine_errcnt     = 0, //legal value: 0 - 256
    parameter word_aligner_state_machine_patterncnt = 0, //legal value: 0 - 256
    parameter word_aligner_pattern_length           = 7,
    parameter word_align_pattern                    = "0000000000", 
    parameter run_length_violation_checking         = 0, //legal value: 0, 1+

    //RM FIFO
    parameter use_rate_match_fifo     = 0, //legal value: 0, 1
    parameter rate_match_pattern1     = "00000000000000000000",
    parameter rate_match_pattern2     = "00000000000000000000",

    //Byte Ordering Block
    parameter byte_order_mode         = "None", //legal value: None, Sync state machine, PLD control"
    parameter byte_order_pattern      = "0", 
    parameter byte_order_pad_pattern  = "0",

    //Hidden parameter to enable 0ppm legality bypass
    parameter coreclk_0ppm_enable     = "false",
    
    //PLL
    parameter pll_refclk_cnt    = 1,          // Number of reference clocks
    parameter pll_refclk_freq   = "125 MHz",  // Frequency of each reference clock
    parameter pll_refclk_select = "0",        // Selects the initial reference clock for each PLL
    parameter cdr_refclk_select = 0,          // Selects the initial reference clock for all RX CDR PLLs
    parameter plls              = 1,          // (1+)
    parameter pll_type          = "AUTO",     // PLL type for each PLL
    parameter pll_select        = 0,          // Selects the initial PLL
    parameter pll_reconfig      = 0,          // (0,1) 0-Disable PLL reconfig, 1-Enable PLL reconfig
    parameter pll_feedback_path = "no_compensation", //no_compensation, tx_clkout
	parameter pll_external_enable = 0,         // (0,1) 0-Disable external TX PLL, 1-Enable external TX PLL

    //system clock rate
    parameter mgmt_clk_in_mhz         = 150,
    parameter embedded_reset          = 1,  // (0,1) 1-Enable embedded reset controller
	parameter channel_interface = 0, //legal value: (0,1) 1-Enable channel reconfiguration
    parameter starting_channel_number = 0  //legal value: 0+
) ( 
  // user data (avalon-MM slave interface) //for all the channel rst, powerdown, rx serilize loopback enable
  input   wire                        mgmt_clk_reset,
  input   wire                        mgmt_clk,
  input   wire  [7:0]                 mgmt_address,
  input   wire                        mgmt_read,
  output  wire  [31:0]                mgmt_readdata,
  input   wire                        mgmt_write,
  input   wire  [31:0]                mgmt_writedata,
  output  wire                        mgmt_waitrequest,

  // Reset inputs
  input   wire    [plls -1:0]     pll_powerdown, 
  input   wire    [lanes-1:0]     tx_analogreset,
  input   wire    [lanes-1:0]     tx_digitalreset,
  input   wire    [lanes-1:0]     rx_analogreset,
  input   wire    [lanes-1:0]     rx_digitalreset,

  // Calibration busy signals
  output  wire    [lanes-1:0]     tx_cal_busy,
  output  wire    [lanes-1:0]     rx_cal_busy,

  //clk signal
  input   wire  [pll_refclk_cnt-1:0]  pll_ref_clk,
  input   wire                        cdr_ref_clk, // used only in SyncE mode
  input   wire  [lanes-1:0]           tx_coreclkin,
  input   wire  [lanes-1:0]           rx_coreclkin,
  input   wire  [(plls*lanes)-1:0]    ext_pll_clk,      // clkout from external PLL

  //data ports - Avalon ST interface
  input  wire [(channel_interface? 44: ser_base_factor*ser_words)*lanes-1:0] tx_parallel_data,
  output wire [(channel_interface? 64: ser_base_factor*ser_words)*lanes-1:0] rx_parallel_data,
  input   wire  [ser_words*lanes-1:0] tx_datak,
  output  wire  [ser_words*lanes-1:0] rx_datak,
  input   wire  [ser_words*lanes-1:0] tx_forcedisp,
  input   wire  [ser_words*lanes-1:0] tx_dispval,
  output  wire  [ser_words*lanes-1:0] rx_runningdisp,
  input   wire  [lanes-1:0]           rx_enabyteord,
  input   wire  [lanes-1:0]           rx_bitslip,

  //optional ports
  input   wire  [lanes*5-1:0]         tx_bitslipboundaryselect,
  input   wire  [lanes-1:0]           tx_forceelecidle,
  output  wire  [ser_words*lanes-1:0] rx_syncstatus,
  output  wire  [ser_words*lanes-1:0] rx_patterndetect,
  output  wire  [lanes-1:0]           rx_signaldetect,
  output  wire  [ser_words*lanes-1:0] rx_errdetect,
  output  wire  [ser_words*lanes-1:0] rx_disperr,
  output  wire  [ser_words*lanes-1:0] rx_a1a2sizeout,
  output  wire  [lanes*5-1:0]         rx_bitslipboundaryselectout,
  output  wire  [ser_words*lanes-1:0] rx_rmfifodatainserted,
  output  wire  [ser_words*lanes-1:0] rx_rmfifodatadeleted,
  output  wire  [lanes-1:0]           rx_rlv,
  output  wire  [lanes-1:0]           rx_byteordflag,

  //conduit
  input   wire  [lanes-1:0]           rx_serial_data,
  output  wire  [lanes-1:0]           tx_serial_data,

  output  wire  [plls-1 :0]           pll_locked,
  output  wire  [lanes-1:0]           rx_is_lockedtodata,
  output  wire  [lanes-1:0]           rx_is_lockedtoref,

  output  wire                        rx_ready,
  output  wire                        tx_ready,

  //clock outputs
  output  wire  [(lanes/bonded_group_size)-1:0] tx_clkout,
  output  wire  [lanes-1:0]                     rx_clkout,
  output  wire  [lanes-1:0]                     rx_recovered_clk,

  // reconfiguration ports and status
  input   wire  [altera_xcvr_functions::get_custom_reconfig_to_width  ("Stratix V",operation_mode,lanes,plls,bonded_group_size,"",bonded_mode)-1:0] reconfig_to_xcvr,
  output  wire  [altera_xcvr_functions::get_custom_reconfig_from_width("Stratix V",operation_mode,lanes,plls,bonded_group_size,"",bonded_mode)-1:0] reconfig_from_xcvr 

);

  localparam  TX_ENABLE = (operation_mode != "Rx" && operation_mode != "RX");
  localparam  RX_ENABLE = (operation_mode != "Tx" && operation_mode != "TX");

  wire [lanes-1:0]            rx_rmfifofull_wire;
  wire [lanes-1:0]            rx_rmfifoempty_wire;
  wire [lanes-1:0]            rx_phase_comp_fifo_error_wire;
  wire [lanes-1:0]            tx_phase_comp_fifo_error_wire;

  wire [lanes-1:0] 	      rxqpipulldn;  // QPI input port
  wire [lanes-1:0] 	      txqpipulldn;  // QPI input port
  wire [lanes-1:0] 	      txqpipullup;  // QPI input port\
  wire [lanes-1:0] 	      rx_clk_slip_in;  // PMA receive clk slip - for QPI
     

  //////////////////////////////////
  // Control & status register map (CSR) outputs
  //////////////////////////////////
  wire                csr_reset_tx_digital;         //to reset controller
  wire                csr_reset_rx_digital;         //to reset controller
  wire                csr_reset_all;                //to reset controller
  wire                csr_pll_powerdown;            //to xcvr instance
  wire [lanes-1:0]    csr_tx_digitalreset;          //to xcvr instance
  wire [lanes-1:0]    csr_rx_analogreset;           //to xcvr instance
  wire [lanes-1:0]    csr_rx_digitalreset;          //to xcvr instance
  wire [lanes-1:0]    csr_phy_loopback_serial;      //to xcvr instance
  wire [lanes-1:0]    csr_tx_invpolarity;           //to xcvr instance
  wire [lanes-1:0]    csr_rx_invpolarity;           //to xcvr instance
  wire [lanes-1:0]    csr_rx_set_locktoref;         //to xcvr instance
  wire [lanes-1:0]    csr_rx_set_locktodata;        //to xcvr instance
  wire [lanes-1:0]    csr_rx_enapatternalign;       //to xcvr instance
  wire [lanes-1:0]    csr_rx_bitreversalenable;     //to xcvr instance
  wire [lanes-1:0]    csr_rx_bytereversalenable;    //to xcvr instance
  wire [lanes-1:0]    csr_rx_a1a2size;              //to xcvr instance
  wire [lanes*5-1:0]  csr_tx_bitslipboundaryselect; //to xcvr instance

  // readdata output from both CSR blocks
  wire [31:0]     mgmt_readdata_common;
  wire [31:0]     mgmt_readdata_pcs;


  //////////////////////////////////
  //reset controller outputs
  //////////////////////////////////
  wire              reset_controller_pll_powerdown;
  wire  [lanes-1:0] reset_controller_tx_digitalreset;
  wire  [lanes-1:0] reset_controller_rx_analogreset;
  wire  [lanes-1:0] reset_controller_rx_digitalreset;
  wire  [lanes-1:0] reset_controller_tx_ready;
  wire  [lanes-1:0] reset_controller_rx_ready;

  // Final reset signals
  wire  [plls-1:0]  pll_powerdown_fnl;
  wire  [lanes-1:0] tx_analogreset_fnl;
  wire  [lanes-1:0] tx_digitalreset_fnl;
  wire  [lanes-1:0] rx_analogreset_fnl;
  wire  [lanes-1:0] rx_digitalreset_fnl;

  assign  pll_powerdown_fnl   = (embedded_reset)  ? {plls {csr_pll_powerdown}} : pll_powerdown;
  assign  tx_analogreset_fnl  = (embedded_reset)  ? {lanes{csr_pll_powerdown}} : tx_analogreset;
  assign  tx_digitalreset_fnl = csr_tx_digitalreset | (embedded_reset ? {lanes{1'b0}} : tx_digitalreset);
  assign  rx_analogreset_fnl  = csr_rx_analogreset  | (embedded_reset ? {lanes{1'b0}} : rx_analogreset );
  assign  rx_digitalreset_fnl = csr_rx_digitalreset | (embedded_reset ? {lanes{1'b0}} : rx_digitalreset);

  assign rxqpipulldn = {lanes{1'b0}};  // QPI
  assign txqpipulldn = {lanes{1'b0}};  // QPI
  assign txqpipullup = {lanes{1'b0}};  // QPI 
  assign rx_clk_slip_in = {lanes{1'b0}};  // for QPI
     
 
  sv_xcvr_custom_native #(
    .device_family                        (device_family                ),
    .protocol_hint                        (protocol_hint                ),
    .lanes                                (lanes                        ),
    .plls                                 (plls                         ),
    .pll_type                             (pll_type                     ),
    .pll_select                           (pll_select                   ),
    .pll_reconfig                         (pll_reconfig                 ),
    .pcs_pma_width                        (pcs_pma_width                ),
    .ser_base_factor                      (ser_base_factor              ),
    .ser_words                            (ser_words                    ),
    .data_rate                            (data_rate                    ),
    .base_data_rate                       (base_data_rate               ),
    .pll_refclk_cnt                       (pll_refclk_cnt               ),
    .pll_refclk_freq                      (pll_refclk_freq              ),
    .pll_refclk_select                    (pll_refclk_select            ),
	.pll_external_enable                  (pll_external_enable          ),
    .cdr_refclk_select                    (cdr_refclk_select            ),
    .operation_mode                       (operation_mode               ),
    .starting_channel_number              (starting_channel_number      ),
    .bonded_group_size                    (bonded_group_size            ),
	.channel_interface                    (channel_interface            ),
    .bonded_mode                          (bonded_mode                  ),
	.std_tx_pcfifo_mode                   (std_tx_pcfifo_mode           ),
	.std_rx_pcfifo_mode                   (std_rx_pcfifo_mode           ),
    .use_8b10b                            (use_8b10b                    ),
    .use_8b10b_manual_control             (use_8b10b_manual_control     ),
    .tx_use_coreclk                       (tx_use_coreclk               ),
    .rx_use_coreclk                       (rx_use_coreclk               ),
    .en_synce_support                     (en_synce_support             ), //expose CDR ref-clk in this mode
    .tx_bitslip_enable                    (tx_bitslip_enable            ),
    .word_aligner_mode                    (word_aligner_mode            ),
    .word_aligner_state_machine_datacnt   (word_aligner_state_machine_datacnt),
    .word_aligner_state_machine_errcnt    (word_aligner_state_machine_errcnt),
    .word_aligner_state_machine_patterncnt(word_aligner_state_machine_patterncnt),
    .run_length_violation_checking        (run_length_violation_checking),
    .word_align_pattern                   (word_align_pattern           ), 
    .word_aligner_pattern_length          (word_aligner_pattern_length  ),
    .use_rate_match_fifo                  (use_rate_match_fifo          ),
    .rate_match_pattern1                  (rate_match_pattern1          ),
    .rate_match_pattern2                  (rate_match_pattern2          ),
    .byte_order_mode                      (byte_order_mode              ),
    .byte_order_pattern                   (byte_order_pattern           ), 
    .byte_order_pad_pattern               (byte_order_pad_pattern       ),
    .coreclk_0ppm_enable                  (coreclk_0ppm_enable          ),
    .pll_feedback_path                    (pll_feedback_path            )
  ) transceiver_core (
    .tx_analogreset             (tx_analogreset_fnl             ),
    .pll_powerdown              (pll_powerdown_fnl              ), 
    .tx_digitalreset            (tx_digitalreset_fnl            ),
    .rx_analogreset             (rx_analogreset_fnl             ),
    .rx_digitalreset            (rx_digitalreset_fnl            ),
    .tx_cal_busy                (tx_cal_busy                    ),
    .rx_cal_busy                (rx_cal_busy                    ),
    .pll_ref_clk                (pll_ref_clk                    ),
    .cdr_ref_clk                (cdr_ref_clk                    ),  // used only in SyncE mode
    .tx_coreclkin               (tx_coreclkin                   ),
    .rx_coreclkin               (rx_coreclkin                   ),
	.ext_pll_clk                (ext_pll_clk                    ),
    .tx_parallel_data           (tx_parallel_data               ),
    .rx_parallel_data           (rx_parallel_data               ),
    .tx_datak                   (tx_datak                       ),
    .rx_datak                   (rx_datak                       ),
    .tx_forcedisp               (tx_forcedisp                   ),
    .tx_dispval                 (tx_dispval                     ),
    .rx_runningdisp             (rx_runningdisp                 ),
    .rx_serial_data             (rx_serial_data                 ),
    .tx_serial_data             (tx_serial_data                 ),
    .tx_clkout                  (tx_clkout                      ),
    .rx_clkout                  (rx_clkout                      ),
    .rx_recovered_clk           (rx_recovered_clk               ),
    .rx_enabyteord              (rx_enabyteord                  ),
    .rx_bitslip                 (rx_bitslip                     ),
    //MM ports
    .tx_forceelecidle           (tx_forceelecidle               ),
    .tx_invpolarity             (csr_tx_invpolarity             ),
    .tx_bitslipboundaryselect   (tx_bitslipboundaryselect
                                 | csr_tx_bitslipboundaryselect ),
    .rx_invpolarity             (csr_rx_invpolarity             ),
    .rx_seriallpbken            (csr_phy_loopback_serial        ),
    .rx_set_locktodata          (csr_rx_set_locktodata          ),
    .rx_set_locktoref           (csr_rx_set_locktoref           ),
    .rx_enapatternalign         (csr_rx_enapatternalign         ),
    .rx_bitreversalenable       (csr_rx_bitreversalenable       ),
    .rx_bytereversalenable      (csr_rx_bytereversalenable      ),
    .rx_a1a2size                (csr_rx_a1a2size                ),
    .rx_rlv                     (rx_rlv                         ),
    .rx_patterndetect           (rx_patterndetect               ),
    .rx_syncstatus              (rx_syncstatus                  ),
    .rx_signaldetect            (rx_signaldetect                ),
    .rx_bitslipboundaryselectout(rx_bitslipboundaryselectout    ),
    .rx_errdetect               (rx_errdetect                   ),
    .rx_disperr                 (rx_disperr                     ),
    .rx_rmfifofull              (rx_rmfifofull_wire             ),
    .rx_rmfifoempty             (rx_rmfifoempty_wire            ),
    .rx_rmfifodatainserted      (rx_rmfifodatainserted          ),
    .rx_rmfifodatadeleted       (rx_rmfifodatadeleted           ),
    .rx_a1a2sizeout             (rx_a1a2sizeout                 ),
    .rx_byteordflag             (rx_byteordflag                 ),
    .rx_is_lockedtoref          (rx_is_lockedtoref              ),
    .rx_is_lockedtodata         (rx_is_lockedtodata             ),
    .pll_locked                 (pll_locked                     ),
    .rx_phase_comp_fifo_error   (rx_phase_comp_fifo_error_wire  ),
    .tx_phase_comp_fifo_error   (tx_phase_comp_fifo_error_wire  ),
    .reconfig_to_xcvr           (reconfig_to_xcvr               ),
    .reconfig_from_xcvr         (reconfig_from_xcvr             ),
    //QPI ports
    .rxqpipulldn                (rxqpipulldn                    ),
    .txqpipulldn                (txqpipulldn                    ),
    .txqpipullup                (txqpipullup                    ),
    .rx_clk_slip_in             (rx_clk_slip_in                 )    
  );

  // Instantiate memory map logic for given number of lanes & PLL's
  // Includes all except PCS
  alt_xcvr_csr_common #(
    .lanes  (lanes),
    .plls   (plls ),
    .rpc    (1    )
  ) csr (
    .clk                              (mgmt_clk                         ),
    .reset                            (mgmt_clk_reset                   ),
    .address                          (mgmt_address                     ),
    .read                             (mgmt_read                        ),
    .write                            (mgmt_write                       ),
    .writedata                        (mgmt_writedata                   ),
    .pll_locked                       (pll_locked                       ),
    .rx_is_lockedtoref                (rx_is_lockedtoref                ),
    .rx_is_lockedtodata               (rx_is_lockedtodata               ),
    .rx_signaldetect                  (rx_signaldetect                  ),
    .reset_controller_tx_ready        (tx_ready                         ),
    .reset_controller_rx_ready        (rx_ready                         ),
    .reset_controller_pll_powerdown   (reset_controller_pll_powerdown   ),
    .reset_controller_tx_digitalreset (reset_controller_tx_digitalreset ),
    .reset_controller_rx_analogreset  (reset_controller_rx_analogreset  ),
    .reset_controller_rx_digitalreset (reset_controller_rx_digitalreset ),
    .readdata                         (mgmt_readdata_common             ),
    .csr_reset_tx_digital             (csr_reset_tx_digital             ),
    .csr_reset_rx_digital             (csr_reset_rx_digital             ),
    .csr_reset_all                    (csr_reset_all                    ),
    .csr_pll_powerdown                (csr_pll_powerdown                ),
    .csr_tx_digitalreset              (csr_tx_digitalreset              ),
    .csr_rx_analogreset               (csr_rx_analogreset               ),
    .csr_rx_digitalreset              (csr_rx_digitalreset              ),
    .csr_phy_loopback_serial          (csr_phy_loopback_serial          ),
    .csr_rx_set_locktoref             (csr_rx_set_locktoref             ),
    .csr_rx_set_locktodata            (csr_rx_set_locktodata            )
  );

  // generate waitrequest for 'top' channel
  altera_wait_generate top_wait (
    .rst            (mgmt_clk_reset   ),
    .clk            (mgmt_clk         ),
    .launch_signal  (mgmt_read        ),
    .wait_req       (mgmt_waitrequest )
  );

  // Instantiate PCS memory map logic for given number of lanes
  alt_xcvr_csr_pcs8g #(
    .lanes  (lanes    ),
    .words  (ser_words)
  ) csr_pcs (
    .clk                          (mgmt_clk                     ),
    .reset                        (mgmt_clk_reset               ),
    .address                      (mgmt_address                 ),
    .read                         (mgmt_read                    ),
    .write                        (mgmt_write                   ),
    .writedata                    (mgmt_writedata               ),
    .readdata                     (mgmt_readdata_pcs            ),
    .rx_clk                       (rx_clkout[0]                 ),
    .tx_clk                       (tx_clkout[0]                 ),
    .rx_patterndetect             (rx_patterndetect             ),
    .rx_syncstatus                (rx_syncstatus                ),
    .rlv                          (rx_rlv                       ),
    .rx_phase_comp_fifo_error     (rx_phase_comp_fifo_error_wire),
    .tx_phase_comp_fifo_error     (tx_phase_comp_fifo_error_wire),
    .rx_errdetect                 (rx_errdetect                 ),
    .rx_disperr                   (rx_disperr                   ),
    .rx_bitslipboundaryselectout  (rx_bitslipboundaryselectout  ),
    .rx_a1a2sizeout               (rx_a1a2sizeout               ),
    .csr_tx_invpolarity           (csr_tx_invpolarity           ),
    .csr_rx_invpolarity           (csr_rx_invpolarity           ),
    .csr_rx_bitreversalenable     (csr_rx_bitreversalenable     ),
    .csr_rx_bitslip               (/*unused*/                   ),
    .csr_rx_enapatternalign       (csr_rx_enapatternalign       ),
    .csr_rx_bytereversalenable    (csr_rx_bytereversalenable    ),
    .csr_rx_a1a2size              (csr_rx_a1a2size              ),
    .csr_tx_bitslipboundaryselect (csr_tx_bitslipboundaryselect )
  );

  // combine readdata output from both CSR blocks
  // each decodes non-overlapping addresses, and outputs "11..111" for undecoded addresses,
  // so an AND is sufficient
  assign mgmt_readdata = mgmt_readdata_common & mgmt_readdata_pcs;

  // Reset Controller
  generate if (embedded_reset) begin : gen_embedded_reset
    localparam  RX_PER_CHANNEL = (bonded_group_size == 1);
    wire  [lanes-1:0]   rx_manual_mode;

    // Put reset controller into manual mode when we are not in auto lock mode
    assign  rx_manual_mode = (csr_rx_set_locktoref | csr_rx_set_locktodata);
    // We have a single tx_ready, rx_ready output per IP instance
    assign  tx_ready  = &reset_controller_tx_ready;
    assign  rx_ready  = &reset_controller_rx_ready;

    altera_xcvr_reset_control
    #(
        .CHANNELS               (lanes          ),  // Number of CHANNELS
        .SYNCHRONIZE_RESET      (0              ),  // (0,1) Synchronize the reset input
        .SYNCHRONIZE_PLL_RESET  (0              ),  // (0,1) Use synchronized reset input for PLL powerdown
                                                    // !NOTE! Will prevent PLL merging across reset controllers
                                                    // !NOTE! Requires SYNCHRONIZE_RESET == 1
        // Reset timings
        .SYS_CLK_IN_MHZ         (mgmt_clk_in_mhz),  // Clock frequency in MHz. Required for reset timers
        .REDUCED_SIM_TIME       (1              ),  // (0,1) 1=Reduced reset timings for simulation
        // PLL options
        .TX_PLL_ENABLE          (TX_ENABLE      ),  // (0,1) Enable TX PLL reset
        .PLLS                   (1              ),  // Number of TX PLLs
        .T_PLL_POWERDOWN        (1000           ),  // pll_powerdown period in ns
        // TX options
        .TX_ENABLE              (TX_ENABLE      ),  // (0,1) Enable TX resets
        .TX_PER_CHANNEL         (0              ),  // (0,1) 1=separate TX reset per channel
        .T_TX_DIGITALRESET      (20             ),  // tx_digitalreset period (after pll_powerdown)
        .T_PLL_LOCK_HYST        (0              ),  // Amount of hysteresis to add to pll_locked status signal
        // RX options
        .RX_ENABLE              (RX_ENABLE      ),  // (0,1) Enable RX resets
        .RX_PER_CHANNEL         (RX_PER_CHANNEL ),  // (0,1) 1=separate RX reset per channel
        .T_RX_ANALOGRESET       (40             ),  // rx_analogreset period
        .T_RX_DIGITALRESET      (4000           )   // rx_digitalreset period (after rx_is_lockedtodata)
    ) reset_controller (
      // User inputs and outputs
      .clock            (mgmt_clk       ),  // System clock
      .reset            (mgmt_clk_reset ),  // Asynchronous reset
      // Reset signals
      .pll_powerdown    (reset_controller_pll_powerdown   ),  // reset TX PLL
      .tx_analogreset   (/*unused*/                       ),  // reset TX PMA
      .tx_digitalreset  (reset_controller_tx_digitalreset ),  // reset TX PCS
      .rx_analogreset   (reset_controller_rx_analogreset  ),  // reset RX PMA
      .rx_digitalreset  (reset_controller_rx_digitalreset ),  // reset RX PCS
      // Status output
      .tx_ready         (reset_controller_tx_ready        ),  // TX is not in reset
      .rx_ready         (reset_controller_rx_ready        ),  // RX is not in reset
      // Digital reset override inputs (must by synchronous with clock)
      .tx_digitalreset_or({lanes{csr_reset_tx_digital}} ), // reset request for tx_digitalreset
      .rx_digitalreset_or({lanes{csr_reset_rx_digital}} ), // reset request for rx_digitalreset
      // TX control inputs
      .pll_locked         (pll_locked[pll_select] ),  // TX PLL is locked status
      .pll_select         (1'b0                   ),  // Select TX PLL locked signal 
      .tx_cal_busy        (tx_cal_busy            ),  // TX channel calibration status
      .tx_manual          ({lanes{1'b1}}          ),  // 1=Manual TX reset mode
      // RX control inputs
      .rx_is_lockedtodata (rx_is_lockedtodata     ),  // RX CDR PLL is locked to data status
      .rx_cal_busy        (rx_cal_busy            ),  // RX channel calibration status
      .rx_manual          (rx_manual_mode         ) // 1=Manual RX reset mode
    );
  end else begin:gen_no_embedded_reset
    assign  reset_controller_pll_powerdown    = 1'b0;
    assign  reset_controller_tx_digitalreset  = {lanes{1'b0}};
    assign  reset_controller_rx_analogreset   = {lanes{1'b0}};
    assign  reset_controller_rx_digitalreset  = {lanes{1'b0}};
    assign  tx_ready = 1'b0;
    assign  rx_ready = 1'b0;
  end
  endgenerate

endmodule
