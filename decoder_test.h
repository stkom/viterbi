/**
 * \file decoder_test.h
 * \brief <short description>.
 */

#ifndef VITERBI_DECODER_TEST_H
#define VITERBI_DECODER_TEST_H

#include <systemc>

#include "decoder.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(decoder_test) {
  SC_HAS_PROCESS(decoder_test);

  sc_in<bool> clk;

  decoder<2, 4> dec{"dec", {"1111", "1101"}};

  sc_signal<bool> dec_rst;
  sc_signal<bool> dec_trigger;
  sc_signal<sc_bv<2>> dec_in;
  sc_fifo<bool> dec_out;

  explicit decoder_test(const sc_module_name &name);

  void main();
};

#endif // VITERBI_DECODER_TEST_H
