#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor(): tot_cpu_usage_(0.0){}
  float Utilization();  // TODO: See src/processor.cpp
  float Total_Usage() const {return tot_cpu_usage_;}
  void Total_Usage(float usage) { tot_cpu_usage_+=usage;}
  void Init_Usage() {tot_cpu_usage_=0.0;}
  // TODO: Declare any necessary private members
 private:
  float tot_cpu_usage_;
};

#endif