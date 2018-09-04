#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
struct {int coverage[10000];} TestCoverage;
typedef int s32;
typedef uint64_t u64;

// test params
bool _wf_sensor_get_temp_succeeds = false;
bool _wf_sensor_get_volts_succeeds = false;
bool _wf_sensor_get_amps_succeeds = false;
s32 _temp = -1;
s32 _power = -1;
int _ret = 100;

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
  int wf_sensor_get_temp_succeeds,
  int wf_sensor_get_volts_succeeds,
  int wf_sensor_get_amps_succeeds)
{
  _ret = 100;
  _temp = -1;
  _power = -1;
  callIdx = 0;
  for (int i = 0; i < 16; i++) {
    calls[i] = NULL;
  }
  _wf_sensor_get_temp_succeeds = wf_sensor_get_temp_succeeds;
  _wf_sensor_get_volts_succeeds = wf_sensor_get_volts_succeeds;
  _wf_sensor_get_amps_succeeds = wf_sensor_get_amps_succeeds;
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
const int EIO = 374124;

int _cpu = 2;
int sens_cpu_temp[] = {124, 724, 3, 21, 1317};
int sens_cpu_volts[] = {772, 41, 8211, 990, 13};
int sens_cpu_amps[] = {557, 54, 918, 480, 7};
s32 _wf_sensor_get_temp_outval = 28347192;
s32 _wf_sensor_get_volts_outval = 93419;
s32 _wf_sensor_get_amps_outval = 13723;
int _power_expected = 19561;
int _FIX32TOPRINT_retval = 21747;

int wf_sensor_get(int stat, s32 *out){
  log_funcall("wf_sensor_get", 1, stat);
  if(stat == sens_cpu_temp[_cpu]){
    *out = _wf_sensor_get_temp_outval;
    return _wf_sensor_get_temp_succeeds ? 0 : -1;
  }
  if(stat == sens_cpu_volts[_cpu]){
    *out = _wf_sensor_get_volts_outval;
    return _wf_sensor_get_volts_succeeds ? 0 : -1;
  }
  if(stat == sens_cpu_amps[_cpu]){
    *out = _wf_sensor_get_amps_outval;
    return _wf_sensor_get_amps_succeeds ? 0 : -1;
  }
}

int FIX32TOPRINT(s32 stat){
  log_funcall("FIX32TOPRINT", 1, stat);
  return _FIX32TOPRINT_retval;
}

void DBG_LOTS(char* fstring, int cpu, int stat){
  assert(cpu == _cpu);
  assert(stat == _FIX32TOPRINT_retval);
  log_funcall("DBG_LOTS", 3, fstring, cpu, stat);
}

void DBG(char *fstring, int cpu){
   log_funcall("DBG", 2, fstring, cpu);
}

#START OF SAMPLE
static int read_one_cpu_vals(int cpu, s32 *temp, s32 *power)
{
  s32 dtemp, volts, amps;
  int rc;

  /* Get diode temperature */
  rc = wf_sensor_get(sens_cpu_temp[cpu], &dtemp);
  if (rc) {
    DBG("  CPU%d: temp reading error !\n", cpu);
    return -EIO;
  }
  DBG_LOTS("  CPU%d: temp   = %d.%03d\n", cpu, FIX32TOPRINT((dtemp)));
  *temp = dtemp;

  /* Get voltage */
  rc = wf_sensor_get(sens_cpu_volts[cpu], &volts);
  if (rc) {
    DBG("  CPU%d, volts reading error !\n", cpu);
    return -EIO;
  }
  DBG_LOTS("  CPU%d: volts  = %d.%03d\n", cpu, FIX32TOPRINT((volts)));

  /* Get current */
  rc = wf_sensor_get(sens_cpu_amps[cpu], &amps);
  if (rc) {
    DBG("  CPU%d, current reading error !\n", cpu);
    return -EIO;
  }
  DBG_LOTS("  CPU%d: amps   = %d.%03d\n", cpu, FIX32TOPRINT((amps)));

  /* Calculate power */

  /* Scale voltage and current raw sensor values according to fixed scales
   * obtained in Darwin and calculate power from I and V
   */
  *power = (((u64)volts) * ((u64)amps)) >> 16;

  DBG_LOTS("  CPU%d: power  = %d.%03d\n", cpu, FIX32TOPRINT((*power)));

  return 0;

}
#END OF SAMPLE

int main(void)
{
  initState(true, true, true);
  _ret = read_one_cpu_vals(_cpu, &_temp, &_power);
  assert(_ret == 0);
  assert(_temp == _wf_sensor_get_temp_outval);
  assert(_power == _power_expected);
  assert(calls[0]->name == "wf_sensor_get");
    assert((int64_t)calls[0]->params[0] == sens_cpu_temp[_cpu]);
  assert(calls[1]->name == "FIX32TOPRINT");
    assert((int64_t)calls[1]->params[0] == _wf_sensor_get_temp_outval);
  assert(calls[2]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[2]->params[0], "  CPU%d: temp   = %d.%03d\n") == 0);
  assert(calls[3]->name == "wf_sensor_get");
    assert((int64_t)calls[3]->params[0] == sens_cpu_volts[_cpu]);
  assert(calls[4]->name == "FIX32TOPRINT");
    assert((int64_t)calls[4]->params[0] == _wf_sensor_get_volts_outval);
  assert(calls[5]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[5]->params[0], "  CPU%d: volts  = %d.%03d\n") == 0);
  assert(calls[6]->name == "wf_sensor_get");
    assert((int64_t)calls[6]->params[0] == sens_cpu_amps[_cpu]);
  assert(calls[7]->name == "FIX32TOPRINT");
    assert((int64_t)calls[7]->params[0] == _wf_sensor_get_amps_outval);
  assert(calls[8]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[8]->params[0], "  CPU%d: amps   = %d.%03d\n") == 0);
  assert(calls[9]->name == "FIX32TOPRINT");
    assert((int64_t)calls[9]->params[0] == _power_expected);
  assert(calls[10]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[10]->params[0], "  CPU%d: power  = %d.%03d\n") == 0);
  assert(calls[11] == NULL);
  cleanupState();

  initState(true, true, false);
  _ret = read_one_cpu_vals(_cpu, &_temp, &_power);
  assert(_ret == -EIO);
  assert(_temp == _wf_sensor_get_temp_outval);
  assert(_power == -1);
  assert(calls[0]->name == "wf_sensor_get");
    assert((int64_t)calls[0]->params[0] == sens_cpu_temp[_cpu]);
  assert(calls[1]->name == "FIX32TOPRINT");
    assert((int64_t)calls[1]->params[0] == _wf_sensor_get_temp_outval);
  assert(calls[2]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[2]->params[0], "  CPU%d: temp   = %d.%03d\n") == 0);
  assert(calls[3]->name == "wf_sensor_get");
    assert((int64_t)calls[3]->params[0] == sens_cpu_volts[_cpu]);
  assert(calls[4]->name == "FIX32TOPRINT");
    assert((int64_t)calls[4]->params[0] == _wf_sensor_get_volts_outval);
  assert(calls[5]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[5]->params[0], "  CPU%d: volts  = %d.%03d\n") == 0);
  assert(calls[6]->name == "wf_sensor_get");
    assert((int64_t)calls[6]->params[0] == sens_cpu_amps[_cpu]);
  assert(calls[7]->name == "DBG");
    assert(strcmp((char *)calls[7]->params[0], "  CPU%d, current reading error !\n") == 0);
  assert(calls[8] == NULL);
  cleanupState();

  initState(true, false, true);
  _ret = read_one_cpu_vals(_cpu, &_temp, &_power);
  assert(_ret == -EIO);
  assert(_temp == _wf_sensor_get_temp_outval);
  assert(_power == -1);
  assert(calls[0]->name == "wf_sensor_get");
    assert((int64_t)calls[0]->params[0] == sens_cpu_temp[_cpu]);
  assert(calls[1]->name == "FIX32TOPRINT");
    assert((int64_t)calls[1]->params[0] == _wf_sensor_get_temp_outval);
  assert(calls[2]->name == "DBG_LOTS");
    assert(strcmp((char *)calls[2]->params[0], "  CPU%d: temp   = %d.%03d\n") == 0);
  assert(calls[3]->name == "wf_sensor_get");
    assert((int64_t)calls[3]->params[0] == sens_cpu_volts[_cpu]);
  assert(calls[4]->name == "DBG");
    assert(strcmp((char *)calls[4]->params[0], "  CPU%d, volts reading error !\n") == 0);
  assert(calls[5] == NULL);
  cleanupState();

  initState(false, true, true);
  _ret = read_one_cpu_vals(_cpu, &_temp, &_power);
  assert(_ret == -EIO);
  assert(_temp == -1);
  assert(_power == -1);
  assert(calls[0]->name == "wf_sensor_get");
    assert((int64_t)calls[0]->params[0] == sens_cpu_temp[_cpu]);
  assert(calls[1]->name == "DBG");
    assert(strcmp((char *)calls[1]->params[0], "  CPU%d: temp reading error !\n") == 0);
  assert(calls[2] == NULL);
  cleanupState();
}
