/**
 * \file decoder.h
 * \brief <short description>.
 */

#ifndef VITERBI_DECODER_H
#define VITERBI_DECODER_H

#include <array>

#include <systemc>

using namespace sc_core;
using namespace sc_dt;

struct acs_unit;

template<unsigned N, unsigned M>
struct trellis_stage {
  sc_bv<M - 1> prev_state0;
  sc_bv<M - 1> prev_state1;
  sc_bv<N> bits0;
  sc_bv<N> bits1;
};

int hamming_distance(unsigned a, unsigned b);

acs_unit *acs_creator(const char *name, unsigned i);

template<unsigned N, unsigned M>
void create_trellis(const std::array<const sc_bv<M>, N> &poly, trellis_stage<N, M> *trellis) {
  const unsigned num_states = static_cast<unsigned>(pow(2, M));

  for (unsigned i = 0; i < num_states; ++i) {
    sc_bv<M> j(i);
    unsigned out = ((j & poly[0]).xor_reduce() << 1) | (j & poly[1]).xor_reduce();
    auto next_state = j >> 1;
    auto &stage = trellis[next_state.to_uint()];

    if (j.get_bit(0)) {
      stage.prev_state1 = i;
      stage.bits1 = out;
    } else {
      stage.prev_state0 = i;
      stage.bits0 = out;
    }
  }
}

SC_MODULE(acs_unit) {
  SC_HAS_PROCESS(acs_unit);

  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<int> bm0;
  sc_in<int> bm1;
  sc_in<int> pm0;
  sc_in<int> pm1;
  sc_out<int> pm_out;
  sc_out<bool> decision;
  const int start;

  acs_unit(const sc_module_name &name, int start_);

  void main();
};

template<unsigned N>
SC_MODULE(branch_metric_unit) {
  SC_HAS_PROCESS(branch_metric_unit);

  const unsigned num_bm = static_cast<unsigned>(pow(2, N));

  sc_in<sc_bv<N>> in;
  sc_vector<sc_out<int>> bm{"branch_metric", num_bm};

  explicit branch_metric_unit(const sc_module_name &name) : sc_module(name) {
    SC_METHOD(main);
    sensitive << in;
  }

  void main() {
    unsigned x = in.read().to_uint();
    for (unsigned i = 0; i < num_bm; ++i) {
      bm[i].write(N - hamming_distance(x, i));
    }
  }
};

template<unsigned N, unsigned M>
SC_MODULE(path_metric_unit) {
  const unsigned num_bm = static_cast<unsigned>(pow(2, N));
  const unsigned num_acs = static_cast<unsigned>(pow(2, M - 1));

  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_vector<sc_in<int>> bm{"bm", num_bm};
  sc_vector<sc_out<int>> pm{"pm", num_acs};
  sc_vector<sc_out<bool>> decisions{"decisions", num_acs};

  sc_vector<acs_unit> acs{"acs", num_acs, acs_creator};

  explicit path_metric_unit(const sc_module_name &name, trellis_stage<N, M> *trellis) : sc_module(name) {
    for (unsigned i = 0; i < num_acs; ++i) {
      auto &unit = acs[i];
      trellis_stage<N, M> &stage = trellis[i];
      unit.clk(clk);
      unit.rst(rst);
      unit.bm0(bm[stage.bits0.to_uint()]);
      unit.bm1(bm[stage.bits1.to_uint()]);
      unit.pm0(pm[stage.prev_state0.to_uint()]);
      unit.pm1(pm[stage.prev_state1.to_uint()]);
      unit.pm_out(pm[i]);
      unit.decision(decisions[i]);
    }
  }
};

template<unsigned N, unsigned M>
SC_MODULE(traceback_unit) {
  SC_HAS_PROCESS(traceback_unit);

  const unsigned num_acs = static_cast<unsigned>(pow(2, M - 1));

  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<bool> trigger;
  sc_vector<sc_in<int>> pm{"pm", num_acs};
  sc_vector<sc_in<bool>> decisions{"decisions", num_acs};
  sc_fifo_out<bool> out;

  trellis_stage<N, M> *trellis;

  explicit traceback_unit(const sc_module_name &name, trellis_stage<N, M> *trellis_)
      : sc_module(name), trellis(trellis_) {
    SC_CTHREAD(main, clk.pos());
    reset_signal_is(rst, true);
  }

  void main() {
    std::array<bool, 20> result{};
    std::array<std::array<bool, 8>, 20> arr{};
    wait();

    unsigned i = 0;
    while (!trigger.read()) {
      for (unsigned j = 0; j < num_acs; ++j) {
        arr[i][j] = decisions[j].read();
      }

      ++i;
      wait();
    }

    unsigned pos = 0;
    for (int j = i - 1; j >= 0; --j) {
      trellis_stage<N, M> &stage = trellis[pos];
      unsigned d0 = stage.prev_state0.to_uint();
      unsigned d1 = stage.prev_state1.to_uint();

      if (!arr[j][pos]) {
        result[j] = d0 < 4;
        pos = d0;
      } else if (arr[j][pos]) {
        result[j] = d1 > 3;
        pos = d1;
      } else {
        sc_assert(false);
      }
    }

    for (unsigned j = 0; j < i; ++j) {
      out.write(result[j]);
    }
  }
};

template<unsigned N, unsigned M>
SC_MODULE(decoder) {
  SC_HAS_PROCESS(decoder);

  const unsigned num_bm = static_cast<unsigned>(pow(2, N));
  const unsigned num_acs = static_cast<unsigned>(pow(2, M - 1));

  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<bool> trigger;
  sc_in<sc_bv<N>> in;
  sc_fifo_out<bool> out;

  branch_metric_unit<N> bmu{"bmu"};
  path_metric_unit<N, M> *pmu = nullptr;
  traceback_unit<N, M> *tbu = nullptr;

  sc_signal<bool> pmu_rst;
  sc_signal<bool> tbu_rst;
  sc_signal<bool> tbu_trigger;
  sc_vector<sc_signal<int>> bm{"bm", num_bm};
  sc_vector<sc_signal<int>> pm{"pm", num_acs};
  sc_vector<sc_signal<bool>> decisions{"decisions", num_acs};

  trellis_stage<N, M> *trellis = nullptr;

  decoder(const sc_module_name &name, const std::array<const sc_bv<M>, N> &poly)
      : sc_module(name) {
    SC_CTHREAD(main, clk.pos());
    reset_signal_is(rst, true);

    trellis = new trellis_stage<N, M>[num_acs];
    create_trellis<N, M>(poly, trellis);

    bmu.in(in);
    bmu.bm(bm);

    pmu = new path_metric_unit<N, M>("pmu", trellis);
    pmu->clk(clk);
    pmu->rst(pmu_rst);
    pmu->bm(bm);
    pmu->pm(pm);
    pmu->decisions(decisions);

    tbu = new traceback_unit<N, M>("tbu", trellis);
    tbu->clk(clk);
    tbu->rst(tbu_rst);
    tbu->trigger(tbu_trigger);
    tbu->pm(pm);
    tbu->decisions(decisions);
    tbu->out(out);
  }

  void main() {
    pmu_rst.write(true);
    tbu_rst.write(true);
    tbu_trigger.write(false);
    wait();
    wait();
    pmu_rst.write(false);
    wait();
    tbu_rst.write(false);

    while (!trigger.read()) {
      wait();
    }

    wait();
    wait();
    tbu_trigger.write(true);
  }

  ~decoder() override {
    delete tbu;
    tbu = nullptr;

    delete pmu;
    pmu = nullptr;

    delete[] trellis;
    trellis = nullptr;
  }
};

#endif // VITERBI_DECODER_H
