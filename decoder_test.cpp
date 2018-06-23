/**
 * \file decoder_test.cpp
 * \brief <short description>.
 */

#include "decoder_test.h"

decoder_test::decoder_test(const sc_module_name &name) : sc_module(name) {
  SC_CTHREAD(main, clk.pos());

  dec.clk(clk);
  dec.rst(dec_rst);
  dec.trigger(dec_trigger);
  dec.in(dec_in);
  dec.out(dec_out);
}

void decoder_test::main() {
  dec_in.write("11");
  wait();
  dec_in.write("11");
  wait();
  dec_in.write("01");
  wait();
  dec_in.write("11");
  wait();
  dec_in.write("01");
  wait();
  dec_in.write("01");
  wait();
  dec_in.write("11");
  wait();

  dec_trigger.write(true);
  wait();
  wait();
  wait();
  wait();
  wait();

  sc_assert(dec_out.num_available() == 7);

  sc_bv<7> result;
  while (dec_out.num_available() > 0) {
    result <<= 1;
    result.set_bit(0, dec_out.read());
  }

  sc_assert(result == "1011000");
  std::cout << name() << "->result{" << result << "}\n";
  std::cout << "[SUCCESS] " << name() << "\n";
}
