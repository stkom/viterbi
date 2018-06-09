/**
 * \file decoder.h
 * \brief <short description>.
 */

#ifndef VITERBI_DECODER_H
#define VITERBI_DECODER_H

#include <array>
#include <iomanip>

#include <systemc>

SC_MODULE(acs_unit) {
  SC_HAS_PROCESS(acs_unit);

  sc_core::sc_in<bool> clk;
  sc_core::sc_in<bool> rst;
  sc_core::sc_in<int> bm0;
  sc_core::sc_in<int> bm1;
  sc_core::sc_in<int> pm0;
  sc_core::sc_in<int> pm1;
  sc_core::sc_out<int> pm_out;
  sc_core::sc_out<bool> bit;

  const int start;

  explicit acs_unit(const sc_core::sc_module_name &name, int _start) : sc_module(name), start(_start) {
    SC_CTHREAD(main, clk.pos());
    reset_signal_is(rst, false);
  }

  void main() {
    pm_out.write(start);
    bit.write(false);
    wait();

    while (true) {
      int m0 = bm0.read() + pm0.read();
      int m1 = bm1.read() + pm1.read();
      bool b = m0 < m1;
      int m = b ? m1 : m0;

      pm_out.write(m);
      bit.write(b);
      std::cout << sc_core::sc_time_stamp() << " - " << name()
                << "{m0=" << std::setw(4) << m0 << ", m1=" << std::setw(4) << m1 << ", m=" << std::setw(4) << m
                << ", bit=" << b << "}\n";
      wait();
    }
  }
};

SC_MODULE(path_metric_unit) {
  sc_core::sc_in<bool> clk;
  sc_core::sc_in<bool> rst;
  sc_core::sc_in<int> bm00;
  sc_core::sc_in<int> bm01;
  sc_core::sc_in<int> bm10;
  sc_core::sc_in<int> bm11;
  sc_core::sc_out<int> pm000;
  sc_core::sc_out<int> pm001;
  sc_core::sc_out<int> pm010;
  sc_core::sc_out<int> pm011;
  sc_core::sc_out<int> pm100;
  sc_core::sc_out<int> pm101;
  sc_core::sc_out<int> pm110;
  sc_core::sc_out<int> pm111;
  sc_core::sc_out<bool> bit000;
  sc_core::sc_out<bool> bit001;
  sc_core::sc_out<bool> bit010;
  sc_core::sc_out<bool> bit011;
  sc_core::sc_out<bool> bit100;
  sc_core::sc_out<bool> bit101;
  sc_core::sc_out<bool> bit110;
  sc_core::sc_out<bool> bit111;

  acs_unit acs000{"acs000", 0};
  acs_unit acs001{"acs001", -0xff};
  acs_unit acs010{"acs010", -0xff};
  acs_unit acs011{"acs011", -0xff};
  acs_unit acs100{"acs100", -0xff};
  acs_unit acs101{"acs101", -0xff};
  acs_unit acs110{"acs110", -0xff};
  acs_unit acs111{"acs111", -0xff};

  explicit path_metric_unit(const sc_core::sc_module_name &name) : sc_module(name) {
    acs000.clk(clk);
    acs000.rst(rst);
    acs000.bm0(bm00);
    acs000.bm1(bm11);
    acs000.pm0(pm000);
    acs000.pm1(pm001);
    acs000.pm_out(pm000);
    acs000.bit(bit000);

    acs001.clk(clk);
    acs001.rst(rst);
    acs001.bm0(bm10);
    acs001.bm1(bm01);
    acs001.pm0(pm010);
    acs001.pm1(pm011);
    acs001.pm_out(pm001);
    acs001.bit(bit001);

    acs010.clk(clk);
    acs010.rst(rst);
    acs010.bm0(bm11);
    acs010.bm1(bm00);
    acs010.pm0(pm100);
    acs010.pm1(pm101);
    acs010.pm_out(pm010);
    acs010.bit(bit010);

    acs011.clk(clk);
    acs011.rst(rst);
    acs011.bm0(bm01);
    acs011.bm1(bm10);
    acs011.pm0(pm110);
    acs011.pm1(pm111);
    acs011.pm_out(pm011);
    acs011.bit(bit011);

    acs100.clk(clk);
    acs100.rst(rst);
    acs100.bm0(bm11);
    acs100.bm1(bm00);
    acs100.pm0(pm000);
    acs100.pm1(pm001);
    acs100.pm_out(pm100);
    acs100.bit(bit100);

    acs101.clk(clk);
    acs101.rst(rst);
    acs101.bm0(bm01);
    acs101.bm1(bm10);
    acs101.pm0(pm010);
    acs101.pm1(pm011);
    acs101.pm_out(pm101);
    acs101.bit(bit101);

    acs110.clk(clk);
    acs110.rst(rst);
    acs110.bm0(bm00);
    acs110.bm1(bm11);
    acs110.pm0(pm100);
    acs110.pm1(pm101);
    acs110.pm_out(pm110);
    acs110.bit(bit110);

    acs111.clk(clk);
    acs111.rst(rst);
    acs111.bm0(bm10);
    acs111.bm1(bm01);
    acs111.pm0(pm110);
    acs111.pm1(pm111);
    acs111.pm_out(pm111);
    acs111.bit(bit111);
  }
};

SC_MODULE(branch_metric_unit) {
  SC_HAS_PROCESS(branch_metric_unit);

  sc_core::sc_in<sc_dt::sc_bv<2>> in;
  sc_core::sc_out<int> bm00;
  sc_core::sc_out<int> bm01;
  sc_core::sc_out<int> bm10;
  sc_core::sc_out<int> bm11;

  explicit branch_metric_unit(const sc_core::sc_module_name &name) : sc_module(name) {
    SC_METHOD(main);
    sensitive << in;
  }

  void main() {
    unsigned i = in.read().to_uint();

    switch (i) {
      case 0:
        bm00.write(2);
        bm01.write(1);
        bm10.write(1);
        bm11.write(0);
        break;
      case 1:
        bm00.write(1);
        bm01.write(2);
        bm10.write(0);
        bm11.write(1);
        break;
      case 2:
        bm00.write(1);
        bm01.write(0);
        bm10.write(2);
        bm11.write(1);
        break;
      case 3:
        bm00.write(0);
        bm01.write(1);
        bm10.write(1);
        bm11.write(2);
        break;
      default:
        sc_assert(false);
    }

    std::cout << sc_core::sc_time_stamp() << " - " << name() << "->in{" << in.read() << "}\n";
  }
};

struct decision {
  int d0;
  int d1;
};

SC_MODULE(traceback_unit) {
  SC_HAS_PROCESS(traceback_unit);

  sc_core::sc_in<bool> clk;
  sc_core::sc_in<bool> rst;
  sc_core::sc_in<bool> enable;
  sc_core::sc_in<int> pm000;
  sc_core::sc_in<int> pm001;
  sc_core::sc_in<int> pm010;
  sc_core::sc_in<int> pm011;
  sc_core::sc_in<int> pm100;
  sc_core::sc_in<int> pm101;
  sc_core::sc_in<int> pm110;
  sc_core::sc_in<int> pm111;
  sc_core::sc_in<bool> d000;
  sc_core::sc_in<bool> d001;
  sc_core::sc_in<bool> d010;
  sc_core::sc_in<bool> d011;
  sc_core::sc_in<bool> d100;
  sc_core::sc_in<bool> d101;
  sc_core::sc_in<bool> d110;
  sc_core::sc_in<bool> d111;

  explicit traceback_unit(const sc_core::sc_module_name &name) : sc_module(name) {
    SC_CTHREAD(main, clk.pos());
    reset_signal_is(rst, false);
  }

  void main() {
    std::array<bool, 7> result{};
    std::array<std::array<bool, 8>, 7> arr{};
    std::array<decision, 8> conn{{
                                     {0, 1}, // 000
                                     {2, 3}, // 001
                                     {4, 5}, // 010
                                     {6, 7}, // 011
                                     {0, 1}, // 100
                                     {2, 3}, // 101
                                     {4, 5}, // 110
                                     {6, 7}, // 111
                                 }};
    wait();

    int i = 0;
    while (true) {
      if (!enable.read()) {
        arr[i][0] = d000.read();
        arr[i][1] = d001.read();
        arr[i][2] = d010.read();
        arr[i][3] = d011.read();
        arr[i][4] = d100.read();
        arr[i][5] = d101.read();
        arr[i][6] = d110.read();
        arr[i][7] = d111.read();
        ++i;
        wait();
        continue;
      }

      int pos = 0;
      while (i-- > 0) {
        int d0 = conn[pos].d0;
        int d1 = conn[pos].d1;

        if (!arr[i][pos]) {
          std::cout << "{acs=" << pos << ",b=" << false << "}\n";
          result[i] = d0 < 4;
          pos = d0;
        } else if (arr[i][pos]) {
          std::cout << "{acs=" << pos << ",b=" << true << "}\n";
          result[i] = d1 > 3;
          pos = d1;
        } else {
          // sc_assert(false);
        }
      }

      for (size_t j = 0; j < 7; ++j) {
        std::cout << result[j];
      }
      std::cout << "\n";

      std::cout << sc_core::sc_time_stamp() << " - " << name() << "->main{enable=" << enable.read() << "}\n";
      wait();
    }
  }
};

SC_MODULE(decoder) {
  SC_HAS_PROCESS(decoder);

  sc_core::sc_in<bool> clk;
  // sc_core::sc_fifo_in<bool> in;
  // sc_core::sc_fifo_out<bool> out;

  branch_metric_unit bmu{"bmu"};
  path_metric_unit pmu{"pmu"};
  traceback_unit tbu{"tbu"};

  sc_core::sc_signal<bool> pmu_rst;
  sc_core::sc_signal<bool> tbu_rst;

  sc_core::sc_signal<sc_dt::sc_bv<2>> bmu_in;
  sc_core::sc_signal<int> bm00;
  sc_core::sc_signal<int> bm01;
  sc_core::sc_signal<int> bm10;
  sc_core::sc_signal<int> bm11;

  sc_core::sc_signal<int> pm000;
  sc_core::sc_signal<int> pm001;
  sc_core::sc_signal<int> pm010;
  sc_core::sc_signal<int> pm011;
  sc_core::sc_signal<int> pm100;
  sc_core::sc_signal<int> pm101;
  sc_core::sc_signal<int> pm110;
  sc_core::sc_signal<int> pm111;

  sc_core::sc_signal<bool> bit000;
  sc_core::sc_signal<bool> bit001;
  sc_core::sc_signal<bool> bit010;
  sc_core::sc_signal<bool> bit011;
  sc_core::sc_signal<bool> bit100;
  sc_core::sc_signal<bool> bit101;
  sc_core::sc_signal<bool> bit110;
  sc_core::sc_signal<bool> bit111;

  sc_core::sc_signal<bool> tbu_enable;

  explicit decoder(const sc_core::sc_module_name &name) : sc_module(name) {
    SC_CTHREAD(main, clk.pos());

    bmu.in(bmu_in);
    bmu.bm00(bm00);
    bmu.bm01(bm01);
    bmu.bm10(bm10);
    bmu.bm11(bm11);

    pmu.clk(clk);
    pmu.rst(pmu_rst);
    pmu.bm00(bm00);
    pmu.bm01(bm01);
    pmu.bm10(bm10);
    pmu.bm11(bm11);
    pmu.pm000(pm000);
    pmu.pm001(pm001);
    pmu.pm010(pm010);
    pmu.pm011(pm011);
    pmu.pm100(pm100);
    pmu.pm101(pm101);
    pmu.pm110(pm110);
    pmu.pm111(pm111);
    pmu.bit000(bit000);
    pmu.bit001(bit001);
    pmu.bit010(bit010);
    pmu.bit011(bit011);
    pmu.bit100(bit100);
    pmu.bit101(bit101);
    pmu.bit110(bit110);
    pmu.bit111(bit111);

    tbu.clk(clk);
    tbu.rst(tbu_rst);
    tbu.enable(tbu_enable);
    tbu.pm000(pm000);
    tbu.pm001(pm001);
    tbu.pm010(pm010);
    tbu.pm011(pm011);
    tbu.pm100(pm100);
    tbu.pm101(pm101);
    tbu.pm110(pm110);
    tbu.pm111(pm111);
    tbu.d000(bit000);
    tbu.d001(bit001);
    tbu.d010(bit010);
    tbu.d011(bit011);
    tbu.d100(bit100);
    tbu.d101(bit101);
    tbu.d110(bit110);
    tbu.d111(bit111);
  }

  void main() {
    pmu_rst.write(false);
    tbu_rst.write(false);
    wait();

    pmu_rst.write(true);
    bmu_in.write({"11"});
    wait();

    tbu_rst.write(true);
    bmu_in.write({"11"});
    wait();

    bmu_in.write({"01"});
    wait();

    bmu_in.write({"11"});
    wait();

    bmu_in.write({"01"});
    wait();

    bmu_in.write({"01"});
    wait();

    bmu_in.write({"11"});
    wait();

    pmu_rst.write(false);
    wait();

    tbu_enable.write(true);
    wait();

    tbu_rst.write(false);
    tbu_enable.write(false);
    wait();
  }
};

#endif // VITERBI_DECODER_H
