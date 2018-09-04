#include <stddef.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
struct {int coverage[10000];} TestCoverage;
int PERF_ELF_C_READ_MMAP = 1;
int ET_EXEC = 2;

typedef struct _1 {
  int ended;
  int fd;
} Elf;

typedef struct GElf_Ehdr {
  int e_type;
} GElf_Ehdr;

int line_no;
Elf* elf_begin(int fd, int mode, int* unused) {
  assert(line_no == 0);
  line_no++;
  assert(mode == PERF_ELF_C_READ_MMAP);
  assert(unused == NULL);
  if(fd==0) {
    return NULL;
  } else {
    Elf* elf = (Elf*)malloc(sizeof(Elf));
    elf->fd = fd;
    return elf;
  }
}
GElf_Ehdr* gelf_getehdr(Elf* elf, GElf_Ehdr* ehdr) {
  assert(line_no == 1);
  line_no++;
  if(elf->fd==1) {
    return NULL;
  } else {
    GElf_Ehdr hdr;
    hdr.e_type = ET_EXEC;
    if(elf->fd==2) {
      hdr.e_type = 0;
    }
    *ehdr = hdr;
    return ehdr;
  }
}

int elf_ended;
void elf_end(Elf* elf) {
  assert(line_no == 2);
  line_no++;
  elf_ended = true;
}
int pr_debugged;
void pr_debug(const char* s, const char* name, int retval) {
  assert(line_no == 3);
  line_no++;
  assert(s == "unwind: elf_is_exec(%s): %d\n");
  assert(name == "NAME");
  pr_debugged = retval;
}

#START OF SAMPLE
static int elf_is_exec(int fd, const char *name)
{
  Elf *elf;
  GElf_Ehdr ehdr;
  int retval = 0;

  elf = elf_begin(fd, PERF_ELF_C_READ_MMAP, NULL);
  if (elf == NULL)
    return 0;
  if (gelf_getehdr(elf, &ehdr) == NULL)
    goto out;

  retval = (ehdr.e_type == ET_EXEC);

out:
  elf_end(elf);
  pr_debug("unwind: elf_is_exec(%s): %d\n", name, retval);
  return retval;
}
#END OF SAMPLE

void test_no_elf() {
  elf_ended = false;
  pr_debugged = -1;
  line_no = 0;
  assert(!elf_is_exec(0, "NAME"));
  assert(!elf_ended);
  assert(pr_debugged == -1);
  assert(line_no == 1);
}
void test_no_hdr() {
  elf_ended = false;
  pr_debugged = -1;
  line_no = 0;
  assert(!elf_is_exec(1, "NAME"));
  assert(elf_ended);
  assert(pr_debugged == 0);
  assert(line_no == 4);
}
void test_not_exec() {
  elf_ended = false;
  pr_debugged = -1;
  line_no = 0;
  assert(!elf_is_exec(2, "NAME"));
  assert(elf_ended);
  assert(pr_debugged == 0);
  assert(line_no == 4);
}
void test_ok() {
  elf_ended = false;
  pr_debugged = false;
  line_no = 0;
  assert(elf_is_exec(3, "NAME"));
  assert(elf_ended);
  assert(pr_debugged == 1);
  assert(line_no == 4);
}

int main() {
  test_no_elf();
  test_no_hdr();
  test_not_exec();
  test_ok();
}
