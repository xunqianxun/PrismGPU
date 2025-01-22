-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
-- Date        : Tue Jan 21 21:59:12 2025
-- Host        : DESKTOP-IDDJK51 running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               d:/PrismGPU/PrismGPU/HardWare/Display/Display.gen/sources_1/bd/Prism/ip/Prism_v_tc_0_0/Prism_v_tc_0_0_stub.vhdl
-- Design      : Prism_v_tc_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7k70tfbg676-2
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Prism_v_tc_0_0 is
  Port ( 
    clk : in STD_LOGIC;
    clken : in STD_LOGIC;
    gen_clken : in STD_LOGIC;
    sof_state : in STD_LOGIC;
    hsync_out : out STD_LOGIC;
    hblank_out : out STD_LOGIC;
    vsync_out : out STD_LOGIC;
    vblank_out : out STD_LOGIC;
    active_video_out : out STD_LOGIC;
    resetn : in STD_LOGIC;
    fsync_out : out STD_LOGIC_VECTOR ( 2 downto 0 )
  );

end Prism_v_tc_0_0;

architecture stub of Prism_v_tc_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk,clken,gen_clken,sof_state,hsync_out,hblank_out,vsync_out,vblank_out,active_video_out,resetn,fsync_out[2:0]";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "v_tc,Vivado 2020.2";
begin
end;
