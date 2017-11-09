
# PlanAhead Launch Script for Post-Synthesis pin planning, created by Project Navigator

create_project -name top -dir "D:/Workstation/Workspace/FPGA/Changed/12_cvbs_display/par_xilinx/planAhead_run_3" -part xc6slx45fgg484-2
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "D:/Workstation/Workspace/FPGA/Changed/12_cvbs_display/par_xilinx/top.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {D:/Workstation/Workspace/FPGA/Changed/12_cvbs_display/par_xilinx} {ipcore_dir} {../src/xilinx} {../src/xilinx/fifo} }
add_files [list {ipcore_dir/cvbs_fifo.ncf}] -fileset [get_property constrset [current_run]]
set_param project.pinAheadLayout  yes
set_property target_constrs_file "top.ucf" [current_fileset -constrset]
add_files [list {top.ucf}] -fileset [get_property constrset [current_run]]
link_design
