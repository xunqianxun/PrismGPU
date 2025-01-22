-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.2 (win64) Build 3064766 Wed Nov 18 09:12:45 MST 2020
-- Date        : Wed Jan 22 09:23:31 2025
-- Host        : DESKTOP-IDDJK51 running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               d:/PrismGPU/PrismGPU/HardWare/Display/Display.gen/sources_1/bd/Prism/ip/Prism_clk_wiz_0/Prism_clk_wiz_0_stub.vhdl
-- Design      : Prism_clk_wiz_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7k70tfbg676-2
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Prism_clk_wiz_0 is
  Port ( 
    clk_55out : out STD_LOGIC;
    clk_276out : out STD_LOGIC;
    clk_100out : out STD_LOGIC;
    clk_200out : out STD_LOGIC;
    resetn : in STD_LOGIC;
    locked : out STD_LOGIC;
    clk_in1 : in STD_LOGIC
  );

end Prism_clk_wiz_0;

architecture stub of Prism_clk_wiz_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_55out,clk_276out,clk_100out,clk_200out,resetn,locked,clk_in1";
begin
end;
