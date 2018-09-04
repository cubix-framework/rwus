#include <assert.h>
#include <stdbool.h>
struct {int coverage[10000];} TestCoverage;
int XBF_WRITE = 1;
int XBF_ASYNC = 2;
int XBF_READ = 4;
int _XBF_DELWRI_Q = 8;
int XBF_WRITE_FAIL = 16;
int XBF_DONE = 32;

int SHUTDOWN_META_IO_ERROR = 1;

bool assert_ok = false;

void ASSERT(bool b) {
  assert_ok = b;
}

struct b_target {
  int bt_mount;
};

struct xfs_buf {
  int b_flags;
  struct b_target *b_target;
  int is_locked;
  int wait_error;
};

int line_no;
int xfs_buf_islocked(struct xfs_buf *bp) {
  assert(line_no == 0);
  line_no++;
  return bp->is_locked;
}
int xfs_buf_submit_wait(struct xfs_buf *bp) {
  assert(line_no == 1);
  line_no++;
  return bp->wait_error;
}
void xfs_force_shutdown(int* bt_mount, int error) {
  assert(line_no == 2);
  line_no++;
  *bt_mount = error;
}

#START OF SAMPLE
int xfs_bwrite(struct xfs_buf    *bp)
{
  int      error;

  ASSERT(xfs_buf_islocked(bp));

  bp->b_flags |= XBF_WRITE;
  bp->b_flags &= ~(XBF_ASYNC | XBF_READ | _XBF_DELWRI_Q |
      XBF_WRITE_FAIL | XBF_DONE);

  error = xfs_buf_submit_wait(bp);
  if (error) {
    xfs_force_shutdown(&bp->b_target->bt_mount,
        SHUTDOWN_META_IO_ERROR);
  }
  return error;
}
#END OF SAMPLE

void test_good() {
  struct xfs_buf bp;
  struct b_target t;
  bp.b_target = &t;
  bp.b_target->bt_mount = 0;
  bp.b_flags = XBF_ASYNC | XBF_READ | _XBF_DELWRI_Q | XBF_WRITE_FAIL | XBF_DONE;
  bp.is_locked = true;
  bp.wait_error = false;
  line_no = 0;
  assert(!xfs_bwrite(&bp));
  assert(bp.b_flags = XBF_WRITE);
  assert(assert_ok);
  assert(bp.b_target->bt_mount == 0);
  assert(line_no == 2);
}
void test_not_locked() {
  struct xfs_buf bp;
  struct b_target t;
  bp.b_target = &t;
  bp.is_locked = false;
  line_no = 0;
  assert(!xfs_bwrite(&bp));
  assert(!assert_ok);
  assert(line_no == 2);
}
void test_wait_error() {
  struct xfs_buf bp;
  struct b_target t;
  bp.b_target = &t;
  bp.b_target->bt_mount = 0;
  bp.is_locked = true;
  bp.wait_error = true;
  line_no = 0;
  assert(xfs_bwrite(&bp));
  assert(bp.b_target->bt_mount == SHUTDOWN_META_IO_ERROR);
  assert(line_no == 3);
}

int main() {
  test_good();
  test_not_locked();
  test_wait_error();
}
