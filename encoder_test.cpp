#include "encoder_test.h"

encoder_test::encoder_test(const sc_module_name &name) : sc_module(name) {
  SC_THREAD(main);

  enc.in(enc_in);
  enc.out(enc_out);
}

void encoder_test::main() {
  const int len = 7;

  sc_bv<len> input("1011000");
  sc_bv<len * 2> result;

  for (int i = len - 1; i >= 0; --i) {
    enc_in.write(input.get_bit(i));
  }

  for (int i = len * 2 - 1; i >= 0; --i) {
    bool b = enc_out.read();
    result.set_bit(i, b);
  }

  std::cout << name() << "->result{" << result << "}\n";
  sc_assert(result == "11110111010111");
  std::cout << "[PASSED] " << name() << "\n";
}
