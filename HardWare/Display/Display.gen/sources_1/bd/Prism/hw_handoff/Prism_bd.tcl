
################################################################
# This is a generated script based on design: Prism
#
# Though there are limitations about the generated script,
# the main purpose of this utility is to make learning
# IP Integrator Tcl commands easier.
################################################################

namespace eval _tcl {
proc get_script_folder {} {
   set script_path [file normalize [info script]]
   set script_folder [file dirname $script_path]
   return $script_folder
}
}
variable script_folder
set script_folder [_tcl::get_script_folder]

################################################################
# Check if script is running in correct Vivado version.
################################################################
set scripts_vivado_version 2020.2
set current_vivado_version [version -short]

if { [string first $scripts_vivado_version $current_vivado_version] == -1 } {
   puts ""
   catch {common::send_gid_msg -ssname BD::TCL -id 2041 -severity "ERROR" "This script was generated using Vivado <$scripts_vivado_version> and is being run in <$current_vivado_version> of Vivado. Please run the script in Vivado <$scripts_vivado_version> then open the design in Vivado <$current_vivado_version>. Upgrade the design by running \"Tools => Report => Report IP Status...\", then run write_bd_tcl to create an updated script."}

   return 1
}

################################################################
# START
################################################################

# To test this script, run the following commands from Vivado Tcl console:
# source Prism_script.tcl

# If there is no project opened, this script will create a
# project, but make sure you do not have an existing project
# <./myproj/project_1.xpr> in the current working folder.

set list_projs [get_projects -quiet]
if { $list_projs eq "" } {
   create_project project_1 myproj -part xc7k70tfbg676-2
}


# CHANGE DESIGN NAME HERE
variable design_name
set design_name Prism

# If you do not already have an existing IP Integrator design open,
# you can create a design using the following command:
#    create_bd_design $design_name

# Creating design if needed
set errMsg ""
set nRet 0

set cur_design [current_bd_design -quiet]
set list_cells [get_bd_cells -quiet]

if { ${design_name} eq "" } {
   # USE CASES:
   #    1) Design_name not set

   set errMsg "Please set the variable <design_name> to a non-empty value."
   set nRet 1

} elseif { ${cur_design} ne "" && ${list_cells} eq "" } {
   # USE CASES:
   #    2): Current design opened AND is empty AND names same.
   #    3): Current design opened AND is empty AND names diff; design_name NOT in project.
   #    4): Current design opened AND is empty AND names diff; design_name exists in project.

   if { $cur_design ne $design_name } {
      common::send_gid_msg -ssname BD::TCL -id 2001 -severity "INFO" "Changing value of <design_name> from <$design_name> to <$cur_design> since current design is empty."
      set design_name [get_property NAME $cur_design]
   }
   common::send_gid_msg -ssname BD::TCL -id 2002 -severity "INFO" "Constructing design in IPI design <$cur_design>..."

} elseif { ${cur_design} ne "" && $list_cells ne "" && $cur_design eq $design_name } {
   # USE CASES:
   #    5) Current design opened AND has components AND same names.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 1
} elseif { [get_files -quiet ${design_name}.bd] ne "" } {
   # USE CASES: 
   #    6) Current opened design, has components, but diff names, design_name exists in project.
   #    7) No opened design, design_name exists in project.

   set errMsg "Design <$design_name> already exists in your project, please set the variable <design_name> to another value."
   set nRet 2

} else {
   # USE CASES:
   #    8) No opened design, design_name not in project.
   #    9) Current opened design, has components, but diff names, design_name not in project.

   common::send_gid_msg -ssname BD::TCL -id 2003 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

   create_bd_design $design_name

   common::send_gid_msg -ssname BD::TCL -id 2004 -severity "INFO" "Making design <$design_name> as current_bd_design."
   current_bd_design $design_name

}

common::send_gid_msg -ssname BD::TCL -id 2005 -severity "INFO" "Currently the variable <design_name> is equal to \"$design_name\"."

if { $nRet != 0 } {
   catch {common::send_gid_msg -ssname BD::TCL -id 2006 -severity "ERROR" $errMsg}
   return $nRet
}


##################################################################
# MIG PRJ FILE TCL PROCs
##################################################################

proc write_mig_file_Prism_mig_7series_0_0 { str_mig_prj_filepath } {

   file mkdir [ file dirname "$str_mig_prj_filepath" ]
   set mig_prj_file [open $str_mig_prj_filepath  w+]

   puts $mig_prj_file {﻿<?xml version="1.0" encoding="UTF-8" standalone="no" ?>}
   puts $mig_prj_file {<Project NoOfControllers="1">}
   puts $mig_prj_file {  }
   puts $mig_prj_file {<!-- IMPORTANT: This is an internal file that has been generated by the MIG software. Any direct editing or changes made to this file may result in unpredictable behavior or data corruption. It is strongly advised that users do not edit the contents of this file. Re-run the MIG GUI with the required settings if any of the options provided below need to be altered. -->}
   puts $mig_prj_file {  <ModuleName>Prism_mig_7series_0_0</ModuleName>}
   puts $mig_prj_file {  <dci_inouts_inputs>1</dci_inouts_inputs>}
   puts $mig_prj_file {  <dci_inputs>1</dci_inputs>}
   puts $mig_prj_file {  <Debug_En>OFF</Debug_En>}
   puts $mig_prj_file {  <DataDepth_En>1024</DataDepth_En>}
   puts $mig_prj_file {  <LowPower_En>ON</LowPower_En>}
   puts $mig_prj_file {  <XADC_En>Enabled</XADC_En>}
   puts $mig_prj_file {  <TargetFPGA>xc7k70t-fbg676/-2</TargetFPGA>}
   puts $mig_prj_file {  <Version>4.2</Version>}
   puts $mig_prj_file {  <SystemClock>No Buffer</SystemClock>}
   puts $mig_prj_file {  <ReferenceClock>No Buffer</ReferenceClock>}
   puts $mig_prj_file {  <SysResetPolarity>ACTIVE LOW</SysResetPolarity>}
   puts $mig_prj_file {  <BankSelectionFlag>FALSE</BankSelectionFlag>}
   puts $mig_prj_file {  <InternalVref>0</InternalVref>}
   puts $mig_prj_file {  <dci_hr_inouts_inputs>50 Ohms</dci_hr_inouts_inputs>}
   puts $mig_prj_file {  <dci_cascade>0</dci_cascade>}
   puts $mig_prj_file {  <Controller number="0">}
   puts $mig_prj_file {    <MemoryDevice>DDR2_SDRAM/Components/MT47H64M16XX-25</MemoryDevice>}
   puts $mig_prj_file {    <TimePeriod>2500</TimePeriod>}
   puts $mig_prj_file {    <VccAuxIO>1.8V</VccAuxIO>}
   puts $mig_prj_file {    <PHYRatio>4:1</PHYRatio>}
   puts $mig_prj_file {    <InputClkFreq>100</InputClkFreq>}
   puts $mig_prj_file {    <UIExtraClocks>0</UIExtraClocks>}
   puts $mig_prj_file {    <MMCM_VCO>1440</MMCM_VCO>}
   puts $mig_prj_file {    <MMCMClkOut0> 1.000</MMCMClkOut0>}
   puts $mig_prj_file {    <MMCMClkOut1>1</MMCMClkOut1>}
   puts $mig_prj_file {    <MMCMClkOut2>1</MMCMClkOut2>}
   puts $mig_prj_file {    <MMCMClkOut3>1</MMCMClkOut3>}
   puts $mig_prj_file {    <MMCMClkOut4>1</MMCMClkOut4>}
   puts $mig_prj_file {    <DataWidth>32</DataWidth>}
   puts $mig_prj_file {    <DeepMemory>1</DeepMemory>}
   puts $mig_prj_file {    <DataMask>1</DataMask>}
   puts $mig_prj_file {    <ECC>Disabled</ECC>}
   puts $mig_prj_file {    <Ordering>Normal</Ordering>}
   puts $mig_prj_file {    <BankMachineCnt>4</BankMachineCnt>}
   puts $mig_prj_file {    <CustomPart>FALSE</CustomPart>}
   puts $mig_prj_file {    <NewPartName/>}
   puts $mig_prj_file {    <RowAddress>13</RowAddress>}
   puts $mig_prj_file {    <ColAddress>10</ColAddress>}
   puts $mig_prj_file {    <BankAddress>3</BankAddress>}
   puts $mig_prj_file {    <C0_MEM_SIZE>268435456</C0_MEM_SIZE>}
   puts $mig_prj_file {    <UserMemoryAddressMap>BANK_ROW_COLUMN</UserMemoryAddressMap>}
   puts $mig_prj_file {    <PinSelection>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC8" SLEW="" VCCAUX_IO="" name="ddr2_addr[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y8" SLEW="" VCCAUX_IO="" name="ddr2_addr[10]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W9" SLEW="" VCCAUX_IO="" name="ddr2_addr[11]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W10" SLEW="" VCCAUX_IO="" name="ddr2_addr[12]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA7" SLEW="" VCCAUX_IO="" name="ddr2_addr[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA8" SLEW="" VCCAUX_IO="" name="ddr2_addr[2]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AF7" SLEW="" VCCAUX_IO="" name="ddr2_addr[3]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AE7" SLEW="" VCCAUX_IO="" name="ddr2_addr[4]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W8" SLEW="" VCCAUX_IO="" name="ddr2_addr[5]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V9" SLEW="" VCCAUX_IO="" name="ddr2_addr[6]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y10" SLEW="" VCCAUX_IO="" name="ddr2_addr[7]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y11" SLEW="" VCCAUX_IO="" name="ddr2_addr[8]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y7" SLEW="" VCCAUX_IO="" name="ddr2_addr[9]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC7" SLEW="" VCCAUX_IO="" name="ddr2_ba[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB7" SLEW="" VCCAUX_IO="" name="ddr2_ba[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AD8" SLEW="" VCCAUX_IO="" name="ddr2_ba[2]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB9" SLEW="" VCCAUX_IO="" name="ddr2_cas_n"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W11" SLEW="" VCCAUX_IO="" name="ddr2_ck_n[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V7" SLEW="" VCCAUX_IO="" name="ddr2_ck_n[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V11" SLEW="" VCCAUX_IO="" name="ddr2_ck_p[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V8" SLEW="" VCCAUX_IO="" name="ddr2_ck_p[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC12" SLEW="" VCCAUX_IO="" name="ddr2_cke[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AD9" SLEW="" VCCAUX_IO="" name="ddr2_cs_n[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB12" SLEW="" VCCAUX_IO="" name="ddr2_cs_n[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="U6" SLEW="" VCCAUX_IO="" name="ddr2_dm[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y3" SLEW="" VCCAUX_IO="" name="ddr2_dm[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB6" SLEW="" VCCAUX_IO="" name="ddr2_dm[2]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AD4" SLEW="" VCCAUX_IO="" name="ddr2_dm[3]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="U5" SLEW="" VCCAUX_IO="" name="ddr2_dq[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V1" SLEW="" VCCAUX_IO="" name="ddr2_dq[10]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W1" SLEW="" VCCAUX_IO="" name="ddr2_dq[11]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y1" SLEW="" VCCAUX_IO="" name="ddr2_dq[12]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB2" SLEW="" VCCAUX_IO="" name="ddr2_dq[13]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC2" SLEW="" VCCAUX_IO="" name="ddr2_dq[14]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA3" SLEW="" VCCAUX_IO="" name="ddr2_dq[15]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA4" SLEW="" VCCAUX_IO="" name="ddr2_dq[16]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB4" SLEW="" VCCAUX_IO="" name="ddr2_dq[17]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC4" SLEW="" VCCAUX_IO="" name="ddr2_dq[18]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC3" SLEW="" VCCAUX_IO="" name="ddr2_dq[19]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="U2" SLEW="" VCCAUX_IO="" name="ddr2_dq[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC6" SLEW="" VCCAUX_IO="" name="ddr2_dq[20]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y6" SLEW="" VCCAUX_IO="" name="ddr2_dq[21]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y5" SLEW="" VCCAUX_IO="" name="ddr2_dq[22]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AD6" SLEW="" VCCAUX_IO="" name="ddr2_dq[23]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AD1" SLEW="" VCCAUX_IO="" name="ddr2_dq[24]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AE1" SLEW="" VCCAUX_IO="" name="ddr2_dq[25]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AE3" SLEW="" VCCAUX_IO="" name="ddr2_dq[26]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AE2" SLEW="" VCCAUX_IO="" name="ddr2_dq[27]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AE6" SLEW="" VCCAUX_IO="" name="ddr2_dq[28]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AE5" SLEW="" VCCAUX_IO="" name="ddr2_dq[29]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="U1" SLEW="" VCCAUX_IO="" name="ddr2_dq[2]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AF3" SLEW="" VCCAUX_IO="" name="ddr2_dq[30]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AF2" SLEW="" VCCAUX_IO="" name="ddr2_dq[31]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V3" SLEW="" VCCAUX_IO="" name="ddr2_dq[3]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W3" SLEW="" VCCAUX_IO="" name="ddr2_dq[4]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="U7" SLEW="" VCCAUX_IO="" name="ddr2_dq[5]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V6" SLEW="" VCCAUX_IO="" name="ddr2_dq[6]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V4" SLEW="" VCCAUX_IO="" name="ddr2_dq[7]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="Y2" SLEW="" VCCAUX_IO="" name="ddr2_dq[8]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="V2" SLEW="" VCCAUX_IO="" name="ddr2_dq[9]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W5" SLEW="" VCCAUX_IO="" name="ddr2_dqs_n[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC1" SLEW="" VCCAUX_IO="" name="ddr2_dqs_n[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB5" SLEW="" VCCAUX_IO="" name="ddr2_dqs_n[2]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AF4" SLEW="" VCCAUX_IO="" name="ddr2_dqs_n[3]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="W6" SLEW="" VCCAUX_IO="" name="ddr2_dqs_p[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AB1" SLEW="" VCCAUX_IO="" name="ddr2_dqs_p[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA5" SLEW="" VCCAUX_IO="" name="ddr2_dqs_p[2]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AF5" SLEW="" VCCAUX_IO="" name="ddr2_dqs_p[3]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA13" SLEW="" VCCAUX_IO="" name="ddr2_odt[0]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA12" SLEW="" VCCAUX_IO="" name="ddr2_odt[1]"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AA9" SLEW="" VCCAUX_IO="" name="ddr2_ras_n"/>}
   puts $mig_prj_file {      <Pin IN_TERM="" IOSTANDARD="" PADName="AC9" SLEW="" VCCAUX_IO="" name="ddr2_we_n"/>}
   puts $mig_prj_file {    </PinSelection>}
   puts $mig_prj_file {    <System_Control>}
   puts $mig_prj_file {      <Pin Bank="Select Bank" PADName="No connect" name="sys_rst"/>}
   puts $mig_prj_file {      <Pin Bank="Select Bank" PADName="No connect" name="init_calib_complete"/>}
   puts $mig_prj_file {      <Pin Bank="Select Bank" PADName="No connect" name="tg_compare_error"/>}
   puts $mig_prj_file {    </System_Control>}
   puts $mig_prj_file {    <TimingParameters>}
   puts $mig_prj_file {      <Parameters tfaw="45" tras="40" trcd="15" trefi="7.8" trfc="127.5" trp="15" trrd="10" trtp="7.5" twtr="7.5"/>}
   puts $mig_prj_file {    </TimingParameters>}
   puts $mig_prj_file {    <mrBurstLength name="Burst Length">8</mrBurstLength>}
   puts $mig_prj_file {    <mrBurstType name="Burst Type">Sequential</mrBurstType>}
   puts $mig_prj_file {    <mrCasLatency name="CAS Latency">6</mrCasLatency>}
   puts $mig_prj_file {    <mrMode name="Mode">Normal</mrMode>}
   puts $mig_prj_file {    <mrDllReset name="DLL Reset">No</mrDllReset>}
   puts $mig_prj_file {    <mrPdMode name="PD Mode">Fast exit</mrPdMode>}
   puts $mig_prj_file {    <mrWriteRecovery name="Write Recovery">6</mrWriteRecovery>}
   puts $mig_prj_file {    <emrDllEnable name="DLL Enable">Enable-Normal</emrDllEnable>}
   puts $mig_prj_file {    <emrOutputDriveStrength name="Output Drive Strength">Fullstrength</emrOutputDriveStrength>}
   puts $mig_prj_file {    <emrCSSelection name="Controller Chip Select Pin">Enable</emrCSSelection>}
   puts $mig_prj_file {    <emrCKSelection name="Memory Clock Selection">2</emrCKSelection>}
   puts $mig_prj_file {    <emrRTT name="RTT (nominal) - ODT">75ohms</emrRTT>}
   puts $mig_prj_file {    <emrPosted name="Additive Latency (AL)">0</emrPosted>}
   puts $mig_prj_file {    <emrOCD name="OCD Operation">OCD Exit</emrOCD>}
   puts $mig_prj_file {    <emrDQS name="DQS# Enable">Enable</emrDQS>}
   puts $mig_prj_file {    <emrRDQS name="RDQS Enable">Disable</emrRDQS>}
   puts $mig_prj_file {    <emrOutputs name="Outputs">Enable</emrOutputs>}
   puts $mig_prj_file {    <PortInterface>AXI</PortInterface>}
   puts $mig_prj_file {    <AXIParameters>}
   puts $mig_prj_file {      <C0_C_RD_WR_ARB_ALGORITHM>RD_PRI_REG</C0_C_RD_WR_ARB_ALGORITHM>}
   puts $mig_prj_file {      <C0_S_AXI_ADDR_WIDTH>28</C0_S_AXI_ADDR_WIDTH>}
   puts $mig_prj_file {      <C0_S_AXI_DATA_WIDTH>128</C0_S_AXI_DATA_WIDTH>}
   puts $mig_prj_file {      <C0_S_AXI_ID_WIDTH>4</C0_S_AXI_ID_WIDTH>}
   puts $mig_prj_file {      <C0_S_AXI_SUPPORTS_NARROW_BURST>0</C0_S_AXI_SUPPORTS_NARROW_BURST>}
   puts $mig_prj_file {    </AXIParameters>}
   puts $mig_prj_file {  </Controller>}
   puts $mig_prj_file {</Project>}

   close $mig_prj_file
}
# End of write_mig_file_Prism_mig_7series_0_0()



##################################################################
# DESIGN PROCs
##################################################################



# Procedure to create entire design; Provide argument to make
# procedure reusable. If parentCell is "", will use root.
proc create_root_design { parentCell } {

  variable script_folder
  variable design_name

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set DDR2_interface [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:ddrx_rtl:1.0 DDR2_interface ]

  set PICe_Clk [ create_bd_intf_port -mode Slave -vlnv xilinx.com:interface:diff_clock_rtl:1.0 PICe_Clk ]

  set pice_interface [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:pcie_7x_mgt_rtl:1.0 pice_interface ]


  # Create ports
  set Pice_rest [ create_bd_port -dir I -type rst Pice_rest ]
  set_property -dict [ list \
   CONFIG.POLARITY {ACTIVE_LOW} \
 ] $Pice_rest
  set TMDS_Clk_n_0 [ create_bd_port -dir O -type clk TMDS_Clk_n_0 ]
  set TMDS_Clk_p_0 [ create_bd_port -dir O -type clk TMDS_Clk_p_0 ]
  set TMDS_Data_n_0 [ create_bd_port -dir O -from 2 -to 0 TMDS_Data_n_0 ]
  set TMDS_Data_p_0 [ create_bd_port -dir O -from 2 -to 0 TMDS_Data_p_0 ]
  set pice_link_up [ create_bd_port -dir O pice_link_up ]
  set sys_clk [ create_bd_port -dir I -type clk -freq_hz 50000000 sys_clk ]

  # Create instance: axi_smc, and set properties
  set axi_smc [ create_bd_cell -type ip -vlnv xilinx.com:ip:smartconnect:1.0 axi_smc ]
  set_property -dict [ list \
   CONFIG.NUM_CLKS {3} \
   CONFIG.NUM_SI {2} \
 ] $axi_smc

  # Create instance: axi_vdma_0, and set properties
  set axi_vdma_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.3 axi_vdma_0 ]
  set_property -dict [ list \
   CONFIG.c_addr_width {64} \
   CONFIG.c_include_mm2s {1} \
   CONFIG.c_include_s2mm {0} \
   CONFIG.c_m_axi_mm2s_data_width {64} \
   CONFIG.c_m_axis_mm2s_tdata_width {24} \
   CONFIG.c_mm2s_genlock_mode {3} \
   CONFIG.c_mm2s_linebuffer_depth {2048} \
   CONFIG.c_mm2s_max_burst_length {8} \
   CONFIG.c_s2mm_genlock_mode {0} \
 ] $axi_vdma_0

  # Create instance: clk_wiz, and set properties
  set clk_wiz [ create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:6.0 clk_wiz ]
  set_property -dict [ list \
   CONFIG.CLKIN1_JITTER_PS {200.0} \
   CONFIG.CLKOUT1_JITTER {170.846} \
   CONFIG.CLKOUT1_PHASE_ERROR {161.056} \
   CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {55.30} \
   CONFIG.CLKOUT1_USED {true} \
   CONFIG.CLKOUT2_JITTER {125.873} \
   CONFIG.CLKOUT2_PHASE_ERROR {161.056} \
   CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {276.5} \
   CONFIG.CLKOUT2_USED {true} \
   CONFIG.CLKOUT3_JITTER {148.921} \
   CONFIG.CLKOUT3_PHASE_ERROR {161.056} \
   CONFIG.CLKOUT3_REQUESTED_OUT_FREQ {100.00} \
   CONFIG.CLKOUT3_USED {true} \
   CONFIG.CLKOUT4_JITTER {134.378} \
   CONFIG.CLKOUT4_PHASE_ERROR {161.056} \
   CONFIG.CLKOUT4_REQUESTED_OUT_FREQ {200.00} \
   CONFIG.CLKOUT4_USED {true} \
   CONFIG.CLK_OUT1_PORT {clk_55out} \
   CONFIG.CLK_OUT2_PORT {clk_276out} \
   CONFIG.CLK_OUT3_PORT {clk_100out} \
   CONFIG.CLK_OUT4_PORT {clk_200out} \
   CONFIG.MMCM_CLKFBOUT_MULT_F {22.125} \
   CONFIG.MMCM_CLKIN1_PERIOD {20.000} \
   CONFIG.MMCM_CLKIN2_PERIOD {10.0} \
   CONFIG.MMCM_CLKOUT0_DIVIDE_F {20.000} \
   CONFIG.MMCM_CLKOUT1_DIVIDE {4} \
   CONFIG.MMCM_CLKOUT2_DIVIDE {11} \
   CONFIG.MMCM_CLKOUT3_DIVIDE {6} \
   CONFIG.MMCM_DIVCLK_DIVIDE {1} \
   CONFIG.NUM_OUT_CLKS {4} \
   CONFIG.PRIM_IN_FREQ {50.000} \
   CONFIG.RESET_PORT {resetn} \
   CONFIG.RESET_TYPE {ACTIVE_LOW} \
 ] $clk_wiz

  # Create instance: mig_7series_0, and set properties
  set mig_7series_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:mig_7series:4.2 mig_7series_0 ]

  # Generate the PRJ File for MIG
  set str_mig_folder [get_property IP_DIR [ get_ips [ get_property CONFIG.Component_Name $mig_7series_0 ] ] ]
  set str_mig_file_name mig_b.prj
  set str_mig_file_path ${str_mig_folder}/${str_mig_file_name}

  write_mig_file_Prism_mig_7series_0_0 $str_mig_file_path

  set_property -dict [ list \
   CONFIG.BOARD_MIG_PARAM {Custom} \
   CONFIG.MIG_DONT_TOUCH_PARAM {Custom} \
   CONFIG.RESET_BOARD_INTERFACE {Custom} \
   CONFIG.XML_INPUT_FILE {mig_b.prj} \
 ] $mig_7series_0

  # Create instance: rgb2dvi_0, and set properties
  set rgb2dvi_0 [ create_bd_cell -type ip -vlnv digilentinc.com:ip:rgb2dvi:1.4 rgb2dvi_0 ]
  set_property -dict [ list \
   CONFIG.kClkPrimitive {PLL} \
   CONFIG.kGenerateSerialClk {false} \
   CONFIG.kRstActiveHigh {false} \
 ] $rgb2dvi_0

  # Create instance: rst_mig_7series_0_100M, and set properties
  set rst_mig_7series_0_100M [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 rst_mig_7series_0_100M ]

  # Create instance: util_ds_buf_0, and set properties
  set util_ds_buf_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:util_ds_buf:2.1 util_ds_buf_0 ]
  set_property -dict [ list \
   CONFIG.C_BUF_TYPE {IBUFDSGTE} \
 ] $util_ds_buf_0

  # Create instance: v_axi4s_vid_out_0, and set properties
  set v_axi4s_vid_out_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_axi4s_vid_out:4.0 v_axi4s_vid_out_0 ]
  set_property -dict [ list \
   CONFIG.C_ADDR_WIDTH {13} \
   CONFIG.C_S_AXIS_VIDEO_DATA_WIDTH {8} \
   CONFIG.C_S_AXIS_VIDEO_FORMAT {6} \
 ] $v_axi4s_vid_out_0

  # Create instance: v_tc_0, and set properties
  set v_tc_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:v_tc:6.2 v_tc_0 ]
  set_property -dict [ list \
   CONFIG.GEN_F0_VSYNC_HSTART {695} \
   CONFIG.HAS_AXI4_LITE {false} \
   CONFIG.VIDEO_MODE {720p} \
   CONFIG.enable_detection {false} \
   CONFIG.frame_syncs {3} \
   CONFIG.max_clocks_per_line {2048} \
   CONFIG.max_lines_per_frame {2048} \
 ] $v_tc_0

  # Create instance: xdma_0_axi_periph, and set properties
  set xdma_0_axi_periph [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 xdma_0_axi_periph ]
  set_property -dict [ list \
   CONFIG.NUM_MI {1} \
   CONFIG.NUM_SI {1} \
 ] $xdma_0_axi_periph

  # Create instance: xdma_1, and set properties
  set xdma_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xdma:4.1 xdma_1 ]
  set_property -dict [ list \
   CONFIG.PF0_DEVICE_ID_mqdma {9024} \
   CONFIG.PF2_DEVICE_ID_mqdma {9024} \
   CONFIG.PF3_DEVICE_ID_mqdma {9024} \
   CONFIG.axi_data_width {128_bit} \
   CONFIG.axilite_master_en {true} \
   CONFIG.axilite_master_scale {Kilobytes} \
   CONFIG.axilite_master_size {64} \
   CONFIG.axist_bypass_en {true} \
   CONFIG.axist_bypass_size {256} \
   CONFIG.axisten_freq {125} \
   CONFIG.cfg_mgmt_if {false} \
   CONFIG.mode_selection {Advanced} \
   CONFIG.pciebar2axibar_axil_master {0x80000000} \
   CONFIG.pf0_device_id {D325} \
   CONFIG.pf0_msix_cap_pba_bir {BAR_1} \
   CONFIG.pf0_msix_cap_table_bir {BAR_1} \
   CONFIG.pf0_revision_id {01} \
   CONFIG.pf0_subsystem_id {D325} \
   CONFIG.pf0_subsystem_vendor_id {1754} \
   CONFIG.pl_link_cap_max_link_speed {5.0_GT/s} \
   CONFIG.pl_link_cap_max_link_width {X4} \
   CONFIG.plltype {QPLL1} \
   CONFIG.vendor_id {1754} \
   CONFIG.xdma_rnum_chnl {2} \
   CONFIG.xdma_wnum_chnl {2} \
 ] $xdma_1

  # Create instance: xlconcat_0, and set properties
  set xlconcat_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0 ]
  set_property -dict [ list \
   CONFIG.IN0_WIDTH {8} \
   CONFIG.IN1_WIDTH {8} \
   CONFIG.IN2_WIDTH {8} \
   CONFIG.NUM_PORTS {3} \
 ] $xlconcat_0

  # Create instance: xlslice_0, and set properties
  set xlslice_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_0 ]
  set_property -dict [ list \
   CONFIG.DIN_FROM {23} \
   CONFIG.DIN_TO {16} \
   CONFIG.DOUT_WIDTH {8} \
 ] $xlslice_0

  # Create instance: xlslice_1, and set properties
  set xlslice_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_1 ]
  set_property -dict [ list \
   CONFIG.DIN_FROM {15} \
   CONFIG.DIN_TO {8} \
   CONFIG.DOUT_WIDTH {8} \
 ] $xlslice_1

  # Create instance: xlslice_2, and set properties
  set xlslice_2 [ create_bd_cell -type ip -vlnv xilinx.com:ip:xlslice:1.0 xlslice_2 ]
  set_property -dict [ list \
   CONFIG.DIN_FROM {7} \
   CONFIG.DOUT_WIDTH {8} \
 ] $xlslice_2

  # Create interface connections
  connect_bd_intf_net -intf_net CLK_IN_D_0_1 [get_bd_intf_ports PICe_Clk] [get_bd_intf_pins util_ds_buf_0/CLK_IN_D]
  connect_bd_intf_net -intf_net axi_smc_M00_AXI [get_bd_intf_pins axi_smc/M00_AXI] [get_bd_intf_pins mig_7series_0/S_AXI]
  connect_bd_intf_net -intf_net axi_vdma_0_M_AXIS_MM2S [get_bd_intf_pins axi_vdma_0/M_AXIS_MM2S] [get_bd_intf_pins v_axi4s_vid_out_0/video_in]
  connect_bd_intf_net -intf_net axi_vdma_0_M_AXI_MM2S [get_bd_intf_pins axi_smc/S01_AXI] [get_bd_intf_pins axi_vdma_0/M_AXI_MM2S]
  connect_bd_intf_net -intf_net mig_7series_0_DDR2 [get_bd_intf_ports DDR2_interface] [get_bd_intf_pins mig_7series_0/DDR2]
  connect_bd_intf_net -intf_net v_tc_0_vtiming_out [get_bd_intf_pins v_axi4s_vid_out_0/vtiming_in] [get_bd_intf_pins v_tc_0/vtiming_out]
  connect_bd_intf_net -intf_net xdma_0_axi_periph_M00_AXI [get_bd_intf_pins axi_vdma_0/S_AXI_LITE] [get_bd_intf_pins xdma_0_axi_periph/M00_AXI]
  connect_bd_intf_net -intf_net xdma_1_M_AXI_BYPASS [get_bd_intf_pins axi_smc/S00_AXI] [get_bd_intf_pins xdma_1/M_AXI_BYPASS]
  connect_bd_intf_net -intf_net xdma_1_M_AXI_LITE [get_bd_intf_pins xdma_0_axi_periph/S00_AXI] [get_bd_intf_pins xdma_1/M_AXI_LITE]
  connect_bd_intf_net -intf_net xdma_1_pcie_mgt [get_bd_intf_ports pice_interface] [get_bd_intf_pins xdma_1/pcie_mgt]

  # Create port connections
  connect_bd_net -net Pice_rest_1 [get_bd_ports Pice_rest] [get_bd_pins clk_wiz/resetn] [get_bd_pins xdma_1/sys_rst_n]
  connect_bd_net -net clk_in1_0_1 [get_bd_ports sys_clk] [get_bd_pins clk_wiz/clk_in1]
  connect_bd_net -net clk_wiz_clk_100out [get_bd_pins clk_wiz/clk_100out] [get_bd_pins mig_7series_0/sys_clk_i]
  connect_bd_net -net clk_wiz_clk_200out [get_bd_pins clk_wiz/clk_200out] [get_bd_pins mig_7series_0/clk_ref_i]
  connect_bd_net -net clk_wiz_clk_276out [get_bd_pins clk_wiz/clk_276out] [get_bd_pins rgb2dvi_0/SerialClk]
  connect_bd_net -net clk_wiz_clk_55out [get_bd_pins axi_smc/aclk2] [get_bd_pins axi_vdma_0/m_axi_mm2s_aclk] [get_bd_pins axi_vdma_0/m_axis_mm2s_aclk] [get_bd_pins axi_vdma_0/s_axi_lite_aclk] [get_bd_pins clk_wiz/clk_55out] [get_bd_pins rgb2dvi_0/PixelClk] [get_bd_pins v_axi4s_vid_out_0/aclk] [get_bd_pins v_tc_0/clk] [get_bd_pins xdma_0_axi_periph/ACLK] [get_bd_pins xdma_0_axi_periph/M00_ACLK]
  connect_bd_net -net clk_wiz_locked [get_bd_pins axi_vdma_0/axi_resetn] [get_bd_pins clk_wiz/locked] [get_bd_pins mig_7series_0/sys_rst] [get_bd_pins rgb2dvi_0/aRst_n] [get_bd_pins v_axi4s_vid_out_0/aclken] [get_bd_pins v_axi4s_vid_out_0/aresetn] [get_bd_pins v_axi4s_vid_out_0/vid_io_out_ce] [get_bd_pins v_tc_0/clken] [get_bd_pins v_tc_0/resetn] [get_bd_pins xdma_0_axi_periph/ARESETN] [get_bd_pins xdma_0_axi_periph/M00_ARESETN]
  connect_bd_net -net mig_7series_0_mmcm_locked [get_bd_pins mig_7series_0/mmcm_locked] [get_bd_pins rst_mig_7series_0_100M/dcm_locked]
  connect_bd_net -net mig_7series_0_ui_clk [get_bd_pins axi_smc/aclk1] [get_bd_pins mig_7series_0/ui_clk] [get_bd_pins rst_mig_7series_0_100M/slowest_sync_clk]
  connect_bd_net -net mig_7series_0_ui_clk_sync_rst [get_bd_pins mig_7series_0/ui_clk_sync_rst] [get_bd_pins rst_mig_7series_0_100M/ext_reset_in]
  connect_bd_net -net rgb2dvi_0_TMDS_Clk_n [get_bd_ports TMDS_Clk_n_0] [get_bd_pins rgb2dvi_0/TMDS_Clk_n]
  connect_bd_net -net rgb2dvi_0_TMDS_Clk_p [get_bd_ports TMDS_Clk_p_0] [get_bd_pins rgb2dvi_0/TMDS_Clk_p]
  connect_bd_net -net rgb2dvi_0_TMDS_Data_n [get_bd_ports TMDS_Data_n_0] [get_bd_pins rgb2dvi_0/TMDS_Data_n]
  connect_bd_net -net rgb2dvi_0_TMDS_Data_p [get_bd_ports TMDS_Data_p_0] [get_bd_pins rgb2dvi_0/TMDS_Data_p]
  connect_bd_net -net rst_mig_7series_0_100M_peripheral_aresetn [get_bd_pins mig_7series_0/aresetn] [get_bd_pins rst_mig_7series_0_100M/peripheral_aresetn]
  connect_bd_net -net util_ds_buf_0_IBUF_OUT [get_bd_pins util_ds_buf_0/IBUF_OUT] [get_bd_pins xdma_1/sys_clk]
  connect_bd_net -net v_axi4s_vid_out_0_vid_active_video [get_bd_pins rgb2dvi_0/vid_pVDE] [get_bd_pins v_axi4s_vid_out_0/vid_active_video]
  connect_bd_net -net v_axi4s_vid_out_0_vid_data [get_bd_pins v_axi4s_vid_out_0/vid_data] [get_bd_pins xlslice_0/Din] [get_bd_pins xlslice_1/Din] [get_bd_pins xlslice_2/Din]
  connect_bd_net -net v_axi4s_vid_out_0_vid_hsync [get_bd_pins rgb2dvi_0/vid_pHSync] [get_bd_pins v_axi4s_vid_out_0/vid_hsync]
  connect_bd_net -net v_axi4s_vid_out_0_vid_vsync [get_bd_pins rgb2dvi_0/vid_pVSync] [get_bd_pins v_axi4s_vid_out_0/vid_vsync]
  connect_bd_net -net v_axi4s_vid_out_0_vtg_ce [get_bd_pins v_axi4s_vid_out_0/vtg_ce] [get_bd_pins v_tc_0/gen_clken]
  connect_bd_net -net xdma_1_axi_aclk [get_bd_pins axi_smc/aclk] [get_bd_pins xdma_0_axi_periph/S00_ACLK] [get_bd_pins xdma_1/axi_aclk]
  connect_bd_net -net xdma_1_axi_aresetn [get_bd_pins axi_smc/aresetn] [get_bd_pins xdma_0_axi_periph/S00_ARESETN] [get_bd_pins xdma_1/axi_aresetn]
  connect_bd_net -net xdma_1_user_lnk_up [get_bd_ports pice_link_up] [get_bd_pins xdma_1/user_lnk_up]
  connect_bd_net -net xlconcat_0_dout [get_bd_pins rgb2dvi_0/vid_pData] [get_bd_pins xlconcat_0/dout]
  connect_bd_net -net xlslice_0_Dout [get_bd_pins xlconcat_0/In0] [get_bd_pins xlslice_0/Dout]
  connect_bd_net -net xlslice_1_Dout [get_bd_pins xlconcat_0/In1] [get_bd_pins xlslice_1/Dout]
  connect_bd_net -net xlslice_2_Dout [get_bd_pins xlconcat_0/In2] [get_bd_pins xlslice_2/Dout]

  # Create address segments
  assign_bd_address -offset 0x00000000 -range 0x10000000 -target_address_space [get_bd_addr_spaces axi_vdma_0/Data_MM2S] [get_bd_addr_segs mig_7series_0/memmap/memaddr] -force
  assign_bd_address -offset 0x80000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces xdma_1/M_AXI_LITE] [get_bd_addr_segs axi_vdma_0/S_AXI_LITE/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x10000000 -target_address_space [get_bd_addr_spaces xdma_1/M_AXI_BYPASS] [get_bd_addr_segs mig_7series_0/memmap/memaddr] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
}
# End of create_root_design()


##################################################################
# MAIN FLOW
##################################################################

create_root_design ""


