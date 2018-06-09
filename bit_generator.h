/**
 * \file bit_generator.h
 * \brief <short description>
 */

#ifndef VITERBI_BIT_GENERATOR_H
#define VITERBI_BIT_GENERATOR_H

#include <systemc>

template<int N>
SC_MODULE(bit_generator) {
  SC_HAS_PROCESS(bit_generator);

  sc_core::sc_fifo_out<bool> out;
  sc_dt::sc_bv<N> data;

  explicit bit_generator(const sc_core::sc_module_name &name, const sc_dt::sc_bv<N> &data_)
      : sc_module(name), data(data_) {
    SC_THREAD(main);
  }

  void main() {
    int length = data.length();

    for (int i = 0; i < length; ++i) {
      out.write(data.get_bit(length - 1));
      data <<= 1;
    }
  }
};

#endif // VITERBI_BIT_GENERATOR_H
