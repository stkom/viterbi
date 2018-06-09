/**
 * \file display.h
 * \brief <short description>
 */

#ifndef VITERBI_DISPLAY_H
#define VITERBI_DISPLAY_H

#include <systemc>

SC_MODULE(display) {
  SC_HAS_PROCESS(display);

  sc_core::sc_fifo_in<bool> in;

  explicit display(const sc_core::sc_module_name &name) : sc_module(name) {
    SC_THREAD(main);
  }

  void main() {
    int i = 0;
    while (true) {
      std::cout << in.read();

      if (++i % 2 == 0) {
        std::cout << " ";
      }
    }
  }
};

#endif // VITERBI_DISPLAY_H
