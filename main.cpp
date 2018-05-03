/**
 * \file main.cpp
 * \brief Provides the SystemC simulation entry point.
 */

#include <iostream>
#include <systemc>

using namespace sc_core;
using namespace sc_dt;

/**
 * \brief The SystemC simulation entry point.
 * \return the exit code.
 */

SC_MODULE(ENCODER){
  sc_uint<1> reg1, reg2, reg3;
  sc_in< sc_uint<1> > input;
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_out< sc_uint<1> > output1;
  sc_out< sc_uint<1> > output2;

  void func(){
    //init
    reg1 = 0;
    reg2 = 0;
    reg3 = 0;
    output1.write(0);
    output2.write(0);

    wait();
        while(true) {
          output1.write((input.read() + reg1 + reg2 + reg3) % 2);
          output2.write((input.read() + reg1 + reg3) % 2);
          reg3 = reg2;
          reg2 = reg1;
          reg1 = input.read();
          wait();
        }
  }

  SC_CTOR(ENCODER){
    SC_CTHREAD( func, clk.pos() );
    reset_signal_is( rst, true );
  }
};

SC_MODULE(tb){
  sc_in<bool> clk;
  sc_out<bool> rst;
  sc_out<sc_uint<1> > input;
  sc_in<sc_uint<1> > output1;
  sc_in<sc_uint<1> > output2;

  void source(){
   //Reset
   input.write(0);
   rst.write(1);
   wait();
   rst.write(0);
   wait();

   sc_uint<1> tmp[7] = {1,0,1,1,0,0,0};

   //send stimulus
    for(int i = 0; i < 7;i++)
    {
      input.write(tmp[i]);
      wait();
    }
  }
  void sink(){
    sc_uint<1> indata1, indata2;
    for(int i = 0; i < 7;i++)
    {
      indata1 = output1.read();
      indata2 = output2.read();
      wait();
      std::cout << i << " :\t" << indata1.to_int() << indata2.to_int()<< "\n";
    }
    //end simulation
    sc_stop();
  }

  SC_CTOR(tb){
    SC_CTHREAD(source, clk.pos());
    SC_CTHREAD(sink, clk.pos());
  }
};

SC_MODULE( SYSTEM ) {
  tb *tb0;
  ENCODER *en0;

  sc_signal<bool> rst_sig;
  sc_signal< sc_uint<1> > input_sig;
  sc_signal< sc_uint<1> > output1_sig;
  sc_signal< sc_uint<1> > output2_sig;

  sc_clock clk_sig;

  explicit SYSTEM(const sc_module_name &name)
    : sc_module(name), clk_sig ("clk_sig",10,SC_NS)
  {
    tb0 = new tb("tb0");
    tb0->clk(clk_sig);
    tb0->rst(rst_sig);
    tb0->input(input_sig);
    tb0->output1(output1_sig);
    tb0->output2(output2_sig);

    en0 = new ENCODER("en0");
    en0->clk(clk_sig);
    en0->rst(rst_sig);
    en0->input(input_sig);
    en0->output1(output1_sig);
    en0->output2(output2_sig);
  }

  ~SYSTEM(){
    delete tb0;
    delete en0;
  }
};

SYSTEM *top = NULL;

int sc_main(int, char *[]) {
  top = new SYSTEM("top");
  sc_start();
  std::cout << "Bla";
  return 0;
}
