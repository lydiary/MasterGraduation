module top(
	//key input
	input sys_key0,

	//led output
	output [3:0] led,
	
	//i2c
	input scl,
	inout sda,	
		
	//hdmi output
	output hdmi_out_clk,
	output hdmi_out_hs,
	output hdmi_out_vs,
	output hdmi_out_de,
	output[7:0]  hdmi_out_rgb_b,
	output[7:0]  hdmi_out_rgb_g,
	output[7:0]  hdmi_out_rgb_r,
	
	//vga output
	output vga_out_clk,         
	output vga_out_hs,          
	output vga_out_vs,          
	output vga_out_de,          
	output[11:0]  vga_out_data,
	
	//cvbs input
	input cvbs_in_clkp,
	input cvbs_in_clkn,
	input[7:0] cvbs_in_data,
	
	
	//ddr3
`ifdef Xilinx	
	inout  [15:0]             mcb3_dram_dq,
	output [13:0]             mcb3_dram_a,
	output [2:0]              mcb3_dram_ba,
	output                    mcb3_dram_ras_n,
	output                    mcb3_dram_cas_n,
	output                    mcb3_dram_we_n,
	output                    mcb3_dram_odt,
	output                    mcb3_dram_reset_n,
	output                    mcb3_dram_cke,
	output                    mcb3_dram_dm,
	inout                     mcb3_dram_udqs,
	inout                     mcb3_dram_udqs_n,
	inout                     mcb3_rzq,
	inout                     mcb3_zio,
	output                    mcb3_dram_udm,
	inout                     mcb3_dram_dqs,
	inout                     mcb3_dram_dqs_n,
	output                    mcb3_dram_ck,
	output                    mcb3_dram_ck_n,
`else
	output  wire[0 : 0]  mem_cs_n,
	output  wire[0 : 0]  mem_cke,
	output  wire[12: 0]  mem_addr,
	output  wire[2 : 0]  mem_ba,
	output  wire  mem_ras_n,
	output  wire  mem_cas_n,
	output  wire  mem_we_n,
	inout  wire[0 : 0]  mem_clk,
	inout  wire[0 : 0]  mem_clk_n,
	output  wire[3 : 0]  mem_dm,
	inout  wire[31: 0]  mem_dq,
	inout  wire[3 : 0]  mem_dqs,
	output[0:0]	mem_odt,
`endif	
	//clock input
	input clk_50m,
	input clk_27m,
	
	
	
	//ǧ����̫���ӿ��ź�	
	output               e_reset,	
	//output               e_mdc,                      //MDIO��ʱ���źţ����ڶ�дPHY�ļĴ���
	//inout                e_mdio,                     //MDIO�������źţ����ڶ�дPHY�ļĴ���	
		
	input	               e_rxc,                      //125Mhz ethernet gmii rx clock
	input	               e_rxdv,	                   //GMII ����������Ч�ź�
	input	               e_rxer,	                   //GMII �������ݴ����ź�					
	input      [7:0]     e_rxd,                      //GMII ��������	      

	input	               e_txc,                      //25Mhz ethernet mii tx clock         
	output               e_gtxc,                     //25Mhz ethernet gmii tx clock  
	output               e_txen,                     //GMII ����������Ч�ź�	
	output               e_txer,                     //GMII �������ݴ����ź�					
	output     [7:0]     e_txd	                      //GMII �������� 	
);
assign sda = 1'bz;
assign led = 4'd2;

parameter H_ACTIVE = 16'd1920;
parameter H_FP = 16'd88;
parameter H_SYNC = 16'd44;
parameter H_BP = 16'd148; 
parameter V_ACTIVE = 16'd1080;
parameter V_FP 	= 16'd4;
parameter V_SYNC  = 16'd5;
parameter V_BP	= 16'd36;


parameter H_TOTAL = H_ACTIVE + H_FP + H_SYNC + H_BP;
parameter V_TOTAL = V_ACTIVE + V_FP + V_SYNC + V_BP;
parameter VCH_NUM = 1;
parameter CH0 = 1;
parameter MEM_DATA_BITS = 64;


wire rst_n;
wire phy_clk;
wire ch0_rd_burst_req;
wire[9:0] ch0_rd_burst_len;
wire[23:0] ch0_rd_burst_addr;
wire  ch0_rd_burst_data_valid;
wire[63:0] ch0_rd_burst_data;
wire ch0_rd_burst_finish;

wire ch0_wr_burst_req;
wire[9:0] ch0_wr_burst_len;
wire[23:0] ch0_wr_burst_addr;
wire ch0_wr_burst_data_req;
wire[63:0] ch0_wr_burst_data;
wire ch0_wr_burst_finish;

wire[VCH_NUM -  1 :  0] is_pal;

wire[16 * VCH_NUM -  1 :  0] vin_s_width;
wire[16 * VCH_NUM -  1 :  0] vin_s_height;
wire[16 * VCH_NUM -  1 :  0] clipper_left;
wire[16 * VCH_NUM -  1 :  0] clipper_width;
wire[16 * VCH_NUM -  1 :  0] clipper_top;
wire[16 * VCH_NUM -  1 :  0] clipper_height;
wire[16 * VCH_NUM -  1 :  0] vin_t_width;
wire[16 * VCH_NUM -  1 :  0] vin_t_height;
wire[16 * VCH_NUM -  1 :  0] vin_K_h;
wire[16 * VCH_NUM -  1 :  0] vin_K_v;
wire[16 * VCH_NUM -  1 :  0] vout_t_width;
wire[16 * VCH_NUM -  1 :  0] vout_t_height;
wire[16 * VCH_NUM -  1 :  0] vout_K_h;
wire[16 * VCH_NUM -  1 :  0] vout_K_v;
wire[ 8 * VCH_NUM -  1 :  0] alpha;
wire[16 * VCH_NUM -  1 :  0] disp_top;
wire[16 * VCH_NUM -  1 :  0] disp_left;
	
wire video_clk;
wire cvbs_27m;

wire ch0_de;
wire ch0_vs;
wire[15:0] ch0_yc_data;
wire ch0_f;

//CVBS����ͷ�ɼ������õļĴ�����TW2867֧��4ͨ��������ͷ���룬
//ÿ��ͨ����ʱ��Ƶ��Ϊ27M��������һ��108M��ʱ����ʵ��4ͨ��������������������������
wire[7:0] cvbs_data_ch0;
wire[7:0] cvbs_data_ch1;
wire[7:0] cvbs_data_ch2;
wire[7:0] cvbs_data_ch3;
reg[7:0] vin_ch0_data;

wire tw2867_108m;
wire tw2867_27m;

wire video_hs;
wire video_vs;
wire video_de;
wire[7:0] video_r;
wire[7:0] video_g;
wire[7:0] video_b;

//VGA���ʹ�õ��ź�
wire vga_hs;
wire vga_vs;
wire vga_de;
wire[7:0] vga_r;
wire[7:0] vga_g;
wire[7:0] vga_b;

//HDMI���ʹ�õ��ź�
wire hdmi_hs;
wire hdmi_vs;
wire hdmi_de;
wire[7:0] hdmi_r;
wire[7:0] hdmi_g;
wire[7:0] hdmi_b;

assign vin_s_width[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd720;											//����ͼ��Ŀ��
assign vin_s_height[16 * CH0 - 5 : 16 * CH0 - 16] = is_pal[CH0-1] ? 12'd576 : 12'd480;		//����ͼ��ĸ߶ȣ�PAL��ʽΪ576,����Ϊ480
assign clipper_left[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd0;					
assign clipper_width[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd720;
assign clipper_top[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd0;
assign clipper_height[16 * CH0 - 5 : 16 * CH0 - 16] = is_pal[CH0-1] ? 12'd576 : 12'd480;	
assign vout_t_width[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd1920;										//���ͼ��Ŀ��
assign vout_t_height[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd1080;										//���ͼ��ĸ߶�
assign vout_K_h[16 * CH0 - 1 : 16 * CH0 - 16] = 16'h0060;
assign vout_K_v[16 * CH0 - 1 : 16 * CH0 - 16] = is_pal[CH0-1] ? 16'h0088 : 16'h0084;
assign disp_top[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd0;
assign disp_left[16 * CH0 - 5 : 16 * CH0 - 16] = 12'd0;
assign alpha[8 * CH0 - 1 : 8 * CH0 - 8] = 8'hff;

//�ϵ��ӳ�һ��ʱ��
reset reset_m0(
	.clk(video_clk),
	.rst_n(rst_n)
);

//����ʱ��
`ifdef Xilinx
pll pll_m0(
	.inclk0(clk_27m),
	.c0(sys_clk),
	.c1(video_clk));		//video_clk = 148.5M
`else
pll pll_m0(
	.inclk0(clk_50m),
	.c0(sys_clk),
	.c1(video_clk));
`endif

//����hdmi���ʱ��
clock_out clock_out_m0
(
	.clk_in(video_clk),
	.clk_out(hdmi_out_clk)
);

//�������ʱ��tw2867_108m
clock_in clock_in_clkp_buf
(
	.clk_in(cvbs_in_clkp),
	.clk_out(tw2867_108m)
);
//�������ʱ��tw2867_27m
clock_in clock_in_clkn_buf
(
	.clk_in(cvbs_in_clkn),
	.clk_out(tw2867_27m)
);

wire DB_key;
reg DB_key_d0;
reg[2:0] video_sel_cnt;
//��������
DeBounce DeBounce_m0
(
	.clk(tw2867_27m), 
	.n_reset(1'b1), 
	.button_in(sys_key0),
	.DB_out(DB_key)
);	

//��ȡ���������룬ÿ��һ�Σ��ı�һ��ͼ��ɼ���ͨ��
always@(posedge tw2867_27m)
begin
	DB_key_d0 <= DB_key;
	if(DB_key_d0 && ~DB_key)
		video_sel_cnt <= (video_sel_cnt == 3'd4) ? 3'd0 : video_sel_cnt + 3'd1;
end

//�⸴�òɼ�������ͷ���ݣ�tw2867֧��4ͨ���������ʽΪBT656			
demux demux_m0(
	.clk_108m(tw2867_108m),
	.clk_27m(tw2867_27m),
	.vin_data(cvbs_in_data),
	.vout_data_ch0(cvbs_data_ch0),
	.vout_data_ch1(cvbs_data_ch1),
	.vout_data_ch2(cvbs_data_ch2),
	.vout_data_ch3(cvbs_data_ch3)
);


wire[7:0] pat_data;			//���������BT656��ʽ��ͼ������

//��������ʱ��ʾ�Ĳ�������ͷ�ɼ�����ͼ����Ϣ�������Լ������ġ���С��720*576
sd_source sd_source_m0(
	.MODE_SELECT_8B(8'd0),
	.clk_in(tw2867_27m),
						
	.o_itu_656_clk(),
	.o_itu_656_data_8b(pat_data)
);

//ѡ��ʹ����һ��ͨ���ĳ��򣬻���ѡ����������ͼ��						
always@(posedge tw2867_27m)
begin
	case(video_sel_cnt)
		3'd1: vin_ch0_data <= cvbs_data_ch0;		//��ǰ��ͼ��Ϊ����ͷ��һͨ����ͼ��
		3'd2: vin_ch0_data <= cvbs_data_ch1;		//��ǰ��ͼ��Ϊ����ͷ�ڶ�ͨ����ͼ��
		3'd3: vin_ch0_data <= cvbs_data_ch2;		//��ǰ��ͼ��Ϊ����ͷ����ͨ����ͼ��
		3'd4: vin_ch0_data <= cvbs_data_ch3;		//��ǰ��ͼ��Ϊ����ͷ����ͨ����ͼ��
		default:vin_ch0_data <= pat_data;			//��ǰ��ͼ��Ϊ���������ͼ��
	endcase
end	

//����ɼ�����ͼ����Ϊ��ģ������ͷ���ɼ�����ͼ���ʽ��BT6565��ʽ�ġ�			
bt656_decode bt656_decode_m0(
	.clk(tw2867_27m),					/*ʱ�ӣ�27Mhz*/
	.bt656_in(vin_ch0_data),		/*BT656����*/	//����ͷ��ǰ��ͼ��
	.yc_data_out(ch0_yc_data),    /*YC�������*/
	.vs(ch0_vs),                  /*��ͬ�����*/
	.hs(),                        /*��ͬ�����*/
	.field(ch0_f),                /*��ż����־*/
	.de(ch0_de),                  /*������Ч*/
	.is_pal(is_pal[CH0-1])        /*PAL��־*/
);

wire ch0_vout_rd_req;
wire[23:0] ch0_vout_ycbcr;
reg ch0_wr_burst_data_req_d0;
//��ͼ��ͨ��FIFO��Ȼ��д��DDR3�С�ch0_vout_ycbcrΪ��DDR3�ж�����ͼ�����ݡ�
video_pro video_pro_m0(
	.rst_n(1'b1),
	.vin_pixel_clk(tw2867_27m),
	.vin_vs(ch0_vs),
	.vin_f(ch0_f),
	.vin_pixel_de(ch0_de),
	.vin_pixel_yc(ch0_yc_data),
	.vin_s_width(vin_s_width[16 * CH0 - 5 : 16 * CH0 - 16]),
	.vin_s_height(vin_s_height[16 * CH0 - 5 : 16 * CH0 - 16]),
	.clipper_left(clipper_left[16 * CH0 - 5 : 16 * CH0 - 16]),
	.clipper_width(clipper_width[16 * CH0 - 5 : 16 * CH0 - 16]),
	.clipper_top(clipper_top[16 * CH0 - 5 : 16 * CH0 - 16]),
	.clipper_height(clipper_height[16 * CH0 - 5 : 16 * CH0 - 16]),
	.vout_pixel_clk(video_clk),
	.vout_vs(hdmi_out_vs),
	.vout_pixel_rd_req(ch0_vout_rd_req),
	.vout_pixel_ycbcr(ch0_vout_ycbcr),	//���ͼ������
	.vout_scaler_clk(video_clk),
	.vout_t_width(vout_t_width[16 * CH0 - 5 : 16 * CH0 - 16]),
	.vout_t_height(vout_t_height[16 * CH0 - 5 : 16 * CH0 - 16]),
	.vout_K_h(vout_K_h[16 * CH0 - 1 : 16 * CH0 - 16]),
	.vout_K_v(vout_K_v[16 * CH0 - 1 : 16 * CH0 - 16]),
	.mem_clk(phy_clk),
	.wr_burst_req(ch0_wr_burst_req),
	.wr_burst_len(ch0_wr_burst_len),
	.wr_burst_addr(ch0_wr_burst_addr),
	.wr_burst_data_req(ch0_wr_burst_data_req),
	.wr_burst_data(ch0_wr_burst_data),
	.wr_burst_finish(ch0_wr_burst_finish),
	.rd_burst_req(ch0_rd_burst_req),
	.rd_burst_len(ch0_rd_burst_len),
	.rd_burst_addr(ch0_rd_burst_addr),
	.rd_burst_data_valid(ch0_rd_burst_data_valid),
	.rd_burst_data(ch0_rd_burst_data),
	.rd_burst_finish(ch0_rd_burst_finish),
	.base_addr(2'd0)
);
defparam
	video_pro_m0.MEM_DATA_BITS = MEM_DATA_BITS;

//�����DDR3�ж���������ch0_vout_ycbcr��
vout_display_pro vout_display_pro_m0(
	.rst_n(rst_n),
	.dp_clk(video_clk),
	.h_fp(H_FP[11:0]),
	.h_sync(H_SYNC[11:0]),
	.h_bp(H_BP[11:0]),
	.h_active(H_ACTIVE[11:0]),
	.h_total(H_TOTAL[11:0]),
	
	.v_fp(V_FP[11:0]),
	.v_sync(V_SYNC[11:0]),
	.v_bp(V_BP[11:0]), 
	.v_active(V_ACTIVE[11:0]),
	.v_total(V_TOTAL[11:0]),
	
	.hs(video_hs),
	.vs(video_vs),
	.de(video_de),
	
	.rgb_r(video_r),
	.rgb_g(video_g),
	.rgb_b(video_b),
	
	.layer0_top(disp_top[16 * CH0 - 5 : 16 * CH0 - 16]),
	.layer0_left(disp_left[16 * CH0 - 5 : 16 * CH0 - 16]),
	.layer0_width(vout_t_width[16 * CH0 - 5 : 16 * CH0 - 16]),
	.layer0_height(vout_t_height[16 * CH0 - 5 : 16 * CH0 - 16]),
	.layer0_alpha(alpha[8 * CH0 - 1 : 8 * CH0 - 8]),
	.layer0_rdreq(ch0_vout_rd_req),
	.layer0_ycbcr(ch0_vout_ycbcr)
);

//DDR3ģ��		
mem_ctrl
#(
	.MEM_DATA_BITS(MEM_DATA_BITS)
)
mem_ctrl_m0(
	.rst_n(rst_n),
	.source_clk(clk_50m),
	.phy_clk(phy_clk),
	.ch0_rd_burst_req(ch0_rd_burst_req),
	.ch0_rd_burst_len(ch0_rd_burst_len),
	.ch0_rd_burst_addr(ch0_rd_burst_addr),
	.ch0_rd_burst_data_valid(ch0_rd_burst_data_valid),
	.ch0_rd_burst_data(ch0_rd_burst_data),
	.ch0_rd_burst_finish(ch0_rd_burst_finish),
		   
	.ch0_wr_burst_req(ch0_wr_burst_req),
	.ch0_wr_burst_len(ch0_wr_burst_len),
	.ch0_wr_burst_addr(ch0_wr_burst_addr),
	.ch0_wr_burst_data_req(ch0_wr_burst_data_req),
	.ch0_wr_burst_data(ch0_wr_burst_data),
	.ch0_wr_burst_finish(ch0_wr_burst_finish),
	
`ifdef Xilinx	
	.mcb3_dram_dq         (mcb3_dram_dq       ),
	.mcb3_dram_a          (mcb3_dram_a        ),
	.mcb3_dram_ba         (mcb3_dram_ba       ),
	.mcb3_dram_ras_n      (mcb3_dram_ras_n    ),
	.mcb3_dram_cas_n      (mcb3_dram_cas_n    ),
	.mcb3_dram_we_n       (mcb3_dram_we_n     ),
	.mcb3_dram_odt        (mcb3_dram_odt      ),
	.mcb3_dram_reset_n    (mcb3_dram_reset_n  ),
	.mcb3_dram_cke        (mcb3_dram_cke      ),
	.mcb3_dram_dm         (mcb3_dram_dm       ),
	.mcb3_dram_udqs       (mcb3_dram_udqs     ),
	.mcb3_dram_udqs_n     (mcb3_dram_udqs_n   ),
	.mcb3_rzq             (mcb3_rzq           ),
	.mcb3_zio             (mcb3_zio           ),
	.mcb3_dram_udm        (mcb3_dram_udm      ),
	.mcb3_dram_dqs        (mcb3_dram_dqs      ),
	.mcb3_dram_dqs_n      (mcb3_dram_dqs_n    ),
	.mcb3_dram_ck         (mcb3_dram_ck       ),
	.mcb3_dram_ck_n       (mcb3_dram_ck_n     )
`else
	.mem_cs_n(mem_cs_n),
	.mem_cke(mem_cke),
	.mem_addr(mem_addr),
	.mem_ba(mem_ba),
	.mem_ras_n(mem_ras_n),
	.mem_cas_n(mem_cas_n),
	.mem_we_n(mem_we_n),
	.mem_clk(mem_clk),
	.mem_clk_n(mem_clk_n),
	.mem_dm(mem_dm),
	.mem_dq(mem_dq),
	.mem_dqs(mem_dqs),
	.mem_odt(mem_odt)
`endif
);

common_std_logic_vector_delay#
(
	.WIDTH(27),
	.DELAY(1)
)
common_std_logic_vector_delay_m0
(
	.clock(video_clk),
	.reset(1'b0),
	.ena(1'b1),
	.data({video_hs,video_vs,video_de,video_r,video_g,video_b}),
	.q({hdmi_out_hs,hdmi_out_vs,hdmi_out_de,hdmi_out_rgb_r,hdmi_out_rgb_g,hdmi_out_rgb_b})
);

common_std_logic_vector_delay#
(
	.WIDTH(27),
	.DELAY(1)
)
common_std_logic_vector_delay_m1
(
	.clock(video_clk),
	.reset(1'b0),
	.ena(1'b1),
	.data({video_hs,video_vs,video_de,video_r,video_g,video_b}),
	.q({vga_hs,vga_vs,vga_de,vga_r,vga_g,vga_b})
);

vga_out_io vga_out_io_m0
(
	.vga_clk      (video_clk    ),
	.vga_hs       (vga_hs       ),
	.vga_vs       (vga_vs       ),
	.vga_de       (vga_de       ),
	.vga_rgb      ({vga_r,vga_g,vga_b}),
	.vga_out_clk  (vga_out_clk  ),
	.vga_out_hs   (vga_out_hs   ),
	.vga_out_de   (vga_out_de   ),
	.vga_out_vs   (vga_out_vs   ),
	.vga_out_data (vga_out_data )
);


assign e_gtxc=e_rxc;	 
assign e_reset = 1'b1; 

wire fifo_rd_en, full, empty;
wire [7:0] fifo_data;
wire [10:0] rd_data_count;

cvbs_fifo cvbs_fifo_inst (
  .rst				(1'b0), 									// input rst
  .wr_clk			(tw2867_27m), 							// input wr_clk
  .rd_clk			(e_rxc), 								// input rd_clk
  .din				(vin_ch0_data), 						// input [7 : 0] din
  .wr_en				(!full), 								// input wr_en
  .rd_en				(fifo_rd_en), 							// input rd_en
  .dout				(fifo_data), 							// output [23 : 0] dout
  .full				(full), 									// output full
  .empty				(empty), 								// output empty
  .rd_data_count	(rd_data_count) 						// output [10 : 0] rd_data_count
);

reg [10:0]	frame_index = 11'd0;
reg [10:0]	frame_index_n;
reg 			rd_en_pre = 1'b0;
reg			rd_en_now = 1'b0;

always @ (posedge e_rxc)
begin
	frame_index <= frame_index_n;
	
	rd_en_now <= fifo_rd_en;
	rd_en_pre <= rd_en_now;
end
always @ (*)
begin
	if (rd_en_pre == 1'b1 && rd_en_now == 1'b0)
		frame_index_n = frame_index + 1'b1;
	else
		frame_index_n = frame_index;
end

/////////////////udp���ͺͽ��ճ���/////////////////// 
udp udp_inst(
	.reset_n                 (1'b0),
	.e_rxc                   (e_rxc),
	.e_rxd                   (e_rxd),
	.e_rxdv                  (e_rxdv),
	.e_txen                  (e_txen),
	.e_txd                   (e_txd),
	.e_txer                  (e_txer),		

	.fifo_data               (fifo_data),      		//FIFO������8bit����/
	.fifo_data_count         (rd_data_count),     	//��FIFO�ж���������������
	.fifo_rd_en              (fifo_rd_en),          //����FIFO��ʹ���ź�

	.frame_index             (frame_index), 

	.tx_total_length         (16'd892),            //����IP�����ܳ���/			IP��ͷ+����
	.tx_data_length          (16'd872)             //����IP�������ݳ���/		UDP��ͷ+����,��Ч���ݳ���Ϊtx_data_length-8
);

	
endmodule