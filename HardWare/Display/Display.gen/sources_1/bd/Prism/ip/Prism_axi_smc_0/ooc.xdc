# aclk {FREQ_HZ 125000000 CLK_DOMAIN Prism_xdma_0_0_axi_aclk PHASE 0.000} aclk1 {FREQ_HZ 100000000 CLK_DOMAIN Prism_mig_7series_0_0_ui_clk PHASE 0} aclk2 {FREQ_HZ 55312500 CLK_DOMAIN /clk_wiz_clk_out1 PHASE 0.0}
# Clock Domain: Prism_xdma_0_0_axi_aclk
create_clock -name aclk -period 8.000 [get_ports aclk]
# Clock Domain: Prism_mig_7series_0_0_ui_clk
create_clock -name aclk1 -period 10.000 [get_ports aclk1]
# Clock Domain: /clk_wiz_clk_out1
create_clock -name aclk2 -period 18.079 [get_ports aclk2]
# Generated clocks
