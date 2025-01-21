set_property SRC_FILE_INFO {cfile:d:/PrismGPU/PrismGPU/HardWare/Display/Display.gen/sources_1/bd/Prism/ip/Prism_clk_wiz_0/Prism_clk_wiz_0.xdc rfile:../../../Display.gen/sources_1/bd/Prism/ip/Prism_clk_wiz_0/Prism_clk_wiz_0.xdc id:1 order:EARLY scoped_inst:inst} [current_design]
current_instance inst
set_property src_info {type:SCOPED_XDC file:1 line:57 export:INPUT save:INPUT read:READ} [current_design]
set_input_jitter [get_clocks -of_objects [get_ports clk_in1]] 0.2
