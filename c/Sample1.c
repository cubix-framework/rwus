#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
struct {int coverage[10000];} TestCoverage;
// test parameters
static int _simple_num_clocks = 2;
static int _simple_clks[] = { 12, 71 };
static struct dwc3_of_simple *_devm_kzalloc_retval = NULL;
static bool _dwc3_of_simple_clk_init_succeeds = false;
static bool _of_platform_populate_succeeds = false;

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
  struct dwc3_of_simple *devm_kzalloc_retval,
  bool dwc3_of_simple_clk_init_succeeds,
  bool of_platform_populate_succeeds)
{

  for (int i = 0; i < 16; i++) {
    calls[i] = NULL;
  }
  callIdx = 0;
  _devm_kzalloc_retval = devm_kzalloc_retval;
  _dwc3_of_simple_clk_init_succeeds = dwc3_of_simple_clk_init_succeeds;
  _of_platform_populate_succeeds = of_platform_populate_succeeds;
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

const int GFP_KERNEL = 23423421;
const int ENOMEM = 75932872;
const int _of_clk_get_parent_count_retval = 83208242;
const int _dwc3_of_simple_clk_init_retval = 994921;
const int _of_platform_populate_retval = 130882;

struct device_node {
};

struct device {
  struct device_node *of_node;
};

struct dwc3_of_simple {
  struct device *dev;
  int num_clocks;
  int* clks;
};

struct platform_device {
  struct device dev;
};

struct dwc3_of_simple *devm_kzalloc(struct device *dev, int size, int gfp_kernel)
{
  log_funcall("devm_kzalloc", 3, dev, size, gfp_kernel);
  assert(gfp_kernel == GFP_KERNEL);
  assert(size == sizeof(struct dwc3_of_simple));

  return _devm_kzalloc_retval;
}

void platform_set_drvdata(struct platform_device *pdev, struct dwc3_of_simple *simple)
{
  log_funcall("platform_set_drvdata", 2, pdev, simple);
}

int of_clk_get_parent_count(struct device_node *np)
{
  log_funcall("of_clk_get_parent_count", 1, np);
  return _of_clk_get_parent_count_retval;
}

int dwc3_of_simple_clk_init(struct dwc3_of_simple *simple, int parent_count)
{
  assert(parent_count == _of_clk_get_parent_count_retval);
  log_funcall("dwc3_of_simple_clk_init", 2, simple, parent_count);
  if (_dwc3_of_simple_clk_init_succeeds)
  {
    return _dwc3_of_simple_clk_init_retval;
  } else {
    return 0;
  }
}

int of_platform_populate(struct device_node *np, void* p2, void* p3, struct device *dev)
{
  assert(p2 == NULL);
  assert(p3 == NULL);
  log_funcall("of_platform_populate", 4, np, p2, p3, dev);
  if (_of_platform_populate_succeeds)
  {
    return _of_platform_populate_retval;
  } else {
    return 0;
  }
}

void clk_disable_unprepare(int clk)
{
  log_funcall("clk_disable_unprepare", 1, clk);
}

void clk_put(int clk) {
  log_funcall("clk_put", 1, clk);
}

void pm_runtime_set_active(struct device *dev)
{
  log_funcall("pm_runtime_set_active", 1, dev);
}

void pm_runtime_enable(struct device *dev)
{
  log_funcall("pm_runtime_enable", 1, dev);
}

void pm_runtime_get_sync(struct device *dev)
{
  log_funcall("pm_runtime_get_sync", 1, dev);
}

#START OF SAMPLE
static int dwc3_of_simple_probe(struct platform_device *pdev)
{
  struct dwc3_of_simple  *simple;
  struct device    *dev = &pdev->dev;
  struct device_node  *np = dev->of_node;

  int      ret;
  int      i;

  simple = devm_kzalloc(dev, sizeof(*simple), GFP_KERNEL);
  if (!simple)
    return -ENOMEM;

  platform_set_drvdata(pdev, simple);
  simple->dev = dev;

  ret = dwc3_of_simple_clk_init(simple, of_clk_get_parent_count(np));
  if (ret)
    return ret;

  ret = of_platform_populate(np, NULL, NULL, dev);
  if (ret) {
    for (i = 0; i < simple->num_clocks; i++) {
      clk_disable_unprepare(simple->clks[i]);
      clk_put(simple->clks[i]);
    }

    return ret;
  }

  pm_runtime_set_active(dev);
  pm_runtime_enable(dev);
  pm_runtime_get_sync(dev);

  return 0;
}
#END OF SAMPLE

int main(void)
{
  struct device_node node = {};
  struct device_node *np = &node;
  struct platform_device pDev = { .dev = { .of_node = np } };
  struct device *dev = &pDev.dev;
  struct dwc3_of_simple simple = { NULL, _simple_num_clocks, _simple_clks };

  int ret;

  initState(&simple, false, false);
  ret = dwc3_of_simple_probe(&pDev);
  assert(ret == 0);
  assert(calls[0]->name == "devm_kzalloc");
    assert((struct device *)calls[0]->params[0] == dev);
  assert(calls[1]->name == "platform_set_drvdata");
    assert((struct platform_device *)calls[1]->params[0] == &pDev);
    assert((struct dwc3_of_simple *)calls[1]->params[1] == &simple);
  assert(calls[2]->name == "of_clk_get_parent_count");
    assert((struct device_node *)calls[2]->params[0] == np);
  assert(calls[3]->name == "dwc3_of_simple_clk_init");
    assert((struct dwc3_of_simple *)calls[3]->params[0] == &simple);
  assert(calls[4]->name == "of_platform_populate");
    assert((struct device_node *)calls[4]->params[0] == np);
    assert((struct device *)calls[4]->params[3] == dev);
  assert(calls[5]->name == "pm_runtime_set_active");
    assert((struct device *)calls[5]->params[0] == dev);
  assert(calls[6]->name == "pm_runtime_enable");
    assert((struct device *)calls[6]->params[0] == dev);
  assert(calls[7]->name == "pm_runtime_get_sync");
    assert((struct device *)calls[7]->params[0] == dev);
  assert(calls[8] == NULL);
  cleanupState();

  simple.dev = NULL;
  initState(&simple, false, true);
  ret = dwc3_of_simple_probe(&pDev);
  assert(ret == _of_platform_populate_retval);
  assert(calls[0]->name == "devm_kzalloc");
    assert((struct device *)calls[0]->params[0] == dev);
  assert(calls[1]->name == "platform_set_drvdata");
    assert((struct platform_device *)calls[1]->params[0] == &pDev);
    assert((struct dwc3_of_simple *)calls[1]->params[1] == &simple);
  assert(calls[2]->name == "of_clk_get_parent_count");
    assert((struct device_node *)calls[2]->params[0] == np);
  assert(calls[3]->name == "dwc3_of_simple_clk_init");
    assert((struct dwc3_of_simple *)calls[3]->params[0] == &simple);
  assert(calls[4]->name == "of_platform_populate");
    assert((struct device_node *)calls[4]->params[0] == np);
    assert((struct device *)calls[4]->params[3] == dev);
  assert(calls[5]->name == "clk_disable_unprepare");
    assert((int64_t)calls[5]->params[0] == _simple_clks[0]);
  assert(calls[6]->name == "clk_put");
    assert((int64_t)calls[6]->params[0] == _simple_clks[0]);
  assert(calls[7]->name == "clk_disable_unprepare");
    assert((int64_t)calls[7]->params[0] == _simple_clks[1]);
  assert(calls[8]->name == "clk_put");
    assert((int64_t)calls[8]->params[0] == _simple_clks[1]);
  assert(calls[9] == NULL);
  cleanupState();

  initState(&simple, true, false);
  ret = dwc3_of_simple_probe(&pDev);
  assert(ret == _dwc3_of_simple_clk_init_retval);
  assert(calls[0]->name == "devm_kzalloc");
    assert((struct device *)calls[0]->params[0] == dev);
  assert(calls[1]->name == "platform_set_drvdata");
    assert((struct platform_device *)calls[1]->params[0] == &pDev);
    assert((struct dwc3_of_simple *)calls[1]->params[1] == &simple);
  assert(calls[2]->name == "of_clk_get_parent_count");
    assert((struct device_node *)calls[2]->params[0] == np);
  assert(calls[3]->name == "dwc3_of_simple_clk_init");
    assert((struct dwc3_of_simple *)calls[3]->params[0] == &simple);
  assert(calls[4] == NULL);
  cleanupState();

  initState(NULL, false, false);
  ret = dwc3_of_simple_probe(&pDev);
  assert(ret == -ENOMEM);
  assert(calls[0]->name == "devm_kzalloc");
    assert((struct device *)calls[0]->params[0] == dev);
  assert(calls[1] == NULL);
  cleanupState();
}
