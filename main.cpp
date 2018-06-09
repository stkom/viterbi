/**
 * \file main.cpp
 * \brief Provides the SystemC simulation entry point.
 */

#include <systemc>

#include "bit_generator.h"
#include "encoder.h"
#include "display.h"

using namespace sc_core;
using namespace sc_dt;

/**
 * \brief The SystemC simulation entry point.
 * \return the exit code.
 */
int sc_main(int, char *[]) {
  sc_fifo<bool> fifo_in;
  sc_fifo<bool> fifo_out;

  bit_generator<7> bit_gen("bit_generator", "1011000");
  bit_gen.out(fifo_in);

  encoder<2, 4> encoder("encoder", {"1111", "1101"});
  encoder.in(fifo_in);
  encoder.out(fifo_out);

  display display("display");
  display.in(fifo_out);

  sc_start(100, SC_NS);
  return 0;
}
