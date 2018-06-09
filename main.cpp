/**
 * \file main.cpp
 * \brief Provides the SystemC simulation entry point.
 */

#include <systemc>

#include "bit_generator.h"
#include "decoder.h"
#include "display.h"
#include "encoder.h"

using namespace sc_core;
using namespace sc_dt;

/**
 * \brief The SystemC simulation entry point.
 * \return the exit code.
 */
int sc_main(int, char *[]) {
  sc_core::sc_clock clk{"clk", 10, sc_core::SC_NS};
  // sc_fifo<bool> fifo_out;

  // dec: 10 11 00 0
  // enc: 11 11 01 11 01 01 11
  decoder decoder("decoder");
  decoder.clk(clk);
  // decoder.out(fifo_out);

  // display display("display");
  // display.in(fifo_out);

  sc_start(200, SC_NS);
  return 0;
}
