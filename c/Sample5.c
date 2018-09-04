#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
struct {int coverage[10000];} TestCoverage;
typedef int u32;

struct DPM {
  int voltage_response_time;
  int backbias_response_time;
};

struct PM {
  struct DPM dpm;
};

struct radeon_device {
  struct PM pm;
};

// test params
struct radeon_device _rdev = {0};

// test helpers
typedef struct {
  const char* name;
  int numParams;
  void** params;
} funcall;

funcall* calls[50];
int callIdx = 0;

void log_funcall(const char *name, int numParams, ...)
{
  void** params = (void**)malloc(numParams * sizeof(void*));

  va_list args;
  va_start(args, numParams);
  for (int i = 0; i < numParams; i++)
  {
    params[i] = va_arg(args, void*);
  }
  va_end(args);

  funcall* call = (funcall*)malloc(sizeof(funcall));
  call->name = name;
  call->numParams = numParams;
  call->params = params;
  calls[callIdx++] = call;
}

void initState(
  int voltage_response_time,
  int backbias_response_time)
{
  callIdx = 0;
  for (int i = 0; i < 16; i++) {
    calls[i] = NULL;
  }
  struct DPM _dpm = {
    .voltage_response_time = voltage_response_time,
    .backbias_response_time = backbias_response_time,
  };
  struct PM _pm = { .dpm = _dpm };
  _rdev.pm = _pm;
}

void cleanupState()
{
  for (int i = 0; i < 16; i++)
  {
    if (calls[i])
    {
      free(calls[i]->params);
      free(calls[i]);
    }
  }
}

// scaffolding
u32 _radeon_get_xclk_retval = 7371;
int NI_SMC_SOFT_REGISTER_mvdd_chg_time = 551193;
int NI_SMC_SOFT_REGISTER_delay_vreg = 471234;
int NI_SMC_SOFT_REGISTER_delay_bbias = 993872;
int NI_SMC_SOFT_REGISTER_delay_acpi = 847661;
int NI_SMC_SOFT_REGISTER_mclk_chg_timeout = 1234412;
int NI_SMC_SOFT_REGISTER_mc_block_delay = 31234;
int NI_SMC_SOFT_REGISTER_mclk_switch_lim = 612216;

u32 radeon_get_xclk(struct radeon_device *rdev){
  assert(rdev == &_rdev);
  log_funcall("radeon_get_xclk", 1, rdev);
  return _radeon_get_xclk_retval;
}

void rv770_write_smc_soft_register(struct radeon_device *rdev, int reg, u32 value){
  assert(rdev == &_rdev);
  log_funcall("rv770_write_smc_soft_register", 3, rdev, reg, value);
}

#START OF SAMPLE
static void ni_program_response_times(struct radeon_device *rdev)
{
  u32 voltage_response_time, backbias_response_time, acpi_delay_time, vbi_time_out;
  u32 vddc_dly, bb_dly, acpi_dly, vbi_dly, mclk_switch_limit;
  u32 reference_clock;

  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_mvdd_chg_time, 1);

  voltage_response_time = (u32)rdev->pm.dpm.voltage_response_time;
  backbias_response_time = (u32)rdev->pm.dpm.backbias_response_time;

  if (voltage_response_time == 0)
    voltage_response_time = 1000;

  if (backbias_response_time == 0)
    backbias_response_time = 1000;

  acpi_delay_time = 15000;
  vbi_time_out = 100000;

  reference_clock = radeon_get_xclk(rdev);

  vddc_dly = (voltage_response_time  * reference_clock) / 1600;
  bb_dly   = (backbias_response_time * reference_clock) / 1600;
  acpi_dly = (acpi_delay_time * reference_clock) / 1600;
  vbi_dly  = (vbi_time_out * reference_clock) / 1600;

  mclk_switch_limit = (460 * reference_clock) / 100;

  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_delay_vreg,  vddc_dly);
  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_delay_bbias, bb_dly);
  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_delay_acpi,  acpi_dly);
  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_mclk_chg_timeout, vbi_dly);
  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_mc_block_delay, 0xAA);
  rv770_write_smc_soft_register(rdev, NI_SMC_SOFT_REGISTER_mclk_switch_lim, mclk_switch_limit);
}
#END OF SAMPLE

int main(void)
{
  initState(13, 13);
  ni_program_response_times(&_rdev);
  assert(calls[0]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[0]->params[1] == NI_SMC_SOFT_REGISTER_mvdd_chg_time);
    assert((int64_t)calls[0]->params[2] == 1);
  assert(calls[1]->name == "radeon_get_xclk");
  assert(calls[2]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[2]->params[1] == NI_SMC_SOFT_REGISTER_delay_vreg);
    assert((int64_t)calls[2]->params[2] == 59);
  assert(calls[3]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[3]->params[1] == NI_SMC_SOFT_REGISTER_delay_bbias);
    assert((int64_t)calls[3]->params[2] == 59);
  assert(calls[4]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[4]->params[1] == NI_SMC_SOFT_REGISTER_delay_acpi);
    assert((int64_t)calls[4]->params[2] == 69103);
  assert(calls[5]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[5]->params[1] == NI_SMC_SOFT_REGISTER_mclk_chg_timeout);
    assert((int64_t)calls[5]->params[2] == 460687);
  assert(calls[6]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[6]->params[1] == NI_SMC_SOFT_REGISTER_mc_block_delay);
    assert((int64_t)calls[6]->params[2] == 0xAA);
  assert(calls[7]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[7]->params[1] == NI_SMC_SOFT_REGISTER_mclk_switch_lim);
    assert((int64_t)calls[7]->params[2] == 33906);
  assert(calls[8] == NULL);
  cleanupState();

  initState(13, 0);
  ni_program_response_times(&_rdev);
  assert(calls[0]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[0]->params[1] == NI_SMC_SOFT_REGISTER_mvdd_chg_time);
    assert((int64_t)calls[0]->params[2] == 1);
  assert(calls[1]->name == "radeon_get_xclk");
  assert(calls[2]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[2]->params[1] == NI_SMC_SOFT_REGISTER_delay_vreg);
    assert((int64_t)calls[2]->params[2] == 59);
  assert(calls[3]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[3]->params[1] == NI_SMC_SOFT_REGISTER_delay_bbias);
    assert((int64_t)calls[3]->params[2] == 4606);
  assert(calls[4]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[4]->params[1] == NI_SMC_SOFT_REGISTER_delay_acpi);
    assert((int64_t)calls[4]->params[2] == 69103);
  assert(calls[5]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[5]->params[1] == NI_SMC_SOFT_REGISTER_mclk_chg_timeout);
    assert((int64_t)calls[5]->params[2] == 460687);
  assert(calls[6]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[6]->params[1] == NI_SMC_SOFT_REGISTER_mc_block_delay);
    assert((int64_t)calls[6]->params[2] == 0xAA);
  assert(calls[7]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[7]->params[1] == NI_SMC_SOFT_REGISTER_mclk_switch_lim);
    assert((int64_t)calls[7]->params[2] == 33906);
  assert(calls[8] == NULL);
  cleanupState();

  initState(0, 13);
  ni_program_response_times(&_rdev);
  assert(calls[0]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[0]->params[1] == NI_SMC_SOFT_REGISTER_mvdd_chg_time);
    assert((int64_t)calls[0]->params[2] == 1);
  assert(calls[1]->name == "radeon_get_xclk");
  assert(calls[2]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[2]->params[1] == NI_SMC_SOFT_REGISTER_delay_vreg);
    assert((int64_t)calls[2]->params[2] == 4606);
  assert(calls[3]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[3]->params[1] == NI_SMC_SOFT_REGISTER_delay_bbias);
    assert((int64_t)calls[3]->params[2] == 59);
  assert(calls[4]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[4]->params[1] == NI_SMC_SOFT_REGISTER_delay_acpi);
    assert((int64_t)calls[4]->params[2] == 69103);
  assert(calls[5]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[5]->params[1] == NI_SMC_SOFT_REGISTER_mclk_chg_timeout);
    assert((int64_t)calls[5]->params[2] == 460687);
  assert(calls[6]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[6]->params[1] == NI_SMC_SOFT_REGISTER_mc_block_delay);
    assert((int64_t)calls[6]->params[2] == 0xAA);
  assert(calls[7]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[7]->params[1] == NI_SMC_SOFT_REGISTER_mclk_switch_lim);
    assert((int64_t)calls[7]->params[2] == 33906);
  assert(calls[8] == NULL);
  cleanupState();

  initState(0, 0);
  ni_program_response_times(&_rdev);
  assert(calls[0]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[0]->params[1] == NI_SMC_SOFT_REGISTER_mvdd_chg_time);
    assert((int64_t)calls[0]->params[2] == 1);
  assert(calls[1]->name == "radeon_get_xclk");
  assert(calls[2]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[2]->params[1] == NI_SMC_SOFT_REGISTER_delay_vreg);
    assert((int64_t)calls[2]->params[2] == 4606);
  assert(calls[3]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[3]->params[1] == NI_SMC_SOFT_REGISTER_delay_bbias);
    assert((int64_t)calls[3]->params[2] == 4606);
  assert(calls[4]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[4]->params[1] == NI_SMC_SOFT_REGISTER_delay_acpi);
    assert((int64_t)calls[4]->params[2] == 69103);
  assert(calls[5]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[5]->params[1] == NI_SMC_SOFT_REGISTER_mclk_chg_timeout);
    assert((int64_t)calls[5]->params[2] == 460687);
  assert(calls[6]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[6]->params[1] == NI_SMC_SOFT_REGISTER_mc_block_delay);
    assert((int64_t)calls[6]->params[2] == 0xAA);
  assert(calls[7]->name == "rv770_write_smc_soft_register");
    assert((int64_t)calls[7]->params[1] == NI_SMC_SOFT_REGISTER_mclk_switch_lim);
    assert((int64_t)calls[7]->params[2] == 33906);
  assert(calls[8] == NULL);
  cleanupState();
}
