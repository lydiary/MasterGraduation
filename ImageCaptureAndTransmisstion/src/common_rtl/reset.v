/*简单上电复位模�当计数器�xffff时复位信号为1
*/
module reset(
	input clk,
	output rst_n
);

reg		[15:0] 		cnt = 16'd0;
reg 						rst_n_reg;

assign rst_n = rst_n_reg;

always@(posedge clk)
begin
	if(cnt != 16'hffff)
		cnt <= cnt + 16'd1;
	else
		cnt <= cnt;
end
		
always@(posedge clk)
begin
	rst_n_reg <= (cnt == 16'hffff);
end
	
endmodule 