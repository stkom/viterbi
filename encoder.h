/**
 * \file encoder.h
 * \brief Provides the Viterbi encoder module.
 */

#ifndef VITERBI_ENCODER_H
#define VITERBI_ENCODER_H

#include <array>

#include <systemc>

template<int N, int M>
SC_MODULE(encoder) {
  SC_HAS_PROCESS(encoder);

  sc_core::sc_fifo_in<bool> in;
  sc_core::sc_fifo_out<bool> out;
  sc_dt::sc_bv<M> memory;
  std::array<sc_dt::sc_bv<M>, N> poly;

  explicit encoder(const sc_core::sc_module_name &name, const std::array<sc_dt::sc_bv<M>, N> &poly_)
      : sc_module(name), poly(poly_) {
    SC_THREAD(main);
  }

  void main() {
    memory &= 0;

    while (true) {
      memory >>= 1;
      memory.set_bit(M - 1, in.read());

      for (int i = 0; i < N; ++i) {
        out.write((memory & poly[i]).xor_reduce());
      }

      // std::cout << "memory{" << memory << "}\n";
    }
  }
};

#endif // VITERBI_ENCODER_H
