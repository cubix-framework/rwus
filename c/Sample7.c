#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
struct {int coverage[10000];} TestCoverage;
struct kref {

};

struct drbd_connection {
  struct kref kref;
};

struct drbd_resource {
  struct kref kref;
};

struct drbd_peer_device {
  struct drbd_connection *connection;
};

struct drbd_md {
  int page;
};

struct drbd_device {
  int this_bdev;
  struct drbd_resource *resource;
  int request_timer;
  int open_cnt;
  int *ldev;
  int act_log;
  int resync;
  int *p_uuid;
  int bitmap;
  struct drbd_md md_io;
  int vdisk;
  int rq_queue;
  int *rs_plan_s;
};

// constants
int drbd_destroy_connection = 2741234;
int drbd_destroy_resource = 774832;
int _device_this_bdev = 6243779;
int _device_request_timer = 99391;
int _device_open_cnt = 119220;
int _device_ldev = 320142;
int _device_act_log = 919476;
int _device_resync = 7513;
int _device_p_uuid = 871729384;
int _device_bitmap = 58499801;
int _device_vdisk = 2677829;
int _device_rq_queue = 4437172;
int _device_rs_plan_s = 86723440;
int _md_io_page = 557672651;

struct drbd_connection _connection = {.kref = {}};
struct drbd_peer_device _peer_device = {};
struct drbd_resource _resource = {.kref = {}};

// test params
struct drbd_device _device = {0};

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
  bool device_has_this_bdev,
  bool device_has_bitmap)
{
  callIdx = 0;
  for (int i = 0; i < 20; i++) {
    calls[i] = NULL;
  }

  struct drbd_md md_io = { .page = _md_io_page };
  _device.this_bdev = device_has_this_bdev ? _device_this_bdev : 0;
  _device.resource = &_resource;
  _device.request_timer = _device_request_timer;
  _device.open_cnt = _device_open_cnt;
  _device.ldev = &_device_ldev;
  _device.act_log = _device_act_log;
  _device.resync = _device_resync;
  _device.p_uuid = &_device_p_uuid;
  _device.bitmap = device_has_bitmap ? _device_bitmap : 0;
  _device.md_io = md_io;
  _device.vdisk = _device_vdisk;
  _device.rq_queue = _device_rq_queue;
  _device.rs_plan_s = &_device_rs_plan_s;

  _peer_device.connection = &_connection;
}

void cleanupState()
{
  for (int i = 0; i < 20; i++)
  {
    if (calls[i])
    {
      free(calls[i]->params);
      free(calls[i]);
    }
  }
}

// scaffold functions
struct drbd_device * _container_of(struct kref *kref, struct kref *kref2){
  log_funcall("container_of", 2, kref, kref2); \
  return &_device;
}

#define container_of(kref, type_t, kref2) ((type_t *) _container_of(kref, kref2));

void del_timer_sync(int *request_timer){
  log_funcall("del_timer_sync", 1, request_timer);
}

void D_ASSERT(struct drbd_device *device, bool predicate){
  log_funcall("D_ASSERT", 2, device, predicate);
}

void bdput(int bdev){
  log_funcall("bdput", 1, bdev);
}

void drbd_backing_dev_free(struct drbd_device *device, int *ldev){
  log_funcall("drbd_backing_dev_free", 2, device, ldev);
}

void drbd_release_all_peer_reqs(struct drbd_device *device){
  log_funcall("drbd_release_all_peer_reqs", 1, device);
}

void lc_destroy(int param){
  log_funcall("lc_destroy", 1, param);
}

void kfree(void *param){
  log_funcall("kfree", 1, param);
}

void drbd_bm_cleanup(struct drbd_device *device){
  log_funcall("drbd_bm_cleanup", 1, device);
}

void __free_page(int page){
  log_funcall("__free_page", 1, page);
}

void put_disk(int vdisk){
  log_funcall("put_disk", 1, vdisk);
}

void blk_cleanup_queue(int queue){
  log_funcall("blk_cleanup_queue", 1, queue);
}

#define for_each_peer_device_safe(peer_dev, tmp, device) \
  log_funcall("for_each_peer_device_safe", 3, peer_dev, tmp, device); \
  peer_dev = &_peer_device; \
  for(int i = 0; i < 1; i++)

void kref_put(struct kref *kref, int cmd){
  log_funcall("kref_put", 2, kref, cmd);
}

#START OF SAMPLE
void drbd_destroy_device(struct kref *kref)
{
  struct drbd_device *device = container_of(kref, struct drbd_device, kref);
  struct drbd_resource *resource = device->resource;
  struct drbd_peer_device *peer_device, *tmp_peer_device;

  del_timer_sync(&device->request_timer);

  /* paranoia asserts */
  D_ASSERT(device, device->open_cnt == 0);
  /* end paranoia asserts */

  /* cleanup stuff that may have been allocated during
   * device (re-)configuration or state changes */

  if (device->this_bdev)
    bdput(device->this_bdev);

  drbd_backing_dev_free(device, device->ldev);
  device->ldev = NULL;

  drbd_release_all_peer_reqs(device);

  lc_destroy(device->act_log);
  lc_destroy(device->resync);

  kfree(device->p_uuid);
  /* device->p_uuid = NULL; */

  if (device->bitmap) /* should no longer be there. */
    drbd_bm_cleanup(device);
  __free_page(device->md_io.page);
  put_disk(device->vdisk);
  blk_cleanup_queue(device->rq_queue);
  kfree(device->rs_plan_s);

  /* not for_each_connection(connection, resource):
   * those may have been cleaned up and disassociated already.
   */
  for_each_peer_device_safe(peer_device, tmp_peer_device, device) {
    kref_put(&peer_device->connection->kref, drbd_destroy_connection);
    kfree(peer_device);
  }
  memset(device, 0xfd, sizeof(*device));
  kfree(device);
  kref_put(&resource->kref, drbd_destroy_resource);
}
#END OF SAMPLE

int main(void)
{
  struct kref _kref = {};
  struct drbd_device _blanked_device;
  memset(&_blanked_device, 0xfd, sizeof(_blanked_device));

  initState(false, false);
  drbd_destroy_device(&_kref);
  assert(calls[0]->name == "container_of");
    assert((struct kref *)calls[0]->params[0] == &_kref);
    assert((struct kref *)calls[0]->params[1] == &_kref);
  assert(calls[1]->name == "del_timer_sync");
    assert((int *)calls[1]->params[0] == &_device.request_timer);
  assert(calls[2]->name == "D_ASSERT");
    assert((struct drbd_device *)calls[2]->params[0] == &_device);
    assert((bool)calls[2]->params[1] == false );
  assert(calls[3]->name == "drbd_backing_dev_free");
    assert((struct drbd_device *)calls[3]->params[0] == &_device);
    assert((int *)calls[3]->params[1] == &_device_ldev);
  assert(calls[4]->name == "drbd_release_all_peer_reqs");
    assert((struct drbd_device *)calls[4]->params[0] == &_device);
  assert(calls[5]->name == "lc_destroy");
    assert((uint64_t)calls[5]->params[0] == _device_act_log);
  assert(calls[6]->name == "lc_destroy");
    assert((uint64_t)calls[6]->params[0] == _device_resync);
  assert(calls[7]->name == "kfree");
    assert((int *)calls[7]->params[0] == &_device_p_uuid);
  assert(calls[8]->name == "__free_page");
    assert((int64_t)calls[8]->params[0] == _md_io_page);
  assert(calls[9]->name == "put_disk");
    assert((int64_t)calls[9]->params[0] == _device_vdisk);
  assert(calls[10]->name == "blk_cleanup_queue");
    assert((int64_t)calls[10]->params[0] == _device_rq_queue);
  assert(calls[11]->name == "kfree");
    assert((int *)calls[11]->params[0] == &_device_rs_plan_s);
  assert(calls[12]->name == "for_each_peer_device_safe");
    assert((struct drbd_device *)calls[12]->params[2] == &_device);
  assert(calls[13]->name == "kref_put");
    assert((struct kref *)calls[13]->params[0] == &_connection.kref);
    assert((uint64_t)calls[13]->params[1] == drbd_destroy_connection);
  assert(calls[14]->name == "kfree");
    assert((struct drbd_peer_device *)calls[14]->params[0] == &_peer_device);
  assert(calls[15]->name == "kfree");
    assert((struct drbd_device *)calls[15]->params[0] == &_device);
  assert(calls[16]->name == "kref_put");
    assert((struct kref *)calls[16]->params[0] == &_resource.kref);
    assert((uint64_t)calls[16]->params[1] == drbd_destroy_resource);
  assert(calls[17] == NULL);
  assert(memcmp(&_device, &_blanked_device, sizeof(_device)) == 0);
  cleanupState();

  initState(false, true);
  drbd_destroy_device(&_kref);
  assert(calls[0]->name == "container_of");
    assert((struct kref *)calls[0]->params[0] == &_kref);
    assert((struct kref *)calls[0]->params[1] == &_kref);
  assert(calls[1]->name == "del_timer_sync");
    assert((int *)calls[1]->params[0] == &_device.request_timer);
  assert(calls[2]->name == "D_ASSERT");
    assert((struct drbd_device *)calls[2]->params[0] == &_device);
    assert((bool)calls[2]->params[1] == false );
  assert(calls[3]->name == "drbd_backing_dev_free");
    assert((struct drbd_device *)calls[3]->params[0] == &_device);
    assert((int *)calls[3]->params[1] == &_device_ldev);
  assert(calls[4]->name == "drbd_release_all_peer_reqs");
    assert((struct drbd_device *)calls[4]->params[0] == &_device);
  assert(calls[5]->name == "lc_destroy");
    assert((uint64_t)calls[5]->params[0] == _device_act_log);
  assert(calls[6]->name == "lc_destroy");
    assert((uint64_t)calls[6]->params[0] == _device_resync);
  assert(calls[7]->name == "kfree");
    assert((int *)calls[7]->params[0] == &_device_p_uuid);
  assert(calls[8]->name == "drbd_bm_cleanup");
    assert((struct drbd_device *)calls[8]->params[0] == &_device);
  assert(calls[9]->name == "__free_page");
    assert((int64_t)calls[9]->params[0] == _md_io_page);
  assert(calls[10]->name == "put_disk");
    assert((int64_t)calls[10]->params[0] == _device_vdisk);
  assert(calls[11]->name == "blk_cleanup_queue");
    assert((int64_t)calls[11]->params[0] == _device_rq_queue);
  assert(calls[12]->name == "kfree");
    assert((int *)calls[12]->params[0] == &_device_rs_plan_s);
  assert(calls[13]->name == "for_each_peer_device_safe");
    assert((struct drbd_device *)calls[13]->params[2] == &_device);
  assert(calls[14]->name == "kref_put");
    assert((struct kref *)calls[14]->params[0] == &_connection.kref);
    assert((uint64_t)calls[14]->params[1] == drbd_destroy_connection);
  assert(calls[15]->name == "kfree");
    assert((struct drbd_peer_device *)calls[15]->params[0] == &_peer_device);
  assert(calls[16]->name == "kfree");
    assert((struct drbd_device *)calls[16]->params[0] == &_device);
  assert(calls[17]->name == "kref_put");
    assert((struct kref *)calls[17]->params[0] == &_resource.kref);
    assert((uint64_t)calls[17]->params[1] == drbd_destroy_resource);
  assert(calls[18] == NULL);
  assert(memcmp(&_device, &_blanked_device, sizeof(_device)) == 0);
  cleanupState();

  initState(true, false);
  drbd_destroy_device(&_kref);
  assert(calls[0]->name == "container_of");
    assert((struct kref *)calls[0]->params[0] == &_kref);
    assert((struct kref *)calls[0]->params[1] == &_kref);
  assert(calls[1]->name == "del_timer_sync");
    assert((int *)calls[1]->params[0] == &_device.request_timer);
  assert(calls[2]->name == "D_ASSERT");
    assert((struct drbd_device *)calls[2]->params[0] == &_device);
    assert((bool)calls[2]->params[1] == false );
  assert(calls[3]->name == "bdput");
    assert((int64_t)calls[3]->params[0] == _device_this_bdev);
  assert(calls[4]->name == "drbd_backing_dev_free");
    assert((struct drbd_device *)calls[4]->params[0] == &_device);
    assert((int *)calls[4]->params[1] == &_device_ldev);
  assert(calls[5]->name == "drbd_release_all_peer_reqs");
    assert((struct drbd_device *)calls[5]->params[0] == &_device);
  assert(calls[6]->name == "lc_destroy");
    assert((uint64_t)calls[6]->params[0] == _device_act_log);
  assert(calls[7]->name == "lc_destroy");
    assert((uint64_t)calls[7]->params[0] == _device_resync);
  assert(calls[8]->name == "kfree");
    assert((int *)calls[8]->params[0] == &_device_p_uuid);
  assert(calls[9]->name == "__free_page");
    assert((int64_t)calls[9]->params[0] == _md_io_page);
  assert(calls[10]->name == "put_disk");
    assert((int64_t)calls[10]->params[0] == _device_vdisk);
  assert(calls[11]->name == "blk_cleanup_queue");
    assert((int64_t)calls[11]->params[0] == _device_rq_queue);
  assert(calls[12]->name == "kfree");
    assert((int *)calls[12]->params[0] == &_device_rs_plan_s);
  assert(calls[13]->name == "for_each_peer_device_safe");
    assert((struct drbd_device *)calls[13]->params[2] == &_device);
  assert(calls[14]->name == "kref_put");
    assert((struct kref *)calls[14]->params[0] == &_connection.kref);
    assert((uint64_t)calls[14]->params[1] == drbd_destroy_connection);
  assert(calls[15]->name == "kfree");
    assert((struct drbd_peer_device *)calls[15]->params[0] == &_peer_device);
  assert(calls[16]->name == "kfree");
    assert((struct drbd_device *)calls[16]->params[0] == &_device);
  assert(calls[17]->name == "kref_put");
    assert((struct kref *)calls[17]->params[0] == &_resource.kref);
    assert((uint64_t)calls[17]->params[1] == drbd_destroy_resource);
  assert(calls[18] == NULL);
  assert(memcmp(&_device, &_blanked_device, sizeof(_device)) == 0);
  cleanupState();

  initState(true, true);
  drbd_destroy_device(&_kref);
  assert(calls[0]->name == "container_of");
    assert((struct kref *)calls[0]->params[0] == &_kref);
    assert((struct kref *)calls[0]->params[1] == &_kref);
  assert(calls[1]->name == "del_timer_sync");
    assert((int *)calls[1]->params[0] == &_device.request_timer);
  assert(calls[2]->name == "D_ASSERT");
    assert((struct drbd_device *)calls[2]->params[0] == &_device);
    assert((bool)calls[2]->params[1] == false );
  assert(calls[3]->name == "bdput");
    assert((int64_t)calls[3]->params[0] == _device_this_bdev);
  assert(calls[4]->name == "drbd_backing_dev_free");
    assert((struct drbd_device *)calls[4]->params[0] == &_device);
    assert((int *)calls[4]->params[1] == &_device_ldev);
  assert(calls[5]->name == "drbd_release_all_peer_reqs");
    assert((struct drbd_device *)calls[5]->params[0] == &_device);
  assert(calls[6]->name == "lc_destroy");
    assert((uint64_t)calls[6]->params[0] == _device_act_log);
  assert(calls[7]->name == "lc_destroy");
    assert((uint64_t)calls[7]->params[0] == _device_resync);
  assert(calls[8]->name == "kfree");
    assert((int *)calls[8]->params[0] == &_device_p_uuid);
  assert(calls[9]->name == "drbd_bm_cleanup");
    assert((struct drbd_device *)calls[9]->params[0] == &_device);
  assert(calls[10]->name == "__free_page");
    assert((int64_t)calls[10]->params[0] == _md_io_page);
  assert(calls[11]->name == "put_disk");
    assert((int64_t)calls[11]->params[0] == _device_vdisk);
  assert(calls[12]->name == "blk_cleanup_queue");
    assert((int64_t)calls[12]->params[0] == _device_rq_queue);
  assert(calls[13]->name == "kfree");
    assert((int *)calls[13]->params[0] == &_device_rs_plan_s);
  assert(calls[14]->name == "for_each_peer_device_safe");
    assert((struct drbd_device *)calls[14]->params[2] == &_device);
  assert(calls[15]->name == "kref_put");
    assert((struct kref *)calls[15]->params[0] == &_connection.kref);
    assert((uint64_t)calls[15]->params[1] == drbd_destroy_connection);
  assert(calls[16]->name == "kfree");
    assert((struct drbd_peer_device *)calls[16]->params[0] == &_peer_device);
  assert(calls[17]->name == "kfree");
    assert((struct drbd_device *)calls[17]->params[0] == &_device);
  assert(calls[18]->name == "kref_put");
    assert((struct kref *)calls[18]->params[0] == &_resource.kref);
    assert((uint64_t)calls[18]->params[1] == drbd_destroy_resource);
  assert(calls[19] == NULL);
  assert(memcmp(&_device, &_blanked_device, sizeof(_device)) == 0);
  cleanupState();
}
