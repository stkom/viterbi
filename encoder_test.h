/**
 * \file encoder_test.h
 * \brief Provides the Viterbi encoder test module.
 */

#ifndef VITERBI_ENCODER_TEST_H
#define VITERBI_ENCODER_TEST_H

#include <systemc>

#include "encoder.h"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(encoder_test) {
  SC_HAS_PROCESS(encoder_test);

  encoder<2, 4> enc{"enc", {"1111", "1101"}};
  sc_fifo<bool> enc_in;
  sc_fifo<bool> enc_out;

  explicit encoder_test(const sc_module_name &name);

  void main();
};

#endif // VITERBI_ENCODER_TEST_H
