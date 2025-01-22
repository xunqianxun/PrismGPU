// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
// Date        : Wed Jan 22 09:23:31 2025
// Host        : DESKTOP-IDDJK51 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               d:/PrismGPU/PrismGPU/HardWare/Display/Display.gen/sources_1/bd/Prism/ip/Prism_clk_wiz_0/Prism_clk_wiz_0_stub.v
// Design      : Prism_clk_wiz_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k70tfbg676-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module Prism_clk_wiz_0(clk_55out, clk_276out, clk_100out, clk_200out, 
  resetn, locked, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_55out,clk_276out,clk_100out,clk_200out,resetn,locked,clk_in1" */;
  output clk_55out;
  output clk_276out;
  output clk_100out;
  output clk_200out;
  input resetn;
  output locked;
  input clk_in1;
endmodule
