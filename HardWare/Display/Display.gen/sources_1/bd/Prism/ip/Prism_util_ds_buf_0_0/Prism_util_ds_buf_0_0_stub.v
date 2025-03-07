// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
// Date        : Wed Jan 22 09:23:29 2025
// Host        : DESKTOP-IDDJK51 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               d:/PrismGPU/PrismGPU/HardWare/Display/Display.gen/sources_1/bd/Prism/ip/Prism_util_ds_buf_0_0/Prism_util_ds_buf_0_0_stub.v
// Design      : Prism_util_ds_buf_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k70tfbg676-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "util_ds_buf,Vivado 2020.2" *)
module Prism_util_ds_buf_0_0(IBUF_DS_P, IBUF_DS_N, IBUF_OUT, IBUF_DS_ODIV2)
/* synthesis syn_black_box black_box_pad_pin="IBUF_DS_P[0:0],IBUF_DS_N[0:0],IBUF_OUT[0:0],IBUF_DS_ODIV2[0:0]" */;
  input [0:0]IBUF_DS_P;
  input [0:0]IBUF_DS_N;
  output [0:0]IBUF_OUT;
  output [0:0]IBUF_DS_ODIV2;
endmodule
