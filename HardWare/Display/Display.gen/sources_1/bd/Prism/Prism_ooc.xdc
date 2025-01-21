################################################################################

# This XDC is used only for OOC mode of synthesis, implementation
# This constraints file contains default clock frequencies to be used during
# out-of-context flows such as OOC Synthesis and Hierarchical Designs.
# This constraints file is not used in normal top-down synthesis (default flow
# of Vivado)
################################################################################
create_clock -name sys_clk -period 20 [get_ports sys_clk]
create_clock -name PICe_Clk_clk_p -period 10 [get_ports PICe_Clk_clk_p]

################################################################################