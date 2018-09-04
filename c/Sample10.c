#include <assert.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
struct {int coverage[10000];} TestCoverage;
int MODE = 13654;
int I_INO = 12352;

int SIZE_CHANGE = 13;
int D_NAME_LEN = 8;
int OLD_DIR_SIZE = 5432;
int OLD_MTIME = 4123941;
int OLD_CTIME = 4312891;
int NEW_MTIME = 6342123;

int NO_BUDGET = 1;
int NEW_INODE_FAILED = 2;
int INIT_SECURITY_FAILED = 3;
int JNL_UPDATE_FAILED = 4;

struct ubifs_info {
  bool fail_on_budget;
  bool fail_new_inode;
  bool fail_on_jnl_update;
};
struct ubifs_budget_req {
  int new_ino;
  int new_dent;
  int dirtied_ino;
};
struct ubifs_inode {
  int ui_mutex;
  int ui_size;
};

struct inode_1 {
  struct ubifs_info *s_fs_info;
};
struct inode {
  int i_ino;
  int i_size;
  int i_mtime;
  int i_ctime;
  struct inode_1 *i_sb;

  bool fail_on_init_security;
};
struct string {
  int len;
};
struct dentry {
  struct string d_name;
};

typedef long long umode_t;

char* fn_calls[50];
int fni;

int CALC_DENT_SIZE(int len) {
  fn_calls[fni] = "CALC_DENT_SIZE";
  fni++;
  return SIZE_CHANGE;
}
struct ubifs_inode* ubifs_inode(struct inode *dir) {
  fn_calls[fni] = "ubifs_inode";
  fni++;
  return malloc(sizeof(ubifs_inode));
}
void dbg_gen(char* s, struct dentry *dentry, umode_t mode, int i_ino) {
  fn_calls[fni] = "dbg_gen";
  fni++;
  assert(s == "dent '%pd', mode %#hx in dir ino %lu");
  // TODO: assert something about dentry
  assert(mode == MODE);
  assert(i_ino == I_INO);
}

int ubifs_budget_space(struct ubifs_info *c, struct ubifs_budget_req *req) {
  fn_calls[fni] = "ubifs_budget_space";
  fni++;
  if(c->fail_on_budget) {
    return NO_BUDGET;
  } else {
    return 0;
  }
}

struct inode* ubifs_new_inode(struct ubifs_info *c, struct inode *dir, umode_t mode) {
  fn_calls[fni] = "ubifs_new_inode";
  fni++;
  if(c->fail_new_inode) {
    return NULL;
  } else {
    struct inode* ret = malloc(sizeof(struct inode));
    ret->i_ctime = NEW_MTIME;
    return ret;
  }
}
int IS_ERR(struct inode *inode) {
  fn_calls[fni] = "IS_ERR";
  fni++;
  return inode == NULL;
}
int PTR_ERR(struct inode *inode) {
  fn_calls[fni] = "PTR_ERR";
  fni++;
  return NEW_INODE_FAILED;
}

int ubifs_init_security(struct inode *dir, struct inode *inode, struct string* d_name) {
  fn_calls[fni] = "ubifs_init_security";
  fni++;
  // TODO: assert about dir, inode, d_name
  if(dir->fail_on_init_security) {
    return INIT_SECURITY_FAILED;
  } else {
    return 0;
  }
}

void mutex_lock(int* ui_mutex) {
  fn_calls[fni] = "mutex_lock";
  fni++;
}

int ubifs_jnl_update(struct ubifs_info *c, struct inode *dir, struct string* d_name, struct inode* inode, int unused1, int unused2) {
  fn_calls[fni] = "ubifs_jnl_update";
  fni++;
  if(c->fail_on_jnl_update) {
    return JNL_UPDATE_FAILED;
  } else {
    return 0;
  }
}
void mutex_unlock(int* ui_mutex) {
  fn_calls[fni] = "mutex_unlock";
  fni++;
}
void ubifs_release_budget(struct ubifs_info* c, struct ubifs_budget_req* req) {
  fn_calls[fni] = "ubifs_release_budget";
  fni++;
}
void insert_inode_hash(struct inode *inode) {
  fn_calls[fni] = "insert_inode_hash";
  fni++;
}
void d_instantiate(struct dentry *dentry, struct inode *inode) {
  fn_calls[fni] = "d_instantiate";
  fni++;
}
void make_bad_inode(struct inode *inode) {
  fn_calls[fni] = "make_bad_inode";
  fni++;
}
void iput(struct inode *inode) {
  fn_calls[fni] = "iput";
  fni++;
}

void ubifs_err(struct ubifs_info *c, char* fmt, int err) {
  fn_calls[fni] = "ubifs_err";
  fni++;
}


#START OF SAMPLE
static int ubifs_create(struct inode *dir, struct dentry *dentry, umode_t mode,
    bool excl)
{
  struct inode *inode;
  struct ubifs_info *c = dir->i_sb->s_fs_info;
  int err, sz_change = CALC_DENT_SIZE(dentry->d_name.len);
  struct ubifs_budget_req req = { .new_ino = 1, .new_dent = 1,
    .dirtied_ino = 1 };
  struct ubifs_inode *dir_ui = ubifs_inode(dir);

  /*
   *    * Budget request settings: new inode, new direntry, changing the
   *       * parent directory inode.
   *          */

  dbg_gen("dent '%pd', mode %#hx in dir ino %lu",
      dentry, mode, dir->i_ino);

  err = ubifs_budget_space(c, &req);
  if (err)
    return err;

  inode = ubifs_new_inode(c, dir, mode);
  if (IS_ERR(inode)) {
    err = PTR_ERR(inode);
    goto out_budg;
  }

  err = ubifs_init_security(dir, inode, &dentry->d_name);
  if (err)
    goto out_inode;

  mutex_lock(&dir_ui->ui_mutex);
  dir->i_size += sz_change;
  dir_ui->ui_size = dir->i_size;
  dir->i_mtime = dir->i_ctime = inode->i_ctime;
  err = ubifs_jnl_update(c, dir, &dentry->d_name, inode, 0, 0);
  if (err)
    goto out_cancel;
  mutex_unlock(&dir_ui->ui_mutex);

  ubifs_release_budget(c, &req);
  insert_inode_hash(inode);
  d_instantiate(dentry, inode);
  return 0;

out_cancel:
  dir->i_size -= sz_change;
  dir_ui->ui_size = dir->i_size;
  mutex_unlock(&dir_ui->ui_mutex);
out_inode:
  make_bad_inode(inode);
  iput(inode);
out_budg:
  ubifs_release_budget(c, &req);
  ubifs_err(c, "cannot create regular file, error %d", err);
  return err;
}
#END OF SAMPLE

void test_no_budget() {
  struct inode dir;
  struct inode_1 i_sb;
  struct ubifs_info s_fs_info;
  dir.i_ino = I_INO;
  dir.i_size = OLD_DIR_SIZE;
  dir.i_mtime = OLD_MTIME;
  dir.i_ctime = OLD_CTIME;
  dir.i_sb = &i_sb;
  dir.i_sb->s_fs_info = &s_fs_info;
  dir.i_sb->s_fs_info->fail_on_budget = true;

  struct dentry dentry;
  struct string d_name;
  dentry.d_name = d_name;
  dentry.d_name.len = D_NAME_LEN;

  umode_t mode = MODE;
  bool excl = true;
  fni = 0;

  assert(ubifs_create(&dir, &dentry, mode, excl) == NO_BUDGET);

  assert(fn_calls[0] == "CALC_DENT_SIZE");
  assert(fn_calls[1] == "ubifs_inode");
  assert(fn_calls[2] == "dbg_gen");
  assert(fn_calls[3] == "ubifs_budget_space");
  assert(fni == 4);

  assert(dir.i_size == OLD_DIR_SIZE);
  assert(dir.i_mtime == OLD_MTIME);
  assert(dir.i_ctime == OLD_CTIME);
}

void test_new_inode_failed() {
  struct inode dir;
  struct inode_1 i_sb;
  struct ubifs_info s_fs_info;
  dir.i_ino = I_INO;
  dir.i_size = OLD_DIR_SIZE;
  dir.i_mtime = OLD_MTIME;
  dir.i_ctime = OLD_CTIME;
  dir.i_sb = &i_sb;
  dir.i_sb->s_fs_info = &s_fs_info;
  dir.i_sb->s_fs_info->fail_on_budget = false;
  dir.i_sb->s_fs_info->fail_new_inode = true;

  struct dentry dentry;
  struct string d_name;
  dentry.d_name = d_name;
  dentry.d_name.len = D_NAME_LEN;

  umode_t mode = MODE;
  bool excl = true;
  fni = 0;
  assert(ubifs_create(&dir, &dentry, mode, excl) == NEW_INODE_FAILED);

  assert(fn_calls[0] == "CALC_DENT_SIZE");
  assert(fn_calls[1] == "ubifs_inode");
  assert(fn_calls[2] == "dbg_gen");
  assert(fn_calls[3] == "ubifs_budget_space");
  assert(fn_calls[4] == "ubifs_new_inode");
  assert(fn_calls[5] == "IS_ERR");
  assert(fn_calls[6] == "PTR_ERR");
  assert(fn_calls[7] == "ubifs_release_budget");
  assert(fn_calls[8] == "ubifs_err");
  assert(fni == 9);

  assert(dir.i_size == OLD_DIR_SIZE);
  assert(dir.i_mtime == OLD_MTIME);
  assert(dir.i_ctime == OLD_CTIME);
}
void test_security_failed() {
  struct inode dir;
  struct inode_1 i_sb;
  struct ubifs_info s_fs_info;
  dir.i_ino = I_INO;
  dir.i_size = OLD_DIR_SIZE;
  dir.i_mtime = OLD_MTIME;
  dir.i_ctime = OLD_CTIME;
  dir.i_sb = &i_sb;
  dir.i_sb->s_fs_info = &s_fs_info;
  dir.i_sb->s_fs_info->fail_on_budget = false;
  dir.i_sb->s_fs_info->fail_new_inode = false;
  dir.fail_on_init_security = true;

  struct dentry dentry;
  struct string d_name;
  dentry.d_name = d_name;
  dentry.d_name.len = D_NAME_LEN;

  umode_t mode = MODE;
  bool excl = true;
  fni = 0;
  assert(ubifs_create(&dir, &dentry, mode, excl) == INIT_SECURITY_FAILED);

  assert(fn_calls[0] == "CALC_DENT_SIZE");
  assert(fn_calls[1] == "ubifs_inode");
  assert(fn_calls[2] == "dbg_gen");
  assert(fn_calls[3] == "ubifs_budget_space");
  assert(fn_calls[4] == "ubifs_new_inode");
  assert(fn_calls[5] == "IS_ERR");
  assert(fn_calls[6] == "ubifs_init_security");
  assert(fn_calls[7] == "make_bad_inode");
  assert(fn_calls[8] == "iput");
  assert(fn_calls[9] == "ubifs_release_budget");
  assert(fn_calls[10] == "ubifs_err");
  assert(fni == 11);

  assert(dir.i_size == OLD_DIR_SIZE);
  assert(dir.i_mtime == OLD_MTIME);
  assert(dir.i_ctime == OLD_CTIME);
}

void test_jnl_update_failed() {
  struct inode dir;
  struct inode_1 i_sb;
  struct ubifs_info s_fs_info;
  dir.i_ino = I_INO;
  dir.i_size = OLD_DIR_SIZE;
  dir.i_mtime = OLD_MTIME;
  dir.i_ctime = OLD_CTIME;
  dir.i_sb = &i_sb;
  dir.i_sb->s_fs_info = &s_fs_info;
  dir.i_sb->s_fs_info->fail_on_budget = false;
  dir.i_sb->s_fs_info->fail_new_inode = false;
  dir.i_sb->s_fs_info->fail_on_jnl_update = true;
  dir.fail_on_init_security = false;

  struct dentry dentry;
  struct string d_name;
  dentry.d_name = d_name;
  dentry.d_name.len = D_NAME_LEN;

  umode_t mode = MODE;
  bool excl = true;
  fni = 0;
  assert(ubifs_create(&dir, &dentry, mode, excl) == JNL_UPDATE_FAILED);

  assert(fn_calls[0] == "CALC_DENT_SIZE");
  assert(fn_calls[1] == "ubifs_inode");
  assert(fn_calls[2] == "dbg_gen");
  assert(fn_calls[3] == "ubifs_budget_space");
  assert(fn_calls[4] == "ubifs_new_inode");
  assert(fn_calls[5] == "IS_ERR");
  assert(fn_calls[6] == "ubifs_init_security");
  assert(fn_calls[7] == "mutex_lock");
  assert(fn_calls[8] == "ubifs_jnl_update");
  assert(fn_calls[9] == "mutex_unlock");
  assert(fn_calls[10] == "make_bad_inode");
  assert(fn_calls[11] == "iput");
  assert(fn_calls[12] == "ubifs_release_budget");
  assert(fn_calls[13] == "ubifs_err");
  assert(fni == 14);

  assert(dir.i_size == OLD_DIR_SIZE);
  assert(dir.i_mtime == NEW_MTIME);
  assert(dir.i_ctime == NEW_MTIME);
}
void test_success() {
  struct inode dir;
  struct inode_1 i_sb;
  struct ubifs_info s_fs_info;
  dir.i_ino = I_INO;
  dir.i_size = OLD_DIR_SIZE;
  dir.i_mtime = OLD_MTIME;
  dir.i_ctime = OLD_CTIME;
  dir.i_sb = &i_sb;
  dir.i_sb->s_fs_info = &s_fs_info;
  dir.i_sb->s_fs_info->fail_on_budget = false;
  dir.i_sb->s_fs_info->fail_new_inode = false;
  dir.i_sb->s_fs_info->fail_on_jnl_update = false;
  dir.fail_on_init_security = false;

  struct dentry dentry;
  struct string d_name;
  dentry.d_name = d_name;
  dentry.d_name.len = D_NAME_LEN;

  umode_t mode = MODE;
  bool excl = true;
  fni = 0;
  assert(ubifs_create(&dir, &dentry, mode, excl) == 0);

  assert(fn_calls[0] == "CALC_DENT_SIZE");
  assert(fn_calls[1] == "ubifs_inode");
  assert(fn_calls[2] == "dbg_gen");
  assert(fn_calls[3] == "ubifs_budget_space");
  assert(fn_calls[4] == "ubifs_new_inode");
  assert(fn_calls[5] == "IS_ERR");
  assert(fn_calls[6] == "ubifs_init_security");
  assert(fn_calls[7] == "mutex_lock");
  assert(fn_calls[8] == "ubifs_jnl_update");
  assert(fn_calls[9] == "mutex_unlock");
  assert(fn_calls[10] == "ubifs_release_budget");
  assert(fn_calls[11] == "insert_inode_hash");
  assert(fn_calls[12] == "d_instantiate");
  assert(fni == 13);

  assert(dir.i_size == OLD_DIR_SIZE + SIZE_CHANGE);
  assert(dir.i_mtime == NEW_MTIME);
  assert(dir.i_ctime == NEW_MTIME);
}

int main() {
  test_no_budget();
  test_new_inode_failed();
  test_security_failed();
  test_jnl_update_failed();
  test_success();
}
