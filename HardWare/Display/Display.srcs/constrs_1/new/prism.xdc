set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets {Prism_i/clk_wiz/inst/clk_in1_Prism_clk_wiz_0}]

set_property PACKAGE_PIN D6 [get_ports {PICe_Clk_clk_p[0]}]
set_property PACKAGE_PIN C19 [get_ports Pice_rest]
set_property PACKAGE_PIN E15 [get_ports pice_link_up]
set_property PACKAGE_PIN D24 [get_ports sys_clk]
set_property PACKAGE_PIN J21 [get_ports TMDS_Clk_p_0]
set_property PACKAGE_PIN K23 [get_ports {TMDS_Data_p_0[2]}]
set_property PACKAGE_PIN G24 [get_ports {TMDS_Data_p_0[1]}]
set_property PACKAGE_PIN N21 [get_ports {TMDS_Data_p_0[0]}]

set_property IOSTANDARD LVCMOS33 [get_ports sys_clk]
set_property IOSTANDARD LVCMOS33 [get_ports pice_link_up]
set_property IOSTANDARD LVCMOS33 [get_ports Pice_rest]
