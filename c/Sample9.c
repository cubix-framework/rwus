#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct {int coverage[10000];} TestCoverage;

typedef int u32;

struct radeon_device {
  int family;
  int num_crtc;
  int usec_timeout;
};

struct radeon_ps {
};

struct bs_low {
  int mclk;
};

struct rv7xx_ps {
  struct bs_low low;
};

int MC_SEQ_STATUS_M = 772493212;
int CHIP_CYPRESS = 6521210;
int CHIP_HEMLOCK = 1140206;
int MC_CONFIG_MCD = 99471234;
int MC_CG_CONFIG_MCD = 8347512;
int MC_CONFIG = 322014723;
int MC_CG_CONFIG = 47120421;
int MC_CG_SEQ_YCLK_SUSPEND = 510274321;
int MC_CG_SEQ_DRAMCONF_S1 = 910243;
int CG_SEQ_REQ_MASK = 88129412;
int MC_CG_SEQ_YCLK_RESUME = 1231024;
int MC_SEQ_CG = 75710999;
struct rv7xx_ps _rv770_get_ps_retval = { .low = { .mclk = 341234120 }};
u32 _cypress_get_strobe_mode_settings_retval = 2314214312;
u32 _CG_SEQ_REQ_retval = 77458381;
u32 _SEQ_CG_RESP_retval = 998124;

int _RREG32_call_count = 0;

// test params
struct radeon_device _rdev = {
  .family = 0,
  .num_crtc = 3,
  .usec_timeout = 2,
};
struct radeon_ps _radeon_boot_state = {};
bool _RREG32_retval_1 = false;
bool _RREG32_retval_2 = false;
bool _RREG32_retval_3 = false;
bool PMG_PWRSTATE = false;

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
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family
  )
{
  callIdx = 0;
  for (int i = 0; i < 25; i++) {
    calls[i] = NULL;
  }

  _RREG32_call_count = 0;
  _RREG32_retval_1 = rreg32_retval1;
  _RREG32_retval_2 = rreg32_retval2;
  _RREG32_retval_3 = rreg32_retval3;
  PMG_PWRSTATE = pmg_pwrstate;

  _rdev.family = rdev_family;
}

void cleanupState()
{
  for (int i = 0; i < 25; i++)
  {
    if (calls[i])
    {
      free(calls[i]->params);
      free(calls[i]);
    }
  }
}

// scaffold functions
struct rv7xx_ps *rv770_get_ps(struct radeon_ps *radeon_boot_state) {
  log_funcall("rv770_get_ps", 1, radeon_boot_state);
  return &_rv770_get_ps_retval;
};

bool RREG32(int status){
  assert(status == MC_SEQ_STATUS_M);
  log_funcall("RREG32", 1, status);

  _RREG32_call_count++;
  return _RREG32_call_count == 1
    ? _RREG32_retval_1
    : (_RREG32_call_count == 2)
      ? _RREG32_retval_2
      : _RREG32_retval_3;
}

void radeon_atom_set_ac_timing(struct radeon_device *rdev, int mclk){
  assert(rdev == &_rdev);
  assert(mclk == _rv770_get_ps_retval.low.mclk);
  log_funcall("radeon_atom_set_ac_timing", 2, rdev, mclk);
}

void radeon_mc_wait_for_idle(struct radeon_device *rdev){
  assert(rdev == &_rdev);
  log_funcall("radeon_mc_wait_for_idle", 1, rdev);
}

void WREG32(int config, int value){
  log_funcall("WREG32", 2, config, value);
}

void radeon_wait_for_vblank(struct radeon_device *rdev, int i){
  assert(rdev == &_rdev);
  log_funcall("radeon_wait_for_vblank", 2, rdev, i);
}

void cypress_wait_for_mc_sequencer(struct radeon_device *rdev, int value){
  assert(rdev == &_rdev);
  log_funcall("cypress_wait_for_mc_sequencer", 2, rdev, value);
}

u32 cypress_get_strobe_mode_settings(struct radeon_device *rdev, int mclk){
  assert(rdev == &_rdev);
  assert(mclk == _rv770_get_ps_retval.low.mclk);
  log_funcall("cypress_get_strobe_mode_settings", 2, rdev, mclk);
  return _cypress_get_strobe_mode_settings_retval;
}

u32 CG_SEQ_REQ(int conf){
  log_funcall("CG_SEQ_REQ", 1, conf);
  return _CG_SEQ_REQ_retval;
}

u32 SEQ_CG_RESP(u32 mode){
  assert(mode == _cypress_get_strobe_mode_settings_retval);
  log_funcall("SEQ_CG_RESP", 1, mode);
  return _SEQ_CG_RESP_retval;
}

void udelay(int v){
  assert(v == 1);
  log_funcall("udelay", 1, v);
}

#START OF SAMPLE
static void cypress_force_mc_use_s1(struct radeon_device *rdev,
            struct radeon_ps *radeon_boot_state)
{
  struct rv7xx_ps *boot_state = rv770_get_ps(radeon_boot_state);
  u32 strobe_mode;
  u32 mc_seq_cg;
  int i;

  if (RREG32(MC_SEQ_STATUS_M) & PMG_PWRSTATE)
    return;

  radeon_atom_set_ac_timing(rdev, boot_state->low.mclk);
  radeon_mc_wait_for_idle(rdev);

  if ((rdev->family == CHIP_CYPRESS) ||
      (rdev->family == CHIP_HEMLOCK)) {
    WREG32(MC_CONFIG_MCD, 0xf);
    WREG32(MC_CG_CONFIG_MCD, 0xf);
  } else {
    WREG32(MC_CONFIG, 0xf);
    WREG32(MC_CG_CONFIG, 0xf);
  }

  for (i = 0; i < rdev->num_crtc; i++)
    radeon_wait_for_vblank(rdev, i);

  WREG32(MC_SEQ_CG, MC_CG_SEQ_YCLK_SUSPEND);
  cypress_wait_for_mc_sequencer(rdev, MC_CG_SEQ_YCLK_SUSPEND);

  strobe_mode = cypress_get_strobe_mode_settings(rdev,
                   boot_state->low.mclk);

  mc_seq_cg = CG_SEQ_REQ(MC_CG_SEQ_DRAMCONF_S1);
  mc_seq_cg |= SEQ_CG_RESP(strobe_mode);
  WREG32(MC_SEQ_CG, mc_seq_cg);

  for (i = 0; i < rdev->usec_timeout; i++) {
    if (RREG32(MC_SEQ_STATUS_M) & PMG_PWRSTATE)
      break;
    udelay(1);
  }

  mc_seq_cg &= ~CG_SEQ_REQ_MASK;
  mc_seq_cg |= CG_SEQ_REQ(MC_CG_SEQ_YCLK_RESUME);
  WREG32(MC_SEQ_CG, mc_seq_cg);

  cypress_wait_for_mc_sequencer(rdev, MC_CG_SEQ_YCLK_RESUME);
}
#END OF SAMPLE


void test_branch_false_false_false_false(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2]->name == "radeon_atom_set_ac_timing");
  assert(calls[3]->name == "radeon_mc_wait_for_idle");
  assert(calls[4]->name == "WREG32");
    assert((int64_t)calls[4]->params[0] == MC_CONFIG);
    assert((int64_t)calls[4]->params[1] == 0xf);
  assert(calls[5]->name == "WREG32");
    assert((int64_t)calls[5]->params[0] == MC_CG_CONFIG);
    assert((int64_t)calls[5]->params[1] == 0xf);
  assert(calls[6]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[6]->params[1] == 0);
  assert(calls[7]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[7]->params[1] == 1);
  assert(calls[8]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[8]->params[1] == 2);
  assert(calls[9]->name == "WREG32");
    assert((int64_t)calls[9]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[9]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[10]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[10]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[11]->name == "cypress_get_strobe_mode_settings");
  assert(calls[12]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[12]->params[0] == MC_CG_SEQ_DRAMCONF_S1);
  assert(calls[13]->name == "SEQ_CG_RESP");
  assert(calls[14]->name == "WREG32");
    assert((int64_t)calls[14]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[14]->params[1] == (_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval));
  assert(calls[15]->name == "RREG32");
  assert(calls[16]->name == "udelay");
  assert(calls[17]->name == "RREG32");
  assert(calls[18]->name == "udelay");
  assert(calls[19]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[19]->params[0] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[20]->name == "WREG32");
    assert((int64_t)calls[20]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[20]->params[1] == (((_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval) & ~CG_SEQ_REQ_MASK) | _CG_SEQ_REQ_retval) );
  assert(calls[21]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[21]->params[1] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[22] == NULL);
  cleanupState();
}

void test_branch_false_false_false_true(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2]->name == "radeon_atom_set_ac_timing");
  assert(calls[3]->name == "radeon_mc_wait_for_idle");
  assert(calls[4]->name == "WREG32");
    assert((int64_t)calls[4]->params[0] == MC_CONFIG);
    assert((int64_t)calls[4]->params[1] == 0xf);
  assert(calls[5]->name == "WREG32");
    assert((int64_t)calls[5]->params[0] == MC_CG_CONFIG);
    assert((int64_t)calls[5]->params[1] == 0xf);
  assert(calls[6]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[6]->params[1] == 0);
  assert(calls[7]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[7]->params[1] == 1);
  assert(calls[8]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[8]->params[1] == 2);
  assert(calls[9]->name == "WREG32");
    assert((int64_t)calls[9]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[9]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[10]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[10]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[11]->name == "cypress_get_strobe_mode_settings");
  assert(calls[12]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[12]->params[0] == MC_CG_SEQ_DRAMCONF_S1);
  assert(calls[13]->name == "SEQ_CG_RESP");
  assert(calls[14]->name == "WREG32");
    assert((int64_t)calls[14]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[14]->params[1] == (_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval));
  assert(calls[15]->name == "RREG32");
  assert(calls[16]->name == "udelay");
  assert(calls[17]->name == "RREG32");
  assert(calls[18]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[18]->params[0] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[19]->name == "WREG32");
    assert((int64_t)calls[19]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[19]->params[1] == (((_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval) & ~CG_SEQ_REQ_MASK) | _CG_SEQ_REQ_retval) );
  assert(calls[20]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[20]->params[1] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[21] == NULL);
  cleanupState();
}

void test_branch_false_false_true_false(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2]->name == "radeon_atom_set_ac_timing");
  assert(calls[3]->name == "radeon_mc_wait_for_idle");
  assert(calls[4]->name == "WREG32");
    assert((int64_t)calls[4]->params[0] == MC_CONFIG);
    assert((int64_t)calls[4]->params[1] == 0xf);
  assert(calls[5]->name == "WREG32");
    assert((int64_t)calls[5]->params[0] == MC_CG_CONFIG);
    assert((int64_t)calls[5]->params[1] == 0xf);
  assert(calls[6]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[6]->params[1] == 0);
  assert(calls[7]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[7]->params[1] == 1);
  assert(calls[8]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[8]->params[1] == 2);
  assert(calls[9]->name == "WREG32");
    assert((int64_t)calls[9]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[9]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[10]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[10]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[11]->name == "cypress_get_strobe_mode_settings");
  assert(calls[12]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[12]->params[0] == MC_CG_SEQ_DRAMCONF_S1);
  assert(calls[13]->name == "SEQ_CG_RESP");
  assert(calls[14]->name == "WREG32");
    assert((int64_t)calls[14]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[14]->params[1] == (_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval));
  assert(calls[15]->name == "RREG32");
  assert(calls[16]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[16]->params[0] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[17]->name == "WREG32");
    assert((int64_t)calls[17]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[17]->params[1] == (((_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval) & ~CG_SEQ_REQ_MASK) | _CG_SEQ_REQ_retval) );
  assert(calls[18]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[18]->params[1] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[19] == NULL);
  cleanupState();
}

void test_branch_false_true_false_false(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2]->name == "radeon_atom_set_ac_timing");
  assert(calls[3]->name == "radeon_mc_wait_for_idle");
  assert(calls[4]->name == "WREG32");
    assert((int64_t)calls[4]->params[0] == MC_CONFIG_MCD);
    assert((int64_t)calls[4]->params[1] == 0xf);
  assert(calls[5]->name == "WREG32");
    assert((int64_t)calls[5]->params[0] == MC_CG_CONFIG_MCD);
    assert((int64_t)calls[5]->params[1] == 0xf);
  assert(calls[6]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[6]->params[1] == 0);
  assert(calls[7]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[7]->params[1] == 1);
  assert(calls[8]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[8]->params[1] == 2);
  assert(calls[9]->name == "WREG32");
    assert((int64_t)calls[9]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[9]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[10]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[10]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[11]->name == "cypress_get_strobe_mode_settings");
  assert(calls[12]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[12]->params[0] == MC_CG_SEQ_DRAMCONF_S1);
  assert(calls[13]->name == "SEQ_CG_RESP");
  assert(calls[14]->name == "WREG32");
    assert((int64_t)calls[14]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[14]->params[1] == (_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval));
  assert(calls[15]->name == "RREG32");
  assert(calls[16]->name == "udelay");
  assert(calls[17]->name == "RREG32");
  assert(calls[18]->name == "udelay");
  assert(calls[19]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[19]->params[0] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[20]->name == "WREG32");
    assert((int64_t)calls[20]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[20]->params[1] == (((_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval) & ~CG_SEQ_REQ_MASK) | _CG_SEQ_REQ_retval) );
  assert(calls[21]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[21]->params[1] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[22] == NULL);
  cleanupState();
}

void test_branch_false_true_false_true(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2]->name == "radeon_atom_set_ac_timing");
  assert(calls[3]->name == "radeon_mc_wait_for_idle");
  assert(calls[4]->name == "WREG32");
    assert((int64_t)calls[4]->params[0] == MC_CONFIG_MCD);
    assert((int64_t)calls[4]->params[1] == 0xf);
  assert(calls[5]->name == "WREG32");
    assert((int64_t)calls[5]->params[0] == MC_CG_CONFIG_MCD);
    assert((int64_t)calls[5]->params[1] == 0xf);
  assert(calls[6]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[6]->params[1] == 0);
  assert(calls[7]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[7]->params[1] == 1);
  assert(calls[8]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[8]->params[1] == 2);
  assert(calls[9]->name == "WREG32");
    assert((int64_t)calls[9]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[9]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[10]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[10]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[11]->name == "cypress_get_strobe_mode_settings");
  assert(calls[12]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[12]->params[0] == MC_CG_SEQ_DRAMCONF_S1);
  assert(calls[13]->name == "SEQ_CG_RESP");
  assert(calls[14]->name == "WREG32");
    assert((int64_t)calls[14]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[14]->params[1] == (_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval));
  assert(calls[15]->name == "RREG32");
  assert(calls[16]->name == "udelay");
  assert(calls[17]->name == "RREG32");
  assert(calls[18]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[18]->params[0] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[19]->name == "WREG32");
    assert((int64_t)calls[19]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[19]->params[1] == (((_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval) & ~CG_SEQ_REQ_MASK) | _CG_SEQ_REQ_retval) );
  assert(calls[20]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[20]->params[1] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[21] == NULL);
  cleanupState();
}

void test_branch_false_true_true_false(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2]->name == "radeon_atom_set_ac_timing");
  assert(calls[3]->name == "radeon_mc_wait_for_idle");
  assert(calls[4]->name == "WREG32");
    assert((int64_t)calls[4]->params[0] == MC_CONFIG_MCD);
    assert((int64_t)calls[4]->params[1] == 0xf);
  assert(calls[5]->name == "WREG32");
    assert((int64_t)calls[5]->params[0] == MC_CG_CONFIG_MCD);
    assert((int64_t)calls[5]->params[1] == 0xf);
  assert(calls[6]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[6]->params[1] == 0);
  assert(calls[7]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[7]->params[1] == 1);
  assert(calls[8]->name == "radeon_wait_for_vblank");
    assert((int64_t)calls[8]->params[1] == 2);
  assert(calls[9]->name == "WREG32");
    assert((int64_t)calls[9]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[9]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[10]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[10]->params[1] == MC_CG_SEQ_YCLK_SUSPEND);
  assert(calls[11]->name == "cypress_get_strobe_mode_settings");
  assert(calls[12]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[12]->params[0] == MC_CG_SEQ_DRAMCONF_S1);
  assert(calls[13]->name == "SEQ_CG_RESP");
  assert(calls[14]->name == "WREG32");
    assert((int64_t)calls[14]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[14]->params[1] == (_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval));
  assert(calls[15]->name == "RREG32");
  assert(calls[16]->name == "CG_SEQ_REQ");
    assert((int64_t)calls[16]->params[0] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[17]->name == "WREG32");
    assert((int64_t)calls[17]->params[0] == MC_SEQ_CG);
    assert((int64_t)calls[17]->params[1] == (((_CG_SEQ_REQ_retval | _SEQ_CG_RESP_retval) & ~CG_SEQ_REQ_MASK) | _CG_SEQ_REQ_retval) );
  assert(calls[18]->name == "cypress_wait_for_mc_sequencer");
    assert((int64_t)calls[18]->params[1] == MC_CG_SEQ_YCLK_RESUME);
  assert(calls[19] == NULL);
  cleanupState();
}

void test_branch_true_false_false_false(
  bool rreg32_retval1,
  bool rreg32_retval2,
  bool rreg32_retval3,
  bool pmg_pwrstate,
  int rdev_family)
{
  initState(rreg32_retval1, rreg32_retval2, rreg32_retval3, pmg_pwrstate, rdev_family);
  cypress_force_mc_use_s1(&_rdev, &_radeon_boot_state);
  assert(calls[0]->name == "rv770_get_ps");
    assert((struct radeon_ps *)calls[0]->params[0] == &_radeon_boot_state);
  assert(calls[1]->name == "RREG32");
  assert(calls[2] == NULL);
  cleanupState();
}

int main(void)
{
  test_branch_false_false_false_false(false, false, false, false, 14);
  test_branch_false_false_false_false(false, false, false, true, 14);
  test_branch_false_false_false_false(false, true, true, false, 14);
  test_branch_false_false_false_false(true, false, false, false, 14);
  test_branch_false_false_false_false(true, true, true, false, 14);

  test_branch_false_false_false_true(false, false, true, true, 14);

  test_branch_false_false_true_false(false, true, false, true, 14);

  test_branch_false_true_false_false(false, false, false, false, CHIP_CYPRESS);
  test_branch_false_true_false_false(false, false, false, false, CHIP_HEMLOCK);

  test_branch_false_true_false_true(false, false, true, true, CHIP_CYPRESS);
  test_branch_false_true_false_true(false, false, true, true, CHIP_HEMLOCK);

  test_branch_false_true_true_false(false, true, false, true, CHIP_CYPRESS);
  test_branch_false_true_true_false(false, true, false, true, CHIP_HEMLOCK);

  test_branch_true_false_false_false(true, false, false, true, 14);
}
