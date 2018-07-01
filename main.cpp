/**
 * \file main.cpp
 * \brief Provides the SystemC simulation entry point.
 */

#include <systemc>

#include "decoder_test.h"
#include "encoder_test.h"

using namespace sc_core;
using namespace sc_dt;

/**
 * \brief The SystemC simulation entry point.
 * \return the exit code.
 */
int sc_main(int, char *[]) {
  sc_core::sc_clock clk{"clk", 10, sc_core::SC_NS};

  encoder_test enc_test("enc_test");

  decoder_test dec_test("dec_test");
  dec_test.clk(clk);

  sc_start(200, SC_NS);
  return 0;
}
