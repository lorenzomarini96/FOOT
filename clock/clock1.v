/* Modulo verilog per la realizzazione di un contatore 
sincrono mod10 (0...999) con comprensivo di 
segnali di enable, reset e riporto.*/

module count1000   // Nome del modulo
(   
    // Porte
    input clk, enable, reset,
    output reg clk_out,
    output reg [9:0] count   // Non sono sicurissimo...
);


    always @(posedge clk or posedge reset) 
    begin
        if (reset)
            count <= 0;
        else if (enable == 1'b1)
        begin
            if (count <= 999)
                begin
                   count <= count + 1;
                   clk_out <= 1'b0;
                end
                else
                    begin
                        count <=0;
                        clk_out <= 1'b1;
                    end 
        end
             
    end
endmodule



/* 
Un flip-flop può essere descritto in Verilog con un always
in cui la sensibilità al fronte positivo o negativo del trigger 
si specifica rispettivamente con le keyword posedge o negedge

*/