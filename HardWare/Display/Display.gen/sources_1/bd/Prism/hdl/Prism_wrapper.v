//Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
//Date        : Tue Jan 21 21:57:34 2025
//Host        : DESKTOP-IDDJK51 running 64-bit major release  (build 9200)
//Command     : generate_target Prism_wrapper.bd
//Design      : Prism_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module Prism_wrapper
   (DDR2_interface_addr,
    DDR2_interface_ba,
    DDR2_interface_cas_n,
    DDR2_interface_ck_n,
    DDR2_interface_ck_p,
    DDR2_interface_cke,
    DDR2_interface_cs_n,
    DDR2_interface_dm,
    DDR2_interface_dq,
    DDR2_interface_dqs_n,
    DDR2_interface_dqs_p,
    DDR2_interface_odt,
    DDR2_interface_ras_n,
    DDR2_interface_we_n,
    PICe_Clk_clk_n,
    PICe_Clk_clk_p,
    Pice_rest,
    TMDS_Clk_n_0,
    TMDS_Clk_p_0,
    TMDS_Data_n_0,
    TMDS_Data_p_0,
    pice_interface_rxn,
    pice_interface_rxp,
    pice_interface_txn,
    pice_interface_txp,
    pice_link_up,
    sys_clk);
  output [12:0]DDR2_interface_addr;
  output [2:0]DDR2_interface_ba;
  output DDR2_interface_cas_n;
  output [1:0]DDR2_interface_ck_n;
  output [1:0]DDR2_interface_ck_p;
  output [0:0]DDR2_interface_cke;
  output [1:0]DDR2_interface_cs_n;
  output [3:0]DDR2_interface_dm;
  inout [31:0]DDR2_interface_dq;
  inout [3:0]DDR2_interface_dqs_n;
  inout [3:0]DDR2_interface_dqs_p;
  output [1:0]DDR2_interface_odt;
  output DDR2_interface_ras_n;
  output DDR2_interface_we_n;
  input [0:0]PICe_Clk_clk_n;
  input [0:0]PICe_Clk_clk_p;
  input Pice_rest;
  output TMDS_Clk_n_0;
  output TMDS_Clk_p_0;
  output [2:0]TMDS_Data_n_0;
  output [2:0]TMDS_Data_p_0;
  input [3:0]pice_interface_rxn;
  input [3:0]pice_interface_rxp;
  output [3:0]pice_interface_txn;
  output [3:0]pice_interface_txp;
  output pice_link_up;
  input sys_clk;

  wire [12:0]DDR2_interface_addr;
  wire [2:0]DDR2_interface_ba;
  wire DDR2_interface_cas_n;
  wire [1:0]DDR2_interface_ck_n;
  wire [1:0]DDR2_interface_ck_p;
  wire [0:0]DDR2_interface_cke;
  wire [1:0]DDR2_interface_cs_n;
  wire [3:0]DDR2_interface_dm;
  wire [31:0]DDR2_interface_dq;
  wire [3:0]DDR2_interface_dqs_n;
  wire [3:0]DDR2_interface_dqs_p;
  wire [1:0]DDR2_interface_odt;
  wire DDR2_interface_ras_n;
  wire DDR2_interface_we_n;
  wire [0:0]PICe_Clk_clk_n;
  wire [0:0]PICe_Clk_clk_p;
  wire Pice_rest;
  wire TMDS_Clk_n_0;
  wire TMDS_Clk_p_0;
  wire [2:0]TMDS_Data_n_0;
  wire [2:0]TMDS_Data_p_0;
  wire [3:0]pice_interface_rxn;
  wire [3:0]pice_interface_rxp;
  wire [3:0]pice_interface_txn;
  wire [3:0]pice_interface_txp;
  wire pice_link_up;
  wire sys_clk;

  Prism Prism_i
       (.DDR2_interface_addr(DDR2_interface_addr),
        .DDR2_interface_ba(DDR2_interface_ba),
        .DDR2_interface_cas_n(DDR2_interface_cas_n),
        .DDR2_interface_ck_n(DDR2_interface_ck_n),
        .DDR2_interface_ck_p(DDR2_interface_ck_p),
        .DDR2_interface_cke(DDR2_interface_cke),
        .DDR2_interface_cs_n(DDR2_interface_cs_n),
        .DDR2_interface_dm(DDR2_interface_dm),
        .DDR2_interface_dq(DDR2_interface_dq),
        .DDR2_interface_dqs_n(DDR2_interface_dqs_n),
        .DDR2_interface_dqs_p(DDR2_interface_dqs_p),
        .DDR2_interface_odt(DDR2_interface_odt),
        .DDR2_interface_ras_n(DDR2_interface_ras_n),
        .DDR2_interface_we_n(DDR2_interface_we_n),
        .PICe_Clk_clk_n(PICe_Clk_clk_n),
        .PICe_Clk_clk_p(PICe_Clk_clk_p),
        .Pice_rest(Pice_rest),
        .TMDS_Clk_n_0(TMDS_Clk_n_0),
        .TMDS_Clk_p_0(TMDS_Clk_p_0),
        .TMDS_Data_n_0(TMDS_Data_n_0),
        .TMDS_Data_p_0(TMDS_Data_p_0),
        .pice_interface_rxn(pice_interface_rxn),
        .pice_interface_rxp(pice_interface_rxp),
        .pice_interface_txn(pice_interface_txn),
        .pice_interface_txp(pice_interface_txp),
        .pice_link_up(pice_link_up),
        .sys_clk(sys_clk));
endmodule
