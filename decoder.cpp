#include "decoder.h"

int hamming_distance(unsigned a, unsigned b) {
  unsigned x = a ^ b;
  int dist = 0;

  while (x != 0) {
    ++dist;
    x &= x - 1;
  }

  return dist;
}

acs_unit *acs_creator(const char *name, unsigned i) {
  return new acs_unit(name, i == 0 ? 0 : -0xff);
}

acs_unit::acs_unit(const sc_core::sc_module_name &name, int start_)
    : sc_module(name), start(start_) {
  SC_CTHREAD(main, clk.pos());
  reset_signal_is(rst, true);
}

void acs_unit::main() {
  pm_out.write(start);
  decision.write(false);
  wait();

  while (true) {
    int m0 = bm0.read() + pm0.read();
    int m1 = bm1.read() + pm1.read();
    bool b = m0 < m1;
    int max_metric = b ? m1 : m0;

    pm_out.write(max_metric);
    decision.write(b);
    wait();
  }
}
