#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
struct {int coverage[10000];} TestCoverage;
void* SEQ_START_TOKEN = NULL;

int SEQ_ID = 134435;
int NET_ID = 433213;
int IPVS_ID = 987212;

typedef int loff_t;
struct seq_file {
  int id;
};
struct net {
  int id;
};

struct list_head {
  struct list_head* next;
  void* entry;
};
struct netns_ipvs {
  int id;
  struct list_head app_list;
};

struct ip_vs_app {
  struct ip_vs_app *app;
  struct list_head a_list;
  struct list_head incs_list;
};

struct ip_vs_app* FIRST;
struct ip_vs_app* SECOND;
struct ip_vs_app* THIRD;
struct list_head app_list;

int line_no;
struct net* seq_file_net(struct seq_file* seq) {
  assert(0 == line_no);
  line_no++;
  assert(seq->id == SEQ_ID);
  struct net* net = malloc(sizeof(struct net));
  net->id = NET_ID;
  return net;
};
struct netns_ipvs* net_ipvs(struct net* net) {
  assert(1 == line_no);
  line_no++;
  assert(net->id == NET_ID);
  struct netns_ipvs* ipvs = malloc(sizeof(struct netns_ipvs));
  ipvs->id = IPVS_ID;
  ipvs->app_list = app_list;
  return ipvs;
};
void* ip_vs_app_idx(struct netns_ipvs* ipvs, int zero) {
  assert(2 == line_no);
  line_no++;
  assert(ipvs->id == IPVS_ID);
  assert(zero == 0);
  return FIRST;
}

#define list_entry(e, type_t, a_list) e->entry
#define list_for_each_entry(inc, incs_list, a_list) return (incs_list)->entry;

#START OF SAMPLE
static void *ip_vs_app_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
  struct ip_vs_app *inc, *app;
  struct list_head *e;
  struct net *net = seq_file_net(seq);
  struct netns_ipvs *ipvs = net_ipvs(net);

  ++*pos;
  if (v == SEQ_START_TOKEN)
    return ip_vs_app_idx(ipvs, 0);

  inc = v;
  app = inc->app;

  if ((e = inc->a_list.next) != &app->incs_list)
    return list_entry(e, struct ip_vs_app, a_list);

  /* go on to next application */
  for (e = app->a_list.next; e != &ipvs->app_list; e = e->next) {
    app = list_entry(e, struct ip_vs_app, a_list);
    list_for_each_entry(inc, &app->incs_list, a_list) {
      return inc;
    }
  }
  return NULL;
}
#END OF SAMPLE

int main() {
  FIRST = malloc(sizeof(struct ip_vs_app));
  SECOND = malloc(sizeof(struct ip_vs_app));
  THIRD = malloc(sizeof(struct ip_vs_app));

  struct list_head l1;
  struct list_head l2;
  FIRST->a_list = l1;
  FIRST->a_list.entry = NULL;
  FIRST->a_list.next = &l2;
  FIRST->a_list.next->entry = SECOND;
  FIRST->a_list.next->next = NULL;
  struct list_head incs_list1;
  struct ip_vs_app first_app;
  FIRST->app = &first_app;
  FIRST->app->incs_list = incs_list1;

  // THIRD = SECOND->app->a_list.next->entry->incs_list.entry
  struct list_head second1;
  struct list_head second2;
  SECOND->a_list = second1;
  SECOND->a_list.entry = NULL;
  struct ip_vs_app second_app;
  struct ip_vs_app second_app2;
  struct list_head second3;
  struct list_head second4;
  struct list_head second5;
  SECOND->app = &second_app;
  SECOND->app->incs_list = second2;
  SECOND->a_list.next = &SECOND->app->incs_list;
  SECOND->a_list.next->entry = NULL;
  SECOND->a_list.next->next = NULL;
  SECOND->app->a_list = second3;
  SECOND->app->a_list.next = &second4;
  SECOND->app->a_list.next->entry = &second_app2;
  second_app2.incs_list = second5;
  second_app2.incs_list.entry = THIRD;

  struct list_head list3;
  THIRD->a_list = list3;
  struct list_head incs_list3;
  THIRD->a_list.next = &incs_list3;
  THIRD->a_list.entry = NULL;
  struct ip_vs_app third_app;
  struct list_head third_app_incs_list;
  THIRD->app = &third_app;
  THIRD->app->incs_list = third_app_incs_list;
  THIRD->app->incs_list.next = &app_list;

  struct seq_file seq;
  seq.id = SEQ_ID;
  void* v = NULL;
  loff_t pos = 0;

  line_no = 0;
  v = ip_vs_app_seq_next(&seq, v, &pos);
  assert(line_no == 3);
  assert(pos == 1);
  assert(v == FIRST);

  line_no = 0;
  v = ip_vs_app_seq_next(&seq, v, &pos);
  assert(line_no == 2);
  assert(pos == 2);
  assert(v == SECOND);

  line_no = 0;
  v = ip_vs_app_seq_next(&seq, v, &pos);
  assert(line_no == 2);
  assert(pos == 3);
  assert(v == THIRD);

  line_no = 0;
  v = ip_vs_app_seq_next(&seq, v, &pos);
  assert(line_no == 2);
  assert(pos == 4);
  assert(v == NULL);

  return 0;
}
