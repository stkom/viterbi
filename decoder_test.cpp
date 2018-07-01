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
  const int len = 7;
  sc_bv<len * 2> input("11110111010111");

  for (int i = 0; i < len; ++i) {
    dec_in.write(input);
    input >>= 2;
    wait();
  }

  dec_trigger.write(true);
  for (int j = 0; j < 5; ++j) {
    wait();
  }

  sc_assert(dec_out.num_available() == 7);

  sc_bv<7> result;
  while (dec_out.num_available() > 0) {
    result <<= 1;
    result.set_bit(0, dec_out.read());
  }

  std::cout << name() << "->result{" << result << "}\n";
  sc_assert(result == "1011000");
  std::cout << "[PASSED] " << name() << "\n";
}
