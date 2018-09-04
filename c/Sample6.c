#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
struct {int coverage[10000];} TestCoverage;
int MIC_DMA_ALIGN_SHIFT = 1;
int MIC_DMA_MEMCPY_LEN_SHIFT = 2;
int MIC_DMA_MEMCPY = 3;
int MIC_DMA_DESC_TYPE_SHIFT = 4;

typedef long long u64;
typedef u64 dma_addr_t;
struct mic_dma_desc {
  u64 qw0;
  u64 qw1;
};

#START OF SAMPLE
static inline void mic_dma_memcpy_desc(struct mic_dma_desc *desc,
    dma_addr_t src_phys, dma_addr_t dst_phys, u64 size)
{
  u64 qw0, qw1;

  qw0 = src_phys;
  qw0 |= (size >> MIC_DMA_ALIGN_SHIFT) << MIC_DMA_MEMCPY_LEN_SHIFT;
  qw1 = MIC_DMA_MEMCPY;
  qw1 <<= MIC_DMA_DESC_TYPE_SHIFT;
  qw1 |= dst_phys;
  desc->qw0 = qw0;
  desc->qw1 = qw1;
}
#END OF SAMPLE

int main() {
  struct mic_dma_desc desc;
  mic_dma_memcpy_desc(&desc, 100, 200, 10);
  assert(desc.qw0 == 116);
  assert(desc.qw1 == 248);
}
